#include "StdAfx.h"
#include "VHDFile.h"
#include "VHD2File.h"
#include "VHDHelper.h"
#include <atldef.h>
#include <atlbase.h>


//===========================================================================
// VHD Creator
//===========================================================================

//CVHDFile
int CVHDFileCreator::Open(const wchar_t* pwszPath, DWORD eMode, 
                          DWORD dwCreateDisp, D2D_ENCRYPTION_INFO* pEncryptionInfo,CDynVHDFile** ppPlainVHD)
{
    int nRet = 0;

    DWORD dwAccess = GENERIC_READ;
    CEnsureCloseHandle<INVALID_HANDLE_VALUE> shFile;

    if (eMode != GENERIC_READ || ppPlainVHD == NULL) 
        return E_INVALIDARG;

    *ppPlainVHD = NULL;

    shFile.Attach(CreateFileW(pwszPath,
        dwAccess,
        FILE_SHARE_READ|FILE_SHARE_WRITE,
        NULL,
        dwCreateDisp,
        FILE_ATTRIBUTE_NORMAL,
        NULL));
    if (shFile == INVALID_HANDLE_VALUE)
    {
        nRet = ::GetLastError();
        AFLOG<<"VHD Creator::Open Plain file failed, path = "<<pwszPath<<", rc = "<<nRet;
        return nRet;
    }

    //2)Read footer, first assume is normal VHD, if failed, then check if is compressed VHD
    VHD_TYPE diskType = VHD_Unknown;
    VHDFOOTER stFooter = {0};

    if( VHDHelper::ReadFooter(shFile, &stFooter) )
    {
        diskType = (VHD_TYPE)stFooter.DiskType;
    }

    if (diskType != VHD_Dynamic && diskType != VHD_Diff)
    {
        AFLOG<<"VHD Creator::Open Plain file, not plain file, path = "<<pwszPath<<", disk type = "<<diskType;
        return -1;
    }

    shFile.Close();

    CDynVHDFile* pVhd = NULL;

    if (diskType == VHD_Dynamic)
        pVhd = new CDynVHDFile();
    else if (diskType == VHD_Diff)
        pVhd = new CDiffVHDFile();

    if (!pVhd) 
    {
        AFLOG<<"VHD Creator::Open Plain file, create obj failed";
        return -1;
    }

    nRet = pVhd->Init(pwszPath, eMode, FILE_SHARE_READ|FILE_SHARE_WRITE, dwCreateDisp,pEncryptionInfo);
    if(nRet)
    {
        pVhd->Release();
        pVhd = NULL;
        AFLOG<<"VHD Creator::Init plain VHD failed, rc = "<<nRet;
    }

    *ppPlainVHD = pVhd;

    return nRet;
}

int CVHDFileCreator::Open(const wchar_t* pwszPath, DWORD eMode, 
                          DWORD dwCreateDisp, D2D_ENCRYPTION_INFO* pEncryptionInfo,CDynVHD2File** ppCmprsVHD)
{
    UNREFERENCED_PARAMETER(pwszPath);
    UNREFERENCED_PARAMETER(eMode);
    UNREFERENCED_PARAMETER(dwCreateDisp);
    UNREFERENCED_PARAMETER(ppCmprsVHD);
    return E_NOTIMPL;
}

int CVHDFileCreator::Open(const wchar_t* pwszPath, DWORD eMode, DWORD dwShareMode, 
                          DWORD dwCreateDisp, D2D_ENCRYPTION_INFO* pEncryptionInfo,IVHDFile** ppVhd)
{
    if (ppVhd == NULL || pwszPath == NULL || pwszPath[0] == L'\0')
        return E_INVALIDARG;

    *ppVhd = NULL;
    enum FILE_TYPE {UNKNOWN, NORMAL_VHD, COMPRESS_VHD};
    DWORD dwAccess = eMode;
    CEnsureCloseHandle<INVALID_HANDLE_VALUE> shFile;
    FILE_TYPE fileType = UNKNOWN;

    //if (eMode != MODE_READ) return NULL;

    shFile.Attach(CreateFileW(pwszPath,
        dwAccess,
        dwShareMode,
        NULL,
        dwCreateDisp,
        FILE_ATTRIBUTE_NORMAL,
        NULL));
    if (shFile == INVALID_HANDLE_VALUE)
    {
        DWORD dwErr = ::GetLastError();
        AFLOG<<"VHD Creator::Open file failed, path = "<<pwszPath<<", rc = "<<dwErr<<", shareMode = "<<dwShareMode<<", dwAccess = "<<dwAccess;
        return dwErr;
    }

    //2)Read footer, first assume is normal VHD, if failed, then check if is compressed VHD
    VHD_TYPE diskType = VHD_Unknown;
    
    do
    {
        VHDFOOTER stFooter = {0};
        if( VHDHelper::ReadFooter(shFile, &stFooter) )
        {
            diskType = (VHD_TYPE)stFooter.DiskType;
        }

        if (diskType == VHD_Dynamic || diskType == VHD_Diff)
        {
            fileType = NORMAL_VHD;
            break;
        }

        //check if is compressed VHD
        DISK_HDR diskHdr = {0};
        DWORD dwRead = 0;

        ::SetFilePointer(shFile, 0, NULL, FILE_BEGIN);
        if (!ReadFile(shFile, &diskHdr, sizeof(diskHdr), &dwRead, NULL))
        {
            AFLOG<<"CVHDFileCreator::Open, read file failed, rc = "<<::GetLastError()<<", file = "<<pwszPath;           
            break;
        }

        if (dwRead != sizeof(diskHdr))
            break;

        if (diskHdr.Magic != VDISK_MAGIC)
        {
			AFLOG<< "CVHDFileCreator::Open, invalid magic, magic = "<<diskHdr.Magic;
            break;
        }

        diskType = (VHD_TYPE)diskHdr.DiskType;
        if (diskType == VHD_Dynamic2 || diskType == VHD_Diff2)
        {
            // for compressed VHD, now there only has DiskType Dynamic2, 
            // so we use the parent path name to set
            wchar_t wszSpace[512] = {0};
            if ( memcmp(diskHdr.Parent.AbsoluteFilename, wszSpace, sizeof(wszSpace)) == 0
                && memcmp(diskHdr.Parent.RelatedFilename, wszSpace, sizeof(wszSpace)) == 0 )
                diskType = VHD_Dynamic2;
            else
                diskType = VHD_Diff2;

            fileType = COMPRESS_VHD;

            break;
        }
        
    } while (0);

    shFile.Close();


    DWORD dwRet = 0;
    if (fileType == NORMAL_VHD)
    {
        CDynVHDFile* pVhd = NULL;

        if (diskType == VHD_Dynamic)
            pVhd = new CDynVHDFile();
        else if (diskType == VHD_Diff)
            pVhd = new CDiffVHDFile();

        if (pVhd == NULL)
        {
            AFLOG<< "VHD Creator::New a plain VHD obj failed, disk type = "<<diskType;
            return -1;
        }

        dwRet = pVhd->Init(pwszPath, eMode, dwShareMode, dwCreateDisp,pEncryptionInfo);
        if(dwRet)
        {
            pVhd->Release();
            pVhd = NULL;
            AFLOG<< "VHD Creator::Init normal VHD failed, rc = "<<dwRet;
        }
        *ppVhd = pVhd;
        return dwRet;
    }
    else if (fileType == COMPRESS_VHD) 
    {
        // Now for compressed VHD, the disk type are all 0x80000000, 
        // so we always create CDiffVHD2File
        CDynVHD2File* pVhd = NULL;

        if (diskType == VHD_Dynamic2)
            pVhd = new CDynVHD2File();/*CDynVHD2File();*/
        else if (diskType == VHD_Diff2)
            pVhd = new CDiffVHD2File();

        dwRet = pVhd->Init(pwszPath, eMode, dwShareMode, dwCreateDisp,pEncryptionInfo);
        if(dwRet)
        {
            pVhd->Release();
            pVhd = NULL;
            AFLOG<< "VHD Creator::Init compressed VHD failed, rc = "<<dwRet;
        }
        *ppVhd = pVhd;
        return dwRet;
    }

    AFLOG<< "VHD Creator::Invalid file type, path = "<<pwszPath;

    return -1;
}


//===========================================================================
// Dynamic VHD file
//===========================================================================

//dynamic VHD file
CDynVHDFile::CDynVHDFile()
: m_wstrFilePath(L"")
, m_openMode(GENERIC_READ)
, m_pBAT(NULL)
, m_ulSectorsPerBlock(0)
, m_ulBlockBitmapSectorCount(0)
, m_ullCurrLPos(0)
, m_pbSecBitmap(NULL)
{
    ZeroMemory(&m_stFooter, sizeof(m_stFooter));
    ZeroMemory(&m_stHeader, sizeof(m_stHeader));
}

CDynVHDFile::~CDynVHDFile(void)
{
    if (m_pBAT)
    {
        VirtualFree(m_pBAT, 0, MEM_RELEASE);
        m_pBAT = NULL;
    }

    if (m_pbSecBitmap)
    {
        VirtualFree(m_pbSecBitmap, 0, MEM_RELEASE);
        m_pbSecBitmap = NULL;
    }
}

int
CDynVHDFile::Init(const wchar_t* pwszPath, DWORD eMode, DWORD dwShareMode, DWORD dwCreateDisp,D2D_ENCRYPTION_INFO* pEncryptionInfo)
{
    int nRet = 0;

    if (pwszPath == NULL)
        return E_VHD_COV_INVALID_ARG;
    
    m_wstrFilePath = pwszPath;
    m_openMode = eMode;

    //DWORD dwAccess = GENERIC_READ;
    //if (eMode |= MODE_WRITE) dwAccess |= GENERIC_WRITE;
    DWORD dwAccess = eMode;

    do
    {
        //1) Create File
        m_shVHD.Attach(CreateFileW(pwszPath,
                        dwAccess,
                        dwShareMode,
                        NULL,
                        dwCreateDisp,
                        FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING,
                        NULL));
        if (m_shVHD == INVALID_HANDLE_VALUE)
        {
            nRet = E_VHD_COV_CREATE_FILE;
            AFLOG<<"Normal VHD::Init, Create file handle failed, rc = "<<::GetLastError()<<", file = "<<pwszPath<<", shareMode = "<<dwShareMode;
            break;
        }
 
        //2)Read footer
        if( !VHDHelper::ReadFooter(m_shVHD, &m_stFooter) )
        {
            nRet = E_VHD_COV_READ_FOOTER;
             AFLOG<<"Normal VHD::Init, read footer failed";
            break;
        }

        if (m_stFooter.DiskType != this->GetVHDType())
        {
            nRet = E_VHD_COV_INVALID_TYPE;
            AFLOG<<"Normal VHD::Init, invalid disk type, type = "<<m_stFooter.DiskType;
            break;
        }

        LARGE_INTEGER li;
        li.QuadPart = m_stFooter.DataOffset;
        if( !SetFilePointerEx(m_shVHD, li, NULL, FILE_BEGIN) )
        {
            nRet = E_VHD_COV_FAILED;
            AFLOG<<"Normal VHD::Init, set file pointer failed2, pos = "<<li.QuadPart;
            break;
        }

        //3)read header
        DWORD dwRead = 0;
        ReadFile(m_shVHD, &m_stHeader, sizeof(m_stHeader), &dwRead, NULL);
        if( dwRead != sizeof(m_stHeader) )
        {
            nRet = E_VHD_COV_READ_HEADER;
            AFLOG<<"Normal VHD::Init, read header failed, rc = "<<::GetLastError()<<", read bytes = "<<dwRead;
            break;
        }

        VHDHelper::ConvHeader(&m_stHeader);
        UINT32 unChecksum = m_stHeader.Checksum;
        m_stHeader.Checksum = 0;
        if( unChecksum != VHDHelper::CalcChecksum(&m_stHeader, sizeof(m_stHeader)) )
        {
            nRet = E_VHD_COV_CHECKSUM;
            AFLOG<<"Normal VHD::Init, calc check sum failed";
            break;
        }
        // Here for D2D, the 
        // m_stHeader.MaxTableEntries != 
        // ROUNDUP(m_stFooter.CurrentSize, m_stHeader.BlockSize) / m_stHeader.BlockSize

        //4) read BAT
        unsigned __int32 nBATSize = m_stHeader.MaxTableEntries;

        LARGE_INTEGER liPos;
        liPos.QuadPart = m_stHeader.TableOffset;
        if( !SetFilePointerEx(m_shVHD, liPos, NULL, FILE_BEGIN) )
        {
            nRet = E_VHD_COV_FAILED;
            AFLOG<<"Normal VHD::Init, set file pointer failed, pos = "<<liPos.QuadPart;
            break;
        }

        // From VHD format specification, The BAT is always extended to a sector boundary.
        unsigned int nSize2Read = nBATSize*sizeof(__int32);
        nSize2Read = chROUNDUP(nSize2Read, VHD_SECTOR_SIZE);  
        m_pBAT = (unsigned __int32*)::VirtualAlloc(NULL, nSize2Read, MEM_COMMIT, PAGE_READWRITE);
        if( !m_pBAT )
        {
            nRet = E_VHD_COV_NO_ENOUGH_MEM;
            AFLOG<<"Normal VHD::Init, alloc BAT failed, not enough mem";
            break;
        }

        ReadFile(m_shVHD, m_pBAT, nSize2Read, &dwRead, NULL);
        if( dwRead != nSize2Read )
        {
            delete[] m_pBAT;
            nRet = E_VHD_COV_READBAT;
            AFLOG<<"Normal VHD::Init, read bat failed, dwRead = "<<dwRead<<", batsize = "<<nBATSize<<", rc = "<<::GetLastError(); 
            break;
        }

        for( unsigned __int32 i=0; i<m_stHeader.MaxTableEntries; i++ )
        {
            if( m_pBAT[i] != 0xffffffff )
                m_pBAT[i] = ntohl(m_pBAT[i]);
        }

        m_ulSectorsPerBlock = m_stHeader.BlockSize/VHD_SECTOR_SIZE; //sector per block
        m_ulBlockBitmapSectorCount = (m_ulSectorsPerBlock+7)/8;   //how many bytes
        m_ulBlockBitmapSectorCount = (m_ulBlockBitmapSectorCount+VHD_SECTOR_SIZE-1)/VHD_SECTOR_SIZE;   //how many sectors
        //allocate the sector bitmap
        if (m_pbSecBitmap == NULL)
        {
            m_pbSecBitmap = (unsigned char*)::VirtualAlloc(
                NULL, m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, MEM_COMMIT, PAGE_READWRITE);
            if (m_pbSecBitmap == NULL)
            {
                nRet = E_VHD_COV_NO_ENOUGH_MEM;
                AFLOG<<"Normal VHD::Init, Alloc bitmap failed, not enough memory";
                break;
            }
        }

    } while (0);

    if (nRet)
        AFLOG<<"Normal VHD::Init failed, rc = "<<nRet;

    return nRet;
}

const wchar_t*    
CDynVHDFile::GetFilePath() const
{
    return m_wstrFilePath.c_str();
}

int    
CDynVHDFile::Release()
{
    delete this;
    return 0;
}

int 
CDynVHDFile::GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
                        /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap)
{
    // 1) round up the starting sector with 8
    // 2) loop (position to the block, get it's bitmap)
    int nRet = 0;

    if (liStartingSec.QuadPart < 0 || nBufLen< 0 || pBitmap == NULL)
        return E_INVALIDARG;

    pBitmap->BitmapSize.QuadPart = 0;
    pBitmap->StartingSec.QuadPart = 0;
    if (nBufLen == 0)
        return 0;

    ATLASSERT(m_ulSectorsPerBlock % 8 == 0);

    if (m_shAttrVHD == INVALID_HANDLE_VALUE)
        m_shAttrVHD.Attach(CreateFileW(m_wstrFilePath.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ|FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL));
    if (m_shAttrVHD == INVALID_HANDLE_VALUE)
    {
        nRet = E_VHD_COV_CREATE_FILE;
        AFLOG<<"Normal VHD::GetBitmap, Create file handle failed, rc = "<<::GetLastError()<<", file = "<<m_wstrFilePath.c_str();
        return nRet;
    }

    //
    // 1) align with 8
    //
    __int64 llSecStart = chROUNDDOWN(liStartingSec.QuadPart, 8);
    __int64 llSecEnd = llSecStart + nBufLen*8;
    if (llSecEnd > (__int64)m_stFooter.CurrentSize/VHD_SECTOR_SIZE)
        llSecEnd = (__int64)m_stFooter.CurrentSize/VHD_SECTOR_SIZE;
    BYTE* p = pBitmap->Buffer;
    // block bitmap len in bytes, align with 512
    CAutoVectorPtr<char> spBlkBitmap(new char[m_ulBlockBitmapSectorCount * VHD_SECTOR_SIZE]);

    //
    // 2) Read Bitmap
    //
    __int64 blkStart = llSecStart/m_ulSectorsPerBlock;
    __int64 blkEnd = llSecEnd/m_ulSectorsPerBlock;
    int secStart = (int)(llSecStart % m_ulSectorsPerBlock);        // the start sector index in this block
    int secEnd = m_ulSectorsPerBlock;                       // the end sector index in this block

    for (__int64 blkIdx = blkStart; blkIdx <= blkEnd && blkIdx < m_stHeader.MaxTableEntries; blkIdx++)
    {
        if (blkIdx != blkStart)
            secStart = 0;
        if (blkIdx == blkEnd)
            secEnd = (int)(llSecEnd % m_ulSectorsPerBlock);

        int nLen = (secEnd-secStart+7)/8;
        if (nLen == 0)
            break;

        if (m_pBAT[blkIdx] == -1) //means 0
        {
            ZeroMemory(p, nLen);
            p += nLen;
            continue;
        }

        LARGE_INTEGER liPos;
        liPos.QuadPart = m_pBAT[blkIdx];
        liPos.QuadPart *= VHD_SECTOR_SIZE;

        DWORD dwRead = 0;
        OVERLAPPED overlap = {0};
        overlap.Offset = liPos.LowPart;
        overlap.OffsetHigh = liPos.HighPart;
        if (!::ReadFile(m_shAttrVHD, spBlkBitmap.m_p, 
                        m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, &dwRead, &overlap))
        {
            nRet = ::GetLastError();
            AFLOG<<"Normal VHD::GetBitmap, read file failed, rc = "<<nRet;
            break;
        }

        if (dwRead != m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE)
        {
            nRet = E_VHD_COV_READBITMAP;
            AFLOG<<"Normal VHD::GetBitmap, read bitmap failed, rc = "<<nRet;
            break;
        }

        memcpy(p, spBlkBitmap.m_p + secStart/8, nLen);
        p += nLen;
    }


    pBitmap->StartingSec.QuadPart = llSecStart;
    pBitmap->BitmapSize.QuadPart = llSecEnd - llSecStart;
    
    return nRet;
}

//
//ERROR_MORE_DATA
int     
CDynVHDFile::QueryRange(const LOCAL_ALLOCATED_RANGE* pRangeToQueryInput, 
                        LOCAL_ALLOCATED_RANGE* pRange, int* pnRangeCnt)
{
    if (pRangeToQueryInput == NULL || pRange == NULL || pnRangeCnt == NULL)
        return E_VHD_COV_INVALID_ARG;

    //<<enhance robust 2010-04-15
    if (pRangeToQueryInput->offset < 0 || pRangeToQueryInput->len < 0)
    {
        AFLOG<<"Normal VHD::query range, input negative range, range->offset = %I64d"<<pRangeToQueryInput->offset<<", len = %I64d"<<pRangeToQueryInput->len;
        return E_INVALIDARG;
    }

    LOCAL_ALLOCATED_RANGE stRange2Query = *pRangeToQueryInput;
    LOCAL_ALLOCATED_RANGE* pRangeToQuery = &stRange2Query;
    pRangeToQuery->offset = (pRangeToQuery->offset/VHD_SECTOR_SIZE) * VHD_SECTOR_SIZE;
    if (pRangeToQuery->len > (__int64)m_stFooter.CurrentSize - pRangeToQuery->offset)
        pRangeToQuery->len = (__int64)m_stFooter.CurrentSize - pRangeToQuery->offset;
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
    int secEnd = m_ulSectorsPerBlock;
    int nRangeCnt = 0;

    for (__int64 blkIdx = blkStart; blkIdx <= blkEnd && blkIdx < m_stHeader.MaxTableEntries; blkIdx++)
    {
        if (m_pBAT[blkIdx] == -1)
            continue;

        if (blkIdx != blkStart)
            secStart = 0;
        if (blkIdx == blkEnd)
            secEnd = (int)((pRangeToQuery->offset + pRangeToQuery->len) % m_stHeader.BlockSize) / VHD_SECTOR_SIZE;

        nRet = PositionBlock(blkIdx);
        if (nRet)
        {
            AFLOG<<"Normal VHD::query range, position block failed, blk = "<<blkIdx<<", rc = %d"<<nRet;
            break;
        }

        //find the Range
        bool bNewRange = false;
        for (int i = secStart; i < secEnd; i++)
        {
            if (!bNewRange && VHDHelper::GetBit(m_pbSecBitmap, i))
            {
                bNewRange = true;
                if (nRangeCnt < *pnRangeCnt)
                {
                    pRange[nRangeCnt].offset = blkIdx*m_stHeader.BlockSize + i*VHD_SECTOR_SIZE;
                }
                else
                {
                    return E_VHD_COV_RANGE_MOREDATA;
                }
            }

            if (bNewRange && !VHDHelper::GetBit(m_pbSecBitmap, i))
            {
                bNewRange = false;
                pRange[nRangeCnt].len = blkIdx*m_stHeader.BlockSize + i*VHD_SECTOR_SIZE;
                pRange[nRangeCnt].len -= pRange[nRangeCnt].offset;
                nRangeCnt++;
            }
        }

        //if has the last
        if (bNewRange)
        {
            pRange[nRangeCnt].len = blkIdx*m_stHeader.BlockSize + secEnd*VHD_SECTOR_SIZE;
            pRange[nRangeCnt].len -= pRange[nRangeCnt].offset;
            nRangeCnt++;
        }

    }

    *pnRangeCnt = nRangeCnt;

    return nRet;
}

int
CDynVHDFile::SetPos(__int64 llOffset)
{
    int nRet = 0;

    if (llOffset % VHD_SECTOR_SIZE)
    {
        AFLOG<<"Normal VHD::Set pos:: invalid range or offset, offset = "<<llOffset;
        return E_VHD_COV_INVALID_ARG;
    }

    unsigned __int64 blkIdx = llOffset / m_stHeader.BlockSize;
    nRet = PositionBlock(blkIdx);
    if (nRet)//position fail
    {
        ATLASSERT(FALSE);
        AFLOG<<"Normal VHD::Set pos failed, blkidx = "<<blkIdx<<", rc = "<<nRet;
        this->SetPos(m_ullCurrLPos);
        return nRet;
    }

    m_ullCurrLPos = llOffset;

    return 0;
}

int     
CDynVHDFile::Read(char* pBuf, unsigned long cbToRead, LOCAL_ALLOCATED_RANGE* pRange, __int64 llStartOffset)
{
    if (pRange == NULL) return E_VHD_COV_INVALID_ARG;

    pRange->offset = -1;
    pRange->len = 0;
    
    int nRet = 0;
    unsigned __int64 offset = (llStartOffset == -1) ? m_ullCurrLPos : llStartOffset;
    if (offset % VHD_SECTOR_SIZE || cbToRead % VHD_SECTOR_SIZE)
    {
        AFLOG<<"Normal VHD::Read:: invalid range or offset, bytestoread = "<<cbToRead<<", offset = "<<offset;
        return E_VHD_COV_INVALID_ARG;
    }

    //<<enhance robust 2010-04-15
    if (offset >= m_stFooter.CurrentSize)
        return 0;
    if ((UINT64)cbToRead > m_stFooter.CurrentSize-offset)
        cbToRead = (ULONG)(m_stFooter.CurrentSize - offset);
    //>>
    unsigned __int64 blkIdx = offset / m_stHeader.BlockSize;
    unsigned __int32 secIdx = (UINT)(offset % m_stHeader.BlockSize) / VHD_SECTOR_SIZE;
    unsigned __int32 secRead = 0;
    bool bFirstBlock = true; //if the first block

    for ( ; secRead < cbToRead/VHD_SECTOR_SIZE && blkIdx<m_stHeader.MaxTableEntries; blkIdx++, bFirstBlock = false)
    {
        if (m_pBAT[blkIdx] == -1)
        {
            if (secRead == 0)
                continue;
            else
                break;
        }

        nRet = PositionBlock(blkIdx);
        if (nRet)//position fail
        {
            ATLASSERT(FALSE);
            break;
        }

        if (!bFirstBlock) 
            secIdx = 0;

        //1)Find start sector to read
        int nStart = -1;
        if (secRead == 0)
        {
            for (unsigned __int32 i = secIdx; i<m_ulSectorsPerBlock; i++)
                if (VHDHelper::GetBit(m_pbSecBitmap, i))
                {
                    nStart = i;
                    break;
                }

            if (nStart == -1) //does not find 1 bitmap
                continue;
        }
        else
        {
            if (VHDHelper::GetBit(m_pbSecBitmap, secIdx))
                nStart = 0;
            else
                break;
        }

        //2)Find end sector to read
        unsigned long ToRead = (cbToRead / VHD_SECTOR_SIZE) - secRead;
        int nEnd = 0;

        for (nEnd = nStart; (ULONG)nEnd<m_ulSectorsPerBlock && (ULONG)nEnd<nStart+ToRead; nEnd++)
            if (!VHDHelper::GetBit(m_pbSecBitmap, nEnd))
                break;

        //Read to buf
        ToRead = (nEnd-nStart)*VHD_SECTOR_SIZE;
        //set file pointer
        LARGE_INTEGER liPos;
        liPos.QuadPart = nStart*VHD_SECTOR_SIZE - m_ullCurrLPos%m_stHeader.BlockSize;
        if (liPos.QuadPart)
        {
            ::SetFilePointerEx(m_shVHD, liPos, NULL, FILE_CURRENT);
            m_ullCurrLPos += liPos.QuadPart;
        }

        DWORD dwRead = 0;
        if (!::ReadFile(m_shVHD, pBuf+secRead*VHD_SECTOR_SIZE, ToRead, &dwRead, NULL))
        {
            nRet = ::GetLastError();
            AFLOG<<"Read file failed, rc = "<<nRet;
            break;
        }
        ATLASSERT(ToRead == dwRead);
        m_ullCurrLPos += dwRead; //update the record pointer

        secRead += ToRead / VHD_SECTOR_SIZE;

        if (pRange->offset == -1)
            pRange->offset = blkIdx*m_stHeader.BlockSize + nStart*VHD_SECTOR_SIZE;

        //if (secRead == cbToRead/VHD_SECTOR_SIZE || blkIdx>=m_stHeader.MaxTableEntries)
        //    break;
        if (nEnd != m_ulSectorsPerBlock) //if the 1 bitmap break during the block
            break;
    }

    pRange->len = secRead*VHD_SECTOR_SIZE;


    return nRet;
}

int
CDynVHDFile::Read(char* pBuf, unsigned long cbToRead, unsigned long* pcbRead, __int64 llStartOffset)
{
    int nRet = 0;

    if (pcbRead == NULL)
        return E_INVALIDARG;

    unsigned __int64 offset = (llStartOffset == -1) ? m_ullCurrLPos : llStartOffset;
    if (offset % VHD_SECTOR_SIZE || cbToRead % VHD_SECTOR_SIZE)
    {
        AFLOG<<"Normal VHD::Read2:: invalid range or offset, cb2Read = "<<cbToRead<<", offset = "<<offset;
        return E_VHD_COV_INVALID_ARG;
    }

    if (offset >= m_stFooter.CurrentSize)
        return 0;
    if ((UINT64)cbToRead > m_stFooter.CurrentSize-offset)
        cbToRead = (ULONG)(m_stFooter.CurrentSize - offset);

    //
    // Position block and read
    //
    char* p = pBuf;
    __int64 blkStart = offset/m_stHeader.BlockSize;
    __int64 blkEnd = (offset + cbToRead)/m_stHeader.BlockSize;
    int secStart = (int)((offset % m_stHeader.BlockSize) / VHD_SECTOR_SIZE);// the start sector index in this block
    int secEnd = m_ulSectorsPerBlock;                       // the end sector index in this block

    for (__int64 blkIdx = blkStart; blkIdx <= blkEnd && blkIdx < m_stHeader.MaxTableEntries; blkIdx++)
    {
        if (blkIdx != blkStart)
            secStart = 0;
        if (blkIdx == blkEnd)
            secEnd = (int)(((offset + cbToRead)%m_stHeader.BlockSize) / VHD_SECTOR_SIZE);

        int nLen = (secEnd-secStart)*VHD_SECTOR_SIZE;
        if (nLen == 0)
            break;

        if (m_pBAT[blkIdx] == -1) //means 0
        {
            ZeroMemory(p, nLen);
            p += nLen;
            continue;
        }

        LARGE_INTEGER liPos;
        liPos.QuadPart = m_pBAT[blkIdx];
        liPos.QuadPart *= VHD_SECTOR_SIZE;
        liPos.QuadPart += m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE;
        liPos.QuadPart += secStart*VHD_SECTOR_SIZE;

        DWORD dwRead = 0;
        OVERLAPPED overlap = {0};
        overlap.Offset = liPos.LowPart;
        overlap.OffsetHigh = liPos.HighPart;
        if (!::ReadFile(m_shVHD, p, nLen, &dwRead, &overlap))
        {
            nRet = ::GetLastError();
            AFLOG<<"Normal VHD::Read2, read file failed, rc = "<<nRet<<", pos = "<<liPos.QuadPart<<", len = "<<nLen;
            break;
        }

        if (dwRead != nLen)
        {
            nRet = E_VHD_COV_READ_DATA;
           //AFLOG<<"Normal VHD::Read2, read data failed, ToRead = %d, read = %d, rc = 0x%08X", 
              //  nLen, dwRead, ::GetLastError());
            break;
        }

        p += nLen;
    }

    if (nRet == 0)
    {
        *pcbRead = (unsigned long)(p - pBuf);
        m_ullCurrLPos += *pcbRead;
    }

    return nRet;
}

int 
CDynVHDFile::PositionBlock(unsigned __int64 nBlockIdx)
{
    if (nBlockIdx >= m_stHeader.MaxTableEntries)
        return -1;

    LARGE_INTEGER liPos;
    liPos.QuadPart = m_pBAT[nBlockIdx];
    if (liPos.QuadPart == -1)
        return E_VHD_COV_UNUSED_BAT_ENTRY;

    liPos.QuadPart *= VHD_SECTOR_SIZE;

    unsigned __int64 nCurrIdx = m_ullCurrLPos/m_stHeader.BlockSize;
    if (nBlockIdx == nCurrIdx && m_ullCurrLPos%m_stHeader.BlockSize)
        return 0;

    //update bitmap
    int nRet = 0;
    do
    {
        if (!::SetFilePointerEx(m_shVHD, liPos, NULL, FILE_BEGIN))
        {
            nRet = ::GetLastError();
            AFLOG<<"Normal VHD::Position block, set file pointer failed, rc = "<<nRet;
            break;
        }

        DWORD dwRead = 0;
        if (!::ReadFile(m_shVHD, m_pbSecBitmap, m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE, &dwRead, NULL))
        {
            nRet = ::GetLastError();
            AFLOG<<"Normal VHD::Position block, read file failed, rc = "<<nRet;
            break;
        }

        if (dwRead != m_ulBlockBitmapSectorCount*VHD_SECTOR_SIZE)
        {
            nRet = E_VHD_COV_READBITMAP;
            AFLOG<<"Normal VHD::Position block, read bitmap failed, rc = "<<nRet;
            break;
        }

        //ensure the m_ullCurrLPos synchronize with filePointer
        m_ullCurrLPos = nBlockIdx*m_stHeader.BlockSize;
    } while (0);

    if (nRet) //error, restore position
    {
        ::SetFilePointer(m_shVHD, 0, NULL, FILE_BEGIN);
        m_ullCurrLPos = 0;
    }

    return nRet;
}

int          
CDynVHDFile::GetFooter(VHDFOOTER& footer)
{
    footer = m_stFooter;

    return 0;
}

int          
CDynVHDFile::GetHeader(VHDDYNHEADER& header)
{
    header = m_stHeader;

    return 0;
}

int                 
CDynVHDFile::GetDiskGeometry(DISK_GEOMETRY& geometry)
{
    geometry.BytesPerSector = VHD_SECTOR_SIZE;
    geometry.Cylinders.QuadPart = m_stFooter.DiskGeometry.Cylinder;
    geometry.SectorsPerTrack = m_stFooter.DiskGeometry.SectorsPerTrack_Cylinder;
    geometry.TracksPerCylinder = m_stFooter.DiskGeometry.Heads;
    geometry.MediaType = FixedMedia;

    return 0;
}

int          
CDynVHDFile::GetDiskID(unsigned long& ulDiskID)
{
    //wstring m_wstrFilePath = L"E:\\D2D_DES\\ARC\\VStore\\S0000000002\\disk3978999266_155ADDC1-7061-4C4C-9CEB-9E737566D91F.D2D";
    static const wchar_t* MARK = L"disk";

    wstring str;
    std::size_t pos = m_wstrFilePath.rfind(MARK);
    if (pos == wstring::npos)
    {
        AFLOG<<"Normal VHD::Get disk ID failed, file = "<<m_wstrFilePath.c_str();
        return -1;
    }

    str = m_wstrFilePath.substr(pos);
    if (str.length() < wcslen(MARK) + 10)
    {
        AFLOG<<"Normal VHD::Get disk ID failed2, file = "<<m_wstrFilePath.c_str();
        return -1;
    }

    str = str.substr(wcslen(MARK), 10);
    __int64 llTemp = _wtoi64(str.c_str());
    ulDiskID = (unsigned long)((unsigned __int64)llTemp & 0xFFFFFFFF);

    return 0;
}

int
CDynVHDFile::GetDiskSize(__int64& llSize)
{
    // Here I suggest it's better to use CurrentSize
    // llSize = m_stFooter.OriginalSize;
    llSize = m_stFooter.CurrentSize;
    return 0;
}

//
//if *ppPath == NULL, indicate the path does not exist
//
int                 
CDynVHDFile::GetParentPath(VHD_PARENTPATH_TYPE type, wchar_t** ppwszPath)
{
    return E_VHD_COV_GETPPATH_NO_PARENT;
}

//
//if pwszPath == NULL, remove the corresponding path
//
int                 
CDynVHDFile::SetParentPath(VHD_PARENTPATH_TYPE type, const wchar_t* pwszPath)
{
    return E_VHD_COV_SETPPATH_NO_PARENT;
}

//
//@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
//
int          
CDynVHDFile::GetParentTimestamp(unsigned __int32& timestamp)
{
    UNREFERENCED_PARAMETER(timestamp);

    return E_VHD_COV_GETPTIME_NO_PARENT;
}

//
//@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
//
int                 
CDynVHDFile::SetParentTimestamp(unsigned __int32 timestamp)
{
    UNREFERENCED_PARAMETER(timestamp);

    return E_VHD_COV_SETPTIME_NO_PARENT;
}

void                
CDynVHDFile::FreeBuffer(void* p)
{
    VirtualFree(p, 0, MEM_RELEASE);
}

int          
CDynVHDFile::SetCreatorCookie(const char* pszCreator)
{
    if (pszCreator == NULL) return E_VHD_COV_INVALID_ARG;

    int nRet = 0;

    //update the cookie and checksum in footer
    size_t nSize = strlen(pszCreator);
    if (nSize > sizeof(m_stFooter.Cookie))
        nSize = sizeof(m_stFooter.Cookie);
    ZeroMemory(m_stFooter.Cookie, sizeof(m_stFooter.Cookie));
    memcpy_s(m_stFooter.Cookie, sizeof(m_stFooter.Cookie), pszCreator, nSize);
    m_stFooter.Checksum = 0;
    m_stFooter.Checksum = VHDHelper::CalcChecksum(&m_stFooter, sizeof(m_stFooter));

    VHDFOOTER stFooter = m_stFooter;
    VHDHelper::ConvFooter(&stFooter);

    do
    {
        DWORD dwWritten;
        LARGE_INTEGER liOffset;
        liOffset.QuadPart = 0;

        nRet = WriteFileAndPreserveTimePos(m_shVHD, &stFooter, sizeof(stFooter), &dwWritten, &liOffset);
        if (nRet)
        {
            AFLOG<<"NormalVHD::set creator cookie:: failed to update creator to file, rc = "<<nRet;
        }
        else if (dwWritten != sizeof(m_stFooter))
        {
            AFLOG<<"NormalVHD::set creator cookie::to write "<<sizeof(stFooter)<<" bytes, only write "<<dwWritten<<" bytes";
            nRet = E_VHD_COV_WRITEFOOTER_FAILED;
        }

        LARGE_INTEGER liPos;
        liPos.QuadPart = -512;
        if( !SetFilePointerEx(m_shVHD, liPos, &liOffset, FILE_END) )
        {
            nRet = ::GetLastError();
            AFLOG<<"NormalVHD::set creator cookie::position trailer footer failed, rc = "<<nRet;
            break;
        }

        nRet = WriteFileAndPreserveTimePos(m_shVHD, &stFooter, sizeof(stFooter), &dwWritten, &liOffset);
        if (nRet)
        {
            AFLOG<<"NormalVHD::set creator cookie2:: failed to update creator to file, rc = "<<nRet;
        }
        else if (dwWritten != sizeof(m_stFooter))
        {
            AFLOG<<"NormalVHD::set creator cookie2::to write "<< sizeof(stFooter)<<" bytes, only write "<<dwWritten<<" bytes";
            nRet = E_VHD_COV_WRITEFOOTER_FAILED;
        }

    } while (0);


    return nRet;
}

int                 
CDynVHDFile::GetGuid(GUID* pGuid)
{
    if (pGuid == NULL)
        return E_INVALIDARG;
    *pGuid = m_stFooter.UniqueId;
    return 0;
}

//===========================================================================
// Differential VHD file
//===========================================================================

CDiffVHDFile::CDiffVHDFile()
: CDynVHDFile()
{
};

CDiffVHDFile::~CDiffVHDFile()
{
};

//
//if *ppPath == NULL, indicate the path does not exist
//
int                 
CDiffVHDFile::GetParentPath(VHD_PARENTPATH_TYPE type, wchar_t** ppwszPath)
{
    if (type != WIN_RELATIVE && type != WIN_ABSOLUTE)
        return E_VHD_COV_GETPPATH_INVALID_TYPE;
    if (ppwszPath == NULL)
        return E_VHD_COV_INVALID_ARG;
    
    __int32 nPlatformCode = (type == WIN_RELATIVE) ? CPlatformCode::W2ru : CPlatformCode::W2ku;
    VHDPARENTLOCATOR* lo = NULL;

    for (int i = 0; i<_countof(m_stHeader.ParentLocators); i++)
    {
        if (m_stHeader.ParentLocators[i].PlatformCode == (UINT)nPlatformCode)
        {
            lo = &m_stHeader.ParentLocators[i];
            break;
        }
    }

    int nRet = 0;

    do
    {
        if (lo == NULL)
        {
            nRet = E_VHD_COV_GETPPATH_NOTEXIST;
            AFLOG<<"DiffVHD::get parent path, cannot find the locator, type = "<<type;
            break;
        }

        unsigned int nLen = (lo->PlatformDataLength + VHD_SECTOR_SIZE - 1) & ~(VHD_SECTOR_SIZE - 1); 
        if (nLen > lo->PlatformDataSpace) //from the VHD generated by HyperV we can found PlatformDataSpace actually is in bytes, not in sectors
        {
            //HALogError(L"DIffVHD::get parent path, capacity failed, length = %d, data space = %d sector", 
                                        //lo->PlatformDataLength, lo->PlatformDataSpace);
            ATLASSERT(FALSE);
            nRet = E_VHD_COV_FAILED;
            break;
        }

        *ppwszPath = (wchar_t*)::VirtualAlloc(NULL, nLen, MEM_COMMIT, PAGE_READWRITE);
        if (*ppwszPath == NULL)
        {
            nRet = E_VHD_COV_NO_ENOUGH_MEM;
            AFLOG<<"DIffVHD::get parent path, out of memory, len = "<<nLen<<" bytes";
            break;
        }

        LARGE_INTEGER liPos = {0};
        LARGE_INTEGER liCur = {0};
        DWORD dwRead = 0;

        ::SetFilePointerEx(m_shVHD, liPos, &liCur, FILE_CURRENT); //get current pos
        liPos.QuadPart = lo->PlatformDataOffset;
        if (!::SetFilePointerEx(m_shVHD, liPos, NULL, FILE_BEGIN))
        {
            nRet = E_VHD_COV_SETFILEPOS;
            AFLOG<<"DIffVHD::get parent path, set file offset failed, offset = "<<lo->PlatformDataOffset;
            break;
        }
        
        if (!ReadFile(m_shVHD, *ppwszPath, nLen, &dwRead, NULL))
        {
            nRet = ::GetLastError();
            AFLOG<<"DIffVHD::get parent path, read path failed, rc = "<<nRet;
            break;
        }

        if (!::SetFilePointerEx(m_shVHD, liCur, NULL, FILE_BEGIN))
        {
            nRet = ::GetLastError();
            AFLOG<<"SetFilePointer failed, pos = "<<liCur.QuadPart<<", rc = nRet"<<nRet;
            break;
        }

    } while (0);

    return nRet;
}

static BOOL ParentLocatorTableIsEmpty(VHDDYNHEADER& header)
{
    for (int i = 0; i<_countof(header.ParentLocators); i++)
    {
        if (header.ParentLocators[i].PlatformCode != CPlatformCode::None)
        {
            return FALSE;
        }
    }

    return TRUE;
}

//
//if pwszPath == NULL, remove the corresponding path
//
int                 
CDiffVHDFile::SetParentPath(VHD_PARENTPATH_TYPE type, const wchar_t* pwszPath)
{
    if (type != WIN_ABSOLUTE && type != WIN_RELATIVE)
    {
        return  E_VHD_COV_GETPPATH_INVALID_TYPE;
    }

    int nRet = 0;
    BYTE* pAlignedBuf = NULL; //buffer aligned with sector size
    __int32 nPlatformCode = (type == WIN_RELATIVE) ? CPlatformCode::W2ru : CPlatformCode::W2ku;
    VHDPARENTLOCATOR* lo = NULL;

    for (int i = 0; i<_countof(m_stHeader.ParentLocators); i++)
    {
        if (m_stHeader.ParentLocators[i].PlatformCode == (UINT)nPlatformCode)
        {
            lo = &m_stHeader.ParentLocators[i];
            break;
        }
    }

    //Remove the ANSI path, because it has been deprecated
    for (size_t i = 0; i<_countof(m_stHeader.ParentLocators); i++)
    {
        VHDPARENTLOCATOR* loc = &m_stHeader.ParentLocators[i];
        if (loc->PlatformCode == CPlatformCode::Wi2k
            || loc->PlatformCode == CPlatformCode::Wi2r)
        {
            loc->PlatformCode = CPlatformCode::None;
            loc->PlatformDataLength = 0;
        }
    }

    do
    {   
        //1)if is remove, just clear the specified entry in the locator table, then update the header
        if (pwszPath == NULL)
        {
            if (lo != NULL)
            {
                lo->PlatformCode = CPlatformCode::None;
                lo->PlatformDataLength = 0;

                //if none entry now, clear the parent name
                if (ParentLocatorTableIsEmpty(m_stHeader))
                {
                    ZeroMemory(m_stHeader.ParentUnicodeName, sizeof(m_stHeader.ParentUnicodeName));
                    //HALogMsg(L"DIffVHD::set parent path::no parent Locator table, clear the parent name");
                }
            }
            break;
        }

        //2)not remove, check if the path already exist
        // if does not exist, use a None Entry
        if (lo == NULL)
        {
            for (int i = 0; i<_countof(m_stHeader.ParentLocators); i++)
            {
                if (m_stHeader.ParentLocators[i].PlatformCode == CPlatformCode::None)
                {
                    lo = &m_stHeader.ParentLocators[i];
                    break; //break for
                }
            }

            if (lo == NULL)
            {
                ATLVERIFY(FALSE);
                lo = &m_stHeader.ParentLocators[_countof(m_stHeader.ParentLocators) - 1];
            }
        }

        // If space is enough, update the path, else add some sector into file, 
        // put path in those sector, and move the header to the end of file.
        unsigned int nLen = (UINT)(wcslen(pwszPath) + 1)*sizeof(wchar_t);
        
        nLen = (nLen + VHD_SECTOR_SIZE - 1) & ~(VHD_SECTOR_SIZE - 1); //align with sector size

        pAlignedBuf = (BYTE*)::VirtualAlloc(0, nLen, MEM_COMMIT, PAGE_READWRITE);
        if (pAlignedBuf == NULL)
        {
            nRet = E_VHD_COV_NO_ENOUGH_MEM;
            AFLOG<<"DIffVHD::set parent path::not enough memory, len = "<<nLen;
            break;
        }

        ZeroMemory(pAlignedBuf, nLen);
        memcpy(pAlignedBuf, pwszPath, wcslen(pwszPath)*sizeof(wchar_t));

        DWORD dwWritten = 0;
        LARGE_INTEGER li = {0};
        bool bAllocNewSpace = false;

        if (lo->PlatformDataSpace >= nLen)
        {
            li.QuadPart = lo->PlatformDataOffset;
        }
        else //here we need to alloc some space in the file, and write the path
        {
            printf("DIffVHD::set parent path::space is not enough to occupy the input path, len = %d, space = %d\n",
                nLen, lo->PlatformDataSpace);
            AFLOG<<"DIffVHD::set parent path::space is not enough to occupy the input path, len = "<<nLen<<", space = "<<lo->PlatformDataSpace; 

            bAllocNewSpace = true;
            LARGE_INTEGER liFileSize  = {0};

            ::GetFileSizeEx(m_shVHD, &liFileSize);
            li.QuadPart = liFileSize.QuadPart - sizeof(VHDFOOTER);
            ATLASSERT(li.QuadPart % VHD_SECTOR_SIZE == 0);

            //update the lo.offset and lo.dataspace
            lo->PlatformDataOffset = li.QuadPart;
            lo->PlatformDataSpace = nLen;
        }

        nRet = WriteFileAndPreserveTimePos(m_shVHD, pAlignedBuf, nLen, &dwWritten, &li);
        if (nRet)
        {
            AFLOG<<"DIffVHD::set parent path:: update into file failed, rc = "<<nRet<<", offset = 0x%016I64X"<<li.QuadPart;
            ATLASSERT(FALSE);
            break;
        }

        //update the ParentLocators
        lo->PlatformCode = nPlatformCode;
        lo->PlatformDataLength = (UINT32)(wcslen(pwszPath))*sizeof(wchar_t); //not including the terminating '\0'

        //update the parent file name
        wstring wstrPath = pwszPath;
        std::size_t pos = wstrPath.rfind(L'\\');
        if (pos != wstring::npos)
            wstrPath = wstrPath.substr(pos+1);
        AFLOG<<"DIffVHD::set parent path::parent new name = "<<wstrPath.c_str();
        wcsncpy_s(m_stHeader.ParentUnicodeName, _countof(m_stHeader.ParentUnicodeName), wstrPath.c_str(), _TRUNCATE);

        
        if (bAllocNewSpace)
        {
            //write out the footer
            li.QuadPart = lo->PlatformDataOffset + lo->PlatformDataSpace;

            VHDFOOTER footer = m_stFooter;
            VHDHelper::ConvFooter(&footer);
            nRet = WriteFileAndPreserveTimePos(m_shVHD, &footer, sizeof(footer), &dwWritten, &li);
            if (nRet)
            {
                AFLOG<<"DIffVHD::set parent path::write new footer failed, rc = "<<nRet<<", offset = 0x%016I64X"<<li.QuadPart;
                ATLASSERT(FALSE);
                break;
            }
        }

    } while (0);


    //3)Update the header
    if (nRet == 0)
    {
        LARGE_INTEGER li = {0};
        DWORD dwWritten = 0;
        //update the header checksum
        m_stHeader.Checksum = 0;
        m_stHeader.Checksum = VHDHelper::CalcChecksum(&m_stHeader, sizeof(m_stHeader));

        VHDDYNHEADER stHeader = m_stHeader;
        VHDHelper::ConvHeader(&stHeader);

        //write out header
        li.QuadPart = sizeof(VHDFOOTER);
        nRet = WriteFileAndPreserveTimePos(m_shVHD, &stHeader, sizeof(stHeader), &dwWritten, &li);
        if (nRet)
        {
            AFLOG<<"DIffVHD::set parent path:: failed to update header, rc = "<<nRet;
        }
        else if (dwWritten != sizeof(stHeader))
        {
            AFLOG<<"DIffVHD::set parent path::to write "<<sizeof(stHeader)<<" bytes, only write "<<dwWritten<<" bytes";
            nRet = E_VHD_COV_WRITEHEADER_FAILED;
        }
    }

    if (pAlignedBuf)
    {
        ::VirtualFree(pAlignedBuf, 0, MEM_RELEASE);
        pAlignedBuf = NULL;
    }

    return nRet;
}

//
//@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
//
int                 
CDiffVHDFile::SetParentTimestamp(unsigned __int32 timestamp)
{
    int nRet = 0;

    //update the parent time and checksum in header
    m_stHeader.ParentTimeStamp = timestamp;
    m_stHeader.Checksum = 0;
    m_stHeader.Checksum = VHDHelper::CalcChecksum(&m_stHeader, sizeof(m_stHeader));

    VHDDYNHEADER stHeader = m_stHeader;
    VHDHelper::ConvHeader(&stHeader);

    DWORD dwWritten;
    LARGE_INTEGER liOffset;
    liOffset.QuadPart = sizeof(VHDFOOTER);

    nRet = WriteFileAndPreserveTimePos(m_shVHD, &stHeader, sizeof(stHeader), &dwWritten, &liOffset);
    if (nRet)
    {
        AFLOG<<"DIffVHD::set parent time:: failed to update time to file, rc = "<<nRet;
    }
    else if (dwWritten != sizeof(stHeader))
    {
        AFLOG<<"DIffVHD::set parent time::to write "<<sizeof(stHeader)<<" bytes, only write "<<dwWritten<<" bytes";
        nRet = E_VHD_COV_WRITEHEADER_FAILED;
    }


    return nRet;
}

//
//@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
//
int          
CDiffVHDFile::GetParentTimestamp(unsigned __int32& timestamp)
{
    timestamp = m_stHeader.ParentTimeStamp;

    return E_VHD_COV_SUCCESS;
}

void                
CDiffVHDFile::FreeBuffer(void* p)
{
    VirtualFree(p, 0, MEM_RELEASE);
}
