////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID VDS_VOLUME_PLEX_PROP_Xml::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(id), id, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(type), type, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(status), status, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(health), health, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(TransitionState), TransitionState, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ullSize), ullSize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ulStripeSize), ulStripeSize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ulNumberOfMembers), ulNumberOfMembers, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID VDS_DISK_EXTENT_Xml::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(diskId), diskId, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(type), type, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ullOffset), ullOffset, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ullSize), ullSize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(volumeId), volumeId, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(plexId), plexId, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(memberIdx), memberIdx, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID CSingleVdsVolumePlexXml::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	m_VDSVolumePlexProp.ToElement(FIELD_NAME(m_VDSVolumePlexProp), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(m_VDSDiskExtents), m_VDSDiskExtents, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID MY_VDS_VOLUME_PROP_Xml::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_id), m_id, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_type), m_type, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_status), m_status, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_health), m_health, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_TransitionState), m_TransitionState, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_ullSize), m_ullSize, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_ulFlags), m_ulFlags, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_RecommendedFileSystemType), m_RecommendedFileSystemType, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_strName), m_strName, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID CSingleVdsVolumeInfoXml::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	m_MyVDSVolumeProp.ToElement(FIELD_NAME(m_MyVDSVolumeProp), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(m_strVolumeGUID), m_strVolumeGUID, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(m_VolumePlexes), m_VolumePlexes, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID VDS_VOLUME_PLEX_PROP_Xml::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(id)))
		{
			ElementToValue(FIELD_NAME(id), id, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(type)))
		{
			ElementToValue(FIELD_NAME(type), type, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(status)))
		{
			ElementToValue(FIELD_NAME(status), status, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(health)))
		{
			ElementToValue(FIELD_NAME(health), health, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(TransitionState)))
		{
			ElementToValue(FIELD_NAME(TransitionState), TransitionState, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ullSize)))
		{
			ElementToValue(FIELD_NAME(ullSize), ullSize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ulStripeSize)))
		{
			ElementToValue(FIELD_NAME(ulStripeSize), ulStripeSize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ulNumberOfMembers)))
		{
			ElementToValue(FIELD_NAME(ulNumberOfMembers), ulNumberOfMembers, Child_jfs654);
		}
	}
}
VOID VDS_VOLUME_PLEX_PROP_Xml::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT VDS_VOLUME_PLEX_PROP_Xml::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT VDS_VOLUME_PLEX_PROP_Xml::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT VDS_VOLUME_PLEX_PROP_Xml::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT VDS_VOLUME_PLEX_PROP_Xml::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT VDS_VOLUME_PLEX_PROP_Xml::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID VDS_DISK_EXTENT_Xml::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(diskId)))
		{
			ElementToValue(FIELD_NAME(diskId), diskId, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(type)))
		{
			ElementToValue(FIELD_NAME(type), type, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ullOffset)))
		{
			ElementToValue(FIELD_NAME(ullOffset), ullOffset, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ullSize)))
		{
			ElementToValue(FIELD_NAME(ullSize), ullSize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(volumeId)))
		{
			ElementToValue(FIELD_NAME(volumeId), volumeId, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(plexId)))
		{
			ElementToValue(FIELD_NAME(plexId), plexId, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(memberIdx)))
		{
			ElementToValue(FIELD_NAME(memberIdx), memberIdx, Child_jfs654);
		}
	}
}
VOID VDS_DISK_EXTENT_Xml::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT VDS_DISK_EXTENT_Xml::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT VDS_DISK_EXTENT_Xml::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT VDS_DISK_EXTENT_Xml::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT VDS_DISK_EXTENT_Xml::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT VDS_DISK_EXTENT_Xml::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID CSingleVdsVolumePlexXml::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_VDSVolumePlexProp)))
		{
			m_VDSVolumePlexProp.FromElement(FIELD_NAME(m_VDSVolumePlexProp), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_VDSDiskExtents)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(m_VDSDiskExtents), m_VDSDiskExtents, Child_jfs654);
		}
	}
}
VOID CSingleVdsVolumePlexXml::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT CSingleVdsVolumePlexXml::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT CSingleVdsVolumePlexXml::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT CSingleVdsVolumePlexXml::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT CSingleVdsVolumePlexXml::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT CSingleVdsVolumePlexXml::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID MY_VDS_VOLUME_PROP_Xml::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
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
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_type)))
		{
			ElementToValue(FIELD_NAME(m_type), m_type, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_status)))
		{
			ElementToValue(FIELD_NAME(m_status), m_status, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_health)))
		{
			ElementToValue(FIELD_NAME(m_health), m_health, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_TransitionState)))
		{
			ElementToValue(FIELD_NAME(m_TransitionState), m_TransitionState, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_ullSize)))
		{
			ElementToValue(FIELD_NAME(m_ullSize), m_ullSize, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_ulFlags)))
		{
			ElementToValue(FIELD_NAME(m_ulFlags), m_ulFlags, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_RecommendedFileSystemType)))
		{
			ElementToValue(FIELD_NAME(m_RecommendedFileSystemType), m_RecommendedFileSystemType, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_strName)))
		{
			ElementToValue(FIELD_NAME(m_strName), m_strName, Child_jfs654);
		}
	}
}
VOID MY_VDS_VOLUME_PROP_Xml::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT MY_VDS_VOLUME_PROP_Xml::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT MY_VDS_VOLUME_PROP_Xml::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT MY_VDS_VOLUME_PROP_Xml::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT MY_VDS_VOLUME_PROP_Xml::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT MY_VDS_VOLUME_PROP_Xml::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID CSingleVdsVolumeInfoXml::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_MyVDSVolumeProp)))
		{
			m_MyVDSVolumeProp.FromElement(FIELD_NAME(m_MyVDSVolumeProp), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_strVolumeGUID)))
		{
			ElementToValue(FIELD_NAME(m_strVolumeGUID), m_strVolumeGUID, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(m_VolumePlexes)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(m_VolumePlexes), m_VolumePlexes, Child_jfs654);
		}
	}
}
VOID CSingleVdsVolumeInfoXml::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT CSingleVdsVolumeInfoXml::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT CSingleVdsVolumeInfoXml::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT CSingleVdsVolumeInfoXml::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT CSingleVdsVolumeInfoXml::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT CSingleVdsVolumeInfoXml::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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

