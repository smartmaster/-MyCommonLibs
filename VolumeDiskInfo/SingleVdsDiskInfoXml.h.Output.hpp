

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT MY_VDS_DISK_PROP_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(MY_VDS_DISK_PROP_Xml), _countof(MethodString(MY_VDS_DISK_PROP_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_id),m_id);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_id));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_status),m_status);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_status));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_ReserveMode),m_ReserveMode);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_ReserveMode));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_health),m_health);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_health));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_dwDeviceType),m_dwDeviceType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_dwDeviceType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_dwMediaType),m_dwMediaType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_dwMediaType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_ullSize),m_ullSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_ullSize));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_ulBytesPerSector),m_ulBytesPerSector);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_ulBytesPerSector));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_ulSectorsPerTrack),m_ulSectorsPerTrack);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_ulSectorsPerTrack));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_ulTracksPerCylinder),m_ulTracksPerCylinder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_ulTracksPerCylinder));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_ulFlags),m_ulFlags);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_ulFlags));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_BusType),m_BusType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_BusType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_PartitionStyle),m_PartitionStyle);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_PartitionStyle));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_DiskGuid),m_DiskGuid);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_DiskGuid));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_strDiskAddress),m_strDiskAddress);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_strDiskAddress));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_strName),m_strName);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_strName));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_strFriendlyName),m_strFriendlyName);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_strFriendlyName));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_strAdaptorName),m_strAdaptorName);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_strAdaptorName));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_strDevicePath),m_strDevicePath);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_strDevicePath));
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

HRESULT MY_VDS_DISK_PROP_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT MY_VDS_DISK_PROP_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT CSingleVdsDiskInfoXml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(CSingleVdsDiskInfoXml), _countof(MethodString(CSingleVdsDiskInfoXml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_MyVDSDiskProp));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_MyVDSDiskProp.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(m_VDSDiskExtents, FIELD_NAME(m_VDSDiskExtents), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_VDSDiskExtents));
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

HRESULT CSingleVdsDiskInfoXml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT CSingleVdsDiskInfoXml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT MY_VDS_DISK_PROP_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(MY_VDS_DISK_PROP_Xml)));
		}
		else if (IS_FIELD_NAME(m_id, Name))
		{
			ATLASSERT(sizeof(m_id) == ValueSize);
			hr = nvs.ReadValueData(m_id);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_status, Name))
		{
			ATLASSERT(sizeof(m_status) == ValueSize);
			hr = nvs.ReadValueData(m_status);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_ReserveMode, Name))
		{
			ATLASSERT(sizeof(m_ReserveMode) == ValueSize);
			hr = nvs.ReadValueData(m_ReserveMode);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_health, Name))
		{
			ATLASSERT(sizeof(m_health) == ValueSize);
			hr = nvs.ReadValueData(m_health);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_dwDeviceType, Name))
		{
			ATLASSERT(sizeof(m_dwDeviceType) == ValueSize);
			hr = nvs.ReadValueData(m_dwDeviceType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_dwMediaType, Name))
		{
			ATLASSERT(sizeof(m_dwMediaType) == ValueSize);
			hr = nvs.ReadValueData(m_dwMediaType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_ullSize, Name))
		{
			ATLASSERT(sizeof(m_ullSize) == ValueSize);
			hr = nvs.ReadValueData(m_ullSize);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_ulBytesPerSector, Name))
		{
			ATLASSERT(sizeof(m_ulBytesPerSector) == ValueSize);
			hr = nvs.ReadValueData(m_ulBytesPerSector);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_ulSectorsPerTrack, Name))
		{
			ATLASSERT(sizeof(m_ulSectorsPerTrack) == ValueSize);
			hr = nvs.ReadValueData(m_ulSectorsPerTrack);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_ulTracksPerCylinder, Name))
		{
			ATLASSERT(sizeof(m_ulTracksPerCylinder) == ValueSize);
			hr = nvs.ReadValueData(m_ulTracksPerCylinder);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_ulFlags, Name))
		{
			ATLASSERT(sizeof(m_ulFlags) == ValueSize);
			hr = nvs.ReadValueData(m_ulFlags);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_BusType, Name))
		{
			ATLASSERT(sizeof(m_BusType) == ValueSize);
			hr = nvs.ReadValueData(m_BusType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_PartitionStyle, Name))
		{
			ATLASSERT(sizeof(m_PartitionStyle) == ValueSize);
			hr = nvs.ReadValueData(m_PartitionStyle);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_DiskGuid, Name))
		{
			ATLASSERT(sizeof(m_DiskGuid) == ValueSize);
			hr = nvs.ReadValueData(m_DiskGuid);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_strDiskAddress, Name))
		{
			ATLASSERT(sizeof(m_strDiskAddress) == ValueSize);
			hr = nvs.ReadValueData(m_strDiskAddress);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_strName, Name))
		{
			ATLASSERT(sizeof(m_strName) == ValueSize);
			hr = nvs.ReadValueData(m_strName);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_strFriendlyName, Name))
		{
			ATLASSERT(sizeof(m_strFriendlyName) == ValueSize);
			hr = nvs.ReadValueData(m_strFriendlyName);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_strAdaptorName, Name))
		{
			ATLASSERT(sizeof(m_strAdaptorName) == ValueSize);
			hr = nvs.ReadValueData(m_strAdaptorName);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_strDevicePath, Name))
		{
			ATLASSERT(sizeof(m_strDevicePath) == ValueSize);
			hr = nvs.ReadValueData(m_strDevicePath);
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

HRESULT MY_VDS_DISK_PROP_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT MY_VDS_DISK_PROP_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT CSingleVdsDiskInfoXml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(CSingleVdsDiskInfoXml)));
		}
		else if (Name == FIELD_NAME( m_MyVDSDiskProp) && ValueSize == -1)
		{
			hr = m_MyVDSDiskProp.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_MyVDSDiskProp);
				break;
			}
		}
		else if (IS_FIELD_NAME(m_VDSDiskExtents, Name))
		{
			hr = ReadComplexVector(m_VDSDiskExtents, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_VDSDiskExtents));
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

HRESULT CSingleVdsDiskInfoXml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT CSingleVdsDiskInfoXml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////

