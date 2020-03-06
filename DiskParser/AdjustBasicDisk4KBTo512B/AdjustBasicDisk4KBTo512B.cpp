#include "stdafx.h"

#include "..\..\PublicHeader\PublicDTraceLib.h"
#include "..\..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\..\PublicHeader\PublicVMemPtr.h"

#include "DiskVolumeStructure.h"
#include "CrcCalc.h"
#include "AdjustBasicDisk4KBTo512B.h"
#include "..\LDMParser\Ldm.h"

namespace { //anonymous namespace

static CONST INT __4KB = 4096;
static CONST INT __512B = 512;
static CONST INT SCALAR = __4KB / __512B;

static HRESULT ReadAtOffsetHelper(IFileDevice * pDiskRW, LONGLONG Offset, ULONG nNumberOfBytesToRead, BYTE* pBuffer )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	LONGLONG NewFilePointer = -1;
	hr = pDiskRW->SetPointerEx(Offset, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesRead = 0;
	hr = pDiskRW->Read(nNumberOfBytesToRead, &NumberOfBytesRead, pBuffer);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);

	return hr;
}

static HRESULT WriteAtOffsetHelper(IFileDevice * pDiskRW, LONGLONG Offset, ULONG nNumberOfBytesToWrite, CONST BYTE* pBuffer)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);

	LONGLONG NewFilePointer = -1;
	hr = pDiskRW->SetPointerEx(Offset, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesWritten = 0;
	hr = pDiskRW->Write(nNumberOfBytesToWrite, &NumberOfBytesWritten, pBuffer);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}





static VOID AdjustBasicMBRPTE4KBTo512B(TMBR_PTE & pte)
{
	pte.relativeSectors *= SCALAR;
	pte.totalSectors *= SCALAR;
}

static VOID AdjustBasicGPTPTE4KBTo512B(TGPT_PTE & pte)
{
	pte.startingLBA *= SCALAR;
	pte.endingLBA *= SCALAR; pte.endingLBA += (SCALAR - 1); 
}

static VOID AdjustBasicGPTPTETable4KBTo512B(TGPT_PTE * pte, INT MaxPartitionCount)
{
	for (INT ii = 0; ii < MaxPartitionCount; ++ ii)
	{
		if (IsEqualGUID(GUID_NULL, pte[ii].uniquePartitionGUID))
		{
			break;
		}
		AdjustBasicGPTPTE4KBTo512B(pte[ii]);
	}
}

static ULONG CalcGPTPartitionTableCrc(TGPT_PTE * pGPTTable, ULONG TableLenInBytes)
{
	CCrcCalc crcCal;
	crcCal.InitCrc32Table();
	ULONG PartitionEntryArrayCRC32 = crcCal.GetCrc((CONST BYTE*)pGPTTable, TableLenInBytes);
	return PartitionEntryArrayCRC32;
}

static ULONG UpdateGPTHeaderCrc(TGPTHeader * pGPTHeader)
{
	CCrcCalc crcCal;
	crcCal.InitCrc32Table();
	ULONG CRC32ChecksumOriginal = pGPTHeader->CRC32Checksum; 
	pGPTHeader->CRC32Checksum = 0;
	ULONG CRC32Checksum = crcCal.GetCrc((CONST BYTE*)pGPTHeader, pGPTHeader->HeaderSize);
	pGPTHeader->CRC32Checksum = CRC32Checksum;
	return CRC32ChecksumOriginal;
}

static VOID MoveDiskMetaData(BYTE * p512BBuffer)
{
	TGPTHeader * pGPTHeader = (TGPTHeader *)(p512BBuffer + __4KB);
	ULONG GptTableLenInBytes = pGPTHeader->SizeofPartitionEntry * pGPTHeader->NumberofPartitionEntries;
	MoveMemory(p512BBuffer + __512B, pGPTHeader, __512B);

	TGPT_PTE * pGPTTable = (TGPT_PTE *)(p512BBuffer + __4KB + __4KB);
	MoveMemory(p512BBuffer + __512B + __512B, pGPTTable, GptTableLenInBytes);
}

static VOID AdjustBasicGPT4KBTo512B(CONST BYTE * p4KBBuffer, BYTE * p512BBuffer, INT BufferSize)
{
	CopyMemory(p512BBuffer, p4KBBuffer, BufferSize);
	p4KBBuffer = NULL;
	MoveDiskMetaData(p512BBuffer);

	//////////////////////////////////////////////////////////////////////////
	//<sonmi01>2012-11-1 #adjust legacy MBR sector
	TLegacyMBR * pPotectivMBR = (TLegacyMBR *)(p512BBuffer);
	//pPotectivMBR->ptes[0].startingSector *= SCALAR;
	//pPotectivMBR->ptes[0].StartingLBA *= SCALAR;

	//////////////////////////////////////////////////////////////////////////
	//<sonmi01>2012-11-1 #adjust GPT header
	//RAW_GPT_HEADER_T * pGPTHeader = (RAW_GPT_HEADER_T *)(p512BBuffer + __4KB);
	TGPTHeader * pGPTHeader = (TGPTHeader *)(p512BBuffer + __512B);
	//pGPTHeader->PrimaryLBA *= SCALAR;
	pGPTHeader->BackupLBA *= SCALAR; pGPTHeader->BackupLBA += (SCALAR - 1);

	pGPTHeader->FirstUsableLBA *= SCALAR;
	pGPTHeader->LastUsableLBA *= SCALAR; pGPTHeader->LastUsableLBA += (SCALAR - 1); 

	//pGPTHeader->PartitionEntryLBA *= SCALAR;

	//////////////////////////////////////////////////////////////////////////
	//<sonmi01>2012-11-1 #adjust GPT disk partition table
	//RAW_GPT_ENTRY_T * pGPTTable = (RAW_GPT_ENTRY_T *)(p512BBuffer + __4KB + __4KB);
	TGPT_PTE * pGPTTable = (TGPT_PTE *)(p512BBuffer + __512B + __512B);
	AdjustBasicGPTPTETable4KBTo512B(pGPTTable, pGPTHeader->NumberofPartitionEntries);

	//////////////////////////////////////////////////////////////////////////
	//<sonmi01>2012-11-1 #adjust GPT disk partition table CRC and GPT header CRC
	ULONG PartitionEntryArrayCRC32 = CalcGPTPartitionTableCrc(pGPTTable, pGPTHeader->SizeofPartitionEntry * pGPTHeader->NumberofPartitionEntries);
	pGPTHeader->PartitionEntryArrayCRC32 = PartitionEntryArrayCRC32;
	UpdateGPTHeaderCrc(pGPTHeader);
}


static VOID AdjustBasicMBR4KBTo512B(CONST BYTE * p4KBBuffer, BYTE * p512BBuffer, INT BufferSize)
{
	CopyMemory(p512BBuffer, p4KBBuffer, BufferSize);
	p4KBBuffer = NULL;

	TMBR * pMBR = (TMBR *)(p512BBuffer);
	CONST INT MAX_PART_COUNT = 4;
	for (INT ii = 0; ii < MAX_PART_COUNT; ++ ii)
	{
		TMBR_PTE & pte = pMBR->ptes[ii];
		if (0 == pte.relativeSectors || 0 == pte.totalSectors)
		{
			break;
		}
		AdjustBasicMBRPTE4KBTo512B(pte);
	}
}

static BOOL IsGPTDisk(CONST BYTE * pDiskHeader)
{
	TLegacyMBR * pPotectivMBR = (TLegacyMBR *)(pDiskHeader);
	TLegacyPTE * pPTE = pPotectivMBR->ptes;
	
	CONST BYTE GPT_PARTITION_ID = 0xee;
	BOOL bRet = FALSE;
	for (INT ii = 0; ii < 4; ++ ii)
	{
		if (GPT_PARTITION_ID == pPTE[ii].SystemID)
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

static BOOL IsMBRDisk(CONST BYTE * pDiskHeader)
{
	return !IsGPTDisk(pDiskHeader);
}

static BOOL IsDynamicGPTPartitionTable(TGPT_PTE * pPTE)
{
	//{5808C8AA-7E8F-42E0-85D2-E1E90434CFB3}
	static const GUID LDM_METADATA_PARTITION_ID  = 
	{ 0x5808C8AA, 0x7E8F, 0x42E0, {0x85, 0xD2, 0xE1, 0xE9, 0x04, 0x34, 0xCF, 0xB3 } };
	
	// {AF9B60A0-1431-4F62-BC68-3311714A69AD}
	static const GUID LDM_DATA_PARTITION_ID  = 
	{ 0xAF9B60A0, 0x1431, 0x4F62, { 0xBC, 0x68, 0x33, 0x11, 0x71, 0x4A, 0x69, 0xAD } };

	BOOL bRet = FALSE;
	CONST INT MAX_PART_COUNT = 128;
	for (INT ii = 0; ii < MAX_PART_COUNT; ++ ii)
	{
		if (IsEqualGUID(GUID_NULL, pPTE[ii].partitionTypeGUID) || IsEqualGUID(GUID_NULL, pPTE[ii].uniquePartitionGUID))
		{
			break;
		}

		if (IsEqualGUID(LDM_METADATA_PARTITION_ID, pPTE[ii].partitionTypeGUID) || IsEqualGUID(LDM_DATA_PARTITION_ID, pPTE[ii].partitionTypeGUID))
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

static BOOL IsMBRDynamicPartition(BYTE PartID)
{
	CONST BYTE DYNAMIC_PARTITION_ID = 0x42;
	return (DYNAMIC_PARTITION_ID == PartID);
}

static BOOL IsDynamicMBRDisk(CONST BYTE * pDiskHeader)
{
	TMBR * pMBR = (TMBR *)(pDiskHeader);
	BOOL bRet = FALSE;
	for (INT ii = 0; ii < 4; ++ ii)
	{
		if (IsMBRDynamicPartition(pMBR->ptes[ii].SystemID))
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

static BOOL IsExtendedPartition(BYTE SysID)
{
	return (0x5 == SysID || 0xf == SysID);
}



static BOOL Is4KBSectorBasicGPTDisk(IFileDevice * pDiskRW)
{
	//////////////////////////////////////////////////////////////////////////
	CONST INT BUFFER_SIZE = 64 * 1024;
	CVMemPtr<BYTE> apDiskHeaderBuffer;
	apDiskHeaderBuffer.Allocate(BUFFER_SIZE);
	if (NULL == apDiskHeaderBuffer.m_pData)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	HRESULT hr = ReadAtOffsetHelper(pDiskRW, 0, BUFFER_SIZE, apDiskHeaderBuffer.m_pData);
	if (FAILED(hr))
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	if (!IsGPTDisk(apDiskHeaderBuffer.m_pData))
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	CONST BYTE EFI_PART[] = "EFI PART";
	TGPTHeader * pGPTHeader512B = (TGPTHeader *)(apDiskHeaderBuffer.m_pData + __512B);
	if (0 == memcmp(EFI_PART, pGPTHeader512B->Signature, _countof(EFI_PART) - 1))
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	TGPTHeader * pGPTHeader4KB = (TGPTHeader *)(apDiskHeaderBuffer.m_pData + __4KB);
	if (0 != memcmp(EFI_PART, pGPTHeader4KB->Signature, _countof(EFI_PART) - 1))
	{
		return FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	TGPT_PTE * pPTE = (TGPT_PTE *)(apDiskHeaderBuffer.m_pData + __4KB + __4KB);
	if (IsDynamicGPTPartitionTable(pPTE))
	{
		return FALSE;
	}

	return TRUE;
}

static BOOL IsValidBootRecord(CONST BYTE * pBoostSectorBuffer)
{
	WORD __AA55 = 0xAA55;
	WORD * pWord = (WORD *)pBoostSectorBuffer;
	return (__AA55 == pWord[255]);
}


static BOOL Is4KBSectorExtendedPartition(IFileDevice * pDdisk, LONGLONG OffsetLBA)
{
	BOOL bRet = FALSE;

	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apBuffer;
	CONST INT BUFFER_SIZE = 64 * 1024;
	apBuffer.Allocate(BUFFER_SIZE);
	if (0 == apBuffer.m_pData)
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG __512BOffset = OffsetLBA;
	__512BOffset *= __512B;
	hr = ReadAtOffsetHelper(pDdisk, __512BOffset, BUFFER_SIZE, apBuffer.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT("Offset=%I64d, ToRead=%d"), __512BOffset, BUFFER_SIZE);
		LEAVE_BLOCK(0);
	}

	if (IsValidBootRecord(apBuffer.m_pData))
	{
		D_INFO(0, TEXT("Found a boot EBR at %I64d * 512B, assume it is a 512B sector disk"), OffsetLBA);
		LEAVE_BLOCK(0); // 512B sector disk, not 4KB sector disk
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG __4KBOffset = OffsetLBA;
	__4KBOffset *= __4KB;
	hr = ReadAtOffsetHelper(pDdisk, __4KBOffset, BUFFER_SIZE, apBuffer.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT("Offset=%I64d, ToRead=%d"), __4KBOffset, BUFFER_SIZE);
		LEAVE_BLOCK(0);
	}

	if (IsValidBootRecord(apBuffer.m_pData))
	{
		bRet = TRUE; // 4KB sector disk
		D_INFO(0, TEXT("Found a EBR at %I64d * 4KB, assume it is a 4KB sector disk"), OffsetLBA);
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return bRet;
}


static BOOL IsFATSignature(BYTE * pSig)
{
	static CONST BYTE MSDOS_SIG[] = "MSDOS";
	static CONST BYTE MSWIN_SIG[] = "MSWIN";
	static CONST BYTE IBM_SIG[] = "IBM";

	return (0 == memcmp(MSDOS_SIG, pSig, _countof(MSDOS_SIG) - 1) || 
		0 == memcmp(MSWIN_SIG, pSig, _countof(MSWIN_SIG) - 1) ||
		0 == memcmp(IBM_SIG, pSig, _countof(IBM_SIG) - 1));
}

static BOOL IsReFSSignature(BYTE * pSig)
{
	static CONST BYTE ReFS_SIG[] = "ReFS";
	return (0 == memcmp(ReFS_SIG, pSig, _countof(ReFS_SIG) - 1));
}

static BOOL IsNTFSSignature(BYTE * pSig)
{
	static CONST BYTE NTFS_SIG[] = "NTFS";
	return (0 == memcmp(NTFS_SIG, pSig, _countof(NTFS_SIG) - 1));
}




static BOOL IsValidFSBootSector(CONST BYTE * pBoostSectorBuffer)
{
	TNTFS_BOOT_SECTOR * pBootSector = (TNTFS_BOOT_SECTOR *)pBoostSectorBuffer;

	return (IsFATSignature(pBootSector->oem) || 
		IsReFSSignature(pBootSector->oem) || 
		IsNTFSSignature(pBootSector->oem));
}

static BOOL Is4KBSectorDataPartition(IFileDevice * pDdisk, LONGLONG OffsetLBA)
{
	BOOL bRet = FALSE;

	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apBuffer;
	CONST INT BUFFER_SIZE = 64 * 1024;
	apBuffer.Allocate(BUFFER_SIZE);
	if (0 == apBuffer.m_pData)
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG __512BOffset = OffsetLBA;
	__512BOffset *= __512B;
	hr = ReadAtOffsetHelper(pDdisk, __512BOffset, BUFFER_SIZE, apBuffer.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT("Offset=%I64d, ToRead=%d"), __512BOffset, BUFFER_SIZE);
		LEAVE_BLOCK(0);
	}

	if (IsValidFSBootSector(apBuffer.m_pData))
	{
		D_INFO(0, TEXT("Found a boot sector at %I64d * 512B, assume it is a 512B sector disk"), OffsetLBA);
		LEAVE_BLOCK(0); // 512B sector disk, not 4KB sector disk
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG __4KBOffset = OffsetLBA;
	__4KBOffset *= __4KB;
	hr = ReadAtOffsetHelper(pDdisk, __4KBOffset, BUFFER_SIZE, apBuffer.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT("Offset=%I64d, ToRead=%d"), __4KBOffset, BUFFER_SIZE);
		LEAVE_BLOCK(0);
	}

	if (IsValidFSBootSector(apBuffer.m_pData))
	{
		bRet = TRUE; // 4KB sector disk
		D_INFO(0, TEXT("Found a boot sector at %I64d * 4KB, assume it is a 4KB sector disk"), OffsetLBA);
		LEAVE_BLOCK(0); 
	}

	END_BLOCK(0);

	return bRet;
}


static INT Test4KBPartitions(IFileDevice * pDiskRW, CONST BYTE * pDiskHeader)
{
	TMBR * pMBR = (TMBR *)pDiskHeader;
	TMBR_PTE * pPTE = pMBR->ptes;
	INT Count = 0;
	for (INT ii = 0; ii < 4; ++ ii)
	{
		if (0 == pPTE[ii].relativeSectors || 0 == pPTE[ii].totalSectors)
		{
			break;
		}

		if (IsExtendedPartition(pPTE[ii].SystemID))
		{
			Count += Is4KBSectorExtendedPartition(pDiskRW, pPTE[ii].relativeSectors);
		}
		else
		{
			Count += Is4KBSectorDataPartition(pDiskRW, pPTE[ii].relativeSectors);
		}
	}

	return Count;
}


static BOOL Is4KBSectorBasicMBRDisk(IFileDevice * pDiskRW)
{
	//////////////////////////////////////////////////////////////////////////
	CONST INT BUFFER_SIZE = 64 * 1024;
	CVMemPtr<BYTE> apDiskHeaderBuffer;
	apDiskHeaderBuffer.Allocate(BUFFER_SIZE);
	if (NULL == apDiskHeaderBuffer.m_pData)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	HRESULT hr = ReadAtOffsetHelper(pDiskRW, 0, BUFFER_SIZE, apDiskHeaderBuffer.m_pData);
	if (FAILED(hr))
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	if (!IsMBRDisk(apDiskHeaderBuffer.m_pData))
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	if (IsDynamicMBRDisk(apDiskHeaderBuffer.m_pData))
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	INT __4KBPartCount = Test4KBPartitions(pDiskRW, apDiskHeaderBuffer.m_pData);
	D_INFO(0, TEXT("Found %d partitions of 4KB sector"), __4KBPartCount);
	if (0 == __4KBPartCount)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}

static VOID AdjustNTFS4KBTo512B(CONST BYTE * p4KBBuffer, BYTE * p512BBuffer, INT BufferSize)
{
	CopyMemory(p512BBuffer, p4KBBuffer, BufferSize);
	p4KBBuffer = NULL;

	TNTFS_BOOT_SECTOR * pBootSector = (TNTFS_BOOT_SECTOR *)p512BBuffer;

	pBootSector->bytesPerSector = __512B;
	pBootSector->sectorsPerCluster *= SCALAR;
	pBootSector->hiddenSectors *= SCALAR;
	pBootSector->totalSectors *= SCALAR;
}


static VOID AdjustFAT4KBTo512B(CONST BYTE * p4KBBuffer, BYTE * p512BBuffer, INT BufferSize)
{

	CopyMemory(p512BBuffer, p4KBBuffer, BufferSize);
	p4KBBuffer = NULL;

	TFAT32_BOOT_SECTOR * pFat32BootSector = (TFAT32_BOOT_SECTOR *)p512BBuffer;

	pFat32BootSector->fat32_bpb.wBytesPerSector = __512B;
	pFat32BootSector->fat32_bpb.bSectorsPerCluster *= SCALAR;
	pFat32BootSector->fat32_bpb.wReservedSectors *= SCALAR;
	pFat32BootSector->fat32_bpb.dwHiddenSectors *= SCALAR;
	pFat32BootSector->fat32_bpb.dwTotalSectors32 *= SCALAR;
	pFat32BootSector->fat32_bpb.dwSectorsPerFAT32 *= SCALAR;
}


//////////////////////////////////////////////////////////////////////////
// http://en.wikipedia.org/wiki/File_Allocation_Table#BPB			
// 0x052 0x47 8 Cf. 0x036 for FAT12/FAT16 (File system type, padded with blanks (0x20), e.g., "FAT32   ") 
//////////////////////////////////////////////////////////////////////////
static BOOL IsFAT32FSTag(CONST BYTE * pBootSector)
{
	CONST BYTE FS_FAT32[] = "FAT32";
	return 0 == memcmp(FS_FAT32, &(pBootSector[0x52]), _countof(FS_FAT32) - 1);
}


static LPCTSTR MBRPartitionTypeString(BYTE PartID)
{
	switch (PartID)
	{
	case 0x01: return TEXT("FAT12 primary partition or logical drive (fewer than 32,680 sectors in the volume)"); //0x01	FAT12 primary partition or logical drive (fewer than 32,680 sectors in the volume)
	case 0x04: return TEXT("FAT16 partition or logical drive (32,680每65,535 sectors or 16MB每33MB)"); //0x04	FAT16 partition or logical drive (32,680每65,535 sectors or 16MB每33MB)
	case 0x05: return TEXT("Extended partition"); //0x05	Extended partition
	case 0x06: return TEXT("BIGDOS FAT16 partition or logical drive (33MB每4GB)"); //0x06	BIGDOS FAT16 partition or logical drive (33MB每4GB)
	case 0x07: return TEXT("Installable File System (NTFS partition or logical drive)"); //0x07	Installable File System (NTFS partition or logical drive)
	case 0x0B: return TEXT("FAT32 partition or logical drive"); //0x0B	FAT32 partition or logical drive
	case 0x0C: return TEXT("FAT32 partition or logical drive using BIOS INT 13h extensions"); //0x0C	FAT32 partition or logical drive using BIOS INT 13h extensions
	case 0x0E: return TEXT("BIGDOS FAT16 partition or logical drive using BIOS INT 13h extensions"); //0x0E	BIGDOS FAT16 partition or logical drive using BIOS INT 13h extensions
	case 0x0F: return TEXT("xtended partition using BIOS INT 13h extensions"); //0x0F	Extended partition using BIOS INT 13h extensions
	case 0x12: return TEXT("EISA partition or OEM partition"); //0x12	EISA partition or OEM partition
	case 0x42: return TEXT("Dynamic volume"); //0x42	Dynamic volume
	case 0x84: return TEXT("Power management hibernation partition"); //0x84	Power management hibernation partition
	case 0x86: return TEXT("Multidisk FAT16 volume created by using WindowsNT4.0"); //0x86	Multidisk FAT16 volume created by using WindowsNT4.0
	case 0x87: return TEXT("Multidisk NTFS volume created by using WindowsNT4.0"); //0x87	Multidisk NTFS volume created by using WindowsNT4.0
	case 0xA0: return TEXT("Laptop hibernation partition"); //0xA0	Laptop hibernation partition
	case 0xDE: return TEXT("Dell OEM partition"); //0xDE	Dell OEM partition
	case 0xFE: return TEXT("IBM OEM partition"); //0xFE	IBM OEM partition
	case 0xEE: return TEXT("GPT partition"); //0xEE	GPT partition
	case 0xEF: return TEXT("EFI System partition on an MBR disk"); //0xEF	EFI System partition on an MBR disk
	default:	return TEXT("Unrecognized partition");
	}	
}



static HRESULT Adjust4KBVolume(IFileDevice * pDiskRW, LONGLONG VolumeOffset/*, BYTE PartID*/)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	CONST INT BUFFER_SIZE = 64 * 1024;
	CVMemPtr<BYTE> apVolumeBootSector4KB;
	apVolumeBootSector4KB.Allocate(BUFFER_SIZE);
	if (NULL == apVolumeBootSector4KB.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = ReadAtOffsetHelper(pDiskRW, VolumeOffset, BUFFER_SIZE, apVolumeBootSector4KB.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apVolumeBootSector512B;
	apVolumeBootSector512B.Allocate(BUFFER_SIZE);
	if (NULL == apVolumeBootSector512B.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	BOOL bAdjusted = FALSE;
	TNTFS_BOOT_SECTOR * pBootSector = (TNTFS_BOOT_SECTOR *)apVolumeBootSector4KB.m_pData;
	if (IsNTFSSignature(pBootSector->oem))
	{
		bAdjusted = TRUE;
		//D_INFO(0, TEXT("the volume at offset %I64d is a NTFS volume, adjust 4KB sector to 512B [PartitionID=%d, Description=%s]"), VolumeOffset, PartID, MBRPartitionTypeString(PartID));
		D_INFO(0, TEXT("the volume at offset %I64d is a NTFS volume, adjust 4KB sector to 512B"), VolumeOffset);
		AdjustNTFS4KBTo512B(apVolumeBootSector4KB.m_pData, apVolumeBootSector512B.m_pData, BUFFER_SIZE);
	}
	else if (IsReFSSignature(pBootSector->oem))
	{
		//D_INFO(0, TEXT("the volume at offset %I64d is a ReFS volume, no need to adjust 4KB sector to 512B [PartitionID=%d, Description=%s]"), VolumeOffset, PartID, MBRPartitionTypeString(PartID));
		D_INFO(0, TEXT("the volume at offset %I64d is a ReFS volume, no need to adjust 4KB sector to 512B"), VolumeOffset);
	} 
	else if (IsFATSignature(pBootSector->oem))
	{
		TFAT32_BOOT_SECTOR  * pFat32BootSec = (TFAT32_BOOT_SECTOR *)pBootSector;
		if (IsFAT32FSTag((CONST BYTE *)pFat32BootSec)) //<sonmi01>2012-11-6 #to do
		{
			bAdjusted = TRUE;
			//D_INFO(0, TEXT("the volume at offset %I64d is a FAT32 volume, adjust 4KB sector to 512B [PartitionID=%d, Description=%s]"), VolumeOffset, PartID, MBRPartitionTypeString(PartID));
			D_INFO(0, TEXT("the volume at offset %I64d is a FAT32 volume, adjust 4KB sector to 512B"), VolumeOffset);
			AdjustFAT4KBTo512B(apVolumeBootSector4KB.m_pData, apVolumeBootSector512B.m_pData, BUFFER_SIZE);
		}
		else
		{
			//D_INFO(0, TEXT("the volume at offset %I64d is a FAT12/16 volume, not support to adjust 4KB sector to 512B [PartitionID=%d, Description=%s]"), VolumeOffset, PartID, MBRPartitionTypeString(PartID));
			D_INFO(0, TEXT("the volume at offset %I64d is a FAT12/16 volume, not support to adjust 4KB sector to 512B"), VolumeOffset);
		}
	}
	else
	{
		//D_INFO(0, TEXT("the volume at offset %I64d cannot be recognized, not support to adjust 4KB sector to 512B [PartitionID=%d, Description=%s]"), VolumeOffset, PartID, MBRPartitionTypeString(PartID));
		D_INFO(0, TEXT("the volume at offset %I64d cannot be recognized, not support to adjust 4KB sector to 512B"), VolumeOffset);
	}

	//////////////////////////////////////////////////////////////////////////
	if (bAdjusted)
	{
		hr = WriteAtOffsetHelper(pDiskRW, VolumeOffset, BUFFER_SIZE, apVolumeBootSector512B.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	END_BLOCK(0);

	return hr;
}




HRESULT AdjustBasicGPT4KBDisk(IFileDevice * pDiskRW)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST INT BUFFER_SIZE = 64 * 1024;
	CVMemPtr<BYTE> apDiskHeader4KB;
	apDiskHeader4KB.Allocate(BUFFER_SIZE);
	if (NULL == apDiskHeader4KB.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = ReadAtOffsetHelper(pDiskRW, 0, BUFFER_SIZE, apDiskHeader4KB.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//if (!Is4KBSectorBasicGPTDisk(apDiskHeader4KB.m_pData))
	//{
	//	D_INFO(0, TEXT("Not a 4KBSectorBasicGPTDisk, skipping adjusting"));
	//	LEAVE_BLOCK(0);
	//}
	
	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apDiskHeader512B;
	apDiskHeader512B.Allocate(BUFFER_SIZE);
	if (NULL == apDiskHeader512B.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	
	AdjustBasicGPT4KBTo512B(apDiskHeader4KB.m_pData, apDiskHeader512B.m_pData, BUFFER_SIZE);

	//////////////////////////////////////////////////////////////////////////
	hr = WriteAtOffsetHelper(pDiskRW, 0, BUFFER_SIZE, apDiskHeader512B.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	TGPT_PTE * pGPTTable = (TGPT_PTE *)(apDiskHeader512B.m_pData + __512B + __512B);
	for (INT ii = 0; ii < 128; ++ ii)
	{
		if (IsEqualGUID(GUID_NULL, pGPTTable[ii].uniquePartitionGUID))
		{
			break;
		}

		LONGLONG VolumeOffset = pGPTTable[ii].startingLBA * __512B;
		D_INFO(0, TEXT("GPT partition number %d is at offset %I64d"), ii, VolumeOffset);
		HRESULT hrVolume = Adjust4KBVolume(pDiskRW, VolumeOffset/*, 0xee*/);
		if (FAILED(hrVolume))
		{
			D_SET_LAST_STATUS(hrVolume, -1);
			D_API_ERR(0, TEXT("Adjust4KBVolume"), hrVolume, TEXT("partition number is %d at offset %I64d"), ii, VolumeOffset);
			//LEAVE_BLOCK(xxxx);
			hr = hrVolume;
		}
	}
	
	END_BLOCK(0);

	return hr;
}


//////////////////////////////////////////////////////////////////////////
//FirstEBROffset must be initialize to ZERO
HRESULT AdjustBasicMBR4KBDisk(IFileDevice * pDiskRW, LONGLONG CurrentMBROffset, LONGLONG & FirstEBROffset)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST INT BUFFER_SIZE = 64 * 1024;
	CVMemPtr<BYTE> apDiskHeader4KB;
	apDiskHeader4KB.Allocate(BUFFER_SIZE);
	if (NULL == apDiskHeader4KB.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = ReadAtOffsetHelper(pDiskRW, CurrentMBROffset, BUFFER_SIZE, apDiskHeader4KB.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//if (!Is4KBSectorBasicMBRDisk(pDiskRW, apDiskHeader4KB.m_pData))
	//{
	//	D_INFO(0, TEXT("Not a 4KBSectorBasicMBRDisk, skipping adjusting"));
	//	LEAVE_BLOCK(0);
	//}

	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apDiskHeader512B;
	apDiskHeader512B.Allocate(BUFFER_SIZE);
	if (NULL == apDiskHeader512B.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	AdjustBasicMBR4KBTo512B(apDiskHeader4KB.m_pData, apDiskHeader512B.m_pData, BUFFER_SIZE);

	//////////////////////////////////////////////////////////////////////////
	hr = WriteAtOffsetHelper(pDiskRW, CurrentMBROffset, BUFFER_SIZE, apDiskHeader512B.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CONST INT MAX_PART_COUNT = 4;
	TMBR * pMBR = (TMBR *)(apDiskHeader512B.m_pData);
	TMBR_PTE * pPTE = pMBR->ptes;
	for (INT ii = 0; ii < MAX_PART_COUNT; ++ ii)
	{
		if (pPTE[ii].relativeSectors && pPTE[ii].totalSectors)
		{
			HRESULT hrPart = S_OK;
			LONGLONG SubMbrOffset = pPTE[ii].relativeSectors;
			SubMbrOffset *= __512B;

			////////////////////////////////////////////////////////////////////////////
			////http://en.wikipedia.org/wiki/Extended_boot_record what a damned design//
			////////////////////////////////////////////////////////////////////////////
			if (IsExtendedPartition(pPTE[ii].SystemID))
			{
				if (0 == FirstEBROffset)
				{
					FirstEBROffset = SubMbrOffset;
					D_INFO(0, TEXT("Found the first extended partition at offset %I64d (0x%I64x), partition number %d"), SubMbrOffset, SubMbrOffset, ii);
				}
				else
				{
					SubMbrOffset += FirstEBROffset; //the offset of the (2nd, 3rd, 4th ...) child extended partition is always relative to that of the first extended partition
					D_INFO(0, TEXT("Found extended partition at offset %I64d (0x%I64x), partition number %d"), SubMbrOffset, SubMbrOffset, ii);
				}
				hrPart = AdjustBasicMBR4KBDisk(pDiskRW, SubMbrOffset, FirstEBROffset); //adjust logical drive in extended partition recursively
			} 
			else
			{
				SubMbrOffset += CurrentMBROffset;//the data partition offset is relative to its own extended partition
				D_INFO(0, TEXT("Found partition at offset %I64d [PartitionID=%d, Description=%s]"), SubMbrOffset, pPTE[ii].SystemID, MBRPartitionTypeString(pPTE[ii].SystemID));
				hrPart = Adjust4KBVolume(pDiskRW, SubMbrOffset/*, pPTE[ii].SystemID*/);
			}
			
			if (FAILED(hrPart))
			{
				hr = hrPart;
			}
		}
	}

	END_BLOCK(0);

	return hr;
}

} //anonymous namespace end


namespace SmartLib
{
	HRESULT GetDiskInfo(IFileDevice * pDiskRW, DiskInfo & di)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		CONST INT BUFFER_SIZE = 64 * 1024;
		CVMemPtr<BYTE> apDiskHeaderBuffer;
		apDiskHeaderBuffer.Allocate(BUFFER_SIZE);
		if (NULL == apDiskHeaderBuffer.m_pData)
		{
			return E_OUTOFMEMORY;
		}

		//////////////////////////////////////////////////////////////////////////
		HRESULT hr = ReadAtOffsetHelper(pDiskRW, 0, BUFFER_SIZE, apDiskHeaderBuffer.m_pData);
		if (FAILED(hr))
		{
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		INT __4KBPartCount = Test4KBPartitions(pDiskRW, apDiskHeaderBuffer.m_pData);
		D_INFO(0, TEXT("Found %d partitions of 4KB sector"), __4KBPartCount);
		if (0 == __4KBPartCount)
		{
			di.m_sectorSize = __512B;
		}
		else
		{
			di.m_sectorSize = __4KB;
		}

		//////////////////////////////////////////////////////////////////////////
		di.m_isGPTDisk = IsGPTDisk(apDiskHeaderBuffer.m_pData);
		if (di.m_isGPTDisk)
		{
			CONST BYTE EFI_PART[] = "EFI PART";
			TGPTHeader * pGPTHeader512B = (TGPTHeader *)(apDiskHeaderBuffer.m_pData + __512B);
			if (0 == memcmp(EFI_PART, pGPTHeader512B->Signature, _countof(EFI_PART) - 1))
			{
				di.m_sectorSize = 512;
			}
			TGPTHeader * pGPTHeader4KB = (TGPTHeader *)(apDiskHeaderBuffer.m_pData + __4KB);
			if (0 == memcmp(EFI_PART, pGPTHeader4KB->Signature, _countof(EFI_PART) - 1))
			{
				di.m_sectorSize = 4096;
			}

			TGPT_PTE * pPTE = (TGPT_PTE *)(apDiskHeaderBuffer.m_pData + di.m_sectorSize + di.m_sectorSize);
			di.m_isDynamicDisk = IsDynamicGPTPartitionTable(pPTE);

			if (di.m_isDynamicDisk)
			{
				static const GUID LDM_METADATA_PARTITION_ID =
				{ 0x5808C8AA, 0x7E8F, 0x42E0,{ 0x85, 0xD2, 0xE1, 0xE9, 0x04, 0x34, 0xCF, 0xB3 } };
				CONST INT MAX_PART_COUNT = 128;
				for (INT ii = 0; ii < MAX_PART_COUNT; ++ii)
				{
					if (IsEqualGUID(LDM_METADATA_PARTITION_ID, pPTE[ii].partitionTypeGUID))
					{
						di.m_GPTLDMStartLBA = pPTE[ii].startingLBA;
						break;
					}
				}
			}
		}
		else
		{
			di.m_isDynamicDisk = IsDynamicMBRDisk(apDiskHeaderBuffer.m_pData);
			if (di.m_isDynamicDisk)
			{
				const PrivHead* privhead512B = reinterpret_cast<const PrivHead*>(apDiskHeaderBuffer.m_pData + g_privHeadOffsetSector * __512B);
				if (0 == memcmp(&privhead512B->magic[0], g_privHeadMagic, sizeof(privhead512B->magic)))
				{
					di.m_sectorSize = __512B;
				}

				const PrivHead* privhead4KB = reinterpret_cast<const PrivHead*>(apDiskHeaderBuffer.m_pData + g_privHeadOffsetSector * __4KB);
				if (0 == memcmp(&privhead4KB->magic[0], g_privHeadMagic, sizeof(privhead4KB->magic)))
				{
					di.m_sectorSize = __4KB;
				}
			}
			else
			{
				//how to detect sector size for basic MBR disk without no volume data accurately?
			}
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	HRESULT Adjust4KBSectorBasicDisk(IFileDevice * pDiskRW)
	{
		HRESULT hr = S_OK;
		if (Is4KBSectorBasicMBRDisk(pDiskRW))
		{
			D_INFO(0, TEXT("Adjust 4KB sector basic MBR disk"));
			LONGLONG FirstEBROffset = 0;
			hr = AdjustBasicMBR4KBDisk(pDiskRW, 0, FirstEBROffset);
		}
		else if (Is4KBSectorBasicGPTDisk(pDiskRW))
		{
			D_INFO(0, TEXT("Adjust 4KB sector basic GPT disk"));
			hr = AdjustBasicGPT4KBDisk(pDiskRW);
		}
		else
		{
			D_INFO(0, TEXT("it is neither a 4KB-sector basic GPT disk nor a 4KB-sector basic MBR disk, skip adjusting"));
		}
		return hr;
	}

}
