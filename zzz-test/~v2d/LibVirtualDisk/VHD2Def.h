#pragma once
#include <rpc.h>
#include <windows.h>
#include <string>
using std::wstring;

#if 0
typedef struct _PurgingFileUnit
{
    DWORD			BucketSize;
    DWORD			Offset;
    DWORD			CheckSum;
}ST_PurgingFileUnit, *LP_PurgingFileUnit;

typedef struct _CompressedBufferHeader
{
    DWORD			Compressed;
    DWORD			OriginalSize;
    DWORD			CompressedSize;
    DWORD			BucketSize;
    DWORD			CheckSum;
}ST_CompressedBufferHeader, *LP_CompressedBufferHeader;

typedef struct _CompressedBuffer
{
    ST_CompressedBufferHeader	_Header;
    unsigned char				Buffer[1];
}ST_CompressedBuffer, *LP_CompressedBuffer;


#define VDISK_MAGIC                        0x4b534944562e4143
#pragma pack(push, __DISK_HEADER, 1)
typedef union _DISK_HEADER
{
    struct  
    {
        unsigned __int64 Magic; //'C','A','.','V','D','I','S','K'
        unsigned __int32 DiskType; //full : inc
        unsigned __int32 BlockSize; //64KB
        unsigned __int64 DataOffset; // sizeof(DISK_HEADER) by default
        unsigned __int64 BATOffset; //offset in byte
        unsigned __int32 BATSize;
        unsigned __int64 DiskSize; //size in byte
        unsigned __int32 MaxTableEntries;
        DISK_GEOMETRY DiskGeometry;
        GUID UniqueID;

        struct  
        {
            unsigned __int32 Algo;
        }Compression;

        struct
        {
            unsigned __int32 Algo;
            unsigned __int8 ValidationDataSize;
            unsigned __int8 ValidationData[256]; //to validate the password
        }Encryption; //for future use

        struct  
        {
            wchar_t AbsoluteFilename[512];
            wchar_t RelatedFilename[512];
            UUID UniqueID;
        }Parent;

        unsigned __int32 HdrChecksum;
    };

    unsigned __int8 HdrData[ 8 * 1024 ];
}DISK_HDR, *PDISK_HDR;
#pragma pack(pop, __DISK_HEADER)


#pragma pack(push, __Disk_SI_BAT, 1)
typedef struct _Disk_SI_BAT
{
    unsigned __int32 SI;
    unsigned __int32 BAT;
}ST_SI_BAT, *LP_SI_BAT;
#pragma pack(pop, __Disk_SI_BAT)


#pragma pack(push, __Disk_SI_BAT, 1)
typedef struct _Disk_Node
{
    DISK_HDR hdr;
    HANDLE hVHDFile;
    HANDLE hIndexFile;
    wstring strVHDFileName;
    wstring strIdxFileName;
    BOOL bSIBmpChanged;
}ST_DiskNode, *LP_DiskNode;
#pragma pack(pop, __Disk_SI_BAT)
#endif //if 0

typedef struct _PurgingFileUnit
{
    DWORD			BucketSize;
    DWORD			Offset;
    DWORD			CheckSum;
}ST_PurgingFileUnit, *LP_PurgingFileUnit;

typedef struct _CompressedBufferHeader
{
    DWORD			Compressed;
    DWORD			OriginalSize;
    DWORD			CompressedSize;
    DWORD			BucketSize;
    DWORD			CheckSum;
}ST_CompressedBufferHeader, *LP_CompressedBufferHeader;

typedef struct _CompressedBuffer
{
    ST_CompressedBufferHeader	_Header;
    unsigned char				Buffer[1];
}ST_CompressedBuffer, *LP_CompressedBuffer;


#define VDISK_MAGIC					0x4b534944562e4143
#define VHD_VERSION					2

// The header and some structure changed between D2D build 423 and RTM build
// Here we also update the structure definition
#if 0
#pragma pack(push, __DISK_HEADER, 1)
typedef union _DISK_HEADER
{
    struct  
    {
        unsigned __int64 Magic; //'C','A','.','V','D','I','S','K'
        unsigned __int32 DiskType; //full : inc
        unsigned __int32 BlockSize; //64KB
        unsigned __int64 DataOffset; // sizeof(DISK_HEADER) by default
        unsigned __int64 BATOffset; //offset in byte
        unsigned __int32 BATSize;
        unsigned __int64 DiskSize; //size in byte
        unsigned __int32 MaxTableEntries;
        DISK_GEOMETRY DiskGeometry;
        GUID UniqueID;

        struct  
        {
            unsigned __int32 Algo;
        }Compression;

        struct
        {
            unsigned __int32 Algo;
            unsigned __int8 ValidationDataSize;
            unsigned __int8 ValidationData[256]; //to validate the password
        }Encryption; //for future use

        struct  
        {
            wchar_t AbsoluteFilename[512];
            wchar_t RelatedFilename[512];
            UUID UniqueID;
        }Parent;

        unsigned __int32 HdrChecksum;
        // in first release
        // this value is zero in beta build
        // when GM this value is 2
        // thus we can indicate the different between beta and GM
        // and format modification should change this value as well
        unsigned __int32 DiskVersion; 
        unsigned __int32 BATBucketSize;
        unsigned __int32 OriginalBATSize;
    };

    unsigned __int8 HdrData[ 8 * 1024 ];
}DISK_HDR, *PDISK_HDR;
#pragma pack(pop, __DISK_HEADER)


#pragma pack(push, __Disk_SI_BAT, 1)
typedef struct _Disk_SI_BAT
{
    unsigned __int32 SI;
    unsigned __int32 BAT;
}ST_SI_BAT, *LP_SI_BAT;
#pragma pack(pop, __Disk_SI_BAT)


#pragma pack(push, __Disk_SI_BAT, 1)
typedef struct _Disk_Node
{
    DISK_HDR hdr;
    HANDLE hVHDFile;
    HANDLE hIndexFile;
    wstring strVHDFileName;
    wstring strIdxFileName;
    BOOL bSIBmpChanged;
}ST_DiskNode, *LP_DiskNode;
#pragma pack(pop, __Disk_SI_BAT)
#endif // if 0

//<< Update the definition for RTM build
#pragma pack(push, __DISK_HEADER, 1)
typedef union _DISK_HEADER
{
    struct  
    {
        unsigned __int64 Magic; //'C','A','.','V','D','I','S','K'
        unsigned __int32 DiskType; //full : inc
        unsigned __int32 BlockSize; //64KB
        unsigned __int64 DataOffset; // sizeof(DISK_HEADER) by default
        unsigned __int64 BATOffset; //offset in byte
        unsigned __int32 BATSize;
        unsigned __int64 DiskSize; //size in byte
        //unsigned __int32 MaxTableEntries;
        DISK_GEOMETRY DiskGeometry;
        GUID UniqueID;

        struct  
        {
            unsigned __int32 Algo;
        }Compression;

        struct
        {
            unsigned __int32 Algo;
            unsigned __int8 ValidationDataSize;
            unsigned __int8 ValidationData[256]; //to validate the password
        }Encryption; //for future use

        struct  
        {
            wchar_t AbsoluteFilename[512];
            wchar_t RelatedFilename[512];
            UUID UniqueID;
        }Parent;

        unsigned __int32 HdrChecksum;
        // in first release
        // this value is zero in beta build
        // when GM this value is 2
        // thus we can indicate the different between beta and GM
        // and format modification should change this value as well
        unsigned __int32 DiskVersion; 
        unsigned __int64 llBATBucketSize;
        unsigned __int64 llOriginalBATSize;
    };

    unsigned __int8 HdrData[ 16 * 1024 ];
}DISK_HDR, *PDISK_HDR;
#pragma pack(pop, __DISK_HEADER)


#pragma pack(push, __Disk_SI_BAT, 1)
typedef struct _Disk_SI_BAT
{
    unsigned __int32 SI;
    unsigned __int64 BAT;
}ST_SI_BAT, *LP_SI_BAT;
#pragma pack(pop, __Disk_SI_BAT)


#pragma pack(push, __Disk_SI_BAT, 1)
typedef struct _Disk_Node
{
    DISK_HDR hdr;
    HANDLE hVHDFile;
    HANDLE hIndexFile;
    wstring strVHDFileName;
    wstring strIdxFileName;
    BOOL bSIBmpChanged;
}ST_DiskNode, *LP_DiskNode;
#pragma pack(pop, __Disk_SI_BAT)
// Update the definition for RTM build>>