#include "stdafx.h"
#include "GPTDiskLayout.h"
#include "MiscHelper.h"


CGptMbrLayout::CGptMbrLayout()
{
	m_apBuffer.Allocate(GPT_LBA_BLOCK_SIZE);
	m_pGptMbr = (BYTE*)(m_apBuffer);
	ZeroMemory(m_pGptMbr, GPT_LBA_BLOCK_SIZE);
}

VOID CGptMbrLayout::InitializeFromSample()
{
	CopyMemory(m_pGptMbr, GPT_MBR_SAMPLE, GPT_LBA_BLOCK_SIZE);
}

BYTE CGptMbrLayout::GetBootIndicator()
{
	CONST INT INDEX =0;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	return m_pGptMbr[llOffset];
}

VOID CGptMbrLayout::SetBootIndicator( BYTE BootIndicator )
{
	CONST INT INDEX =0;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	m_pGptMbr[llOffset] = BootIndicator;
}

BYTE CGptMbrLayout::GetStartingHead()
{
	CONST INT INDEX =1;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	return m_pGptMbr[llOffset];
}

VOID CGptMbrLayout::SetStartingHead( BYTE StartingHead )
{
	CONST INT INDEX =1;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	m_pGptMbr[llOffset] = StartingHead;
}

BYTE CGptMbrLayout::GetStartingSector()
{
	CONST INT INDEX =2;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	return m_pGptMbr[llOffset];
}

VOID CGptMbrLayout::SetStartingSector( BYTE StartingSector )
{
	CONST INT INDEX =2;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	m_pGptMbr[llOffset] = StartingSector;
}

BYTE CGptMbrLayout::GetStartingCylinde()
{
	CONST INT INDEX = 3;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	return m_pGptMbr[llOffset];
}

VOID CGptMbrLayout::SetStartingCylinde( BYTE StartingCylinde )
{
	CONST INT INDEX = 3;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	m_pGptMbr[llOffset] = StartingCylinde;
}

BYTE CGptMbrLayout::GetSystemID()
{
	CONST INT INDEX = 4;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	return m_pGptMbr[llOffset];
}

VOID CGptMbrLayout::SetSystemID( BYTE SystemID )
{
	CONST INT INDEX = 4;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	m_pGptMbr[llOffset] = SystemID;
}

BYTE CGptMbrLayout::GetEndingHead()
{
	CONST INT INDEX = 5;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	return m_pGptMbr[llOffset];
}

VOID CGptMbrLayout::SetEndingHead( BYTE EndingHead )
{
	CONST INT INDEX = 5;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	m_pGptMbr[llOffset] = EndingHead;
}

BYTE CGptMbrLayout::GetEndingSector()
{
	CONST INT INDEX = 6;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	return m_pGptMbr[llOffset];
}

VOID CGptMbrLayout::SetEndingSector( BYTE EndingSector )
{
	CONST INT INDEX = 6;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	m_pGptMbr[llOffset] = EndingSector;
}

BYTE CGptMbrLayout::GetEndingCylinder()
{
	CONST INT INDEX = 7;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	return m_pGptMbr[llOffset];
}

VOID CGptMbrLayout::SetEndingCylinder( BYTE EndingCylinder )
{
	CONST INT INDEX = 7;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;
	m_pGptMbr[llOffset] = EndingCylinder;
}

ULONG CGptMbrLayout::GetStartingLBA()
{
	CONST INT INDEX = 8;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;

	ULONG StartingLBA = 0;
	CopyMemory(&StartingLBA, m_pGptMbr+llOffset, sizeof(ULONG));
	return StartingLBA;
}

VOID CGptMbrLayout::SetStartingLBA( ULONG StartingLBA )
{
	CONST INT INDEX = 8;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptMbr+llOffset, &StartingLBA, sizeof(ULONG));
}

ULONG CGptMbrLayout::GetSizeInLBA()
{
	CONST INT INDEX = 9;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;

	ULONG SizeInLBA = 0;
	CopyMemory(&SizeInLBA, m_pGptMbr+llOffset, sizeof(ULONG));
	return SizeInLBA;
}

VOID CGptMbrLayout::SetSizeInLBA( ULONG SizeInLBA )
{
	CONST INT INDEX = 9;
	LONGLONG llOffset = GPT_MBR_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_MBR_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptMbr+llOffset, &SizeInLBA, sizeof(ULONG));
}

CGptHeaderLayout::CGptHeaderLayout()
{
	m_apBuffer.Allocate(GPT_LBA_BLOCK_SIZE);
	m_pGptHeader = (BYTE*)(m_apBuffer);
	ZeroMemory(m_pGptHeader, GPT_LBA_BLOCK_SIZE);
}

VOID CGptHeaderLayout::InitializeFromSample()
{
	CopyMemory(m_pGptHeader, GPT_HEADER_SAMPLE, GPT_LBA_BLOCK_SIZE);
}

VOID CGptHeaderLayout::SetSignature( CONST BYTE* Signature, INT nLen /*= 8*/ )
{
	CONST INT INDEX = 0;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, Signature, 8);
}

VOID CGptHeaderLayout::GetSignature( BYTE* Signature, INT nLen /*= 8*/ )
{
	CONST INT INDEX = 0;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(Signature, m_pGptHeader+llOffset, 8);
}

VOID CGptHeaderLayout::SetRevision( ULONG Revision )
{
	CONST INT INDEX = 1;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &Revision, 4);
}

VOID CGptHeaderLayout::GetRevision( ULONG & Revision )
{
	CONST INT INDEX = 1;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&Revision, m_pGptHeader+llOffset, 4);
}

VOID CGptHeaderLayout::SetHeaderSize( ULONG HeaderSize )
{
	CONST INT INDEX = 2;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &HeaderSize, 4);
}

VOID CGptHeaderLayout::GetHeaderSize( ULONG & HeaderSize )
{
	CONST INT INDEX = 2;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&HeaderSize, m_pGptHeader+llOffset, 4);
}

VOID CGptHeaderLayout::UpdateGptHeaderCrc()
{
	CONST INT INDEX = 3;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	ULONG crcInitValue = (ULONG)(0);
	CopyMemory((BYTE*)m_pGptHeader+llOffset, &crcInitValue, sizeof(ULONG));

	CCrcCalc crc;
	crc.InitCrc32Table();
	ULONG ulCrc = crc.GetCrc((BYTE*)m_pGptHeader, 0x5c);
	CopyMemory((BYTE*)m_pGptHeader+llOffset, &ulCrc, sizeof(ULONG));
}

VOID CGptHeaderLayout::GetGptHeaderCrc( ULONG & GptHeaderCrc )
{
	CONST INT INDEX = 3;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&GptHeaderCrc, m_pGptHeader+llOffset, 4);
}

VOID CGptHeaderLayout::SetReserved( ULONG Reserved )
{
	CONST INT INDEX = 4;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &Reserved, 4);
}

VOID CGptHeaderLayout::GetReserved( ULONG & Reserved )
{
	CONST INT INDEX = 4;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&Reserved, m_pGptHeader+llOffset, 4);
}

VOID CGptHeaderLayout::SetPrimaryLBA( LONGLONG PrimaryLBA )
{
	CONST INT INDEX = 5;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &PrimaryLBA, 8);
}

VOID CGptHeaderLayout::GetPrimaryLBA( LONGLONG & PrimaryLBA )
{
	CONST INT INDEX = 5;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&PrimaryLBA, m_pGptHeader+llOffset, 8);
}

VOID CGptHeaderLayout::SetBackupLBA( LONGLONG BackupLBA )
{
	CONST INT INDEX = 6;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &BackupLBA, 8);
}

VOID CGptHeaderLayout::GetBackupLBA( LONGLONG & BackupLBA )
{
	CONST INT INDEX = 6;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&BackupLBA, m_pGptHeader+llOffset, 8);
}

VOID CGptHeaderLayout::SetFirstUsableLBA( LONGLONG FirstUsableLBA )
{
	CONST INT INDEX = 7;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &FirstUsableLBA, 8);
}

VOID CGptHeaderLayout::GetFirstUsableLBA( LONGLONG & FirstUsableLBA )
{
	CONST INT INDEX = 7;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&FirstUsableLBA, m_pGptHeader+llOffset, 8);
}

VOID CGptHeaderLayout::SetLastUsableLBA( LONGLONG LastUsableLBA )
{
	CONST INT INDEX = 8;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &LastUsableLBA, 8);
}

VOID CGptHeaderLayout::GetLastUsableLBA( LONGLONG & LastUsableLBA )
{
	CONST INT INDEX = 8;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&LastUsableLBA, m_pGptHeader+llOffset, 8);
}

VOID CGptHeaderLayout::SetDiskGUID( CONST GUID & DiskGUID )
{
	CONST INT INDEX = 9;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &DiskGUID, 16);
}

VOID CGptHeaderLayout::GetDiskGUID( GUID & DiskGUID )
{
	CONST INT INDEX = 9;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&DiskGUID, m_pGptHeader+llOffset, 16);
}

VOID CGptHeaderLayout::SetPartitionEntryLBA( LONGLONG PartitionEntryLBA )
{
	CONST INT INDEX = 10;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &PartitionEntryLBA, 8);
}

VOID CGptHeaderLayout::GetPartitionEntryLBA( LONGLONG & PartitionEntryLBA )
{
	CONST INT INDEX = 10;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&PartitionEntryLBA, m_pGptHeader+llOffset, 8);
}

VOID CGptHeaderLayout::SetNumberOfPartitionEntries( ULONG NumberOfPartitionEntries )
{
	CONST INT INDEX = 11;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &NumberOfPartitionEntries, 4);
}

VOID CGptHeaderLayout::GetNumberOfPartitionEntries( ULONG & NumberOfPartitionEntries )
{
	CONST INT INDEX = 11;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&NumberOfPartitionEntries, m_pGptHeader+llOffset, 4);
}

VOID CGptHeaderLayout::SetSizeOfPartitionEntry( ULONG SizeOfPartitionEntry )
{
	CONST INT INDEX = 12;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &SizeOfPartitionEntry, 4);
}

VOID CGptHeaderLayout::GetSizeOfPartitionEntry( ULONG & SizeOfPartitionEntry )
{
	CONST INT INDEX = 12;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&SizeOfPartitionEntry, m_pGptHeader+llOffset, 4);
}

VOID CGptHeaderLayout::SetPartitionEntryArrayCrc32( ULONG PartitionEntryArrayCrc32 )
{
	CONST INT INDEX = 13;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(m_pGptHeader+llOffset, &PartitionEntryArrayCrc32, 4);
}

VOID CGptHeaderLayout::GetPartitionEntryArrayCrc32( ULONG & PartitionEntryArrayCrc32 )
{
	CONST INT INDEX = 13;
	LONGLONG llOffset = GPT_HEADER_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_HEADER_LAYOUT[INDEX].m_llLength;

	CopyMemory(&PartitionEntryArrayCrc32, m_pGptHeader+llOffset, 4);
}

CGptPartitionTableEntriesLayout::CGptPartitionTableEntriesLayout()
{
	m_apBuffer.Allocate(GPT_LBA_BLOCK_SIZE*GPT_LBA_BLOCK_COUNT_PARTITION_ENTRIES);
	m_pGptPartitionTableEntries= (BYTE*)(m_apBuffer);
	ZeroMemory(m_pGptPartitionTableEntries, GPT_LBA_BLOCK_SIZE*GPT_LBA_BLOCK_COUNT_PARTITION_ENTRIES);
}

VOID CGptPartitionTableEntriesLayout::InitializeFromSample()
{
	CopyMemory(m_pGptPartitionTableEntries, GPT_PARTITION_TABLE_ENTRY_SAMPLE, sizeof(GPT_PARTITION_TABLE_ENTRY_SAMPLE));
}

VOID CGptPartitionTableEntriesLayout::SetPartitionTypeGUID( INT nPartIndex, CONST GUID & PartitionTypeGUID )
{
	CONST INT INDEX = 0;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;
	
	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(pPartition+llOffset, &PartitionTypeGUID, 16);
}

VOID CGptPartitionTableEntriesLayout::GetPartitionTypeGUID( INT nPartIndex, GUID & PartitionTypeGUID )
{
	CONST INT INDEX = 0;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(&PartitionTypeGUID, pPartition+llOffset, 16);
}

VOID CGptPartitionTableEntriesLayout::SetUniquePartitionGUID( INT nPartIndex, CONST GUID & UniquePartitionGUID )
{
	CONST INT INDEX = 1;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(pPartition+llOffset, &UniquePartitionGUID, 16);
}

VOID CGptPartitionTableEntriesLayout::GetUniquePartitionGUID( INT nPartIndex, GUID & UniquePartitionGUID )
{
	CONST INT INDEX = 1;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(&UniquePartitionGUID, pPartition+llOffset, 16);
}

VOID CGptPartitionTableEntriesLayout::SetStartingLBA( INT nPartIndex, CONST LONGLONG & StartingLBA )
{
	CONST INT INDEX = 2;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(pPartition+llOffset, &StartingLBA, 8);
}

VOID CGptPartitionTableEntriesLayout::GetStartingLBA( INT nPartIndex, LONGLONG & StartingLBA )
{
	CONST INT INDEX = 2;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(&StartingLBA, pPartition+llOffset, 8);
}

VOID CGptPartitionTableEntriesLayout::SetEndingLBA( INT nPartIndex, CONST LONGLONG & EndingLBA )
{
	CONST INT INDEX = 3;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(pPartition+llOffset, &EndingLBA, 8);
}

VOID CGptPartitionTableEntriesLayout::GetEndingLBA( INT nPartIndex, LONGLONG & EndingLBA )
{
	CONST INT INDEX = 3;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(&EndingLBA, pPartition+llOffset, 8);
}

VOID CGptPartitionTableEntriesLayout::SetAttributeBits( INT nPartIndex, CONST LONGLONG & AttributeBits )
{
	CONST INT INDEX = 4;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(pPartition+llOffset, &AttributeBits, 8);
}

VOID CGptPartitionTableEntriesLayout::GetAttributeBits( INT nPartIndex, LONGLONG & AttributeBits )
{
	CONST INT INDEX = 4;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(&AttributeBits, pPartition+llOffset, 8);
}

VOID CGptPartitionTableEntriesLayout::SetPartitionName( INT nPartIndex, LPCTSTR PartitionName, INT nLength /*= 36*/ )
{
	CONST INT INDEX = 5;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(pPartition+llOffset, PartitionName, 72);
}

VOID CGptPartitionTableEntriesLayout::GetPartitionName( INT nPartIndex, LPTSTR PartitionName, INT nLength /*= 36*/ )
{
	CONST INT INDEX = 5;
	LONGLONG llOffset = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llOffset;
	LONGLONG llLength = GPT_PARTITION_TABLE_ENTRY_LAYOUT[INDEX].m_llLength;

	BYTE * pPartition = m_pGptPartitionTableEntries + nPartIndex*GPT_PARTITION_TABLE_ENTRY_SIZE;
	CopyMemory(PartitionName, pPartition+llOffset, 72);
}

ULONG CGptPartitionTableEntriesLayout::CalcCrc()
{
	CCrcCalc crc;
	crc.InitCrc32Table();
	ULONG ulCrc = crc.GetCrc(m_pGptPartitionTableEntries, GPT_LBA_BLOCK_COUNT_PARTITION_ENTRIES*GPT_LBA_BLOCK_SIZE);
	return ulCrc;
}


CGptDiskPartition::CGptDiskPartition( LONGLONG llDiskSize, IFileDeviceObject * pIFileDevice ) :
m_llDiskSize(llDiskSize),
m_pIFileDevice(pIFileDevice),
m_llDiskStartLba(0),
m_llDiskEndLba(0),
m_llPartitionStartLba(0),
m_llPartitionEndLba(0)
{
	ZeroMemory(&m_UniquePartitionGUID, sizeof(GUID));
	ZeroMemory(&m_DiskGUID, sizeof(GUID));
}

INT CGptDiskPartition::Partition(BOOL bWriteSecondary)
{
	INT nStatus = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		LONGLONG llTotalLbaCount = m_llDiskSize;
		llTotalLbaCount /= GPT_LBA_BLOCK_SIZE;

		//******************************************
		INT nPartIndex = 0;
		CGptPartitionTableEntriesLayout GptPartitionTableEntriesLayout;

		GUID PartitionTypeGUID = {0};
		TCHAR szPartitionTypeGUID[] = TEXT("{ebd0a0a2-b9e5-4433-87c0-68b6b72699c7}");
		CMiscHelper::GuidFromString(PartitionTypeGUID, szPartitionTypeGUID);
		GptPartitionTableEntriesLayout.SetPartitionTypeGUID(nPartIndex, PartitionTypeGUID);

		//GUID UniquePartitionGUID = {0};
		CString strUniquePartitionGUID;
		CMiscHelper::CreateGuid(m_UniquePartitionGUID, strUniquePartitionGUID);
		GptPartitionTableEntriesLayout.SetUniquePartitionGUID(nPartIndex, m_UniquePartitionGUID);

		LONGLONG StartingLBA = START_RESERVED_SPACE;
		StartingLBA /= GPT_LBA_BLOCK_SIZE;
		GptPartitionTableEntriesLayout.SetStartingLBA(nPartIndex, StartingLBA);
		m_llPartitionStartLba = StartingLBA;

		LONGLONG EndingLBA = llTotalLbaCount;
		EndingLBA -= END_RESERVED_SPACE/GPT_LBA_BLOCK_SIZE;
		//EndingLBA -= 1;
		GptPartitionTableEntriesLayout.SetEndingLBA(nPartIndex, EndingLBA);
		m_llPartitionEndLba = EndingLBA;

		LONGLONG AttributeBits = 0;
		GptPartitionTableEntriesLayout.SetAttributeBits(nPartIndex, AttributeBits);

		TCHAR szPartName[36] = TEXT("Basic data partition by SMART");
		GptPartitionTableEntriesLayout.SetPartitionName(nPartIndex, szPartName, _countof(szPartName));

		ULONG ulCrcPartTableEntries = GptPartitionTableEntriesLayout.CalcCrc();


		//******************************************
		CGptHeaderLayout GptHeaderLayout;
		GptHeaderLayout.InitializeFromSample();

		LONGLONG BackupLBA = llTotalLbaCount - 1;
		GptHeaderLayout.SetBackupLBA(BackupLBA);

		LONGLONG FirstUsableLBA = START_USABLE_RESERVED_SPACE;
		FirstUsableLBA = START_USABLE_RESERVED_SPACE/GPT_LBA_BLOCK_SIZE;
		GptHeaderLayout.SetFirstUsableLBA(FirstUsableLBA);
		m_llDiskStartLba = FirstUsableLBA;

		LONGLONG LastUsableLBA = llTotalLbaCount;
		LastUsableLBA -= END_USABLE_RESERVED_SPACE/GPT_LBA_BLOCK_SIZE;
		//LastUsableLBA -= (32 + 1 + 1);
		GptHeaderLayout.SetLastUsableLBA(LastUsableLBA);
		m_llDiskEndLba = LastUsableLBA;

		//GUID DiskGUID = {0};
		CString strDiskGUID;
		CMiscHelper::CreateGuid(m_DiskGUID, strDiskGUID);
		GptHeaderLayout.SetDiskGUID(m_DiskGUID);

		GptHeaderLayout.SetPartitionEntryArrayCrc32(ulCrcPartTableEntries);

		GptHeaderLayout.UpdateGptHeaderCrc();

		//******************************************
		CGptMbrLayout GptMbrLayout;
		GptMbrLayout.InitializeFromSample();

		//******************************************
		LONGLONG llNewFilePointer = 0;
		hr = m_pIFileDevice->SetPointerEx(
			0,//LONGLONG llDistanceToMove, 
			&llNewFilePointer,//LONGLONG* lpNewFilePointer, 
			FILE_BEGIN//ULONG ulMoveMethod
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr));
			break;
		}

		ULONG ulNumberOfBytesWritten = 0;
		hr = m_pIFileDevice->Write(
			GPT_LBA_BLOCK_SIZE,//ULONG nNumberOfBytesToWrite, 
			&ulNumberOfBytesWritten,//ULONG* lpNumberOfBytesWritten, 
			GptMbrLayout.m_pGptMbr//BYTE* pBuffer
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Write"), hr ));
			break;
		}

		//******************************************
		llNewFilePointer  = 0;
		hr = m_pIFileDevice->SetPointerEx(
			GPT_LBA_BLOCK_SIZE,//LONGLONG llDistanceToMove, 
			&llNewFilePointer,//LONGLONG* lpNewFilePointer, 
			FILE_BEGIN//ULONG ulMoveMethod
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr));
			break;
		}

		ulNumberOfBytesWritten = 0;
		hr = m_pIFileDevice->Write(
			GPT_LBA_BLOCK_SIZE,//ULONG nNumberOfBytesToWrite, 
			&ulNumberOfBytesWritten,//ULONG* lpNumberOfBytesWritten, 
			GptHeaderLayout.m_pGptHeader//BYTE* pBuffer
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Write"), hr));
			break;
		}


		//******************************************
		llNewFilePointer  = 0;
		hr = m_pIFileDevice->SetPointerEx(
			GPT_LBA_BLOCK_SIZE+GPT_LBA_BLOCK_SIZE,//LONGLONG llDistanceToMove, 
			&llNewFilePointer,//LONGLONG* lpNewFilePointer, 
			FILE_BEGIN//ULONG ulMoveMethod
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr));
			break;
		}

		ulNumberOfBytesWritten = 0;
		hr = m_pIFileDevice->Write(
			GPT_LBA_BLOCK_COUNT_PARTITION_ENTRIES*GPT_LBA_BLOCK_SIZE,//ULONG nNumberOfBytesToWrite, 
			&ulNumberOfBytesWritten,//ULONG* lpNumberOfBytesWritten, 
			GptPartitionTableEntriesLayout.m_pGptPartitionTableEntries//BYTE* pBuffer
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Write"), hr));
			break;
		}

		//******************************************
		if (bWriteSecondary)
		{
			LONGLONG llSecondaryOffset = llTotalLbaCount;
			llSecondaryOffset -= (GPT_LBA_BLOCK_COUNT_PARTITION_ENTRIES + GPT_LBA_BLOCK_COUNT_HEADER);
			llSecondaryOffset *= GPT_LBA_BLOCK_SIZE;

			llNewFilePointer = 0;
			hr = m_pIFileDevice->SetPointerEx(
				llSecondaryOffset,//LONGLONG llDistanceToMove, 
				&llNewFilePointer,//LONGLONG* lpNewFilePointer, 
				FILE_BEGIN//ULONG ulMoveMethod
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr));
				break;
			}

			ulNumberOfBytesWritten = 0;
			hr = m_pIFileDevice->Write(
				GPT_LBA_BLOCK_COUNT_PARTITION_ENTRIES*GPT_LBA_BLOCK_SIZE,//ULONG nNumberOfBytesToWrite, 
				&ulNumberOfBytesWritten,//ULONG* lpNumberOfBytesWritten, 
				GptPartitionTableEntriesLayout.m_pGptPartitionTableEntries//BYTE* pBuffer
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Write"), hr));
				break;
			}

			//******************************************
			llSecondaryOffset = llTotalLbaCount;
			llSecondaryOffset -= (GPT_LBA_BLOCK_COUNT_HEADER);
			llSecondaryOffset *= GPT_LBA_BLOCK_SIZE;

			llNewFilePointer = 0;
			hr = m_pIFileDevice->SetPointerEx(
				llSecondaryOffset,//LONGLONG llDistanceToMove, 
				&llNewFilePointer,//LONGLONG* lpNewFilePointer, 
				FILE_BEGIN//ULONG ulMoveMethod
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr));
				break;
			}

			ulNumberOfBytesWritten = 0;
			hr = m_pIFileDevice->Write(
				GPT_LBA_BLOCK_SIZE,//ULONG nNumberOfBytesToWrite, 
				&ulNumberOfBytesWritten,//ULONG* lpNumberOfBytesWritten, 
				GptHeaderLayout.m_pGptHeader//BYTE* pBuffer
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Write"), hr));
				break;
			}
		}


	} while (FALSE);

	return nStatus;
}

INT CGptDiskPartition::GetPartitionInfo( GUID & UniquePartitionGUID, GUID & DiskGUID, LONGLONG & llDiskStartLba, LONGLONG & llDiskEndLba, LONGLONG & llPartitionStartLba, LONGLONG & llPartitionEndLba )
{
	CopyMemory(&UniquePartitionGUID, &m_UniquePartitionGUID, sizeof(GUID)) ;
	CopyMemory(&DiskGUID, &m_DiskGUID, sizeof(GUID));
	llDiskStartLba = m_llDiskStartLba;
	llDiskEndLba = m_llDiskEndLba;
	llPartitionStartLba = m_llPartitionStartLba;
	llPartitionEndLba = m_llPartitionEndLba;

	return 0;
}

INT CGptDiskOffsetLengthName::Get( CONST BYTE * pBufferStart, BYTE* pUserBuffer, LONGLONG llUserBufferLength, CONST CGptDiskOffsetLengthName & OffsetLengthName )
{
	INT nStatus = 0;
	if (llUserBufferLength == OffsetLengthName.m_llLength)
	{
		CopyMemory(pUserBuffer, pBufferStart + OffsetLengthName.m_llOffset, OffsetLengthName.m_llLength);
	}
	else
	{
		nStatus = E_INVALIDARG;
	}
	return nStatus;
}

INT CGptDiskOffsetLengthName::Set( BYTE * pBufferStart, CONST BYTE* pUserBuffer, LONGLONG llUserBufferLength, CONST CGptDiskOffsetLengthName & OffsetLengthName )
{
	INT nStatus = 0;
	if (llUserBufferLength == OffsetLengthName.m_llLength)
	{
		CopyMemory(pBufferStart + OffsetLengthName.m_llOffset, pUserBuffer, OffsetLengthName.m_llLength);
	}
	else
	{
		nStatus = E_INVALIDARG;
	}
	return nStatus;
}