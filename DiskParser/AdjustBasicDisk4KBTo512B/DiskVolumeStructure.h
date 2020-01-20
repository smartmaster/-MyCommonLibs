#pragma once

#include <windows.h>


#include <pshpack1.h>

//////////////////////////////////////////////////////////////////////////
// http://technet.microsoft.com/en-us/library/cc739412(v=WS.10).aspx	//
//////////////////////////////////////////////////////////////////////////

struct TMBR_PTE // raw partition table entry
{
	BYTE		bootIndicator;
	BYTE		startingHead;
	USHORT		startingSector : 6;
	USHORT		startingCylinder : 10;
	BYTE		SystemID;
	BYTE		endingHead;
	USHORT		endingSector : 6;
	USHORT		endingCylinder : 10;
	ULONG		relativeSectors;
	ULONG		totalSectors;
};

struct TMBR // raw master boot record
{
	BYTE			code[440]; // Bootstrap code area
	ULONG			diskSignature;
	USHORT			reserve; // Usually nulls
	TMBR_PTE		ptes[4];
	USHORT			signature;
};

struct TLegacyPTE // raw partition table entry
{
	BYTE		bootIndicator;
	BYTE		startingHead;
	BYTE		startingSector;
	BYTE		startingCylinder;
	BYTE		SystemID;
	BYTE		endingHead;
	BYTE		endingSector;
	BYTE		endingCylinder;
	ULONG		StartingLBA;
	ULONG		SizeInLBA;
};

struct TLegacyMBR // raw master boot record
{
	BYTE					code[440]; // Bootstrap code area
	ULONG					diskSignature;
	USHORT					reserve; // Usually nulls
	TLegacyPTE				ptes[4];
	USHORT					signature;
};

struct TGPTHeader
{
	BYTE		Signature[8];
	USHORT		Revision[2];
	ULONG		HeaderSize;
	ULONG		CRC32Checksum;
	ULONG		Reserved;
	LONGLONG	PrimaryLBA;
	LONGLONG	BackupLBA;
	LONGLONG	FirstUsableLBA;
	LONGLONG	LastUsableLBA;
	GUID		DiskGUID;
	LONGLONG	PartitionEntryLBA;
	ULONG		NumberofPartitionEntries;
	ULONG		SizeofPartitionEntry;
	ULONG		PartitionEntryArrayCRC32;
	BYTE		Reserved1[420];
};


struct TGPT_PTE // raw GUID partition table
{
	GUID		partitionTypeGUID;
	GUID		uniquePartitionGUID;
	ULONGLONG   startingLBA;
	ULONGLONG   endingLBA;
	ULONGLONG   attributeBits;
	WCHAR		partitionName[36];
};

//////////////////////////////////////////////////////////////////////////
// volume boot sector													//
// http://bootmaster.filerecovery.biz/appnote3.html						//
//////////////////////////////////////////////////////////////////////////

typedef struct _TNTFS_BOOT_SECTOR_
{
	BYTE       jmpcode[3];                             // offset=0  : jump to IPL entry point
	BYTE       oem[8];                                 // offset=3  : oem
	USHORT     bytesPerSector;                         // offset=11 : bytes per sector
	BYTE       sectorsPerCluster;                      // offset=13 : sectors per cluster
	USHORT     reservedSectors;                        // offset=14 : reserved sectors
	BYTE       reserved1[3];                           // offset=16 : reserved
	BYTE       notUsed1[2];                            // offset=19 : not used
	BYTE       media;                                  // offset=21 : media descriptor
	BYTE       reserved2[2];                           // offset=22 : reserved
	USHORT     sectorsPerTrack;                        // offset=24 : sectors per track
	USHORT     numberOfHeads;                          // offset=26 : number of heads
	ULONG      hiddenSectors;                          // offset=28 : hidden sectors
	BYTE       notUsed2[4];                            // offset=32 : not used
	BYTE       notUsed3[4];                            // offset=36 : not used
	ULONGLONG  totalSectors;                           // offset=40 : total sectors
	ULONGLONG  mftStartCluster;                        // offset=48 : $MFT start cluster
	ULONGLONG  mftMirrorStartCluster;                  // offset=56 : $MFTMirr start cluster
	ULONG      clustersPerFRS;                         // offset=64 : clusters per FRS (File Record Segment)
	ULONG      clustersPerIndex;                       // offset=68 : clusters per index
	ULONGLONG  volumeSerialNumber;                     // offset=72 : volume serial number
	ULONG      crc;                                    // offset=80 : CRC
	BYTE       bootStrap[0x200-0x054];                 // offset=84 : volume boot code
}TNTFS_BOOT_SECTOR;


//////////////////////////////////////////////////////////////////////////
//	http://en.wikipedia.org/wiki/FAT32#EBPB32							//
//////////////////////////////////////////////////////////////////////////

typedef struct _TFAT32_BPB
{
	WORD			wBytesPerSector;
	BYTE			bSectorsPerCluster;
	WORD			wReservedSectors;
	BYTE			bFATs;
	WORD			wMaxRootDirEntries;
	WORD			wTotalSectors;
	BYTE			bMediaDescriptor;
	WORD			wSectorsPerFAT;
	WORD			wSecotrsPerTrack;
	WORD			wHeads;
	DWORD32			dwHiddenSectors;
	DWORD32			dwTotalSectors32; //For FAT32, must be nonzero, if it is 0, then wTotalSectors must be nonzero

	//the rest is for FAT32 only
	DWORD32			dwSectorsPerFAT32; 
	WORD			wExtFlags32;
	WORD			wFSVer32;
	DWORD32			dwRootCluster32;
	WORD			wFSInfo32;
	WORD			wBkBootSec32;
	BYTE			bReserved32[12];
} TFAT32_BPB;


typedef struct _TFAT32_BOOT_SECTOR
{
	BYTE			JmpCode[3];
	BYTE			oem[8];
	TFAT32_BPB		fat32_bpb;
	BYTE			BootCode[0x200 - sizeof(BYTE)*3  - sizeof(BYTE)*8 - sizeof(TFAT32_BPB)- sizeof(BYTE) - sizeof(WORD)];
	BYTE			reserved0;
	WORD			BootSignature;
}TFAT32_BOOT_SECTOR;

#include <poppack.h>