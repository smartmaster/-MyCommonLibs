/*
 * NTFS data structures and definitions
  */
#pragma once

#ifndef	__NTFS_DATATYPE_H_3B4851CD_4EB5_4612_9288_B5B444B35FBE
#define	__NTFS_DATATYPE_H_3B4851CD_4EB5_4612_9288_B5B444B35FBE

// NTFS Boot Sector BPB

#define	NTFS_SIGNATURE		"NTFS    "


CONST ULONGLONG NTFS_FILE_REF_MASK = 0x0000FFFFFFFFFFFFULL;

#include <pshpack1.h>
typedef struct tagNTFS_BPB
{
	// jump instruction
	BYTE		m_Jmp[3];

	// signature
	BYTE		m_Signature[8];

	// BPB and extended BPB
	WORD		m_BytesPerSector;
	BYTE		m_SectorsPerCluster;
	WORD		m_ReservedSectors;
	BYTE		m_Zeros1[3];
	WORD		m_NotUsed1;
	BYTE		m_MediaDescriptor;
	WORD		m_Zeros2;
	WORD		m_SectorsPerTrack;
	WORD		m_NumberOfHeads;
	DWORD		m_HiddenSectors;
	DWORD		m_NotUsed2;
	DWORD		m_NotUsed3;
	ULONGLONG	m_TotalSectors;
	ULONGLONG	m_LCN_MFT;
	ULONGLONG	m_LCN_MFTMirr;
	INT8		m_ClustersPerFileRecord; //<sonmi01>2014-4-16 ###???
	INT8		m_ClustersPerFileRecord_unused[3];
	INT8		m_ClustersPerIndexBlock;
	INT8		m_ClustersPerIndexBlock_unused[3];
	BYTE		m_VolumeSN[8];

	// boot code
	BYTE		m_Code[430];

	//0xAA55
	BYTE		m__AA;
	BYTE		m__55;
} NTFS_BPB;
#include <poppack.h>


// MFT Indexes
#define	MFT_IDX_MFT				0
#define	MFT_IDX_MFT_MIRR		1
#define	MFT_IDX_LOG_FILE		2
#define	MFT_IDX_VOLUME			3
#define	MFT_IDX_ATTR_DEF		4
#define	MFT_IDX_ROOT			5
#define	MFT_IDX_BITMAP			6
#define	MFT_IDX_BOOT			7
#define	MFT_IDX_BAD_CLUSTER		8
#define	MFT_IDX_SECURE			9
#define	MFT_IDX_UPCASE			10
#define	MFT_IDX_EXTEND			11
#define	MFT_IDX_RESERVED12		12
#define	MFT_IDX_RESERVED13		13
#define	MFT_IDX_RESERVED14		14
#define	MFT_IDX_RESERVED15		15
#define	MFT_IDX_USER			16


/******************************
		File Record
	---------------------
	| File Record Header|
	---------------------
	|    Attribute 1    |
	---------------------
	|    Attribute 2    |
	---------------------
	|      ......       |
	---------------------
	|     0xFFFFFFFF    |
	---------------------
*******************************/

// File Record Header

#define	FILE_RECORD_MAGIC		'ELIF'
#define	FILE_RECORD_FLAG_INUSE	0x01	// File record is in use
#define	FILE_RECORD_FLAG_DIR	0x02	// File record is a directory

#include <pshpack1.h>
typedef struct tagFILE_RECORD_HEADER
{
	DWORD		m_Magic;			// "FILE"
	WORD		m_OffsetOfUS;		// Offset of Update Sequence
	WORD		m_SizeOfUS;		// Size in words of Update Sequence Number & Array
	ULONGLONG	m_LSN;			// $LogFile Sequence Number
	WORD		m_SeqNo;			// Sequence number
	WORD		m_Hardlinks;		// Hard link count
	WORD		m_OffsetOfAttr;	// Offset of the first Attribute
	WORD		m_Flags;			// Flags
	DWORD		m_RealSize;		// Real size of the FILE record
	DWORD		m_AllocSize;		// Allocated size of the FILE record
	ULONGLONG	m_RefToBase;		// File reference to the base FILE record
	WORD		m_NextAttrId;		// Next Attribute Id
	WORD		m_Align;			// Align to 4 byte boundary
	DWORD		m_RecordNo;		// Number of this MFT Record
} FILE_RECORD_HEADER;
#include <poppack.h>

/******************************
		Attribute
	--------------------
	| Attribute Header |
	--------------------
	|  Attribute Data  |
	--------------------
*******************************/

// Attribute Header

#define	ATTR_TYPE_STANDARD_INFORMATION	0x10
#define	ATTR_TYPE_ATTRIBUTE_LIST		0x20
#define	ATTR_TYPE_FILE_NAME				0x30
#define	ATTR_TYPE_OBJECT_ID				0x40
#define	ATTR_TYPE_SECURITY_DESCRIPTOR	0x50
#define	ATTR_TYPE_VOLUME_NAME			0x60
#define	ATTR_TYPE_VOLUME_INFORMATION	0x70
#define	ATTR_TYPE_DATA					0x80
#define	ATTR_TYPE_INDEX_ROOT			0x90
#define	ATTR_TYPE_INDEX_ALLOCATION		0xA0
#define	ATTR_TYPE_BITMAP				0xB0
#define	ATTR_TYPE_REPARSE_POINT			0xC0
#define	ATTR_TYPE_EA_INFORMATION		0xD0
#define	ATTR_TYPE_EA					0xE0
#define	ATTR_TYPE_LOGGED_UTILITY_STREAM	0x100

#define	ATTR_FLAG_COMPRESSED			0x0001
#define	ATTR_FLAG_ENCRYPTED				0x4000
#define	ATTR_FLAG_SPARSE				0x8000

#include <pshpack1.h>
typedef	struct tagATTR_HEADER_COMMON
{
	DWORD		m_Type;			// Attribute Type
	DWORD		m_TotalSize;		// Length (including this header)
	BYTE		m_NonResident;	// 0 - resident, 1 - non resident
	BYTE		m_NameLength;		// name length in words
	WORD		m_NameOffset;		// offset to the name
	WORD		m_Flags;			// Flags
	WORD		m_Id;				// Attribute Id
} ATTR_HEADER_COMMON;
#include <poppack.h>

#include <pshpack1.h>
typedef	struct tagATTR_HEADER_RESIDENT
{
	ATTR_HEADER_COMMON	m_Header;			// Common data structure
	DWORD				m_AttrSize;		// Length of the attribute body
	WORD				m_AttrOffset;		// Offset to the Attribute
	BYTE				m_IndexedFlag;	// Indexed flag
	BYTE				m_Padding;		// Padding
} ATTR_HEADER_RESIDENT;
#include <poppack.h>

#include <pshpack1.h>
typedef struct tagATTR_HEADER_NON_RESIDENT
{
	ATTR_HEADER_COMMON	m_Header;			// Common data structure
	ULONGLONG			m_StartVCN;		// Starting VCN
	ULONGLONG			m_LastVCN;		// Last VCN
	WORD				m_DataRunOffset;	// Offset to the Data Runs
	WORD				m_CompUnitSize;	// Compression unit size
	DWORD				m_Padding;		// Padding
	ULONGLONG			m_AllocSize;		// Allocated size of the attribute
	ULONGLONG			m_RealSize;		// Real size of the attribute
	ULONGLONG			m_IniSize;		// Initialized data size of the stream 
} ATTR_HEADER_NON_RESIDENT;
#include <poppack.h>

// Attribute: STANDARD_INFORMATION

#define	ATTR_STDINFO_PERMISSION_READONLY 	0x00000001
#define	ATTR_STDINFO_PERMISSION_HIDDEN		0x00000002
#define	ATTR_STDINFO_PERMISSION_SYSTEM		0x00000004
#define	ATTR_STDINFO_PERMISSION_ARCHIVE		0x00000020
#define	ATTR_STDINFO_PERMISSION_DEVICE		0x00000040
#define	ATTR_STDINFO_PERMISSION_NORMAL		0x00000080
#define	ATTR_STDINFO_PERMISSION_TEMP		0x00000100
#define	ATTR_STDINFO_PERMISSION_SPARSE		0x00000200
#define	ATTR_STDINFO_PERMISSION_REPARSE		0x00000400
#define	ATTR_STDINFO_PERMISSION_COMPRESSED	0x00000800
#define	ATTR_STDINFO_PERMISSION_OFFLINE		0x00001000
#define	ATTR_STDINFO_PERMISSION_NCI			0x00002000
#define	ATTR_STDINFO_PERMISSION_ENCRYPTED	0x00004000

#include <pshpack1.h>
typedef struct tagATTR_STANDARD_INFORMATION
{
	ULONGLONG	m_CreateTime;		// File creation time
	ULONGLONG	m_AlterTime;		// File altered time
	ULONGLONG	m_MFTTime;		// MFT changed time
	ULONGLONG	m_ReadTime;		// File read time
	DWORD		m_Permission;		// Dos file permission
	DWORD		m_MaxVersionNo;	// Maxim number of file versions
	DWORD		m_VersionNo;		// File version number
	DWORD		m_ClassId;		// Class Id
	DWORD		m_OwnerId;		// Owner Id
	DWORD		m_SecurityId;		// Security Id
	ULONGLONG	m_QuotaCharged;	// Quota charged
	ULONGLONG	m_USN;			// USN Journel
} ATTR_STANDARD_INFORMATION;
#include <poppack.h>

// Attribute: ATTRIBUTE_LIST

#include <pshpack1.h>
typedef struct tagATTR_ATTRIBUTE_LIST
{
	DWORD		m_AttrType;		// Attribute type
	WORD		m_RecordSize;		// Record length
	BYTE		m_NameLength;		// Name length in characters
	BYTE		m_NameOffset;		// Name offset
	ULONGLONG	m_StartVCN;		// Start VCN
	ULONGLONG	m_BaseRef;		// Base file reference to the attribute
	WORD		m_AttrId;			// Attribute Id
} ATTR_ATTRIBUTE_LIST;
#include <poppack.h>

// Attribute: FILE_NAME

#define	ATTR_FILENAME_FLAG_READONLY		0x00000001
#define	ATTR_FILENAME_FLAG_HIDDEN		0x00000002
#define	ATTR_FILENAME_FLAG_SYSTEM		0x00000004
#define	ATTR_FILENAME_FLAG_ARCHIVE		0x00000020
#define	ATTR_FILENAME_FLAG_DEVICE		0x00000040
#define	ATTR_FILENAME_FLAG_NORMAL		0x00000080
#define	ATTR_FILENAME_FLAG_TEMP			0x00000100
#define	ATTR_FILENAME_FLAG_SPARSE		0x00000200
#define	ATTR_FILENAME_FLAG_REPARSE		0x00000400
#define	ATTR_FILENAME_FLAG_COMPRESSED	0x00000800
#define	ATTR_FILENAME_FLAG_OFFLINE		0x00001000
#define	ATTR_FILENAME_FLAG_NCI			0x00002000
#define	ATTR_FILENAME_FLAG_ENCRYPTED	0x00004000
#define	ATTR_FILENAME_FLAG_DIRECTORY	0x10000000
#define	ATTR_FILENAME_FLAG_INDEXVIEW	0x20000000

#define	ATTR_FILENAME_NAMESPACE_POSIX	0x00
#define	ATTR_FILENAME_NAMESPACE_WIN32	0x01
#define	ATTR_FILENAME_NAMESPACE_DOS		0x02

#include <pshpack1.h>
typedef struct tagATTR_FILE_NAME
{
	ULONGLONG	m_ParentRef;		// File reference to the parent directory
	ULONGLONG	m_CreateTime;		// File creation time
	ULONGLONG	m_AlterTime;		// File altered time
	ULONGLONG	m_MFTTime;		// MFT changed time
	ULONGLONG	m_ReadTime;		// File read time
	ULONGLONG	m_AllocSize;		// Allocated size of the file
	ULONGLONG	m_RealSize;		// Real size of the file
	DWORD		m_Flags;			// Flags
	DWORD		m_ER;				// Used by EAs and Reparse
	BYTE		m_NameLength;		// Filename length in characters
	BYTE		m_NameSpace;		// Filename space
	WORD		m_Name[1];		// Filename
} ATTR_FILE_NAME;
#include <poppack.h>

// Attribute: VOLUME_INFORMATION

#define	ATTR_VOLINFO_FLAG_DIRTY		0x0001	// Dirty
#define	ATTR_VOLINFO_FLAG_RLF		0x0002	// Resize logfile
#define	ATTR_VOLINFO_FLAG_UOM		0x0004	// Upgrade on mount
#define	ATTR_VOLINFO_FLAG_MONT		0x0008	// Mounted on NT4
#define	ATTR_VOLINFO_FLAG_DUSN		0x0010	// Delete USN underway
#define	ATTR_VOLINFO_FLAG_ROI		0x0020	// Repair object Ids
#define	ATTR_VOLINFO_FLAG_MBC		0x8000	// Modified by chkdsk

#include <pshpack1.h>
typedef struct tagATTR_VOLUME_INFORMATION
{
	BYTE	m_Reserved1[8];	// Always 0 ?
	BYTE	m_MajorVersion;	// Major version
	BYTE	MinorVersion;	// Minor version
	WORD	m_Flags;			// Flags
	BYTE	m_Reserved2[4];	// Always 0 ?
} ATTR_VOLUME_INFORMATION;
#include <poppack.h>


// Attribute: INDEX_ROOT
/******************************
		INDEX_ROOT
	---------------------
	| Index Root Header |
	---------------------
	|    Index Header   |
	---------------------
	|    Index Entry    |
	---------------------
	|    Index Entry    |
	---------------------
	|      ......       |
	---------------------
*******************************/

#define	ATTR_INDEXROOT_FLAG_SMALL	0x00	// Fits in Index Root File Record
#define	ATTR_INDEXROOT_FLAG_LARGE	0x01	// Index Allocation and Bitmap needed

#include <pshpack1.h>
typedef struct tagATTR_INDEX_ROOT
{
	// Index Root Header
	DWORD		m_AttrType;			// Attribute type (ATTR_TYPE_FILE_NAME: Directory, 0: Index View)
	DWORD		m_CollRule;			// Collation rule
	DWORD		m_IBSize;				// Size of index block
	BYTE		m_ClustersPerIB;		// Clusters per index block (same as BPB?)
	BYTE		m_Padding1[3];		// Padding
	// Index Header
	DWORD		m_EntryOffset;		// Offset to the first index entry, relative to this address(0x10)
	DWORD		m_TotalEntrySize;		// Total size of the index entries
	DWORD		m_AllocEntrySize;		// Allocated size of the index entries
	BYTE		m_Flags;				// Flags
	BYTE		m_Padding2[3];		// Padding
} ATTR_INDEX_ROOT;
#include <poppack.h>


// INDEX ENTRY

#define	INDEX_ENTRY_FLAG_SUBNODE	0x01	// Index entry points to a sub-node
#define	INDEX_ENTRY_FLAG_LAST		0x02	// Last index entry in the node, no Stream

#include <pshpack1.h>
typedef struct tagINDEX_ENTRY
{
	ULONGLONG	m_FileReference;	// Low 6B: MFT record index, High 2B: MFT record sequence number
	WORD		m_Size;			// Length of the index entry
	WORD		m_StreamSize;		// Length of the stream
	BYTE		m_Flags;			// Flags
	BYTE		m_Padding[3];		// Padding
	BYTE		m_Stream[1];		// Stream
	// VCN of the sub node in Index Allocation, Offset = Size - 8
} INDEX_ENTRY;
#include <poppack.h>


#include <pshpack1.h>
typedef struct tagINDEX_ENTRY_RP
{
	WORD		m_OffsetToData;//0x00 2 0x1C Offset to data
	WORD		m_SizeOfData;//0x02 2 0x00 Size of data
	DWORD		m_Padding;//0x04 4 0x00 Padding
	WORD		m_SizeOfIndexEntry;//0x08 2 0x20 Size of Index Entry
	WORD		m_SizeOfIndexKey; //0x0A 2 0x0C Size of Index Key
	WORD		m_Flags; //0x0C 2 Flags
	WORD		m_Padding1; //0x0E 2 0x00 Padding
	DWORD		m_KeyReparseTag; //0x10 4 Key Reparse Tag(and Flags)
	ULONGLONG	m_FileReference; //0x14 8 Key MFT Reference of Reparse Point
	DWORD		m_KeyPadding; //0x1C 4 0x00 Key Padding(align to 8 bytes)
} INDEX_ENTRY_RP;
#include <poppack.h>


// INDEX BLOCK
/******************************
		 INDEX_BLOCK
	-----------------------
	|  Index Block Header |
	-----------------------
	|     Index Header    |
	-----------------------
	|     Index Entry     |
	-----------------------
	|     Index Entry     |
	-----------------------
	|       ......        |
	-----------------------
*******************************/

#define	INDEX_BLOCK_MAGIC		'XDNI'

#include <pshpack1.h>
typedef struct tagINDEX_BLOCK
{
	// Index Block Header
	DWORD		m_Magic;			// "INDX"
	WORD		m_OffsetOfUS;		// Offset of Update Sequence
	WORD		m_SizeOfUS;		// Size in words of Update Sequence Number & Array
	ULONGLONG	m_LSN;			// $LogFile Sequence Number
	ULONGLONG	m_VCN;			// VCN of this index block in the index allocation
	// Index Header
	DWORD		m_EntryOffset;	// Offset of the index entries, relative to this address(0x18)
	DWORD		m_TotalEntrySize;	// Total size of the index entries
	DWORD		m_AllocEntrySize;	// Allocated size of index entries
	BYTE		m_NotLeaf;		// 1 if not leaf node (has children)
	BYTE		m_Padding[3];		// Padding
} INDEX_BLOCK;
#include <poppack.h>

//////////////////////////////////////////////////////////////////////////
//Reparse Tag Flags
#define RP_IS_ALIAS						0x20000000 //Is alias
#define RP_IS_HIGH_LATENCY				0x40000000 //Is high latency
#define RP_IS_MICROSOFT					0x80000000 //Is Microsoft
#define RP_NSS							0x68000005 //NSS
#define RP_NSS_RECOVER					0x68000006 //NSS recover
#define RP_SIS							0x68000007 //SIS
#define RP_DFS							0x68000008 //DFS
#define RP_MOUNT_POINT					0x88000003 //Mount point
#define RP_HSM							0xA8000004 //HSM
#define RP_SYMBOLIC_LINK				0xE8000000 //Symbolic link

#define IsRpTagFlag(var, flag) ((flag) == ((var) & (flag)))



#include <pshpack1.h>
typedef struct tagSYMBOLIC_LINK_REPARSE_DATA
{
	WORD SubstituteNameOffset;//0x00 2 Substitute Name Offset
	WORD SubstituteNameLength;//0x02 2 Substitute Name Length
	WORD PrintNameOffset;//0x04 2 Print Name Offset
	//WORD padding1;
	WORD PrintNameLength;//0x08 2 Print Name Length
	//WORD padding2[3];
	WCHAR PathBuffer[1];//0x10 V Path Buffer
} SYMBOLIC_LINK_REPARSE_DATA;
#include <poppack.h>

#include <pshpack1.h>
typedef struct tagVOLUME_LINK_REPARSE_DATA
{
	WORD SubstituteNameOffset;//0x00 2 Substitute Name Offset
	WORD SubstituteNameLength;//0x02 2 Substitute Name Length
	WORD PrintNameOffset;//0x04 2 Print Name Offset
	//WORD padding1;
	WORD PrintNameLength;//0x08 2 Print Name Length
	//WORD padding2[3];
	WCHAR PathBuffer[1];//0x10 V Path Buffer
} VOLUME_LINK_REPARSE_DATA;
#include <poppack.h>

#include <pshpack1.h>
typedef struct tagREPARSE_POINT
{
	DWORD	ReparseTypeFlags;//0x00 4 Reparse Type(and Flags)
	WORD	ReparseDataLength;//	0x04 2 Reparse Data Length
	WORD	Padding;//	0x06 2 Padding(align to 8 bytes)
	//WCHAR	ReparseData[1];//	0x08 V Reparse Data(a)
	union REPARSE_DATA
	{
		SYMBOLIC_LINK_REPARSE_DATA	symbolic;
		VOLUME_LINK_REPARSE_DATA	volume;
	} u;
} REPARSE_POINT;
#include <poppack.h>

#include <pshpack1.h>
typedef struct tagREPARSE_POINT_3RDP
{
	DWORD ReparseTypeFlags;//0x00 4 Reparse Type(and Flags)
	WORD ReparseDataLength;//0x04 2 Reparse Data Length
	WORD Padding;//0x06 2 Padding(align to 8 bytes)
	GUID ReparseGUID;//0x08 16 Reparse GUID
	//WCHAR ReparseData[1];//0x18 V Reparse Data(a)
	union REPARSE_DATA
	{
		SYMBOLIC_LINK_REPARSE_DATA	symbolic;
		VOLUME_LINK_REPARSE_DATA	volume;
	} u;
} REPARSE_POINT_3RDP;
#include <poppack.h>



#endif
