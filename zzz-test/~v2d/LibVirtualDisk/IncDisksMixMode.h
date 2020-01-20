#ifndef _HA_INCDISKS_MIXMODE_H_
#define _HA_INCDISKS_MIXMODE_H_
#include "HAVhdUtility.h"
using namespace VHDConv;
#include <string>
using std::wstring;

class CIncDisksMixMode : public IVDiskDiff
{
public:
    CIncDisksMixMode(void);
protected:
    virtual ~CIncDisksMixMode(void);
    void UnInit();
    inline int CheckBitVal_InFull(__int64 llSec, BOOL& bSet); //check if bit is 1 on m_pVDiffInFull bitmap
    inline int CheckBitVal_Out(__int64 llSec, BOOL& bSet);    //check if bit is 1 on m_pVDiffOut bitmap
public:
    int Init(const wchar_t* pwszFullVhd, IDiskBmpReader* pDiffBitmap,
             const wchar_t* pwszIncVhd, D2D_ENCRYPTION_INFO* pEncInfo);
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
    IVDiskDiff*             m_pVDiffInFull;      // the incremental disks in full VHD
    IVDiskDiff*             m_pVDiffOut;         // the incremental disks out of full
    bool                    m_bContainFullInDiffOut;// If contain FULL in m_pVDiffOut
    wstring                 m_wstrFullVhd;
    wstring                 m_wstrIncVhd;
    __int64                 m_llCurrPosInSec;
    __int64                 m_llTotalSec;
    ST_VDISK_BITMAP_BUFFER* m_pstBmpInFull;     // bitmap cache for diskdiff in full VHD
    ST_VDISK_BITMAP_BUFFER* m_pstBmpOut;        // bitmap cache for diskdiff out of full

};




#endif //_HA_INCDISKS_MIXMODE_H_