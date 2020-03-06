#include "stdafx.h"

#include "..\PublicHeader\PublicMiscHelper.h"

#include "DiskImageVhdx.h"


BOOL CDiskImageVhdx::FindParentVhdxFile( ParserSeparateVHDX * pVHDX, CONST CString & strChild, CString & strParent )
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

BOOL CDiskImageVhdx::IsChildParent( CONST ParserSeparateVHDX * child, CONST ParserSeparateVHDX * parent )
{
	ATLASSERT(child && child->m_FileParameters.m_HasParent && parent);

	if (child->m_LogicalSectorSize.m_LogicalSectorSize != parent->m_LogicalSectorSize.m_LogicalSectorSize)
	{
		D_INFO(0, TEXT("Sector size does not match [child, parent] = [%d, %d]"), child->m_LogicalSectorSize.m_LogicalSectorSize, parent->m_LogicalSectorSize.m_LogicalSectorSize);
		return FALSE;
	}

	//if (child->m_VirtualDiskSize.m_VirtualDiskSize != parent->m_VirtualDiskSize.m_VirtualDiskSize) //compare virtual disk size?? VHDX spec does not say
	//{
	//	D_INFO(0, TEXT("Virtual disk size does not match [child, parent] = [%I64d, %I64d]"), child->m_VirtualDiskSize.m_VirtualDiskSize, parent->m_VirtualDiskSize.m_VirtualDiskSize);
	//	return FALSE;
	//}

	map<CString, CString>::const_iterator citer = child->m_ParentLocatorEntries.find(VHDX_PARENT_LOCATOR_ENTRY_NS::parent_linkage);
	if (child->m_ParentLocatorEntries.end() == citer)
	{
		D_INFO(0, TEXT("the child disk does not have parent locator entries"));
		return FALSE;
	}

	GUID guid = {0};
	CMiscHelper::GuidFromString(guid, citer->second);
	return IsEqualGUID(guid, parent->m_pVhdxHeader->m_DataWrite);
}


HRESULT CDiskImageVhdx::ChainVHDX()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (GENERIC_READ == m_DesiredAccess)
	{
		hr = ChainExistingVHDXHelper(m_CurrentVhdxFilename, GENERIC_READ);  //read existing
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ChainExistingVHDXHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	else if (GENERIC_WRITE == m_DesiredAccess)
	{
		if (m_parentVhdxFileName.GetLength())
		{
			// create new child for write
			//////////////////////////////////////////////////////////////////////////
			hr = ChainExistingVHDXHelper(m_parentVhdxFileName, GENERIC_READ);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ChainExistingVHDXHelper"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			CComPtr<IFileDevice> spIFileDevice;
			ULONG ulFlagsAndAttributesTmp = FILE_FLAG_NO_BUFFERING;
			hr = CreateInstanceFileDeviceWin32(
				m_CurrentVhdxFilename.GetString(),//IN LPCTSTR strFileName, 
				GENERIC_ALL,//IN ULONG ulDesiredAccess, 
				0,//IN ULONG ulShareMode, 
				CREATE_NEW,//IN ULONG ulCreationDisposition, 
				ulFlagsAndAttributesTmp,//IN ULONG ulFlagsAndAttributes, 
				&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
				TRUE//BOOL bCreateMember
				);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			CAutoPtr<ParserSeparateVHDX> apParserVHDX(new ParserSeparateVHDX(spIFileDevice.Detach()));
			
			ParserSeparateVHDX * Parent = m_ChainVHDXBase.m_Chain[0];

			//////////////////////////////////////////////////////////////////////////
			map<CString, CString> ParentLocatorEntries;
			
			//////////////////////////////////////////////////////////////////////////
			CString strDataWriteGuid;
			CMiscHelper::StringFromGuid(strDataWriteGuid, Parent->m_pVhdxHeader->m_DataWrite);
			ParentLocatorEntries.insert(make_pair(VHDX_PARENT_LOCATOR_ENTRY_NS::parent_linkage, strDataWriteGuid));

			//////////////////////////////////////////////////////////////////////////
			ParentLocatorEntries.insert(make_pair(VHDX_PARENT_LOCATOR_ENTRY_NS::absolute_win32_path, m_parentVhdxFileName));
			
			CString str_volume_path;
			CMiscHelper::AbsoluteWin32PathToVolumepath(m_parentVhdxFileName.GetString(), str_volume_path);
			if (str_volume_path.GetLength())
			{
				ParentLocatorEntries.insert(make_pair(VHDX_PARENT_LOCATOR_ENTRY_NS::volume_path, str_volume_path));
			}

			CString str_relative_path;
			CMiscHelper::GetParentRelativePath(m_parentVhdxFileName, m_CurrentVhdxFilename, str_relative_path);
			if (str_relative_path.GetLength())
			{
				ParentLocatorEntries.insert(make_pair(VHDX_PARENT_LOCATOR_ENTRY_NS::relative_path, str_relative_path));
			}
			
			//////////////////////////////////////////////////////////////////////////
			CONST LONG __1M = 1024 * 1024;
			hr = apParserVHDX->ParseCreateNew(
				Parent->m_VirtualDiskSize.m_VirtualDiskSize, 
				Parent->m_LogicalSectorSize.m_LogicalSectorSize, 
				Parent->m_PhysicalSectorSize.m_PhysicalSectorSize, 
				(m_BlockSize? m_BlockSize : 2 * __1M), 
				ParentLocatorEntries,
				Parent->m_VHDX_PAGE83_DATA.m_Page83Data, //2018-3-23 //!!@@##
				m_vhdxType //<sonmi01>2014-4-21 ###???
				);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("Parse"), hr, TEXT("Invalid VHDX"));
				LEAVE_BLOCK(0);
			}
			
			//////////////////////////////////////////////////////////////////////////
			m_ChainVHDXBase.m_Chain.insert(m_ChainVHDXBase.m_Chain.begin(), apParserVHDX.Detach());
			m_VhdxFiles.insert(m_VhdxFiles.begin(), m_CurrentVhdxFilename);
		}
		else // m_parentVhdxFileName.GetLength() == 0
		{
			if (CMiscHelper::IsFileExists(m_CurrentVhdxFilename.GetString()))
			{
				//open existing for write
				hr = ChainExistingVHDXHelper(m_CurrentVhdxFilename, GENERIC_WRITE);
				if (FAILED(hr))
				{
					D_API_ERR(0, TEXT("ChainExistingVHDXHelper"), hr, TEXT(""));
					LEAVE_BLOCK(0);
				}
			}
			else //File not Exists
			{
				//create new for write
				CONST LONG MIN_DISK_SZIE = 2 * 1024 * 1024;

				LONGLONG __64T = 1;
				__64T <<= 46;

				if (m_VirtualDiskSize <= MIN_DISK_SZIE || m_VirtualDiskSize > __64T)
				{
					hr = E_INVALIDARG;
					D_API_ERR(0, TEXT("Invalid virtual disk size"), hr, TEXT("%I64d"), m_VirtualDiskSize);
					LEAVE_BLOCK(0);
				}


				CComPtr<IFileDevice> spIFileDevice;
				ULONG ulFlagsAndAttributesTmp = FILE_FLAG_NO_BUFFERING;
				hr = CreateInstanceFileDeviceWin32(
					m_CurrentVhdxFilename.GetString(),//IN LPCTSTR strFileName, 
					GENERIC_ALL,//IN ULONG ulDesiredAccess, 
					0,//IN ULONG ulShareMode, 
					CREATE_NEW,//IN ULONG ulCreationDisposition, 
					ulFlagsAndAttributesTmp,//IN ULONG ulFlagsAndAttributes, 
					&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
					TRUE//BOOL bCreateMember
					);
				if (FAILED(hr))
				{
					D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT(""));
					LEAVE_BLOCK(0);
				}

				CAutoPtr<ParserSeparateVHDX> apParserVHDX(new ParserSeparateVHDX(spIFileDevice.Detach()));
				CONST LONG __1M = 1024 * 1024;
				CONST LONG __512_SECTOR = 512;
				CONST LONG __4096_SECTOR = 4096;

				CONST map<CString, CString> ParentLocatorEntries;
				hr = apParserVHDX->ParseCreateNew(
					m_VirtualDiskSize, 
					(m_LogicalSectorSize? m_LogicalSectorSize : __512_SECTOR), 
					(m_PhysicalSectorSize? m_PhysicalSectorSize : __4096_SECTOR), 
					(m_BlockSize? m_BlockSize : 2 * __1M), 
					ParentLocatorEntries,
					GUID_NULL, //2018-3-23 //!!@@##
					m_vhdxType //<sonmi01>2014-4-21 ###???
					);
				if (FAILED(hr))
				{
					D_API_ERR(0, TEXT("Parse"), hr, TEXT("Invalid VHDX"));
					LEAVE_BLOCK(0);
				}

				//////////////////////////////////////////////////////////////////////////
				m_ChainVHDXBase.m_Chain.push_back(apParserVHDX.Detach());
				m_VhdxFiles.push_back(m_CurrentVhdxFilename);
			}
		}
	}
	else
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT(""), hr, TEXT("Invalid access, GENERIC_READ or GENERIC_WRITE is expected"));
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	return hr;
}



HRESULT CDiskImageVhdx::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = ChainVHDX();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ChainVHDX"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = m_ChainVHDXBase.FindMaxBlockSizeAndAllocateBuffer();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ChianVHDX.FindMaxBlockSizeAndAllocateBuffer"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_pFileDeviceForVhdxChainCacheIO = new CFileDeviceForVhdxChainCacheIO(&m_ChainVHDXBase);
	if (NULL == m_pFileDeviceForVhdxChainCacheIO)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("new CFileDeviceForVhdxChainCacheIO"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_pCacheIOChainVHDX = new CCacheIO(m_pFileDeviceForVhdxChainCacheIO, FALSE, m_ChainVHDXBase.m_MaxBlockSize);
	if (NULL == m_pCacheIOChainVHDX)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("new CCacheIO"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CDiskImageVhdx::DestroyMember()
{
	FlushData();
	FlushMetadata();

	if (m_pCacheIOChainVHDX)
	{
		m_pCacheIOChainVHDX->Flush();
		delete m_pCacheIOChainVHDX;
		m_pCacheIOChainVHDX = NULL;
	}

	if (m_pFileDeviceForVhdxChainCacheIO)
	{
		delete m_pFileDeviceForVhdxChainCacheIO;
		m_pFileDeviceForVhdxChainCacheIO = NULL;
	}

	return S_OK;
}

HRESULT CDiskImageVhdx::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	HRESULT hr = S_OK;

	LONGLONG TempCurrent = 0;
	if (FILE_BEGIN == ulMoveMethod)
	{
		TempCurrent = llDistanceToMove;
	}
	else if (FILE_END == ulMoveMethod)
	{
		TempCurrent = m_ChainVHDXBase.m_Chain[0]->m_VirtualDiskSize.m_VirtualDiskSize + llDistanceToMove;
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
		if (TempCurrent >= 0 && TempCurrent < m_ChainVHDXBase.m_Chain[0]->m_VirtualDiskSize.m_VirtualDiskSize)
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

HRESULT CDiskImageVhdx::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	HRESULT hr = m_pCacheIOChainVHDX->ReadAtOffset(m_CurrentPointer, nNumberOfBytesToRead, pBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CacheIO.Read"), hr, TEXT(""));
	}
	else
	{
		if (lpNumberOfBytesRead)
		{
			* lpNumberOfBytesRead = nNumberOfBytesToRead;
		}
		m_CurrentPointer += nNumberOfBytesToRead;
	}
	return hr;
}

HRESULT CDiskImageVhdx::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	HRESULT hr = m_pCacheIOChainVHDX->WriteAtOffset(m_CurrentPointer, nNumberOfBytesToWrite, pBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CacheIO.Write"), hr, TEXT(""));
	}
	else
	{
		if (lpNumberOfBytesWritten)
		{
			* lpNumberOfBytesWritten = nNumberOfBytesToWrite;
		}
		m_CurrentPointer += nNumberOfBytesToWrite;
	}
	return hr;
}

HRESULT CDiskImageVhdx::GetFileSize( LONGLONG * pFileSize ) // disk provision size
{
	HRESULT hr = S_OK;
	if (0 == m_ChainVHDXBase.m_Chain.size())
	{
		hr = HRESULT_FROM_WIN32(ERROR_DEVICE_REINITIALIZATION_NEEDED);
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
	}
	else
	{
		*pFileSize = m_ChainVHDXBase.m_Chain[0]->m_VirtualDiskSize.m_VirtualDiskSize;
	}

	return hr;
}

LONG CDiskImageVhdx::GetBlockSize() //a number of sectors
{
	return m_ChainVHDXBase.m_MaxBlockSize;
}

LONG CDiskImageVhdx::GetSectorSize()
{
	return m_ChainVHDXBase.m_Chain[0]->m_LogicalSectorSize.m_LogicalSectorSize;
}

CDiskImageVhdx::CDiskImageVhdx(CONST CreateInstanceCDiskImageVhdxParams & params ) :
	m_RefCount(0),
	m_parentVhdxFileName(params.parentVhdxFileName),
	m_CurrentVhdxFilename(params.vhdxFileName),
	m_DesiredAccess(params.DesiredAccess),
	m_CreationDisposition(params.CreationDisposition),
	m_VirtualDiskSize(params.VirtualDiskSize),
	m_LogicalSectorSize(params.LogicalSectorSize),
	m_PhysicalSectorSize(params.PhysicalSectorSize),
	m_BlockSize(params.BlockSize),
	m_vhdxType(params.VhdxDiskType), //<sonmi01>2014-4-21 ###???
	m_pFileDeviceForVhdxChainCacheIO(NULL),
	m_pCacheIOChainVHDX(NULL),
	m_CurrentPointer(0)
{

}

LONG CDiskImageVhdx::GetBitmapSizePerBlock()
{
	return m_ChainVHDXBase.m_MaxBlockSize / m_ChainVHDXBase.m_Chain[0]->m_LogicalSectorSize.m_LogicalSectorSize / 8;
}

HRESULT CDiskImageVhdx::GetBlocKBitmap(LONG BlockIndex, IN OUT LONG Len, BYTE * Buffer, BOOL & bContainOne, BOOL & bAllOne) //<sonmi01>2013-10-29 ###???
{
	return m_ChainVHDXBase.ReadBlocKBitmap(BlockIndex, Len, Buffer, bContainOne, bAllOne);
}

HRESULT CDiskImageVhdx::GetBitmap(LONG StarBlock, LONG Count,  IFileDevice * pBitmapFile )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	static CONST LONG __4M = 4 * 1024 * 1024;
	CVMemPtr<BYTE> apBitmapBuffer;
	apBitmapBuffer.Allocate(__4M);
	if (NULL == apBitmapBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("4M size"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG BlockCount = GetBlockCount();
	LONG BitmapSize = GetBitmapSizePerBlock();
	LONG MemSize = 0;
	BOOL bContainOne = FALSE;
	BOOL bAllOne = FALSE;

	//////////////////////////////////////////////////////////////////////////
	if (StarBlock < 0)
	{
		StarBlock = 0;
	}


	//////////////////////////////////////////////////////////////////////////
	LONG UpperBound = 0;
	if (Count < 0)
	{
		UpperBound = BlockCount;
	}
	else
	{
		UpperBound = StarBlock + Count;
		if (UpperBound > BlockCount)
		{
			UpperBound = BlockCount;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	for (LONG ii = StarBlock; ii < UpperBound; ++ ii)
	{
		//////////////////////////////////////////////////////////////////////////
		hr = GetBlocKBitmap(ii, BitmapSize, apBitmapBuffer.m_pData + MemSize, bContainOne, bAllOne); //<sonmi01>2013-10-29 ###???
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadBlocKBitmap"), hr, TEXT(""));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		MemSize += BitmapSize;
		if ((MemSize >= __4M) || (ii >= UpperBound - 1))
		{
			ULONG NumberOfBytesWritten = 0;
			hr = pBitmapFile->Write(MemSize, &NumberOfBytesWritten, apBitmapBuffer.m_pData);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
				break;
			}

			MemSize = 0;
		}
		//////////////////////////////////////////////////////////////////////////
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;

}

LONG CDiskImageVhdx::GetBlockCount()
{
	return m_ChainVHDXBase.m_Chain[m_ChainVHDXBase.m_MaxBlockSizeVhdxIndex]->m_DataBlocksCount;
}

VOID CDiskImageVhdx::SetReadRange( LONG VDiskStartIndex, LONG VDiskCount )
{
	m_ChainVHDXBase.SetReadRange(VDiskStartIndex, VDiskCount);
}

CONST vector<CString> & CDiskImageVhdx::VDiskFileList()
{
	return m_VhdxFiles;
}

HRESULT CDiskImageVhdx::FlushData()
{
	return (m_pCacheIOChainVHDX? m_pCacheIOChainVHDX->Flush() : E_POINTER);
}

HRESULT CDiskImageVhdx::FlushMetadata()
{
	return m_ChainVHDXBase.UpdateBAT();
}

HRESULT CDiskImageVhdx::ChainExistingVHDXHelper( LPCTSTR strTopChild, DWORD DesiredAccess )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	CString strVhdxName = strTopChild;
	for (;;)
	{
		//////////////////////////////////////////////////////////////////////////
		ULONG ulDesiredAccessTmp = GENERIC_READ;
		ULONG ulShareModeTmp = FILE_SHARE_READ;

		if (0 == m_ChainVHDXBase.m_Chain.size())
		{
			if (GENERIC_WRITE == DesiredAccess)
			{
				ulDesiredAccessTmp = GENERIC_ALL;
				ulShareModeTmp = 0;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		CComPtr<IFileDevice> spIFileDevice;
		ULONG ulFlagsAndAttributesTmp = FILE_FLAG_NO_BUFFERING;
		hr = CreateInstanceFileDeviceWin32(
			strVhdxName.GetString(),//IN LPCTSTR strFileName, 
			ulDesiredAccessTmp,//IN ULONG ulDesiredAccess, 
			ulShareModeTmp,//IN ULONG ulShareMode, 
			OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
			ulFlagsAndAttributesTmp,//IN ULONG ulFlagsAndAttributes, 
			&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
			TRUE//BOOL bCreateMember
			);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("%s"), strVhdxName.GetString());
			break;;
		}

		//////////////////////////////////////////////////////////////////////////
		CAutoPtr<ParserSeparateVHDX> apParserVHDX(new ParserSeparateVHDX(spIFileDevice.Detach()));
		hr = apParserVHDX->Parse();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Parse"), hr, TEXT("Invalid VHDX"));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		if (m_ChainVHDXBase.m_Chain.size())
		{
			if (!IsChildParent(m_ChainVHDXBase.m_Chain[m_ChainVHDXBase.m_Chain.size() - 1], apParserVHDX))
			{
				hr = E_INVALIDARG;
				D_API_ERR(0, TEXT("Not a child-parent VKDX chain"), hr, TEXT("Invalid VHDX"));
				break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		m_ChainVHDXBase.m_Chain.push_back(apParserVHDX.Detach());
		m_VhdxFiles.push_back(strVhdxName);

		//////////////////////////////////////////////////////////////////////////
		ParserSeparateVHDX * pSeparateVHDX = m_ChainVHDXBase.m_Chain[m_ChainVHDXBase.m_Chain.size() - 1];
		CString strParentName;
		if (pSeparateVHDX->m_FileParameters.m_HasParent)
		{
			BOOL bFound = FindParentVhdxFile(pSeparateVHDX, strVhdxName, strParentName);
			if (!bFound)
			{
				hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
				D_API_ERR(0, TEXT("Can not find parent VHDX"), hr, TEXT("current=%s"), strVhdxName.GetString());
				break;
			}

			strVhdxName = strParentName;
		}
		else
		{
			break;
		}
		//////////////////////////////////////////////////////////////////////////
	}

	return hr;
}

HANDLE CDiskImageVhdx::GetRawHandle()
{
	return INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////
//API
HRESULT CreateInstanceCDiskImageVhdx(CONST CreateInstanceCDiskImageVhdxParams & params, IDiskImageVirtual ** ppIDiskImageVirtual, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		new CDiskImageVhdx(params), 
		ppIDiskImageVirtual, 
		bCreateMember
		);
}
