#include "stdafx.h"
#include "..\PublicHeader\PublicSerializeFieldDefine.h"

#include "..\PublicHeader\PublicDTraceLib.h"

#include "StructParser.h"


static VOID READ_WRITE_MORE_CODE(CString & strCode, LPCTSTR StructName);

//////////////////////////////////////////////////////////////////////////
//begin			-			strCode += L"
//middle		-			"; strCode += FieldName; strCode += L"
//end			-			"; strCode += L"\r\n";
//////////////////////////////////////////////////////////////////////////

VOID XmlStreamConverter::WRITE_BASIC_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"	AllocateOneMore(xmlElement.m_Children);"; strCode += L"\r\n";
	strCode += L"	ValueToElement(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Last(xmlElement.m_Children));"; strCode += L"\r\n";
	strCode += L"\r\n";
}

VOID XmlStreamConverter::WRITE_CSTRING_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	WRITE_BASIC_FIELD_CODE( strCode, FieldName );
}

VOID XmlStreamConverter::WRITE_STDSTRING_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	WRITE_BASIC_FIELD_CODE( strCode, FieldName );
}

VOID XmlStreamConverter::WRITE_FIXED_ARRAY_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"	//<!-- FIXED_ARRAY_FIELD not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}

VOID XmlStreamConverter::WRITE_RAW_ARRAY_FIELD_CODE( CString & strCode, LPCTSTR FieldName, LPCTSTR Length )
{
	strCode += L"	//<!-- RAW_ARRAY_FIELD not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}

VOID XmlStreamConverter::WRITE_COMPLEX_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"	AllocateOneMore(xmlElement.m_Children);"; strCode += L"\r\n";
	strCode += L"	"; strCode += FieldName; strCode += L".ToElement(FIELD_NAME("; strCode += FieldName; strCode += L"), Last(xmlElement.m_Children));"; strCode += L"\r\n";
	strCode += L"\r\n";
}


VOID XmlStreamConverter::WRITE_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"	AllocateOneMore(xmlElement.m_Children);"; strCode += L"\r\n";
	strCode += L"	/*VectorToElement*/ContainerToElement(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Last(xmlElement.m_Children));"; strCode += L"\r\n";
	strCode += L"\r\n";
}

VOID XmlStreamConverter::WRITE_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	AllocateOneMore(xmlElement.m_Children);"; strCode += L"\r\n";
	strCode += L"	Vector2DToElement(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Last(xmlElement.m_Children));"; strCode += L"\r\n";
	strCode += L"\r\n";
}

VOID XmlStreamConverter::WRITE_COMPLEX_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"	AllocateOneMore(xmlElement.m_Children);"; strCode += L"\r\n";
	strCode += L"	/*VectorStructToElement*/ContainerStructToElement(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Last(xmlElement.m_Children));"; strCode += L"\r\n";
	strCode += L"\r\n";
}

VOID XmlStreamConverter::WRITE_COMPLEX_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	AllocateOneMore(xmlElement.m_Children);"; strCode += L"\r\n";
	strCode += L"	Vector2DStructToElement(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Last(xmlElement.m_Children));"; strCode += L"\r\n";
	strCode += L"\r\n";
}

VOID XmlStreamConverter::WRITE_HEADER_CODE( CString & strCode, LPCTSTR StructName, LPCTSTR MethodName )
{
	strCode += L"////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////" L"\r\n\r\n";
	strCode += L"VOID "; strCode += StructName; strCode += L"::ToElement(LPCTSTR RootName, XmlElement & xmlElement) CONST"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	xmlElement.m_Name = RootName;"; strCode += L"\r\n";
	strCode += L"\r\n";
}

VOID XmlStreamConverter::WRITE_ENDING_CODE( CString & strCode, LPCTSTR StructName, LPCTSTR MethodName )
{
	strCode += L"}"; strCode += L"\r\n";
	
	strCode += L"////////////////////////////WWWWWW END//////////////////////////////////////////////" L"\r\n\r\n";
}

VOID XmlStreamConverter::READ_HEADER_CODE( CString & strCode, LPCTSTR StructName, LPCTSTR MethodName )
{
	strCode += L"\r\n\r\n////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////"; strCode += L"\r\n";
	strCode += L"VOID "; strCode += StructName; strCode += L"::FromElement(LPCTSTR RootName, CONST XmlElement & xmlElement)"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	ATLASSERT(0 == xmlElement.m_Name.CompareNoCase(RootName));"; strCode += L"\r\n";
	strCode += L"	for (size_t ik9856i = 0; ik9856i < xmlElement.m_Children.size(); ++ ik9856i )"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		CONST XmlElement & Child_jfs654 = xmlElement.m_Children[ik9856i];"; strCode += L"\r\n";
	strCode += L"		if (0)"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID XmlStreamConverter::READ_ENDING_CODE( CString & strCode, LPCTSTR StructName , LPCTSTR MethodName)
{
	strCode += L"	}"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n";
	READ_WRITE_MORE_CODE(strCode, StructName);
	strCode += L"////////////////////////////RRRRRR END//////////////////////////////////////////////"; strCode += L"\r\n"; strCode += L"\r\n";
}

VOID XmlStreamConverter::READ_BASIC_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME("; strCode += FieldName; strCode += L")))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			ElementToValue(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Child_jfs654);"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID XmlStreamConverter::READ_CSTRING_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	READ_BASIC_FIELD_CODE( strCode, FieldName );
}

VOID XmlStreamConverter::READ_STDSTRING_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	READ_BASIC_FIELD_CODE( strCode, FieldName );
}

VOID XmlStreamConverter::READ_FIXED_ARRAY_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"		//<!-- FIXED_ARRAY_FIELD not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}

VOID XmlStreamConverter::READ_RAW_ARRAY_FIELD_CODE( CString & strCode, LPCTSTR FieldName, LPCTSTR Length )
{
	strCode += L"		//<!-- RAW_ARRAY_FIELD not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}

VOID XmlStreamConverter::READ_COMPLEX_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME("; strCode += FieldName; strCode += L")))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			"; strCode += FieldName; strCode += L".FromElement(FIELD_NAME("; strCode += FieldName; strCode += L"), Child_jfs654);"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}


VOID XmlStreamConverter::READ_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME("; strCode += FieldName; strCode += L")))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			/*ElementToVector*/ElementToContainer(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Child_jfs654);"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}


VOID XmlStreamConverter::READ_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME("; strCode += FieldName; strCode += L")))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			ElementToVector2D(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Child_jfs654);"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID XmlStreamConverter::READ_COMPLEX_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME("; strCode += FieldName; strCode += L")))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			/*ElementToVectorStruct*/ElementToContainerStruct(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Child_jfs654);"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID XmlStreamConverter::READ_COMPLEX_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME("; strCode += FieldName; strCode += L")))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			ElementToVector2DStruct(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Child_jfs654);"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

//////////////////////////////////////////////////////////////////////////

VOID XmlStreamConverter::READ_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME("; strCode += FieldName; strCode += L")))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			ElementToVector3D(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Child_jfs654);"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID XmlStreamConverter::READ_COMPLEX_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"		else if (0 == Child_jfs654.m_Name.CompareNoCase(FIELD_NAME("; strCode += FieldName; strCode += L")))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			ElementToVector3DStruct(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Child_jfs654);"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}


VOID XmlStreamConverter::WRITE_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	AllocateOneMore(xmlElement.m_Children);"; strCode += L"\r\n";
	strCode += L"	Vector3DToElement(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Last(xmlElement.m_Children));"; strCode += L"\r\n";
	strCode += L"\r\n";
}

VOID XmlStreamConverter::WRITE_COMPLEX_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	AllocateOneMore(xmlElement.m_Children);"; strCode += L"\r\n";
	strCode += L"	Vector3DStructToElement(FIELD_NAME("; strCode += FieldName; strCode += L"), "; strCode += FieldName; strCode += L", Last(xmlElement.m_Children));"; strCode += L"\r\n";
	strCode += L"\r\n";
}


static VOID READ_WRITE_MORE_CODE(CString & strCode, LPCTSTR StructName)
{
	strCode += L"VOID "; strCode += StructName; strCode += L"::ToString( LPCTSTR RootName, CString & strXml ) CONST"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	XmlElement xmlElement;"; strCode += L"\r\n";
	strCode += L"	ToElement(RootName, xmlElement);"; strCode += L"\r\n";
	strCode += L"	xmlElement.ToString(strXml);"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n";
	strCode += L"\r\n";

	strCode += L"HRESULT "; strCode += StructName; strCode += L"::FromString( LPCTSTR RootName, CONST CString & strXml )"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	XmlElement xmlElement;"; strCode += L"\r\n";
	strCode += L"	INT Start = 0;"; strCode += L"\r\n";
	strCode += L"	HRESULT hr = S_OK;"; strCode += L"\r\n";
	strCode += L"	LONG xml_ele_count = xmlElement.FromString(strXml, Start);"; strCode += L"\r\n";
	strCode += L"	if (xml_ele_count > 0)"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		FromElement(RootName, xmlElement);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
	strCode += L"	else"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		hr = HRESULT_FROM_WIN32(ERROR_BAD_FORMAT);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
	strCode += L"	return hr;"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n";
	strCode += L"\r\n";

	strCode += L"HRESULT "; strCode += StructName; strCode += L"::ToStream( LPCTSTR RootName, IFileDevice * pIFileDevice ) CONST"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	HRESULT hr = S_OK;"; strCode += L"\r\n";

	strCode += L"	BEGIN_BLOCK(0);"; strCode += L"\r\n";

	strCode += L"	CString strXml;"; strCode += L"\r\n";
	strCode += L"	ToString(RootName, strXml);"; strCode += L"\r\n";
	strCode += L"	static CONST BYTE UTF16_TAG[] = {0xff, 0xfe};"; strCode += L"\r\n";

	strCode += L"	ULONG NumberOfBytesWritten = 0;"; strCode += L"\r\n";
	strCode += L"	hr = pIFileDevice->Write(sizeof(UTF16_TAG), &NumberOfBytesWritten, UTF16_TAG);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"Write\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";

	strCode += L"	NumberOfBytesWritten = 0;"; strCode += L"\r\n";
	strCode += L"	hr = pIFileDevice->Write(strXml.GetLength() * sizeof(TCHAR), &NumberOfBytesWritten, (LPCBYTE)strXml.GetString());"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"Write\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";

	strCode += L"	END_BLOCK(0);"; strCode += L"\r\n";

	strCode += L"	return hr;"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n";
	strCode += L"\r\n";

	strCode += L"HRESULT "; strCode += StructName; strCode += L"::FromStream( LPCTSTR RootName, IFileDevice * pIFileDevice )"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	HRESULT hr = S_OK;"; strCode += L"\r\n";
	strCode += L"	DWORD LastError = 0;"; strCode += L"\r\n";

	strCode += L"	BEGIN_BLOCK(0);"; strCode += L"\r\n";
	//////////////////////////////////////////////////////////////////////////
	strCode += L"	LONGLONG FileSize = 0;"; strCode += L"\r\n";
	strCode += L"	hr = pIFileDevice->GetFileSize(&FileSize);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"GetFileSize\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";

	//////////////////////////////////////////////////////////////////////////
	strCode += L"	CString strXML;"; strCode += L"\r\n";
	strCode += L"	INT CharCount = (INT)FileSize / sizeof(TCHAR) + 2;"; strCode += L"\r\n";
	strCode += L"	LPTSTR pBuffer = strXML.GetBuffer(CharCount);"; strCode += L"\r\n";
	strCode += L"	pBuffer[CharCount - 1] = 0;"; strCode += L"\r\n";
	strCode += L"	pBuffer[CharCount - 2] = 0;"; strCode += L"\r\n";

	strCode += L"	ULONG NumberOfBytesRead = 0;"; strCode += L"\r\n";
	strCode += L"	hr = pIFileDevice->Read((ULONG)FileSize, &NumberOfBytesRead, (LPBYTE)pBuffer);"; strCode += L"\r\n";
	strCode += L"	strXML.ReleaseBuffer();"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"Read\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";

	//////////////////////////////////////////////////////////////////////////
	strCode += L"	hr = FromString(RootName, strXML);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"FromString\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
	//////////////////////////////////////////////////////////////////////////
	strCode += L"	END_BLOCK(0);"; strCode += L"\r\n";

	strCode += L"	return hr;"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n";
	strCode += L"\r\n";

	strCode += L"HRESULT "; strCode += StructName; strCode += L"::ToFile( LPCTSTR RootName, LPCTSTR pFileName ) CONST"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	HRESULT hr = S_OK;"; strCode += L"\r\n";
	strCode += L"	DWORD LastError = 0;"; strCode += L"\r\n";

	strCode += L"	BEGIN_BLOCK(0);"; strCode += L"\r\n";
	//////////////////////////////////////////////////////////////////////////
	strCode += L"	CComPtr<IFileDevice> spIFileDevice;"; strCode += L"\r\n";
	strCode += L"	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &spIFileDevice, TRUE);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"CreateInstanceFileDeviceWin32\"), hr, TEXT(\"[%s]\"), pFileName);"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";

	//////////////////////////////////////////////////////////////////////////
	strCode += L"	hr = ToStream(RootName, spIFileDevice);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"ToStream\"), hr, TEXT(\"[%s]\"), pFileName);"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
	strCode += L"	END_BLOCK(0);"; strCode += L"\r\n";

	strCode += L"	return hr;"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n";
	strCode += L"\r\n";

	strCode += L"HRESULT "; strCode += StructName; strCode += L"::FromFile( LPCTSTR RootName, LPCTSTR pFileName )"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	HRESULT hr = S_OK;"; strCode += L"\r\n";
	strCode += L"	DWORD LastError = 0;"; strCode += L"\r\n";

	strCode += L"	BEGIN_BLOCK(0);"; strCode += L"\r\n";
	//////////////////////////////////////////////////////////////////////////
	strCode += L"	CComPtr<IFileDevice> spIFileDevice;"; strCode += L"\r\n";
	strCode += L"	hr = CreateInstanceFileDeviceWin32(pFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"CreateInstanceFileDeviceWin32\"), hr, TEXT(\"[%s]\"), pFileName);"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";

	//////////////////////////////////////////////////////////////////////////
	strCode += L"	hr = FromStream(RootName, spIFileDevice);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"FromStream\"), hr, TEXT(\"[%s]\"), pFileName);"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
	strCode += L"	END_BLOCK(0);"; strCode += L"\r\n";

	strCode += L"	return hr;"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n";
	strCode += L"\r\n";

}

