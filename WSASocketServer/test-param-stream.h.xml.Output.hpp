////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID my_simple_t::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ii), ii, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ll), ll, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(cstr), cstr, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(stdstr), stdstr, Last(xmlElement.m_Children));

	//<!-- FIXED_ARRAY_FIELD not supported fixedArr -->
	//<!-- RAW_ARRAY_FIELD not supported FakeRawArr -->
}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID my_complex_t::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ii), ii, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ll), ll, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(cstr), cstr, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(stdstr), stdstr, Last(xmlElement.m_Children));

	//<!-- FIXED_ARRAY_FIELD not supported fixedArr -->
	//<!-- RAW_ARRAY_FIELD not supported FakeRawArr -->
	AllocateOneMore(xmlElement.m_Children);
	t1.ToElement(FIELD_NAME(t1), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	t2.ToElement(FIELD_NAME(t2), Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID my_very_complex_t::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ii), ii, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ll), ll, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(cstr), cstr, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(stdstr), stdstr, Last(xmlElement.m_Children));

	//<!-- FIXED_ARRAY_FIELD not supported fixedArr -->
	//<!-- RAW_ARRAY_FIELD not supported FakeRawArr -->
	AllocateOneMore(xmlElement.m_Children);
	t1.ToElement(FIELD_NAME(t1), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	t2.ToElement(FIELD_NAME(t2), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ct1.ToElement(FIELD_NAME(ct1), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ct2.ToElement(FIELD_NAME(ct2), Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID my_hard_t::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ii), ii, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(ll), ll, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(cstr), cstr, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ValueToElement(FIELD_NAME(stdstr), stdstr, Last(xmlElement.m_Children));

	//<!-- FIXED_ARRAY_FIELD not supported fixedArr -->
	//<!-- RAW_ARRAY_FIELD not supported FakeRawArr -->
	AllocateOneMore(xmlElement.m_Children);
	t1.ToElement(FIELD_NAME(t1), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	t2.ToElement(FIELD_NAME(t2), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ct1.ToElement(FIELD_NAME(ct1), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	ct2.ToElement(FIELD_NAME(ct2), Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vecVh), vecVh, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vecCstr), vecCstr, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(vecSimple), vecSimple, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(vecComplex), vecComplex, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vecstdstr), vecstdstr, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vecVh1), vecVh1, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vecCstr1), vecCstr1, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(vecSimple1), vecSimple1, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(vecComplex1), vecComplex1, Last(xmlElement.m_Children));

	AllocateOneMore(xmlElement.m_Children);
	/*VectorToElement*/ContainerToElement(FIELD_NAME(vecstdstr1), vecstdstr1, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////

////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////

VOID my_hard_array_t::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST
{
	xmlElement.m_Name = RootName;

	AllocateOneMore(xmlElement.m_Children);
	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME(arr), arr, Last(xmlElement.m_Children));

}
////////////////////////////WWWWWW END//////////////////////////////////////////////



////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////
VOID my_simple_t::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ii)))
		{
			ElementToValue(FIELD_NAME(ii), ii, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ll)))
		{
			ElementToValue(FIELD_NAME(ll), ll, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(cstr)))
		{
			ElementToValue(FIELD_NAME(cstr), cstr, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(stdstr)))
		{
			ElementToValue(FIELD_NAME(stdstr), stdstr, Child_jfs654);
		}
		//<!-- FIXED_ARRAY_FIELD not supported fixedArr -->
		//<!-- RAW_ARRAY_FIELD not supported FakeRawArr -->
	}
}
VOID my_simple_t::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT my_simple_t::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT my_simple_t::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT my_simple_t::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT my_simple_t::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT my_simple_t::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID my_complex_t::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ii)))
		{
			ElementToValue(FIELD_NAME(ii), ii, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ll)))
		{
			ElementToValue(FIELD_NAME(ll), ll, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(cstr)))
		{
			ElementToValue(FIELD_NAME(cstr), cstr, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(stdstr)))
		{
			ElementToValue(FIELD_NAME(stdstr), stdstr, Child_jfs654);
		}
		//<!-- FIXED_ARRAY_FIELD not supported fixedArr -->
		//<!-- RAW_ARRAY_FIELD not supported FakeRawArr -->
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(t1)))
		{
			t1.FromElement(FIELD_NAME(t1), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(t2)))
		{
			t2.FromElement(FIELD_NAME(t2), Child_jfs654);
		}
	}
}
VOID my_complex_t::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT my_complex_t::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT my_complex_t::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT my_complex_t::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT my_complex_t::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT my_complex_t::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID my_very_complex_t::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ii)))
		{
			ElementToValue(FIELD_NAME(ii), ii, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ll)))
		{
			ElementToValue(FIELD_NAME(ll), ll, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(cstr)))
		{
			ElementToValue(FIELD_NAME(cstr), cstr, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(stdstr)))
		{
			ElementToValue(FIELD_NAME(stdstr), stdstr, Child_jfs654);
		}
		//<!-- FIXED_ARRAY_FIELD not supported fixedArr -->
		//<!-- RAW_ARRAY_FIELD not supported FakeRawArr -->
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(t1)))
		{
			t1.FromElement(FIELD_NAME(t1), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(t2)))
		{
			t2.FromElement(FIELD_NAME(t2), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ct1)))
		{
			ct1.FromElement(FIELD_NAME(ct1), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ct2)))
		{
			ct2.FromElement(FIELD_NAME(ct2), Child_jfs654);
		}
	}
}
VOID my_very_complex_t::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT my_very_complex_t::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT my_very_complex_t::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT my_very_complex_t::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT my_very_complex_t::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT my_very_complex_t::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID my_hard_t::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ii)))
		{
			ElementToValue(FIELD_NAME(ii), ii, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ll)))
		{
			ElementToValue(FIELD_NAME(ll), ll, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(cstr)))
		{
			ElementToValue(FIELD_NAME(cstr), cstr, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(stdstr)))
		{
			ElementToValue(FIELD_NAME(stdstr), stdstr, Child_jfs654);
		}
		//<!-- FIXED_ARRAY_FIELD not supported fixedArr -->
		//<!-- RAW_ARRAY_FIELD not supported FakeRawArr -->
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(t1)))
		{
			t1.FromElement(FIELD_NAME(t1), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(t2)))
		{
			t2.FromElement(FIELD_NAME(t2), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ct1)))
		{
			ct1.FromElement(FIELD_NAME(ct1), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(ct2)))
		{
			ct2.FromElement(FIELD_NAME(ct2), Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vecVh)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vecVh), vecVh, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vecCstr)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vecCstr), vecCstr, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vecSimple)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(vecSimple), vecSimple, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vecComplex)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(vecComplex), vecComplex, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vecstdstr)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vecstdstr), vecstdstr, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vecVh1)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vecVh1), vecVh1, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vecCstr1)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vecCstr1), vecCstr1, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vecSimple1)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(vecSimple1), vecSimple1, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vecComplex1)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(vecComplex1), vecComplex1, Child_jfs654);
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(vecstdstr1)))
		{
			/*ElementToVector*/ElementToContainer(FIELD_NAME(vecstdstr1), vecstdstr1, Child_jfs654);
		}
	}
}
VOID my_hard_t::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT my_hard_t::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT my_hard_t::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT my_hard_t::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT my_hard_t::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT my_hard_t::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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
VOID my_hard_array_t::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)
{
	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));
	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )
	{
		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];
		if (0)
		{
		}
		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME(arr)))
		{
			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME(arr), arr, Child_jfs654);
		}
	}
}
VOID my_hard_array_t::ToString( LPCTSTR RootName, CString & strXml ) CONST
{
	XmlElement xmlElement;
	ToElement(RootName, xmlElement);
	xmlElement.ToString(strXml);
}

HRESULT my_hard_array_t::FromString( LPCTSTR RootName, CONST CString & strXml )
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

HRESULT my_hard_array_t::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST
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

HRESULT my_hard_array_t::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )
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

HRESULT my_hard_array_t::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST
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

HRESULT my_hard_array_t::FromFile( LPCTSTR RootName, LPCTSTR pFileName )
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

