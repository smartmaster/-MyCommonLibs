#include "stdafx.h"
#include "VHDImage.h"
#include "HALog.h"
#include "vhddef.h"
#include "VHDFileWImp.h"


CVHDImage::CVHDImage()
: m_bROLast(false)
, m_pReader(NULL)
, m_pLastVhdW(NULL)
, m_llCurrPosInSec(0)
{
};

CVHDImage::~CVHDImage()
{
    if (m_pReader)
    {
        m_pReader->Release();
        m_pReader = NULL;
    }

    if (m_pLastVhdW)
    {
        m_pLastVhdW->Release();
        m_pLastVhdW = NULL;
    }
    m_llCurrPosInSec = 0;
};

int CVHDImage::Init(const wchar_t* pwszPath, bool bROLast)
{
    if (pwszPath == NULL || pwszPath[0] == L'\0')
        return E_INVALIDARG;

    int nRet = 0;

    do
    {
        m_bROLast = bROLast;
        m_pReader = new CVDiskDiffImp;
        nRet = m_pReader->Init(NULL, pwszPath, bROLast);
        if (nRet)
        {
            HALogError(L"CreateVDiskDiffEx::Init CVDiskVHD failed, rc = %d", nRet);
            break;
        }

        if (bROLast)
            break;

        ST_CREATE_PARMS stParms = {0};
        stParms.dwCreateDisp = OPEN_ALWAYS;
        CDynVHDFileW* pWriter = new CDynVHDFileW;
        nRet = pWriter->Init(pwszPath, &stParms, NULL); //TODO: here maybe a explicitly Open() is better
        if (nRet)
        {
            pWriter->Release();
            HALogError(L"CreateVDiskDiffEx::Init CVDiskVHD failed, rc = %d", nRet);
            break;
        }
        m_pLastVhdW = pWriter;
        
    } while (0);

    if (nRet)
    {
        if (m_pReader)
            m_pReader->Release();
        m_pReader = NULL;
        if (m_pLastVhdW)
            m_pLastVhdW->Release();
        m_pLastVhdW = NULL;
    }

    return nRet;
}

int CVHDImage::Release()
{
    delete this;
    return 0;
}

int CVHDImage::GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
                      /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap)
{
    return m_pReader->GetBitmap(liStartingSec, nBufLen, pBitmap);
}


int CVHDImage::GetTotalSectors(/*OUT*/ __int64* pllTotal)
{
    return m_pReader->GetTotalSectors(pllTotal);
}

int CVHDImage::GetSectorSize(/*OUT*/ int* pnSize)
{
    return m_pReader->GetSectorSize(pnSize);
}

int CVHDImage::GetDiskGeometry(DISK_GEOMETRY& geometry)
{
    return m_pReader->GetDiskGeometry(geometry);
}

int CVHDImage::Seek(__int64 llOffsetInSec, __int64* pllNewPosInSec, int nMethod)
{
    if (nMethod != VDISK_SEEK_BEGIN && nMethod != VDISK_SEEK_CURR && nMethod != VDISK_SEEK_END)
        return E_INVALIDARG;

    __int64 llNewPos = 0;
    bool bReadSeekFail = false;
    bool bWriteSeekFail = false;

    int nRet = m_pReader->Seek(llOffsetInSec, &llNewPos, nMethod);
    if (nRet != 0)
    {
        bReadSeekFail = true;
        HALogError(L"CVHDImage::read seek failed, off_in = %I64d, method = %d, rc = %d",
            llOffsetInSec, nMethod, nRet);
    }

    if (!bReadSeekFail && m_pLastVhdW != NULL)
    {
        __int64 cbOffset = 0;
        nRet = m_pLastVhdW->Seek(llOffsetInSec*VHD_SECTOR_SIZE, &cbOffset, nMethod);
        if (nRet)
        {
            bWriteSeekFail = true;
            HALogError(L"CVHDImage::write seek failed, off_in = %I64d, method = %d, rc = %d",
                llOffsetInSec, nMethod, nRet);
        }
        llNewPos = cbOffset/VHD_SECTOR_SIZE;
    }

    //restore if fail
    if (bReadSeekFail)
    {
        int nTemp = m_pReader->Seek(m_llCurrPosInSec, &llNewPos, VDISK_SEEK_BEGIN);
        if (nTemp)
        {
            HALogError(L"CVHDImage::Seek, restore read pointer failed, rc = %d, off_to_restore = %I64d",
                nTemp, m_llCurrPosInSec);
        }
    }
    if (bWriteSeekFail)
    {
        __int64 cbOffset = 0;
        int nTemp = m_pLastVhdW->Seek(m_llCurrPosInSec*VHD_SECTOR_SIZE, &cbOffset, VDISK_SEEK_BEGIN);
        if (nTemp)
        {
            HALogError(L"CVHDImage::Seek, restore write pointer failed, rc = %d, off_to_restore = %I64d",
                nTemp, m_llCurrPosInSec);
        }
        llNewPos = cbOffset/VHD_SECTOR_SIZE;
    }

    m_llCurrPosInSec = llNewPos;
    if (pllNewPosInSec != NULL)
        *pllNewPosInSec = llNewPos;

    return nRet;

}

int CVHDImage::Read(/*OUT*/ void* lpBuffer, /*IN*/ int nSecs2Read, 
                 /*OUT*/ int* pnSecsRead, /*IN*/ __int64 llStartingSec)
{
    int nRet = 0;

    if (llStartingSec == -1)
    {
        llStartingSec = m_llCurrPosInSec;
        HALogDebug(  L"CVHDImage::read, pos in sector = 0x%016I64X", llStartingSec);
    }

    nRet = m_pReader->Read(lpBuffer, nSecs2Read, pnSecsRead, llStartingSec);
    m_pReader->Seek(0, &m_llCurrPosInSec, VDISK_SEEK_CURR);
    

    return nRet;
}

int CVHDImage::Write(const void* lpBuffer, int nSecs2Write, int* pnSecsWritten, __int64 llStartingSec)
{
    if (m_pLastVhdW == NULL)
        return E_POINTER;

    if (pnSecsWritten != NULL)
        *pnSecsWritten = 0;
    if (llStartingSec == -1)
        llStartingSec = m_llCurrPosInSec;

    if (nSecs2Write > 0xFFFFFFFF/VHD_SECTOR_SIZE)
        nSecs2Write = 0xFFFFFFFF/VHD_SECTOR_SIZE;
    unsigned long cbWritten = 0;
    int nRet = m_pLastVhdW->Write((const char*)lpBuffer, nSecs2Write*VHD_SECTOR_SIZE, &cbWritten, llStartingSec*VHD_SECTOR_SIZE);
    if (nRet)
    {
        HALogError(L"CVHDImage::Write failed, nSecs2Write = %I64d, llStartSec = %I64d, rc = %d",
            nSecs2Write, llStartingSec, nRet);
    }

    if (pnSecsWritten)
        *pnSecsWritten = cbWritten/VHD_SECTOR_SIZE;

    m_pReader->FlushCache(); // to protect the case that "sth written to the vhd file does not be valid by read" not happen

    __int64 cbOffset = 0;
    m_pLastVhdW->Seek(0, &cbOffset, VDISK_SEEK_CURR);
    m_llCurrPosInSec = cbOffset/VHD_SECTOR_SIZE;


    return nRet;
}
