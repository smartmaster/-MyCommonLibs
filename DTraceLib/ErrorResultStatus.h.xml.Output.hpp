////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID ErrorElement::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(hr), hr, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Message), Message, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LocallizedMessage), LocallizedMessage, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(HelpLink), HelpLink, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LocalizedHelpLink), LocalizedHelpLink, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Pid), Pid, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Tid), Tid, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Module), Module, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(File), File, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Line), Line, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Enum), Enum, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(Flags), Flags, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(MoreData), MoreData, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(MoreString), MoreString, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID ErrorCollection::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	CurrentError.ToElement(FIELD_NAME(CurrentError), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(LastErrors), LastErrors, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID ErrorElement::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(hr)))
		{
			ElementToValue(FIELD_NAME(hr), hr, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Message)))
		{
			ElementToValue(FIELD_NAME(Message), Message, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LocallizedMessage)))
		{
			ElementToValue(FIELD_NAME(LocallizedMessage), LocallizedMessage, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(HelpLink)))
		{
			ElementToValue(FIELD_NAME(HelpLink), HelpLink, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LocalizedHelpLink)))
		{
			ElementToValue(FIELD_NAME(LocalizedHelpLink), LocalizedHelpLink, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Pid)))
		{
			ElementToValue(FIELD_NAME(Pid), Pid, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Tid)))
		{
			ElementToValue(FIELD_NAME(Tid), Tid, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Module)))
		{
			ElementToValue(FIELD_NAME(Module), Module, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(File)))
		{
			ElementToValue(FIELD_NAME(File), File, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Line)))
		{
			ElementToValue(FIELD_NAME(Line), Line, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Enum)))
		{
			ElementToValue(FIELD_NAME(Enum), Enum, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(Flags)))
		{
			ElementToValue(FIELD_NAME(Flags), Flags, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MoreData)))
		{
			ElementToValue(FIELD_NAME(MoreData), MoreData, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(MoreString)))
		{
			ElementToValue(FIELD_NAME(MoreString), MoreString, Child_jfs654);
		}
	}
}
VOID ErrorElement::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT ErrorElement::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT ErrorElement::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT ErrorElement::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT ErrorElement::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT ErrorElement::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID ErrorCollection::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CurrentError)))
		{
			CurrentError.FromElement(FIELD_NAME(CurrentError), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LastErrors)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(LastErrors), LastErrors, Child_jfs654);
		}
	}
}
VOID ErrorCollection::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT ErrorCollection::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT ErrorCollection::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT ErrorCollection::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT ErrorCollection::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT ErrorCollection::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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

