#include "StdAfx.h"
#include "VDiskDiffImp.h"
#include "HAVhdUtility.h"
#include "vhdhelper.h"
#include "CommonUtil.h"
#include "HALog.h"
#include <atlbase.h>

static const int SECTOR_SIZE = 512;
static const int READ_BLOCK_SIZE = 2*1024*1024; // each time each 2M

static bool IsD2DFile(const wchar_t* pwszFileName)
{
    if (pwszFileName == NULL)
        return false;

    const wchar_t* p = wcsrchr(pwszFileName, L'.');
    return ( p && _wcsicmp(p, L".D2D") == 0 );
}

inline bool IsBitSet(BYTE ch, int nIndex)
{
    if (nIndex > 7)
        nIndex = 7;
    else if (nIndex < 0)
        nIndex = 0;

   unsigned char cMask = (unsigned char)(1<<(7-nIndex));

   return (ch & cMask) > 0;
}

#if 0
static BOOL IsBitSet(unsigned char* pBuf, int nBit)
{
   unsigned char cByte = pBuf[nBit/8];
   int nBitLeft = nBit%8;
   unsigned char cMask = 0;
   cMask = 1<<(7-nBitLeft);

   return cByte & cMask;
}
#endif //if0

CVDiskDiffImp::CVDiskDiffImp(void)
: m_llCurrPosInSec(0)
, m_pbImageData(NULL)
, m_pbParentData(NULL)
, m_llCacheStartOffset(0)
, m_nCacheDataLen(0)
, m_pstImageBmp(NULL)
, m_pstParentBmp(NULL)
, m_llTotalSec(-1)
, m_bContainFullVHD(false)
{
}

CVDiskDiffImp::~CVDiskDiffImp(void)
{
    UnInit();
}

int 
CVDiskDiffImp::Init(const wchar_t* pwszVHDFile, D2D_ENCRYPTION_INFO* pEncInfo)
{
    if (pwszVHDFile == NULL)
        return E_INVALIDARG;

    UnInit();

    HALogMsg(L"CVDiskDiffImp, Init begin, path = %s", pwszVHDFile);

    int nRet = 0;
    IVHDFile* pVHDFile = NULL;
    //nRet = CreateVHDFileEx(pwszVHDFile, GENERIC_READ, FILE_SHARE_READ, pEncInfo, &pVHDFile);
    if (nRet)
    {
        if (nRet == ERROR_SHARING_VIOLATION)
        {
            HALogMsg(L"CVDiskDiffImp::Init, try create IVHDFile with share_rw, file = %s", pwszVHDFile);
            //nRet = CreateVHDFileEx(pwszVHDFile, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, pEncInfo, &pVHDFile);
        }

        if (nRet)
        {
            HALogError(L"CVDiskDiffImp::Init, create IVHD failed, file = %s, rc = %d", pwszVHDFile, nRet);
            return nRet;
        }
    }
    if (pVHDFile->GetVHDType() != VHD_Diff && pVHDFile->GetVHDType() != VHD_Diff2)
        m_bContainFullVHD = true;
    m_vecVHDFile.push_back(pVHDFile);

    return 0;
}

/**
* @wstrPath - [in] the relative path [out] the absolute path
* @pwszCurr - [in] current path
*/
static BOOL GetAbsPathByRel(wstring& wstrPath, LPCWSTR pwszCurr)
{
    int nBackCount = 0;
    std::wstring strName;
    wchar_t wszParent[1024] = {0};
    wcsncpy_s(wszParent, _countof(wszParent), wstrPath.c_str(), _TRUNCATE);

    LPWSTR p = wcsstr(wszParent, L"..\\");
    LPWSTR pName = wszParent;
    while( p )
    {
        nBackCount++;
        pName = p+3;
        p = wcsstr(p+3, L"..\\");
    }

    if( !_wcsnicmp(pName, L".\\", 2) )
    {
        pName+=2;
    }

    strName = pName;

    wcsncpy_s(wszParent, _countof(wszParent), pwszCurr, _TRUNCATE);

    //remove the file name
    p = wcsrchr(wszParent, L'\\');
    if( !p )
    {
        return FALSE;
    }
    *p=0;

    for( int i=0; i<nBackCount; i++)
    {
        p = wcsrchr(wszParent, L'\\');
        if( !p )
        {
            return FALSE;
        }

        *p=0;
    }

    wstrPath = wszParent;
    wstrPath += L"\\";
    wstrPath += strName.c_str();
    return TRUE;
}

static int GetParentPath(IVHDFile* pVHDFile, bool bD2DExt, VHD_PARENTPATH_TYPE type, wstring& wstrToOpen)
{
    int nRet = 0;
    wchar_t* pwszParent = NULL;

    do
    {
        nRet = pVHDFile->GetParentPath(type, &pwszParent);
        if (nRet)
        {
            HALogError(L"CVDiskDiffImp::Init2, Get parent path failed, curr = %s, type = %d, rc = %d", 
                pVHDFile->GetFilePath(), type, nRet);
            break;
        }
        wstrToOpen = pwszParent;

        if (type == WIN_RELATIVE)
        {
            const wchar_t* p = pVHDFile->GetFilePath();
            if (!GetAbsPathByRel(wstrToOpen, p))
            {
                HALogWarning(L"CVDiskDiffImp::Get ABS by REL failed, ABS = %s, curr = %s", wstrToOpen, p);
                wstrToOpen = L"";
            }
        }

        if (bD2DExt)
        {
            wstring::size_type pos = wstrToOpen.rfind(L'.');
            if (pos != wstring::npos)
            {
                wstrToOpen = wstrToOpen.substr(0, pos) + L".D2D";
            }
        }
    } while (0);

    if (pwszParent)
    {
        pVHDFile->FreeBuffer(pwszParent);
        pwszParent = NULL;
    }


    return nRet;
}

int
CVDiskDiffImp::Init(const wchar_t* pwszVHDOld, const wchar_t* pwszVHDNew, D2D_ENCRYPTION_INFO* pEncInfo)
{
    return Init_inner(pwszVHDOld, pwszVHDNew, pEncInfo, true);
}

int 
CVDiskDiffImp::Init(const wchar_t* pwszVHDOld, const wchar_t* pwszVHDNew, bool bRONew)
{
    return Init_inner(pwszVHDOld, pwszVHDNew, NULL, bRONew);
}

int 
CVDiskDiffImp::Init_inner(const wchar_t* pwszVHDOld, const wchar_t* pwszVHDNew, 
                          D2D_ENCRYPTION_INFO* pEncInfo, bool bRONew)
{
    HALogMsg(L"CVDiskDiffImp::Init2, oldvhd = %s, newvhd = %s, read_only = %d", 
        pwszVHDOld ? pwszVHDOld : L"NULL",
        pwszVHDNew ? pwszVHDNew : L"NULL",
        bRONew);

    if (pwszVHDNew == NULL)
        return E_INVALIDARG;

    UnInit();

    // loop find the pwszVHDOld by pwszVHDNew
    int nRet = 0;
    bool bFoundParent = false;
    wstring wstrToOpen = pwszVHDNew;
    bool bIsD2D = IsD2DFile(pwszVHDNew);
    // If read all VHD in the parent-child VHD list
    bool bReadAllVHD = (pwszVHDOld == NULL || wcslen(pwszVHDOld) == 0);  
    while (true)
    {
        IVHDFile* pVHDFile = NULL;
        if (!bRONew && _wcsicmp(wstrToOpen.c_str(), pwszVHDNew) == 0)
		{
            //nRet = CreateVHDFileEx(wstrToOpen.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, pEncInfo, &pVHDFile);
		}
        else
		{
            //nRet = CreateVHDFileEx(wstrToOpen.c_str(), GENERIC_READ, FILE_SHARE_READ, pEncInfo, &pVHDFile);
		}
        if (nRet)
        {
            if (nRet == ERROR_SHARING_VIOLATION)
            {
                HALogMsg(L"CVDiskDiffImp::Init2, try create IVHD with share_rw, file = %s", wstrToOpen.c_str());
                //nRet = CreateVHDFileEx(wstrToOpen.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, pEncInfo, &pVHDFile);
            }

            if (nRet)
            {
                HALogError(L"CVDiskDiffImp::Init2, create IVHD failed, path = %s, rc = %d", wstrToOpen.c_str(), nRet);
                break;
            }
        }

        m_vecVHDFile.push_back(pVHDFile);

        if (pVHDFile->GetVHDType() != VHD_Diff && pVHDFile->GetVHDType() != VHD_Diff2)
        {
            m_bContainFullVHD = true;
            bFoundParent = true; // if there has FULL VHD in the middle, do not read VHD older than this FULL.
            HALogMsg(L"CVDiskDiffImp::Init 2, found FULL vhd, path = %s", wstrToOpen.c_str());
            break;
        }

        // first use relative path
        // Get parent path and UUID
        // TODO: add check for parent's UUID
        bool bFoundByRel = false;
        nRet = GetParentPath(pVHDFile, bIsD2D, WIN_RELATIVE, wstrToOpen);
        if (nRet == 0)
        {
            bFoundByRel = true;
            HALogMsg(L"CVDiskDiffImp::Init2, path from REL is %s", wstrToOpen.c_str());

            if (!bReadAllVHD && _wcsicmp(wstrToOpen.c_str(), pwszVHDOld) == 0)
            {
                HALogMsg(L"CVDiskDiffImp::Init2, found the parent by REL");
                bFoundParent = true;
                break;
            }
        }

        if (!bFoundByRel || !FileExist(wstrToOpen.c_str())) //try absolute path
        {
            HALogMsg(L"CVDiskDiffImp::Init2, REL path file does NOT exist, try get ABS");
            nRet = GetParentPath(pVHDFile, bIsD2D, WIN_ABSOLUTE, wstrToOpen);
            if (nRet)
            {
                break;
            }

            HALogMsg(L"CVDiskDiffImp::Init2, path from ABS is %s", wstrToOpen.c_str());

            if (!bReadAllVHD && _wcsicmp(wstrToOpen.c_str(), pwszVHDOld) == 0)
            {
                HALogMsg(L"CVDiskDiffImp::Init, found the parent by ABS");
                bFoundParent = true;
                break;
            }

            if (!FileExist(wstrToOpen.c_str()))
            {
                HALogError(L"CVDiskDiffImp::Init2, REL path file does NOT exist");
                break;
            }
        }
    };

    if (!bFoundParent)
    {
        HALogError(L"CVDiskDiffImp::Init2, cannot found the parent");
        nRet = -5;
    }

    if (nRet)
    {
        UnInit();
    }

    return nRet;
}

bool CVDiskDiffImp::ContainFullVHD()
{
    return m_bContainFullVHD;
}

int CVDiskDiffImp::UnInit()
{
    if (!m_vecVHDFile.empty())
    {
        for (size_t i = 0; i<m_vecVHDFile.size(); i++)
        {
            m_vecVHDFile[i]->Release();
        }
        m_vecVHDFile.clear();
    }

    m_llCurrPosInSec = 0;
    m_llTotalSec = -1;

    if (m_pbParentData)
    {
        ::VirtualFree(m_pbParentData, 0, MEM_RELEASE);
        m_pbParentData = NULL;
    }

    if (m_pbImageData)
    {
        ::VirtualFree(m_pbImageData, 0, MEM_RELEASE);
        m_pbImageData = NULL;
    }

    if (m_pstImageBmp)
    {
        free (m_pstImageBmp);
        m_pstImageBmp = NULL;
    }
    if (m_pstParentBmp)
    {
        free (m_pstParentBmp);
        m_pstParentBmp = NULL;
    }

    m_llCacheStartOffset = 0;
    m_nCacheDataLen = 0;

    return 0;
}

int 
CVDiskDiffImp::Release()
{
    UnInit();
    delete this;
    return 0;
}

int 
CVDiskDiffImp::GetTotalSectors(/*OUT*/ __int64* pllTotal)
{
    if (pllTotal == NULL)
        return E_INVALIDARG;

    if (m_vecVHDFile.empty())
    {
        HALogError(L"CVDiskDiffImp::GetTotalSectors, empty VHD handle vector");
        return E_INVALIDARG;
    }


    IVHDFile* pVHDFile = m_vecVHDFile[0];
    if (pVHDFile == NULL)
    {
        HALogError(L"CVDiskDiffImp::GetTotalSectors, NULL pVHDFile in vector");
        return E_INVALIDARG;
    }

    __int64 llSize = 0;
    if (int nRet = pVHDFile->GetDiskSize(llSize))
    {
        HALogError(L"CVDiskDiffImp::GetTotalSectors, IVHDFile->GetDiskSize failed, rc = %d", nRet);
        return nRet;
    }

    *pllTotal = llSize/SECTOR_SIZE;
    return 0;
}

int 
CVDiskDiffImp::GetSectorSize(/*OUT*/ int* pnSize)
{
    if (pnSize == NULL) return E_INVALIDARG;
    *pnSize = SECTOR_SIZE;
    return 0;
}

int 
CVDiskDiffImp::GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap)
{
    int nRet = 0;

    do
    {
        if (m_vecVHDFile.empty())
        {
            HALogError(L"CVDiskDiffImp::GetBitmap, empty VHD handle vector");
            nRet = E_INVALIDARG;
            break;
        }

        IVHDFile* pVHDFile = m_vecVHDFile[0];
        ATLASSERT(pVHDFile != NULL);
        nRet = pVHDFile->GetBitmap(liStartingSec, nBufLen, pBitmap);
        if (nRet)
        {
            HALogError(L"CVDiskDiffImp::GetBitmap, failed, file = %s, rc = %d", 
                pVHDFile->GetFilePath(), nRet);
            break;
        }

        // If only 1 VHD file, just break and return
        if (m_vecVHDFile.size() == 1)
            break;

        // Get bitmap from other VHDs, operator |
        ST_VDISK_BITMAP_BUFFER* pBitmapDiff = (ST_VDISK_BITMAP_BUFFER*)malloc(
            offsetof(ST_VDISK_BITMAP_BUFFER, Buffer) + nBufLen);
        if (pBitmapDiff == NULL)
        {
            HALogError(L"CVDiskDiffImp::GetBitmap, alloc mem failed");
            nRet = E_OUTOFMEMORY;
            break;
        }
        for (size_t i = 1; i<m_vecVHDFile.size(); i++)
        {
            pBitmapDiff->StartingSec.QuadPart = 0;
            pBitmapDiff->BitmapSize.QuadPart = 0;
            pVHDFile = m_vecVHDFile[i];
            nRet = pVHDFile->GetBitmap(liStartingSec, nBufLen, pBitmapDiff);
            if (nRet)
            {
                HALogError(L"CVDiskDiffImp::GetBitmap for diff, failed, file = %s, rc = %d", 
                    pVHDFile->GetFilePath(), nRet);
                break; //break for
            }
            ATLASSERT(pBitmapDiff->StartingSec.QuadPart == pBitmap->StartingSec.QuadPart);
            ATLASSERT(pBitmapDiff->BitmapSize.QuadPart == pBitmap->BitmapSize.QuadPart);
            for (__int64 j = 0; j< (pBitmap->BitmapSize.QuadPart+7)/8; j++)
                pBitmap->Buffer[j] |= pBitmapDiff->Buffer[j];
        }

        if (pBitmapDiff)
        {
            free (pBitmapDiff);
            pBitmapDiff = NULL;
        }
    } while (0);

    return nRet;
}

int
CVDiskDiffImp::Seek(IN __int64 llOffsetInSec, OUT __int64* pllNewPosInSec, IN int nMethod)
{
    if (nMethod != VDISK_SEEK_BEGIN && nMethod != VDISK_SEEK_CURR && nMethod != VDISK_SEEK_END)
        return E_INVALIDARG;

    if (m_vecVHDFile.empty())
    {
        HALogError(L"CVDiskDiffImp::Seek, empty vhd file list");
        return E_INVALIDARG;
    }

    __int64 llNewPos = 0;

    if (nMethod == VDISK_SEEK_BEGIN)
        llNewPos = llOffsetInSec;
    else if (nMethod == VDISK_SEEK_CURR)
        llNewPos = m_llCurrPosInSec + llOffsetInSec;
    else
    {
        int nRet = 0;
        __int64 llTotal = 0;
        nRet = m_vecVHDFile[0]->GetDiskSize(llTotal); //total bytes
        if (nRet)
        {
            HALogError(L"CVDiskDiffImp::Seek, Get total sectors failed, rc = %d", nRet);
            return nRet;
        }
        llTotal  = llTotal/SECTOR_SIZE; //in sector
        llNewPos = llTotal - llOffsetInSec;
    }

    if (llNewPos < 0)
    {
        HALogError(L"CVDiskDiffImp::Seek, before the beginning of file, pos = %I64d", llNewPos);
        return ERROR_NEGATIVE_SEEK;
        // llNewPos = 0;
    }
    //else if (llNewPos > m_llTotalSectors)
    //    llNewPos = m_llTotalSectors;

    m_llCurrPosInSec = llNewPos;
    if (pllNewPosInSec != NULL)
        *pllNewPosInSec = llNewPos;

    HALogDebug(  L"CVDiskDiffImp::Seek::Seek to sector 0x%016I64X", m_llCurrPosInSec);

    return 0;
}

int
CVDiskDiffImp::ReadBlockIntoCache(__int64 llStartOffset)
{
    if (llStartOffset % READ_BLOCK_SIZE)
    {
        HALogError(L"::Read::starting offset is not READ_BLOCK_SIZE aligned, off = %I64d", llStartOffset);
        return -1;
    }

    int nRet = 0;

    if (m_pstImageBmp == NULL)
    {
        m_pstImageBmp = (ST_VDISK_BITMAP_BUFFER*)malloc(
            offsetof(ST_VDISK_BITMAP_BUFFER, Buffer) + (READ_BLOCK_SIZE/SECTOR_SIZE +7)/8);
    }

    if (m_pbImageData == NULL)
    {
        m_pbImageData = (char*)VirtualAlloc(NULL, READ_BLOCK_SIZE, MEM_COMMIT, PAGE_READWRITE);
        if (m_pbImageData == NULL)
        {
            HALogError(L"::Read::Alloc Image buffer failed");
            nRet = E_OUTOFMEMORY;
            return nRet;
        }
    }

    ZeroMemory(m_pbImageData, READ_BLOCK_SIZE);

    do
    {
        //
        // read from the nearest child
        //
        IVHDFile* pVHDFile = m_vecVHDFile[0];
        LARGE_INTEGER li;
        li.QuadPart = llStartOffset/SECTOR_SIZE;
        nRet = pVHDFile->GetBitmap(li, (READ_BLOCK_SIZE/SECTOR_SIZE +7)/8, m_pstImageBmp);
        if (nRet)
        {
            HALogError(L"::Read::Read first disk's bitmap failed, rc = %d, file = %s", nRet,
                pVHDFile->GetFilePath());
            break;
        }

        DWORD dwRead = 0;   
        nRet = pVHDFile->Read(m_pbImageData, READ_BLOCK_SIZE, &dwRead, llStartOffset);
        if (nRet)
        {
            HALogError(L"::Read::Read first disk's data failed, rc = %d, file = %s", nRet,
                pVHDFile->GetFilePath());
            break;
        }
        m_nCacheDataLen = dwRead;
        m_llCacheStartOffset = llStartOffset;

        // If no parent, directly break
        if (m_vecVHDFile.size() <= 1)
            break;

        if (m_pstParentBmp == NULL)
        {
            m_pstParentBmp = (ST_VDISK_BITMAP_BUFFER*)malloc(
                offsetof(ST_VDISK_BITMAP_BUFFER, Buffer) + (READ_BLOCK_SIZE/SECTOR_SIZE +7)/8);
        }

        if (m_pbParentData == NULL)
        {
            m_pbParentData = (char*)VirtualAlloc(NULL, READ_BLOCK_SIZE, MEM_COMMIT, PAGE_READWRITE);
            if (m_pbParentData == NULL)
            {
                HALogError(L"::Read::Alloc parent data buffer failed");
                nRet = E_OUTOFMEMORY;
                break;
            }
        }

        //
        // Read from parent disk
        //
        for (size_t i = 1; i<m_vecVHDFile.size(); i++)
        {
            bool bHaveReadParent = false;   // optimize read pfm
            IVHDFile* pVHDFile = m_vecVHDFile[i];
            LARGE_INTEGER li;
            li.QuadPart = llStartOffset/SECTOR_SIZE;
            nRet = pVHDFile->GetBitmap(li, (READ_BLOCK_SIZE/SECTOR_SIZE +7)/8, m_pstParentBmp);
            if (nRet)
            {
                HALogError(L"::Read::Read parent disk's bitmap failed, rc = %d, file = %s", nRet,
                    pVHDFile->GetFilePath());
                break;
            }
            ATLASSERT(m_pstParentBmp->StartingSec.QuadPart == llStartOffset);

            // <<comment this block for optimize read pfm
            //DWORD dwRead = 0;   
            //nRet = pVHDFile->Read(m_pbParentData, READ_BLOCK_SIZE, &dwRead, llStartOffset);
            //if (nRet)
            //{
            //    HALogError(L"::Read::Read parent disk's data failed, rc = %d, file = %s", nRet,
            //        pVHDFile->GetFilePath());
            //    break;
            //}
            //ATLASSERT(dwRead == m_CacheDataSize);
            // >>

            // loop find the bit that is 0 on child and 1 on parent
            // copy the sector represent by the bit from parent to image
            __int64 left = m_pstImageBmp->BitmapSize.QuadPart;
            BYTE chImage;
            BYTE chParent;
            for (__int64 j = 0; j<m_pstImageBmp->BitmapSize.QuadPart; j+= 8, left -= 8)
            {
                chImage = m_pstImageBmp->Buffer[j/8];
                chParent = m_pstParentBmp->Buffer[j/8];
                for (int k = 0; k< (left < 8 ? left : 8); k++)
                {
                    // add a optimize chImage == 0 && chParent == 0xFF
                    if ( (chImage == 0 || !IsBitSet(chImage, k))
                        && (chParent == (BYTE)0xFF || IsBitSet(chParent, k)) )
                    {
                        //<<optimize read pfm
                        if (!bHaveReadParent)
                        {
                            DWORD dwRead = 0;   
                            nRet = pVHDFile->Read(m_pbParentData, READ_BLOCK_SIZE, &dwRead, llStartOffset);
                            if (nRet)
                            {
                                HALogError(L"::Read::Read parent disk's data failed, rc = %d, file = %s", nRet,
                                    pVHDFile->GetFilePath());
                                break;
                            }
                            bHaveReadParent = true;
                            ATLASSERT(dwRead == READ_BLOCK_SIZE);

                        }//>>
                        memcpy_s(m_pbImageData + (j+k)*SECTOR_SIZE, SECTOR_SIZE,
                                 m_pbParentData + (j+k)*SECTOR_SIZE, SECTOR_SIZE);
                        VHDHelper::SetBit(m_pstImageBmp->Buffer, j+k, 1);
                    }
                }

                if (nRet) break; // optimize read pfm
            }
        } //end for

    } while (0);

     return nRet;
}

int
CVDiskDiffImp::Read(/*OUT*/ void* lpBuffer, /*IN*/ int nSecs2Read, 
                        /*OUT*/ int* pnSecsRead, /*IN*/ __int64 llStartingSec)
{
    int nRet = 0;

    if (m_vecVHDFile.empty())
    {
        HALogError(L"CVDiskDiffImp::Read::empty vhd handle list");
        return E_INVALIDARG;
    }

    if (lpBuffer == NULL && pnSecsRead == NULL && nSecs2Read == 0)
    {
        m_llCurrPosInSec = llStartingSec;
        HALogMsg(L"CVDiskDiffImp::Read::Seek to position 0x%016I64X", llStartingSec*0x200);
        return 0;
    }

    if (pnSecsRead == NULL)
    {
        HALogError(L"CVDiskDiffImp::Read::Input NULL pnSecsRead");
        return E_INVALIDARG;
    }

    *pnSecsRead = 0;
    //
    // Calc starting position
    //
    if (llStartingSec == -1)
    {
        llStartingSec = m_llCurrPosInSec;
        HALogDebug(  L"CVDiskDiffImp::read, pos in sector = 0x%016I64X", llStartingSec);
    }

    if (m_llTotalSec == -1)
    {
        __int64 llTotal = 0; //total sectors
        nRet = m_vecVHDFile[0]->GetDiskSize(llTotal); //total bytes
        if (nRet)
        {
            HALogError(L"CVDiskDiffImp::Read, Get total sectors failed, rc = %d", nRet);
            return nRet;
        }
        m_llTotalSec  = llTotal/SECTOR_SIZE; //in sector
    }

    if (llStartingSec >= m_llTotalSec)
    {
        HALogMsg(L"CVDiskDiffImp::Read::reach the end");
        return 0;
    }
    if (llStartingSec + nSecs2Read > m_llTotalSec)
        nSecs2Read = m_llTotalSec - llStartingSec;

    char* pOut = (char*)lpBuffer;
    //
    // Begin to read
    //
    __int64 llBytes2Read = nSecs2Read * SECTOR_SIZE;
    __int64 llStartOffset = llStartingSec * SECTOR_SIZE;

    while (llBytes2Read > 0)
    {
        // 1)first find in cache
        if (llStartOffset >= m_llCacheStartOffset 
            && llStartOffset < m_llCacheStartOffset + m_nCacheDataLen)
        {
            __int64 len = llBytes2Read;
            if (len > (m_llCacheStartOffset+m_nCacheDataLen-llStartOffset))
                len = m_llCacheStartOffset+m_nCacheDataLen-llStartOffset;
            memcpy(pOut, m_pbImageData+ (llStartOffset-m_llCacheStartOffset), len);
            llBytes2Read -= len;
            llStartOffset += len;
            pOut += len;
        }
        else
        {
            // 2)If not in cache, read data to cache
            nRet = ReadBlockIntoCache( (llStartOffset/READ_BLOCK_SIZE) * READ_BLOCK_SIZE );
            if (nRet)
            {
                HALogError(L"CVDiskDiffImp::Read::ReadBlockIntoCache failed, rc = %d, \
                            startoffset = %I64d", nRet, llStartOffset);
                break;
            }
        }
    }

    if (nRet)
        *pnSecsRead = 0;
    else
        *pnSecsRead = (int)((pOut - (char*)lpBuffer)/SECTOR_SIZE);
    
    m_llCurrPosInSec = llStartingSec + (*pnSecsRead);

    return nRet;
}


int
CVDiskDiffImp::GetDiskGeometry(DISK_GEOMETRY& geometry)
{
    if (m_vecVHDFile.empty())
    {
        HALogError(L"CVDiskDiffImp::GetDiskGeometry, empty VHD handle vector");
        return E_INVALIDARG;
    }


    IVHDFile* pVHDFile = m_vecVHDFile[0];
    if (pVHDFile == NULL)
    {
        HALogError(L"CVDiskDiffImp::GetDiskGeometry, NULL pVHDFile in vector");
        return E_INVALIDARG;
    }

    return pVHDFile->GetDiskGeometry(geometry);
}

void
CVDiskDiffImp::FlushCache()
{
    m_llCacheStartOffset = 0;
    m_nCacheDataLen = 0;
}
