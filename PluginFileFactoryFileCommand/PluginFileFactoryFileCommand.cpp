#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include "PluginFileFactoryFileCommand.h"
#include "MyAllSystemInfo.h"


HRESULT CPluginFileFactoryFileCommandDiskVolumeInfo::CreateFileHelper(CONST BODY_FILE_DESCRITION & FileDesc, IN ULONG ulDesiredAccess, IN ULONG ulShareMode, IN ULONG ulCreationDisposition, IN ULONG ulFlagsAndAttributes, OUT IFileDevice ** ppIFileDevice)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strXml;
	HRESULT hrCoInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	{
		CMyAllSystemInfo MyAllSystemInfo(nullptr, nullptr, nullptr, nullptr);
		MyAllSystemInfo.Retrieve();
		MyAllSystemInfo.ToString(FIELD_NAME(AllVolumeDiskInfoXml), strXml);
	}
	if (SUCCEEDED(hrCoInit))
	{
		CoUninitialize();
	}

	
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDeviceMem> spIFileDeviceMem;
	LONG Length = strXml.GetLength() * sizeof(TCHAR);
	hr = CreateInstanceCFileDeviceMem(nullptr, Length, &spIFileDeviceMem, TRUE);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceMem"), hr, TEXT("Length=%d"), Length);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	ULONG NumberOfBytesWritten = 0;
	hr = WriteAtOffsetHelper(spIFileDeviceMem, 0, Length, &NumberOfBytesWritten, (CONST BYTE*)strXml.GetString());
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	*ppIFileDevice = spIFileDeviceMem.Detach();
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT CPluginFileFactoryFileCommandDiskVolumeInfo::IsBitmapAvailable(IN IFileDevice * pIFileDevice, OUT BOOL & bAvailable)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
#if 1
	bAvailable = FALSE;
#else
	//////////////////////////////////////////////////////////////////////////
	if (nullptr == pIFileDevice)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(E_INVALIDARG);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("nullptr"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	bAvailable = TRUE;
#endif
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CPluginFileFactoryFileCommandDiskVolumeInfo::BlockSize(IN IFileDevice * pIFileDevice, OUT LONG & BlockSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
#if 0
	BlockSize = 0;
#else

	//////////////////////////////////////////////////////////////////////////
	if (nullptr == pIFileDevice)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(E_INVALIDARG);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("nullptr"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	BlockSize = 1; //one byte per block
	//////////////////////////////////////////////////////////////////////////
#endif
	END_BLOCK(0);

	return hr;
}

HRESULT CPluginFileFactoryFileCommandDiskVolumeInfo::BlockCount(IN IFileDevice * pIFileDevice, OUT LONGLONG & BlockCount)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
#if 0
	BlockCount = 0;
#else
	//////////////////////////////////////////////////////////////////////////
	if (nullptr == pIFileDevice)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(E_INVALIDARG);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("nullptr"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = pIFileDevice->GetFileSize(&BlockCount); //one byte per block, then blockCount is equal to fileSize
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
#endif
	
	END_BLOCK(0);

	return hr;
}

HRESULT CPluginFileFactoryFileCommandDiskVolumeInfo::GetBlocksBitmap(IN IFileDevice * pIFileDevice, LONGLONG StarBlock, LONGLONG Count, IFileDevice * pBitmapFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
#if 0
	return hr;
#else
	//////////////////////////////////////////////////////////////////////////
	if (nullptr == pIFileDevice)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(E_INVALIDARG);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("nullptr"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	LONGLONG size_bytes = AtlAlignUp(Count, 8) / 8;
	CVMemPtr<BYTE> buffer;
	buffer.Allocate(size_bytes);
	if (nullptr == buffer.m_pData)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = E_OUTOFMEMORY;
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("size_bytes=%I64d"), size_bytes);
		LEAVE_BLOCK(0);
	}

	CSimpleBitArray bitarray(buffer.m_pData, Count);
	bitarray.SetRange(0, Count);
	ULONG NumberOfBytesWritten = 0;
	hr = pBitmapFile->Write(size_bytes, &NumberOfBytesWritten, buffer.m_pData);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
#endif
	END_BLOCK(0);

	return hr;
}



HRESULT CreateInstanceIPluginFileFactory(IPluginFileFactory ** pp)
{
	return CREATE_INSTCANCE_COBJECT(new CPluginFileFactoryFileCommandDiskVolumeInfo(), pp, TRUE);
}