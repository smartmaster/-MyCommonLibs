#include "StdAfx.h"
#include "VHDFileWImp.h"
#include "VHDHelper.h"
#include "HALog.h"
#include <atlbase.h>
#include "CommonUtil.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))

CDynVHDFileW::CDynVHDFileW(void)
: m_wstrFilePath(L"")
, m_hVhd(INVALID_HANDLE_VALUE)
, m_llCurrDiskOffset(0)
, m_pBitmap(NULL)
, m_nSPB(0)
, m_llCurrBitmapIdx(-1)
, m_pBAT(NULL)
{
    ZeroMemory(&m_stCreateParms, sizeof(m_stCreateParms));
    ZeroMemory(&m_stFooter, sizeof(m_stFooter));
    ZeroMemory(&m_stHeader, sizeof(m_stHeader));
}

CDynVHDFileW::~CDynVHDFileW(void)
{
	if (m_pBAT != NULL)
	{
		::VirtualFree(m_pBAT, NULL, MEM_RELEASE);
		m_pBAT = NULL;
	}

    if (m_hVhd != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_hVhd);
        m_hVhd = INVALID_HANDLE_VALUE;
    }

    if (m_pBitmap != NULL)
    {
        ::VirtualFree(m_pBitmap, NULL, MEM_RELEASE);
        m_pBitmap = NULL;
    }
    m_llCurrBitmapIdx = -1;
}

int 
CDynVHDFileW::Release()
{
    int nRet = Close();
    
    // here we delete this obj even Close is failed, and return the error code of close()
    // This is to avoid under some circumstance close() always fail and delete this can not be called.
    delete this; 

    return nRet;
}

int CDynVHDFileW::Close()
{
    if (m_hVhd != INVALID_HANDLE_VALUE
        && m_stHeader.Cookie!= 0 
        && m_stFooter.Cookie != 0)
    {
        __int64 llPos = 0;
        int nRet = GetDataEndPos(llPos);
        if (nRet)
        {
            HALogError(L"Close,GetDataEndPos failed, rc = %d", nRet);
            return nRet;
        }
        return WriteFooterAt(&m_stFooter, llPos);
    }

    return 0;
}

int CDynVHDFileW::Seek(__int64 llOffset, __int64* pllNewPos, int nMethod)
{
    if (llOffset % VHD_SECTOR_SIZE)
        return E_INVALIDARG;
    if (nMethod != VDISK_SEEK_BEGIN && nMethod != VDISK_SEEK_CURR && nMethod != VDISK_SEEK_END)
        return E_INVALIDARG;

    __int64 llNewPos = 0;
    if (nMethod == VDISK_SEEK_BEGIN)
        llNewPos = llOffset;
    else if (nMethod == VDISK_SEEK_CURR)
        llNewPos = m_llCurrDiskOffset + llOffset;
    else
    {
        llNewPos = m_stCreateParms.llDiskSize - llOffset;
    }

    if (llNewPos < 0)
    {
        HALogError(L"CDynVHDFileW::Seek, before the beginning of file, pos = %I64d", llNewPos);
        return ERROR_NEGATIVE_SEEK;
    }

    m_llCurrDiskOffset = llNewPos;
    if (pllNewPos)
        *pllNewPos = llNewPos;
    return 0;
}

int 
CDynVHDFileW::Write(const char* pBuf, unsigned long cbToWrite,
                  unsigned long* pcbWritten, __int64 llStartOffset)
{
    if (llStartOffset == -1)
        llStartOffset = m_llCurrDiskOffset;

    if (cbToWrite % VHD_SECTOR_SIZE || llStartOffset % VHD_SECTOR_SIZE)
    {
        ATLASSERT(FALSE);
        return E_INVALIDARG;
    }

    if (llStartOffset + cbToWrite > m_stFooter.CurrentSize)
    {
        ATLASSERT(FALSE);
        return E_INVALIDARG;
    }

    int nRet = 0;
    __int64 sector = llStartOffset/VHD_SECTOR_SIZE;
    __int64 secs   = cbToWrite/VHD_SECTOR_SIZE;
    __int64 off, blk;
    int sec, cnt;

    nRet = ReadBAT();
    if (nRet)
        return nRet;

    do 
    {
        blk = sector / m_nSPB;
        sec = (int)(sector % m_nSPB);

        off = m_pBAT[blk];
        //HALogMsg(L"TEST::blk = 0x%I64X, off = 0x%I64X", blk, off);
        if (off == CA_BLK_UNUSED) 
        {
            nRet = AllocateNewBlock(blk);
            if (nRet)
            {
                break;
            }

            off = m_pBAT[blk];
            //HALogMsg(L"TEST::AllocNewBlk::blk = 0x%I64X, off = 0x%I64X", blk, off);
            ZeroMemory(m_pBitmap, VHDHelper::Bytes_PaddedInSectorSize(m_nSPB/8));
            m_llCurrBitmapIdx = blk;
        }

        //off = off + m_nSPB/(8*VHD_SECTOR_SIZE) + sec;
        off = off + VHDHelper::SecsRoundUp(m_nSPB/8) + sec;

        cnt = MIN(secs, m_nSPB - sec);
        nRet = WriteOut(pBuf, VHDHelper::SectorsToBytes(cnt), VHDHelper::SectorsToBytes(off));
        if (nRet)
        {
            break;
        }

        if (blk != m_llCurrBitmapIdx)
            nRet = ReadBitmap(blk, m_pBitmap, (int)VHDHelper::Bytes_PaddedInSectorSize(m_nSPB/8)); //512K support
            //nRet = ReadBitmap(blk, m_pBitmap, VHD_SECTOR_SIZE);
        if (nRet)
        {
            break;
        }

        for (int i = 0; i < cnt; i++)
            VHDHelper::SetBit((BYTE*)m_pBitmap, sec + i, TRUE);

        //nRet = WriteBitmap(blk, m_pBitmap, VHD_SECTOR_SIZE);
        nRet = WriteBitmap(blk, m_pBitmap, (int)VHDHelper::Bytes_PaddedInSectorSize(m_nSPB/8));//512K support
        if (nRet)
        {
            break;
        }

        secs   -= cnt;
        sector += cnt;
        pBuf   += VHDHelper::SectorsToBytes(cnt);
    } while (secs);

    //TODO: Write the latest footer

    if (nRet == 0)
    {
        m_llCurrDiskOffset = llStartOffset + cbToWrite;
        if (pcbWritten)
            *pcbWritten = cbToWrite;
    }

    return nRet;
}

int
CDynVHDFileW::AllocateNewBlock(__int64 llBlk)
{
    int nRet = 0;
    __int64 llDataEndInSec = 0;

    nRet = GetDataEndPos(llDataEndInSec);
    if (nRet)
    {
        HALogError(L"AllocateNewBlock, GetDataEndPos failed, rc = %d", nRet);
        return nRet;
    }
    llDataEndInSec >>= VHD_SECTOR_SHIFT;

    //// TODO: zero file here
    //  __int64 llSize = m_stHeader.BlockSize + m_nSPB/8;
    ////__int64 llSize = VHD_BLOCK_SIZE + m_nSPB/8;
    ////BYTE* p = new BYTE[llSize];
    ////ZeroMemory(p, llSize);
    ////WriteOut(p, llSize, VHDHelper::SectorsToBytes(llDataEndInSec));
    ////delete[] p;

    m_pBAT[llBlk] = llDataEndInSec;
    nRet = WriteBAT();
    if (nRet)
    {
        HALogError(L"AllocateNewBlock, Write BAT failed, rc = %d", nRet);
    }
    
    return nRet;
}

int
CDynVHDFileW::ReadBAT()
{
    if (m_pBAT != NULL)
        return 0;

    int nRet = 0;
    unsigned int nSize = (unsigned int)VHDHelper::Bytes_PaddedInSectorSize(m_stHeader.MaxTableEntries * sizeof(__int32));
    m_pBAT = (unsigned int*)::VirtualAlloc(NULL, nSize, MEM_COMMIT, PAGE_READWRITE);
    if (m_pBAT == NULL)
    {
        ATLASSERT(FALSE);
        HALogError(L"CDynVHDFileW::ReadBAT, Virtual alloc failed, size=%d", nSize);
        return E_OUTOFMEMORY;
    }
    ZeroMemory(m_pBAT, nSize);

    DWORD dwRead = 0;
    nRet = ReadIn(m_pBAT, nSize, &dwRead, m_stHeader.TableOffset);
    if (dwRead != nSize)
    {
        HALogError(L"ReadBAT, ToRead = %d, read = %d", nSize, dwRead);
        nRet = -1;
    }
    if (nRet == 0)
        VHDHelper::Ntoh_BAT(m_pBAT, nSize/sizeof(int));
    else
    {
        HALogError(L"CDynVHDFileW::ReadBAT, ReadIn failed, offset=0x%016I64X, ToRead = %d, rc = %d", 
            m_stHeader.TableOffset, nSize, nRet);
        VirtualFree(m_pBAT, 0, MEM_RELEASE);
        m_pBAT = NULL;
    }

    return nRet;
}

int 
CDynVHDFileW::ReadFooter(VHDFOOTER* pstFooter, __int64 llOffset)
{
    if (pstFooter == NULL)
        return E_INVALIDARG;

    int nRet = 0;
    DWORD cbRead = 0;
    nRet = ReadIn(pstFooter, sizeof(VHDFOOTER), &cbRead, llOffset);
    if (nRet == 0 && cbRead == sizeof(VHDFOOTER))
    {
        VHDHelper::Hton_Footer(pstFooter);
    }
    else if (nRet == 0)
    {
        HALogError(L"ReadFooter, ToRead = %d, Read = %d, offset = 0x%I64X", 
            sizeof(VHDFOOTER), cbRead, llOffset);
        nRet = -1;
    }
    else if (nRet)
    {
        HALogError(L"ReadFooter failed, rc = %d, offset = 0x%I64X", nRet, llOffset);
    }

    return nRet;
}

int 
CDynVHDFileW::ReadHeader(VHDDYNHEADER* pstHeader)
{
    if (pstHeader == NULL)
        return E_INVALIDARG;

    int nRet = 0;
    DWORD cbRead = 0;
    nRet = ReadIn(pstHeader, sizeof(VHDDYNHEADER), &cbRead, sizeof(VHDFOOTER));
    if (nRet == 0 && cbRead == sizeof(VHDDYNHEADER))
    {
        VHDHelper::Hton_Header(pstHeader);
    }
    else if (nRet == 0)
    {
        HALogError(L"ReadHeader, ToRead = %d, Read = %d", sizeof(VHDDYNHEADER), cbRead);
        nRet = -1;
    }
    else if (nRet)
    {
        HALogError(L"ReadHeader failed, rc = %d", nRet);
    }

    return nRet;
}

int
CDynVHDFileW::ReadBitmap(__int64 llBlk, char* pBuf, int nBufSize)
{
    //ATLENSURE(m_nSPB/8 == VHD_SECTOR_SIZE); //512K support
    if (llBlk >= m_stHeader.MaxTableEntries)
        return E_INVALIDARG;
    if (nBufSize < VHDHelper::Bytes_PaddedInSectorSize(m_nSPB/8))
        return E_INVALIDARG;

    LARGE_INTEGER liPos;
    liPos.QuadPart = m_pBAT[llBlk];
    if (liPos.QuadPart == CA_BLK_UNUSED)
    {
        ZeroMemory(pBuf, nBufSize);
        return 0;
    }

    int nRet = 0;
    DWORD cbToRead = (DWORD)VHDHelper::Bytes_PaddedInSectorSize(m_nSPB/8);
    DWORD cbRead = 0;
    nRet = ReadIn(pBuf, cbToRead, &cbRead, VHDHelper::SectorsToBytes(liPos.QuadPart));
    if (nRet == 0 && cbRead != cbToRead)
    {
        HALogError(L"Read Bitmap, ToRead = %d, Read = %d", cbToRead, cbRead);
        nRet = -1;
    }
    else if (nRet)
    {
        HALogError(L"Read Bitmap failed, rc = %d, blk_idx = %I64d, off_in_sec = %I64d",
            nRet, llBlk, liPos.QuadPart);
    }

    return nRet;
}

int 
CDynVHDFileW::WriteBitmap(__int64 llBlk, const char* pBuf, int nBufSize)
{
    //ATLENSURE(m_nSPB/8 == VHD_SECTOR_SIZE); //512K support
    if (llBlk >= m_stHeader.MaxTableEntries)
        return E_INVALIDARG;
    //if (nBufSize != m_nSPB/8)
    if (nBufSize != VHDHelper::Bytes_PaddedInSectorSize(m_nSPB/8)) //512K support
    {
        HALogError(L"WriteBitmap, Invalid buf size, size = %d", nBufSize);
        return E_INVALIDARG;
    }

    LARGE_INTEGER liPos;
    liPos.QuadPart = m_pBAT[llBlk];
    if (liPos.QuadPart == CA_BLK_UNUSED)
    {
        HALogError(L"WriteBitmap, unused block, blk = %I64d", llBlk);
        return E_INVALIDARG;
    }

    int nRet = 0;
    nRet = WriteOut(pBuf, nBufSize, VHDHelper::SectorsToBytes(liPos.QuadPart));
    if (nRet)
    {
        HALogError(L"Write Bitmap failed, rc = %d, blk_idx = %I64d, off_in_sec = %I64d",
            nRet, llBlk, liPos.QuadPart);
    }

    return nRet;
}

static inline size_t
ParentLocatorSize(const VHDPARENTLOCATOR *pLoc)
{
    //
    // In VHD specification, space is in sector, but MS sometimes use bytes
    // So tricky!
    //
    if (pLoc->PlatformDataSpace < 512)
        return VHDHelper::SectorsToBytes(pLoc->PlatformDataSpace);
    else if (pLoc->PlatformDataSpace % 512 == 0)
        return pLoc->PlatformDataSpace;
    else
        return 0;
}

int 
CDynVHDFileW::GetDataEndPos(__int64& llPos)
{
    // attention: consider for diff VHD, which has parent locator.
    if (m_pBAT == NULL)
        return E_INVALIDARG;

    __int64 llOffset = 0;

    //
    // First calc the max of footer, header and BAT
    //
    llOffset = m_stFooter.DataOffset + sizeof(VHDDYNHEADER);
    __int64 llEndOfBAT = m_stHeader.TableOffset + 
        VHDHelper::Bytes_PaddedInSectorSize(m_stHeader.MaxTableEntries*sizeof(__int32));
    if (llOffset < llEndOfBAT)
        llOffset = llEndOfBAT;
    
    //
    // Then check the parent locators
    //
    __int64 llEndOfLoc = 0;
    VHDPARENTLOCATOR* pLoc = NULL;
    for (int i = 0; i<_countof(m_stHeader.ParentLocators); i++)
    {
        pLoc = &m_stHeader.ParentLocators[i];
        if (pLoc->PlatformCode == CPlatformCode::None)
            continue;
        if (pLoc->PlatformDataOffset + ParentLocatorSize(pLoc) > llEndOfLoc)
            llEndOfLoc = pLoc->PlatformDataOffset + ParentLocatorSize(pLoc);
    }
    if (llOffset < llEndOfLoc)
        llOffset = llEndOfLoc;


    //
    // Check for the existing data block
    //
    for (UINT32 i = 0; i < m_stHeader.MaxTableEntries; i++)
    {
        if (m_pBAT[i] == CA_BLK_UNUSED)
            continue;

        //if (__int64(m_pBAT[i])*VHD_SECTOR_SIZE + VHD_BLOCK_SIZE + m_nSPB/8 > llOffset)
        //{
        //    llOffset = __int64(m_pBAT[i])*VHD_SECTOR_SIZE + VHD_BLOCK_SIZE + m_nSPB/8;
        //}
        if (__int64(m_pBAT[i])*VHD_SECTOR_SIZE + m_stHeader.BlockSize 
            + VHDHelper::Bytes_PaddedInSectorSize(m_nSPB/8) > llOffset) //512K support
        {
            llOffset = __int64(m_pBAT[i])*VHD_SECTOR_SIZE + m_stHeader.BlockSize + VHDHelper::Bytes_PaddedInSectorSize(m_nSPB/8);
        }
    }

    llPos = llOffset;
    //HALogMsg(L"GetDataEndPos, data end pos = 0x%I64X", llOffset);


    return 0;
}

int
CDynVHDFileW::Init(const wchar_t*         pwszFilePath, 
                   const ST_CREATE_PARMS* pstCreateParms)
{
    return Init(pwszFilePath, pstCreateParms, NULL);
}

int
CDynVHDFileW::Init(const wchar_t*         pwszFilePath, 
                const ST_CREATE_PARMS* pstCreateParms,
                const wchar_t*         pwszParentPath)
{
    int nRet = 0;
    bool bCreateNew = false;

    if (pstCreateParms == NULL || pwszFilePath == NULL || pwszFilePath[0] == L'\0')
        return E_INVALIDARG;
    if (pstCreateParms->dwCreateDisp != CREATE_ALWAYS && pstCreateParms->dwCreateDisp != OPEN_ALWAYS)
    {
        HALogError(L"CDynVHDFileW::Init, create_disp not create_always nor open_always, file = %s, disp = %d",
            pwszFilePath, pstCreateParms->dwCreateDisp);
        return E_INVALIDARG;
    }

    m_wstrFilePath = pwszFilePath;
    m_stCreateParms = *pstCreateParms;

    do
    {
        if (pstCreateParms->dwCreateDisp == CREATE_ALWAYS 
            || pstCreateParms->dwCreateDisp == OPEN_ALWAYS && !FileExist(pwszFilePath))
        {
            nRet = Create(pwszFilePath, pstCreateParms, pwszParentPath);
            if (nRet)
            {
                ATLASSERT(FALSE);
                break;
            }
            bCreateNew = true;
        }
        // TODO: change FlagAndAttributes according to dwFlags
        m_hVhd = ::CreateFileW(pwszFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (m_hVhd == INVALID_HANDLE_VALUE)
        {
            nRet = ::GetLastError();
            ATLASSERT(FALSE);
            break;
        }

        // 
        // read footer
        // Currently we only read footer from beginning of file
        nRet = ReadFooter(&m_stFooter, 0);
        if (nRet)
        {
            ATLASSERT(FALSE);
            break;
        }

        // read header
        nRet = ReadHeader(&m_stHeader);
        if (nRet)
        {
            ATLASSERT(FALSE);
            break;
        }

        if (m_stHeader.BlockSize != VHD_BLOCK_SIZE)
        {
            HALogMsg(L"CDynVHDFileW::Init,not 2M, path = %s, BlockSize = 0x%X.", pwszFilePath, m_stHeader.BlockSize);
        }
        m_nSPB = m_stHeader.BlockSize >> VHD_SECTOR_SHIFT;
        m_pBitmap = (char*)::VirtualAlloc(NULL, VHDHelper::Bytes_PaddedInSectorSize(m_nSPB/8), MEM_COMMIT, PAGE_READWRITE);
        if (m_pBitmap == NULL)
        {
            nRet = E_OUTOFMEMORY;
            HALogError(L"Alloc mem for bitmap failed, size = %d", VHDHelper::Bytes_PaddedInSectorSize(m_nSPB/8));
            break;
        }

        // If is open a existed vhd file, double check the vhd footer.
        if (!bCreateNew) 
        {
            nRet = DoubleCheckFooter();
            if (nRet)
            {
                HALogError(L"CDynVHDFileW::DoubleCheck footer failed, file = %s, rc = %d", pwszFilePath, nRet);
                break;
            }
        }

        if (m_stCreateParms.llDiskSize == 0)
            m_stCreateParms.llDiskSize = m_stFooter.CurrentSize;
    } while (0);

    return nRet;
}

int
CDynVHDFileW::Create(const wchar_t*         pwszFilePath, 
                     const ST_CREATE_PARMS* pstCreateParms,
                     const wchar_t*         pwszParentPath)
{
    if (pwszFilePath == NULL || pstCreateParms == NULL)
        return E_INVALIDARG;

    int nRet = 0;

    do
    {
        // 1) Create file
        m_hVhd = ::CreateFileW(pwszFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
            NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (m_hVhd == INVALID_HANDLE_VALUE)
        {
            nRet = ::GetLastError();
            HALogError(L"CDynVHDFileW::Create, create file failed, path = %s, rc = %d", pwszFilePath, nRet);
            break;
        }



        // 2) Init footer, init header, write footer write header
        GUID uniqueID = {0};
        if (pstCreateParms->ulFlags & VHD_CREATE_FLAG_CUSTOM_GUID)
        {
            uniqueID = pstCreateParms->uniqueID;
            HALogMsg(L"CDynVHDFileW::Create, use custom GUID, path = %s", pwszFilePath);
        }
        else
        {
            CoCreateGuid(&uniqueID);
        }
        nRet = InitializeFooter(pstCreateParms->eType, pstCreateParms->nVer, pstCreateParms->llDiskSize, uniqueID);
        if (nRet)
        {
            break;
        }

        nRet = InitializeHeader(pstCreateParms->eType, pstCreateParms->llDiskSize,
                                pwszParentPath);
        if (nRet)
        {
            break;
        }

        nRet = WriteFooterAt(&m_stFooter, 0);
        if (nRet)
        {
            HALogError(L"CDynVHDFileW::Create, WriteFooterAt failed, offset = 0, rc = %d", nRet);
            break;
        }
        nRet = WriteHeaderAt(&m_stHeader, VHD_SECTOR_SIZE);
        if (nRet)
        {
            HALogError(L"CDynVHDFileW::Create, WriteHeaderAt failed, offset = SECTOR_SIZE, rc = %d", nRet);
            break;
        }

        // 3) Create bat
        nRet = CreateBAT();
        if (nRet)
        {
            HALogError(L"CDynVHDFileW::Create, CreateBAT failed, rc = %d", nRet);
            break;
        }

        // 4) If is diff, write parent locator
        if (pstCreateParms->eType == VHD_Diff)
        {
            nRet = WriteParentLocators(pwszParentPath);
            if (nRet)
            {
                HALogError(L"WriteParentLocators failed, rc = %d, parent path = %s",
                    nRet,pwszParentPath);
                break;
            }
            //For diff VHD, write header again.
            nRet = WriteHeaderAt(&m_stHeader, VHD_SECTOR_SIZE);
            if (nRet)
            {
                HALogError(L"Write header for diff VHD failed, 2nd, rc = %d", nRet);
                break;
            }
        }


        // 5) write another footer
        // come to the end of file
        LARGE_INTEGER liZero ={0};
        LARGE_INTEGER liEnd = {0};
        ::SetFilePointerEx(m_hVhd, liZero, &liEnd, FILE_END);
        nRet = WriteFooterAt(&m_stFooter, liEnd.QuadPart);
        if (nRet)
        {
            HALogError(L"Create, WriteFooter at end failed, rc = %d, offset = 0x%016I64X", liEnd.QuadPart, nRet);
            break;
        }
    } while (0);

    if (m_hVhd != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_hVhd);
        m_hVhd = INVALID_HANDLE_VALUE;
    }
    if (m_pBAT)
    {
        ::VirtualFree(m_pBAT, 0, MEM_RELEASE);
        m_pBAT = NULL;
    }
    ZeroMemory(&m_stFooter, sizeof(m_stFooter));
    ZeroMemory(&m_stHeader, sizeof(m_stHeader));


    return nRet;
}

int
CDynVHDFileW::InitializeFooter(VHD_TYPE eType, int nVer, __int64 llDiskSize, const GUID& uniqueID)
{
    ZeroMemory(&m_stFooter, sizeof(m_stFooter));
    memcpy(m_stFooter.Cookie, CA_VHD_COOKIE, sizeof(m_stFooter.Cookie));
    m_stFooter.Features     = CA_VHD_RESERVED;
    m_stFooter.FileFormatVersion    = CA_VHD_FF_VERSION;
    time_t tCurTime = 0;
    time(&tCurTime);
    m_stFooter.TimeStamp          = VHDHelper::ConvTimeStamp(tCurTime);
    memcpy_s(m_stFooter.CreatorApplication, sizeof(m_stFooter.CreatorApplication), "BAB", 4);
    m_stFooter.CreatorVersion     = nVer;
    memcpy_s(m_stFooter.CreatorHostOS, sizeof(m_stFooter.CreatorHostOS), "Wi2k", 4);
    m_stFooter.OriginalSize       = llDiskSize;
    m_stFooter.CurrentSize        = llDiskSize;
    //TODO: here maybe not (non-zero)
    __int64 llTotalSectors = VHDHelper::SecsRoundUp(llDiskSize);
    m_stFooter.DiskGeometry.Value = VHDHelper::CalcCHS(llTotalSectors);
    m_stFooter.DiskType           = eType;
    m_stFooter.SavedState         = 0;
    m_stFooter.DataOffset         = -1;
    m_stFooter.UniqueId           = uniqueID;

    return 0;
}

int
CDynVHDFileW::InitializeHeader(VHD_TYPE eType, __int64 llDiskSize,
                               const wchar_t* pwszParentPath)
{
    int nRet = 0;

    ZeroMemory(&m_stHeader, sizeof(m_stHeader));
    memcpy_s(m_stHeader.Cookie, sizeof(m_stHeader.Cookie), "cxsparse", 8);
    m_stHeader.DataOffset       = -1;
    m_stHeader.TableOffset      = sizeof(VHDFOOTER) + sizeof(VHDDYNHEADER); /* 1 ftr + 2 hdr */
    m_stHeader.HeaderVersion    = CA_VHD_HR_VER;
    m_stHeader.BlockSize        = VHD_BLOCK_SIZE;
    m_stHeader.ParentTimeStamp  = 0;
    m_stHeader.Reserved         = 0;
    m_stHeader.MaxTableEntries = (m_stFooter.CurrentSize +
        VHD_BLOCK_SIZE - 1) >> VHD_BLOCK_SHIFT;

    m_stFooter.DataOffset  = VHD_SECTOR_SIZE;

    if (m_stFooter.DiskType == VHD_Dynamic)
        return 0;
    
    // Diff If parent does no exist, return fail
    IVHDFile* pParent = NULL;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    do
    {
        //Get parent time
        WIN32_FIND_DATA find_data = {0};
        hFind = FindFirstFile(pwszParentPath, &find_data);
        if (hFind == INVALID_HANDLE_VALUE)
        {
            HALogError(L"CDynVHDFileW::InitializeHeader, can NOT find parent file, path = %s", pwszParentPath);
            nRet = ERROR_FILE_NOT_FOUND;
            break;
        }

        //nRet = CreateVHDFileEx(pwszParentPath, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, &pParent);
        if (nRet)
        {
            HALogError(L"CDynVHDFileW::InitializeHeader, CreateVHDFileEx read only failed, path = %s, rc = %d",
                pwszParentPath, nRet);
            break;
        }

        
        m_stHeader.ParentTimeStamp = find_data.ftLastWriteTime.dwLowDateTime;
        //Get UUID
        GUID obGuid = {0};
        nRet = pParent->GetGuid(&obGuid);
        m_stHeader.ParentUniqueID   = obGuid;

        // Need use parent size?

        //Set parent name
        const wchar_t* pName = wcsrchr(pwszParentPath, L'\\');
        if( !pName )
            pName = pwszParentPath;
        else
            pName++;

        wcsncpy_s(m_stHeader.ParentUnicodeName, _countof(m_stHeader.ParentUnicodeName), pName, _TRUNCATE);
    } while (0);

    if (pParent)
    {
        pParent->Release();
        pParent = NULL;
    }
    if (hFind != INVALID_HANDLE_VALUE)
    {
        ::FindClose(hFind);
        hFind = INVALID_HANDLE_VALUE;
    }

    return nRet;
}


int
CDynVHDFileW::WriteFooterAt(const VHDFOOTER* pstFooter, __int64 llOffset)
{
    int nRet = 0;

    int nSize = (int)VHDHelper::Bytes_PaddedInSectorSize(sizeof(VHDFOOTER));
    VHDFOOTER* pf = (VHDFOOTER*)::VirtualAlloc(NULL, nSize, MEM_COMMIT, PAGE_READWRITE);
    if (pf == NULL)
    {
        HALogError(L"CDynVHDFileW::WriteFooterAt, Virtual alloc failed, size=%d", nSize);
        return E_OUTOFMEMORY;
    }

    ZeroMemory(pf, nSize);
    memcpy_s(pf, sizeof(VHDFOOTER), pstFooter, sizeof(VHDFOOTER));
    pf->Checksum = 0;
    pf->Checksum = VHDHelper::CalcChecksum(pf, sizeof(VHDFOOTER));

    VHDHelper::Hton_Footer(pf);
    nRet = WriteOut(pf, sizeof(VHDFOOTER), llOffset);

    ::VirtualFree(pf, 0, MEM_RELEASE);

    return nRet;
}

int
CDynVHDFileW::WriteHeaderAt(const VHDDYNHEADER* pstHeader, __int64 llOffset)
{
    int nRet = 0;

    int nSize = (int)VHDHelper::Bytes_PaddedInSectorSize(sizeof(VHDDYNHEADER));
    VHDDYNHEADER* ph = (VHDDYNHEADER*)::VirtualAlloc(NULL, nSize, MEM_COMMIT, PAGE_READWRITE);
    if (ph == NULL)
    {
        HALogError(L"CDynVHDFileW::WriteFooterAt, Virtual alloc failed, size=%d", nSize);
        return E_OUTOFMEMORY;
    }

    ZeroMemory(ph, nSize);
    memcpy_s(ph, sizeof(VHDDYNHEADER), pstHeader, sizeof(VHDDYNHEADER));
    ph->Checksum = 0;
    ph->Checksum = VHDHelper::CalcChecksum(ph, sizeof(VHDDYNHEADER));

    VHDHelper::Hton_Header(ph);
    nRet = WriteOut(ph, sizeof(VHDDYNHEADER), llOffset);
    if (nRet)
    {
        HALogError(L"WriteHeaderAt::WriteOut failed, offset = 0x%016I64X, rc = %d", llOffset, nRet);
    }

    ::VirtualFree(ph, 0, MEM_RELEASE);

    return nRet;
}

int
CDynVHDFileW::CreateBAT()
{
    int nRet = 0;

    do
    {
        __int64 llSize = VHDHelper::Bytes_PaddedInSectorSize(m_stHeader.MaxTableEntries * sizeof(__int32));
        m_pBAT = (unsigned int*)VirtualAlloc(NULL, llSize, MEM_COMMIT, PAGE_READWRITE);
        if (m_pBAT == NULL)
        {
            HALogError(L"CDynVHDFileW::CreateBAT, Virtual alloc failed, size=%I64d", llSize);
            break;
        }
        ZeroMemory(m_pBAT, llSize);
        for (unsigned int i = 0; i < m_stHeader.MaxTableEntries; i++)
            m_pBAT[i] = CA_BLK_UNUSED;

        m_nSPB = m_stHeader.BlockSize >> VHD_SECTOR_SHIFT;

        nRet = WriteBAT();
        if (nRet)
        {
            HALogError(L"CDynVHDFileW::CreateBAT, WriteBAT failed, rc = %d", nRet);
            break;
        }
    } while (0);



    return nRet;
}


int
CDynVHDFileW::WriteBAT()
{
    if (m_pBAT == NULL)
        return E_INVALIDARG;

    int nRet = 0;

    __int64 llSize = VHDHelper::Bytes_PaddedInSectorSize(m_stHeader.MaxTableEntries * sizeof(__int32));
    unsigned int* pBAT = (unsigned int*)::VirtualAlloc(NULL, llSize, MEM_COMMIT, PAGE_READWRITE);
    if (pBAT == NULL)
    {
        HALogError(L"CDynVHDFileW::WriteBAT, Virtual alloc failed, size=%I64d", llSize);
        return E_OUTOFMEMORY;
    }
    ZeroMemory(pBAT, llSize);
    memcpy_s(pBAT, llSize, m_pBAT, m_stHeader.MaxTableEntries * sizeof(__int32));

    VHDHelper::Hton_BAT(pBAT, m_stHeader.MaxTableEntries);

    nRet = WriteOut(pBAT, llSize, m_stHeader.TableOffset);
    if (nRet)
    {
        HALogError(L"CDynVHDFileW::WriteBAT, WriteOut failed, offset = 0x%016I64X, size = %I64d, rc = %d", 
            m_stHeader.TableOffset, llSize, nRet);
    }
    ::VirtualFree(pBAT, 0, MEM_RELEASE);

    return nRet;
}

int
CDynVHDFileW::DoubleCheckFooter()
{
    if (m_hVhd == INVALID_HANDLE_VALUE)
        return E_INVALIDARG;

    LARGE_INTEGER liSize = {0};
    if (!::GetFileSizeEx(m_hVhd, &liSize))
    {
        int nRet = ::GetLastError();
        HALogError(L"CDynVHDFileW::DoubleCheckFooter, Get file size failed, rc = %d", nRet);
        return nRet;
    }

    bool bNeedRepair = true;
    if (liSize.QuadPart > sizeof(VHDFOOTER)*2 + sizeof(VHDDYNHEADER)
        && liSize.QuadPart % VHD_SECTOR_SIZE == 0)
    {
        VHDFOOTER stFooter = {0};
        if (ReadFooter(&stFooter, liSize.QuadPart - VHD_SECTOR_SIZE) == 0)
        {
            UINT32 dwCheckSum = stFooter.Checksum;
            stFooter.Checksum = 0;
            if (dwCheckSum == VHDHelper::CalcChecksum(&stFooter, sizeof(VHDFOOTER)))
                bNeedRepair = false;
        }
    }
    
    // write the m_stFooter to the end of file.
    if (bNeedRepair)
    {
        HALogMsg(L"CDynVHDFileW::DoubleCheckFooter, Need repair, file = %s", m_wstrFilePath.c_str());
        int nRet = ReadBAT();
        if (nRet)
            return nRet;

         __int64 llPos = 0;
        nRet = GetDataEndPos(llPos);
        if (nRet)
        {
            HALogError(L"CDynVHDFileW::DoubleCheckFooter, GetDataEndPos failed, rc = %d", nRet);
            return nRet;
        }
        return WriteFooterAt(&m_stFooter, llPos);
    }

    return 0;
}

int 
CDynVHDFileW::WriteParentLocators(const wchar_t* pwszParentPath)
{
    if (pwszParentPath == NULL)
        return E_INVALIDARG;


    int nRet = 0;
    __int64 llOffset = 0;
    int nPlatCode = CPlatformCode::None;

    //if (m_stCreateParms.eType != VHD_Diff)
    //    return E_INVALIDARG;

    // Write the locator after the BAT
    llOffset = m_stFooter.DataOffset + sizeof(VHDDYNHEADER);
    __int64 llEndOfBAT = m_stHeader.TableOffset + 
        VHDHelper::Bytes_PaddedInSectorSize(m_stHeader.MaxTableEntries*sizeof(__int32));
    if (llOffset < llEndOfBAT)
        llOffset = llEndOfBAT;

    for (int i = 0; i < 2; i++) 
    {
        switch (i) 
        {
        case 0:
            nPlatCode = CPlatformCode::W2ku;
            break;
        case 1:
            nPlatCode = CPlatformCode::W2ru;
            break;
        }

        nRet = WriteParentLocatorAt(pwszParentPath, nPlatCode,
            &m_stHeader.ParentLocators[i], llOffset);
        if (nRet)
            break;

        llOffset += ParentLocatorSize(&m_stHeader.ParentLocators[i]);
    }

    return 0;
}

int 
CDynVHDFileW::WriteParentLocatorAt(const wchar_t* pwszParentPath, 
                                   int nPlatCode, VHDPARENTLOCATOR* pLoc, __int64 llOffset)
{
    if (nPlatCode != CPlatformCode::W2ku && nPlatCode != CPlatformCode::W2ru)
        return E_INVALIDARG;

    wstring wstrRelativepath;
    if (nPlatCode == CPlatformCode::W2ru)
    {
        int nTemp = GenParentRelitivePath(m_wstrFilePath.c_str(), pwszParentPath, wstrRelativepath);
        if (nTemp)
        {
            HALogMsg(L"WriteParentLocatorAt, can NOT gen relative path, parent = %s, child = %s, rc=%d",
                pwszParentPath, m_wstrFilePath.c_str(), nTemp);
            return 0; //here if does not support relative, directly return 0
        }
        // else run down to write locator
        pwszParentPath = wstrRelativepath.c_str();
        
    }

    int nSpaceInWords = 256;
    pLoc->PlatformCode          = nPlatCode;
    pLoc->PlatformDataLength    = wcslen(pwszParentPath) * sizeof(wchar_t);
    pLoc->PlatformDataSpace     = nSpaceInWords*sizeof(wchar_t);
    pLoc->Reserved              = 0;
    pLoc->PlatformDataOffset    = llOffset;

    wchar_t* pwszTemp = new wchar_t[nSpaceInWords];
    ZeroMemory(pwszTemp, nSpaceInWords*sizeof(wchar_t));
    wcsncpy_s(pwszTemp, nSpaceInWords, pwszParentPath, _TRUNCATE);

    int nRet = WriteOut(pwszTemp, nSpaceInWords*sizeof(wchar_t), llOffset);
    if (nRet)
    {
        HALogError(L"WriteParentLocatorAt failed, rc = %d, path = %s, offset = %I64d",
            nRet, pwszParentPath, llOffset);
    }
    delete[] pwszTemp;

    return nRet;
}










int 
CDynVHDFileW::WriteOut(const void* pBuf, unsigned long cbToWrite, __int64 llOffset)
{
    DWORD cbWritten = 0;
    OVERLAPPED op = {0};
    op.Offset     = (DWORD)(llOffset & (DWORD)(-1));
    op.OffsetHigh = (DWORD)(llOffset >> 32);

    if (!::WriteFile(m_hVhd, pBuf, cbToWrite, &cbWritten, &op))
    {
        int nRet = ::GetLastError();
        HALogError(L"WriteOut failed, rc = %d, offset = %I64d, ToWrite = %d",
            nRet, llOffset, cbToWrite);
        ATLASSERT(FALSE);
        return nRet;
    }
    
    return 0;
}

int 
CDynVHDFileW::ReadIn(void* pBuf, unsigned long cbToRead,
           unsigned long* pcbRead, __int64 llOffset)
{
    OVERLAPPED op = {0};
    op.Offset     = (DWORD)(llOffset & (DWORD)(-1));
    op.OffsetHigh = (DWORD)(llOffset >> 32);

    if (!::ReadFile(m_hVhd, pBuf, cbToRead, pcbRead, &op))
    {
        int nRet = ::GetLastError();
        HALogError(L"ReadIn failed, rc = %d, offset = %I64d, cbToRead = %d",
            nRet, llOffset, cbToRead);
        return nRet;
    }

    return 0;
}





