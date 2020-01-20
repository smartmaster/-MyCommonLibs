

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT CAllVdsDiskVolumeInfoXml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(CAllVdsDiskVolumeInfoXml), _countof(MethodString(CAllVdsDiskVolumeInfoXml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(m_pVdsDiskInfos, FIELD_NAME(m_pVdsDiskInfos), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_pVdsDiskInfos));
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(m_pVdsVolumeInfos, FIELD_NAME(m_pVdsVolumeInfos), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_pVdsVolumeInfos));
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

HRESULT CAllVdsDiskVolumeInfoXml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT CAllVdsDiskVolumeInfoXml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT CAllVdsDiskVolumeInfoXml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(CAllVdsDiskVolumeInfoXml)));
		}
		else if (IS_FIELD_NAME(m_pVdsDiskInfos, Name))
		{
			hr = ReadComplexVector(m_pVdsDiskInfos, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_pVdsDiskInfos));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(m_pVdsVolumeInfos, Name))
		{
			hr = ReadComplexVector(m_pVdsVolumeInfos, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_pVdsVolumeInfos));
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

HRESULT CAllVdsDiskVolumeInfoXml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT CAllVdsDiskVolumeInfoXml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////

