/******************************************************************************
*
*        filename  :      D2DBaseDef.h
*        created   :      
*        Written by:      
*        comment   :      the base structure definition of D2D
*
******************************************************************************/
#ifndef _D2D_BASE_DEFINE_
#define _D2D_BASE_DEFINE_
#pragma warning(push, 3)
#include <WinIoCtl.h>
#pragma warning(pop)

enum VHD_TYPE
{
    VHD_Unknown = 0,
    VHD_Fixed = 2,
    VHD_Dynamic = 3,            //plain full
    VHD_Diff = 4,               //plain incremental
    VHD_Dynamic2 = 0x80000000,  //compressed VHD full
    VHD_Diff2 = 0x80000001      //compressed VHD incremental
};

enum VHD_CREATOR
{
    UnknownCreator = 0,
    Conectix = 1,   //MS, mountable
    D2D = 2         //D2D, unmountable
};

enum VHD_PARENTPATH_TYPE
{
    WIN_RELATIVE = 0,
    WIN_ABSOLUTE = 1,
};

typedef struct _t_local_allocated_range
{
    __int64 offset;
    __int64 len;
} LOCAL_ALLOCATED_RANGE, *PLOCAL_ALLOCATED_RANGE;

typedef struct _t_vdisk_bitmap_buffer
{  
    LARGE_INTEGER StartingSec;  // Starting sector, align with 8
    LARGE_INTEGER BitmapSize;   // Count of sectors
    BYTE Buffer[1];
} ST_VDISK_BITMAP_BUFFER, *PST_VDISK_BITMAP_BUFFER;


// Some limitation parameter in D2D
// maximal character count of session password.
#ifndef AF_SESSPWD_MAXLEN_IN_CHARACTER
#define AF_SESSPWD_MAXLEN_IN_CHARACTER   23
#endif   // #ifndef _ASDEFS_H_

//2010-5-10 encrypt support
typedef struct _D2D_ENCRYPTION_INFO
{
	WCHAR   szSessionPassword[AF_SESSPWD_MAXLEN_IN_CHARACTER + 1];
	UINT16  uiCryptoType; //the encrypt type asked by user
	UCHAR   ucAlgorithmType;
	UCHAR   ucUsingCompression; // indicate whether user ask for compression
	UCHAR   bSupportCompression;// indicate whether support compression
	UCHAR   reserved[64 - 2*(AF_SESSPWD_MAXLEN_IN_CHARACTER + 1) -2 - 1 - 1 - 1];
}D2D_ENCRYPTION_INFO, *PD2D_ENCRYPTION_INFO;



class ILocalAllocReader
{
public:
    virtual int    Release() = 0;
   
    //
    // Get the bitmap
    // @nBufLen - length of input pBitmap's Buffer, in bytes
    //
    virtual int GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap) = 0;

    //
    //query
    //return value:
    //0 - success
    //E_VHD_COV_RANGE_MOREDATA - The input buffer is not enough, and return *pnRangeCnt Ranges.
    //
    virtual int     QueryRange(const LOCAL_ALLOCATED_RANGE* pRangeToQuery, 
                               LOCAL_ALLOCATED_RANGE* pRange, int* pnRangeCnt) = 0;

    //
    //Set the logic disk offset
    //
    virtual int     SetPos(__int64 llOffset) = 0;

    //
    //Read the LocalAllocatedData
    //return 0 success, others fail
    //if return 0 and returned pRange->len == 0 && pRange->offset == -1, have read to the end of file.
    //@llStartOffset - the logical offset from where to start read, if -1, read from current file's pointer
    //@pRange - return the buffer range we read.
    //
    virtual int     Read(char* pBuf, unsigned long cbToRead, 
                         LOCAL_ALLOCATED_RANGE* pRange, __int64 llStartOffset) = 0;

    //
    // @cbToRead      - number of bytes to read, must be sector(512) aligned
    // @llStartOffset - logical offset from where to read, must be sector(512) aligned,
    //                  if -1, from current position.
    // return value: 0 success, others fail.
    //
    virtual int                 Read(char* pBuf, unsigned long cbToRead, 
                                     unsigned long* pcbRead, __int64 llStartOffset) = 0;
};

class IVHDFile : public ILocalAllocReader
{
protected:
    IVHDFile() {};
    virtual ~IVHDFile() {};
    IVHDFile(const IVHDFile&);
    IVHDFile& operator=(const IVHDFile&);
public:
    virtual const wchar_t*      GetFilePath() const = 0;
    virtual VHD_TYPE            GetVHDType() = 0;
    virtual VHD_CREATOR         GetVHDCreator() = 0;
    virtual int                 GetDiskID(unsigned long& ulDiskID) = 0;
    virtual int                 GetDiskGeometry(DISK_GEOMETRY& geometry) = 0;
    virtual int                 GetDiskSize(__int64& llSize) = 0;
    virtual int                 GetGuid(GUID* pGuid) = 0;

#if 0
    //
    // @cbToRead      - number of bytes to read, must be sector(512) aligned
    // @llStartOffset - logical offset from where to read, must be sector(512) aligned,
    //                  if -1, from current position.
    // return value: 0 success, others fail.
    //
    virtual int                 Read(char* pBuf, unsigned long cbToRead, 
                                     unsigned long* pcbRead, __int64 llStartOffset) = 0;
#endif //if0

    //
    //if *ppPath == NULL, indicate the path does not exist
    //call FreeBuffer() to release *ppwszPath
    //
    virtual int                 GetParentPath(VHD_PARENTPATH_TYPE type, wchar_t** ppwszPath) = 0;

    //
    //if pwszPath == NULL, remove the corresponding path
    //
    virtual int                 SetParentPath(VHD_PARENTPATH_TYPE type, const wchar_t* pwszPath) = 0;

    //
    //@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
    //
    virtual int                 GetParentTimestamp(unsigned __int32& timestamp) = 0;

    //
    //@timestamp - the number of seconds since January 1, 2000 12:00:00 AM in UTC/GMT
    //
    virtual int                 SetParentTimestamp(unsigned __int32 timestamp) = 0;

    //
    //Free buf
    //
    virtual void                FreeBuffer(void* p) = 0;
};












#endif //_D2D_BASE_DEFINE_

