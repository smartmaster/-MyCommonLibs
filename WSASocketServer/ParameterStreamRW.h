#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "SerializeFieldDefine.h"
#include "NameValueStream.h"
#include "WSADataBuffer.h"



#include "ParameterStreamDef.h"


//////////////////////////////////////////////////////////////////////////
HRESULT SkipSubStruct(IFileDevice * pFileDevice);


template<class T>
HRESULT READ_FROM_BUFFER( LPBYTE pBuffer, LONGLONG Length, T * This) 
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CComPtr<IFileDeviceMem> spIFileDeviceMem;
	hr = CreateInstanceCFileDeviceMem(pBuffer, Length, &spIFileDeviceMem, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceMem"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = This->ReadFrom(spIFileDeviceMem);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

template<class T>
HRESULT WRITE_TO_BUFFER(  LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength, T * This )
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CComPtr<IFileDeviceMem> spIFileDeviceMem;
	hr = CreateInstanceCFileDeviceMem(pBuffer, Length, &spIFileDeviceMem, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceMem"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = This->WriteTo(spIFileDeviceMem);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	spIFileDeviceMem->GetFileSize(&WrittenLength);
	END_BLOCK(0);
	return hr;
}

template<class T>
HRESULT READ_FROM_FILE(LPCTSTR pFileName, T * This)
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr =  CreateInstanceFileDeviceWin32(
		pFileName,//IN LPCTSTR strFileName, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = This->ReadFrom(spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

template <class T>
HRESULT WRITE_TO_FILE(LPCTSTR pFileName, LONGLONG & WrittenLength, T * This)
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(
		pFileName,//IN LPCTSTR strFileName, 
		GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		0,//IN ULONG ulFlagsAndAttributes, 
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceMem"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = This->WriteTo(spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	spIFileDevice->GetFileSize(&WrittenLength);
	END_BLOCK(0);
	return hr;
}


//////////////////////////////////////////////////////////////////////////
//// name element_count single_element_value ... single_element_value
//////////////////////////////////////////////////////////////////////////
template<typename T>
HRESULT WriteVector(CONST vector<T> & vec, LPCTSTR pFieldName, CNameValueStream & nvs)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	//CNameValueStream nvs(pFileDevice);
	hr = nvs.WriteNameValueSize(pFieldName, vec.size());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValueLength"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	for (size_t ii = 0; ii < vec.size(); ++ ii)
	{
		hr = nvs.WriteData(vec[ii]);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteElementNameValueLength"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

template<>
HRESULT WriteVector(CONST vector<CString> & vec, LPCTSTR pFieldName, CNameValueStream & nvs);

template<>
HRESULT WriteVector(CONST vector<wstring> & vec, LPCTSTR pFieldName, CNameValueStream & nvs);

template<typename T>
HRESULT ReadVector(vector<T> & vec, CONST wstring & vecName, LONG ElementCount, CNameValueStream & nvs)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	//CNameValueStream nvs(pFileDevice);
	vec.resize(ElementCount);

	TCHAR szTempBuffer[1024] = {0};
	LONG Length = 0;
	for (size_t ii = 0; ii < vec.size(); ++ ii)
	{
		hr = nvs.ReadData(vec[ii]);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadElement"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

template<>
HRESULT ReadVector(vector<CString> & vec, CONST wstring & vecName, LONG ElementCount, CNameValueStream & nvs);

template<>
HRESULT ReadVector(vector<wstring> & vec, CONST wstring & vecName, LONG ElementCount, CNameValueStream & nvs);


template<typename T>
HRESULT WriteComplexVector(CONST vector<T> & vec, LPCTSTR pFieldName, IFileDevice * pFileDevice)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	CNameValueStream nvs(pFileDevice);
	hr = nvs.WriteNameValueSize(pFieldName, vec.size());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValueLength"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	for (size_t ii = 0; ii < vec.size(); ++ ii)
	{
		T & temp = const_cast<T&>(vec[ii]);
		hr = temp.WriteTo(pFileDevice);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteElementNameValueLength"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

template<typename T> HRESULT ReadComplexVector(vector<T> & vec, CONST wstring & vecName, LONG ElementCount, IFileDevice * pFileDevice)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	CNameValueStream nvs(pFileDevice);
	vec.resize(ElementCount);

	for (size_t ii = 0; ii < vec.size(); ++ ii)
	{
		hr = vec[ii].ReadFrom(pFileDevice);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadElement"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}


