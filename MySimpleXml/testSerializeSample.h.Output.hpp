

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT tsimple::WriteTo( IFileDevice * pFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CNameValueStream nvs(pFileDevice);
	CNameValueStream::DATA_BYTE_ORDER ByteOrder = CNameValueStream::LITTLE_ENDIAN;
	hr = nvs.SetByteOrder(ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(tsimple), _countof(MethodString(tsimple)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(bool_Value),bool_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(bool_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(bool_Value1),bool_Value1);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(bool_Value1));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(CString_Value),CString_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(CString_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(wstring_Value),wstring_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(wstring_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(LONGLONG_Value),LONGLONG_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(LONGLONG_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ULONGLONG_Value),ULONGLONG_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ULONGLONG_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(LONG_Value),LONG_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(LONG_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ULONG_Value),ULONG_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ULONG_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(CHAR_Value),CHAR_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(CHAR_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(UCHAR_Value),UCHAR_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(UCHAR_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(SHORT_Value),SHORT_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(SHORT_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(USHORT_Value),USHORT_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(USHORT_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(INT_Value),INT_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(INT_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(UINT_Value),UINT_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(UINT_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(vector_BYTE_Value),vector_BYTE_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(vector_BYTE_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(GUID_Value),GUID_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(GUID_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(string_Value),string_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(string_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(CStringA_Value),CStringA_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(CStringA_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(SYSTEMTIME_Value),SYSTEMTIME_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(SYSTEMTIME_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DOUBLE_Value),DOUBLE_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DOUBLE_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(FLOAT_Value),FLOAT_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(FLOAT_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BYTE_Value),BYTE_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BYTE_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_bool_Value, FIELD_NAME(vector_bool_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_bool_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_bool_Value1, FIELD_NAME(vector_bool_Value1), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_bool_Value1));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_CString_Value, FIELD_NAME(vector_CString_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_CString_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_wstring_Value, FIELD_NAME(vector_wstring_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_wstring_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_LONGLONG_Value, FIELD_NAME(vector_LONGLONG_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_LONGLONG_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_ULONGLONG_Value, FIELD_NAME(vector_ULONGLONG_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_ULONGLONG_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_LONG_Value, FIELD_NAME(vector_LONG_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_LONG_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_ULONG_Value, FIELD_NAME(vector_ULONG_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_ULONG_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_CHAR_Value, FIELD_NAME(vector_CHAR_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_CHAR_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_UCHAR_Value, FIELD_NAME(vector_UCHAR_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_UCHAR_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_SHORT_Value, FIELD_NAME(vector_SHORT_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_SHORT_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_USHORT_Value, FIELD_NAME(vector_USHORT_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_USHORT_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_INT_Value, FIELD_NAME(vector_INT_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_INT_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_UINT_Value, FIELD_NAME(vector_UINT_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_UINT_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_vector_BYTE_Value, FIELD_NAME(vector_vector_BYTE_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_vector_BYTE_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_GUID_Value, FIELD_NAME(vector_GUID_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_GUID_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_string_Value, FIELD_NAME(vector_string_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_string_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_CStringA_Value, FIELD_NAME(vector_CStringA_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_CStringA_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_SYSTEMTIME_Value, FIELD_NAME(vector_SYSTEMTIME_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_SYSTEMTIME_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_DOUBLE_Value, FIELD_NAME(vector_DOUBLE_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_DOUBLE_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_FLOAT_Value, FIELD_NAME(vector_FLOAT_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_FLOAT_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_basic_BYTE_Value, FIELD_NAME(vector_basic_BYTE_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_basic_BYTE_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(FILETIME_Value),FILETIME_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(FILETIME_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_FILETIME_Value, FIELD_NAME(vector_FILETIME_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_FILETIME_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteEnding();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteEnding"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT tsimple::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT tsimple::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT tcomplex::WriteTo( IFileDevice * pFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CNameValueStream nvs(pFileDevice);
	CNameValueStream::DATA_BYTE_ORDER ByteOrder = CNameValueStream::LITTLE_ENDIAN;
	hr = nvs.SetByteOrder(ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(tcomplex), _countof(MethodString(tcomplex)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(tsimple_Value),tsimple_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(tsimple_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_tsimple_Value, FIELD_NAME(vector_tsimple_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_tsimple_Value));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteEnding();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteEnding"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT tcomplex::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT tcomplex::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT tcomplex2::WriteTo( IFileDevice * pFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CNameValueStream nvs(pFileDevice);
	CNameValueStream::DATA_BYTE_ORDER ByteOrder = CNameValueStream::LITTLE_ENDIAN;
	hr = nvs.SetByteOrder(ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(tcomplex2), _countof(MethodString(tcomplex2)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(tcomplex_Value),tcomplex_Value);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(tcomplex_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vector_tcomplex_Value, FIELD_NAME(vector_tcomplex_Value), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_tcomplex_Value));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(v3_tcomplex, FIELD_NAME(v3_tcomplex), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(v3_tcomplex));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(l3_tcomplex, FIELD_NAME(l3_tcomplex), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(l3_tcomplex));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(q3_tcomplex, FIELD_NAME(q3_tcomplex), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(q3_tcomplex));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(q4_tcomplex, FIELD_NAME(q4_tcomplex), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(q4_tcomplex));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteEnding();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteEnding"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT tcomplex2::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT tcomplex2::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT tsimple::ReadFrom( IFileDevice * pFileDevice )
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CNameValueStream nvs(pFileDevice);
	CNameValueStream::DATA_BYTE_ORDER ByteOrder;
	hr = nvs.GetByteOrder(ByteOrder);
	ATLASSERT(CNameValueStream::LITTLE_ENDIAN == ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	LONG ValueSize = 0;
	wstring Name;
	TCHAR szTempBuffer[256] = {0};
	for (;;)
	{
		hr = nvs.ReadNameValueSize(Name, ValueSize);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadNameValueSize"), hr, TEXT(""));
			break;
		}
		if (IS_STRING_EQUAL(METHOD_META_NAME, Name))
		{
			hr = nvs.ReadValueData(szTempBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			D_INFO(99, TEXT("Method Name=%s"), szTempBuffer);
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(tsimple)));
		}
		else if (IS_FIELD_NAME(bool_Value, Name))
		{
			ATLASSERT(sizeof(bool_Value) == ValueSize);
			hr = nvs.ReadValueData(bool_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(bool_Value1, Name))
		{
			ATLASSERT(sizeof(bool_Value1) == ValueSize);
			hr = nvs.ReadValueData(bool_Value1);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(CString_Value, Name))
		{
			ATLASSERT(sizeof(CString_Value) == ValueSize);
			hr = nvs.ReadValueData(CString_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(wstring_Value, Name))
		{
			ATLASSERT(sizeof(wstring_Value) == ValueSize);
			hr = nvs.ReadValueData(wstring_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(LONGLONG_Value, Name))
		{
			ATLASSERT(sizeof(LONGLONG_Value) == ValueSize);
			hr = nvs.ReadValueData(LONGLONG_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ULONGLONG_Value, Name))
		{
			ATLASSERT(sizeof(ULONGLONG_Value) == ValueSize);
			hr = nvs.ReadValueData(ULONGLONG_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(LONG_Value, Name))
		{
			ATLASSERT(sizeof(LONG_Value) == ValueSize);
			hr = nvs.ReadValueData(LONG_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ULONG_Value, Name))
		{
			ATLASSERT(sizeof(ULONG_Value) == ValueSize);
			hr = nvs.ReadValueData(ULONG_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(CHAR_Value, Name))
		{
			ATLASSERT(sizeof(CHAR_Value) == ValueSize);
			hr = nvs.ReadValueData(CHAR_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(UCHAR_Value, Name))
		{
			ATLASSERT(sizeof(UCHAR_Value) == ValueSize);
			hr = nvs.ReadValueData(UCHAR_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(SHORT_Value, Name))
		{
			ATLASSERT(sizeof(SHORT_Value) == ValueSize);
			hr = nvs.ReadValueData(SHORT_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(USHORT_Value, Name))
		{
			ATLASSERT(sizeof(USHORT_Value) == ValueSize);
			hr = nvs.ReadValueData(USHORT_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(INT_Value, Name))
		{
			ATLASSERT(sizeof(INT_Value) == ValueSize);
			hr = nvs.ReadValueData(INT_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(UINT_Value, Name))
		{
			ATLASSERT(sizeof(UINT_Value) == ValueSize);
			hr = nvs.ReadValueData(UINT_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(vector_BYTE_Value, Name))
		{
			ATLASSERT(sizeof(vector_BYTE_Value) == ValueSize);
			hr = nvs.ReadValueData(vector_BYTE_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(GUID_Value, Name))
		{
			ATLASSERT(sizeof(GUID_Value) == ValueSize);
			hr = nvs.ReadValueData(GUID_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(string_Value, Name))
		{
			ATLASSERT(sizeof(string_Value) == ValueSize);
			hr = nvs.ReadValueData(string_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(CStringA_Value, Name))
		{
			ATLASSERT(sizeof(CStringA_Value) == ValueSize);
			hr = nvs.ReadValueData(CStringA_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(SYSTEMTIME_Value, Name))
		{
			ATLASSERT(sizeof(SYSTEMTIME_Value) == ValueSize);
			hr = nvs.ReadValueData(SYSTEMTIME_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DOUBLE_Value, Name))
		{
			ATLASSERT(sizeof(DOUBLE_Value) == ValueSize);
			hr = nvs.ReadValueData(DOUBLE_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(FLOAT_Value, Name))
		{
			ATLASSERT(sizeof(FLOAT_Value) == ValueSize);
			hr = nvs.ReadValueData(FLOAT_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(BYTE_Value, Name))
		{
			ATLASSERT(sizeof(BYTE_Value) == ValueSize);
			hr = nvs.ReadValueData(BYTE_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(vector_bool_Value, Name))
		{
			hr = ReadVector(vector_bool_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_bool_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_bool_Value1, Name))
		{
			hr = ReadVector(vector_bool_Value1, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_bool_Value1));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_CString_Value, Name))
		{
			hr = ReadVector(vector_CString_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_CString_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_wstring_Value, Name))
		{
			hr = ReadVector(vector_wstring_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_wstring_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_LONGLONG_Value, Name))
		{
			hr = ReadVector(vector_LONGLONG_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_LONGLONG_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_ULONGLONG_Value, Name))
		{
			hr = ReadVector(vector_ULONGLONG_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_ULONGLONG_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_LONG_Value, Name))
		{
			hr = ReadVector(vector_LONG_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_LONG_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_ULONG_Value, Name))
		{
			hr = ReadVector(vector_ULONG_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_ULONG_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_CHAR_Value, Name))
		{
			hr = ReadVector(vector_CHAR_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_CHAR_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_UCHAR_Value, Name))
		{
			hr = ReadVector(vector_UCHAR_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_UCHAR_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_SHORT_Value, Name))
		{
			hr = ReadVector(vector_SHORT_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_SHORT_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_USHORT_Value, Name))
		{
			hr = ReadVector(vector_USHORT_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_USHORT_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_INT_Value, Name))
		{
			hr = ReadVector(vector_INT_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_INT_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_UINT_Value, Name))
		{
			hr = ReadVector(vector_UINT_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_UINT_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_vector_BYTE_Value, Name))
		{
			hr = ReadVector(vector_vector_BYTE_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_vector_BYTE_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_GUID_Value, Name))
		{
			hr = ReadVector(vector_GUID_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_GUID_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_string_Value, Name))
		{
			hr = ReadVector(vector_string_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_string_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_CStringA_Value, Name))
		{
			hr = ReadVector(vector_CStringA_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_CStringA_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_SYSTEMTIME_Value, Name))
		{
			hr = ReadVector(vector_SYSTEMTIME_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_SYSTEMTIME_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_DOUBLE_Value, Name))
		{
			hr = ReadVector(vector_DOUBLE_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_DOUBLE_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_FLOAT_Value, Name))
		{
			hr = ReadVector(vector_FLOAT_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_FLOAT_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vector_basic_BYTE_Value, Name))
		{
			hr = ReadVector(vector_basic_BYTE_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_basic_BYTE_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(FILETIME_Value, Name))
		{
			ATLASSERT(sizeof(FILETIME_Value) == ValueSize);
			hr = nvs.ReadValueData(FILETIME_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(vector_FILETIME_Value, Name))
		{
			hr = ReadVector(vector_FILETIME_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_FILETIME_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (CNameValueStream::IsEnding(Name, ValueSize))
		{
			D_INFO(99, TEXT("Stream END"));
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
	}
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadDataValue"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT tsimple::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT tsimple::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT tcomplex::ReadFrom( IFileDevice * pFileDevice )
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CNameValueStream nvs(pFileDevice);
	CNameValueStream::DATA_BYTE_ORDER ByteOrder;
	hr = nvs.GetByteOrder(ByteOrder);
	ATLASSERT(CNameValueStream::LITTLE_ENDIAN == ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	LONG ValueSize = 0;
	wstring Name;
	TCHAR szTempBuffer[256] = {0};
	for (;;)
	{
		hr = nvs.ReadNameValueSize(Name, ValueSize);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadNameValueSize"), hr, TEXT(""));
			break;
		}
		if (IS_STRING_EQUAL(METHOD_META_NAME, Name))
		{
			hr = nvs.ReadValueData(szTempBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			D_INFO(99, TEXT("Method Name=%s"), szTempBuffer);
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(tcomplex)));
		}
		else if (IS_FIELD_NAME(tsimple_Value, Name))
		{
			ATLASSERT(sizeof(tsimple_Value) == ValueSize);
			hr = nvs.ReadValueData(tsimple_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(vector_tsimple_Value, Name))
		{
			hr = ReadVector(vector_tsimple_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_tsimple_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (CNameValueStream::IsEnding(Name, ValueSize))
		{
			D_INFO(99, TEXT("Stream END"));
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
	}
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadDataValue"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT tcomplex::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT tcomplex::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT tcomplex2::ReadFrom( IFileDevice * pFileDevice )
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CNameValueStream nvs(pFileDevice);
	CNameValueStream::DATA_BYTE_ORDER ByteOrder;
	hr = nvs.GetByteOrder(ByteOrder);
	ATLASSERT(CNameValueStream::LITTLE_ENDIAN == ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	LONG ValueSize = 0;
	wstring Name;
	TCHAR szTempBuffer[256] = {0};
	for (;;)
	{
		hr = nvs.ReadNameValueSize(Name, ValueSize);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadNameValueSize"), hr, TEXT(""));
			break;
		}
		if (IS_STRING_EQUAL(METHOD_META_NAME, Name))
		{
			hr = nvs.ReadValueData(szTempBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			D_INFO(99, TEXT("Method Name=%s"), szTempBuffer);
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(tcomplex2)));
		}
		else if (IS_FIELD_NAME(tcomplex_Value, Name))
		{
			ATLASSERT(sizeof(tcomplex_Value) == ValueSize);
			hr = nvs.ReadValueData(tcomplex_Value);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(vector_tcomplex_Value, Name))
		{
			hr = ReadVector(vector_tcomplex_Value, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vector_tcomplex_Value));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(v3_tcomplex, Name))
		{
			hr = ReadVector(v3_tcomplex, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(v3_tcomplex));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(l3_tcomplex, Name))
		{
			hr = ReadVector(l3_tcomplex, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(l3_tcomplex));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(q3_tcomplex, Name))
		{
			hr = ReadVector(q3_tcomplex, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(q3_tcomplex));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(q4_tcomplex, Name))
		{
			hr = ReadVector(q4_tcomplex, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(q4_tcomplex));
				LEAVE_BLOCK(0);
			}
		}
		else if (CNameValueStream::IsEnding(Name, ValueSize))
		{
			D_INFO(99, TEXT("Stream END"));
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
	}
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadDataValue"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT tcomplex2::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT tcomplex2::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////

