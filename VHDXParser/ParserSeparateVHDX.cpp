#include "stdafx.h"

#include <intsafe.h>

#include "..\PublicHeader\PublicVMemPtr.h"
#include "..\PublicHeader\PublicSTCopy.h"

#include "ParserSeparateVHDX.h"

#include "VhdxFileIdentifier.h"
#include "VhdxHeader.h"
#include "VHDXRegionTableHeader.h"
#include "VHDXRegionTableEntry.h"
#include "VHDXMetadataTableHeader.h"
#include "VHDXMetadataTableEntry.h"

#include "MergeBuffer.h"

#include "VHDXLogReplay.h"
#include "VhdxHelper.h"

#include "DiskImageInterface.h" //<sonmi01>2014-4-21 ###???


ParserSeparateVHDX::ParserSeparateVHDX( IFileDevice * pIFileDevice ) :
	m_pIFileDevice(pIFileDevice),
	m_CacheIoBitmap(NULL, FALSE, 0),
	m_CacheIoBatTable(NULL, FALSE, 0),
	m_pDumpedBatTalbeFile(NULL), //<sonmi01>2014-1-9 ###???
	m_pCacheIoDumpedBatTalbe(NULL),
	m_ReplayedLogFileDevice(NULL), //<sonmi01>2014-2-24 ###???
	m_pLogReplayCallback(NULL)
{
	m_pVhdxHeader = NULL;
	ZeroMemory(m_VhdxHeaderBuffer, sizeof(m_VhdxHeaderBuffer));
	ZeroValue(m_VHDXRegionEntryBAT);
	ZeroValue(m_FileParameters);
	ZeroValue(m_VirtualDiskSize);
	ZeroValue(m_LogicalSectorSize);
	ZeroValue(m_PhysicalSectorSize);

	m_ChunkRatio = 0;
	m_DataBlocksCount = 0;
	m_BitmapBlocksCount = 0;
	m_BatEntriesCount = 0;
	m_AllocPosition = 0;
	m_NeedToUpdateBAT = FALSE;
}

HRESULT ParserSeparateVHDX::Parse()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
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
	hr = ReadAtOffsetHelper(m_pIFileDevice, 0, VhdxFileIdentifier::HeadersSizeSpecConst(), &NumberOfBytesRead, apBufferHeaders.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
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
			CopyValue(m_VHDXRegionEntryBAT, pRegionEntry[ii]);
			D_INFO(0, TEXT("BatEntry. offset=%I64d, m_Required=%s"), pRegionEntry[ii].m_FileOffset, (pRegionEntry[ii].m_Required? TEXT("TRUE") : TEXT("FALSE")));
		}
		else if (pRegionEntry[ii].IsMetadataRegionEntry())
		{
			D_INFO(0, TEXT("MetadataRegionEntry. offset=%I64d, m_Required=%s"), pRegionEntry[ii].m_FileOffset, (pRegionEntry[ii].m_Required? TEXT("TRUE") : TEXT("FALSE")));
			hr = ParseMetadata(pRegionEntry[ii].m_FileOffset, AtlAlignUp(pRegionEntry[ii].m_Length, VHDXMetadataTableHeader::SizeSpecConst()));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ParseMetadata"), hr, TEXT(""));
				break;
			}
		}
		else
		{
			D_INFO(0, TEXT("UnknownRegionEntry. offset=%I64d, m_Required=%s"), pRegionEntry[ii].m_FileOffset, (pRegionEntry[ii].m_Required? TEXT("TRUE") : TEXT("FALSE")));
		}
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	hr = ReplayLog(); //<sonmi01>2014-2-24 ###???
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("ReplayLog"));
		D_API_ERR(0, TEXT("ReplayLog"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = ParseBatTable();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ParseBat"), hr, TEXT("Invalid BAT or read failure"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = m_pIFileDevice->GetFileSize(&m_AllocPosition);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CONST LONG __1M = 1024 * 1024;
	m_AllocPosition = AtlAlignUp(m_AllocPosition, __1M);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

int ParserSeparateVHDX::ParseMetadataEntry( LPBYTE pMetadataStart, VHDXMetadata & Metadata, VHDXMetadataTableEntry * pVHDXMetadataTableEntry, ULONG EntryCount, map<CString, CString> & ParentPath )
{
	for (ULONG ii = 0; ii < EntryCount; ++ ii)
	{
		pVHDXMetadataTableEntry[ii].Print();
		pVHDXMetadataTableEntry[ii].ParseMetadata(pMetadataStart, Metadata, ParentPath);
	}

	return 0;
}

HRESULT ParserSeparateVHDX::ParseMetadata(LONGLONG MetadataOffset, LONG Length)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = FALSE;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apMetadataBuffer;
	apMetadataBuffer.Allocate(Length/*VHDXMetadataTableHeader::SizeSpecConst()*/);
	if (NULL == apMetadataBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	ULONG NumberOfBytesRead = 0;
	hr = ReadAtOffsetHelper(m_pIFileDevice, MetadataOffset, Length/*VHDXMetadataTableHeader::SizeSpecConst()*/, &NumberOfBytesRead, apMetadataBuffer.m_pData);
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
	ParseMetadataEntry(apMetadataBuffer.m_pData, Metadata, pVHDXMetadataTableEntry, pVHDXMetadataTableHeader->m_EntryCount, m_ParentLocatorEntries);
	if (Metadata.m_pVHDX_FILE_PARAMETERS)
	{
		CopyMemory(&m_FileParameters, Metadata.m_pVHDX_FILE_PARAMETERS, sizeof(VHDX_FILE_PARAMETERS));
	}

	if (Metadata.m_pVHDX_VIRTUAL_DISK_SIZE)
	{
		CopyMemory(&m_VirtualDiskSize, Metadata.m_pVHDX_VIRTUAL_DISK_SIZE, sizeof(VHDX_VIRTUAL_DISK_SIZE ));
	}

	if (Metadata.m_pVHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE)
	{
		CopyMemory(&m_LogicalSectorSize, Metadata.m_pVHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE, sizeof(VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE ));
	}

	if (Metadata.m_pVHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE)
	{
		CopyMemory(&m_PhysicalSectorSize, Metadata.m_pVHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE, sizeof(VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE ));
	}

	//<sonmi01>2015-6-3 ###???
	if (Metadata.m_pVHDX_PAGE83_DATA)
	{
		CopyMemory(&m_VHDX_PAGE83_DATA, Metadata.m_pVHDX_PAGE83_DATA, sizeof(VHDX_PAGE83_DATA));
	}
	else
	{
		m_VHDX_PAGE83_DATA.m_Page83Data = GUID_NULL;
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserSeparateVHDX::ParseBatTable()
{
	//////////////////////////////////////////////////////////////////////////
	CONST LONGLONG SECTOR_COUNT_PER_BITMAP_BLOCK = 8 * 1024 * 1024; //LONGLONG to avoid overflow
	m_ChunkRatio = (SECTOR_COUNT_PER_BITMAP_BLOCK * m_LogicalSectorSize.m_LogicalSectorSize) / m_FileParameters.m_BlockSize;

	m_DataBlocksCount = AtlAlignUp(m_VirtualDiskSize.m_VirtualDiskSize, m_FileParameters.m_BlockSize) / m_FileParameters.m_BlockSize;

	m_BitmapBlocksCount = (m_DataBlocksCount + m_ChunkRatio - 1) / m_ChunkRatio;

	if (m_FileParameters.m_HasParent)
	{
		m_BatEntriesCount = m_BitmapBlocksCount * (m_ChunkRatio + 1);
	}
	else
	{
		m_BatEntriesCount = m_DataBlocksCount + (m_DataBlocksCount - 1) /m_ChunkRatio;
	}

	m_BlockBitmapBufferSize = m_FileParameters.m_BlockSize / m_LogicalSectorSize.m_LogicalSectorSize / 8;

	//<sonmi01>2014-1-14 ###???
	D_INFO(0, TEXT("VirtualDiskSize=%I64d, BlockSize=%d,  LogicalSectorSize=%d,  PhysicalSectorSize=%d"), m_VirtualDiskSize.m_VirtualDiskSize, m_FileParameters.m_BlockSize,  m_LogicalSectorSize.m_LogicalSectorSize,  m_PhysicalSectorSize.m_PhysicalSectorSize);
	D_INFO(0, TEXT("ChunkRatio=%d, DataBlocksCount=%d, BitmapBlocksCount=%d, BatEntriesCount=%d, BlockBitmapBufferSize=%d"), m_ChunkRatio, m_DataBlocksCount, m_BitmapBlocksCount, m_BatEntriesCount, m_BlockBitmapBufferSize);

	//////////////////////////////////////////////////////////////////////////
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_ReplayedLogFileDevice)
	{
		//////////////////////////////////////////////////////////////////////////
		if (m_VHDXRegionEntryBAT.m_Length > MAX_BAT_TABLE_LEN)
		{
			CONST LONG __1M = 1024 * 1024;
			m_CacheIoBatTable.InitParams(m_ReplayedLogFileDevice, __1M);
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			m_apBatTable.Allocate(m_VHDXRegionEntryBAT.m_Length / sizeof(VHDX_BAT_ENTRY));
			if (NULL == m_apBatTable.m_pData)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			ULONG NumberOfBytesRead = 0;
			hr = ReadAtOffsetHelper(m_ReplayedLogFileDevice, m_VHDXRegionEntryBAT.m_FileOffset, m_VHDXRegionEntryBAT.m_Length, &NumberOfBytesRead, (BYTE*)m_apBatTable.m_pData);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//debug purpose - remove when release
			//for (LONG ii = 0; ii < m_BatEntriesCount; ++ ii)
			//{
			//	m_apBatTable.m_pData[ii].Print(ii);
			//}
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		if (m_VHDXRegionEntryBAT.m_Length > MAX_BAT_TABLE_LEN) //<sonmi01>2013-9-11 ###???
		{
			CONST LONG __1M = 1024 * 1024;
			m_CacheIoBatTable.InitParams(m_pIFileDevice, __1M);
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			m_apBatTable.Allocate(m_VHDXRegionEntryBAT.m_Length / sizeof(VHDX_BAT_ENTRY));
			if (NULL == m_apBatTable.m_pData)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			ULONG NumberOfBytesRead = 0;
			hr = ReadAtOffsetHelper(m_pIFileDevice, m_VHDXRegionEntryBAT.m_FileOffset, m_VHDXRegionEntryBAT.m_Length, &NumberOfBytesRead, (BYTE*)m_apBatTable.m_pData);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//debug purpose - remove when release
			//for (LONG ii = 0; ii < m_BatEntriesCount; ++ ii)
			//{
			//	m_apBatTable.m_pData[ii].Print(ii);
			//}
		}
	}
		
	

	//////////////////////////////////////////////////////////////////////////
	//if (m_ReplayedLogFileDevice) //<sonmi01>2014-3-1 ###???
	//{
	//	m_CacheIoBitmap.InitParams(m_ReplayedLogFileDevice, VHDX_BAT_ENTRY::SECTOR_BITMAP_BLOCK_SIZE);
	//}
	//else
	{
		m_CacheIoBitmap.InitParams(m_pIFileDevice, VHDX_BAT_ENTRY::SECTOR_BITMAP_BLOCK_SIZE);
	}
	

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

LONG ParserSeparateVHDX::BlockIndexToBatIndex( LONG BlockIndex )
{
	return (BlockIndex / m_ChunkRatio * (m_ChunkRatio + 1)) + (BlockIndex % m_ChunkRatio);
}

HRESULT ParserSeparateVHDX::ReadBlocK(IN BOOL bFillZero, IN LONG BlockIndex, OUT BYTE * pBuffer, IN LONG BufferLen )
{
	ATLASSERT(BlockIndex < m_DataBlocksCount);
	ATLASSERT(BufferLen >= m_FileParameters.m_BlockSize);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG BatIndex = BlockIndexToBatIndex(BlockIndex);
	VHDX_BAT_ENTRY BatEntry = {0}; //<sonmi01>2013-9-11 ###???
	hr = ReadBatEntry(BatIndex, BatEntry);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadBatEntry"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	switch (BatEntry.m_State)
	{
		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_NOT_PRESENT:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNDEFINED:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_ZERO:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNMAPPED:
		{
			if (bFillZero)
			{
				ZeroMemory(pBuffer, m_FileParameters.m_BlockSize);
			}
		}
		break;

		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_FULLY_PRESENT:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_PARTIALLY_PRESENT:
		{
			//////////////////////////////////////////////////////////////////////////
			LONGLONG NewFilePointer = 0;
			LONGLONG BlockOffset = BatEntry.m_FileOffsetMB;
			BlockOffset <<= 20;
		
			ULONG NumberOfBytesRead = 0;
			hr = ReadAtOffsetHelper(m_pIFileDevice, BlockOffset, m_FileParameters.m_BlockSize, &NumberOfBytesRead, pBuffer);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
				break;
			}

			//<sonmi01>2014-3-1 ###???
			//if (m_pLogReplayCallback)
			//{
			//	ReplayPayloadBlock(BlockOffset, m_FileParameters.m_BlockSize, pBuffer, m_pLogReplayCallback);
			//}
		}
		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		{
			hr = E_INVALIDARG;
			D_INFO(0, TEXT("Invalid Data Block BAT entry state %d, BlockIndex=%d, BatIndex=%d"), (INT)BatEntry.m_State, BlockIndex, BatIndex);
		}
		break;
	}

	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

LONG ParserSeparateVHDX::BitmapBlockIndexToBatIndex( LONG BitmapBlockIndex )
{
	return BitmapBlockIndex * (m_ChunkRatio + 1) + m_ChunkRatio;
}

HRESULT ParserSeparateVHDX::ReadBlocKBitmap( LONG BlockIndex, IN OUT LONG Len, BYTE * Buffer )
{
	ATLASSERT(BlockIndex < m_DataBlocksCount);
	ATLASSERT(Len >= m_BlockBitmapBufferSize);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG BatIndex = BlockIndexToBatIndex(BlockIndex);
	VHDX_BAT_ENTRY BatEntry = {0}; //<sonmi01>2013-9-11 ###???
	hr = ReadBatEntry(BatIndex, BatEntry);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadBatEntry"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//for dynamic disk, bit 1 means data block is allocated, 0 means data block is not-allocated or is undefined
	//for differencing disk, bit 1 means data block is on current disk, 0 means data block need to be inspected from its parent or is undefined

	//////////////////////////////////////////////////////////////////////////
	switch (BatEntry.m_State)
	{
		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_NOT_PRESENT:
		{
			ZeroMemory(Buffer, m_BlockBitmapBufferSize);
		}
		break;

	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNDEFINED: //<sonmi01>2014-1-13 ###???
		{
			//ZeroMemory(Buffer, m_BlockBitmapBufferSize); //<sonmi01>2014-1-15 ###???
			//if (m_FileParameters.m_HasParent) //<sonmi01>2014-1-16 ###???
			{
				memset(Buffer, 0xff, m_BlockBitmapBufferSize);
			}
			//else
			//{
			//	ZeroMemory(Buffer, m_BlockBitmapBufferSize);
			//}
		}
		break;

	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNMAPPED:
		{
			//ZeroMemory(Buffer, m_BlockBitmapBufferSize); //<sonmi01>2014-1-15 ###???
			//if (m_FileParameters.m_HasParent) //<sonmi01>2014-1-16 ###???
			{
				memset(Buffer, 0xff, m_BlockBitmapBufferSize);
			}
			//else
			//{
			//	ZeroMemory(Buffer, m_BlockBitmapBufferSize);
			//}
		}
		break;

		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_ZERO:
		{
			//if (m_FileParameters.m_HasParent)
			{
				memset(Buffer, 0xff, m_BlockBitmapBufferSize);
			}
			//else
			//{
			//	ZeroMemory(Buffer, m_BlockBitmapBufferSize);
			//}
		}
		break;

	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_FULLY_PRESENT:
		{
			memset(Buffer, 0xff, m_BlockBitmapBufferSize);
		}
		break;

		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_PARTIALLY_PRESENT:
		{
			//////////////////////////////////////////////////////////////////////////
			LONGLONG offset = GetBlockBitmapFileOffset(BlockIndex);
			hr = m_CacheIoBitmap.ReadAtOffset(offset, m_BlockBitmapBufferSize, Buffer);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("CacheIoBitmap.Read"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			if (m_pLogReplayCallback) //<sonmi01>2014-3-1 ###???
			{
				ReplayPayloadBlock(offset, m_BlockBitmapBufferSize, Buffer, m_pLogReplayCallback);
			}
		}
		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		{
			hr = E_INVALIDARG;
			D_INFO(0, TEXT("Invalid Data Block BAT entry state %d, BlockIndex=%d, BatIndex=%d"), (INT)BatEntry.m_State, BlockIndex, BatIndex);
		}
		break;
	}

	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}




ParserSeparateVHDX::~ParserSeparateVHDX()
{
	UpdateBAT();

	m_CacheIoBatTable.Flush();
	m_CacheIoBitmap.Flush();

	if (m_ReplayedLogFileDevice) //<sonmi01>2014-2-24 ###???
	{
		m_ReplayedLogFileDevice->Release();
		m_ReplayedLogFileDevice = NULL;
		
	}

	//<sonmi01>2014-2-24 ###???
	if (m_ReplayedLogFileName.GetLength() > 3 && CMiscHelper::IsFileExists(m_ReplayedLogFileName.GetString()))
	{
		BOOL bDeleteFile = ::DeleteFile(m_ReplayedLogFileName.GetString());
		if (!bDeleteFile)
		{
			DWORD LastError = GetLastError();
			HRESULT hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("DeleteFile"), hr, TEXT("%s"), m_ReplayedLogFileName.GetString());
		}
		else
		{
			D_INFO(0, TEXT("DeleteFile %s"), m_ReplayedLogFileName.GetString());
		}
	}


	if (m_pIFileDevice)
	{
		m_pIFileDevice->Release();
		m_pIFileDevice = NULL;
	}

	if (m_pLogReplayCallback) //<sonmi01>2014-3-1 ###???
	{
		m_pLogReplayCallback->Release();
		m_pLogReplayCallback = NULL;
	}

	CloseDumpedBatTable(); //<sonmi01>2014-1-9 ###???
}

BOOL ParserSeparateVHDX::NeedToInspectParentBlock( LONG BlockIndex )
{
	ATLASSERT(BlockIndex < m_DataBlocksCount);

	//////////////////////////////////////////////////////////////////////////
	if (!m_FileParameters.m_HasParent)
	{
		return FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	LONG BatIndex = BlockIndexToBatIndex(BlockIndex);
	VHDX_BAT_ENTRY BatEntry = {0}; //<sonmi01>2013-9-11 ###???
	HRESULT hr = ReadBatEntry(BatIndex, BatEntry);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadBatEntry"), hr, TEXT(""));
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	switch (BatEntry.m_State)
	{
		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_NOT_PRESENT:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_PARTIALLY_PRESENT:
		{
			return TRUE;
		}
		//break;

	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNDEFINED:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_ZERO:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNMAPPED:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_FULLY_PRESENT:
		{
			return FALSE;
		}
		//break;

		//////////////////////////////////////////////////////////////////////////
	default:
		{
			return FALSE;
		}
		//break;
	}
}

HRESULT ParserSeparateVHDX::WriteBlocK( IN LONG BlockIndex, IN CONST BYTE * pBuffer, IN LONG BufferLen )
{
	ATLASSERT(BlockIndex < m_DataBlocksCount);
	ATLASSERT(BufferLen >= m_FileParameters.m_BlockSize);

	m_NeedToUpdateBAT = TRUE; 

	//////////////////////////////////////////////////////////////////////////
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG BatIndex = BlockIndexToBatIndex(BlockIndex);
	VHDX_BAT_ENTRY BatEntry = {0}; //<sonmi01>2013-9-11 ###???
	hr = ReadBatEntry(BatIndex, BatEntry);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadBatEntry"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	if (IsAllZero( pBuffer, m_FileParameters.m_BlockSize))
	{
		BatEntry.m_State = VHDX_BAT_ENTRY::PAYLOAD_BLOCK_ZERO;
		BatEntry.m_FileOffsetMB = 0;
		hr = S_OK;
		LEAVE_BLOCK(0);
	}
	

	//////////////////////////////////////////////////////////////////////////
	switch (BatEntry.m_State)
	{
		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_NOT_PRESENT:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNDEFINED:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_ZERO:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNMAPPED:
		{
			ULONG NumberOfBytesWritten = 0;
			hr = WriteAtOffsetHelper(m_pIFileDevice, m_AllocPosition, m_FileParameters.m_BlockSize, &NumberOfBytesWritten, pBuffer);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			BatEntry.m_FileOffsetMB = (m_AllocPosition >> 20);
			BatEntry.m_State = VHDX_BAT_ENTRY::PAYLOAD_BLOCK_FULLY_PRESENT;
			m_AllocPosition += m_FileParameters.m_BlockSize;
		}
		break;

		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_FULLY_PRESENT:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_PARTIALLY_PRESENT:
		{
			//////////////////////////////////////////////////////////////////////////
			LONGLONG NewFilePointer = 0;
			LONGLONG BlockOffset = BatEntry.m_FileOffsetMB;
			BlockOffset <<= 20;

			ULONG NumberOfBytesWritten = 0;
			hr = WriteAtOffsetHelper(m_pIFileDevice, BlockOffset, m_FileParameters.m_BlockSize, &NumberOfBytesWritten, pBuffer);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			BatEntry.m_State = VHDX_BAT_ENTRY::PAYLOAD_BLOCK_FULLY_PRESENT;
		}
		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		{
			hr = E_INVALIDARG;
			D_INFO(0, TEXT("Invalid Data Block BAT entry state %d, BlockIndex=%d, BatIndex=%d"), (INT)BatEntry.m_State, BlockIndex, BatIndex);
		}
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	else
	{
		hr = WriteBatEntry(BatIndex, BatEntry);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteBatEntry"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserSeparateVHDX::UpdateBAT()
{
	if (!m_NeedToUpdateBAT)
	{
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_apBatTable.m_pData) //<sonmi01>2013-9-11 ###???
	{
		ULONG NumberOfBytesWritten = 0;
		hr = WriteAtOffsetHelper(m_pIFileDevice, m_VHDXRegionEntryBAT.m_FileOffset, m_VHDXRegionEntryBAT.m_Length, &NumberOfBytesWritten, (LPCBYTE)m_apBatTable.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		hr = m_CacheIoBatTable.Flush();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("CacheIoBatTable.Flush"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	
	m_NeedToUpdateBAT = FALSE;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

LONGLONG ParserSeparateVHDX::GetBlockBitmapFileOffset( LONG BlockIndex )
{
	LONG BitCountPerBlock = m_FileParameters.m_BlockSize / m_LogicalSectorSize.m_LogicalSectorSize;

	LONGLONG BitOffset = BlockIndex;
	BitOffset *= BitCountPerBlock;

	LONGLONG ByteOffset = BitOffset / 8;
	LONG BitmapBlockIndex = ByteOffset / VHDX_BAT_ENTRY::SECTOR_BITMAP_BLOCK_SIZE;
	LONG BatEntryIndex = BitmapBlockIndexToBatIndex(BitmapBlockIndex);

	LONGLONG offset;
	VHDX_BAT_ENTRY BatEntry = {0}; //<sonmi01>2013-9-11 ###???
	HRESULT hr = ReadBatEntry(BatEntryIndex, BatEntry);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadBatEntry"), hr, TEXT(""));
		LONGLONG __64T = 1;
		__64T <<= 46;
		offset = - __64T; //give an invalid offset
	}
	else
	{
		ATLASSERT(VHDX_BAT_ENTRY::SB_BLOCK_PRESENT == BatEntry.m_State);

		offset = BatEntry.m_FileOffsetMB;
		offset <<= 20;
		offset += (ByteOffset % VHDX_BAT_ENTRY::SECTOR_BITMAP_BLOCK_SIZE);
	}

	return offset;
}

HRESULT ParserSeparateVHDX::ParseCreateNew( LONGLONG VirtualDiskSize, LONG LogicalSectorSize, LONG PhysicalSectorSize, LONG BlockSize, CONST map<CString, CString> & ParentLocatorEntries, CONST GUID & page83Id, LONG VhdxType ) //2018-3-23 //!!@@##  //<sonmi01>2014-4-21 ###???
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	m_VHDX_PAGE83_DATA.m_Page83Data = page83Id; //2018-3-23 //!!@@##

	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apBufferHeaders;
	apBufferHeaders.Allocate(VhdxFileIdentifier::HeadersTotalSizeSpecConst());
	if (NULL == apBufferHeaders.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	ZeroMemory(apBufferHeaders.m_pData, VhdxFileIdentifier::HeadersTotalSizeSpecConst());  //zero data first

	//////////////////////////////////////////////////////////////////////////
	VhdxFileIdentifier * pVhdxFileIdentifier = (VhdxFileIdentifier*)(apBufferHeaders.m_pData + VhdxFileIdentifier::OffsetSpecConst());
	pVhdxFileIdentifier->m_Signature = VhdxFileIdentifier::SignatureSpecConst();
	static CONST TCHAR VHDX_CREATOR[] = TEXT("smart.master@live.com");
	_tcscpy_s((LPTSTR)pVhdxFileIdentifier->m_Creator, _countof(pVhdxFileIdentifier->m_Creator), VHDX_CREATOR);

	//////////////////////////////////////////////////////////////////////////
	VhdxHeader * pVhdxHeader1 = (VhdxHeader*)(apBufferHeaders.m_pData + VhdxHeader::Headser1OffsetSpecConst());
	pVhdxHeader1->m_Signature = VhdxHeader::SignatureSpecConst();

	CString strGuidFileWrite;
	CMiscHelper::CreateGuid(pVhdxHeader1->m_FileWrite, strGuidFileWrite);

	CString strGuidDataWrite;
	CMiscHelper::CreateGuid(pVhdxHeader1->m_DataWrite, strGuidDataWrite);

	CONST LONG VHDX_VERSION = 1;
	pVhdxHeader1->m_Version = VHDX_VERSION;

	CONST LONG __1M = 1024 *1024;
	pVhdxHeader1->m_LogOffset = __1M;
	pVhdxHeader1->m_LogLength = __1M;

	//////////////////////////////////////////////////////////////////////////
	VhdxHeader * pVhdxHeader2 = (VhdxHeader*)(apBufferHeaders.m_pData + VhdxHeader::Headser2OffsetSpecConst());
	CopyMemory(pVhdxHeader2, pVhdxHeader1, sizeof(VhdxHeader));

	pVhdxHeader1->m_SequenceNumber = 1;
	pVhdxHeader2->m_SequenceNumber = 2;

	pVhdxHeader1->CalcChecksum();
	pVhdxHeader2->CalcChecksum();

	CopyMemory(m_VhdxHeaderBuffer, pVhdxHeader2, sizeof(m_VhdxHeaderBuffer));
	m_pVhdxHeader = (VhdxHeader *)(m_VhdxHeaderBuffer);


	//////////////////////////////////////////////////////////////////////////
	VHDXRegionTableHeader * pVHDXRegionTableHeader = (VHDXRegionTableHeader*)(apBufferHeaders.m_pData + VHDXRegionTableHeader::OffsetSpecConst());
	pVHDXRegionTableHeader->m_Signature = VHDXRegionTableHeader::SignatureSpecConst();
	pVHDXRegionTableHeader->m_EntryCount = 2;

	VHDXRegionTableEntry * pRegionEntry = (VHDXRegionTableEntry*)((BYTE*)(pVHDXRegionTableHeader) + sizeof(VHDXRegionTableHeader));
	SetMetadataAndCalcBatTableCreateNew(VirtualDiskSize, LogicalSectorSize, PhysicalSectorSize, BlockSize, ParentLocatorEntries, VhdxType); //<sonmi01>2014-4-21 ###???
	CopyValue(pRegionEntry[0], m_VHDXRegionEntryBAT);

	pRegionEntry[1].m_Guid = VHDXRegionTableEntry::MetadataRegionGuidSpecConst();
	pRegionEntry[1].m_FileOffset = 2 * __1M;
	pRegionEntry[1].m_Length = __1M;
	pRegionEntry[1].m_Required = 1;

	pVHDXRegionTableHeader->CalcChecksum();

	//////////////////////////////////////////////////////////////////////////
	ULONG NumberOfBytesWritten = 0;
	hr = WriteAtOffsetHelper(m_pIFileDevice, 0, VhdxFileIdentifier::HeadersTotalSizeSpecConst(), &NumberOfBytesWritten, apBufferHeaders.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = ParseMetadataCreateNew(pRegionEntry[1].m_FileOffset); 
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ParseMetadata"), hr, TEXT(""));
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	hr = ParseBatTableCreateNew();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ParseBatTableCreateNew"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_AllocPosition = m_VHDXRegionEntryBAT.m_FileOffset + m_VHDXRegionEntryBAT.m_Length;

	if (VHDX_DISK_TYPE_ENUM::FIXED == VhdxType) //<sonmi01>2014-4-21 ###???
	{
		LONGLONG fileSize = AtlAlignUp(m_AllocPosition + VirtualDiskSize, BlockSize);
		D_INFO(0, TEXT("Fixed VHDX file size is %I64d"), fileSize);
		SetFileSizeFast(m_pIFileDevice->GetRawHandle(), fileSize);

		if (m_FileParameters.m_HasParent)
		{
			//do nothing
		}
		else
		{
			hr = UpdateFixedBat(m_AllocPosition);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("UpdateFixedBat"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
		}
		
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

VOID ParserSeparateVHDX::SetMetadataAndCalcBatTableCreateNew(LONGLONG VirtualDiskSize, LONG LogicalSectorSize, LONG PhysicalSectorSize, LONG BlockSize, CONST map<CString, CString> & ParentLocatorEntries, LONG VhdxType) //<sonmi01>2014-4-21 ###???
{
	//////////////////////////////////////////////////////////////////////////
	m_VirtualDiskSize.m_VirtualDiskSize = VirtualDiskSize;
	m_LogicalSectorSize.m_LogicalSectorSize = LogicalSectorSize;
	m_PhysicalSectorSize.m_PhysicalSectorSize = PhysicalSectorSize;
	m_FileParameters.m_BlockSize = BlockSize;
	if (VHDX_DISK_TYPE_ENUM::FIXED == VhdxType) //<sonmi01>2014-4-21 ###???
	{
		m_FileParameters.m_LeaveBlocksAllocated = 1;
	}
	if (ParentLocatorEntries.size())
	{
		m_ParentLocatorEntries = ParentLocatorEntries;
		m_FileParameters.m_HasParent = 1;
	}

	//////////////////////////////////////////////////////////////////////////
	CONST LONGLONG SECTOR_COUNT_PER_BITMAP_BLOCK = 8 * 1024 * 1024; //LONGLONG to avoid overflow
	m_ChunkRatio = (SECTOR_COUNT_PER_BITMAP_BLOCK * m_LogicalSectorSize.m_LogicalSectorSize) / m_FileParameters.m_BlockSize;

	m_DataBlocksCount = AtlAlignUp(m_VirtualDiskSize.m_VirtualDiskSize, m_FileParameters.m_BlockSize) / m_FileParameters.m_BlockSize;

	m_BitmapBlocksCount = (m_DataBlocksCount + m_ChunkRatio - 1) / m_ChunkRatio;

	if (m_FileParameters.m_HasParent)
	{
		m_BatEntriesCount = m_BitmapBlocksCount * (m_ChunkRatio + 1);
	}
	else
	{
		m_BatEntriesCount = m_DataBlocksCount + (m_DataBlocksCount - 1) /m_ChunkRatio;
	}

	m_BlockBitmapBufferSize = m_FileParameters.m_BlockSize / m_LogicalSectorSize.m_LogicalSectorSize / 8;

	D_INFO(0, TEXT("VirtualDiskSize=%I64d, BlockSize=%d,  LogicalSectorSize=%d,  PhysicalSectorSize=%d"), m_VirtualDiskSize.m_VirtualDiskSize, m_FileParameters.m_BlockSize,  m_LogicalSectorSize.m_LogicalSectorSize,  m_PhysicalSectorSize.m_PhysicalSectorSize);
	D_INFO(0, TEXT("ChunkRatio=%d, DataBlocksCount=%d, BitmapBlocksCount=%d, BatEntriesCount=%d, BlockBitmapBufferSize=%d"), m_ChunkRatio, m_DataBlocksCount, m_BitmapBlocksCount, m_BatEntriesCount, m_BlockBitmapBufferSize);

	//////////////////////////////////////////////////////////////////////////
	m_VHDXRegionEntryBAT.m_Guid = VHDXRegionTableEntry::BatGuidSpecConst();

	CONST LONG __1M = 1024 * 1024;
	m_VHDXRegionEntryBAT.m_FileOffset = 3 * __1M;
	m_VHDXRegionEntryBAT.m_Length = AtlAlignUp(m_BatEntriesCount * sizeof(VHDX_BAT_ENTRY), __1M);

	m_VHDXRegionEntryBAT.m_Required = 1;
}

HRESULT ParserSeparateVHDX::ParseMetadataCreateNew( LONGLONG MetadataOffset )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apMetadataBuffer;
	apMetadataBuffer.Allocate(VHDXMetadataTableHeader::SizeSpecConst());
	if (NULL == apMetadataBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	ZeroMemory(apMetadataBuffer.m_pData, VHDXMetadataTableHeader::SizeSpecConst());

	//////////////////////////////////////////////////////////////////////////
	VHDXMetadataTableHeader * pVHDXMetadataTableHeader = (VHDXMetadataTableHeader *)(apMetadataBuffer.m_pData);
	pVHDXMetadataTableHeader->m_Signature = VHDXMetadataTableHeader::SignatuireSpecConst();
	if (m_ParentLocatorEntries.size())
	{
		pVHDXMetadataTableHeader->m_EntryCount = 6;
	}
	else
	{
		pVHDXMetadataTableHeader->m_EntryCount = 5;
	}

	//////////////////////////////////////////////////////////////////////////
	VHDXMetadataTableEntry * pVHDXMetadataTableEntry = (VHDXMetadataTableEntry *)((BYTE*)(pVHDXMetadataTableHeader) + sizeof(VHDXMetadataTableHeader));
	ParseMetadataEntryCreateNew(apMetadataBuffer.m_pData, pVHDXMetadataTableEntry, pVHDXMetadataTableHeader->m_EntryCount);

	//////////////////////////////////////////////////////////////////////////
	ULONG NumberOfBytesWritten = 0;
	hr = WriteAtOffsetHelper(m_pIFileDevice, MetadataOffset, VHDXMetadataTableHeader::SizeSpecConst(), &NumberOfBytesWritten, apMetadataBuffer.m_pData);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

VOID ParserSeparateVHDX::ParseMetadataEntryCreateNew( LPBYTE pMetadataStart, VHDXMetadataTableEntry * pVHDXMetadataTableEntry, ULONG EntryCount )
{
	LONG Offset = VHDXMetadataTableEntry::MinOffsetSpecConst();

	LONG index = 0;

	//////////////////////////////////////////////////////////////////////////
	pVHDXMetadataTableEntry[index].m_ItemId = VHDXMetadataTableEntry::FileParametersGuidSpecConst();
	pVHDXMetadataTableEntry[index].m_Offset = Offset;
	pVHDXMetadataTableEntry[index].m_Length = sizeof(VHDX_FILE_PARAMETERS);
	pVHDXMetadataTableEntry[index].m_IsUser = 0;
	pVHDXMetadataTableEntry[index].m_IsVirtualDisk = 0;
	pVHDXMetadataTableEntry[index].m_IsRequired = 1;
	CopyMemory((VHDX_FILE_PARAMETERS *)(pMetadataStart + Offset), &m_FileParameters, pVHDXMetadataTableEntry[index].m_Length);
	Offset += pVHDXMetadataTableEntry[index].m_Offset;
	++ index;

	//////////////////////////////////////////////////////////////////////////
	pVHDXMetadataTableEntry[index].m_ItemId = VHDXMetadataTableEntry::VirtualDiskSizeGuidSpecConst();
	pVHDXMetadataTableEntry[index].m_Offset = Offset;
	pVHDXMetadataTableEntry[index].m_Length = sizeof(VHDX_VIRTUAL_DISK_SIZE);
	pVHDXMetadataTableEntry[index].m_IsUser = 0;
	pVHDXMetadataTableEntry[index].m_IsVirtualDisk = 1;
	pVHDXMetadataTableEntry[index].m_IsRequired = 1;
	CopyMemory((VHDX_VIRTUAL_DISK_SIZE*)(pMetadataStart + Offset), &m_VirtualDiskSize, pVHDXMetadataTableEntry[index].m_Length);
	Offset += pVHDXMetadataTableEntry[index].m_Length;
	++ index;

	//////////////////////////////////////////////////////////////////////////
	pVHDXMetadataTableEntry[index].m_ItemId = VHDXMetadataTableEntry::PhysicalSectorSizeGuidSpecConst();
	pVHDXMetadataTableEntry[index].m_Offset = Offset;
	pVHDXMetadataTableEntry[index].m_Length = sizeof(VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE);
	pVHDXMetadataTableEntry[index].m_IsUser = 0;
	pVHDXMetadataTableEntry[index].m_IsVirtualDisk = 1;
	pVHDXMetadataTableEntry[index].m_IsRequired = 1;
	CopyMemory((VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE*)(pMetadataStart + Offset), &m_PhysicalSectorSize, pVHDXMetadataTableEntry[index].m_Length);
	Offset += pVHDXMetadataTableEntry[index].m_Length;
	++ index;

	//////////////////////////////////////////////////////////////////////////
	pVHDXMetadataTableEntry[index].m_ItemId = VHDXMetadataTableEntry::LogicalSectorSizeGuidSpecConst();
	pVHDXMetadataTableEntry[index].m_Offset = Offset;
	pVHDXMetadataTableEntry[index].m_Length = sizeof(VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE);
	pVHDXMetadataTableEntry[index].m_IsUser = 0;
	pVHDXMetadataTableEntry[index].m_IsVirtualDisk = 1;
	pVHDXMetadataTableEntry[index].m_IsRequired = 1;
	CopyMemory((VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE*)(pMetadataStart + Offset), &m_LogicalSectorSize, pVHDXMetadataTableEntry[index].m_Length);
	Offset += pVHDXMetadataTableEntry[index].m_Length;
	++ index;

	//////////////////////////////////////////////////////////////////////////
	pVHDXMetadataTableEntry[index].m_ItemId = VHDXMetadataTableEntry::Page83DataGuidSpecConst();
	pVHDXMetadataTableEntry[index].m_Offset = Offset;
	pVHDXMetadataTableEntry[index].m_Length = sizeof(VHDX_PAGE83_DATA);
	pVHDXMetadataTableEntry[index].m_IsUser = 0;
	pVHDXMetadataTableEntry[index].m_IsVirtualDisk = 1;
	pVHDXMetadataTableEntry[index].m_IsRequired = 1;
	//static CONST GUID Page83DataGuid = {0x625C11F6, 0xC704, 0x4377, {0x8B, 0xDC, 0xD4, 0xC4, 0x12, 0xC6, 0x70, 0x60}}; //do not know why?? copy a value from MS created VHDX
	//CopyMemory((VHDX_PAGE83_DATA*)(pMetadataStart + Offset), &Page83DataGuid, pVHDXMetadataTableEntry[index].m_Length);
	if (IsEqualGUID(m_VHDX_PAGE83_DATA.m_Page83Data, GUID_NULL))
	{
		CString strGuidTemp;
		CMiscHelper::CreateGuid(m_VHDX_PAGE83_DATA.m_Page83Data, strGuidTemp);
	}
	CopyMemory((VHDX_PAGE83_DATA*)(pMetadataStart + Offset), &m_VHDX_PAGE83_DATA.m_Page83Data, pVHDXMetadataTableEntry[index].m_Length);
	Offset += pVHDXMetadataTableEntry[index].m_Length;
	++ index;

	//////////////////////////////////////////////////////////////////////////
	if (m_ParentLocatorEntries.size())
	{
		pVHDXMetadataTableEntry[index].m_ItemId = VHDXMetadataTableEntry::ParentLocatorGuidSpecConst();
		//Offset = ArithmeticAlignUp(Offset, sizeof(VHDX_PARENT_LOCATOR_HEADER));
		pVHDXMetadataTableEntry[index].m_Offset = Offset;
		pVHDXMetadataTableEntry[index].m_Length = -1;
		pVHDXMetadataTableEntry[index].m_IsUser = 0;
		pVHDXMetadataTableEntry[index].m_IsVirtualDisk = 0;
		pVHDXMetadataTableEntry[index].m_IsRequired = 0;

		LONG len = 0;

		VHDX_PARENT_LOCATOR_HEADER  * pVHDX_PARENT_LOCATOR_HEADER = (VHDX_PARENT_LOCATOR_HEADER  *)(pMetadataStart + pVHDXMetadataTableEntry[index].m_Offset);
		pVHDX_PARENT_LOCATOR_HEADER->m_LocatorType = VHDX_PARENT_LOCATOR_HEADER::LocatorTypeGuidSpecConst();
		len += sizeof(VHDX_PARENT_LOCATOR_HEADER);

		pVHDX_PARENT_LOCATOR_HEADER->m_KeyValueCount = m_ParentLocatorEntries.size();
		VHDX_PARENT_LOCATOR_ENTRY * pVHDX_PARENT_LOCATOR_ENTRY = (VHDX_PARENT_LOCATOR_ENTRY *)((BYTE*)(pVHDX_PARENT_LOCATOR_HEADER) + sizeof(VHDX_PARENT_LOCATOR_HEADER));
		len += sizeof(VHDX_PARENT_LOCATOR_HEADER) * pVHDX_PARENT_LOCATOR_HEADER->m_KeyValueCount;

		map<CString, CString>::const_iterator citer = m_ParentLocatorEntries.begin();

		for (LONG ii = 0; ii < pVHDX_PARENT_LOCATOR_HEADER->m_KeyValueCount; ++ ii)
		{
			pVHDX_PARENT_LOCATOR_ENTRY[ii].m_KeyOffset = len;
			pVHDX_PARENT_LOCATOR_ENTRY[ii].m_KeyLength = citer->first.GetLength() * sizeof(TCHAR);
			len += pVHDX_PARENT_LOCATOR_ENTRY[ii].m_KeyLength;

			CopyMemory((LPBYTE)pVHDX_PARENT_LOCATOR_HEADER + pVHDX_PARENT_LOCATOR_ENTRY[ii].m_KeyOffset, citer->first.GetString(), pVHDX_PARENT_LOCATOR_ENTRY[ii].m_KeyLength);

			pVHDX_PARENT_LOCATOR_ENTRY[ii].m_ValueOffset = len;
			pVHDX_PARENT_LOCATOR_ENTRY[ii].m_ValueLength = citer->second.GetLength() * sizeof(TCHAR);
			len += pVHDX_PARENT_LOCATOR_ENTRY[ii].m_ValueLength;

			CopyMemory((LPBYTE)pVHDX_PARENT_LOCATOR_HEADER + pVHDX_PARENT_LOCATOR_ENTRY[ii].m_ValueOffset, citer->second.GetString(), pVHDX_PARENT_LOCATOR_ENTRY[ii].m_ValueLength);;

			++ citer;
		}

		pVHDXMetadataTableEntry[index].m_Length = len;
		Offset += pVHDXMetadataTableEntry[index].m_Length;
		++ index;
	}

	ATLASSERT(EntryCount == index);
}

HRESULT ParserSeparateVHDX::ParseBatTableCreateNew()
{
	//////////////////////////////////////////////////////////////////////////
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_VHDXRegionEntryBAT.m_Length > MAX_BAT_TABLE_LEN)
	{
		CONST LONG __1M = 1024 * 1024;
		m_CacheIoBatTable.InitParams(m_pIFileDevice, __1M);

		CVMemPtr<BYTE> apBatTableTmp;
		apBatTableTmp.Allocate(__1M);
		if (NULL == apBatTableTmp.m_pData)
		{
			hr = E_OUTOFMEMORY;
			D_API_ERR(0, TEXT("Allocate"), hr, TEXT("1M"));
			LEAVE_BLOCK(0);
		}
		ZeroMemory(apBatTableTmp.m_pData, __1M);

		//////////////////////////////////////////////////////////////////////////
		LONGLONG BatFileOffset = m_VHDXRegionEntryBAT.m_FileOffset;
		for (LONG ii = 0; ii < m_VHDXRegionEntryBAT.m_Length / __1M; ++ ii)
		{
			ULONG NumberOfBytesWritten = 0;
			hr = WriteAtOffsetHelper(m_pIFileDevice, BatFileOffset, __1M, &NumberOfBytesWritten, (LPCBYTE)apBatTableTmp.m_pData);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
				break;
			}

			BatFileOffset += __1M;
		}
		if (FAILED(hr))
		{
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		m_apBatTable.Allocate(m_VHDXRegionEntryBAT.m_Length / sizeof(VHDX_BAT_ENTRY));
		if (NULL == m_apBatTable.m_pData)
		{
			hr = E_OUTOFMEMORY;
			D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		ZeroMemory(m_apBatTable.m_pData, m_VHDXRegionEntryBAT.m_Length);

		//////////////////////////////////////////////////////////////////////////
		ULONG NumberOfBytesWritten = 0;
		hr = WriteAtOffsetHelper(m_pIFileDevice, m_VHDXRegionEntryBAT.m_FileOffset, m_VHDXRegionEntryBAT.m_Length, &NumberOfBytesWritten, (LPCBYTE)m_apBatTable.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
	}
	

	//////////////////////////////////////////////////////////////////////////
	m_CacheIoBitmap.InitParams(m_pIFileDevice, VHDX_BAT_ENTRY::SECTOR_BITMAP_BLOCK_SIZE);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserSeparateVHDX::ReadBatEntry( LONG BatIndex, VHDX_BAT_ENTRY & batEntry )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_apBatTable.m_pData)
	{
		CopyMemory(&batEntry, &(m_apBatTable.m_pData[BatIndex]), sizeof(VHDX_BAT_ENTRY));
	}
	else
	{
		LONGLONG FileOffset = m_VHDXRegionEntryBAT.m_FileOffset + BatIndex * sizeof(VHDX_BAT_ENTRY);
		hr = m_CacheIoBatTable.ReadAtOffset(FileOffset, sizeof(VHDX_BAT_ENTRY), (LPBYTE)(&batEntry));
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadAtOffset"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserSeparateVHDX::WriteBatEntry( LONG BatIndex, CONST VHDX_BAT_ENTRY & batEntry )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);

	m_NeedToUpdateBAT = TRUE;

	//////////////////////////////////////////////////////////////////////////
	if (m_apBatTable.m_pData)
	{
		CopyMemory(&(m_apBatTable.m_pData[BatIndex]), &batEntry, sizeof(VHDX_BAT_ENTRY));
	}
	else
	{
		LONGLONG FileOffset = m_VHDXRegionEntryBAT.m_FileOffset + BatIndex * sizeof(VHDX_BAT_ENTRY);
		hr = m_CacheIoBatTable.WriteAtOffset(FileOffset, sizeof(VHDX_BAT_ENTRY), (LPCBYTE)(&batEntry));
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadAtOffset"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

LONGLONG ParserSeparateVHDX::GetPhysicalAddress(LONGLONG LogicalAddress) //<sonmi01>2013-10-29 ###???
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	LONGLONG PhysicalAddress = -1;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG BlockIndex = LogicalAddress / m_FileParameters.m_BlockSize;
	ATLASSERT(BlockIndex < m_DataBlocksCount);

	LONG BatIndex = BlockIndexToBatIndex(BlockIndex);
	VHDX_BAT_ENTRY BatEntry = { 0 }; //<sonmi01>2013-9-11 ###???
	hr = ReadBatEntry(BatIndex, BatEntry);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadBatEntry"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	switch (BatEntry.m_State)
	{
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_NOT_PRESENT:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNDEFINED:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_ZERO:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNMAPPED:
		D_INFO(999, TEXT("Data Block not allocated. BAT entry state=%d, BlockIndex=%d, BatIndex=%d"), (INT)BatEntry.m_State, BlockIndex, BatIndex);
		break;

	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_FULLY_PRESENT:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_PARTIALLY_PRESENT:
		PhysicalAddress = BatEntry.m_FileOffsetMB;
		PhysicalAddress <<= 20;
		PhysicalAddress += (LogicalAddress % m_FileParameters.m_BlockSize);
		break;

	default:
		hr = E_INVALIDARG;
		D_INFO(0, TEXT("Invalid Data Block BAT entry state %d, BlockIndex=%d, BatIndex=%d"), (INT)BatEntry.m_State, BlockIndex, BatIndex);
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return PhysicalAddress;
}

//<sonmi01>2014-1-9 ###???
HRESULT ParserSeparateVHDX::DumpBatTable( IFileDevice * pIFileDeviceOut )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONGLONG Offset = m_VHDXRegionEntryBAT.m_FileOffset;
	LONG TotalLength = m_VHDXRegionEntryBAT.m_Length;

	//////////////////////////////////////////////////////////////////////////
	CONST LONG BUFFER_LENTH = 1024 * 1024;
	CVMemPtr<BYTE> apBuffer;
	apBuffer.Allocate(BUFFER_LENTH);
	if (NULL == apBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG Readlength = 0;
	LONGLONG WriteOffset = 0;
	ULONG NumberOfBytesRead = 0;
	ULONG NumberOfBytesWritten = 0;
	while (TotalLength)
	{
		if (TotalLength >= BUFFER_LENTH)
		{
			Readlength = BUFFER_LENTH;
		}
		else
		{
			Readlength = TotalLength;
		}

		NumberOfBytesRead = 0;
		hr = ReadAtOffsetHelper(m_pIFileDevice, Offset, Readlength, &NumberOfBytesRead, apBuffer.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
			break;
		}

		if (Readlength != NumberOfBytesRead)
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT("ReadLengthError"), hr, TEXT("%u, %u"), Readlength, NumberOfBytesRead);
			break;
		}

		NumberOfBytesWritten = 0;
		hr = WriteAtOffsetHelper(pIFileDeviceOut, WriteOffset, Readlength, &NumberOfBytesWritten, apBuffer.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			break;
		}

		if (Readlength != NumberOfBytesWritten)
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT("WriteLengthError"), hr, TEXT("%u, %u"), Readlength, NumberOfBytesWritten);
			break;
		}

		Offset += Readlength;
		WriteOffset += Readlength;
		TotalLength -= Readlength;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//<sonmi01>2014-1-9 ###???
HRESULT ParserSeparateVHDX::DumpBatTable( LPCTSTR FileName )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		FileName,//IN LPCTSTR strFileName, 
		GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("FileName=%s"), FileName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = DumpBatTable(spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("DumpBatTable"), hr, TEXT("FileName=%s"), FileName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	if (FAILED(hr))
	{
		BOOL bRet = DeleteFile(FileName);
		if (!bRet)
		{
			DWORD LastError = GetLastError();
			D_API_ERR(0, TEXT("DeleteFile"), LastError, TEXT("FileName=%s"), FileName);
		}
	}

	return hr;
}

HRESULT ParserSeparateVHDX::OpenDumpedBatTable( LPCTSTR FileName )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CloseDumpedBatTable();

	//////////////////////////////////////////////////////////////////////////
	hr =  CreateInstanceFileDeviceWin32(
		FileName,//IN LPCTSTR strFileName, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&m_pDumpedBatTalbeFile,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("FileName=%s"), FileName);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CONST LONG __1M = 1024 * 1024;
	m_pCacheIoDumpedBatTalbe = new CCacheIO(m_pDumpedBatTalbeFile, FALSE, __1M);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserSeparateVHDX::ReadDumpedBatEntry( LONG BlockIndex, VHDX_BAT_ENTRY & BatEntry )
{
	if (NULL == m_pDumpedBatTalbeFile)
	{
		return E_POINTER; //<sonmi01>2014-1-13 ###???
	}

	LONG BatIndex = BlockIndexToBatIndex(BlockIndex);
	LONGLONG FileOffset = BatIndex * sizeof(VHDX_BAT_ENTRY);
	HRESULT hr = m_pCacheIoDumpedBatTalbe->ReadAtOffset(FileOffset, sizeof(VHDX_BAT_ENTRY), (LPBYTE)(&BatEntry));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadAtOffset"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}
	return hr;
}

HRESULT ParserSeparateVHDX::CloseDumpedBatTable()
{
	if (m_pDumpedBatTalbeFile)
	{
		m_pCacheIoDumpedBatTalbe->Flush();
		delete m_pCacheIoDumpedBatTalbe; m_pCacheIoDumpedBatTalbe = NULL;

		m_pDumpedBatTalbeFile->Release(); m_pDumpedBatTalbeFile = NULL;
	}
	return S_OK;
}

//<sonmi01>2014-1-13 ###???
HRESULT ParserSeparateVHDX::CompareWithDumpedBatForZeroedData( LONG BlockIndex, BOOL & bResult )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	bResult = FALSE;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	VHDX_BAT_ENTRY DumpedBatEntry = {0};
	hr = ReadDumpedBatEntry(BlockIndex, DumpedBatEntry);
	if (FAILED(hr))
	{
		if (E_POINTER != hr)
		{
			D_API_ERR(0, TEXT("ReadDumpedBatEntry"), hr, TEXT(""));
		}
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	VHDX_BAT_ENTRY CurrentBatEntry = {0};
	hr = ReadBatEntry(BlockIndex, CurrentBatEntry);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadDumpedBatEntry"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	bResult = (VHDX_BAT_ENTRY::CurentDiskDataZeroedOrUndefined(DumpedBatEntry) && VHDX_BAT_ENTRY::CurentDiskDataZeroedOrUndefined(CurrentBatEntry));

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//<sonmi01>2014-2-24 ###???
HRESULT ParserSeparateVHDX::GenerateReplayedLogFile(CONST CString & OriginalVhdxFile, CONST CString & ReplayedLogFolder, CString & LogFileName, IFileDevice ** ppIFileDevice)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	static CONST TCHAR REPLAYED_LOG_EXT[] = TEXT(".VhdxReplay"); //<sonmi01>2014-2-25 ###???
	static CONST TCHAR REPLAYED_LOG_FOLDER[] = TEXT("TEMP");
	if (0 == OriginalVhdxFile.GetLength() && 0 == ReplayedLogFolder.GetLength())
	{
		//////////////////////////////////////////////////////////////////////////
		CONST LONG BUFFER_SIZE = 1024;
		LPTSTR pBuffer = LogFileName.GetBuffer(BUFFER_SIZE);
		DWORD dwGetModuleFileName = GetModuleFileName(
			NULL,//_In_opt_  HMODULE hModule,
			pBuffer,//_Out_     LPTSTR lpFilename,
			BUFFER_SIZE//_In_      DWORD nSize
			);
		if (0 == dwGetModuleFileName)
		{
			LogFileName.ReleaseBuffer();
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_FISRT_STATUS(hr, LastError);
			D_SET_FISRT_ERROR_MESSAGE(TEXT("GetModuleFileName"));
			D_API_ERR(0, TEXT("GetModuleFileName"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		LPTSTR pDelim = _tcsrchr(pBuffer, TEXT('\\'));
		if (pDelim)
		{
			pDelim[1] = 0;
		}
		LogFileName.ReleaseBuffer();
		LogFileName += REPLAYED_LOG_FOLDER;

		//////////////////////////////////////////////////////////////////////////
		hr = CMiscHelper::CreateDirectoryRecursively(LogFileName.GetString());
		if (FAILED(hr))
		{
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			D_SET_FISRT_STATUS(hr, LastError);
			D_SET_FISRT_ERROR_MESSAGE(TEXT("CreateDirectoryRecursively"));
			D_API_ERR(0, TEXT("CreateDirectoryRecursively"), hr, TEXT("%s"), LogFileName.GetString());
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		GUID Guid = { 0 };
		CString strGuid;
		CMiscHelper::CreateGuid(Guid, strGuid);
		LogFileName += TEXT('\\');
		LogFileName += strGuid;
		LogFileName += REPLAYED_LOG_EXT;
	}
	else
	{
		LogFileName = ReplayedLogFolder;
		INT Index = OriginalVhdxFile.ReverseFind(TEXT('\\'));
		if (-1 != Index)
		{
			LogFileName += OriginalVhdxFile.Mid(Index + 1);
		}
		else
		{
			LogFileName += OriginalVhdxFile;
		}
		LogFileName += REPLAYED_LOG_EXT;
	}

	//////////////////////////////////////////////////////////////////////////
	hr = CreateInstanceFileDeviceWin32(
		LogFileName.GetString(),//IN LPCTSTR strFileName,
		GENERIC_ALL,//IN ULONG ulDesiredAccess,
		0,//IN ULONG ulShareMode,
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition,
		0,//IN ULONG ulFlagsAndAttributes,
		ppIFileDevice,//OUT IFileDevice ** ppFileDevice,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("CreateInstanceFileDeviceWin32"));
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("%s"), LogFileName.GetString());
		LEAVE_BLOCK(0);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserSeparateVHDX::CopyNewReplayedLog(LONGLONG CopyLen)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDevice;
	if (NULL == m_ReplayedLogFileDevice) //<sonmi01>2014-3-1 ###???
	{
		hr = GenerateReplayedLogFile(m_OriginalVhdxFile, m_ReplayedLogFolder, m_ReplayedLogFileName, &spIFileDevice);
		if (FAILED(hr))
		{
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			D_SET_FISRT_STATUS(hr, LastError);
			D_SET_FISRT_ERROR_MESSAGE(TEXT("GenerateReplayedLogFile"));
			D_API_ERR(0, TEXT("GenerateReplayedLogFile"), hr, TEXT("%s"), m_ReplayedLogFileName.GetString());
			LEAVE_BLOCK(0);
		}
	}
	

	//////////////////////////////////////////////////////////////////////////
	//LONGLONG CopyLen = max(m_pVhdxHeader->m_LogOffset + m_pVhdxHeader->m_LogLength, m_VHDXRegionEntryBAT.m_FileOffset + m_VHDXRegionEntryBAT.m_Length);
	map<LONGLONG, LONGLONG> DataRange;
	DataRange.insert(make_pair(0LL, CopyLen));
	CComPtr<IFileCopy> spIFileCopy;
	hr = CreateInstanceCSTCopy(
		m_pIFileDevice,//IFileDevice * pSource, 
		spIFileDevice.p ? spIFileDevice.p : m_ReplayedLogFileDevice,//IFileDevice * pDest, 
		&DataRange,//CONST map<LONGLONG, LONGLONG> & DataRange, 
		0,//LONGLONG OffsetDiff, 
		&spIFileCopy,//IFileCopy ** ppObj, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("CreateInstanceCSTCopy"));
		D_API_ERR(0, TEXT("CreateInstanceCSTCopy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spIFileCopy->Copy();
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("IFileCopy->Copy"));
		D_API_ERR(0, TEXT("IFileCopy->Copy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (NULL == m_ReplayedLogFileDevice) //<sonmi01>2014-3-1 ###???
	{
		m_ReplayedLogFileDevice = spIFileDevice.Detach(); //<sonmi01>2014-2-25 ###???
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserSeparateVHDX::ReplayLog()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	static GUID NULL_GUID = { 0 };
	if (0 == memcmp(&m_pVhdxHeader->m_Log, &NULL_GUID, sizeof(GUID)))
	{
		D_INFO(0, TEXT("Log GUID is null, no need to replay log"));
		LEAVE_BLOCK(0);
	}
	D_INFO(0, TEXT("Log GUID is not null and replay log"));


	//////////////////////////////////////////////////////////////////////////
	LONGLONG CopyLen = max(m_pVhdxHeader->m_LogOffset + m_pVhdxHeader->m_LogLength, m_VHDXRegionEntryBAT.m_FileOffset + m_VHDXRegionEntryBAT.m_Length);
	hr = CopyNewReplayedLog(CopyLen); //<sonmi01>2014-3-1 ###???
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("CopyNewReplayedLog"));
		D_API_ERR(0, TEXT("CopyNewReplayedLog"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CreateInstcanceCVHDXLogReplayCallback(CopyLen, &m_pLogReplayCallback, TRUE);
	if (NULL == m_pLogReplayCallback)
	{
		hr = E_OUTOFMEMORY;
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("CreateInstcanceCVHDXLogReplayCallback"));
		D_API_ERR(0, TEXT("CreateInstcanceCVHDXLogReplayCallback"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = VhdxReplayLogAPI(m_ReplayedLogFileDevice, LONGLONG_MAX, m_pLogReplayCallback); //<sonmi01>2014-2-25 ###???
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_FISRT_STATUS(hr, LastError);
		D_SET_FISRT_ERROR_MESSAGE(TEXT("VhdxReplayLogAPI"));
		D_API_ERR(0, TEXT("VhdxReplayLogAPI"), hr, TEXT("%s"), m_ReplayedLogFileName.GetString());
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	//LONG NewCopyLen = m_pLogReplayCallback->GetMaxOffset(); //<sonmi01>2014-3-1 ###???
	//if (NewCopyLen > CopyLen)
	//{
	//	CONST LONG __1M = 1024 * 1024;
	//	NewCopyLen = AtlAlignUp(NewCopyLen, __1M);
	//	hr = CopyNewReplayedLog(NewCopyLen); //<sonmi01>2014-3-1 ###???
	//	if (FAILED(hr))
	//	{
	//		//LastError = GetLastError();
	//		//hr = HRESULT_FROM_WIN32(LastError);
	//		D_SET_FISRT_STATUS(hr, LastError);
	//		D_SET_FISRT_ERROR_MESSAGE(TEXT("CopyNewReplayedLog"));
	//		D_API_ERR(0, TEXT("CopyNewReplayedLog"), hr, TEXT(""));
	//		LEAVE_BLOCK(0);
	//	}

	//	hr = VhdxReplayLogAPI(m_ReplayedLogFileDevice, LONGLONG_MAX, m_pLogReplayCallback); //<sonmi01>2014-2-25 ###???
	//	if (FAILED(hr))
	//	{
	//		//LastError = GetLastError();
	//		//hr = HRESULT_FROM_WIN32(LastError);
	//		D_SET_FISRT_STATUS(hr, LastError);
	//		D_SET_FISRT_ERROR_MESSAGE(TEXT("VhdxReplayLogAPI"));
	//		D_API_ERR(0, TEXT("VhdxReplayLogAPI"), hr, TEXT("%s"), m_ReplayedLogFileName.GetString());
	//		LEAVE_BLOCK(0);
	//	}
	//}


	//hr = CreateInstanceFileDeviceWin32(
	//	m_ReplayedLogFileName.GetString(),//IN LPCTSTR strFileName,
	//	GENERIC_ALL,//IN ULONG ulDesiredAccess,
	//	0,//IN ULONG ulShareMode,
	//	OPEN_EXISTING,//IN ULONG ulCreationDisposition,
	//	0,//IN ULONG ulFlagsAndAttributes,
	//	&m_ReplayedLogFileDevice,//OUT IFileDevice ** ppFileDevice,
	//	TRUE//BOOL bCreateMember
	//	);
	//if (FAILED(hr))
	//{
	//	//LastError = GetLastError();
	//	//hr = HRESULT_FROM_WIN32(LastError);
	//	D_SET_FISRT_STATUS(hr, LastError);
	//	D_SET_FISRT_ERROR_MESSAGE(TEXT("CreateInstanceFileDeviceWin32"));
	//	D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("%s"), m_ReplayedLogFileName.GetString());
	//	LEAVE_BLOCK(0);
	//}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	if (FAILED(hr)) //<sonmi01>2014-2-25 ###???
	{
		if (m_ReplayedLogFileDevice)
		{
			m_ReplayedLogFileDevice->Release();
			m_ReplayedLogFileDevice = NULL;
		}
	}
	
	return hr;
}

VOID ParserSeparateVHDX::ReplayPayloadBlockAll(LONGLONG BlockOffset, LONG BlockLen, BYTE * pBuffer, CONST vector<TReplayedData> & replayedData) //<sonmi01>2014-3-1 ###???
{
	for (size_t ii = 0; ii < replayedData.size(); ++ii)
	{
		ReplayPayloadBlockSingle(BlockOffset, BlockLen, pBuffer, replayedData[ii]);
	}
}

VOID ParserSeparateVHDX::ReplayPayloadBlockSingle(LONGLONG BlockOffset, LONG BlockLen, BYTE * pBuffer, CONST TReplayedData & replayedData) //<sonmi01>2014-3-1 ###???
{
	LONGLONG start = 0;
	LONGLONG end = 0;
	LineIntersect<LONGLONG>(
		BlockOffset, BlockOffset + BlockLen,
		replayedData.m_FileOffset, replayedData.m_FileOffset + replayedData.m_DataLength,
		start, end);

	if (start < end)
	{
		if (replayedData.m_pData)
		{
			CopyMemory(pBuffer + (start - BlockOffset), replayedData.m_pData + (start - replayedData.m_FileOffset), end - start);
		}
		else
		{
			ZeroMemory(pBuffer + (start - BlockOffset), end - start);
		}
	}


#if 0
	if (replayedData.m_FileOffset >= BlockOffset && replayedData.m_FileOffset < BlockOffset + BlockLen)
	{
		LONGLONG CopyLen = min(replayedData.m_DataLength, BlockOffset + BlockLen - replayedData.m_FileOffset);
		LONGLONG Offset = replayedData.m_FileOffset - BlockOffset;
		if (replayedData.m_pData)
		{
			CopyMemory(pBuffer + Offset, replayedData.m_pData, CopyLen);
		}
		else
		{
			ZeroMemory(pBuffer + Offset, CopyLen);
		}
	}
	else if (replayedData.m_FileOffset + replayedData.m_DataLength > BlockOffset && replayedData.m_FileOffset + replayedData.m_DataLength <= BlockOffset + BlockLen)
	{
		LONGLONG CopyLen = replayedData.m_FileOffset + replayedData.m_DataLength - BlockOffset;
		LONGLONG Offset = BlockOffset - replayedData.m_FileOffset;
		if (replayedData.m_pData)
		{
			CopyMemory(pBuffer, replayedData.m_pData + Offset, CopyLen);
		}
		else
		{
			ZeroMemory(pBuffer, CopyLen);
		}
	}
	else if (replayedData.m_FileOffset <= BlockOffset && replayedData.m_FileOffset + replayedData.m_DataLength >= BlockOffset + BlockLen)
	{
		LONGLONG CopyLen = BlockLen;
		LONGLONG Offset = BlockOffset - replayedData.m_FileOffset;
		if (replayedData.m_pData)
		{
			CopyMemory(pBuffer, replayedData.m_pData + Offset, CopyLen);
		}
		else
		{
			ZeroMemory(pBuffer, CopyLen);
		}
	}
#endif
}

VOID ParserSeparateVHDX::ReplayPayloadBlock(LONGLONG BlockOffset, LONG BlockLen, BYTE * pBuffer, IVHDXLogReplayCallback * pCallBack)
{
	if (pCallBack)
	{
		LONGLONG start = 0;
		LONGLONG end = 0;
		LineIntersect<LONGLONG>(BlockOffset, BlockOffset + BlockLen, pCallBack->GetMinOffset(), pCallBack->GetMaxOffset(), start, end);
		if (start < end)
		{
			ReplayPayloadBlockAll(BlockOffset, BlockLen, pBuffer, pCallBack->GetReplayedData());
		}
	}

#if 0
	if (pCallBack)
	{
		LONGLONG MaxOffset = pCallBack->GetMaxOffset();
		LONGLONG MinOffset = pCallBack->GetMinOffset();
		if ((BlockOffset >= MinOffset && BlockOffset < MaxOffset) ||
			(BlockOffset + BlockLen > MinOffset && BlockOffset + BlockLen <= MaxOffset))
		{
			ReplayPayloadBlockAll(BlockOffset, BlockLen, pBuffer, pCallBack->GetReplayedData());
		}
	}
#endif
}

//win12r2 has fixed the defect that fixed VHDX got shrunken
HRESULT ParserSeparateVHDX::UpdateFixedBat(LONGLONG StartOffset)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	VHDX_BAT_ENTRY BatEntry = { 0 };
	BatEntry.m_State = VHDX_BAT_ENTRY::PAYLOAD_BLOCK_FULLY_PRESENT;
	for (LONG BlockIndex = 0; BlockIndex < m_DataBlocksCount; ++BlockIndex)
	{
		LONG BatIndex = BlockIndexToBatIndex(BlockIndex);
		BatEntry.m_FileOffsetMB = (StartOffset >> 20);
		hr = WriteBatEntry(BatIndex, BatEntry);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteBatEntry"), hr, TEXT("failed for [BlockIndex, BatIndex]=[%u, %u]"), BlockIndex, BatIndex);
			break;
		}
		StartOffset += m_FileParameters.m_BlockSize;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//<sonmi01>2014-9-24 ###???
HRESULT ParserSeparateVHDX::IsBolockContainsData(LONG BlockIndex, BOOL & bResult)
{
	ATLASSERT(BlockIndex < m_DataBlocksCount);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	bResult = FALSE;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG BatIndex = BlockIndexToBatIndex(BlockIndex);
	VHDX_BAT_ENTRY BatEntry = { 0 }; //<sonmi01>2013-9-11 ###???
	hr = ReadBatEntry(BatIndex, BatEntry);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadBatEntry"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//for dynamic disk, bit 1 means data block is allocated, 0 means data block is not-allocated or is undefined
	//for differencing disk, bit 1 means data block is on current disk, 0 means data block need to be inspected from its parent or is undefined

	//////////////////////////////////////////////////////////////////////////
	switch (BatEntry.m_State)
	{
		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_NOT_PRESENT:
	{
		bResult = FALSE;
	}
		break;

	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNDEFINED: //<sonmi01>2014-1-13 ###???
	{
		bResult = FALSE;
	}
		break;

	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNMAPPED:
	{
		//ZeroMemory(Buffer, m_BlockBitmapBufferSize); //<sonmi01>2014-1-9 #bitmap all ONE, data all ZERO for child VHDX
		if (m_FileParameters.m_HasParent)
		{
			bResult = TRUE;
		}
		else
		{
			bResult = FALSE;
		}
	}
		break;

		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_ZERO:
	{
		//if (m_FileParameters.m_HasParent)
		{
			bResult = TRUE;
		}
		//else
		//{
		//	ZeroMemory(Buffer, m_BlockBitmapBufferSize);
		//}
	}
		break;

	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_FULLY_PRESENT:
	{
		bResult = TRUE;
	}
		break;

		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_PARTIALLY_PRESENT:
	{
		bResult = TRUE;
	}
		break;

		//////////////////////////////////////////////////////////////////////////
	default:
	{
		hr = E_INVALIDARG;
		D_INFO(0, TEXT("Invalid Data Block BAT entry state %d, BlockIndex=%d, BatIndex=%d"), (INT)BatEntry.m_State, BlockIndex, BatIndex);
	}
		break;
	}

	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserSeparateVHDX::FindDataBlock(LONG StartBlockIndex, LONG & First)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	LONG ii;
	for (ii = StartBlockIndex; ii < m_DataBlocksCount; ++ii)
	{
		BOOL bResult = FALSE;
		hr = IsBolockContainsData(ii, bResult);
		if (FAILED(hr))
		{
			break;
		}

		if (bResult)
		{
			First = ii;
			break;
		}
	}

	if (ii >= m_DataBlocksCount)
	{
		First = m_DataBlocksCount;
	}
	return hr;
}

HRESULT ParserSeparateVHDX::FindNonDataBlock(LONG StartBlockIndex, LONG & Last)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	LONG ii;
	for (ii = StartBlockIndex; ii < m_DataBlocksCount; ++ii)
	{
		BOOL bResult = FALSE;
		hr = IsBolockContainsData(ii, bResult);
		if (FAILED(hr))
		{
			break;
		}

		if (!bResult)
		{
			Last = ii;
			break;
		}
	}

	if (ii >= m_DataBlocksCount)
	{
		Last = m_DataBlocksCount;
	}
	return hr;
}

HRESULT ParserSeparateVHDX::FindDataBLockRange(LONG StartBlockIndex, LONG & First, LONG & Last)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = FindDataBlock(StartBlockIndex, First);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	hr = FindNonDataBlock(First, Last);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserSeparateVHDX::GetBlocksBitmap(LONG & BlockSize, LONG & BlockCount, BYTE ** ppBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	BlockSize = m_FileParameters.m_BlockSize;
	BlockCount = m_DataBlocksCount;

	LONG BufferLengthInBytes = AtlAlignUp(BlockCount, 8) / 8;
	CHeapPtr<BYTE> apbuffer;
	apbuffer.Allocate(BufferLengthInBytes);
	if (NULL == apbuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("BufferLengthInBytes=%d"), BufferLengthInBytes);
		LEAVE_BLOCK(0);
	}
	ZeroMemory(apbuffer.m_pData, BufferLengthInBytes);
	CSimpleBitArray BitArr(apbuffer.m_pData, BlockCount);

	LONG StartIndex = 0;
	LONG First = 0;
	LONG Last = 0;
	while (StartIndex < BlockCount)
	{
		hr = FindDataBLockRange(StartIndex, First, Last);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("FindDataBLockRange"), hr, TEXT("StartIndex=%d"), StartIndex);
			LEAVE_BLOCK(0);
		}

		BitArr.SetRange(First, Last);

		StartIndex = Last;
	}


	*ppBuffer = apbuffer.Detach();

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParserSeparateVHDX::FreeBlocksBitmapBuffer(BYTE * pBuffer)
{
	CHeapPtr<BYTE> apBuffer;
	apBuffer.Attach(pBuffer);
	apBuffer.Free();
	return S_OK;
}

//<sonmi01>2015-2-15 ###???
ParserSeparateVHDX::EBlockState ParserSeparateVHDX::GetBlocKState( LONGLONG LogicalAddrress )
{
	ParserSeparateVHDX::EBlockState state = ParserSeparateVHDX::EBS_UNKNOWN;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG BlockIndex = LogicalAddrress / m_FileParameters.m_BlockSize;
	LONG BatIndex = BlockIndexToBatIndex(BlockIndex);
	VHDX_BAT_ENTRY BatEntry = {0}; //<sonmi01>2013-9-11 ###???
	HRESULT hr = ReadBatEntry(BatIndex, BatEntry);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadBatEntry"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	
	//////////////////////////////////////////////////////////////////////////
	switch (BatEntry.m_State)
	{
		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_NOT_PRESENT:
		{
			state = ParserSeparateVHDX::EBS_NOT_PRESENT;
		}
		break;

	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNDEFINED:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_ZERO:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_UNMAPPED:
		{
			state = ParserSeparateVHDX::EBS_ALL_ZERO;
		}
		break;

		//////////////////////////////////////////////////////////////////////////
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_FULLY_PRESENT:
	case VHDX_BAT_ENTRY::PAYLOAD_BLOCK_PARTIALLY_PRESENT:
		{
			state =  ParserSeparateVHDX::EBS_ALLOCATED;
		}
		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		{
			hr = E_INVALIDARG;
			D_INFO(0, TEXT("Invalid Data Block BAT entry state %d, BlockIndex=%d, BatIndex=%d"), (INT)BatEntry.m_State, BlockIndex, BatIndex);
		}
		break;
	}

	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return state;
}
