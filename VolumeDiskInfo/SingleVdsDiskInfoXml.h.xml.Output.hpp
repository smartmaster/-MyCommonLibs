////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID MY_VDS_DISK_PROP_Xml::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_id), m_id, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_status), m_status, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_ReserveMode), m_ReserveMode, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_health), m_health, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_dwDeviceType), m_dwDeviceType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_dwMediaType), m_dwMediaType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_ullSize), m_ullSize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_ulBytesPerSector), m_ulBytesPerSector, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_ulSectorsPerTrack), m_ulSectorsPerTrack, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_ulTracksPerCylinder), m_ulTracksPerCylinder, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_ulFlags), m_ulFlags, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_BusType), m_BusType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_PartitionStyle), m_PartitionStyle, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_DiskGuid), m_DiskGuid, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_strDiskAddress), m_strDiskAddress, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_strName), m_strName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_strFriendlyName), m_strFriendlyName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_strAdaptorName), m_strAdaptorName, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_strDevicePath), m_strDevicePath, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID CSingleVdsDiskInfoXml::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	m_MyVDSDiskProp.ToElement(FIELD_NAME(m_MyVDSDiskProp), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(m_VDSDiskExtents), m_VDSDiskExtents, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID MY_VDS_DISK_PROP_Xml::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_id)))
		{
			ElementToValue(FIELD_NAME(m_id), m_id, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_status)))
		{
			ElementToValue(FIELD_NAME(m_status), m_status, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_ReserveMode)))
		{
			ElementToValue(FIELD_NAME(m_ReserveMode), m_ReserveMode, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_health)))
		{
			ElementToValue(FIELD_NAME(m_health), m_health, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_dwDeviceType)))
		{
			ElementToValue(FIELD_NAME(m_dwDeviceType), m_dwDeviceType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_dwMediaType)))
		{
			ElementToValue(FIELD_NAME(m_dwMediaType), m_dwMediaType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_ullSize)))
		{
			ElementToValue(FIELD_NAME(m_ullSize), m_ullSize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_ulBytesPerSector)))
		{
			ElementToValue(FIELD_NAME(m_ulBytesPerSector), m_ulBytesPerSector, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_ulSectorsPerTrack)))
		{
			ElementToValue(FIELD_NAME(m_ulSectorsPerTrack), m_ulSectorsPerTrack, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_ulTracksPerCylinder)))
		{
			ElementToValue(FIELD_NAME(m_ulTracksPerCylinder), m_ulTracksPerCylinder, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_ulFlags)))
		{
			ElementToValue(FIELD_NAME(m_ulFlags), m_ulFlags, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_BusType)))
		{
			ElementToValue(FIELD_NAME(m_BusType), m_BusType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_PartitionStyle)))
		{
			ElementToValue(FIELD_NAME(m_PartitionStyle), m_PartitionStyle, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_DiskGuid)))
		{
			ElementToValue(FIELD_NAME(m_DiskGuid), m_DiskGuid, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_strDiskAddress)))
		{
			ElementToValue(FIELD_NAME(m_strDiskAddress), m_strDiskAddress, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_strName)))
		{
			ElementToValue(FIELD_NAME(m_strName), m_strName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_strFriendlyName)))
		{
			ElementToValue(FIELD_NAME(m_strFriendlyName), m_strFriendlyName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_strAdaptorName)))
		{
			ElementToValue(FIELD_NAME(m_strAdaptorName), m_strAdaptorName, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_strDevicePath)))
		{
			ElementToValue(FIELD_NAME(m_strDevicePath), m_strDevicePath, Child_jfs654);
		}
	}
}
VOID MY_VDS_DISK_PROP_Xml::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT MY_VDS_DISK_PROP_Xml::FromString( LPCTSTR RootName, CONST CString & strXml )
{
	XmlElement xmlElement;
	INT Start = 0;
	HRESULT hr = S_OK;
	LONG xml_ele_count = xmlElement.FromString(strXml, Start);
	if (xml_ele_count > 0)
	{
		FromElement(RootName, xmlElement);
	}
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);
	}
	return hr;
}

HRESULT MY_VDS_DISK_PROP_Xml::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CString strXml;
	ToString(RootName, strXml);
	static CONST BYTE UTF16_TAG[] = {0xff, 0xfe};
	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT MY_VDS_DISK_PROP_Xml::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	LONGLONG FileSize = 0;
	hr = pIFileDevice->GetFileSize(&FileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CString strXML;
	INT CharCount = (INT)FileSize / sizeof(TCHAR) + 2;
	LPTSTR pBuffer = strXML.GetBuffer(CharCount);
	pBuffer[CharCount - 1] = 0;
	pBuffer[CharCount - 2] = 0;
	ULONG NumberOfBytesRead = 0;
	hr = pIFileDevice->Read((ULONG)FileSize, &NumberOfBytesRead, (LPBYTE)pBuffer);
	strXML.ReleaseBuffer();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = FromString(RootName, strXML);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromString"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT MY_VDS_DISK_PROP_Xml::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = ToStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ToStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT MY_VDS_DISK_PROP_Xml::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = FromStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

////////////////////////////RRRRRR END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID CSingleVdsDiskInfoXml::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_MyVDSDiskProp)))
		{
			m_MyVDSDiskProp.FromElement(FIELD_NAME(m_MyVDSDiskProp), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_VDSDiskExtents)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(m_VDSDiskExtents), m_VDSDiskExtents, Child_jfs654);
		}
	}
}
VOID CSingleVdsDiskInfoXml::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT CSingleVdsDiskInfoXml::FromString( LPCTSTR RootName, CONST CString & strXml )
{
	XmlElement xmlElement;
	INT Start = 0;
	HRESULT hr = S_OK;
	LONG xml_ele_count = xmlElement.FromString(strXml, Start);
	if (xml_ele_count > 0)
	{
		FromElement(RootName, xmlElement);
	}
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);
	}
	return hr;
}

HRESULT CSingleVdsDiskInfoXml::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
{
	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);
	CString strXml;
	ToString(RootName, strXml);
	static CONST BYTE UTF16_TAG[] = {0xff, 0xfe};
	ULONG NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	NumberOfBytesWritten = 0;
	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT CSingleVdsDiskInfoXml::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	LONGLONG FileSize = 0;
	hr = pIFileDevice->GetFileSize(&FileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CString strXML;
	INT CharCount = (INT)FileSize / sizeof(TCHAR) + 2;
	LPTSTR pBuffer = strXML.GetBuffer(CharCount);
	pBuffer[CharCount - 1] = 0;
	pBuffer[CharCount - 2] = 0;
	ULONG NumberOfBytesRead = 0;
	hr = pIFileDevice->Read((ULONG)FileSize, &NumberOfBytesRead, (LPBYTE)pBuffer);
	strXML.ReleaseBuffer();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hr = FromString(RootName, strXML);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromString"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT CSingleVdsDiskInfoXml::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = ToStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ToStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

HRESULT CSingleVdsDiskInfoXml::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	BEGIN_BLOCK(0);
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	hr = FromStream(RootName, spIFileDevice);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("FromStream"), hr, TEXT("[%s]"), pFileName);
		LEAVE_BLOCK(0);
	}
	END_BLOCK(0);
	return hr;
}

////////////////////////////RRRRRR END//////////////////////////////////////////////

