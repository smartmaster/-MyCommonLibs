#include "stdafx.h"
#include "SeparateVhdx.h"


CSeparateVhdx::CSeparateVhdx( LPCTSTR pVhdxFile ) :
m_RefCount(0),
	m_vhdxFileName(pVhdxFile),
	m_CurrentPointer(0),
	//m_pVhdxFile(NULL), 
	m_pParserSeparateVHDX(NULL),
	m_pCFileDeviceForVhdxCacheIO(NULL),
	m_pCCacheIO(NULL),
	m_BitmapBufferSize(0),
	m_LogicalBitmapBufferSize(0),
	m_CahcedBitmapOfPhysicalBlockIndex(-1)
{

}

HRESULT CSeparateVhdx::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDevice;
	ULONG ulFlagsAndAttributesTmp = FILE_FLAG_NO_BUFFERING;
	hr = CreateInstanceFileDeviceWin32(
		m_vhdxFileName.GetString(),//IN LPCTSTR strFileName, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		ulFlagsAndAttributesTmp,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), m_vhdxFileName.GetString());
		LEAVE_BLOCK(0);
	}
	//m_pVhdxFile = spIFileDevice.Detach();


	//////////////////////////////////////////////////////////////////////////
	m_pParserSeparateVHDX = new ParserSeparateVHDX(spIFileDevice.Detach());
	if (NULL == m_pParserSeparateVHDX)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("new ParserSeparateVHDX"), hr, TEXT("[%s]"), m_vhdxFileName.GetString());
		LEAVE_BLOCK(0);
	}

	hr = m_pParserSeparateVHDX->Parse();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Parse"), hr, TEXT("[%s]"), m_vhdxFileName.GetString());
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	m_pCFileDeviceForVhdxCacheIO = new CFileDeviceForVhdxCacheIO(m_pParserSeparateVHDX);
	if (NULL == m_pCFileDeviceForVhdxCacheIO)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("new CFileDeviceForVhdxCacheIO"), hr, TEXT("[%s]"), m_vhdxFileName.GetString());
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	m_pCCacheIO = new CCacheIO(m_pCFileDeviceForVhdxCacheIO, FALSE, m_pParserSeparateVHDX->m_FileParameters.m_BlockSize);
	if (NULL == m_pCCacheIO)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("new CCacheIO"), hr, TEXT("[%s]"), m_vhdxFileName.GetString());
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CSeparateVhdx::DestroyMember()
{
	if (m_pCCacheIO)
	{
		m_pCCacheIO->Flush(); // in fact, not needed as read only
		delete m_pCCacheIO;
		m_pCCacheIO = NULL;
	}

	if (m_pCFileDeviceForVhdxCacheIO)
	{
		delete m_pCFileDeviceForVhdxCacheIO;
		m_pCFileDeviceForVhdxCacheIO = NULL;
	}

	if (m_pParserSeparateVHDX)
	{
		delete m_pParserSeparateVHDX;
		m_pParserSeparateVHDX = NULL;
	}

	//<sonmi01>2013-10-23 ###???
	//if (m_pVhdxFile)
	//{
	//	m_pVhdxFile->Release();
	//	m_pVhdxFile = NULL;
	//}

	return S_OK;
}

HRESULT CSeparateVhdx::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	HRESULT hr = S_OK;

	LONGLONG TempCurrent = 0;
	if (FILE_BEGIN == ulMoveMethod)
	{
		TempCurrent = llDistanceToMove;
	}
	else if (FILE_END == ulMoveMethod)
	{
		TempCurrent = m_pParserSeparateVHDX->m_VirtualDiskSize.m_VirtualDiskSize + llDistanceToMove;
	}
	else if (FILE_CURRENT == ulMoveMethod)
	{
		TempCurrent = m_CurrentPointer + llDistanceToMove;
	}
	else
	{
		hr = E_INVALIDARG;
	}

	if (SUCCEEDED(hr))
	{
		if (TempCurrent >= 0 && TempCurrent < m_pParserSeparateVHDX->m_VirtualDiskSize.m_VirtualDiskSize)
		{
			m_CurrentPointer = TempCurrent;
			*lpNewFilePointer = m_CurrentPointer;
		}
		else
		{
			CONST LONG MY_E_BOUNDS =   _HRESULT_TYPEDEF_(0x8000000BL);
			hr = MY_E_BOUNDS;
			D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("DistanceToMove=%I64d, MoveMethod=%u, --%s--"), llDistanceToMove, ulMoveMethod, TEXT("FileMemMod"));
		}
	}

	return hr;
}

HRESULT CSeparateVhdx::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	HRESULT hr = m_pCCacheIO->ReadAtOffset(m_CurrentPointer, nNumberOfBytesToRead, pBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CacheIO.Read"), hr, TEXT(""));
	}
	else
	{
		if (lpNumberOfBytesRead)
		{
			*lpNumberOfBytesRead = nNumberOfBytesToRead;
		}
		m_CurrentPointer += nNumberOfBytesToRead;
	}
	return hr;
}

HRESULT CSeparateVhdx::GetFileSize( LONGLONG * pFileSize )
{
	*pFileSize = m_pParserSeparateVHDX->m_VirtualDiskSize.m_VirtualDiskSize;
	return S_OK;
}

LONG CSeparateVhdx::GetPhysicalBlockSize()
{
	return m_pParserSeparateVHDX->m_FileParameters.m_BlockSize;
}

LONG CSeparateVhdx::GetLogicalBlockSize()
{
	CONST LONG _1M = 1024 * 1024;
	CONST LONG LOGICAL_BLOCK_SIZE = 2 * _1M;
	if (m_pParserSeparateVHDX->m_FileParameters.m_BlockSize < LOGICAL_BLOCK_SIZE)
	{
		D_API_ERR(0, TEXT("BlockSize"), E_INVALIDARG, TEXT("Block size is [%d] and too small, it is not offically supported and suggest to remove [%s] and try again"), m_pParserSeparateVHDX->m_FileParameters.m_BlockSize, m_vhdxFileName.GetString());
		//<sonmi01>2015-1-30 ###??? //return _1M; //the minimum block size is 1M for VHDX
	}
	return LOGICAL_BLOCK_SIZE;
}

LONG CSeparateVhdx::GetLogicalBlockCount()
{
	return m_pParserSeparateVHDX->m_VirtualDiskSize.m_VirtualDiskSize / GetLogicalBlockSize();
}

HRESULT CSeparateVhdx::ReadLogicalBlock( LONG LogicalBlockIndex, LONG BufferLen, BYTE * pBuffer )
{
	LONGLONG offset = LogicalBlockIndex;
	offset *= GetLogicalBlockSize();

	ULONG NumberOfBytesRead = 0;
	return ReadAtOffsetHelper(this, offset, BufferLen, &NumberOfBytesRead, pBuffer);
}

HRESULT CSeparateVhdx::ReadLogicalBlockBitmap(LONG LogicalBlockIndex, LONG BufferLen, BYTE * pBuffer, BOOL & bContainOne, BOOL & bAllOne) //<sonmi01>2013-10-29 ###???
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG PhysicalBlockIndex = LogicalBlockIndexToPhysical(LogicalBlockIndex);

	if (NULL == m_BitmapBuffer.m_pData)
	{
		m_LogicalBitmapBufferSize = GetLogicalBlockSize() / m_pParserSeparateVHDX->m_LogicalSectorSize.m_LogicalSectorSize / 8;
		m_BitmapBufferSize = m_pParserSeparateVHDX->m_FileParameters.m_BlockSize / m_pParserSeparateVHDX->m_LogicalSectorSize.m_LogicalSectorSize / 8;
		m_BitmapBuffer.Allocate(max(m_LogicalBitmapBufferSize, m_BitmapBufferSize));
	}

	if (PhysicalBlockIndex != m_CahcedBitmapOfPhysicalBlockIndex)
	{
		//<sonmi01>2015-1-30 ###???
		LONG BufferCount = m_LogicalBitmapBufferSize / m_BitmapBufferSize;
		if (BufferCount > 1)
		{
			LONG offsetTemp = 0;
			for (LONG ii = 0; ii < BufferCount; ++ ii)
			{
				hr = m_pParserSeparateVHDX->ReadBlocKBitmap(PhysicalBlockIndex + ii, m_BitmapBufferSize, m_BitmapBuffer.m_pData + offsetTemp);
				if (FAILED(hr))
				{
					D_API_ERR(0, TEXT("ReadBlocKBitmap"), hr, TEXT("PhysicalBlockIndex=[%d]"), PhysicalBlockIndex);
					break;;
				}
				offsetTemp += m_BitmapBufferSize;
			}
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}
		}
		else
		{
			hr = m_pParserSeparateVHDX->ReadBlocKBitmap(PhysicalBlockIndex, m_BitmapBufferSize, m_BitmapBuffer.m_pData);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadBlocKBitmap"), hr, TEXT("PhysicalBlockIndex=[%d]"), PhysicalBlockIndex);
				LEAVE_BLOCK(0);
			}
		}
		
		m_CahcedBitmapOfPhysicalBlockIndex = PhysicalBlockIndex;
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG offset = LogicalBlockIndex; //data index offset
	offset *= GetLogicalBlockSize(); //data bytes offset
	offset %= m_pParserSeparateVHDX->m_FileParameters.m_BlockSize; //data inner bytes offset inside block
	offset /= GetSectorSize(); //bit offset inside bitmap buffer
	offset /= 8; //bytes offset inside bitmap buffer
	CopyMemory(pBuffer, m_BitmapBuffer.m_pData + offset, min(m_LogicalBitmapBufferSize, BufferLen));

	bContainOne = ContainsOne(pBuffer, min(m_LogicalBitmapBufferSize, BufferLen)); //<sonmi01>2013-10-29 ###???
	bAllOne = IsAllOne(pBuffer, min(m_LogicalBitmapBufferSize, BufferLen));

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

LONG CSeparateVhdx::GetSectorSize()
{
	return m_pParserSeparateVHDX->m_LogicalSectorSize.m_LogicalSectorSize;
}

LONG CSeparateVhdx::LogicalBlockIndexToPhysical( LONG LogicalBlockIndex )
{
	LONGLONG offset = LogicalBlockIndex;
	offset *= GetLogicalBlockSize();
	return offset / m_pParserSeparateVHDX->m_FileParameters.m_BlockSize;
}


GUID CSeparateVhdx::GetUniqueID() const
{
	return m_pParserSeparateVHDX->m_pVhdxHeader->m_DataWrite;
}


GUID CSeparateVhdx::GetParentID() const
{
	GUID guid = {0};

	if (m_pParserSeparateVHDX->m_FileParameters.m_HasParent)
	{
		map<CString, CString>::const_iterator citer = m_pParserSeparateVHDX->m_ParentLocatorEntries.find(VHDX_PARENT_LOCATOR_ENTRY_NS::parent_linkage);
		if (m_pParserSeparateVHDX->m_ParentLocatorEntries.end() != citer)
		{
			CMiscHelper::GuidFromString(guid, citer->second);
		}
		else
		{
			D_INFO(0, TEXT("the child disk does not have parent locator entries"));
		}
	}

	return guid;
}

const wstring& CSeparateVhdx::GetParentDiskName() const
{
	if (!m_pParserSeparateVHDX->m_FileParameters.m_HasParent)
	{
		return m_ParentVhdxName;
	}

	if (m_ParentVhdxName.size())
	{
		return m_ParentVhdxName;
	}

	CString strParent;
	FindParentVhdxFile( m_pParserSeparateVHDX, m_vhdxFileName, strParent);
	(const_cast<wstring&>(m_ParentVhdxName)) = strParent.GetString();
	return m_ParentVhdxName;
}

BOOL CSeparateVhdx::FindParentVhdxFile( ParserSeparateVHDX * pVHDX, CONST CString & strChild, CString & strParent )
{
	ATLASSERT(pVHDX && pVHDX->m_FileParameters.m_HasParent);

	map<CString, CString>::const_iterator citer;

	BOOL bFound = FALSE;

	static CONST LPCTSTR search_order[] = 
	{
		VHDX_PARENT_LOCATOR_ENTRY_NS::relative_path,
		VHDX_PARENT_LOCATOR_ENTRY_NS::volume_path,
		VHDX_PARENT_LOCATOR_ENTRY_NS::absolute_win32_path
	};

	for (INT ii = 0; ii < _countof(search_order); ++ ii)
	{
		citer = pVHDX->m_ParentLocatorEntries.find(search_order[ii]);

		if (pVHDX->m_ParentLocatorEntries.end() != citer)
		{
			if (0 == ii)
			{
				CMiscHelper::ConvertRelativeToAbsolutePath(strChild, citer->second, strParent);
			}
			else if (1 == ii)
			{
				strParent = citer->second;
			}
			else if (2 == ii)
			{
				strParent = citer->second;
			}

			if (CMiscHelper::IsFileExists(strParent.GetString()))
			{
				bFound = TRUE;
				break;
			}
		}
	}

	return bFound;
}

//<sonmi01>2013-10-29 ###???
LONGLONG CSeparateVhdx::GetPhysicalAddress(LONGLONG LogicalAddress)
{
	return m_pParserSeparateVHDX->GetPhysicalAddress(LogicalAddress);
}

//<sonmi01>2014-1-9 ###???
HRESULT CSeparateVhdx::DumpBatTable( LPCTSTR FileName ) const
{
	return m_pParserSeparateVHDX->DumpBatTable(FileName);
}

HRESULT CSeparateVhdx::OpenDumpedBatTable( LPCTSTR FileName )
{
	return m_pParserSeparateVHDX->OpenDumpedBatTable(FileName);
}

HRESULT CSeparateVhdx::ReadDumpedBatEntry( LONG LogicalBlockIndex, VHDX_BAT_ENTRY & BatEntry )
{
	LONG PhysicalBlockIndex = LogicalBlockIndexToPhysical(LogicalBlockIndex); //<sonmi01>2014-1-13 ###???
	return m_pParserSeparateVHDX->ReadDumpedBatEntry(PhysicalBlockIndex, BatEntry);
}

HRESULT CSeparateVhdx::CloseDumpedBatTable()
{
	return m_pParserSeparateVHDX->CloseDumpedBatTable();
}

//<sonmi01>2014-1-13 ###???
HRESULT CSeparateVhdx::CompareWithDumpedBatForZeroedData( LONG LogicalBlockIndex, BOOL & bResult )
{
	LONG PhysicalBlockIndex = LogicalBlockIndexToPhysical(LogicalBlockIndex);
	HRESULT hr = m_pParserSeparateVHDX->CompareWithDumpedBatForZeroedData(PhysicalBlockIndex, bResult);
	return hr;
}

//<sonmi01>2014-9-24 ###???
HRESULT CSeparateVhdx::GetBlocksBitmap(LONG & BlockSize, LONG & BlockCount, BYTE ** ppBuffer)
{
	return m_pParserSeparateVHDX->GetBlocksBitmap(BlockSize, BlockCount, ppBuffer);
}

HRESULT CSeparateVhdx::FreeBlocksBitmapBuffer(BYTE * pBuffer)
{
	return m_pParserSeparateVHDX->FreeBlocksBitmapBuffer(pBuffer);
}

//<sonmi01>2015-2-15 ###???
ISeparateVhdx::EBlockState CSeparateVhdx::GetBlockState( LONGLONG logicalAddress )
{
	ParserSeparateVHDX::EBlockState rawState = m_pParserSeparateVHDX->GetBlocKState(logicalAddress);
	ISeparateVhdx::EBlockState state = ISeparateVhdx::EBS_UNKNOWN;

	switch (rawState)
	{
	case ParserSeparateVHDX::EBS_UNKNOWN:
		state = ISeparateVhdx::EBS_UNKNOWN;
		break;
	case ParserSeparateVHDX::EBS_NOT_PRESENT:
		state = ISeparateVhdx::EBS_NOT_PRESENT;
		break;
	case ParserSeparateVHDX::EBS_ALL_ZERO:
		state = ISeparateVhdx::EBS_ALL_ZERO;
		break;
	case ParserSeparateVHDX::EBS_ALLOCATED:
		state = ISeparateVhdx::EBS_ALLOCATED;
		break;
	}
	
	return state;
}

//<sonmi01>2015-6-3 ###???
HRESULT CSeparateVhdx::GetVDiskMiscInfo(VDiskMiscInfo & MiscInfo)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	MiscInfo.m_Page83Data = m_pParserSeparateVHDX->m_VHDX_PAGE83_DATA.m_Page83Data;
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//<sonmi01>2013-10-22 ###???
//API
HRESULT CreateInstanceCSeparateVhdx( LPCTSTR pVhdxFile, ISeparateVhdx ** ppISeparateVhdx, BOOL bCreateMember /*= TRUE*/ )
{
	return CREATE_INSTCANCE_COBJECT(
		new CSeparateVhdx(pVhdxFile), 
		ppISeparateVhdx, 
		bCreateMember
		);
}
