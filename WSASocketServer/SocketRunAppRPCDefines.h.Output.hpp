

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT ParameterRunApp::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(RunApp), _countof(MethodString(RunApp)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Username),Username.GetString(),Username.GetLength() + 1);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Username));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Domain),Domain.GetString(),Domain.GetLength() + 1);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Domain));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Password),Password.GetString(),Password.GetLength() + 1);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Password));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(CommandLine),CommandLine.GetString(),CommandLine.GetLength() + 1);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(CommandLine));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(CurDir),CurDir.GetString(),CurDir.GetLength() + 1);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(CurDir));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(TimeOut),TimeOut);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(TimeOut));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(TerminateOnTimeOut),TerminateOnTimeOut);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(TerminateOnTimeOut));
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

HRESULT ParameterRunApp::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT ParameterRunApp::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT ParameterResultRunApp::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(ResultRunApp), _countof(MethodString(ResultRunApp)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(hrStatus),hrStatus);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(hrStatus));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(strErrorMessage),strErrorMessage.GetString(),strErrorMessage.GetLength() + 1);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(strErrorMessage));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ExitCode),ExitCode);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ExitCode));
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

HRESULT ParameterResultRunApp::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT ParameterResultRunApp::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT ParameterRunApp::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(RunApp)));
		}
		else if (IS_FIELD_NAME(Username, Name))
		{
			LPTSTR pBuffer = Username.GetBuffer(ValueSize);
			hr = nvs.ReadValueData(pBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			Username.ReleaseBuffer();
		}
		else if (IS_FIELD_NAME(Domain, Name))
		{
			LPTSTR pBuffer = Domain.GetBuffer(ValueSize);
			hr = nvs.ReadValueData(pBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			Domain.ReleaseBuffer();
		}
		else if (IS_FIELD_NAME(Password, Name))
		{
			LPTSTR pBuffer = Password.GetBuffer(ValueSize);
			hr = nvs.ReadValueData(pBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			Password.ReleaseBuffer();
		}
		else if (IS_FIELD_NAME(CommandLine, Name))
		{
			LPTSTR pBuffer = CommandLine.GetBuffer(ValueSize);
			hr = nvs.ReadValueData(pBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			CommandLine.ReleaseBuffer();
		}
		else if (IS_FIELD_NAME(CurDir, Name))
		{
			LPTSTR pBuffer = CurDir.GetBuffer(ValueSize);
			hr = nvs.ReadValueData(pBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			CurDir.ReleaseBuffer();
		}
		else if (IS_FIELD_NAME(TimeOut, Name))
		{
			ATLASSERT(sizeof(TimeOut) == ValueSize);
			hr = nvs.ReadValueData(TimeOut);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(TerminateOnTimeOut, Name))
		{
			ATLASSERT(sizeof(TerminateOnTimeOut) == ValueSize);
			hr = nvs.ReadValueData(TerminateOnTimeOut);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
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

HRESULT ParameterRunApp::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT ParameterRunApp::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT ParameterResultRunApp::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(ResultRunApp)));
		}
		else if (IS_FIELD_NAME(hrStatus, Name))
		{
			ATLASSERT(sizeof(hrStatus) == ValueSize);
			hr = nvs.ReadValueData(hrStatus);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(strErrorMessage, Name))
		{
			LPTSTR pBuffer = strErrorMessage.GetBuffer(ValueSize);
			hr = nvs.ReadValueData(pBuffer, ValueSize/sizeof(TCHAR));
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
			strErrorMessage.ReleaseBuffer();
		}
		else if (IS_FIELD_NAME(ExitCode, Name))
		{
			ATLASSERT(sizeof(ExitCode) == ValueSize);
			hr = nvs.ReadValueData(ExitCode);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
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

HRESULT ParameterResultRunApp::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT ParameterResultRunApp::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////

