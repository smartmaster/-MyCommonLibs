////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID tsimple::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(bool_Value), bool_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(bool_Value1), bool_Value1, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CString_Value), CString_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(wstring_Value), wstring_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LONGLONG_Value), LONGLONG_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ULONGLONG_Value), ULONGLONG_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(LONG_Value), LONG_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ULONG_Value), ULONG_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CHAR_Value), CHAR_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(UCHAR_Value), UCHAR_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SHORT_Value), SHORT_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(USHORT_Value), USHORT_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(INT_Value), INT_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(UINT_Value), UINT_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(vector_BYTE_Value), vector_BYTE_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(GUID_Value), GUID_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(string_Value), string_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(CStringA_Value), CStringA_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(SYSTEMTIME_Value), SYSTEMTIME_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(DOUBLE_Value), DOUBLE_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(FLOAT_Value), FLOAT_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(BYTE_Value), BYTE_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_bool_Value), vector_bool_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_bool_Value1), vector_bool_Value1, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_CString_Value), vector_CString_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_wstring_Value), vector_wstring_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_LONGLONG_Value), vector_LONGLONG_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_ULONGLONG_Value), vector_ULONGLONG_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_LONG_Value), vector_LONG_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_ULONG_Value), vector_ULONG_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_CHAR_Value), vector_CHAR_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_UCHAR_Value), vector_UCHAR_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_SHORT_Value), vector_SHORT_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_USHORT_Value), vector_USHORT_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_INT_Value), vector_INT_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_UINT_Value), vector_UINT_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_vector_BYTE_Value), vector_vector_BYTE_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_GUID_Value), vector_GUID_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_string_Value), vector_string_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_CStringA_Value), vector_CStringA_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_SYSTEMTIME_Value), vector_SYSTEMTIME_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_DOUBLE_Value), vector_DOUBLE_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_FLOAT_Value), vector_FLOAT_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_basic_BYTE_Value), vector_basic_BYTE_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(FILETIME_Value), FILETIME_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_FILETIME_Value), vector_FILETIME_Value, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID tcomplex::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(tsimple_Value), tsimple_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_tsimple_Value), vector_tsimple_Value, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID tcomplex2::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(tcomplex_Value), tcomplex_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vector_tcomplex_Value), vector_tcomplex_Value, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(v3_tcomplex), v3_tcomplex, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(l3_tcomplex), l3_tcomplex, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(q3_tcomplex), q3_tcomplex, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(q4_tcomplex), q4_tcomplex, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID tsimple::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(bool_Value)))
		{
			ElementToValue(FIELD_NAME(bool_Value), bool_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(bool_Value1)))
		{
			ElementToValue(FIELD_NAME(bool_Value1), bool_Value1, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CString_Value)))
		{
			ElementToValue(FIELD_NAME(CString_Value), CString_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(wstring_Value)))
		{
			ElementToValue(FIELD_NAME(wstring_Value), wstring_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LONGLONG_Value)))
		{
			ElementToValue(FIELD_NAME(LONGLONG_Value), LONGLONG_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ULONGLONG_Value)))
		{
			ElementToValue(FIELD_NAME(ULONGLONG_Value), ULONGLONG_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(LONG_Value)))
		{
			ElementToValue(FIELD_NAME(LONG_Value), LONG_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ULONG_Value)))
		{
			ElementToValue(FIELD_NAME(ULONG_Value), ULONG_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CHAR_Value)))
		{
			ElementToValue(FIELD_NAME(CHAR_Value), CHAR_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(UCHAR_Value)))
		{
			ElementToValue(FIELD_NAME(UCHAR_Value), UCHAR_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SHORT_Value)))
		{
			ElementToValue(FIELD_NAME(SHORT_Value), SHORT_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(USHORT_Value)))
		{
			ElementToValue(FIELD_NAME(USHORT_Value), USHORT_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(INT_Value)))
		{
			ElementToValue(FIELD_NAME(INT_Value), INT_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(UINT_Value)))
		{
			ElementToValue(FIELD_NAME(UINT_Value), UINT_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_BYTE_Value)))
		{
			ElementToValue(FIELD_NAME(vector_BYTE_Value), vector_BYTE_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(GUID_Value)))
		{
			ElementToValue(FIELD_NAME(GUID_Value), GUID_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(string_Value)))
		{
			ElementToValue(FIELD_NAME(string_Value), string_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(CStringA_Value)))
		{
			ElementToValue(FIELD_NAME(CStringA_Value), CStringA_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(SYSTEMTIME_Value)))
		{
			ElementToValue(FIELD_NAME(SYSTEMTIME_Value), SYSTEMTIME_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(DOUBLE_Value)))
		{
			ElementToValue(FIELD_NAME(DOUBLE_Value), DOUBLE_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(FLOAT_Value)))
		{
			ElementToValue(FIELD_NAME(FLOAT_Value), FLOAT_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(BYTE_Value)))
		{
			ElementToValue(FIELD_NAME(BYTE_Value), BYTE_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_bool_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_bool_Value), vector_bool_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_bool_Value1)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_bool_Value1), vector_bool_Value1, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_CString_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_CString_Value), vector_CString_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_wstring_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_wstring_Value), vector_wstring_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_LONGLONG_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_LONGLONG_Value), vector_LONGLONG_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_ULONGLONG_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_ULONGLONG_Value), vector_ULONGLONG_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_LONG_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_LONG_Value), vector_LONG_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_ULONG_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_ULONG_Value), vector_ULONG_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_CHAR_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_CHAR_Value), vector_CHAR_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_UCHAR_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_UCHAR_Value), vector_UCHAR_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_SHORT_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_SHORT_Value), vector_SHORT_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_USHORT_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_USHORT_Value), vector_USHORT_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_INT_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_INT_Value), vector_INT_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_UINT_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_UINT_Value), vector_UINT_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_vector_BYTE_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_vector_BYTE_Value), vector_vector_BYTE_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_GUID_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_GUID_Value), vector_GUID_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_string_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_string_Value), vector_string_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_CStringA_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_CStringA_Value), vector_CStringA_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_SYSTEMTIME_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_SYSTEMTIME_Value), vector_SYSTEMTIME_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_DOUBLE_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_DOUBLE_Value), vector_DOUBLE_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_FLOAT_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_FLOAT_Value), vector_FLOAT_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_basic_BYTE_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_basic_BYTE_Value), vector_basic_BYTE_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(FILETIME_Value)))
		{
			ElementToValue(FIELD_NAME(FILETIME_Value), FILETIME_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_FILETIME_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_FILETIME_Value), vector_FILETIME_Value, Child_jfs654);
		}
	}
}
VOID tsimple::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT tsimple::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT tsimple::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT tsimple::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT tsimple::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT tsimple::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID tcomplex::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(tsimple_Value)))
		{
			ElementToValue(FIELD_NAME(tsimple_Value), tsimple_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_tsimple_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_tsimple_Value), vector_tsimple_Value, Child_jfs654);
		}
	}
}
VOID tcomplex::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT tcomplex::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT tcomplex::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT tcomplex::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT tcomplex::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT tcomplex::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID tcomplex2::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(tcomplex_Value)))
		{
			ElementToValue(FIELD_NAME(tcomplex_Value), tcomplex_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vector_tcomplex_Value)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vector_tcomplex_Value), vector_tcomplex_Value, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(v3_tcomplex)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(v3_tcomplex), v3_tcomplex, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(l3_tcomplex)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(l3_tcomplex), l3_tcomplex, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(q3_tcomplex)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(q3_tcomplex), q3_tcomplex, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(q4_tcomplex)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(q4_tcomplex), q4_tcomplex, Child_jfs654);
		}
	}
}
VOID tcomplex2::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT tcomplex2::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT tcomplex2::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT tcomplex2::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT tcomplex2::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT tcomplex2::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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

