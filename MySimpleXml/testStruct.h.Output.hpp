

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT simple::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(simple), _countof(MethodString(simple)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(s),s);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(s));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ull),ull);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ull));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(cstr),cstr.GetString(),cstr.GetLength() + 1);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(cstr));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vecs, FIELD_NAME(vecs), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecs));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vecull, FIELD_NAME(vecull), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecull));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vecstr, FIELD_NAME(vecstr), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecstr));
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

HRESULT simple::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT simple::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT complex_1::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(complex_1), _countof(MethodString(complex_1)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(sub));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = sub.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(vecsub, FIELD_NAME(vecsub), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(vecsub));
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

HRESULT complex_1::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT complex_1::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT sssvecstr::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(sssvecstr), _countof(MethodString(sssvecstr)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vecs, FIELD_NAME(vecs), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecs));
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

HRESULT sssvecstr::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT sssvecstr::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT SSSSBufferTest::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(SSSSBufferTest), _countof(MethodString(SSSSBufferTest)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(buffer1),buffer1);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(buffer1));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(buffer2),buffer2);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(buffer2));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vecBuffer1, FIELD_NAME(vecBuffer1), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecBuffer1));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vecBuffer2, FIELD_NAME(vecBuffer2), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecBuffer2));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(guid),guid);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(guid));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vecGuid, FIELD_NAME(vecGuid), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecGuid));
		LEAVE_BLOCK(0);
	}
	//<!-- VECTOR_2D not supported vecsdStr -->
	//<!-- COMPLEX_VECTOR_2D not supported vecComplex2D -->
	hr = WriteComplexVector(vecvec, FIELD_NAME(vecvec), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(vecvec));
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

HRESULT SSSSBufferTest::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT SSSSBufferTest::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT vec2d_simple::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(vec2d_simple), _countof(MethodString(vec2d_simple)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	//<!-- VECTOR_2D not supported v2str -->
	//<!-- VECTOR_2D not supported vvguid -->
	hr = nvs.WriteEnding();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteEnding"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT vec2d_simple::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT vec2d_simple::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT vec2d_complex::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(vec2d_complex), _countof(MethodString(vec2d_complex)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	//<!-- COMPLEX_VECTOR_2D not supported vvcomplex -->
	hr = nvs.WriteEnding();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteEnding"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT vec2d_complex::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT vec2d_complex::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT ut_simle::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(ut_simle), _countof(MethodString(ut_simle)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(x),x);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(x));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(vx, FIELD_NAME(vx), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vx));
		LEAVE_BLOCK(0);
	}
	//<!-- VECTOR_2D not supported v2x -->
	//<!-- VECTOR_3D not supported v3x -->
	hr = nvs.WriteEnding();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteEnding"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT ut_simle::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT ut_simle::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT ut_complex::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(ut_complex), _countof(MethodString(ut_complex)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(x));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = x.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(vx, FIELD_NAME(vx), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(vx));
		LEAVE_BLOCK(0);
	}
	//<!-- COMPLEX_VECTOR_2D not supported v2x -->
	//<!-- COMPLEX_VECTOR_3D not supported v3x -->
	hr = nvs.WriteEnding();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteEnding"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT ut_complex::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT ut_complex::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



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
HRESULT simple::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(simple)));
		}
		else if (IS_FIELD_NAME(s, Name))
		{
			ATLASSERT(sizeof(s) == ValueSize);
			hr = nvs.ReadValueData(s);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ull, Name))
		{
			ATLASSERT(sizeof(ull) == ValueSize);
			hr = nvs.ReadValueData(ull);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(cstr, Name))
		{
			LPTSTR pBuffer = cstr.GetBuffer(ValueSize);
			hr = nvs.ReadValueData(pBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			cstr.ReleaseBuffer();
		}
		else if (IS_FIELD_NAME(vecs, Name))
		{
			hr = ReadVector(vecs, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecs));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vecull, Name))
		{
			hr = ReadVector(vecull, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecull));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vecstr, Name))
		{
			hr = ReadVector(vecstr, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecstr));
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

HRESULT simple::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT simple::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT complex_1::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(complex_1)));
		}
		else if (Name == FIELD_NAME( sub) && ValueSize == -1)
		{
			hr = sub.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), sub);
				break;
			}
		}
		else if (IS_FIELD_NAME(vecsub, Name))
		{
			hr = ReadComplexVector(vecsub, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(vecsub));
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

HRESULT complex_1::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT complex_1::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT sssvecstr::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(sssvecstr)));
		}
		else if (IS_FIELD_NAME(vecs, Name))
		{
			hr = ReadVector(vecs, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecs));
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

HRESULT sssvecstr::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT sssvecstr::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT SSSSBufferTest::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(SSSSBufferTest)));
		}
		else if (IS_FIELD_NAME(buffer1, Name))
		{
			ATLASSERT(sizeof(buffer1) == ValueSize);
			hr = nvs.ReadValueData(buffer1);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(buffer2, Name))
		{
			ATLASSERT(sizeof(buffer2) == ValueSize);
			hr = nvs.ReadValueData(buffer2);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(vecBuffer1, Name))
		{
			hr = ReadVector(vecBuffer1, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecBuffer1));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(vecBuffer2, Name))
		{
			hr = ReadVector(vecBuffer2, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecBuffer2));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(guid, Name))
		{
			ATLASSERT(sizeof(guid) == ValueSize);
			hr = nvs.ReadValueData(guid);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(vecGuid, Name))
		{
			hr = ReadVector(vecGuid, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vecGuid));
				LEAVE_BLOCK(0);
			}
		}
	//<!-- VECTOR_2D not supported vecsdStr -->
	//<!-- COMPLEX_VECTOR_2D not supported vecComplex2D -->
		else if (IS_FIELD_NAME(vecvec, Name))
		{
			hr = ReadComplexVector(vecvec, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(vecvec));
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

HRESULT SSSSBufferTest::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT SSSSBufferTest::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT vec2d_simple::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(vec2d_simple)));
		}
	//<!-- VECTOR_2D not supported v2str -->
	//<!-- VECTOR_2D not supported vvguid -->
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

HRESULT vec2d_simple::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT vec2d_simple::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT vec2d_complex::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(vec2d_complex)));
		}
	//<!-- COMPLEX_VECTOR_2D not supported vvcomplex -->
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

HRESULT vec2d_complex::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT vec2d_complex::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT ut_simle::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(ut_simle)));
		}
		else if (IS_FIELD_NAME(x, Name))
		{
			ATLASSERT(sizeof(x) == ValueSize);
			hr = nvs.ReadValueData(x);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(vx, Name))
		{
			hr = ReadVector(vx, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(vx));
				LEAVE_BLOCK(0);
			}
		}
	//<!-- VECTOR_2D not supported v2x -->
	//<!-- VECTOR_3D not supported v3x -->
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

HRESULT ut_simle::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT ut_simle::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT ut_complex::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(ut_complex)));
		}
		else if (Name == FIELD_NAME( x) && ValueSize == -1)
		{
			hr = x.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), x);
				break;
			}
		}
		else if (IS_FIELD_NAME(vx, Name))
		{
			hr = ReadComplexVector(vx, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(vx));
				LEAVE_BLOCK(0);
			}
		}
	//<!-- COMPLEX_VECTOR_2D not supported v2x -->
	//<!-- COMPLEX_VECTOR_3D not supported v3x -->
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

HRESULT ut_complex::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT ut_complex::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



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

