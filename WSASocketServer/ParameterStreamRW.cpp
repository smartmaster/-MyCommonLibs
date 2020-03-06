
#include "stdafx.h"
#include "ParameterStreamRW.h"



HRESULT SkipSubStruct(IFileDevice * pFileDevice)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	CNameValueStream nvs(pFileDevice);

	//////////////////////////////////////////////////////////////////////////
	CNameValueStream::DATA_BYTE_ORDER byteOrder;
	hr = nvs.GetByteOrder(byteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	wstring Name;
	LONG ValueSize = 0;
	for (;;)
	{
		hr = nvs.ReadNameValueSize(Name, ValueSize);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadNameValueSize"), hr, TEXT(""));
			break;
		}


		if (Name.size() && ValueSize > 0)
		{
			LONGLONG NewPos = -1;
			hr = pFileDevice->SetPointerEx(ValueSize, &NewPos, FILE_CURRENT);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (Name.size() && -1 == ValueSize)
		{
			hr = SkipSubStruct(pFileDevice); //Recursively skip sub structure of structure
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("SkipSubStructRecursively"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (CNameValueStream::IsEnding(Name, ValueSize))
		{
			D_INFO(0, TEXT("Stream ending hit"));
			break;
		}
		else
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT("Unexpected stream format"), hr, TEXT("%s"), Name.c_str());
			break;
		}
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


template<>
HRESULT ReadVector( vector<CString> & vec, CONST wstring & vecName, LONG ElementCount, CNameValueStream & nvs)
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
		hr = nvs.ReadData(Length);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		szTempBuffer[0] = 0;
		hr = nvs.ReadData(szTempBuffer, Length);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		vec[ii] = szTempBuffer;
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
HRESULT WriteVector( CONST vector<CString> & vec, LPCTSTR pFieldName, CNameValueStream & nvs )
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
		hr = nvs.WriteData((LONG)(vec[ii].GetLength() + 1));
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = nvs.WriteData(vec[ii].GetString(), vec[ii].GetLength() + 1);
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

//////////////////////////////////////////////////////////////////////////
template<>
HRESULT ReadVector( vector<wstring> & vec, CONST wstring & vecName, LONG ElementCount, CNameValueStream & nvs)
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
		hr = nvs.ReadData(Length);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		szTempBuffer[0] = 0;
		hr = nvs.ReadData(szTempBuffer, Length);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		vec[ii] = szTempBuffer;
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
HRESULT WriteVector( CONST vector<wstring> & vec, LPCTSTR pFieldName, CNameValueStream & nvs )
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
		hr = nvs.WriteData((LONG)(vec[ii].size() + 1));
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteData"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = nvs.WriteData(vec[ii].c_str(), vec[ii].size() + 1);
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

