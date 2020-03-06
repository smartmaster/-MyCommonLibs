#include "stdafx.h"
#include "SocketFileDeviceHandlerDefines.h"

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

		if (CNameValueStream::IsEnding(Name, ValueSize))
		{
			D_INFO(0, TEXT("Stream ending hit"));
			break;
		}
		else if (Name.size() && ValueSize > 0)
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

HRESULT ParameterCreateInstanceFileDeviceWin32T::WriteTo( IFileDevice * pFileDevice )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CNameValueStream nvs(pFileDevice);

	CNameValueStream::DATA_BYTE_ORDER ByteOrder = CNameValueStream::LITTLE_ENDIAN;
	hr = nvs.SetByteOrder(ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(CreateInstanceFileDeviceWin32T), _countof(MethodString(CreateInstanceFileDeviceWin32T)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}

	WRITE_ARRAY_FIELD(hr, nvs, strFileName, strFileName.GetString(), strFileName.GetLength() + 1);
	WRITE_SINGLE_FIELD(hr, nvs, ulDesiredAccess);
	WRITE_SINGLE_FIELD(hr, nvs, ulShareMode);
	WRITE_SINGLE_FIELD(hr, nvs, ulCreationDisposition);
	WRITE_SINGLE_FIELD(hr, nvs, ulFlagsAndAttributes);
	WRITE_SINGLE_FIELD(hr, nvs, bCreateMember);

	//////////////////////////////////////////////////////////////////////////
	hr = nvs.WriteEnding();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteEnding"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


HRESULT ParameterCreateInstanceFileDeviceWin32T::ReadFrom( IFileDevice * pFileDevice )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CNameValueStream nvs(pFileDevice);

	CNameValueStream::DATA_BYTE_ORDER ByteOrder;
	hr = nvs.GetByteOrder(ByteOrder);
	ATLASSERT(CNameValueStream::LITTLE_ENDIAN == ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG ValueSize = 0;
	wstring Name;
	TCHAR szTempBuffer[256] = {0};
	for (;;)
	{
		hr = nvs.ReadNameValueSize(Name, ValueSize);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("GetByteOrder"), hr, TEXT(""));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		if (IS_STRING_EQUAL(METHOD_META_NAME, Name))
		{
			hr = nvs.ReadValueData(szTempBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			D_INFO(0, TEXT("Method Name=%s"), szTempBuffer);
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(CreateInstanceFileDeviceWin32T)));
		}
		//////////////////////////////////////////////////////////////////////////
		else if (IS_FIELD_NAME(strFileName, Name))
		{
			LPTSTR pBuffer = strFileName.GetBuffer(ValueSize);
			hr = nvs.ReadValueData(pBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			strFileName.ReleaseBuffer();
		}
		ELSE_READ_SINGLE_FILED(hr, nvs, ulDesiredAccess, Name, ValueSize)
		ELSE_READ_SINGLE_FILED(hr, nvs, ulShareMode, Name, ValueSize)
		ELSE_READ_SINGLE_FILED(hr, nvs, ulCreationDisposition, Name, ValueSize)
		ELSE_READ_SINGLE_FILED(hr, nvs, ulFlagsAndAttributes, Name, ValueSize)
		ELSE_READ_SINGLE_FILED(hr, nvs, bCreateMember, Name, ValueSize)
		else if (CNameValueStream::IsEnding(Name, ValueSize))
		{
			D_INFO(0, TEXT("Stream END"));
			break;
		}
		else if (Name.size() && ValueSize > 0)
		{
			D_INFO(0, TEXT("Unrecognized field name = %s, ValueSize = %u, skip it"), Name.c_str(), ValueSize);
			LONGLONG NewPos = -1;
			hr = pFileDevice->SetPointerEx(ValueSize, &NewPos, FILE_CURRENT);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		else if (Name.size() && -1 == ValueSize)
		{
			D_INFO(0, TEXT("Unrecognized structure name = %s, ValueSize = %u, skip it"), Name.c_str(), ValueSize);
			hr = SkipSubStruct(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("SkipSubStruct"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT(""), E_FAIL, TEXT("Unexpected field name = %s, ValueSize = %u"), Name.c_str(), ValueSize);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
	}
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadDataValue"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

HRESULT ParameterCreateInstanceFileDeviceWin32T::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength )
{
	WRITE_TO_BUFFER(pBuffer, Length, WrittenLength);
}


HRESULT ParameterCreateInstanceFileDeviceWin32T::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	READ_FROM_BUFFER(pBuffer, Length);
}

CONST LPCTSTR ParameterCreateInstanceFileDeviceWin32T::RootName = MethodString(CreateInstanceFileDeviceWin32T);


HRESULT ParameterSetPointerEx::WriteTo( IFileDevice * pFileDevice )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CNameValueStream nvs(pFileDevice);

	CNameValueStream::DATA_BYTE_ORDER ByteOrder = CNameValueStream::LITTLE_ENDIAN;
	hr = nvs.SetByteOrder(ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(SetPointerEx), _countof(MethodString(SetPointerEx)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}

	WRITE_SINGLE_FIELD(hr, nvs, llDistanceToMove);
	WRITE_SINGLE_FIELD(hr, nvs, ulMoveMethod);

	//////////////////////////////////////////////////////////////////////////
	hr = nvs.WriteEnding();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteEnding"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT ParameterSetPointerEx::ReadFrom( IFileDevice * pFileDevice )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CNameValueStream nvs(pFileDevice);

	CNameValueStream::DATA_BYTE_ORDER ByteOrder;
	hr = nvs.GetByteOrder(ByteOrder);
	ATLASSERT(CNameValueStream::LITTLE_ENDIAN == ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG ValueSize = 0;
	wstring Name;
	TCHAR szTempBuffer[256] = {0};
	for (;;)
	{
		hr = nvs.ReadNameValueSize(Name, ValueSize);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("GetByteOrder"), hr, TEXT(""));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		if (IS_STRING_EQUAL(METHOD_META_NAME, Name))
		{
			hr = nvs.ReadValueData(szTempBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			D_INFO(0, TEXT("Method Name=%s"), szTempBuffer);
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(SetPointerEx)));
		}
		//////////////////////////////////////////////////////////////////////////
		ELSE_READ_SINGLE_FILED(hr, nvs, llDistanceToMove, Name, ValueSize)
		ELSE_READ_SINGLE_FILED(hr, nvs, ulMoveMethod, Name, ValueSize)
		//////////////////////////////////////////////////////////////////////////
		else if (CNameValueStream::IsEnding(Name, ValueSize))
		{
			D_INFO(0, TEXT("Stream END"));
			break;
		}
		else if (Name.size() && ValueSize > 0)
		{
			D_INFO(0, TEXT("Unrecognized field name = %s, ValueSize = %u, skip it"), Name.c_str(), ValueSize);
			LONGLONG NewPos = -1;
			hr = pFileDevice->SetPointerEx(ValueSize, &NewPos, FILE_CURRENT);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		else if (Name.size() && -1 == ValueSize)
		{
			D_INFO(0, TEXT("Unrecognized structure name = %s, ValueSize = %u, skip it"), Name.c_str(), ValueSize);
			hr = SkipSubStruct(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("SkipSubStruct"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT(""), E_FAIL, TEXT("Unexpected field name = %s, ValueSize = %u"), Name.c_str(), ValueSize);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
	}
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadDataValue"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

HRESULT ParameterSetPointerEx::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength )
{
	WRITE_TO_BUFFER(pBuffer, Length, WrittenLength);
}

HRESULT ParameterSetPointerEx::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	READ_FROM_BUFFER(pBuffer, Length);
}

CONST LPCTSTR ParameterSetPointerEx::RootName = MethodString(SetPointerEx);
