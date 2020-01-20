#pragma once
#include "HAVhdUtility.h"
using namespace HaVhdUtility;
#include <vector>
using std::vector;

class CVDiskDiffImp : public IVDiskDiff
{
public:
    CVDiskDiffImp();
protected:
    virtual ~CVDiskDiffImp(void);
    int UnInit();
    int ReadBlockIntoCache(__int64 llStartOffset);
public:
    int Init(const wchar_t* pwszVHDFile, D2D_ENCRYPTION_INFO* pEncInfo);
    int Init(const wchar_t* pwszVHDOld, const wchar_t* pwszVHDNew, D2D_ENCRYPTION_INFO* pEncInfo);
    
    // @bRONew - if open pwszVHDNew with share read only.
    int Init(const wchar_t* pwszVHDOld, const wchar_t* pwszVHDNew, bool bRONew);
    virtual int                 GetDiskGeometry(DISK_GEOMETRY& geometry);
    virtual void                FlushCache(); //clear the read cache
    bool ContainFullVHD(void); // if contain full VHD in (pwszVHDOld, pwszVHDNew] or if pwszVHDFile is FULL

protected:
    int Init_inner(const wchar_t* pwszVHDOld, const wchar_t* pwszVHDNew, 
             D2D_ENCRYPTION_INFO* pEncInfo, bool bRONew);
public:
    virtual int Release();
    virtual int GetTotalSectors(/*OUT*/ __int64* pllTotal);

    virtual int GetSectorSize(/*OUT*/ int* pnSize);
    virtual int GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap);
    virtual int Seek(/*IN*/ __int64 llOffsetInSec, /*OUT*/ __int64* pllNewPosInSec, /*IN*/ int nMethod);
    virtual int Read(/*OUT*/ void* lpBuffer, /*IN*/ int nSecs2Read, 
        /*OUT*/ int* pnSecsRead, /*IN*/ __int64 llStartingSec);
protected:
    bool                m_bContainFullVHD;       // If contains FULL vhd in the input VHDs
    vector< IVHDFile* > m_vecVHDFile;
    __int64             m_llTotalSec;            // total sectors of the diff disk, -1 means not initialize
    __int64             m_llCurrPosInSec;        // a internal record for current pos, in sectors

    __int64             m_llCacheStartOffset;    // the offset for the first bytes of cache (m_pbImageData), in bytes
    long                m_nCacheDataLen;         // the data len in the cache, in bytes
    char*               m_pbImageData;
    ST_VDISK_BITMAP_BUFFER* m_pstImageBmp;
    char*               m_pbParentData;
    ST_VDISK_BITMAP_BUFFER* m_pstParentBmp;
};
