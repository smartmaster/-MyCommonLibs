

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT CGetVolumeInformationXml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(CGetVolumeInformationXml), _countof(MethodString(CGetVolumeInformationXml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_VolumeNameBuffer),m_VolumeNameBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_VolumeNameBuffer));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_VolumeSerialNumber),m_VolumeSerialNumber);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_VolumeSerialNumber));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_MaximumComponentLength),m_MaximumComponentLength);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_MaximumComponentLength));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_FileSystemFlags),m_FileSystemFlags);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_FileSystemFlags));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_FileSystemNameBuffer),m_FileSystemNameBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_FileSystemNameBuffer));
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

HRESULT CGetVolumeInformationXml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT CGetVolumeInformationXml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT NTFS_VOLUME_DATA_BUFFER_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(NTFS_VOLUME_DATA_BUFFER_Xml), _countof(MethodString(NTFS_VOLUME_DATA_BUFFER_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(VolumeSerialNumber),VolumeSerialNumber);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(VolumeSerialNumber));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(NumberSectors),NumberSectors);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(NumberSectors));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(TotalClusters),TotalClusters);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(TotalClusters));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(FreeClusters),FreeClusters);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(FreeClusters));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(TotalReserved),TotalReserved);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(TotalReserved));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BytesPerSector),BytesPerSector);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BytesPerSector));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BytesPerCluster),BytesPerCluster);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BytesPerCluster));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BytesPerFileRecordSegment),BytesPerFileRecordSegment);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BytesPerFileRecordSegment));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ClustersPerFileRecordSegment),ClustersPerFileRecordSegment);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ClustersPerFileRecordSegment));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MftValidDataLength),MftValidDataLength);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MftValidDataLength));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MftStartLcn),MftStartLcn);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MftStartLcn));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Mft2StartLcn),Mft2StartLcn);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Mft2StartLcn));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MftZoneStart),MftZoneStart);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MftZoneStart));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MftZoneEnd),MftZoneEnd);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MftZoneEnd));
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

HRESULT NTFS_VOLUME_DATA_BUFFER_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT NTFS_VOLUME_DATA_BUFFER_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT DISK_EXTENT_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(DISK_EXTENT_Xml), _countof(MethodString(DISK_EXTENT_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DiskNumber),DiskNumber);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DiskNumber));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(StartingOffset),StartingOffset);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(StartingOffset));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ExtentLength),ExtentLength);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ExtentLength));
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

HRESULT DISK_EXTENT_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT DISK_EXTENT_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT VOLUME_DISK_EXTENTS_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(VOLUME_DISK_EXTENTS_Xml), _countof(MethodString(VOLUME_DISK_EXTENTS_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(NumberOfDiskExtents),NumberOfDiskExtents);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(NumberOfDiskExtents));
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(Extents, FIELD_NAME(Extents), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(Extents));
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

HRESULT VOLUME_DISK_EXTENTS_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT VOLUME_DISK_EXTENTS_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT VOLUME_BITMAP_BUFFER_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(VOLUME_BITMAP_BUFFER_Xml), _countof(MethodString(VOLUME_BITMAP_BUFFER_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(StartingLcn),StartingLcn);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(StartingLcn));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BitmapSize),BitmapSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BitmapSize));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Buffer),Buffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Buffer));
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

HRESULT VOLUME_BITMAP_BUFFER_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT VOLUME_BITMAP_BUFFER_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT GET_LENGTH_INFORMATION_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(GET_LENGTH_INFORMATION_Xml), _countof(MethodString(GET_LENGTH_INFORMATION_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Length),Length);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Length));
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

HRESULT GET_LENGTH_INFORMATION_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT GET_LENGTH_INFORMATION_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT CSingleVolumeInfoXml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(CSingleVolumeInfoXml), _countof(MethodString(CSingleVolumeInfoXml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_strVolumeGUID),m_strVolumeGUID);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_strVolumeGUID));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(m_Paths, FIELD_NAME(m_Paths), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(m_Paths));
		LEAVE_BLOCK(0);
	}
	hr = WriteVector(m_MountPoint, FIELD_NAME(m_MountPoint), nvs);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(m_MountPoint));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_GetVolumeInformation));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_GetVolumeInformation.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_NtfsVolumeDataBuffer));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_NtfsVolumeDataBuffer.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_VolumeDiskExtents));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_VolumeDiskExtents.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_VolumeBitmapBuffer));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_VolumeBitmapBuffer.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_GetLengthInformation));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_GetLengthInformation.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
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

HRESULT CSingleVolumeInfoXml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT CSingleVolumeInfoXml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT CAllVolumeInfoXml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(CAllVolumeInfoXml), _countof(MethodString(CAllVolumeInfoXml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(m_pVolumeInfos, FIELD_NAME(m_pVolumeInfos), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_pVolumeInfos));
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

HRESULT CAllVolumeInfoXml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT CAllVolumeInfoXml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT CGetVolumeInformationXml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(CGetVolumeInformationXml)));
		}
		else if (IS_FIELD_NAME(m_VolumeNameBuffer, Name))
		{
			ATLASSERT(sizeof(m_VolumeNameBuffer) == ValueSize);
			hr = nvs.ReadValueData(m_VolumeNameBuffer);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_VolumeSerialNumber, Name))
		{
			ATLASSERT(sizeof(m_VolumeSerialNumber) == ValueSize);
			hr = nvs.ReadValueData(m_VolumeSerialNumber);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_MaximumComponentLength, Name))
		{
			ATLASSERT(sizeof(m_MaximumComponentLength) == ValueSize);
			hr = nvs.ReadValueData(m_MaximumComponentLength);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_FileSystemFlags, Name))
		{
			ATLASSERT(sizeof(m_FileSystemFlags) == ValueSize);
			hr = nvs.ReadValueData(m_FileSystemFlags);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(m_FileSystemNameBuffer, Name))
		{
			ATLASSERT(sizeof(m_FileSystemNameBuffer) == ValueSize);
			hr = nvs.ReadValueData(m_FileSystemNameBuffer);
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

HRESULT CGetVolumeInformationXml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT CGetVolumeInformationXml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT NTFS_VOLUME_DATA_BUFFER_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(NTFS_VOLUME_DATA_BUFFER_Xml)));
		}
		else if (IS_FIELD_NAME(VolumeSerialNumber, Name))
		{
			ATLASSERT(sizeof(VolumeSerialNumber) == ValueSize);
			hr = nvs.ReadValueData(VolumeSerialNumber);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(NumberSectors, Name))
		{
			ATLASSERT(sizeof(NumberSectors) == ValueSize);
			hr = nvs.ReadValueData(NumberSectors);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(TotalClusters, Name))
		{
			ATLASSERT(sizeof(TotalClusters) == ValueSize);
			hr = nvs.ReadValueData(TotalClusters);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(FreeClusters, Name))
		{
			ATLASSERT(sizeof(FreeClusters) == ValueSize);
			hr = nvs.ReadValueData(FreeClusters);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(TotalReserved, Name))
		{
			ATLASSERT(sizeof(TotalReserved) == ValueSize);
			hr = nvs.ReadValueData(TotalReserved);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(BytesPerSector, Name))
		{
			ATLASSERT(sizeof(BytesPerSector) == ValueSize);
			hr = nvs.ReadValueData(BytesPerSector);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(BytesPerCluster, Name))
		{
			ATLASSERT(sizeof(BytesPerCluster) == ValueSize);
			hr = nvs.ReadValueData(BytesPerCluster);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(BytesPerFileRecordSegment, Name))
		{
			ATLASSERT(sizeof(BytesPerFileRecordSegment) == ValueSize);
			hr = nvs.ReadValueData(BytesPerFileRecordSegment);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ClustersPerFileRecordSegment, Name))
		{
			ATLASSERT(sizeof(ClustersPerFileRecordSegment) == ValueSize);
			hr = nvs.ReadValueData(ClustersPerFileRecordSegment);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MftValidDataLength, Name))
		{
			ATLASSERT(sizeof(MftValidDataLength) == ValueSize);
			hr = nvs.ReadValueData(MftValidDataLength);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MftStartLcn, Name))
		{
			ATLASSERT(sizeof(MftStartLcn) == ValueSize);
			hr = nvs.ReadValueData(MftStartLcn);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(Mft2StartLcn, Name))
		{
			ATLASSERT(sizeof(Mft2StartLcn) == ValueSize);
			hr = nvs.ReadValueData(Mft2StartLcn);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MftZoneStart, Name))
		{
			ATLASSERT(sizeof(MftZoneStart) == ValueSize);
			hr = nvs.ReadValueData(MftZoneStart);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MftZoneEnd, Name))
		{
			ATLASSERT(sizeof(MftZoneEnd) == ValueSize);
			hr = nvs.ReadValueData(MftZoneEnd);
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

HRESULT NTFS_VOLUME_DATA_BUFFER_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT NTFS_VOLUME_DATA_BUFFER_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT DISK_EXTENT_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(DISK_EXTENT_Xml)));
		}
		else if (IS_FIELD_NAME(DiskNumber, Name))
		{
			ATLASSERT(sizeof(DiskNumber) == ValueSize);
			hr = nvs.ReadValueData(DiskNumber);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(StartingOffset, Name))
		{
			ATLASSERT(sizeof(StartingOffset) == ValueSize);
			hr = nvs.ReadValueData(StartingOffset);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ExtentLength, Name))
		{
			ATLASSERT(sizeof(ExtentLength) == ValueSize);
			hr = nvs.ReadValueData(ExtentLength);
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

HRESULT DISK_EXTENT_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT DISK_EXTENT_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT VOLUME_DISK_EXTENTS_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(VOLUME_DISK_EXTENTS_Xml)));
		}
		else if (IS_FIELD_NAME(NumberOfDiskExtents, Name))
		{
			ATLASSERT(sizeof(NumberOfDiskExtents) == ValueSize);
			hr = nvs.ReadValueData(NumberOfDiskExtents);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(Extents, Name))
		{
			hr = ReadComplexVector(Extents, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(Extents));
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

HRESULT VOLUME_DISK_EXTENTS_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT VOLUME_DISK_EXTENTS_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT VOLUME_BITMAP_BUFFER_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(VOLUME_BITMAP_BUFFER_Xml)));
		}
		else if (IS_FIELD_NAME(StartingLcn, Name))
		{
			ATLASSERT(sizeof(StartingLcn) == ValueSize);
			hr = nvs.ReadValueData(StartingLcn);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(BitmapSize, Name))
		{
			ATLASSERT(sizeof(BitmapSize) == ValueSize);
			hr = nvs.ReadValueData(BitmapSize);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(Buffer, Name))
		{
			ATLASSERT(sizeof(Buffer) == ValueSize);
			hr = nvs.ReadValueData(Buffer);
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

HRESULT VOLUME_BITMAP_BUFFER_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT VOLUME_BITMAP_BUFFER_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT GET_LENGTH_INFORMATION_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(GET_LENGTH_INFORMATION_Xml)));
		}
		else if (IS_FIELD_NAME(Length, Name))
		{
			ATLASSERT(sizeof(Length) == ValueSize);
			hr = nvs.ReadValueData(Length);
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

HRESULT GET_LENGTH_INFORMATION_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT GET_LENGTH_INFORMATION_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT CSingleVolumeInfoXml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(CSingleVolumeInfoXml)));
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
		else if (IS_FIELD_NAME(m_Paths, Name))
		{
			hr = ReadVector(m_Paths, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(m_Paths));
				LEAVE_BLOCK(0);
			}
		}
		else if (IS_FIELD_NAME(m_MountPoint, Name))
		{
			hr = ReadVector(m_MountPoint, Name, ValueSize, nvs);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteVector"), hr, TEXT("%s"), FIELD_NAME(m_MountPoint));
				LEAVE_BLOCK(0);
			}
		}
		else if (Name == FIELD_NAME( m_GetVolumeInformation) && ValueSize == -1)
		{
			hr = m_GetVolumeInformation.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_GetVolumeInformation);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_NtfsVolumeDataBuffer) && ValueSize == -1)
		{
			hr = m_NtfsVolumeDataBuffer.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_NtfsVolumeDataBuffer);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_VolumeDiskExtents) && ValueSize == -1)
		{
			hr = m_VolumeDiskExtents.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_VolumeDiskExtents);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_VolumeBitmapBuffer) && ValueSize == -1)
		{
			hr = m_VolumeBitmapBuffer.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_VolumeBitmapBuffer);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_GetLengthInformation) && ValueSize == -1)
		{
			hr = m_GetLengthInformation.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_GetLengthInformation);
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

HRESULT CSingleVolumeInfoXml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT CSingleVolumeInfoXml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT CAllVolumeInfoXml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(CAllVolumeInfoXml)));
		}
		else if (IS_FIELD_NAME(m_pVolumeInfos, Name))
		{
			hr = ReadComplexVector(m_pVolumeInfos, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_pVolumeInfos));
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

HRESULT CAllVolumeInfoXml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT CAllVolumeInfoXml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////

