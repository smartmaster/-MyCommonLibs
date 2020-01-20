#pragma once
//#include "HAVhdUtility.h"
#include "DynamicVHD2.h"
#include "vhddef.h"
#include "vhd.h"
//#include "VHD2Def.h"
#include "MultipleThreadCompressor.h"
#include "CommonUtil.h"
#include "../h/AFStorInterface.h"
#include "HaVhdUtilErrorcode.h"
//using namespace HaVhdUtility;
#include <Windows.h>
#include <string>
using namespace std;


static const wchar_t* VHD2_POSTFIX = L".D2D";

//====================================================
//dynamic compressed VHD file
//====================================================
class CDynVHD2File : public IVHDFile
{
    friend class CVHDFileCreator;
protected:
    CDynVHD2File();
    virtual ~CDynVHD2File(void);
    CDynVHD2File(const CDynVHD2File&);
    CDynVHD2File& operator=(const CDynVHD2File&);
public:
    virtual const wchar_t*    GetFilePath() const;

    virtual int    Release();

    virtual int GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
                          /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap);

    virtual int     QueryRange(const LOCAL_ALLOCATED_RANGE* pRangeToQuery, 
                               LOCAL_ALLOCATED_RANGE* pRange, int* pnRangeCnt);

    virtual int     SetPos(__int64 llOffset);
    //
    //Read the LocalAllocatedData
    //return 0 success, others fail
    //if return 0 and returned pRange->len == 0 && pRange->offset == -1, have read to the end of file.
    //@llStartOffset - the logical offset from where to start read, if -1, read from current file's pointer
    //@pRange - return the buffer range we read.
    //@cbToRead - The bytes to read, should be VHD_SECTOR_SIZE aligned
    //
    virtual int     Read(char* pBuf, unsigned long cbToRead, LOCAL_ALLOCATED_RANGE* pRange, __int64 llStartOffset);

    virtual int     Read(char* pBuf, unsigned long cbToRead, unsigned long* pcbRead, __int64 llStartOffset);

    virtual VHD_TYPE            GetVHDType()
    {
        return VHD_Dynamic2; 
    };

    virtual VHD_CREATOR         GetVHDCreator()
    {
        return D2D;
    };

    virtual int          GetDiskID(unsigned long& ulDiskID);
    virtual int          GetDiskGeometry(DISK_GEOMETRY& geometry);
    virtual int          GetDiskSize(__int64& llSize);
    virtual int          GetGuid(GUID* pGuid);

    //
    //if *ppPath == NULL, indicate the path does not exist
    //
    virtual int          GetParentPath(VHD_PARENTPATH_TYPE type, wchar_t** ppwszPath);

    //
    //if pwszPath == NULL, remove the corresponding path
    //
    virtual int          SetParentPath(VHD_PARENTPATH_TYPE type, const wchar_t* pwszPath);

    //
    //@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
    //
    virtual int          GetParentTimestamp(unsigned __int32& timestamp);

    //
    //@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
    //
    virtual int          SetParentTimestamp(unsigned __int32 timestamp);

    //
    //Free buffer
    //
    virtual void                FreeBuffer(void* p);
protected:
    virtual int Init(const wchar_t* pwszPath, DWORD eMode, DWORD dwShareMode, DWORD dwCreateDisp,D2D_ENCRYPTION_INFO* pEncryptionInfo);
    int InitCompress();
	__int32 InitEncrypt();
	BOOL CheckEncInfo(PDISK_HDR pDiskHdr);
    int ReadFileHeader();
    int SplitSIBat();
    int GetSIBat(__int64 blkIdx, ST_SI_BAT& sibat);

    DWORD GetReadingSize(DWORD dwSize);
    DWORD GetSizeWithBlockPadding(DWORD sizeToPad, DWORD dwAlign);
    //
    //return 0 success, and read m_stHeader.BlockSize bytes
    //otherwise failed, read 0 bytes
    //
    int ReadOneBlock(__int64 blkIdx, const ST_SI_BAT* pSiBat, BYTE* pBuf, DWORD dwLen);
protected:
    DWORD m_openMode;
    wstring m_wstrFilePath;
    wstring m_wstrIdxPath;
    CEnsureCloseHandle<INVALID_HANDLE_VALUE> m_shVHD;
    CEnsureCloseHandle<INVALID_HANDLE_VALUE> m_shIdx;
    DISK_HDR m_stHeader;
    D2D_ENCRYPTION_INFO m_stVHDEncInfo;
	BOOL m_bVHDEncInited;
	BOOL                    m_bEncInitStatus;
    unsigned __int64 m_ullCurrLPos; //The current logic position

    DWORD m_dwDiskSectorSize;

    LP_CompressedBuffer 	m_pCmprsedBuf;
    DWORD m_dwCmprsedBufSize;
    BYTE* m_pCacheBuffer;
    DWORD m_dwCacheBufferSize;
    __int64 m_BlkIdxCached; //the block index of current cache
	CMultipleThreadEncCompressor mtce;

    static const int MAX_SIBAT_COUNT = 256;
    struct 
    {
        __int64     llIdxBegin;
        DWORD       dwCnt;
        ST_SI_BAT   data[MAX_SIBAT_COUNT];
    } m_sibatCache;
};


//===========================================================================
// compressed Differential VHD file
//===========================================================================
class CDiffVHD2File : public CDynVHD2File
{
    friend class CVHDFileCreator;
protected:
    CDiffVHD2File();
    virtual ~CDiffVHD2File(void);
    CDiffVHD2File(const CDiffVHD2File&);
    CDiffVHD2File& operator=(const CDiffVHD2File&);
public:
    virtual VHD_TYPE            GetVHDType()
    {
        return VHD_Diff2; 
    };

    //
    //if *ppPath == NULL, indicate the path does not exist
    //
    virtual int GetParentPath(VHD_PARENTPATH_TYPE type, wchar_t** ppwszPath);

    //
    //if pwszPath == NULL, remove the corresponding path
    //
    virtual int SetParentPath(VHD_PARENTPATH_TYPE type, const wchar_t* pwszPath);

    //
    //@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
    //
    virtual int GetParentTimestamp(unsigned __int32& timestamp);

    //
    //@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
    //
    virtual int SetParentTimestamp(unsigned __int32 timestamp);
    //
    //Free buffer
    //
    virtual void                FreeBuffer(void* p);
};
