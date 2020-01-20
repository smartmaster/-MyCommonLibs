/******************************************************************************
*
*        filename  :      HAVmIoIntf.h
*        created   :      2010/04/21
*        Written by:      baide02
*        comment   :      interface of virtual disk and virtual volume
*
******************************************************************************/
#ifndef _HA_VM_IO_INTERFACE_
#define _HA_VM_IO_INTERFACE_
#include <windows.h>
#include "D2DBaseDef.h"


namespace HaVhdUtility
{
//=======================================================================
// Virtual Disk
//=======================================================================

#define VDISK_SEEK_BEGIN  0
#define VDISK_SEEK_CURR   1
#define VDISK_SEEK_END    2

/**
* Represent a virtual disk
*/
struct IVDisk
{
    virtual int Release() = 0;

    //
    // @nBufLen - length of input ST_VDISK_BITMAP_BUFFER::Buffer
    //
    virtual int GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap) = 0;

    virtual int GetTotalSectors(/*OUT*/ __int64* pllTotal) = 0;

    virtual int GetSectorSize(/*OUT*/ int* pnSize) = 0;

    virtual int GetDiskGeometry(DISK_GEOMETRY& geometry) = 0;

    //
    // @llOffsetInSec  [in]  - the distance to move in sectors
    // @pllNewPosInSec [out] - if not NULL, return the new position, in sectors
    // @nMethod        [in]  - VDISK_SEEK_BEGIN or VDISK_SEEK_CURR or VDISK_SEEK_END
    // return 0 success, others fail
    // If seek before the beginning of file, return ERROR_NEGATIVE_SEEK
    // If seek after the end of file, will return 0, and new position will exceed the end of file.
    //
    virtual int Seek(__int64 llOffsetInSec, __int64* pllNewPosInSec, int nMethod) = 0;

    //
    // 1) If lpBuffer == NULL && nSecs2Read == 0 && pnSecsRead == 0, the behavior is Seek
    // 2) Else if llStartingSec == -1, will read from internal position sequentially.
    //    If Read() is call on the first time with llStartingSec == -1, read from beginning of disk(0)
    // return value:
    // 0 success, others fail.
    // return value == 0 && *pnSecsRead == 0 means reach the end of VDisk.
    //
    virtual int Read(/*OUT*/ void* lpBuffer, /*IN*/ int nSecs2Read, 
        /*OUT*/ int* pnSecsRead, /*IN*/ __int64 llStartingSec) = 0;

    //virtual int ReadEx(__int64 llStartingSec, int nSecs2Read, IReadCallback* pCb) = 0;

    //
    // If llStartingSec == -1, write at internal position sequentially.
    // If exceed the VDISK's max sector, will return fail.
    // return 0 success, others fail.
    //
    virtual int Write(const void* lpBuffer, int nSecs2Write, int* pnSecsWritten, __int64 llStartingSec) = 0;
};

/**
* Represent the Difference of Virtual Disk
*/
struct IVDiskDiff
{
    virtual int Release() = 0;

    //
    // @nBufLen - length of input ST_VDISK_BITMAP_BUFFER::Buffer
    //
    virtual int GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap) = 0;

    virtual int GetTotalSectors(/*OUT*/ __int64* pllTotal) = 0;

    virtual int GetSectorSize(/*OUT*/ int* pnSize) = 0;

    //
    // Seek
    //
    virtual int Seek(__int64 llOffsetInSec, __int64* pllNewPosInSec, int nMethod) = 0;

    //
    // Read
    //
    virtual int Read(/*OUT*/ void* lpBuffer, /*IN*/ int nSecs2Read, 
        /*OUT*/ int* pnSecsRead, /*IN*/ __int64 llStartingSec) = 0;

    //
    // ReadEx
    //
};

/**
* Represent a disk bitmap reader. 
* This can be used to read a full disk bitmap or read the bitmap of the difference 
* between disks (bitmap of IVDiskDiff).
*/
struct IDiskBmpReader
{
    virtual int Release() = 0;
    //
    // @nBufLen - length of input ST_VDISK_BITMAP_BUFFER::Buffer
    //
    virtual int GetBitmap(/*IN*/ LARGE_INTEGER liStartingSec, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VDISK_BITMAP_BUFFER pBitmap) = 0;

    virtual int GetTotalSectors(/*OUT*/ __int64* pllTotal) = 0;
};

//=======================================================================
// Virtual Volume
//=======================================================================
typedef struct _t_vvol_bitmap_buffer
{
    LARGE_INTEGER StartingLcn;  // Starting cluster, align with 8
    LARGE_INTEGER BitmapSize;   // Count of clusters
    BYTE Buffer[1];
} ST_VVOL_BITMAP_BUFFER, *PST_VVOL_BITMAP_BUFFER;

// sizeof(ST_VVOL_DISK_INFO) == 0x230
// Attention: make sure "sizeof(ST_VVOL_DISK_INFO) == sizeof(VM_VOL_DISK_INFO)" at DR side
typedef struct _t_vvol_disk_info 
{
    WCHAR       wszVDiskName[MAX_PATH];  // virtual disk file name for this extent
    ULONG       ulDiskNum;               // BACKUP_BLI, disk number
    ULONG       ulSignature;             // disk signature
    LONGLONG    ulExtentStartPos;        // The offset from the beginning of the disk to 
                                         // the extent, in bytes.
    LONGLONG    ulExtentLength;          // The number of bytes in this extent
    LONGLONG    ulVolumeOffset;          // Volume offset corresponding to the start of 
                                         // this extent (ulExtentStartPos)
    ULONG       ulSectorSize;            // Size of every sector on the disk. VMware & MSFT 
                                         // virtual solutions have it fixed to 512 bytes, kept for 
                                         // future compatibility.
    char        padding[4];              // to make sure the size of this structure be the same as
                                         // VM_VOL_DISK_INFO at DR side. 
}ST_VVOL_DISK_INFO, *PST_VVOL_DISK_INFO;


#define VVOL_FLAG_UNFORMATED           0x00000001  // the volume is not formated
#define VVOL_FLAG_MOUNTPOINT           0x00000002  // no drive letter but mount point
#define VVOL_FLAG_NOT_MOUNTED          0x00000004  // no drive letter and no mount point
#define VVOL_FLAG_FS_FAT               0x00000008  // If this flag is not specified assume volume is NTFS
#define VVOL_FLAG_FS_FAT32             0x00000010  // the volume is FAT32

typedef struct _t_vvol_attr 
{
    WCHAR wszVolGuid[64];               // volume ID
    WCHAR wszVolName[MAX_PATH];         // drive name or mount point
    ULONG ulOptions;                    // formatted, mountpoint, etc
    //ULONG ulNumOfDisks;                 // number of disks 
    //ST_VVOL_DISK_INFO vm_disks[1];
}ST_VVOL_ATTR, *PST_VVOL_ATTR;


/**
* Represent a virtual volume
*/
struct IVVol
{
    virtual int Release() = 0;

    //
    // @nBufLen [in] - Length of input PST_VOL_BITMAP_BUFFER::Buffer
    // @pBitmap [out]- pBitmap::Buffer, the bitmap buffer from pBitmap::StartingLcn, 
    //                 pBitmap::StartingLcn, the starting cluster of the buffer returned,
    //                 pBitmap::BitmapSize, count of clusters returned.
    // return 0 success, others fail
    // If nBufLen*8 > pBitmap->BitmapSize, means reach the end of volume.
    //
    virtual int GetBitmap(/*IN*/ LARGE_INTEGER liStartingLcn, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VVOL_BITMAP_BUFFER pBitmap) = 0;

    //
    // Get the attributes of the volume
    // @pstVolAttr [out] - the attributes returns
    // return 0 success, others fail
    //
    virtual int GetAttributes(ST_VVOL_ATTR* pstVolAttr) = 0;

    //
    // Get the total count of disk
    // @pnCnt [out] - the count of disk for this volume
    // return 0 success, others fail
    //
    virtual int GetDiskCount(int* pnCnt) = 0;

    //
    // Get the volume disk info.
    // @pstDiskInfo [out] - return the volume's disk info.
    // @nBufCnt     [in]  - the count of input pstDiskInfo, in ST_VVOL_DISK_INFO.
    // @pnCntOut    [out] - the count of pstDiskInfo returns.
    // return 
    // 0 success
    // others fail, ERROR_MORE_DATA means not enough memory.
    //
    virtual int GetVolDiskInfo(ST_VVOL_DISK_INFO* pstDiskInfo, int nBufCnt, int* pnCntOut) = 0;
};

/**
* Represent the Difference of Virtual volume
*/
struct IVVolDiff
{
    virtual int Release() = 0;

    //
    // @nBufLen [in] - Length of input PST_VOL_BITMAP_BUFFER::Buffer
    // @pBitmap [out]- pBitmap::Buffer, the bitmap buffer from pBitmap::StartingLcn, 
    //                 pBitmap::StartingLcn, the starting cluster of the buffer returned,
    //                 pBitmap::BitmapSize, count of clusters returned.
    // return 0 success, others fail
    // If nBufLen*8 > pBitmap->BitmapSize, means reach the end of volume.
    //
    virtual int GetBitmap(/*IN*/ LARGE_INTEGER liStartingLcn, /*IN*/ int nBufLen, 
        /*OUT*/ PST_VVOL_BITMAP_BUFFER pBitmap) = 0;
};
} // namespace HaVhdUtility



#endif //_HA_VM_IO_INTERFACE_