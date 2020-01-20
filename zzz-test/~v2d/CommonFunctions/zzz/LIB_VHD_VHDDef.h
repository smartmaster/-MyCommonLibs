#pragma once

#define VHD_SECTOR_SIZE  512
#define VHD_BLOCK_SIZE   (2*1024*1024)

#pragma pack(push, CVHDImageDef)
#pragma  pack(1)
typedef union _VHDFooter
{
	struct  
	{
		CHAR                    Cookie[8];
		UINT32 Features;
		UINT32 FileFormatVersion;
		UINT64 DataOffset;
		UINT32 TimeStamp;
		CHAR                     CreatorApplication[4];
		UINT32 CreatorVersion;
		CHAR                    CreatorHostOS[4];
		UINT64 OriginalSize;
		UINT64 CurrentSize;
		union
		{
			struct  
			{
				UINT8   SectorsPerTrack_Cylinder;
				UINT8   Heads;
				UINT16 Cylinder;
			};
			UINT32 Value;
		}DiskGeometry;
		UINT32 DiskType;
		UINT32 Checksum;
		GUID                  UniqueId;
		UINT8   SavedState;
	};
	BYTE   argBuffer[512];
}VHDFOOTER, *PVHDFOOTER;

typedef struct _VHDParentLocator
{
	UINT32  PlatformCode;
	UINT32  PlatformDataSpace;
	UINT32  PlatformDataLength;
	UINT32  Reserved;
	UINT64  PlatformDataOffset;
}VHDPARENTLOCATOR, *PVHDPARENTLOCATOR;

typedef union _VHDDYNHeader
{
	struct  
	{
		CHAR Cookie[8];
		UINT64 DataOffset;
		UINT64  TableOffset;
		UINT32  HeaderVersion;
		UINT32  MaxTableEntries;
		UINT32  BlockSize;
		UINT32  Checksum;
		GUID ParentUniqueID;
		UINT32  ParentTimeStamp;
		UINT32  Reserved;
		WCHAR   ParentUnicodeName[256];
		VHDPARENTLOCATOR ParentLocators[8];
	};

	BYTE argBuffer[1024];
}VHDDYNHEADER, *PVHDDYNHEADER;

#pragma  pack(pop, CVHDImageDef)

#define PLATFORMCODE_WI2R 0x72326957
#define PLATFORMCODE_WI2K 0x6b326957
#define PLATFORMCODE_W2RU 0x75723257
#define PLATFORMCODE_W2KU 0x756b3257

//VHD-Writer macro

#define VHD_RET_SUCCESSFULL 0
#define VHD_RET_INVALID_PARA 1
#define VHD_RET_FAILED 2

#define VHD_RET_MASK                (0xF0000000)

#define VHD_RET_CHKSUM_MISMATCH     (0x00000001|VHD_RET_MASK)
#define VHD_RET_BAD_DISK_NAME       (0x00000002|VHD_RET_MASK)
#define VHD_RET_BAD_DISK_FILE       (0x00000003|VHD_RET_MASK)
#define VHD_RET_BAD_DESC_FILE       (0x00000004|VHD_RET_MASK)
#define VHD_RET_BAD_META_FILE       (0x00000005|VHD_RET_MASK)
#define VHD_RET_NO_DEST_FOUND       (0x00000006|VHD_RET_MASK)
#define VHD_RET_BAD_COMMAND         (0x00000007|VHD_RET_MASK)
#define VHD_RET_NO_SPARSE_SUPPORT   (0x00000008|VHD_RET_MASK)
#define VHD_RET_FILE_SIZE_LIMITED   (0x00000009|VHD_RET_MASK)
#define VHD_RET_INVALID_FOOTER      (0x0000000a|VHD_RET_MASK)
#define VHD_RET_INVALID_PARAMETER   (0x0000000b|VHD_RET_MASK)
#define VHD_RET_INVALID_DISK_TYPE   (0x0000000c|VHD_RET_MASK)
#define VHD_RET_ROLLBACK_FAILED     (0x0000000d|VHD_RET_MASK)
#define VHD_RET_FOUND_NO_PARENT     (0x0000000e|VHD_RET_MASK)
#define VHD_RET_FOUND_NO_DISK       (0x0000000f|VHD_RET_MASK)

#define VHD_RET_INTERNAL_ERROR      (0xFFFFFFFF)