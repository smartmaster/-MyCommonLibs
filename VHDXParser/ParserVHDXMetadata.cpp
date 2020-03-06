#include "stdafx.h"
#include "..\PublicHeader\PublicVMemPtr.h"
#include "ParserVHDXMetadata.h"

#include "VhdxFileIdentifier.h"
#include "VhdxHeader.h"
#include "VHDXRegionTableHeader.h"
#include "VHDXRegionTableEntry.h"
#include "VHDXMetadataTableHeader.h"
#include "VHDXMetadataTableEntry.h"


ParserVHDXMetadata::ParserVHDXMetadata( IFileDevice * pIFileDevice ) :
	m_pIFileDevice(pIFileDevice)
{
	m_pVhdxHeader = NULL;
	ZeroMemory(m_VhdxHeaderBuffer, sizeof(m_VhdxHeaderBuffer));
	ZeroValue(m_VHDXRegionTableEntryBAT);
	ZeroValue(m_VHDX_FILE_PARAMETERS);
	ZeroValue(m_VHDX_VIRTUAL_DISK_SIZE);
	ZeroValue(m_VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE);
	ZeroValue(m_VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE);

	m_ChunkRatio = 0;
	m_DataBlocksCount = 0;
	m_BitmapBlocksCount = 0;
	m_BatEntriesCount = 0;
}

HRESULT ParserVHDXMetadata::Parse()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONGLONG NewFilePointer = 0;
	hr = m_pIFileDevice->SetPointerEx(0, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//CONST LONG HEADER_SIZE = 256 * 1024;
	CVMemPtr<BYTE> apBufferHeaders;
	apBufferHeaders.Allocate(VhdxFileIdentifier::HeadersSizeSpecConst());
	if (NULL == apBufferHeaders.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	ULONG NumberOfBytesRead = 0;
	hr = m_pIFileDevice->Read(
		VhdxFileIdentifier::HeadersSizeSpecConst(),//ULONG nNumberOfBytesToRead, 
		&NumberOfBytesRead,//ULONG* lpNumberOfBytesRead, 
		apBufferHeaders.m_pData//BYTE* pBuffer
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	VhdxFileIdentifier * pVhdxFileIdentifier = (VhdxFileIdentifier*)(apBufferHeaders.m_pData + VhdxFileIdentifier::OffsetSpecConst());
	pVhdxFileIdentifier->Print();
	BOOL bRet = pVhdxFileIdentifier->IsValid();
	if (!bRet)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("VhdxFileIdentifier"), hr, TEXT("Invalid VhdxFileIdentifier"));
		LEAVE_BLOCK(0);
	}
	//CString strCreator;
	//pVhdxFileIdentifier->ReadCreator(strCreator);

	//////////////////////////////////////////////////////////////////////////
	VhdxHeader * pVhdxHeader1 = (VhdxHeader*)(apBufferHeaders.m_pData + VhdxHeader::Headser1OffsetSpecConst());
	pVhdxHeader1->Print();
	BOOL bRetVhdxHeader1 = pVhdxHeader1->IsValid();

	VhdxHeader * pVhdxHeader2 = (VhdxHeader*)(apBufferHeaders.m_pData + VhdxHeader::Headser2OffsetSpecConst());
	pVhdxHeader2->Print();
	BOOL bRetVhdxHeader2 = pVhdxHeader2->IsValid();

	bRet = (bRetVhdxHeader1 || bRetVhdxHeader2);
	if (!bRet)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("VhdxHeader"), hr, TEXT("Invalid VhdxHeader"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	VhdxHeader * pVhdxHeaderActive = NULL;
	if (bRetVhdxHeader1 && bRetVhdxHeader2)
	{
		if (pVhdxHeader1->m_SequenceNumber >= pVhdxHeader2->m_SequenceNumber)
		{
			pVhdxHeaderActive = pVhdxHeader1;
		}
		else
		{
			pVhdxHeaderActive = pVhdxHeader2;
		}
	}
	else if (bRetVhdxHeader1)
	{
		pVhdxHeaderActive = pVhdxHeader1;
	}
	else //if (bRetVhdxHeader2)
	{
		pVhdxHeaderActive = pVhdxHeader2;
	}
	CopyMemory(m_VhdxHeaderBuffer, pVhdxHeaderActive, sizeof(m_VhdxHeaderBuffer));
	m_pVhdxHeader = (VhdxHeader *)(m_VhdxHeaderBuffer);


	//////////////////////////////////////////////////////////////////////////
	VHDXRegionTableHeader * pVHDXRegionTableHeader = (VHDXRegionTableHeader*)(apBufferHeaders.m_pData + VHDXRegionTableHeader::OffsetSpecConst());
	pVHDXRegionTableHeader->Print();
	bRet = pVHDXRegionTableHeader->IsValid();
	if (!bRet)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("VHDXRegionTableHeader"), hr, TEXT("Invalid VHDXRegionTableHeader"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	VHDXRegionTableEntry * pRegionEntry = (VHDXRegionTableEntry*)((BYTE*)(pVHDXRegionTableHeader) + sizeof(VHDXRegionTableHeader));
	for (UINT32 ii = 0; ii < pVHDXRegionTableHeader->m_EntryCount; ++ ii)
	{
		pRegionEntry[ii].Print();

		if (pRegionEntry[ii].IsBatEntry())
		{
			CopyValue(m_VHDXRegionTableEntryBAT, pRegionEntry[ii]);
			D_INFO(0, TEXT("BatEntry. offset=%I64d, m_Required=%s"), pRegionEntry[ii].m_FileOffset, (pRegionEntry[ii].m_Required? TEXT("TRUE") : TEXT("FALSE")));
		}
		else if (pRegionEntry[ii].IsMetadataRegionEntry())
		{
			D_INFO(0, TEXT("MetadataRegionEntry. offset=%I64d, m_Required=%s"), pRegionEntry[ii].m_FileOffset, (pRegionEntry[ii].m_Required? TEXT("TRUE") : TEXT("FALSE")));
			ParseMetadata(pRegionEntry[ii].m_FileOffset); 
		}
		else
		{
			D_INFO(0, TEXT("UnknownRegionEntry. offset=%I64d, m_Required=%s"), pRegionEntry[ii].m_FileOffset, (pRegionEntry[ii].m_Required? TEXT("TRUE") : TEXT("FALSE")));
		}
	}

	END_BLOCK(0);

	return S_OK;
}

int ParserVHDXMetadata::ParseMetadataEntry( LPBYTE pMetadataStart, VHDXMetadata & Metadata, VHDXMetadataTableEntry * pVHDXMetadataTableEntry, ULONG EntryCount, map<CString, CString> & ParentPath )
{
	for (ULONG ii = 0; ii < EntryCount; ++ ii)
	{
		pVHDXMetadataTableEntry[ii].Print();
		pVHDXMetadataTableEntry[ii].ParseMetadata(pMetadataStart, Metadata, ParentPath);
	}

	return 0;
}

HRESULT ParserVHDXMetadata::ParseMetadata(LONGLONG MetadataOffset )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = FALSE;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONGLONG NewFilePointer = 0;
	hr = m_pIFileDevice->SetPointerEx(MetadataOffset, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apMetadataBuffer;
	apMetadataBuffer.Allocate(VHDXMetadataTableHeader::SizeSpecConst());
	if (NULL == apMetadataBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	ULONG NumberOfBytesRead = 0;
	hr = m_pIFileDevice->Read(VHDXMetadataTableHeader::SizeSpecConst(), &NumberOfBytesRead, apMetadataBuffer.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	VHDXMetadataTableHeader * pVHDXMetadataTableHeader = (VHDXMetadataTableHeader *)(apMetadataBuffer.m_pData);
	pVHDXMetadataTableHeader->Print();
	bRet = pVHDXMetadataTableHeader->IsValid();
	if (!bRet)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("VHDXMetadataTableHeader"), hr, TEXT("Invalid VHDXMetadataTableHeader"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	VHDXMetadata Metadata;
	VHDXMetadataTableEntry * pVHDXMetadataTableEntry = (VHDXMetadataTableEntry *)((BYTE*)(pVHDXMetadataTableHeader) + sizeof(VHDXMetadataTableHeader));
	ParseMetadataEntry(apMetadataBuffer.m_pData, Metadata, pVHDXMetadataTableEntry, pVHDXMetadataTableHeader->m_EntryCount, m_VHDX_PARENT_LOCATOR_ENTRIES);
	if (Metadata.m_pVHDX_FILE_PARAMETERS)
	{
		CopyMemory(&m_VHDX_FILE_PARAMETERS, Metadata.m_pVHDX_FILE_PARAMETERS, sizeof(VHDX_FILE_PARAMETERS));
	}

	if (Metadata.m_pVHDX_VIRTUAL_DISK_SIZE)
	{
		CopyMemory(&m_VHDX_VIRTUAL_DISK_SIZE, Metadata.m_pVHDX_VIRTUAL_DISK_SIZE, sizeof(VHDX_VIRTUAL_DISK_SIZE ));
	}

	if (Metadata.m_pVHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE)
	{
		CopyMemory(&m_VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE, Metadata.m_pVHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE, sizeof(VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE ));
	}

	if (Metadata.m_pVHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE)
	{
		CopyMemory(&m_VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE, Metadata.m_pVHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE, sizeof(VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE ));
	}

	//////////////////////////////////////////////////////////////////////////
	hr = ParseBatTable();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ParseBat"), hr, TEXT("Invalid BAT or read failure"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserVHDXMetadata::ParseBatTable()
{
	//////////////////////////////////////////////////////////////////////////
	CONST LONGLONG SECTOR_COUNT_PER_BITMAP_BLOCK = 8 * 1024 * 1024; //LONGLONG to avoid overflow
	m_ChunkRatio = (SECTOR_COUNT_PER_BITMAP_BLOCK * m_VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE.m_LogicalSectorSize) / m_VHDX_FILE_PARAMETERS.m_BlockSize;

	m_DataBlocksCount = AtlAlignUp(m_VHDX_VIRTUAL_DISK_SIZE.m_VirtualDiskSize, m_VHDX_FILE_PARAMETERS.m_BlockSize) / m_VHDX_FILE_PARAMETERS.m_BlockSize;

	m_BitmapBlocksCount = (m_DataBlocksCount + m_ChunkRatio - 1) / m_ChunkRatio;

	if (m_VHDX_FILE_PARAMETERS.m_HasParent)
	{
		m_BatEntriesCount = m_BitmapBlocksCount * (m_ChunkRatio + 1);
	}
	else
	{
		m_BatEntriesCount = m_DataBlocksCount + (m_DataBlocksCount - 1) /m_ChunkRatio;
	}

	D_INFO(0, TEXT("ChunkRatio=%d, DataBlocksCount=%d, BitmapBlocksCount=%d, BatEntriesCount=%d"), m_ChunkRatio, m_DataBlocksCount, m_BitmapBlocksCount, m_BatEntriesCount);

	//////////////////////////////////////////////////////////////////////////
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONGLONG NewFilePointer = 0;
	hr = m_pIFileDevice->SetPointerEx(m_VHDXRegionTableEntryBAT.m_FileOffset, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_apBatTable.Allocate(m_VHDXRegionTableEntryBAT.m_Length / sizeof(VHDX_BAT_ENTRY));
	if (NULL == m_apBatTable.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	ULONG NumberOfBytesRead = 0;
	hr = m_pIFileDevice->Read(m_VHDXRegionTableEntryBAT.m_Length, &NumberOfBytesRead, (BYTE*)m_apBatTable.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//debug purpose - remove when release
	for (LONG ii = 0; ii < m_BatEntriesCount; ++ ii)
	{
		m_apBatTable.m_pData[ii].Print();
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

LONG ParserVHDXMetadata::BlockIndexToBatIndex( LONG BlockIndex )
{
	return (BlockIndex / m_ChunkRatio * (m_ChunkRatio + 1)) + (BlockIndex % m_ChunkRatio);
}
