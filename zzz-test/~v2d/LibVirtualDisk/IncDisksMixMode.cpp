#include "StdAfx.h"
#include "IncDisksMixMode.h"
#include "VDiskDiffImp.h"
#include "HALog.h"
#include <atlbase.h>

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

CIncDisksMixMode::CIncDisksMixMode(void)
: m_pVDiffInFull(NULL)
, m_pVDiffOut(NULL)
, m_wstrFullVhd(L"")
, m_wstrIncVhd(L"")
, m_llCurrPosInSec(0)
, m_llTotalSec(-1)
, m_pstBmpOut(NULL)
, m_pstBmpInFull(NULL)
, m_bContainFullInDiffOut(false)
{
}

CIncDisksMixMode::~CIncDisksMixMode(void)
{
    UnInit();
}

int 
CIncDisksMixMode::Init(const wchar_t* pwszFullVhd, IDiskBmpReader* pDiffBitmap,
             const wchar_t* pwszIncVhd, D2D_ENCRYPTION_INFO* pEncInfo)
{
    if (pwszFullVhd == NULL || pwszFullVhd[0] == L'\0'
        || pDiffBitmap == NULL
        || pwszIncVhd == NULL || pwszIncVhd[0] == L'\0')
        return E_INVALIDARG;

    m_wstrFullVhd = pwszFullVhd;
    m_wstrIncVhd = pwszIncVhd;
    m_llCurrPosInSec = 0;
    m_llTotalSec = -1;
    
    HALogMsg(L"CIncDisksMixMode::Init, begin, full = %s, inc = %s", pwszFullVhd, pwszIncVhd);

    int nRet = 0;
    do
    {
        //Create DiffOut
        //nRet = CreateVDiskDiffEx(pwszFullVhd, pwszIncVhd, &m_pVDiffOut);
        CVDiskDiffImp* pDiff = new CVDiskDiffImp;
        nRet = pDiff->Init(pwszFullVhd, pwszIncVhd, pEncInfo);
        if (nRet)
        {
            HALogError(L"CIncDisksMixMode::Init, create disk diff out of full failed, rc = %d", nRet);
            pDiff->Release();
            pDiff = NULL;
            break;
        }
        m_pVDiffOut = pDiff;

        int nLen = offsetof(ST_VDISK_BITMAP_BUFFER, Buffer) + BITMAP_CACHE_SIZE;
        if (m_pstBmpOut == NULL)
        {
            m_pstBmpOut = (ST_VDISK_BITMAP_BUFFER*)malloc(nLen);
            if (m_pstBmpOut == NULL)
            {
                HALogError(L"CIncDisksMixMode::Init, alloc mem for bmpcache out of full failed");
                break;
            }
        }
        ZeroMemory(m_pstBmpOut, nLen);

        if (pDiff->ContainFullVHD())
        {
            m_bContainFullInDiffOut = true;
            HALogMsg(L"CIncDisksMixMode::Init, contains full in diffout, do not create diff_in_full");
            break;
        }

        // Create DiffInFull
        nRet = GetIncDisksFromFull(pwszFullVhd, pDiffBitmap, pEncInfo, &m_pVDiffInFull);
        if (nRet)
        {
            HALogError(L"CIncDisksMixMode::Init, get Inc from full failed, rc = %d", nRet);
            break;
        }

        if (m_pstBmpInFull == NULL)
        {
            m_pstBmpInFull = (ST_VDISK_BITMAP_BUFFER*)malloc(nLen);
            if (m_pstBmpInFull == NULL)
            {
                HALogError(L"CIncDisksMixMode::Init, alloc mem for bmpcache in full failed");
                break;
            }
        }
        ZeroMemory(m_pstBmpInFull, nLen);
    } while (0);

    if (nRet != 0)
    {
        UnInit();
    }

    return nRet;
}

void
CIncDisksMixMode::UnInit()
{
    if (m_pVDiffInFull)
    {
        m_pVDiffInFull->Release();
        m_pVDiffInFull = NULL;
    }

    if (m_pVDiffOut)
    {
        m_pVDiffOut->Release();
        m_pVDiffOut = NULL;
    }

    if (m_pstBmpOut)
    {
        free (m_pstBmpOut);
        m_pstBmpOut = NULL;
    }

    if (m_pstBmpInFull)
    {
        free (m_pstBmpInFull);
        m_pstBmpInFull = NULL;
    }
    m_llCurrPosInSec = 0;
    m_llTotalSec = -1;
}

int 
CIncDisksMixMode::Release()
{
    delete this;
    return 0;
}

int 
CIncDisksMixMode::GetTotalSectors(/*OUT*/ __int64* pllTotal)
{
    return m_pVDiffOut->GetTotalSectors(pllTotal);
}

int 
CIncDisksMixMode::GetSectorSize(/*OUT*/ int* pnSize)
{
    return m_pVDiffOut->GetSectorSize(pnSize);
}

int 
CIncDisksMixMode::GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap)
{
    // combine the bitmap of In and Out
    int nRet = 0;
    ST_VDISK_BITMAP_BUFFER* pBitmapInFull = NULL;

    do
    {
        if (m_pVDiffOut == NULL
            || (!m_bContainFullInDiffOut && m_pVDiffInFull == NULL ))
        {
            HALogError(L"CIncDisksMixMode::GetBitmap, NULL pointer, vdiff_in = %p, vdiff_out = %p, bContainFullInOut = %s",
                m_pVDiffInFull, m_pVDiffOut, m_bContainFullInDiffOut ? L"true" : L"false");
            nRet = E_POINTER;
            break;
        }

        // first get bitmap by VDiffOut
        nRet = m_pVDiffOut->GetBitmap(liStartingSec, nBufLen, pBitmap);
        if (nRet)
        {
            HALogError(L"CIncDisksMixMode::GetBitmap by diff_out failed, rc = %d", nRet);
            break;
        }

        if (m_bContainFullInDiffOut)
            break;

        // then get bitmap from the vhd's in full, operator |
        ST_VDISK_BITMAP_BUFFER* pBitmapInFull = (ST_VDISK_BITMAP_BUFFER*)malloc(
            offsetof(ST_VDISK_BITMAP_BUFFER, Buffer) + nBufLen);
        if (pBitmapInFull == NULL)
        {
            HALogError(L"CIncDisksMixMode::GetBitmap by diff_in, alloc mem failed");
            nRet = E_OUTOFMEMORY;
            break;
        }

        pBitmapInFull->StartingSec.QuadPart = 0;
        pBitmapInFull->BitmapSize.QuadPart = 0;
        nRet = m_pVDiffInFull->GetBitmap(liStartingSec, nBufLen, pBitmapInFull);
        if (nRet)
        {
            HALogError(L"CIncDisksMixMode::GetBitmap by diff_in, failed, rc = %d", nRet);
            break; //break for
        }
        ATLASSERT(pBitmapInFull->StartingSec.QuadPart == pBitmap->StartingSec.QuadPart);
        ATLASSERT(pBitmapInFull->BitmapSize.QuadPart == pBitmap->BitmapSize.QuadPart);
        for (__int64 j = 0; j< (pBitmap->BitmapSize.QuadPart+7)/8; j++)
            pBitmap->Buffer[j] |= pBitmapInFull->Buffer[j];
    } while (0);

    if (pBitmapInFull)
    {
        free (pBitmapInFull);
        pBitmapInFull = NULL;
    }

    return nRet;
}

int 
CIncDisksMixMode::Seek(/*IN*/ __int64 llOffsetInSec, /*OUT*/ __int64* pllNewPosInSec, /*IN*/ int nMethod)
{
    if (nMethod != VDISK_SEEK_BEGIN && nMethod != VDISK_SEEK_CURR && nMethod != VDISK_SEEK_END)
    {
        HALogError(L"CIncDisksMixMode::Seek, invalid seek method (%d)", nMethod);
        return E_INVALIDARG;
    }

    if (m_pVDiffOut == NULL
        || (!m_bContainFullInDiffOut && m_pVDiffInFull == NULL ))
    {
        HALogError(L"CIncDisksMixMode::Seek, NULL vhd pointer, diff_in = %p, diff_out = %p",
            m_pVDiffInFull, m_pVDiffOut);
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
            if (int nRet = m_pVDiffOut->GetTotalSectors(&llTotal)) //total sectors
            {
                HALogError(L"CIncDisksMixMode::Seek, Get total sectors failed, rc = %d", nRet);
                return nRet;
            }
            m_llTotalSec = llTotal;
        }
        llNewPos = m_llTotalSec - llOffsetInSec;
    }

    if (llNewPos < 0)
    {
        HALogError(L"CIncDisksMixMode::Seek, before the beginning of file, pos = %I64d", llNewPos);
        return ERROR_NEGATIVE_SEEK;
    }

    m_llCurrPosInSec = llNewPos;
    if (pllNewPosInSec != NULL)
        *pllNewPosInSec = llNewPos;

    HALogDebug(  L"CIncDisksMixMode::Seek::Seek to sector 0x%I64X", m_llCurrPosInSec);

    return 0;
}

int 
CIncDisksMixMode::Read(/*OUT*/ void* lpBuffer, /*IN*/ int nSecs2Read, 
        /*OUT*/ int* pnSecsRead, /*IN*/ __int64 llStartingSec)
{
    int nRet = 0;


    if (m_pVDiffOut == NULL
        || (!m_bContainFullInDiffOut && m_pVDiffInFull == NULL ))
    {
        HALogError(L"CIncDisksMixMode::Read::NULL vhd pointer, diff_in = %p, diff_out = %p",
            m_pVDiffInFull, m_pVDiffOut);
        return E_POINTER;
    }

    if (lpBuffer == NULL || pnSecsRead == NULL)
    {
        m_llCurrPosInSec = llStartingSec;
        HALogMsg(L"CIncDisksMixMode::Read::input NULL lpBuffer or pnSecsRead");
        return E_INVALIDARG;
    }

    *pnSecsRead = 0;
    //
    // Calc starting position
    //
    if (llStartingSec == -1)
    {
        llStartingSec = m_llCurrPosInSec;
        HALogDebug(  L"CIncDisksMixMode::read, pos in sector = 0x%I64X", llStartingSec);
    }

    if (m_llTotalSec == -1)
    {
        __int64 llTotal = 0; //total sectors
        nRet = m_pVDiffOut->GetTotalSectors(&llTotal);
        if (nRet)
        {
            HALogError(L"CIncDisksMixMode::Read, diff_out Get total sectors failed, rc = %d", nRet);
            return nRet;
        }
        m_llTotalSec = llTotal;
    }

    if (llStartingSec >= m_llTotalSec)
    {
        HALogMsg(L"CIncDisksMixMode::Read::reach the end");
        return 0;
    }

    if (llStartingSec + nSecs2Read > m_llTotalSec)
        nSecs2Read = (int)(m_llTotalSec - llStartingSec);

    do
    {
    //
    // 1) read from the diff_out
    //
    int nSecsRead = 0;
    nRet = m_pVDiffOut->Read(lpBuffer, nSecs2Read, &nSecsRead, llStartingSec);
    if (nRet)
    {
        HALogError(L"CIncDisksMixMode::Read::diff_out read data failed, rc = %d", nRet);
        break;
    }

    if (m_bContainFullInDiffOut)
        break;

    //
    // 2) read from diff_in if bit is 0 at diff_out and is 1 at diff_in
    //
    BOOL bSet = false;
    int i = 0;
    for (i = 0; i<nSecs2Read; i++)
    {
        if (nRet = CheckBitVal_Out(llStartingSec+i, bSet))
            break;
        if (bSet) // bit set in diff_out
            continue;

        if (nRet = CheckBitVal_InFull(llStartingSec+i, bSet))
            break;
        if (bSet) //bit set in diff_infull, read data
        {
            nRet = m_pVDiffInFull->Read((char*)lpBuffer+i*SECTOR_SIZE, 1, &nSecsRead, llStartingSec+i);
            if (nRet)
            {
                HALogError(L"CIncDisksMixMode::Read::diff_in read failed, rc = %d, start_sec = %I64X",
                    nRet, llStartingSec+i);
                break;
            }
            if (nSecsRead == 0)
            {//only log a warning at now
                HALogWarning(L"CIncDisksMixMode::Read::diff_in read 0 sectors, start_sec = %I64X", llStartingSec+i);
            }
        }
    } // for

    *pnSecsRead = i;
    m_llCurrPosInSec = llStartingSec + *pnSecsRead;

    } while (0);


    return nRet;
}


int 
CIncDisksMixMode::CheckBitVal_InFull(__int64 llSec, BOOL& bSet)
{
    if (llSec < m_pstBmpInFull->StartingSec.QuadPart
        || llSec >= m_pstBmpInFull->StartingSec.QuadPart+m_pstBmpInFull->BitmapSize.QuadPart)
    {
        //cache mismatch, need to update
        LARGE_INTEGER li;
        li.QuadPart = (llSec/8)*8; // 8 aligned
        int nRet = m_pVDiffInFull->GetBitmap(li, BITMAP_CACHE_SIZE, m_pstBmpInFull);
        if (nRet)
        {
            HALogError(L"CIncDksMix::CheckBit_InFull, get bitmap failed, rc = %d, startsec = %I64X",
                nRet, li.QuadPart);
            return nRet;
        }

        if (llSec < m_pstBmpInFull->StartingSec.QuadPart
            || llSec>= m_pstBmpInFull->StartingSec.QuadPart + m_pstBmpInFull->BitmapSize.QuadPart)
        {
            HALogError(L"CIncDksMix::CheckBit_InFull, still miss, sec = 0x%I64X, start = %I64X, size = 0x%I64X",
                llSec, m_pstBmpInFull->StartingSec.QuadPart, m_pstBmpInFull->BitmapSize.QuadPart);
            return nRet;
        }
    }

    bSet = IsBitSet(m_pstBmpInFull->Buffer, (int)(llSec - m_pstBmpInFull->StartingSec.QuadPart));

    return 0;
}

int 
CIncDisksMixMode::CheckBitVal_Out(__int64 llSec, BOOL& bSet)
{
    if (llSec < m_pstBmpOut->StartingSec.QuadPart
        || llSec >= m_pstBmpOut->StartingSec.QuadPart + m_pstBmpOut->BitmapSize.QuadPart)
    {
        //cache mismatch, need to update
        LARGE_INTEGER li;
        li.QuadPart = (llSec/8)*8; // 8 aligned
        int nRet = m_pVDiffOut->GetBitmap(li, BITMAP_CACHE_SIZE, m_pstBmpOut);
        if (nRet)
        {
            HALogError(L"CIncDksMix::CheckBit_out, get bitmap failed, rc = %d, startsec = %I64X",
                nRet, li.QuadPart);
            return nRet;
        }

        if (llSec < m_pstBmpOut->StartingSec.QuadPart
            || llSec>= m_pstBmpOut->StartingSec.QuadPart + m_pstBmpOut->BitmapSize.QuadPart)
        {
            HALogError(L"CIncDksMix::CheckBit_out, still miss, sec = 0x%I64X, start = %I64X, size = 0x%I64X",
                llSec, m_pstBmpOut->StartingSec.QuadPart, m_pstBmpOut->BitmapSize.QuadPart);
            return nRet;
        }
    }

    bSet = IsBitSet(m_pstBmpOut->Buffer, (int)(llSec - m_pstBmpOut->StartingSec.QuadPart));

    return 0;
}
