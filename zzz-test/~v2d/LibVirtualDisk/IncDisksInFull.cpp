#include "StdAfx.h"
#include "IncDisksInFull.h"
#include "HALog.h"

static const int SECTOR_SIZE = 512;
static const int BITMAP_CACHE_SIZE = 512;
inline BOOL IsBitSet(unsigned char* pBuf, int nBit)
{
   unsigned char cByte = pBuf[nBit/8];
   int nBitLeft = nBit%8;
   unsigned char cMask = 0;
   cMask = 1<<(7-nBitLeft);

   return cByte & cMask;
}

CIncDisksInFull::CIncDisksInFull(void)
: m_pFullVhd(NULL)
, m_pDiffBitmap(NULL)
, m_wstrFullVhd(L"")
, m_llCurrPosInSec(0)
, m_llTotalSec(-1)
, m_pstBmpIncs(NULL)
{
}

CIncDisksInFull::~CIncDisksInFull(void)
{
    UnInit();
}

int 
CIncDisksInFull::Init(const wchar_t* pwszFullVhd, IDiskBmpReader* pDiffBitmap, D2D_ENCRYPTION_INFO* pEncInfo)
{
    int nRet = 0;
    if (pwszFullVhd == NULL || pwszFullVhd[0] == L'\0' || pDiffBitmap == NULL)
        return E_INVALIDARG;
    
    do
    {
    //nRet = CreateVHDFileEx(pwszFullVhd, GENERIC_READ, FILE_SHARE_READ, pEncInfo, &m_pFullVhd);
    if (nRet)
    {
        if (nRet == ERROR_SHARING_VIOLATION)
        {
            HALogWarning(L"CIncDisksInFull::Init, try create IVhdFile with share rw, path = %s", pwszFullVhd);
           // nRet = CreateVHDFileEx(pwszFullVhd, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, pEncInfo, &m_pFullVhd);
        }

        if (nRet)
        {
            HALogError(L"CIncDisksInFull::Init, create IVHD failed, file = %s, rc = %d", pwszFullVhd, nRet);
            break;
        }
    }

    int nLen = offsetof(ST_VDISK_BITMAP_BUFFER, Buffer) + BITMAP_CACHE_SIZE;
    if (m_pstBmpIncs == NULL)
    {
        m_pstBmpIncs = (ST_VDISK_BITMAP_BUFFER*)malloc(nLen);
        if (m_pstBmpIncs == NULL)
        {
            nRet = E_OUTOFMEMORY;
            HALogError(L"CIncDisksInFull::Init, alloc mem for bmpcache in full failed");
            break;
        }
    }
    ZeroMemory(m_pstBmpIncs, nLen);

    m_pDiffBitmap = pDiffBitmap;
    m_wstrFullVhd = pwszFullVhd;
    m_llCurrPosInSec = 0;
    } while (0);

    HALogMsg(L"CIncDisksInFull init , full vhd = %s, rc = %d", pwszFullVhd, nRet);

    return nRet;
}

void
CIncDisksInFull::UnInit()
{
    m_pDiffBitmap = NULL;
    if (m_pFullVhd != NULL)
        m_pFullVhd->Release();
    m_pFullVhd = NULL;

    if (m_pstBmpIncs != NULL)
    {
        free(m_pstBmpIncs);
        m_pstBmpIncs = NULL;
    }
}

int 
CIncDisksInFull::Release()
{
    delete this;
    return 0;
}

int 
CIncDisksInFull::GetTotalSectors(/*OUT*/ __int64* pllTotal)
{
    return m_pDiffBitmap->GetTotalSectors(pllTotal);
}

int 
CIncDisksInFull::GetSectorSize(/*OUT*/ int* pnSize)
{
    if (pnSize == NULL) return E_INVALIDARG;

    DISK_GEOMETRY stDiskGeo = {0};
    int nRet = m_pFullVhd->GetDiskGeometry(stDiskGeo);
    if (nRet)
    {
        HALogError(L"CIncDisksInFull, get disk geometry failed, rc = %d, vhd = %s", nRet, m_wstrFullVhd.c_str());
        return nRet;
    }
    *pnSize = stDiskGeo.BytesPerSector;

    return 0;
}

int 
CIncDisksInFull::GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap)
{
    return m_pDiffBitmap->GetBitmap(liStartingSec, nBufLen, pBitmap);
}

int 
CIncDisksInFull::Seek(/*IN*/ __int64 llOffsetInSec, /*OUT*/ __int64* pllNewPosInSec, /*IN*/ int nMethod)
{
    if (nMethod != VDISK_SEEK_BEGIN && nMethod != VDISK_SEEK_CURR && nMethod != VDISK_SEEK_END)
    {
        HALogError(L"CIncDisksInFull::Seek, invalid seek method (%d)", nMethod);
        return E_INVALIDARG;
    }

    if (m_pFullVhd == NULL)
    {
        HALogError(L"CIncDisksInFull::Seek, NULL vhd pointer");
        return E_POINTER;
    }

    __int64 llNewPos = 0;

    if (nMethod == VDISK_SEEK_BEGIN)
        llNewPos = llOffsetInSec;
    else if (nMethod == VDISK_SEEK_CURR)
        llNewPos = m_llCurrPosInSec + llOffsetInSec;
    else
    {
        if (m_llTotalSec == -1)
        {
            __int64 llTotal = 0;
            if (int nRet = m_pDiffBitmap->GetTotalSectors(&llTotal)) //total sectors
            {
                HALogError(L"CIncDisksInFull::Seek, Get total sectors failed, rc = %d", nRet);
                return nRet;
            }
            m_llTotalSec = llTotal;
        }
        llNewPos = m_llTotalSec - llOffsetInSec;
    }

    if (llNewPos < 0)
    {
        HALogError(L"CIncDisksInFull::Seek, before the beginning of file, pos = %I64d", llNewPos);
        return ERROR_NEGATIVE_SEEK;
    }

    m_llCurrPosInSec = llNewPos;
    if (pllNewPosInSec != NULL)
        *pllNewPosInSec = llNewPos;

    HALogDebug(  L"CIncDisksInFull::Seek::Seek to sector 0x%I64X", m_llCurrPosInSec);

    return 0;
}

int 
CIncDisksInFull::CheckBitVal(__int64 llSec, BOOL& bSet)
{
    if (llSec < m_pstBmpIncs->StartingSec.QuadPart
        || llSec >= m_pstBmpIncs->StartingSec.QuadPart + m_pstBmpIncs->BitmapSize.QuadPart)
    {
        //cache mismatch, need to update
        LARGE_INTEGER li;
        li.QuadPart = (llSec/8)*8; // 8 aligned
        int nRet = this->GetBitmap(li, BITMAP_CACHE_SIZE, m_pstBmpIncs);
        if (nRet)
        {
            HALogError(L"CIncDisksInFull::CheckBit, get bitmap failed, rc = %d, startsec = %I64X",
                nRet, li.QuadPart);
            return nRet;
        }

        if (llSec < m_pstBmpIncs->StartingSec.QuadPart
            || llSec>= m_pstBmpIncs->StartingSec.QuadPart + m_pstBmpIncs->BitmapSize.QuadPart)
        {
            HALogError(L"CIncDisksInFull::CheckBit, still miss, sec = 0x%I64X, start = %I64X, size = 0x%I64X",
                llSec, m_pstBmpIncs->StartingSec.QuadPart, m_pstBmpIncs->BitmapSize.QuadPart);
            return nRet;
        }
    }

    bSet = IsBitSet(m_pstBmpIncs->Buffer, (int)(llSec - m_pstBmpIncs->StartingSec.QuadPart));

    return 0;
}

int 
CIncDisksInFull::Read(/*OUT*/ void* lpBuffer, /*IN*/ int nSecs2Read, 
        /*OUT*/ int* pnSecsRead, /*IN*/ __int64 llStartingSec)
{
    int nRet = 0;

    if (m_pFullVhd == NULL)
    {
        HALogError(L"CIncDisksInFull::Read::empty vhd handle list");
        return E_INVALIDARG;
    }

    if (lpBuffer == NULL || pnSecsRead == NULL)
    {
        m_llCurrPosInSec = llStartingSec;
        HALogMsg(L"CIncDisksInFull::Read::input NULL lpBuffer or pnSecsRead");
        return E_INVALIDARG;
    }

    *pnSecsRead = 0;
    //
    // Calc starting position
    //
    if (llStartingSec == -1)
    {
        llStartingSec = m_llCurrPosInSec;
        HALogDebug(  L"CIncDisksInFull::read, pos in sector = 0x%I64X", llStartingSec);
    }

    if (m_llTotalSec == -1)
    {
        __int64 llTotal = 0; //total sectors
        nRet = m_pDiffBitmap->GetTotalSectors(&llTotal);
        if (nRet)
        {
            HALogError(L"CIncDisksInFull::Read, Get total sectors failed, rc = %d", nRet);
            return nRet;
        }
        m_llTotalSec = llTotal;
    }

    if (llStartingSec >= m_llTotalSec)
    {
        HALogMsg(L"CIncDisksInFull::Read::reach the end");
        return 0;
    }

    if (llStartingSec + nSecs2Read > m_llTotalSec)
        nSecs2Read = (int)(m_llTotalSec - llStartingSec);

    //first zero the sector if its bitmap is 0.
    ZeroMemory(lpBuffer, nSecs2Read*SECTOR_SIZE);
    BOOL bSet = FALSE;
    unsigned long cbRead = 0;
    for (unsigned long i = 0; i<nSecs2Read; i++)
    {
        nRet = CheckBitVal(llStartingSec+i, bSet);
        if (nRet)
        {
            HALogError(L"CIncDisksInFull, check bit val failed, starting_sec = 0x%I64X", llStartingSec+i);
            break;
        }

        if (bSet)
        {
            nRet = m_pFullVhd->Read((char*)lpBuffer+i*SECTOR_SIZE, SECTOR_SIZE, &cbRead, (llStartingSec+i)*SECTOR_SIZE);
            if (nRet)
            {
                HALogError(L"CIncDisksInFull, full Vhd read failed, starting_sec = 0x%I64X, sec_to_read = %d",
                    llStartingSec+i, 1);
                break;
            }
        }
    }

    if (nRet == 0)
    {
        *pnSecsRead = nSecs2Read;
        m_llCurrPosInSec = llStartingSec + *pnSecsRead;
    }


    return nRet;
}

