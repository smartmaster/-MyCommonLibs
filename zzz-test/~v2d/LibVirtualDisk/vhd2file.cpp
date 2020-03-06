#include "stdafx.h"
#include "VHD2File.h"
#include "VHDHelper.h"
#include "CommonUtil.h"
#include "zlib.h"
#include <atlbase.h>
#include <atlstr.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "cmpr.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))

//====================================================
//dynamic compressed VHD file
//====================================================
CDynVHD2File::CDynVHD2File()
: m_wstrFilePath(L"")
, m_wstrIdxPath(L"")
, m_ullCurrLPos(0)
, m_dwDiskSectorSize(VHD_SECTOR_SIZE)
, m_openMode(GENERIC_READ)
, m_pCmprsedBuf(NULL)
, m_dwCmprsedBufSize(0)
, m_pCacheBuffer(NULL)
, m_dwCacheBufferSize(0)
, m_BlkIdxCached(-1)
{
    ZeroMemory(&m_stHeader, sizeof(m_stHeader));
	memset(&m_stVHDEncInfo,0,sizeof(m_stVHDEncInfo));
	m_bVHDEncInited = FALSE;
	m_bEncInitStatus = TRUE;
};

CDynVHD2File::~CDynVHD2File()
{
    if(m_pCmprsedBuf != NULL)
    {
        VirtualFree(m_pCmprsedBuf, 0, MEM_RELEASE);
        m_pCmprsedBuf = NULL;
    }

    if(m_pCacheBuffer != NULL)
    {
        VirtualFree(m_pCacheBuffer, 0, MEM_RELEASE);
        m_pCacheBuffer = NULL;
    }

    m_shIdx.Close();
    //::DeleteFileW(m_wstrIdxPath.c_str());

};

const wchar_t*    
CDynVHD2File::GetFilePath() const
{
    return m_wstrFilePath.c_str();
};

int    
CDynVHD2File::Release()
{
    delete this;
    return 0;
};

int 
CDynVHD2File::GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
                      /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap)
{
    if (liStartingSec.QuadPart < 0 || nBufLen < 0 || pBitmap == NULL)
        return E_INVALIDARG;

    int nRet = 0;

    //
    // 1) align with 8
    //
    __int64 llSecStart = chROUNDDOWN(liStartingSec.QuadPart, 8);
    __int64 llSecEnd = llSecStart + nBufLen*8;
    if (llSecEnd > (__int64)m_stHeader.DiskSize/VHD_SECTOR_SIZE)
        llSecEnd = (__int64)m_stHeader.DiskSize/VHD_SECTOR_SIZE;

    pBitmap->StartingSec.QuadPart = llSecStart;
    pBitmap->BitmapSize.QuadPart = llSecEnd - llSecStart;

    //
    // 2) loop get bitmap
    //
    __int64 llBlkIdx = 0;
    ST_SI_BAT sibat = {0};
    BYTE* p = pBitmap->Buffer;
    while (llSecStart < llSecEnd)
    {
        llBlkIdx = llSecStart / (m_stHeader.BlockSize/VHD_SECTOR_SIZE);

        nRet = GetSIBat(llBlkIdx, sibat);
        if (nRet)
        {
            AFLOG<<"VHD2::GetBitmap, GetSIBat failed, blkIdx = "<<llBlkIdx<<", rc = "<<nRet;
            break;
        }

        
        int nRemain = 0; // number of sectors remains in this block
        if (chROUNDUP(llSecStart, m_stHeader.BlockSize/VHD_SECTOR_SIZE) >= llSecEnd) // the last loop
            nRemain = (int)(llSecEnd - llSecStart);
        else
        {
            int nSecIdxInBlk = (int)(llSecStart % (m_stHeader.BlockSize / VHD_SECTOR_SIZE));
            nRemain = m_stHeader.BlockSize/VHD_SECTOR_SIZE - nSecIdxInBlk; 
        }

        if (sibat.SI == 0 && sibat.BAT != -1) // in current file
            memset(p, 0xFF, (nRemain+7)/8);
        else
            ZeroMemory(p, (nRemain+7)/8);

        
        llSecStart += nRemain;
        p += (nRemain+7)/8;
    }

    //
    // 3) if fail, reset the output parameter
    //
    if (nRet != 0)
    {
        pBitmap->BitmapSize.QuadPart = 0;
        pBitmap->StartingSec.QuadPart = 0;
    }

    return nRet;
}

int     
CDynVHD2File::QueryRange(const LOCAL_ALLOCATED_RANGE* pRangeToQueryInput, LOCAL_ALLOCATED_RANGE* pRange, int* pnRangeCnt)
{
    if (pRangeToQueryInput == NULL || pRange == NULL || pnRangeCnt == NULL)
        return E_VHD_COV_INVALID_ARG;

    //<<enhance robust 2010-04-15
    if (pRangeToQueryInput->offset < 0 || pRangeToQueryInput->len < 0)
    {
        //HALogWarning(L"VHD2::QueryRange, input negative range, range->offset = %I64d, len = %I64d",
          //  pRangeToQueryInput->offset, pRangeToQueryInput->len);
        return E_INVALIDARG;
    }

    LOCAL_ALLOCATED_RANGE stRange2Query = *pRangeToQueryInput;
    LOCAL_ALLOCATED_RANGE* pRangeToQuery = &stRange2Query;
    pRangeToQuery->offset = (pRangeToQuery->offset/VHD_SECTOR_SIZE) * VHD_SECTOR_SIZE;
    if (pRangeToQuery->len > (__int64)m_stHeader.DiskSize - pRangeToQuery->offset)
        pRangeToQuery->len = (__int64)m_stHeader.DiskSize - pRangeToQuery->offset;
    if (pRangeToQuery->len <= 0)
    {
        *pnRangeCnt = 0;
        return 0;
    }
    //>>

    int nRet = 0;

    __int64 blkStart = pRangeToQuery->offset / m_stHeader.BlockSize;
    __int64 blkEnd = (pRangeToQuery->offset + pRangeToQuery->len) / m_stHeader.BlockSize;
    int secStart = (int)(pRangeToQuery->offset % m_stHeader.BlockSize) / VHD_SECTOR_SIZE;
    int secEnd = m_stHeader.BlockSize / VHD_SECTOR_SIZE;
    int nRangeCnt = 0;
    ST_SI_BAT sibat = {0};
    bool bNewRange = false;
    
    //unsigned int nBlockCnt = m_stHeader.MaxTableEntries;
    //if (nBlockCnt > m_stHeader.llOriginalBATSize/sizeof(ST_SI_BAT))
    //    nBlockCnt = m_stHeader.llOriginalBATSize/sizeof(ST_SI_BAT);
    __int64 nBlockCnt = m_stHeader.llOriginalBATSize/sizeof(ST_SI_BAT);

    for (__int64 blkIdx = blkStart; blkIdx <= blkEnd && blkIdx < nBlockCnt; blkIdx++)
    {
        if (blkIdx != blkStart)
            secStart = 0;
        if (blkIdx == blkEnd)
            secEnd = (int)((pRangeToQuery->offset + pRangeToQuery->len) % m_stHeader.BlockSize) / VHD_SECTOR_SIZE;

        nRet = GetSIBat(blkIdx, sibat);
        if (nRet)
        {
            AFLOG<<"VHD2::QueryRange, GetSIBat failed, blkIdx = "<<blkIdx<<", rc = "<<nRet;
            break;
        }

        if (bNewRange == false && sibat.BAT != -1 && sibat.SI == 0)
        {
            bNewRange = true;
            if (nRangeCnt < *pnRangeCnt)
            {
                pRange[nRangeCnt].offset = blkIdx*m_stHeader.BlockSize + secStart*VHD_SECTOR_SIZE;
            }
            else
            {
                //HALogWarning("VHD2::Need more data");
                return E_VHD_COV_RANGE_MOREDATA;
            }
        }

        if (bNewRange == true 
            && (sibat.BAT == -1 || sibat.SI != 0))
        {
            bNewRange = false;
            pRange[nRangeCnt].len = blkIdx*m_stHeader.BlockSize;
            pRange[nRangeCnt].len -= pRange[nRangeCnt].offset;
            nRangeCnt++;
        }
    }

    //if has the last
    if (bNewRange)
    {
        //secEnd = ((pRangeToQuery->offset + pRangeToQuery->len) % m_stHeader.BlockSize) / VHD_SECTOR_SIZE;
        //pRange[nRangeCnt].len = blkIdx*m_stHeader.BlockSize + secEnd*VHD_SECTOR_SIZE;
        pRange[nRangeCnt].len = (pRangeToQuery->offset + pRangeToQuery->len) & ~(VHD_SECTOR_SIZE-1);
        pRange[nRangeCnt].len -= pRange[nRangeCnt].offset;
        nRangeCnt++;
    }

    *pnRangeCnt = nRangeCnt;

    return nRet;
};

int
CDynVHD2File::SetPos(__int64 offset)
{
//    int nRet = 0;

    if (offset % VHD_SECTOR_SIZE)
    {
        AFLOG<<"VHD2::Set pos:: invalid range or offset, offset = "<<offset;
        ATLASSERT(FALSE);
        return E_VHD_COV_INVALID_ARG;
    }

    m_ullCurrLPos = offset;

    return 0;
}

//
//Read the LocalAllocatedData
//return 0 success, others fail
//if return 0 and returned pRange->len == 0 && pRange->offset == -1, have read to the end of file.
//@llStartOffset - the logical offset from where to start read, if -1, read from current file's pointer
//@pRange - return the buffer range we read.
//@cbToRead - The bytes to read, should be VHD_SECTOR_SIZE aligned
//
int     
CDynVHD2File::Read(char* pBuf, unsigned long cbToRead, LOCAL_ALLOCATED_RANGE* pRange, __int64 llStartOffset)
{
    if (pRange == NULL) return E_VHD_COV_INVALID_ARG;

    pRange->offset = -1;
    pRange->len = 0;

    int nRet = 0;
    unsigned __int64 offset = (llStartOffset == -1) ? m_ullCurrLPos : llStartOffset;
    if (offset % VHD_SECTOR_SIZE || cbToRead % VHD_SECTOR_SIZE)
    {
        AFLOG<<"VHD2::Read:: invalid range or offset, bytestoread = "<<cbToRead<<", offset = "<<offset;
        return E_VHD_COV_INVALID_ARG;
    }

    //<<2010-04-15
    if (offset >= m_stHeader.DiskSize)
        return 0;
    if ((UINT64)cbToRead > m_stHeader.DiskSize -offset)
        cbToRead = (ULONG)(m_stHeader.DiskSize - offset);
    //>>

    __int64 blkIdx = offset / m_stHeader.BlockSize;
    unsigned __int32 secIdx = (UINT32)(offset % m_stHeader.BlockSize) / VHD_SECTOR_SIZE;
    bool bFirstBlock = true; //if the first block
    unsigned __int32 secRead = 0; //the sector have read
    ST_SI_BAT sibat = {0};
    //unsigned int nBlockCnt = m_stHeader.MaxTableEntries;
    //if (nBlockCnt > m_stHeader.llOriginalBATSize/sizeof(ST_SI_BAT))
    //    nBlockCnt = m_stHeader.llOriginalBATSize/sizeof(ST_SI_BAT);
    __int64 nBlockCnt = m_stHeader.llOriginalBATSize/sizeof(ST_SI_BAT);

    for ( ; secRead < cbToRead/VHD_SECTOR_SIZE && blkIdx<nBlockCnt; blkIdx++, bFirstBlock = false)
    {
        if (!bFirstBlock)
            secIdx = 0;
       
        nRet = GetSIBat(blkIdx, sibat);
        if (nRet) 
        {
            AFLOG<<"VHD2::Read::Get SIBAT failed, rc = "<<nRet;
            break;
        }

        if (sibat.BAT == -1 || sibat.SI != 0) //not in the current file
        {
            if (secRead == 0)
                continue;
            else
                break;
        }

        unsigned int secToRead = cbToRead/VHD_SECTOR_SIZE - secRead; //sector need to read

        if (secIdx == 0 && secToRead >= m_stHeader.BlockSize / VHD_SECTOR_SIZE) 
        {
            //read whole block
            nRet = ReadOneBlock(blkIdx, &sibat, (BYTE*)pBuf + secRead*VHD_SECTOR_SIZE, m_stHeader.BlockSize);
            if (nRet)
            {
                AFLOG<<"VHD2::Read::Read Block failed, rc = "<<nRet<<", blk = "<<blkIdx;
                break;
            }
            secRead += m_stHeader.BlockSize/VHD_SECTOR_SIZE;
        }
        else
        {
            if (blkIdx != m_BlkIdxCached) //read a new block to cache
            {
                nRet = ReadOneBlock(blkIdx, &sibat, m_pCacheBuffer, m_dwCacheBufferSize);
                if (nRet)
                {
                    AFLOG<<"VHD2::Read::Read Block failed2, rc = "<<nRet<<", blk = "<<blkIdx;
                    break;
                }
                m_BlkIdxCached = blkIdx;
            }

            if (secIdx + secToRead > m_stHeader.BlockSize / VHD_SECTOR_SIZE) 
            {
                //read from cache, secIdx to BlockSize/VHD_SECTOR_SIZE, then to (secIdx+secToRead)
                memcpy(pBuf + secRead*VHD_SECTOR_SIZE, m_pCacheBuffer+secIdx*VHD_SECTOR_SIZE, m_stHeader.BlockSize - secIdx*VHD_SECTOR_SIZE);
                secRead += (m_stHeader.BlockSize/VHD_SECTOR_SIZE - secIdx);
            }
            else
            {
                //read from cache, secIdx to secIdx+secToRead, break;
                memcpy(pBuf + secRead*VHD_SECTOR_SIZE, m_pCacheBuffer+secIdx*VHD_SECTOR_SIZE, secToRead*VHD_SECTOR_SIZE);
                secRead += secToRead;
            }
        }

        if (pRange->offset == -1) //update the start offset
            pRange->offset = blkIdx*m_stHeader.BlockSize + secIdx*VHD_SECTOR_SIZE;
    }

    pRange->len = MIN(secRead*VHD_SECTOR_SIZE, m_stHeader.DiskSize - pRange->offset);
    m_ullCurrLPos = pRange->offset + pRange->len;

    return nRet;
};

int
CDynVHD2File::Read(char* pBuf, unsigned long cbToRead, unsigned long* pcbRead, __int64 llStartOffset)
{
    int nRet = 0;

    if (pcbRead == NULL || pBuf == NULL)
        return E_INVALIDARG;

    *pcbRead = 0;
    if (llStartOffset == -1)
        llStartOffset = m_ullCurrLPos;
    if (llStartOffset % VHD_SECTOR_SIZE || llStartOffset % VHD_SECTOR_SIZE)
    {
        AFLOG<<"VHD2::Read2:: invalid range or offset, cb2Read = "<<cbToRead<<", offset = "<<llStartOffset;
        return E_VHD_COV_INVALID_ARG;
    }

    if (llStartOffset >= m_stHeader.DiskSize)
        return 0;

    if ((UINT64)cbToRead > m_stHeader.DiskSize - llStartOffset)
        cbToRead = (ULONG)(m_stHeader.DiskSize - llStartOffset);

    __int64 blkIdx = llStartOffset / m_stHeader.BlockSize;
    unsigned __int32 secIdx = (UINT32)(llStartOffset % m_stHeader.BlockSize) / VHD_SECTOR_SIZE;
    bool bFirstBlock = true; //if the first block
    unsigned __int32 secRead = 0; //the sector have read
    ST_SI_BAT sibat = {0};
    __int64 nBlockCnt = m_stHeader.llOriginalBATSize/sizeof(ST_SI_BAT);

    for ( ; secRead < cbToRead/VHD_SECTOR_SIZE && blkIdx<nBlockCnt; blkIdx++, bFirstBlock = false)
    {
        if (!bFirstBlock)
            secIdx = 0;
       
        nRet = GetSIBat(blkIdx, sibat);
        if (nRet) 
        {
            AFLOG<<"VHD2::Read::Get SIBAT failed, rc = "<<nRet;
            break;
        }

        unsigned int secToRead = cbToRead/VHD_SECTOR_SIZE - secRead; //sector need to read
        if (sibat.BAT == -1 || sibat.SI != 0) //not in the current file
        {
            if (secToRead > m_stHeader.BlockSize / VHD_SECTOR_SIZE - secIdx)
                secToRead = m_stHeader.BlockSize / VHD_SECTOR_SIZE - secIdx;
            ZeroMemory(pBuf + secRead*VHD_SECTOR_SIZE, secToRead*VHD_SECTOR_SIZE);
            secRead += secToRead;
        }
        else if (secIdx == 0 && secToRead >= m_stHeader.BlockSize / VHD_SECTOR_SIZE) 
        {
            //read whole block
            nRet = ReadOneBlock(blkIdx, &sibat, (BYTE*)pBuf + secRead*VHD_SECTOR_SIZE, m_stHeader.BlockSize);
            if (nRet)
            {
                AFLOG<<"VHD2::Read::Read Block failed, rc = "<<nRet<<", blk = "<<blkIdx;
                break;
            }
            secRead += m_stHeader.BlockSize/VHD_SECTOR_SIZE;
        }
        else
        {
            if (blkIdx != m_BlkIdxCached) //read a new block to cache
            {
                nRet = ReadOneBlock(blkIdx, &sibat, m_pCacheBuffer, m_dwCacheBufferSize);
                if (nRet)
                {
                    AFLOG<<"VHD2::Read::Read Block failed2, rc = "<<nRet<<", blk = %d"<<blkIdx;
                    break;
                }
                m_BlkIdxCached = blkIdx;
            }

            if (secIdx + secToRead > m_stHeader.BlockSize / VHD_SECTOR_SIZE) 
            {
                //read from cache, secIdx to BlockSize/VHD_SECTOR_SIZE, then to (secIdx+secToRead)
                memcpy(pBuf + secRead*VHD_SECTOR_SIZE, m_pCacheBuffer+secIdx*VHD_SECTOR_SIZE, m_stHeader.BlockSize - secIdx*VHD_SECTOR_SIZE);
                secRead += (m_stHeader.BlockSize/VHD_SECTOR_SIZE - secIdx);
            }
            else
            {
                //read from cache, secIdx to secIdx+secToRead, break;
                memcpy(pBuf + secRead*VHD_SECTOR_SIZE, m_pCacheBuffer+secIdx*VHD_SECTOR_SIZE, secToRead*VHD_SECTOR_SIZE);
                secRead += secToRead;
            }
        }
    } //for

    if (nRet == 0)
    {
        *pcbRead = MIN(secRead*VHD_SECTOR_SIZE, m_stHeader.DiskSize - llStartOffset);
        m_ullCurrLPos = llStartOffset + (*pcbRead);
    }

    return nRet;
}

int          
CDynVHD2File::GetDiskID(unsigned long& ulDiskID)
{
    static const wchar_t* MARK = L"disk";

    wstring str;
    std::size_t pos = m_wstrFilePath.rfind(MARK);
    if (pos == wstring::npos)
    {
        AFLOG<<"VHD2::Get disk ID failed, file = "<<m_wstrFilePath.c_str();
        return -1;
    }

    str = m_wstrFilePath.substr(pos);
    if (str.length() < wcslen(MARK) + 10)
    {
        AFLOG<<"VHD2::Get disk ID failed2, file = "<<m_wstrFilePath.c_str();
        return -1;
    }

    str = str.substr(wcslen(MARK), 10);
    __int64 llTemp = _wtoi64(str.c_str());
    ulDiskID = (unsigned long)((unsigned __int64)llTemp & 0xFFFFFFFF);

    return 0;
}

int          
CDynVHD2File::GetDiskGeometry(DISK_GEOMETRY& geometry)
{
    geometry = m_stHeader.DiskGeometry;
    return 0;
}

int          
CDynVHD2File::GetDiskSize(__int64& llSize)
{
    //llSize = m_stHeader.MaxTableEntries;
    //llSize *= m_stHeader.BlockSize;
    llSize = m_stHeader.DiskSize;

    return 0;
}

int                 
CDynVHD2File::GetGuid(GUID* pGuid)
{
    if (pGuid == NULL)
        return E_INVALIDARG;
    *pGuid = m_stHeader.UniqueID;
    return 0;
}

//
//if *ppPath == NULL, indicate the path does not exist
//
int          
CDynVHD2File::GetParentPath(VHD_PARENTPATH_TYPE type, wchar_t** ppwszPath)
{
    UNREFERENCED_PARAMETER(type);
    UNREFERENCED_PARAMETER(ppwszPath);
    return E_VHD_COV_GETPPATH_NO_PARENT;
}

//
//if pwszPath == NULL, remove the corresponding path
//
int          
CDynVHD2File::SetParentPath(VHD_PARENTPATH_TYPE type, const wchar_t* pwszPath)
{
    UNREFERENCED_PARAMETER(type);
    UNREFERENCED_PARAMETER(pwszPath);
    return E_VHD_COV_SETPPATH_NO_PARENT;
}
//
//@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
//
int          
CDynVHD2File::GetParentTimestamp(unsigned __int32& timestamp)
{
    UNREFERENCED_PARAMETER(timestamp);

    return E_VHD_COV_VHD2_NO_PTIME;
}

//
//@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
//
int          
CDynVHD2File::SetParentTimestamp(unsigned __int32 timestamp)
{
    UNREFERENCED_PARAMETER(timestamp);
    return E_VHD_COV_VHD2_NO_PTIME;
}

//
//Free buffer
//
void                
CDynVHD2File::FreeBuffer(void* p)
{
    delete[] p;
}

int 
CDynVHD2File::Init(const wchar_t* pwszPath, DWORD eMode,DWORD dwShareMode, DWORD dwCreateDisp,D2D_ENCRYPTION_INFO* pEncryptionInfo)
{
    AFLOG<<"Enter VHD2::Init, path = "<<pwszPath<<", create disp = "<<dwCreateDisp;

    ZeroMemory(&m_sibatCache, sizeof(m_sibatCache));
	//set encinfo
    if (pEncryptionInfo == NULL)
	{
		m_stVHDEncInfo.uiCryptoType = 0;//non-Enc session
		memset(&m_stVHDEncInfo,0,sizeof(D2D_ENCRYPTION_INFO));
	}
	else
	{
          memcpy_s(&m_stVHDEncInfo,sizeof(m_stVHDEncInfo),pEncryptionInfo,sizeof(D2D_ENCRYPTION_INFO));
	}	
    int nRet = 0;

    if (pwszPath == NULL)
        return E_VHD_COV_INVALID_ARG;

    m_wstrFilePath = pwszPath;
    m_openMode = eMode;
    DWORD dwAccess = eMode;

    do
    {
        //open file
        m_shVHD.Attach(CreateFile(pwszPath, 
            dwAccess, 
            dwShareMode/*FILE_SHARE_READ|FILE_SHARE_WRITE*/ ,
            NULL, 
            OPEN_EXISTING, 
            FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_SEQUENTIAL_SCAN*//*|FILE_FLAG_NO_BUFFERING*/, 
            NULL));
        if(m_shVHD == INVALID_HANDLE_VALUE)
        {
            //HALogError(L"VHD2::Init::OpenFile failed, rc = %d, path = %s, access = %d, share = %d",
            //    ::GetLastError(), pwszPath, dwAccess, dwShareMode);
            nRet = E_VHD_COV_CREATE_FILE;
            break;
        }

        //read header
        nRet = ReadFileHeader();
        if (nRet)
        {
            AFLOG<<"VHD2::Init::Read File Header failed, file = "<<pwszPath<<", rc = "<<nRet;
            break;
        }
        m_stVHDEncInfo.uiCryptoType = m_stHeader.Encryption.EncLibType;
		m_stVHDEncInfo.ucAlgorithmType = m_stHeader.Encryption.AlgoType;

        //read SI_BAT
        nRet = SplitSIBat();
        if (nRet)
        {
            AFLOG<<"VHD2::Init::Split SIBAT failed, file = "<<pwszPath<<", rc = "<<nRet;
            break;
        }
        nRet = InitEncrypt();
		if(VHD_RET_SUCCESSFULL != nRet)
		{
			m_bEncInitStatus = FALSE;
			InitCompress();
		}
		else 
		{
			 nRet = InitCompress();
		}	
        if (nRet)
        {
            AFLOG<<"VHD2::Init::Init compress failed, file = "<<pwszPath<<", rc = "<<nRet;
            break;
        }

    } while (0);



    return nRet;
}

int
CDynVHD2File::ReadFileHeader()
{
    int nRet = 0;

    do
    {
        //read out the header
        DWORD dwRead = 0;
        if (!ReadFile(m_shVHD, &m_stHeader, sizeof(m_stHeader), &dwRead, NULL))
        {
            nRet = E_VHD_COV_READ_HEADER;
            AFLOG<<"VHD2::ReadHeader, read failed, rc = "<<::GetLastError();
            break;
        }

        if (dwRead != sizeof(m_stHeader))
        {
            nRet = E_VHD_COV_READ_HEADER;
            AFLOG<<"VHD2::ReadHeader, read failed2, dwRead = "<<dwRead<<", rc = "<<::GetLastError(); 
            break;
        }

        if(m_stHeader.Magic != VDISK_MAGIC)
        {
            nRet = E_VHD_COV_ERROR_MAGIC;
            AFLOG<<"VHD2::ReadHeader, error magic, magic = "<<m_stHeader.Magic; 
            break;
        }

        //check checksum
        DWORD dwSum1 = m_stHeader.HdrChecksum;
        m_stHeader.HdrChecksum = 0;
        DWORD dwSum2 = VHDHelper::CalcChecksum(&m_stHeader, sizeof(m_stHeader));
        if(dwSum1 != dwSum2)
        {
            nRet = E_VHD_COV_CHECKSUM;
            AFLOG<<"VHD2::ReadHeader, error checksum";
            break;
        }

    } while (0);


    return nRet;
}

// The sibat compress changed on Jan, 19,2010, so here disable the old code
// Using new SplitSIBat routine to support it
#if 0
int CDynVHD2File::SplitSIBat()
{
    int nRet = 0;

    HANDLE hLv2 = INVALID_HANDLE_VALUE;
    HANDLE hLv1 = INVALID_HANDLE_VALUE;
    wstring strLv1;
    wstring strLv2;
    wstring strIdx;

    gzFile zFileLv2 = NULL;
    gzFile zFileLv1 = NULL;

    do
    {
        //Create idx file
        strIdx = m_wstrFilePath;
        strIdx += L".idx";

        m_shIdx.Attach(CreateFile(strIdx.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL/* | FILE_FLAG_DELETE_ON_CLOSE*/,
            NULL));
        if(m_shIdx == INVALID_HANDLE_VALUE)
        {
            nRet = E_VHD_COV_CREATE_IDXFILE;
            break;
        }

        CAutoVectorPtr<BYTE> spBuf;

        //1)read compressed BAT from VHD file

        LARGE_INTEGER liPos;
        liPos.QuadPart = m_stHeader.BATOffset;
        if(FALSE == SetFilePointerEx(m_shVHD, liPos, &liPos, FILE_BEGIN))
        {
            nRet = E_VHD_COV_FAILED;
            break;
        }

        strLv2 = m_wstrFilePath;
        strLv2 += L".idx.lv1.lv2";

        hLv2 = CreateFile(strLv2.c_str(),
            GENERIC_READ|GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if(hLv2 == INVALID_HANDLE_VALUE)
        {
            nRet = E_VHD_COV_FAILED;
            break;
        }

        DWORD dwBufSize = 1024*1024*8;
        spBuf.Attach(new BYTE[dwBufSize]);

        DWORD dwRead = 0;
        DWORD dwWrite = 0;
        while(TRUE)
        {
            if(FALSE == ReadFile(m_shVHD, spBuf.m_p, dwBufSize, &dwRead, NULL))
            {
                nRet = E_VHD_COV_FAILED;
                break;
            }
            if(FALSE == WriteFile(hLv2, spBuf.m_p, dwRead, &dwWrite, NULL))
            {
                nRet = E_VHD_COV_FAILED;
                break;
            }
            dwRead += dwRead;
            if(dwRead > m_stHeader.BATSize || dwRead == 0)
            {
                break;
            }
        }
        if(nRet != VHD_RET_SUCCESSFULL)
        {
            break;
        }
        SetFilePointer(hLv2, m_stHeader.BATSize, NULL, FILE_BEGIN);
        SetEndOfFile(hLv2);
        CloseHandle(hLv2);
        hLv2 = INVALID_HANDLE_VALUE;

        //2)decompress once

        int fdLv2; //file descriptor for lv2
        strLv1 = m_wstrFilePath;
        strLv1 += L".idx.lv1";
        hLv1 = CreateFile(strLv1.c_str(),
            GENERIC_READ|GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if(hLv1 == INVALID_HANDLE_VALUE)
        {
            nRet = E_VHD_COV_FAILED;
            break;
        }

        errno_t err = _wsopen_s( &fdLv2, strLv2.c_str(), _O_RDONLY | _O_BINARY, _SH_DENYNO, _S_IREAD);
        if (err != 0)
        {
            nRet = E_VHD_COV_FAILED;
            break;
        }

        zFileLv2 = gzdopen(fdLv2, "rb");
        if(zFileLv2 == NULL)
        {
            nRet = E_VHD_COV_FAILED;
            break;
        }

        while(TRUE)
        {
            int read = gzread(zFileLv2, spBuf.m_p, dwBufSize);
            if(read == 0)
            {
                break;
            }
            DWORD dw;
            if(FALSE == WriteFile(hLv1, spBuf.m_p, read, &dw, NULL))
            {
                nRet = E_VHD_COV_FAILED;
                break;
            }
        }
        if(nRet != VHD_RET_SUCCESSFULL)
        {
            break;
        }

        CloseHandle(hLv1);
        hLv1 = INVALID_HANDLE_VALUE;


        //3) decompress again
        int fdLv1;
        err = _wsopen_s( &fdLv1, strLv1.c_str(), _O_RDONLY | _O_BINARY, _SH_DENYNO, _S_IREAD);
        if (err != 0)
        {
            nRet = E_VHD_COV_FAILED;
            break;
        }

        zFileLv1 = gzdopen(fdLv1, "rb");
        if(zFileLv1 == NULL)
        {
            nRet = E_VHD_COV_FAILED;
            break;
        }

        while(TRUE)
        {
            int read = gzread(zFileLv1, spBuf.m_p, dwBufSize);
            if(read == 0)
            {
                break;
            }
            DWORD dw;
            if(FALSE == WriteFile(m_shIdx, spBuf.m_p, read, &dw, NULL))
            {
                nRet = E_VHD_COV_FAILED;
                break;
            }
        }
        if(nRet != VHD_RET_SUCCESSFULL)
        {
            break;
        }

    } while (0);

    if(zFileLv2 != NULL)
    {
        gzclose(zFileLv2);
    }	
    if(zFileLv1 != NULL)
    {
        gzclose(zFileLv1);
    }

    if(hLv1 != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hLv1);
    }
    if(hLv2 != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hLv2);
    }

    DeleteFile(strLv2.c_str());
    DeleteFile(strLv1.c_str());

    if(nRet != 0)
    {
        DeleteFile(strIdx.c_str());
        m_shIdx.Close();
    }
    

    return nRet;
}
#endif //if 0

#if 0
int CDynVHD2File::SplitSIBat()
{
    int nRet = 0;
    //CFileLock cLock(pDiskNode->strVHDFileName.c_str());
    //if( !cLock.IsAccessible() || !cLock.IsLocked() )
    //{
    //    return ERROR_ACCESS_DENIED;
    //}

    wstring strIdx;
    unsigned char *pReadingBuffer = NULL;
    unsigned char *pIndexBuf = NULL;

    do 
    {
        strIdx = m_wstrFilePath;
        strIdx += L".idx";

        m_shIdx.Attach(CreateFile(strIdx.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE/* | FILE_SHARE_DELETE*/,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL/* | FILE_FLAG_DELETE_ON_CLOSE*/,
            NULL));
        if(m_shIdx == INVALID_HANDLE_VALUE)
        {
            nRet = E_VHD_COV_CREATE_IDXFILE;
            HALogError(L"Can not create Read/Write idx file, path = %s, rc = 0x%08X", 
                        strIdx.c_str(), ::GetLastError());
            break;
        }

        DWORD dwTick1 = GetTickCount();
        HALogMsg(L"Begin to uncompress index file, compressed BAT size = %d", m_stHeader.BATSize);

        DWORD dwIndexBufSize = m_stHeader.OriginalBATSize;
        DWORD dwReadingBufSize = GetSizeWithBlockPadding(m_stHeader.BATSize, m_dwDiskSectorSize);
        pReadingBuffer = new unsigned char[dwReadingBufSize];
        pIndexBuf = new unsigned char[dwIndexBufSize];
        if(pReadingBuffer == NULL || pIndexBuf == NULL)
        {
            nRet = E_VHD_COV_NO_ENOUGH_MEM;
            HALogError(L"Allocate memory error, reading buffer size[%d], Index buffer size[%d]",
                dwReadingBufSize, dwIndexBufSize);
            break;
        }
        
        HALogMsg(L"Allocate memory succeed, reading buffer size[%d], Index buffer size[%d]",
                dwReadingBufSize, dwIndexBufSize);
        

        LARGE_INTEGER liPos;
        liPos.QuadPart = m_stHeader.BATOffset;
        if(FALSE == SetFilePointerEx(m_shVHD, liPos, &liPos, FILE_BEGIN))
        {
            nRet = GetLastError();
            HALogError(L"SetFilePointerEx to hdr.BATOffset failed, pos = %I64d, Error = 0x%08X", 
                        m_stHeader.BATOffset, nRet); 
            break;
        }
        DWORD dwRead = 0;
        if(FALSE == ReadFile(m_shVHD, pReadingBuffer, dwReadingBufSize, &dwRead, NULL))
        {
            nRet = GetLastError();
            HALogError(L"ReadFile compressed index from VHD file failed, Error = 0x%08X", nRet); 
            break;
        }
        if (dwRead < m_stHeader.BATSize)
        {
            nRet = E_VHD_COV_INTERNAL_ERROR;
            HALogError(L"Read compressed index from VHD file failed, size read = %d", dwRead);
            break;
        }

        nRet = uncompress(pIndexBuf, &dwIndexBufSize, pReadingBuffer, m_stHeader.BATSize/*dwReadingBufSize*/);
        if(Z_OK != nRet)
        {
            HALogError(L"uncompress index file failed, rc = %d",nRet);
            nRet = E_VHD_COV_INTERNAL_ERROR;
            break;
        }

        HALogMsg(L"uncompress index file succeed");

        ULARGE_INTEGER liFreeSpace = {0};
        CString strFolderName = m_wstrFilePath.c_str();
        strFolderName = strFolderName.Left(strFolderName.ReverseFind(L'\\') + 1);
        HALogMsg(L"Folder name = %s", strFolderName);
        if( FALSE == GetDiskFreeSpaceEx(strFolderName, &liFreeSpace, NULL, NULL) )
        {
            nRet = GetLastError();
            HALogError(L"GetDiskFreeSpaceEx Failed, Error Number = %d", nRet);
            break;
        }
        if(liFreeSpace.QuadPart <= dwIndexBufSize)
        {
            nRet = E_VHD_COV_FILE_SIZE_LIMITED;
            HALogError(L"There is no enough free space to preform split index file, current free \
                space [%I64d], required space [%d]", liFreeSpace.QuadPart, dwIndexBufSize);
            break;
        }
        
        HALogMsg(L"Disk free space is enough for index file, free = %I64d, required = %d",
                 liFreeSpace.QuadPart, dwIndexBufSize);
        
        //Write to Idx file
        DWORD dwWrite = 0;
        if(FALSE == WriteFile(m_shIdx, pIndexBuf, dwIndexBufSize, &dwWrite, NULL))
        {
            nRet = GetLastError();
            HALogError(L"Write index file failed, path = %s, rc = 0x%08X", strIdx.c_str(), nRet);
            break;
        }
        if(dwIndexBufSize != dwWrite)
        {
            HALogError(L"Write index file failed, data size is not equal, path = %s, bytes2Write = %d, \
                written = %d", strIdx.c_str(), dwIndexBufSize, dwWrite); 
            nRet = E_VHD_COV_INTERNAL_ERROR;
            break;
        }

        HALogMsg(L"End to uncompress index file, time cost = [%d ms]", GetTickCount() - dwTick1);

    } while (0);

    if( pReadingBuffer )
    {
        delete [] pReadingBuffer;
        pReadingBuffer = NULL;
    }
    if (pIndexBuf)
    {
        delete [] pIndexBuf;
        pIndexBuf = NULL;
    }

    if(nRet != VHD_RET_SUCCESSFULL)
    {
        DeleteFile(strIdx.c_str());
    }

    return nRet;
}
#endif // if 0

int CDynVHD2File::SplitSIBat()
{
    int nRet = 0;

	CFileLock cLock(m_wstrFilePath.c_str());
	if( !cLock.IsAccessible() || !cLock.IsLocked() )
	{
		return ERROR_ACCESS_DENIED;
	}
    wstring strIdx;

    do 
    {
        strIdx = m_wstrFilePath;
        strIdx += L".idx";

        if (FileExist(strIdx.c_str()))
        {
            m_shIdx.Attach(CreateFile(strIdx.c_str(),
                            GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL/* | FILE_FLAG_DELETE_ON_CLOSE*/,
                            NULL));
            if(m_shIdx == INVALID_HANDLE_VALUE)
            {
                nRet = ::GetLastError();
                AFLOG<<"VHD2::Idx exists, but can not open read idx file, path = "<<strIdx.c_str()<<", rc = "<<nRet;
                break;
            }
            TELOG(1)<<"VHD2::SplitSIBat, idx already exists";

            break;
        }

        m_shIdx.Attach(CreateFile(strIdx.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ/* | FILE_SHARE_DELETE*/,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL/* | FILE_FLAG_DELETE_ON_CLOSE*/,
            NULL));
        if(m_shIdx == INVALID_HANDLE_VALUE)
        {
            nRet = ::GetLastError();
            AFLOG<<"VHD2::Can not create Read/Write idx file, path = "<<strIdx.c_str()<<", rc = "<<nRet;
            break;
        }
		USHORT Inbuffer = COMPRESSION_FORMAT_DEFAULT;
	    DWORD ByteReturned = 0;
	    BOOL Ioret = DeviceIoControl(m_shIdx,FSCTL_SET_COMPRESSION,&Inbuffer,sizeof(USHORT),NULL,0,&ByteReturned,NULL);
        if(!Ioret)
	    {
            AFLOG<<"Can't set compress!";
	    }

        DWORD dwTick1 = GetTickCount();
//        HALogMsg(L"VHD2::SplitSIBat::Begin to uncompress index file, BAT size = %d, origin BAT size = %I64d", 
          //  m_stHeader.BATSize, m_stHeader.llOriginalBATSize);

        //
        // Check free space
        //
        ULARGE_INTEGER liFreeSpace = {0};
        wstring strFolderName = m_wstrFilePath;
        strFolderName = strFolderName.substr(0, strFolderName.rfind(L'\\'));
       // HALogMsg(L"VHD2::SplitSIBat::Folder name = %s", strFolderName.c_str());
        if( FALSE == GetDiskFreeSpaceEx(strFolderName.c_str(), &liFreeSpace, NULL, NULL) )
        {
            nRet = GetLastError();
            //HALogMsg(L"VHD2::SplitSIBat::GetDiskFreeSpaceEx Failed, rc = %d", nRet);
            break;
        }
        if(liFreeSpace.QuadPart <= m_stHeader.llOriginalBATSize)
        {
            nRet = E_VHD_COV_FILE_SIZE_LIMITED;
            AFLOG<<"VHD2::SplitSIBat::No enough free space to preform split, avail = "<<liFreeSpace.QuadPart<<", required = "<<m_stHeader.llOriginalBATSize;
            break;
        }
        else
        {
            //HALogMsg(L"VHD2::SplitSIBat::Disk free space is enough to split index, avail = %I64d, required = %I64d",
               // liFreeSpace.QuadPart, m_stHeader.llOriginalBATSize);
        }

        //
        // Begin to uncompress
        //
        LARGE_INTEGER liPos;
        liPos.QuadPart = m_stHeader.BATOffset;
        if(FALSE == SetFilePointerEx(m_shVHD, liPos, &liPos, FILE_BEGIN))
        {
            nRet = GetLastError();
            AFLOG<<"VHD2::SplitSIBat::setFilePointerEx to hdr.BATOffset failed, pos = "<<m_stHeader.BATOffset<<", Error = "<<nRet; 
            break;
        }

        __int64 unCompressedSize = 0;
        nRet = CMPR::UncompressTo(m_wstrFilePath.c_str(), m_shVHD, m_shIdx, &unCompressedSize);
        if(nRet != 0)
        {
            AFLOG<<"VHD2::SplitSIBat::Uncompress index failed, err code = "<<nRet;
            break;
        }
        if(unCompressedSize != m_stHeader.llOriginalBATSize)
        {
            AFLOG<<"VHD2::SplitSIBat::Uncompress idx, size not equal. uncompressed = "
				<<unCompressedSize<<", origin = "<<m_stHeader.llOriginalBATSize<<", idx = "<<strIdx.c_str();
            nRet = E_VHD_COV_INTERNAL_ERROR;
            break;
        }

        AFLOG<<"uncompress index file succeed";
        AFLOG<<"End to uncompress index file, time cost = "<<GetTickCount() - dwTick1;
    } while (0);

    if (nRet == 0)
    {
        //
        //set file pointer
        //
        LARGE_INTEGER liMove = {0};
        if (!SetFilePointerEx(m_shIdx, liMove, NULL, FILE_BEGIN))
        {
            nRet = ::GetLastError();
            AFLOG<<"VHD2::SplitSIBat::Set idx file pointer to header failed, rc = "<<nRet;
        }
        else
        {
            liMove.QuadPart = sizeof(DISK_HDR);
            if (!SetFilePointerEx(m_shIdx, liMove, NULL, FILE_BEGIN))
            {
                nRet = ::GetLastError();
                AFLOG<<"VHD2::SplitSIBat::Set VHD file pointer after DISK_HDR failed, rc = "<<nRet;
            }
        }
    }

    if(nRet != VHD_RET_SUCCESSFULL)
    {
        m_shIdx.Close();
        DeleteFile(strIdx.c_str());
    }
    else
        m_wstrIdxPath = strIdx;

    return nRet;
}

DWORD CDynVHD2File::GetReadingSize(DWORD dwSize)
{
    return GetSizeWithBlockPadding(dwSize, m_dwDiskSectorSize);
}

DWORD CDynVHD2File::GetSizeWithBlockPadding(DWORD sizeToPad, DWORD dwAlign)
{
    return (sizeToPad + dwAlign - 1) & (~(dwAlign-1));
}

int CDynVHD2File::GetSIBat(__int64 blkIdx, ST_SI_BAT& sibat)
{
    if (blkIdx >= m_sibatCache.llIdxBegin && blkIdx < m_sibatCache.llIdxBegin + m_sibatCache.dwCnt)
    {
        sibat = m_sibatCache.data[blkIdx - m_sibatCache.llIdxBegin];
        return 0;
    }

    int nRet = 0;
    DWORD dwRead = 0;
    OVERLAPPED overlap = {0};
    ULARGE_INTEGER li;

    __int64 nBlockCnt = m_stHeader.llOriginalBATSize/sizeof(ST_SI_BAT);
    if (nBlockCnt - blkIdx > MAX_SIBAT_COUNT)
        m_sibatCache.dwCnt = MAX_SIBAT_COUNT;
    else
        m_sibatCache.dwCnt = nBlockCnt - blkIdx;

    ZeroMemory(&sibat, sizeof(sibat));
    li.QuadPart = blkIdx * sizeof(ST_SI_BAT),
    overlap.Offset = li.LowPart;
    overlap.OffsetHigh = li.HighPart;

    if (ReadFile(m_shIdx, m_sibatCache.data, m_sibatCache.dwCnt * sizeof(ST_SI_BAT), &dwRead, &overlap))
    {
        m_sibatCache.llIdxBegin = blkIdx;
        sibat = m_sibatCache.data[0];
    }
    else
    {
        nRet = ::GetLastError();
        m_sibatCache.llIdxBegin = 0;
        m_sibatCache.dwCnt = 0;
    }

    
    return nRet;
}
//<< modify by code from MultipleThreadCompressor.cpp in AFStor 2010-02-10
#define SUBBLOCK_FLAG_COMPRESSED 0x01

typedef struct _SUBHDR
{
    ULONG ulSize;
    ULONG ulFlags;
}SUBHDR, *PSUBHDR;

static int D2dUncompress(void* pSrcBuf, ULONG ulSrcSize, void* pDstBuf, ULONG* pulDstSize)
{
    int nRet = 0;
    PSUBHDR pHDR = NULL;
    UCHAR* pCurSrc = (UCHAR*)pSrcBuf;
    UCHAR* pCurDst = (UCHAR*)pDstBuf;
    ULONG ulDstBufSize = *pulDstSize;
    *pulDstSize = 0;

    pHDR = (PSUBHDR)pCurSrc;
    ULONG ulUncompressedSize = ulDstBufSize;
    while( pHDR->ulSize )
    {
        ulUncompressedSize = ulDstBufSize;
        if( pHDR->ulFlags & SUBBLOCK_FLAG_COMPRESSED )
        {
            nRet = uncompress(pCurDst, &ulUncompressedSize, pCurSrc+sizeof(SUBHDR), pHDR->ulSize);
            if( Z_OK != nRet)
            {
                break;
            }
        }
        else
        {
            memcpy_s(pCurDst, ulUncompressedSize, pCurSrc+sizeof(SUBHDR), pHDR->ulSize);
            ulUncompressedSize = pHDR->ulSize;
        }

        pCurDst += ulUncompressedSize;
        (*pulDstSize) += ulUncompressedSize;
        pCurSrc += pHDR->ulSize + sizeof(SUBHDR);
        pHDR = (PSUBHDR)pCurSrc;
    }

    return nRet;
}// 2010-02-10>>

int CDynVHD2File::ReadOneBlock(__int64 blkIdx, const ST_SI_BAT* pSiBat, BYTE* pBuf, DWORD dwLen)
{
    int nRet = 0;

    if (pSiBat == NULL)
        return E_VHD_COV_INVALID_ARG;

    if (dwLen < m_stHeader.BlockSize)
        return E_VHD_COV_INVALID_ARG;

    if (pSiBat->BAT == -1 || pSiBat->SI != 0)
        return E_VHD_COV_UNUSED_BAT_ENTRY;


    //if(ulBatIndex == 0xFDF92)
    //{
    //    int a = 0;
    //}


    ULONGLONG ullOffset = (ULONGLONG)pSiBat->BAT * VHD_SECTOR_SIZE;

    do
    {
        DWORD dw;
        LARGE_INTEGER liPos;
        liPos.QuadPart = ullOffset;
        SetFilePointerEx(m_shVHD, liPos, NULL, FILE_BEGIN);

        unsigned char tempBuf[512];
        ReadFile(m_shVHD, tempBuf, sizeof(tempBuf), &dw, NULL);
        if(dw != sizeof(tempBuf))
        {
            nRet = ::GetLastError();
            break;
        }

        LP_CompressedBufferHeader pHeader = (LP_CompressedBufferHeader)tempBuf;
        DWORD dw1 = pHeader->CheckSum;
        pHeader->CheckSum = 0;
        DWORD dw2 = VHDHelper::CalcChecksum(pHeader, sizeof(ST_CompressedBufferHeader));
        if(dw1 != dw2)
        {
            AFLOG<<"VHD2::ReadOneBlock Compress header check sum error! file="<<m_wstrFilePath.c_str()<<", idx = "<<blkIdx<<", SI = "<<pSiBat->SI<<", BAT = "<<pSiBat->BAT; 
            nRet = E_VHD_COV_CHECKSUM;
            break;
        }

        liPos.QuadPart = -512;
        SetFilePointerEx(m_shVHD, liPos, NULL, FILE_CURRENT);

        DWORD dwReadingSize = GetReadingSize(pHeader->EncCompressedSize + sizeof(ST_CompressedBufferHeader));
        ATLASSERT(dwReadingSize != 0);
        if (dwReadingSize == 0)
        {
            AFLOG<<"Attention: ReadOneBlock GetReadingSize return 0";
        }

        ReadFile(m_shVHD, m_pCmprsedBuf, dwReadingSize, &dw, NULL);
        if(dw != dwReadingSize)
        {
            nRet = ::GetLastError();
            break;
        }


        if(m_pCmprsedBuf->_Header.Compressed != 0)
        {
            if(pBuf)
			{
				if(!mtce.DecryptUncompress(m_pCmprsedBuf->Buffer, m_pCmprsedBuf->_Header.EncCompressedSize, pBuf, &dwLen,m_pCmprsedBuf->_Header.Compressed
					))
				{
					nRet = Z_FILTERED;
					break;
				}
				//unret = uncompress(m_pCacheBuffer, &cacheSize, m_pCompressedBuffer->Buffer, m_pCompressedBuffer->_Header.CompressedSize);
			}

            if(dwLen != m_pCmprsedBuf->_Header.OriginalSize)
            {
                nRet = E_VHD_COV_FAILED;
                break;
            }
        }
        else
        {
            if (dwLen < m_pCmprsedBuf->_Header.OriginalSize)
            {
                ATLASSERT(FALSE);
                nRet = E_VHD_COV_NEED_MORE_INPUTBUF;
                AFLOG<<"VHD2::ReadOneBlock, Need more input buffer";
                break;
            }
            memcpy(pBuf, m_pCmprsedBuf->Buffer, m_pCmprsedBuf->_Header.OriginalSize);
        }

    } while (0);

    return nRet;
}

__int32 CDynVHD2File::InitEncrypt()
{
	__int32 retVal = VHD_RET_SUCCESSFULL;

	do 
	{
		//Init ENC
		if (!m_bVHDEncInited && m_stVHDEncInfo.uiCryptoType != 0)
		{

			unsigned __int64 MagicI = m_stHeader.Magic;// VDISK_MAGIC;
			unsigned __int32 DiskVersionI = m_stHeader.DiskVersion;// VHD_VERSION;
			wchar_t wcMagic[sizeof(MagicI)/sizeof(wchar_t) + 1] = {0};
			wchar_t wcDiskVersion[sizeof(DiskVersionI)/sizeof(wchar_t) + 1] = {0};

			memcpy_s(wcMagic,sizeof(wcMagic)*sizeof(wchar_t),
				&MagicI, sizeof(MagicI));

			memcpy_s(wcDiskVersion,sizeof(wcDiskVersion)*sizeof(wchar_t),
				&DiskVersionI, sizeof(DiskVersionI));


			wstring wsSalt = wcMagic;
			wsSalt += wcDiskVersion;

			if (mtce.InitEnc(&m_stVHDEncInfo,wsSalt))
			{
				m_bVHDEncInited = TRUE;
			}
			else
			{
				//
				AFLOG << __FUNCTION__ << "(" << __LINE__ << "):" << "Failed to Init encrypt proxy :" << m_stVHDEncInfo.uiCryptoType << m_stVHDEncInfo.ucAlgorithmType;
				retVal = VHD_RET_INVALID_ENCINFO;
				break;
			}


			if(!CheckEncInfo(&m_stHeader))
			{
				TELOG(1) << __FUNCTION__ << "(" << __LINE__ << ")" <<  "Check Enc Info Failed, VHD:" << m_wstrFilePath.c_str(); 
				retVal = VHD_RET_INVALID_ENCINFO;
				break;
			}

		}
	} while(0);

	return retVal;
}
BOOL CDynVHD2File::CheckEncInfo(PDISK_HDR pDiskHdr)
{
	BOOL bRes = true;

	if (pDiskHdr != NULL && pDiskHdr->Encryption.VerifyInfoValid && m_stVHDEncInfo.uiCryptoType != 0)
	{
		if (pDiskHdr->Encryption.EncLibType != m_stVHDEncInfo.uiCryptoType)
		{
			AFLOG << __FUNCTION__ << "(" << __LINE__ << "):" << "Invalid Enc Type: " << pDiskHdr->Encryption.EncLibType << "|" << m_stVHDEncInfo.uiCryptoType;
			bRes = false;
		}
		else if (pDiskHdr->Encryption.AlgoType != m_stVHDEncInfo.ucAlgorithmType)
		{
			AFLOG << __FUNCTION__ << "(" << __LINE__ << "):" << "Invalid Algorithm Type: " << pDiskHdr->Encryption.AlgoType << "|" << m_stVHDEncInfo.ucAlgorithmType;
			bRes = false;
		}
		else
		{
			ULONG ulDesLen = sizeof(pDiskHdr->Encryption.ValidationData);
			DISK_ENC_INFO stEncInfo;
			memset(stEncInfo.ValidationData,0,sizeof(stEncInfo.ValidationData));
			if(!mtce.RawDecrypt((void*)pDiskHdr->Encryption.ValidationData,ulDesLen,
				stEncInfo.ValidationData,&ulDesLen,m_stVHDEncInfo.uiCryptoType,m_stVHDEncInfo.ucAlgorithmType))
			{
				AFLOG << __FUNCTION__ << "(" << __LINE__ << "):" << "Decrypt validation data failed!";
				bRes = false;
			}
			else if (stEncInfo.CheckSum != 
				VHDHelper::CalcChecksum(stEncInfo.CheckOutData,sizeof(stEncInfo.CheckOutData)))
			{
				AFLOG << __FUNCTION__ << "(" << __LINE__ << "):" << "Enc validation data check sum error!";
				bRes = false;
			}
		}
	}

	return bRes;
}
int CDynVHD2File::InitCompress()
{
    wstring strDrive;
    std::size_t pos = m_wstrFilePath.find(L":");
    m_dwDiskSectorSize = 0;
    if(!mtce.InitCom(0))
	{
		 LOGGER<< __FUNCTION__ << "(" << __LINE__ << ")" <<  ":Init Compress failed, Error code:" << VHD_RET_INTERNAL_ERROR;
		return VHD_RET_INTERNAL_ERROR;
	}
    do 
    {
        if(pos == wstring::npos)
        {
            AFLOG<<"Init compress::can not parse volume from path "<<m_wstrFilePath.c_str()<<"\n";
            break;
        }
        strDrive = m_wstrFilePath.substr(0, pos + 1);
        strDrive += L"\\";
        wchar_t wszVolumeName[1024] = {0};
        if (!GetVolumeNameForVolumeMountPoint(strDrive.c_str(), wszVolumeName, _countof(wszVolumeName)))
        {
            printf("Init compress, get volume name failed, rc = %08X\n", ::GetLastError());
            break;
        }

        wchar_t pwszVolumeName[1024];
        wcsncpy_s(pwszVolumeName, wcslen(wszVolumeName)+1, wszVolumeName, _TRUNCATE);

        if(pwszVolumeName[wcslen(pwszVolumeName)-1] == '\\')
        {
            pwszVolumeName[wcslen(pwszVolumeName)-1] = '\0';
        }

        DWORD dwIoRead;
        CEnsureCloseHandle<INVALID_HANDLE_VALUE> shVolume;
        shVolume.Attach(CreateFile(pwszVolumeName, 
                                    GENERIC_READ|GENERIC_WRITE,
                                    FILE_SHARE_WRITE|FILE_SHARE_READ, 
                                    NULL, 
                                    OPEN_EXISTING, 
                                    FILE_ATTRIBUTE_DEVICE, 
                                    NULL));
        if (shVolume == INVALID_HANDLE_VALUE)
        {
            AFLOG<<"Init compress, create volume handle failed, rc = "<<::GetLastError();
            break;
        }

        NTFS_VOLUME_DATA_BUFFER nvdb;
        if(TRUE == DeviceIoControl(shVolume,
                                    FSCTL_GET_NTFS_VOLUME_DATA,
                                    NULL,
                                    0,
                                    &nvdb,
                                    sizeof(nvdb),
                                    &dwIoRead,
                                    NULL))
        {
            m_dwDiskSectorSize = nvdb.BytesPerSector;
        }
    } while(0);

    if(m_dwDiskSectorSize == 0)
    {
        m_dwDiskSectorSize = 512;
    }

    m_dwCacheBufferSize = m_stHeader.BlockSize;

    if (m_pCacheBuffer == NULL)
        m_pCacheBuffer = (BYTE*)::VirtualAlloc(NULL, m_dwCacheBufferSize, MEM_COMMIT, PAGE_READWRITE);
    if(m_pCacheBuffer == NULL)
    {
        AFLOG<<"VHD2::InitCompress, allocate memory failed, size = "<<m_dwCacheBufferSize;
        return E_VHD_COV_FAILED;
    }
    ZeroMemory(m_pCacheBuffer, m_dwCacheBufferSize);

    m_dwCmprsedBufSize = m_stHeader.BlockSize * 2;
    m_pCmprsedBuf = (LP_CompressedBuffer) VirtualAlloc(NULL, 
        m_dwCmprsedBufSize + sizeof(ST_CompressedBufferHeader), MEM_COMMIT, PAGE_READWRITE );// new unsigned char[m_dwCmprsedBufSize];
    if(m_pCmprsedBuf == NULL)
    {
        AFLOG<<"VHD2::InitCompress, allocate cmprsed memory failed, size = "<<m_dwCmprsedBufSize;
        return E_VHD_COV_FAILED;
    }
    ZeroMemory(m_pCmprsedBuf, m_dwCmprsedBufSize);

    return VHD_RET_SUCCESSFULL;
}



//===========================================================================
// Differential VHD file
//===========================================================================

CDiffVHD2File::CDiffVHD2File()
: CDynVHD2File()
{
};

CDiffVHD2File::~CDiffVHD2File()
{
};

//
//if *ppPath == NULL, indicate the path does not exist
//
int          
CDiffVHD2File::GetParentPath(VHD_PARENTPATH_TYPE type, wchar_t** ppwszPath)
{
    int nRet = 0;

    do
    {
        if (ppwszPath == NULL) 
        {
            nRet = E_VHD_COV_INVALID_ARG;
            break;
        }

        if (type == WIN_ABSOLUTE)
        {
            int nSize = _countof(m_stHeader.Parent.AbsoluteFilename) + 8;
            *ppwszPath = new wchar_t[nSize];
            if (*ppwszPath == NULL)
            {
                nRet = E_VHD_COV_NO_ENOUGH_MEM;
                break;
            }

            ZeroMemory(*ppwszPath, nSize*sizeof(wchar_t));
            memcpy(*ppwszPath, m_stHeader.Parent.AbsoluteFilename, sizeof(m_stHeader.Parent.AbsoluteFilename));
        }
        else if (type == WIN_RELATIVE)
        {
            int nSize = _countof(m_stHeader.Parent.RelatedFilename) + 8;
            *ppwszPath = new wchar_t[nSize];
            if (*ppwszPath == NULL)
            {
                nRet = E_VHD_COV_NO_ENOUGH_MEM;
                break;
            }

            ZeroMemory(*ppwszPath, nSize*sizeof(wchar_t));
            memcpy(*ppwszPath, m_stHeader.Parent.RelatedFilename, sizeof(m_stHeader.Parent.RelatedFilename));
        }
        else
        {
            nRet = E_VHD_COV_GETPPATH_INVALID_TYPE;
            break;
        }
    } while (0);

    return nRet;
}

//
//if pwszPath == NULL, remove the corresponding path
//
int          
CDiffVHD2File::SetParentPath(VHD_PARENTPATH_TYPE type, const wchar_t* pwszPath)
{
    int nRet = 0;

    do
    {
        if (type == WIN_ABSOLUTE)
        {
            if (pwszPath == NULL)
                ZeroMemory(m_stHeader.Parent.AbsoluteFilename, sizeof(m_stHeader.Parent.AbsoluteFilename));
            else
                wcsncpy_s(m_stHeader.Parent.AbsoluteFilename, _countof(m_stHeader.Parent.AbsoluteFilename), pwszPath, _TRUNCATE);
        }
        else if (type == WIN_RELATIVE)
        {
            if (pwszPath == NULL)
                ZeroMemory(m_stHeader.Parent.RelatedFilename, sizeof(m_stHeader.Parent.RelatedFilename));
            else
                wcsncpy_s(m_stHeader.Parent.RelatedFilename, _countof(m_stHeader.Parent.AbsoluteFilename), pwszPath, _TRUNCATE);
        }
        else
        {
            nRet = E_VHD_COV_GETPPATH_INVALID_TYPE;
            break;
        }

        m_stHeader.HdrChecksum = 0;
        m_stHeader.HdrChecksum = VHDHelper::CalcChecksum(&m_stHeader, sizeof(m_stHeader));

        LARGE_INTEGER li = {0};
        DWORD dwWritten = 0;

        nRet = WriteFileAndPreserveTimePos(m_shVHD, &m_stHeader, sizeof(m_stHeader), &dwWritten, &li);
        if (nRet)
        {
            AFLOG<<"DIffVHD2::set parent path:: update into file failed, rc = "<<nRet<<", offset = "<<li.QuadPart;
            ATLASSERT(FALSE);
            break;
        }
        else if (dwWritten != sizeof(m_stHeader))
        {
            nRet = E_VHD_COV_WRITEHEADER_FAILED;
            AFLOG<<"DIffVHD2::set parent path:: update header into file failed, dwWritten = "<<dwWritten<<", offset = "<<li.QuadPart;
            ATLASSERT(FALSE);
            break;
        }

    } while (0);

    return nRet;
}

//
//@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
//
int          
CDiffVHD2File::GetParentTimestamp(unsigned __int32& timestamp)
{
    UNREFERENCED_PARAMETER(timestamp);

    return E_VHD_COV_VHD2_NO_PTIME;
}

//
//@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
//
int          
CDiffVHD2File::SetParentTimestamp(unsigned __int32 timestamp)
{
    UNREFERENCED_PARAMETER(timestamp);

    return E_VHD_COV_VHD2_NO_PTIME;
}

//
//Free buffer
//
void                
CDiffVHD2File::FreeBuffer(void* p)
{
    delete[] p;
}
