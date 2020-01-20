#ifndef _HA_INCDISKS_INFULL_H_
#define _HA_INCDISKS_INFULL_H_
#include "HAVhdUtility.h"
using namespace VHDConv;
#include <string>
using std::wstring;

class CIncDisksInFull : public IVDiskDiff
{
public:
    CIncDisksInFull(void);
protected:
    virtual ~CIncDisksInFull(void);
    void UnInit();
    int CheckBitVal(__int64 llSec, BOOL& bSet); // If this sector need to read
public:
    int Init(const wchar_t* pwszFullVhd, IDiskBmpReader* pDiffBitmap, D2D_ENCRYPTION_INFO* pEncInfo);
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
    IVHDFile*               m_pFullVhd;
    IDiskBmpReader*         m_pDiffBitmap;
    wstring                 m_wstrFullVhd;
    ST_VDISK_BITMAP_BUFFER* m_pstBmpIncs;        // bitmap cache for the incs to read from full
    __int64                 m_llCurrPosInSec;
    __int64                 m_llTotalSec;
};




#endif //_HA_INCDISKS_INFULL_H_
