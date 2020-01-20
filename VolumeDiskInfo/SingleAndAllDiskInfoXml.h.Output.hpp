

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT DISK_GEOMETRY_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(DISK_GEOMETRY_Xml), _countof(MethodString(DISK_GEOMETRY_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Cylinders),Cylinders);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Cylinders));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MediaType),MediaType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MediaType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(TracksPerCylinder),TracksPerCylinder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(TracksPerCylinder));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(SectorsPerTrack),SectorsPerTrack);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(SectorsPerTrack));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BytesPerSector),BytesPerSector);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BytesPerSector));
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

HRESULT DISK_GEOMETRY_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT DISK_GEOMETRY_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT DISK_GEOMETRY_EX_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(DISK_GEOMETRY_EX_Xml), _countof(MethodString(DISK_GEOMETRY_EX_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(Geometry));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = Geometry.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DiskSize),DiskSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DiskSize));
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

HRESULT DISK_GEOMETRY_EX_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT DISK_GEOMETRY_EX_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT DRIVE_LAYOUT_INFORMATION_GPT_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(DRIVE_LAYOUT_INFORMATION_GPT_Xml), _countof(MethodString(DRIVE_LAYOUT_INFORMATION_GPT_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DiskId),DiskId);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DiskId));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(StartingUsableOffset),StartingUsableOffset);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(StartingUsableOffset));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(UsableLength),UsableLength);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(UsableLength));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MaxPartitionCount),MaxPartitionCount);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MaxPartitionCount));
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

HRESULT DRIVE_LAYOUT_INFORMATION_GPT_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT DRIVE_LAYOUT_INFORMATION_GPT_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT PARTITION_INFORMATION_GPT_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(PARTITION_INFORMATION_GPT_Xml), _countof(MethodString(PARTITION_INFORMATION_GPT_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(PartitionType),PartitionType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(PartitionType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(PartitionId),PartitionId);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(PartitionId));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Attributes),Attributes);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Attributes));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Name),Name);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Name));
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

HRESULT PARTITION_INFORMATION_GPT_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT PARTITION_INFORMATION_GPT_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT PARTITION_INFORMATION_EX_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(PARTITION_INFORMATION_EX_Xml), _countof(MethodString(PARTITION_INFORMATION_EX_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(PartitionStyle),PartitionStyle);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(PartitionStyle));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(StartingOffset),StartingOffset);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(StartingOffset));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(PartitionLength),PartitionLength);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(PartitionLength));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(PartitionNumber),PartitionNumber);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(PartitionNumber));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(RewritePartition),RewritePartition);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(RewritePartition));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(Gpt));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = Gpt.WriteTo(pFileDevice);
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

HRESULT PARTITION_INFORMATION_EX_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT PARTITION_INFORMATION_EX_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT DRIVE_LAYOUT_INFORMATION_EX_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(DRIVE_LAYOUT_INFORMATION_EX_Xml), _countof(MethodString(DRIVE_LAYOUT_INFORMATION_EX_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(PartitionStyle),PartitionStyle);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(PartitionStyle));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(PartitionCount),PartitionCount);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(PartitionCount));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(Gpt));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = Gpt.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(PartitionEntry, FIELD_NAME(PartitionEntry), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(PartitionEntry));
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

HRESULT DRIVE_LAYOUT_INFORMATION_EX_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT DRIVE_LAYOUT_INFORMATION_EX_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT STORAGE_DEVICE_DESCRIPTOR_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(STORAGE_DEVICE_DESCRIPTOR_Xml), _countof(MethodString(STORAGE_DEVICE_DESCRIPTOR_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Version),Version);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Version));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Size),Size);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Size));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DeviceType),DeviceType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DeviceType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DeviceTypeModifier),DeviceTypeModifier);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DeviceTypeModifier));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(RemovableMedia),RemovableMedia);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(RemovableMedia));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(CommandQueueing),CommandQueueing);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(CommandQueueing));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(VendorIdOffset),VendorIdOffset);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(VendorIdOffset));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ProductIdOffset),ProductIdOffset);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ProductIdOffset));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(ProductRevisionOffset),ProductRevisionOffset);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(ProductRevisionOffset));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(SerialNumberOffset),SerialNumberOffset);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(SerialNumberOffset));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BusType),BusType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BusType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(RawPropertiesLength),RawPropertiesLength);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(RawPropertiesLength));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(RawDeviceProperties),RawDeviceProperties);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(RawDeviceProperties));
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

HRESULT STORAGE_DEVICE_DESCRIPTOR_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT STORAGE_DEVICE_DESCRIPTOR_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT STORAGE_ADAPTER_DESCRIPTOR_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(STORAGE_ADAPTER_DESCRIPTOR_Xml), _countof(MethodString(STORAGE_ADAPTER_DESCRIPTOR_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Version),Version);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Version));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Size),Size);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Size));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MaximumTransferLength),MaximumTransferLength);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MaximumTransferLength));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MaximumPhysicalPages),MaximumPhysicalPages);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MaximumPhysicalPages));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(AlignmentMask),AlignmentMask);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(AlignmentMask));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(AdapterUsesPio),AdapterUsesPio);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(AdapterUsesPio));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(AdapterScansDown),AdapterScansDown);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(AdapterScansDown));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(CommandQueueing),CommandQueueing);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(CommandQueueing));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(AcceleratedTransfer),AcceleratedTransfer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(AcceleratedTransfer));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BusType),BusType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BusType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BusType),BusType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BusType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BusMajorVersion),BusMajorVersion);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BusMajorVersion));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(BusMinorVersion),BusMinorVersion);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(BusMinorVersion));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(SrbType),SrbType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(SrbType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(AddressType),AddressType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(AddressType));
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

HRESULT STORAGE_ADAPTER_DESCRIPTOR_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT STORAGE_ADAPTER_DESCRIPTOR_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT STORAGE_DEVICE_ID_DESCRIPTOR_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(STORAGE_DEVICE_ID_DESCRIPTOR_Xml), _countof(MethodString(STORAGE_DEVICE_ID_DESCRIPTOR_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Version),Version);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Version));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Size),Size);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Size));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(NumberOfIdentifiers),NumberOfIdentifiers);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(NumberOfIdentifiers));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Identifiers),Identifiers);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Identifiers));
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

HRESULT STORAGE_DEVICE_ID_DESCRIPTOR_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT STORAGE_DEVICE_ID_DESCRIPTOR_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT STORAGE_DEVICE_NUMBER_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(STORAGE_DEVICE_NUMBER_Xml), _countof(MethodString(STORAGE_DEVICE_NUMBER_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DeviceType),DeviceType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DeviceType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DeviceNumber),DeviceNumber);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DeviceNumber));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(PartitionNumber),PartitionNumber);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(PartitionNumber));
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

HRESULT STORAGE_DEVICE_NUMBER_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT STORAGE_DEVICE_NUMBER_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT STORAGE_HOTPLUG_INFO_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(STORAGE_HOTPLUG_INFO_Xml), _countof(MethodString(STORAGE_HOTPLUG_INFO_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(Size),Size);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(Size));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MediaRemovable),MediaRemovable);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MediaRemovable));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MediaHotplug),MediaHotplug);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MediaHotplug));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DeviceHotplug),DeviceHotplug);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DeviceHotplug));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(WriteCacheEnableOverride),WriteCacheEnableOverride);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(WriteCacheEnableOverride));
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

HRESULT STORAGE_HOTPLUG_INFO_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT STORAGE_HOTPLUG_INFO_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT DEVICE_MEDIA_INFO_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(DEVICE_MEDIA_INFO_Xml), _countof(MethodString(DEVICE_MEDIA_INFO_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DiskInfo.Cylinders),DiskInfo.Cylinders);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DiskInfo.Cylinders));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DiskInfo.MediaType),DiskInfo.MediaType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DiskInfo.MediaType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DiskInfo.TracksPerCylinder),DiskInfo.TracksPerCylinder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DiskInfo.TracksPerCylinder));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DiskInfo.SectorsPerTrack),DiskInfo.SectorsPerTrack);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DiskInfo.SectorsPerTrack));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DiskInfo.BytesPerSector),DiskInfo.BytesPerSector);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DiskInfo.BytesPerSector));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DiskInfo.NumberMediaSides),DiskInfo.NumberMediaSides);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DiskInfo.NumberMediaSides));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DiskInfo.MediaCharacteristics),DiskInfo.MediaCharacteristics);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DiskInfo.MediaCharacteristics));
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

HRESULT DEVICE_MEDIA_INFO_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT DEVICE_MEDIA_INFO_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT GET_MEDIA_TYPES_Xml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(GET_MEDIA_TYPES_Xml), _countof(MethodString(GET_MEDIA_TYPES_Xml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(DeviceType),DeviceType);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(DeviceType));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(MediaInfoCount),MediaInfoCount);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(MediaInfoCount));
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(MediaInfo, FIELD_NAME(MediaInfo), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(MediaInfo));
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

HRESULT GET_MEDIA_TYPES_Xml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT GET_MEDIA_TYPES_Xml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT CSingleDiskInfoXml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(CSingleDiskInfoXml), _countof(MethodString(CSingleDiskInfoXml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteNameValue(FIELD_NAME(m_DiskNumber),m_DiskNumber);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, TEXT("%s"), FIELD_NAME(m_DiskNumber));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_pDiskGeometryEx));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_pDiskGeometryEx.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_pDriveLayoutInformationEx));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_pDriveLayoutInformationEx.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_PartitionInformationEx));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_PartitionInformationEx.WriteTo(pFileDevice);
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
	hr = nvs.WriteStructHeader(FIELD_NAME(m_pStorageDeviceDescriptor));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_pStorageDeviceDescriptor.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_pStorageAdapterDescriptor));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_pStorageAdapterDescriptor.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_pStorageDeviceIDDescriptor));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_pStorageDeviceIDDescriptor.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_pStorageDeviceUniqueIDDescriptor));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_pStorageDeviceUniqueIDDescriptor.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_StorageDeviceNumber));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_StorageDeviceNumber.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_StorageHotplugInfo));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_StorageHotplugInfo.WriteTo(pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteTo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = nvs.WriteStructHeader(FIELD_NAME(m_pMEDIA_TYPES));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteStructHeader"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = m_pMEDIA_TYPES.WriteTo(pFileDevice);
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

HRESULT CSingleDiskInfoXml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT CSingleDiskInfoXml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////
HRESULT CAllDiskInfoXml::WriteTo( IFileDevice * pFileDevice ) CONST
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
	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString(CAllDiskInfoXml), _countof(MethodString(CAllDiskInfoXml)));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteNameValue"), hr, METHOD_META_NAME);
		LEAVE_BLOCK(0);
	}
	hr = WriteComplexVector(m_pDiskInfos, FIELD_NAME(m_pDiskInfos), pFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_pDiskInfos));
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

HRESULT CAllDiskInfoXml::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST
{
	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);
}

HRESULT CAllDiskInfoXml::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST
{
	return WRITE_TO_FILE(pFileName, WrittenLength, this);
}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT DISK_GEOMETRY_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(DISK_GEOMETRY_Xml)));
		}
		else if (IS_FIELD_NAME(Cylinders, Name))
		{
			ATLASSERT(sizeof(Cylinders) == ValueSize);
			hr = nvs.ReadValueData(Cylinders);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MediaType, Name))
		{
			ATLASSERT(sizeof(MediaType) == ValueSize);
			hr = nvs.ReadValueData(MediaType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(TracksPerCylinder, Name))
		{
			ATLASSERT(sizeof(TracksPerCylinder) == ValueSize);
			hr = nvs.ReadValueData(TracksPerCylinder);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(SectorsPerTrack, Name))
		{
			ATLASSERT(sizeof(SectorsPerTrack) == ValueSize);
			hr = nvs.ReadValueData(SectorsPerTrack);
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

HRESULT DISK_GEOMETRY_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT DISK_GEOMETRY_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT DISK_GEOMETRY_EX_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(DISK_GEOMETRY_EX_Xml)));
		}
		else if (Name == FIELD_NAME( Geometry) && ValueSize == -1)
		{
			hr = Geometry.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), Geometry);
				break;
			}
		}
		else if (IS_FIELD_NAME(DiskSize, Name))
		{
			ATLASSERT(sizeof(DiskSize) == ValueSize);
			hr = nvs.ReadValueData(DiskSize);
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

HRESULT DISK_GEOMETRY_EX_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT DISK_GEOMETRY_EX_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT DRIVE_LAYOUT_INFORMATION_GPT_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(DRIVE_LAYOUT_INFORMATION_GPT_Xml)));
		}
		else if (IS_FIELD_NAME(DiskId, Name))
		{
			ATLASSERT(sizeof(DiskId) == ValueSize);
			hr = nvs.ReadValueData(DiskId);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(StartingUsableOffset, Name))
		{
			ATLASSERT(sizeof(StartingUsableOffset) == ValueSize);
			hr = nvs.ReadValueData(StartingUsableOffset);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(UsableLength, Name))
		{
			ATLASSERT(sizeof(UsableLength) == ValueSize);
			hr = nvs.ReadValueData(UsableLength);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MaxPartitionCount, Name))
		{
			ATLASSERT(sizeof(MaxPartitionCount) == ValueSize);
			hr = nvs.ReadValueData(MaxPartitionCount);
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

HRESULT DRIVE_LAYOUT_INFORMATION_GPT_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT DRIVE_LAYOUT_INFORMATION_GPT_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT PARTITION_INFORMATION_GPT_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(PARTITION_INFORMATION_GPT_Xml)));
		}
		else if (IS_FIELD_NAME(PartitionType, Name))
		{
			ATLASSERT(sizeof(PartitionType) == ValueSize);
			hr = nvs.ReadValueData(PartitionType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(PartitionId, Name))
		{
			ATLASSERT(sizeof(PartitionId) == ValueSize);
			hr = nvs.ReadValueData(PartitionId);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(Attributes, Name))
		{
			ATLASSERT(sizeof(Attributes) == ValueSize);
			hr = nvs.ReadValueData(Attributes);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(Name, Name))
		{
			ATLASSERT(sizeof(Name) == ValueSize);
			hr = nvs.ReadValueData(Name);
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

HRESULT PARTITION_INFORMATION_GPT_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT PARTITION_INFORMATION_GPT_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT PARTITION_INFORMATION_EX_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(PARTITION_INFORMATION_EX_Xml)));
		}
		else if (IS_FIELD_NAME(PartitionStyle, Name))
		{
			ATLASSERT(sizeof(PartitionStyle) == ValueSize);
			hr = nvs.ReadValueData(PartitionStyle);
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
		else if (IS_FIELD_NAME(PartitionLength, Name))
		{
			ATLASSERT(sizeof(PartitionLength) == ValueSize);
			hr = nvs.ReadValueData(PartitionLength);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(PartitionNumber, Name))
		{
			ATLASSERT(sizeof(PartitionNumber) == ValueSize);
			hr = nvs.ReadValueData(PartitionNumber);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(RewritePartition, Name))
		{
			ATLASSERT(sizeof(RewritePartition) == ValueSize);
			hr = nvs.ReadValueData(RewritePartition);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (Name == FIELD_NAME( Gpt) && ValueSize == -1)
		{
			hr = Gpt.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), Gpt);
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

HRESULT PARTITION_INFORMATION_EX_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT PARTITION_INFORMATION_EX_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT DRIVE_LAYOUT_INFORMATION_EX_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(DRIVE_LAYOUT_INFORMATION_EX_Xml)));
		}
		else if (IS_FIELD_NAME(PartitionStyle, Name))
		{
			ATLASSERT(sizeof(PartitionStyle) == ValueSize);
			hr = nvs.ReadValueData(PartitionStyle);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(PartitionCount, Name))
		{
			ATLASSERT(sizeof(PartitionCount) == ValueSize);
			hr = nvs.ReadValueData(PartitionCount);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (Name == FIELD_NAME( Gpt) && ValueSize == -1)
		{
			hr = Gpt.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), Gpt);
				break;
			}
		}
		else if (IS_FIELD_NAME(PartitionEntry, Name))
		{
			hr = ReadComplexVector(PartitionEntry, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(PartitionEntry));
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

HRESULT DRIVE_LAYOUT_INFORMATION_EX_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT DRIVE_LAYOUT_INFORMATION_EX_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT STORAGE_DEVICE_DESCRIPTOR_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(STORAGE_DEVICE_DESCRIPTOR_Xml)));
		}
		else if (IS_FIELD_NAME(Version, Name))
		{
			ATLASSERT(sizeof(Version) == ValueSize);
			hr = nvs.ReadValueData(Version);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(Size, Name))
		{
			ATLASSERT(sizeof(Size) == ValueSize);
			hr = nvs.ReadValueData(Size);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DeviceType, Name))
		{
			ATLASSERT(sizeof(DeviceType) == ValueSize);
			hr = nvs.ReadValueData(DeviceType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DeviceTypeModifier, Name))
		{
			ATLASSERT(sizeof(DeviceTypeModifier) == ValueSize);
			hr = nvs.ReadValueData(DeviceTypeModifier);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(RemovableMedia, Name))
		{
			ATLASSERT(sizeof(RemovableMedia) == ValueSize);
			hr = nvs.ReadValueData(RemovableMedia);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(CommandQueueing, Name))
		{
			ATLASSERT(sizeof(CommandQueueing) == ValueSize);
			hr = nvs.ReadValueData(CommandQueueing);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(VendorIdOffset, Name))
		{
			ATLASSERT(sizeof(VendorIdOffset) == ValueSize);
			hr = nvs.ReadValueData(VendorIdOffset);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ProductIdOffset, Name))
		{
			ATLASSERT(sizeof(ProductIdOffset) == ValueSize);
			hr = nvs.ReadValueData(ProductIdOffset);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(ProductRevisionOffset, Name))
		{
			ATLASSERT(sizeof(ProductRevisionOffset) == ValueSize);
			hr = nvs.ReadValueData(ProductRevisionOffset);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(SerialNumberOffset, Name))
		{
			ATLASSERT(sizeof(SerialNumberOffset) == ValueSize);
			hr = nvs.ReadValueData(SerialNumberOffset);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(BusType, Name))
		{
			ATLASSERT(sizeof(BusType) == ValueSize);
			hr = nvs.ReadValueData(BusType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(RawPropertiesLength, Name))
		{
			ATLASSERT(sizeof(RawPropertiesLength) == ValueSize);
			hr = nvs.ReadValueData(RawPropertiesLength);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(RawDeviceProperties, Name))
		{
			ATLASSERT(sizeof(RawDeviceProperties) == ValueSize);
			hr = nvs.ReadValueData(RawDeviceProperties);
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

HRESULT STORAGE_DEVICE_DESCRIPTOR_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT STORAGE_DEVICE_DESCRIPTOR_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT STORAGE_ADAPTER_DESCRIPTOR_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(STORAGE_ADAPTER_DESCRIPTOR_Xml)));
		}
		else if (IS_FIELD_NAME(Version, Name))
		{
			ATLASSERT(sizeof(Version) == ValueSize);
			hr = nvs.ReadValueData(Version);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(Size, Name))
		{
			ATLASSERT(sizeof(Size) == ValueSize);
			hr = nvs.ReadValueData(Size);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MaximumTransferLength, Name))
		{
			ATLASSERT(sizeof(MaximumTransferLength) == ValueSize);
			hr = nvs.ReadValueData(MaximumTransferLength);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MaximumPhysicalPages, Name))
		{
			ATLASSERT(sizeof(MaximumPhysicalPages) == ValueSize);
			hr = nvs.ReadValueData(MaximumPhysicalPages);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(AlignmentMask, Name))
		{
			ATLASSERT(sizeof(AlignmentMask) == ValueSize);
			hr = nvs.ReadValueData(AlignmentMask);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(AdapterUsesPio, Name))
		{
			ATLASSERT(sizeof(AdapterUsesPio) == ValueSize);
			hr = nvs.ReadValueData(AdapterUsesPio);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(AdapterScansDown, Name))
		{
			ATLASSERT(sizeof(AdapterScansDown) == ValueSize);
			hr = nvs.ReadValueData(AdapterScansDown);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(CommandQueueing, Name))
		{
			ATLASSERT(sizeof(CommandQueueing) == ValueSize);
			hr = nvs.ReadValueData(CommandQueueing);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(AcceleratedTransfer, Name))
		{
			ATLASSERT(sizeof(AcceleratedTransfer) == ValueSize);
			hr = nvs.ReadValueData(AcceleratedTransfer);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(BusType, Name))
		{
			ATLASSERT(sizeof(BusType) == ValueSize);
			hr = nvs.ReadValueData(BusType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(BusType, Name))
		{
			ATLASSERT(sizeof(BusType) == ValueSize);
			hr = nvs.ReadValueData(BusType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(BusMajorVersion, Name))
		{
			ATLASSERT(sizeof(BusMajorVersion) == ValueSize);
			hr = nvs.ReadValueData(BusMajorVersion);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(BusMinorVersion, Name))
		{
			ATLASSERT(sizeof(BusMinorVersion) == ValueSize);
			hr = nvs.ReadValueData(BusMinorVersion);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(SrbType, Name))
		{
			ATLASSERT(sizeof(SrbType) == ValueSize);
			hr = nvs.ReadValueData(SrbType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(AddressType, Name))
		{
			ATLASSERT(sizeof(AddressType) == ValueSize);
			hr = nvs.ReadValueData(AddressType);
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

HRESULT STORAGE_ADAPTER_DESCRIPTOR_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT STORAGE_ADAPTER_DESCRIPTOR_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT STORAGE_DEVICE_ID_DESCRIPTOR_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(STORAGE_DEVICE_ID_DESCRIPTOR_Xml)));
		}
		else if (IS_FIELD_NAME(Version, Name))
		{
			ATLASSERT(sizeof(Version) == ValueSize);
			hr = nvs.ReadValueData(Version);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(Size, Name))
		{
			ATLASSERT(sizeof(Size) == ValueSize);
			hr = nvs.ReadValueData(Size);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(NumberOfIdentifiers, Name))
		{
			ATLASSERT(sizeof(NumberOfIdentifiers) == ValueSize);
			hr = nvs.ReadValueData(NumberOfIdentifiers);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(Identifiers, Name))
		{
			ATLASSERT(sizeof(Identifiers) == ValueSize);
			hr = nvs.ReadValueData(Identifiers);
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

HRESULT STORAGE_DEVICE_ID_DESCRIPTOR_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT STORAGE_DEVICE_ID_DESCRIPTOR_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT STORAGE_DEVICE_NUMBER_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(STORAGE_DEVICE_NUMBER_Xml)));
		}
		else if (IS_FIELD_NAME(DeviceType, Name))
		{
			ATLASSERT(sizeof(DeviceType) == ValueSize);
			hr = nvs.ReadValueData(DeviceType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DeviceNumber, Name))
		{
			ATLASSERT(sizeof(DeviceNumber) == ValueSize);
			hr = nvs.ReadValueData(DeviceNumber);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(PartitionNumber, Name))
		{
			ATLASSERT(sizeof(PartitionNumber) == ValueSize);
			hr = nvs.ReadValueData(PartitionNumber);
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

HRESULT STORAGE_DEVICE_NUMBER_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT STORAGE_DEVICE_NUMBER_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT STORAGE_HOTPLUG_INFO_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(STORAGE_HOTPLUG_INFO_Xml)));
		}
		else if (IS_FIELD_NAME(Size, Name))
		{
			ATLASSERT(sizeof(Size) == ValueSize);
			hr = nvs.ReadValueData(Size);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MediaRemovable, Name))
		{
			ATLASSERT(sizeof(MediaRemovable) == ValueSize);
			hr = nvs.ReadValueData(MediaRemovable);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MediaHotplug, Name))
		{
			ATLASSERT(sizeof(MediaHotplug) == ValueSize);
			hr = nvs.ReadValueData(MediaHotplug);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DeviceHotplug, Name))
		{
			ATLASSERT(sizeof(DeviceHotplug) == ValueSize);
			hr = nvs.ReadValueData(DeviceHotplug);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(WriteCacheEnableOverride, Name))
		{
			ATLASSERT(sizeof(WriteCacheEnableOverride) == ValueSize);
			hr = nvs.ReadValueData(WriteCacheEnableOverride);
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

HRESULT STORAGE_HOTPLUG_INFO_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT STORAGE_HOTPLUG_INFO_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT DEVICE_MEDIA_INFO_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(DEVICE_MEDIA_INFO_Xml)));
		}
		else if (IS_FIELD_NAME(DiskInfo.Cylinders, Name))
		{
			ATLASSERT(sizeof(DiskInfo.Cylinders) == ValueSize);
			hr = nvs.ReadValueData(DiskInfo.Cylinders);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DiskInfo.MediaType, Name))
		{
			ATLASSERT(sizeof(DiskInfo.MediaType) == ValueSize);
			hr = nvs.ReadValueData(DiskInfo.MediaType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DiskInfo.TracksPerCylinder, Name))
		{
			ATLASSERT(sizeof(DiskInfo.TracksPerCylinder) == ValueSize);
			hr = nvs.ReadValueData(DiskInfo.TracksPerCylinder);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DiskInfo.SectorsPerTrack, Name))
		{
			ATLASSERT(sizeof(DiskInfo.SectorsPerTrack) == ValueSize);
			hr = nvs.ReadValueData(DiskInfo.SectorsPerTrack);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DiskInfo.BytesPerSector, Name))
		{
			ATLASSERT(sizeof(DiskInfo.BytesPerSector) == ValueSize);
			hr = nvs.ReadValueData(DiskInfo.BytesPerSector);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DiskInfo.NumberMediaSides, Name))
		{
			ATLASSERT(sizeof(DiskInfo.NumberMediaSides) == ValueSize);
			hr = nvs.ReadValueData(DiskInfo.NumberMediaSides);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(DiskInfo.MediaCharacteristics, Name))
		{
			ATLASSERT(sizeof(DiskInfo.MediaCharacteristics) == ValueSize);
			hr = nvs.ReadValueData(DiskInfo.MediaCharacteristics);
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

HRESULT DEVICE_MEDIA_INFO_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT DEVICE_MEDIA_INFO_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT GET_MEDIA_TYPES_Xml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(GET_MEDIA_TYPES_Xml)));
		}
		else if (IS_FIELD_NAME(DeviceType, Name))
		{
			ATLASSERT(sizeof(DeviceType) == ValueSize);
			hr = nvs.ReadValueData(DeviceType);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MediaInfoCount, Name))
		{
			ATLASSERT(sizeof(MediaInfoCount) == ValueSize);
			hr = nvs.ReadValueData(MediaInfoCount);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (IS_FIELD_NAME(MediaInfo, Name))
		{
			hr = ReadComplexVector(MediaInfo, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(MediaInfo));
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

HRESULT GET_MEDIA_TYPES_Xml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT GET_MEDIA_TYPES_Xml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT CSingleDiskInfoXml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(CSingleDiskInfoXml)));
		}
		else if (IS_FIELD_NAME(m_DiskNumber, Name))
		{
			ATLASSERT(sizeof(m_DiskNumber) == ValueSize);
			hr = nvs.ReadValueData(m_DiskNumber);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
				break;
			}
		}
		else if (Name == FIELD_NAME( m_pDiskGeometryEx) && ValueSize == -1)
		{
			hr = m_pDiskGeometryEx.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_pDiskGeometryEx);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_pDriveLayoutInformationEx) && ValueSize == -1)
		{
			hr = m_pDriveLayoutInformationEx.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_pDriveLayoutInformationEx);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_PartitionInformationEx) && ValueSize == -1)
		{
			hr = m_PartitionInformationEx.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_PartitionInformationEx);
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
		else if (Name == FIELD_NAME( m_pStorageDeviceDescriptor) && ValueSize == -1)
		{
			hr = m_pStorageDeviceDescriptor.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_pStorageDeviceDescriptor);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_pStorageAdapterDescriptor) && ValueSize == -1)
		{
			hr = m_pStorageAdapterDescriptor.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_pStorageAdapterDescriptor);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_pStorageDeviceIDDescriptor) && ValueSize == -1)
		{
			hr = m_pStorageDeviceIDDescriptor.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_pStorageDeviceIDDescriptor);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_pStorageDeviceUniqueIDDescriptor) && ValueSize == -1)
		{
			hr = m_pStorageDeviceUniqueIDDescriptor.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_pStorageDeviceUniqueIDDescriptor);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_StorageDeviceNumber) && ValueSize == -1)
		{
			hr = m_StorageDeviceNumber.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_StorageDeviceNumber);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_StorageHotplugInfo) && ValueSize == -1)
		{
			hr = m_StorageHotplugInfo.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_StorageHotplugInfo);
				break;
			}
		}
		else if (Name == FIELD_NAME( m_pMEDIA_TYPES) && ValueSize == -1)
		{
			hr = m_pMEDIA_TYPES.ReadFrom(pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFrom"), hr, TEXT("FieldName=%s"), m_pMEDIA_TYPES);
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

HRESULT CSingleDiskInfoXml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT CSingleDiskInfoXml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
HRESULT CAllDiskInfoXml::ReadFrom( IFileDevice * pFileDevice )
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
			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString(CAllDiskInfoXml)));
		}
		else if (IS_FIELD_NAME(m_pDiskInfos, Name))
		{
			hr = ReadComplexVector(m_pDiskInfos, Name, ValueSize, pFileDevice);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("WriteComplexVector"), hr, TEXT("%s"), FIELD_NAME(m_pDiskInfos));
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

HRESULT CAllDiskInfoXml::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )
{
	return READ_FROM_BUFFER(pBuffer, Length, this);
}

HRESULT CAllDiskInfoXml::ReadFromFile(LPCTSTR pFileName)
{
	return READ_FROM_FILE(pFileName, this);
}
////////////////////////////RRRRRR END//////////////////////////////////////////////

