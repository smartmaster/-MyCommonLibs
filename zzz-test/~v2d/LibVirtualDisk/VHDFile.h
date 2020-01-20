#pragma once
//#include "HAVhdUtility.h"
//#include "InnerInterface.h"
#include "CommonUtil.h"
#include "VHDDef.h"
#include "../h/AFStorInterface.h"
#include "HaVhdUtilErrorcode.h"
#include <string>
using std::wstring;
//using namespace HaVhdUtility;


static const wchar_t* VHD_POSTFIX = L".D2D";
static const char*  ORIG_CREATOR_D2D = "DdDdDdDd";
static const char*  ORIG_CREATOR_MS = "conectix";

//===========================================================================
// VHD File Creator
//===========================================================================
class CDynVHDFile;
class CDynVHD2File;
class CVHDFileCreator
{
protected:
    CVHDFileCreator() {};
    virtual ~CVHDFileCreator(void) {};
public:
    static int Open(const wchar_t* pwszPath, DWORD eMode, DWORD dwShareMode, 
                    DWORD dwCreateDisp, D2D_ENCRYPTION_INFO* pEncryptionInfo,IVHDFile** ppVhd);

    //return 0 success, others fail. The case that not return 0 but *ppPlainVHD will NEVER occur.
    static int       Open(const wchar_t* pwszPath, DWORD eMode, DWORD dwCreateDisp, D2D_ENCRYPTION_INFO* pEncryptionInfo,CDynVHDFile** ppPlainVHD);
    static int       Open(const wchar_t* pwszPath, DWORD eMode, DWORD dwCreateDisp, D2D_ENCRYPTION_INFO* pEncryptionInfo,CDynVHD2File** ppCmprsVHD);

};


//===========================================================================
// Dynamic VHD file
//===========================================================================
class CDynVHDFile : public IVHDFile
{
    friend class CVHDFileCreator;
protected:
    CDynVHDFile();
    virtual ~CDynVHDFile(void);
    CDynVHDFile(const CDynVHDFile&);
    CDynVHDFile& operator=(const CDynVHDFile&);
public:
    virtual const wchar_t*    GetFilePath() const;

    virtual int    Release();

    virtual int     GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
                              /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap);

    virtual int     QueryRange(const LOCAL_ALLOCATED_RANGE* pRangeToQuery, 
                               LOCAL_ALLOCATED_RANGE* pRange, int* pnRangeCnt);

    //
    //Set the logic disk offset
    //
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
        return VHD_Dynamic; 
    };

    virtual VHD_CREATOR         GetVHDCreator()
    {
        if (memcmp(m_stFooter.Cookie, ORIG_CREATOR_MS, sizeof(m_stFooter.Cookie)) == 0)
            return Conectix;
        else if (memcmp(m_stFooter.Cookie, ORIG_CREATOR_D2D, sizeof(m_stFooter.Cookie)) == 0)
            return D2D;
        else
            return UnknownCreator;
    };

    virtual int          GetDiskID(unsigned long& ulDiskID);
    virtual int          GetFooter(VHDFOOTER& footer);
    virtual int          GetHeader(VHDDYNHEADER& header);
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
    virtual void         FreeBuffer(void* p);

public:
    //
    //Set the cookie that identify the creator in the footer.
    //The creator cookie is case sensitive
    //For example "conectix" or "DdDdDdDd"
    //
    virtual int          SetCreatorCookie(const char* pszCreator);
protected:
    virtual int Init(const wchar_t* pwszPath, DWORD eMode, DWORD dwShareMode, DWORD dwCreateDisp,D2D_ENCRYPTION_INFO* pEncryptionInfo);

    //
    //Position to block and update the current bitmap
    //return 0, others fail.
    //
    virtual int PositionBlock(unsigned __int64 nBlockIdx);
protected:
    wstring m_wstrFilePath;
    DWORD m_openMode;
    CEnsureCloseHandle<INVALID_HANDLE_VALUE> m_shVHD;       // the handle to read data
    CEnsureCloseHandle<INVALID_HANDLE_VALUE> m_shAttrVHD;   // the handle to get bitmap 

    VHDFOOTER m_stFooter;
    VHDDYNHEADER m_stHeader;
    unsigned __int32* m_pBAT;
    ULONG m_ulSectorsPerBlock;
    ULONG m_ulBlockBitmapSectorCount;

    unsigned __int64 m_ullCurrLPos; //The current logic position
    unsigned char* m_pbSecBitmap;   //the current block's sector bitmap
};

//===========================================================================
// Differential VHD file
//===========================================================================
class CDiffVHDFile : public CDynVHDFile
{
    friend class CVHDFileCreator;
protected:
    CDiffVHDFile();
    virtual ~CDiffVHDFile(void);
    CDiffVHDFile(const CDiffVHDFile&);
    CDiffVHDFile& operator=(const CDiffVHDFile&);
public:
    virtual VHD_TYPE            GetVHDType()
    {
        return VHD_Diff; 
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
