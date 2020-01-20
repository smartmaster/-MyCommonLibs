#include "InnerInterface.h"
#include "VDiskDiffImp.h"

//
//This class represent a whole VHD Disk Image
//
class CVHDImage : public IVDisk
{
public:
    CVHDImage();
    ~CVHDImage();
public:
    virtual int Release();

    virtual int GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap);

    virtual int GetTotalSectors(/*OUT*/ __int64* pllTotal);

    virtual int GetSectorSize(/*OUT*/ int* pnSize);

    virtual int GetDiskGeometry(DISK_GEOMETRY& geometry);

    virtual int Seek(__int64 llOffsetInSec, __int64* pllNewPosInSec, int nMethod);

    virtual int Read(/*OUT*/ void* lpBuffer, /*IN*/ int nSecs2Read, 
        /*OUT*/ int* pnSecsRead, /*IN*/ __int64 llStartingSec);

    virtual int Write(const void* lpBuffer, int nSecs2Write, int* pnSecsWritten, __int64 llStartingSec);
public:
    int Init(const wchar_t* pwszPath, bool bROLast);
protected:
    bool            m_bROLast;     // If the last VHD is read only.
    CVDiskDiffImp*  m_pReader;
    IVHDFileW*      m_pLastVhdW;
    __int64         m_llCurrPosInSec; 
};
