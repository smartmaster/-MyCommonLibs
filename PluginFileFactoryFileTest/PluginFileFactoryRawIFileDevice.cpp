#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicPluginFileFactoryConfig.h"
#include "PluginFileFactoryRawIFileDevice.h"


HRESULT CPluginFileFactoryRawIFileDevice::CreateFileHelper(CONST BODY_FILE_DESCRITION & FileDesc, IN ULONG ulDesiredAccess, IN ULONG ulShareMode, IN ULONG ulCreationDisposition, IN ULONG ulFlagsAndAttributes, OUT IFileDevice ** ppIFileDevice)
{
	CreateInstanceRawIFileDeviceXml RawIFileDeviceXml(nullptr);
	RawIFileDeviceXml.FromString(FIELD_NAME(CreateInstanceRawIFileDeviceXml), FileDesc.m_MoreContext);

	IFileDevice * p = (IFileDevice*)(RawIFileDeviceXml.FileDevicePtr);
	ATLASSERT(nullptr != p);
	if (nullptr != p)
	{
		p->AddRef();
		*ppIFileDevice = p;
	}
	return p ? S_OK : E_INVALIDARG;
}

HRESULT CPluginFileFactoryRawIFileDevice::IsBitmapAvailable(IN IFileDevice * pIFileDevice, OUT BOOL & bAvailable)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
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

	//bAvailable = TRUE;
	bAvailable = FALSE;

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CPluginFileFactoryRawIFileDevice::BlockSize(IN IFileDevice * pIFileDevice, OUT LONG & BlockSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
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
	END_BLOCK(0);

	return hr;
}

HRESULT CPluginFileFactoryRawIFileDevice::BlockCount(IN IFileDevice * pIFileDevice, OUT LONGLONG & BlockCount)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
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
	END_BLOCK(0);

	return hr;
}

HRESULT CPluginFileFactoryRawIFileDevice::GetBlocksBitmap(IN IFileDevice * pIFileDevice, LONGLONG StarBlock, LONGLONG Count, IFileDevice * pBitmapFile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
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
	END_BLOCK(0);

	return hr;
}



HRESULT CreateInstanceIPluginFileFactoryRawIFileDevice(IPluginFileFactory ** pp)
{
	return CREATE_INSTCANCE_COBJECT(new CPluginFileFactoryRawIFileDevice(), pp, TRUE);
}