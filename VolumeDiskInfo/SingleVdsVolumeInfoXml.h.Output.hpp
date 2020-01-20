

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT VDS_VOLUME_PLEX_PROP_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(VDS_VOLUME_PLEX_PROP_Xml), _countof(MethodString(VDS_VOLUME_PLEX_PROP_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(id),id);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(id));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(type),type);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(type));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(status),status);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(status));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(health),health);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(health));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(TransitionState),TransitionState);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(TransitionState));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ullSize),ullSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ullSize));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ulStripeSize),ulStripeSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ulStripeSize));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ulNumberOfMembers),ulNumberOfMembers);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ulNumberOfMembers));
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

HRESULT VDS_VOLUME_PLEX_PROP_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT VDS_VOLUME_PLEX_PROP_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT VDS_DISK_EXTENT_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(VDS_DISK_EXTENT_Xml), _countof(MethodString(VDS_DISK_EXTENT_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(diskId),diskId);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(diskId));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(type),type);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(type));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ullOffset),ullOffset);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ullOffset));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ullSize),ullSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ullSize));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(volumeId),volumeId);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(volumeId));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(plexId),plexId);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(plexId));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(memberIdx),memberIdx);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(memberIdx));
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

HRESULT VDS_DISK_EXTENT_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT VDS_DISK_EXTENT_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT CSingleVdsVolumePlexXml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(CSingleVdsVolumePlexXml), _countof(MethodString(CSingleVdsVolumePlexXml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_VDSVolumePlexProp));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_VDSVolumePlexProp.WriteTo(pFileDevice);
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

HRESULT CSingleVdsVolumePlexXml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT CSingleVdsVolumePlexXml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT MY_VDS_VOLUME_PROP_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(MY_VDS_VOLUME_PROP_Xml), _countof(MethodString(MY_VDS_VOLUME_PROP_Xml)));
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
	hr = nvs.WriteNameValue(FIELD_NAME(m_type),m_type);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_type));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_status),m_status);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_status));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_health),m_health);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_health));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_TransitionState),m_TransitionState);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_TransitionState));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_ullSize),m_ullSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_ullSize));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_ulFlags),m_ulFlags);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_ulFlags));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_RecommendedFileSystemType),m_RecommendedFileSystemType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_RecommendedFileSystemType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_strName),m_strName);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_strName));
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

HRESULT MY_VDS_VOLUME_PROP_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT MY_VDS_VOLUME_PROP_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT CSingleVdsVolumeInfoXml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(CSingleVdsVolumeInfoXml), _countof(MethodString(CSingleVdsVolumeInfoXml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_MyVDSVolumeProp));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_MyVDSVolumeProp.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_strVolumeGUID),m_strVolumeGUID);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_strVolumeGUID));
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(m_VolumePlexes, FIELD_NAME(m_VolumePlexes), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_VolumePlexes));
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

HRESULT CSingleVdsVolumeInfoXml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT CSingleVdsVolumeInfoXml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT VDS_VOLUME_PLEX_PROP_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(VDS_VOLUME_PLEX_PROP_Xml)));
		}
		else if (IS_FIELD_NAME(id, Name))
		{
			ATLASSERT(sizeof(id) == ValueSize);
			hr = nvs.ReadValueData(id);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(type, Name))
		{
			ATLASSERT(sizeof(type) == ValueSize);
			hr = nvs.ReadValueData(type);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(status, Name))
		{
			ATLASSERT(sizeof(status) == ValueSize);
			hr = nvs.ReadValueData(status);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(health, Name))
		{
			ATLASSERT(sizeof(health) == ValueSize);
			hr = nvs.ReadValueData(health);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(TransitionState, Name))
		{
			ATLASSERT(sizeof(TransitionState) == ValueSize);
			hr = nvs.ReadValueData(TransitionState);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ullSize, Name))
		{
			ATLASSERT(sizeof(ullSize) == ValueSize);
			hr = nvs.ReadValueData(ullSize);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ulStripeSize, Name))
		{
			ATLASSERT(sizeof(ulStripeSize) == ValueSize);
			hr = nvs.ReadValueData(ulStripeSize);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ulNumberOfMembers, Name))
		{
			ATLASSERT(sizeof(ulNumberOfMembers) == ValueSize);
			hr = nvs.ReadValueData(ulNumberOfMembers);
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

HRESULT VDS_VOLUME_PLEX_PROP_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT VDS_VOLUME_PLEX_PROP_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT VDS_DISK_EXTENT_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(VDS_DISK_EXTENT_Xml)));
		}
		else if (IS_FIELD_NAME(diskId, Name))
		{
			ATLASSERT(sizeof(diskId) == ValueSize);
			hr = nvs.ReadValueData(diskId);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(type, Name))
		{
			ATLASSERT(sizeof(type) == ValueSize);
			hr = nvs.ReadValueData(type);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ullOffset, Name))
		{
			ATLASSERT(sizeof(ullOffset) == ValueSize);
			hr = nvs.ReadValueData(ullOffset);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ullSize, Name))
		{
			ATLASSERT(sizeof(ullSize) == ValueSize);
			hr = nvs.ReadValueData(ullSize);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(volumeId, Name))
		{
			ATLASSERT(sizeof(volumeId) == ValueSize);
			hr = nvs.ReadValueData(volumeId);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(plexId, Name))
		{
			ATLASSERT(sizeof(plexId) == ValueSize);
			hr = nvs.ReadValueData(plexId);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(memberIdx, Name))
		{
			ATLASSERT(sizeof(memberIdx) == ValueSize);
			hr = nvs.ReadValueData(memberIdx);
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

HRESULT VDS_DISK_EXTENT_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT VDS_DISK_EXTENT_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT CSingleVdsVolumePlexXml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(CSingleVdsVolumePlexXml)));
		}
		else if (Name == FIELD_NAME( m_VDSVolumePlexProp) && ValueSize == -1)
		{
			hr = m_VDSVolumePlexProp.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_VDSVolumePlexProp);
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

HRESULT CSingleVdsVolumePlexXml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT CSingleVdsVolumePlexXml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT MY_VDS_VOLUME_PROP_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(MY_VDS_VOLUME_PROP_Xml)));
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
		else if (IS_FIELD_NAME(m_type, Name))
		{
			ATLASSERT(sizeof(m_type) == ValueSize);
			hr = nvs.ReadValueData(m_type);
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
		else if (IS_FIELD_NAME(m_TransitionState, Name))
		{
			ATLASSERT(sizeof(m_TransitionState) == ValueSize);
			hr = nvs.ReadValueData(m_TransitionState);
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
		else if (IS_FIELD_NAME(m_RecommendedFileSystemType, Name))
		{
			ATLASSERT(sizeof(m_RecommendedFileSystemType) == ValueSize);
			hr = nvs.ReadValueData(m_RecommendedFileSystemType);
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

HRESULT MY_VDS_VOLUME_PROP_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT MY_VDS_VOLUME_PROP_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT CSingleVdsVolumeInfoXml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(CSingleVdsVolumeInfoXml)));
		}
		else if (Name == FIELD_NAME( m_MyVDSVolumeProp) && ValueSize == -1)
		{
			hr = m_MyVDSVolumeProp.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_MyVDSVolumeProp);
				break;
			}
		}
		else if (IS_FIELD_NAME(m_strVolumeGUID, Name))
		{
			ATLASSERT(sizeof(m_strVolumeGUID) == ValueSize);
			hr = nvs.ReadValueData(m_strVolumeGUID);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_VolumePlexes, Name))
		{
			hr = ReadComplexVector(m_VolumePlexes, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_VolumePlexes));
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

HRESULT CSingleVdsVolumeInfoXml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT CSingleVdsVolumeInfoXml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////

