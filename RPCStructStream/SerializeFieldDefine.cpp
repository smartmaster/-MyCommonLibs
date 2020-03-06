#include "stdafx.h"
#include "..\PublicHeader\PublicSerializeFieldDefine.h"

#include "..\PublicHeader\PublicDTraceLib.h"

#include "StructParser.h"

//////////////////////////////////////////////////////////////////////////
//begin			-			strCode += L"
//middle		-			"; strCode += FieldName; strCode += L"
//end			-			"; strCode += L"\r\n";
//////////////////////////////////////////////////////////////////////////

VOID StreamConverter::WRITE_BASIC_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"WRITE_SINGLE_FIELD(hr, nvs, "; 
	//strCode += FieldName;
	//strCode += L");";
	//strCode += L"\r\n";

	//#define WRITE_SINGLE_FIELD(hr, nvs, myField)
	strCode += L"	hr = nvs.WriteNameValue(FIELD_NAME("; strCode += FieldName; strCode += L"),"; strCode += FieldName; strCode += L");"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteNameValue\"), hr, TEXT(\"%s\"), FIELD_NAME("; strCode += FieldName; strCode += L"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
}

VOID StreamConverter::WRITE_CSTRING_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"WRITE_CSTRING_FIELD(hr, nvs, "; 
	//strCode += FieldName;
	//strCode += L");";
	//strCode += L"\r\n";
	strCode += L"	hr = nvs.WriteNameValue(FIELD_NAME("; strCode += FieldName; strCode += L"),"; strCode += FieldName; strCode += L".GetString()"; strCode += L","; strCode += FieldName; strCode += L".GetLength() + 1"; strCode += L");"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteNameValue\"), hr, TEXT(\"%s\"), FIELD_NAME("; strCode += FieldName; strCode += L"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
}

VOID StreamConverter::WRITE_STDSTRING_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"WRITE_STDSTRING_FIELD(hr, nvs, "; 
	//strCode += FieldName;
	//strCode += L");";
	//strCode += L"\r\n";
	strCode += L"	hr = nvs.WriteNameValue(FIELD_NAME("; strCode += FieldName; strCode += L"),"; strCode += FieldName; strCode += L".c_str()"; strCode += L","; strCode += FieldName; strCode += L".size() + 1"; strCode += L");"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteNameValue\"), hr, TEXT(\"%s\"), FIELD_NAME("; strCode += FieldName; strCode += L"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
}

VOID StreamConverter::WRITE_FIXED_ARRAY_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"WRITE_FIXED_ARRAY_FIELD(hr, nvs, "; 
	//strCode += FieldName;
	//strCode += L");";
	//strCode += L"\r\n";
	strCode += L"	hr = nvs.WriteNameValue(FIELD_NAME("; strCode += FieldName; strCode += L"),"; strCode += FieldName; strCode += L","; strCode += L"_countof("; strCode += FieldName; strCode += L")"; strCode += L");"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteNameValue\"), hr, TEXT(\"%s\"), FIELD_NAME("; strCode += FieldName; strCode += L"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";

}

VOID StreamConverter::WRITE_RAW_ARRAY_FIELD_CODE( CString & strCode, LPCTSTR FieldName, LPCTSTR Length )
{
	//strCode += L"WRITE_RAW_ARRAY_FIELD(hr, nvs, "; 
	//strCode += FieldName;
	//strCode += L", ";
	//strCode += FieldName;
	//strCode += L", ";
	//strCode += Length;
	//strCode += L");";
	//strCode += L"\r\n";
	strCode += L"	hr = nvs.WriteNameValue(FIELD_NAME("; strCode += FieldName; strCode += L"),"; strCode += FieldName; strCode += L","; strCode += Length; strCode += L");"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteNameValue\"), hr, TEXT(\"%s\"), FIELD_NAME("; strCode += FieldName; strCode += L"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
}

VOID StreamConverter::WRITE_COMPLEX_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"	hr = nvs.WriteStructHeader(FIELD_NAME("; strCode += FieldName; strCode += L"));"; strCode += L"\r\n";

	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteStructHeader\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";

	strCode += L"	hr = "; strCode +=FieldName; strCode += L".WriteTo(pFileDevice);"; strCode += L"\r\n";

	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteTo\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
}


VOID StreamConverter::WRITE_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"WRITE_VECTOR(hr, nvs, "; 
	//strCode += FieldName;
	//strCode += L");";
	//strCode += L"\r\n";
	strCode += L"	hr = WriteVector("; strCode +=FieldName; strCode += L", FIELD_NAME("; strCode +=FieldName; strCode += L"), nvs);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteVector\"), hr, TEXT(\"%s\"), FIELD_NAME("; strCode +=FieldName; strCode += L"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
}

VOID StreamConverter::WRITE_COMPLEX_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"WRITE_COMPLEX_VECTOR(hr, pFileDevice, "; 
	//strCode += FieldName;
	//strCode += L");";
	//strCode += L"\r\n";
	strCode += L"	hr = WriteComplexVector(";	strCode +=FieldName; strCode += L", FIELD_NAME("; strCode +=FieldName; strCode += L"), pFileDevice);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteComplexVector\"), hr, TEXT(\"%s\"), FIELD_NAME("; strCode +=FieldName; strCode += L"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
}


VOID StreamConverter::WRITE_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	//<!-- VECTOR_2D not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}

VOID StreamConverter::WRITE_COMPLEX_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	//<!-- COMPLEX_VECTOR_2D not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}

VOID StreamConverter::WRITE_HEADER_CODE( CString & strCode, LPCTSTR StructName, LPCTSTR MethodName )
{
	
	strCode += L"\r\n\r\n////////////////////////////WWWWWW BEGIN//////////////////////////////////////////////"; strCode += L"\r\n";
	
	strCode += L"HRESULT "; strCode += StructName; strCode += L"::WriteTo( IFileDevice * pFileDevice ) CONST"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	HRESULT hr = S_OK;"; strCode += L"\r\n";
	strCode += L"	BEGIN_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	CNameValueStream nvs(pFileDevice);"; strCode += L"\r\n";
	//strCode += L"	SET_BYTE_ORDER(hr, nvs);"; strCode += L"\r\n";
	strCode += L"	CNameValueStream::DATA_BYTE_ORDER ByteOrder = CNameValueStream::LITTLE_ENDIAN;"; strCode += L"\r\n";
	strCode += L"	hr = nvs.SetByteOrder(ByteOrder);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"SetByteOrder\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
	//strCode += L"	WRITE_METHOD_META_NAME(hr, nvs, "; strCode += MethodName; strCode += L");"; strCode += L"\r\n";
	strCode += L"	hr = nvs.WriteNameValue(METHOD_META_NAME, MethodString("; strCode += MethodName; strCode += L"), _countof(MethodString("; strCode += MethodName; strCode += L")));"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteNameValue\"), hr, METHOD_META_NAME);"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
}

VOID StreamConverter::WRITE_ENDING_CODE( CString & strCode, LPCTSTR StructName, LPCTSTR MethodName )
{
	//strCode += L"	WRITE_ENDING(hr, nvs);"; strCode += L"\r\n";
	strCode += L"	hr = nvs.WriteEnding();"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"WriteEnding\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";
	strCode += L"	END_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	return hr;"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n"; strCode += L"\r\n";

	strCode += L"HRESULT "; strCode += StructName; strCode += L"::WriteToBuffer( LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength ) CONST"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	return WRITE_TO_BUFFER(pBuffer, Length, WrittenLength, this);"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n"; strCode += L"\r\n";

	strCode += L"HRESULT "; strCode += StructName; strCode += L"::WriteToFile(LPCTSTR pFileName, LONGLONG & WrittenLength) CONST"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	return WRITE_TO_FILE(pFileName, WrittenLength, this);"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n";

	strCode += L"////////////////////////////WWWWWW END//////////////////////////////////////////////" L"\r\n\r\n";

}

VOID StreamConverter::READ_HEADER_CODE( CString & strCode, LPCTSTR StructName, LPCTSTR MethodName )
{
	strCode += L"\r\n\r\n////////////////////////////RRRRRR BEGIN//////////////////////////////////////////////"; strCode += L"\r\n";
	strCode += L"HRESULT "; strCode += StructName; strCode += L"::ReadFrom( IFileDevice * pFileDevice )"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	HRESULT hr = S_OK;"; strCode += L"\r\n";

	strCode += L"	BEGIN_BLOCK(0);"; strCode += L"\r\n";
	//////////////////////////////////////////////////////////////////////////
	strCode += L"	CNameValueStream nvs(pFileDevice);"; strCode += L"\r\n";

	//strCode += L"	GET_BYTE_ORDER(hr, nvs);"; strCode += L"\r\n";
	strCode += L"	CNameValueStream::DATA_BYTE_ORDER ByteOrder;"; strCode += L"\r\n";
	strCode += L"	hr = nvs.GetByteOrder(ByteOrder);"; strCode += L"\r\n";
	strCode += L"	ATLASSERT(CNameValueStream::LITTLE_ENDIAN == ByteOrder);"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"GetByteOrder\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";

	//////////////////////////////////////////////////////////////////////////
	strCode += L"	LONG ValueSize = 0;"; strCode += L"\r\n";
	strCode += L"	wstring Name;"; strCode += L"\r\n";
	strCode += L"	TCHAR szTempBuffer[256] = {0};"; strCode += L"\r\n";
	strCode += L"	for (;;)"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		hr = nvs.ReadNameValueSize(Name, ValueSize);"; strCode += L"\r\n";
	strCode += L"		if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			D_API_ERR(0, TEXT(\"ReadNameValueSize\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"			break;"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
	//strCode += L"		READ_METHOD_META_NAME(hr, nvs, Name, ValueSize, "; strCode += MethodName; strCode += L" , szTempBuffer)"; strCode += L"\r\n";
	strCode += L"		if (IS_STRING_EQUAL(METHOD_META_NAME, Name))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			hr = nvs.ReadValueData(szTempBuffer, ValueSize/sizeof(TCHAR));"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"ReadValueData\"), hr, TEXT(\"%s\"), Name.c_str());"; strCode += L"\r\n";
	strCode += L"				break;"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"			D_INFO(99, TEXT(\"Method Name=%s\"), szTempBuffer);"; strCode += L"\r\n";
	strCode += L"			ATLASSERT(IS_STRING_EQUAL_SZ(szTempBuffer, MethodString("; strCode += MethodName; strCode += L")));"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID StreamConverter::READ_ENDING_CODE( CString & strCode, LPCTSTR StructName , LPCTSTR MethodName)
{
	//strCode += L"		ELSE_CHECK_ENDING(hr, nvs, Name, ValueSize, pFileDevice)"; strCode += L"\r\n";
	strCode += L"		else if (CNameValueStream::IsEnding(Name, ValueSize))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			D_INFO(99, TEXT(\"Stream END\"));"; strCode += L"\r\n";
	strCode += L"			break;"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
	strCode += L"		else if (Name.size() && ValueSize > 0)"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			D_INFO(0, TEXT(\"Unrecognized field name = %s, ValueSize = %u, skip it\"), Name.c_str(), ValueSize);"; strCode += L"\r\n";
	strCode += L"			LONGLONG NewPos = -1;"; strCode += L"\r\n";
	strCode += L"			hr = pFileDevice->SetPointerEx(ValueSize, &NewPos, FILE_CURRENT);"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"SetPointerEx\"), hr, TEXT(\"%s\"), Name.c_str());"; strCode += L"\r\n";
	strCode += L"				break;"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
	strCode += L"		else if (Name.size() && -1 == ValueSize)"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			D_INFO(0, TEXT(\"Unrecognized structure name = %s, ValueSize = %u, skip it\"), Name.c_str(), ValueSize);"; strCode += L"\r\n";
	strCode += L"			hr = SkipSubStruct(pFileDevice);"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"SkipSubStruct\"), hr, TEXT(\"%s\"), Name.c_str());"; strCode += L"\r\n";
	strCode += L"				break;"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
	strCode += L"		else"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			hr = E_FAIL;"; strCode += L"\r\n";
	strCode += L"			D_API_ERR(0, TEXT(\"\"), E_FAIL, TEXT(\"Unexpected field name = %s, ValueSize = %u\"), Name.c_str(), ValueSize);"; strCode += L"\r\n";
	strCode += L"			break;"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";

	//////////////////////////////////////////////////////////////////////////
	strCode += L"	}"; strCode += L"\r\n";
	strCode += L"	if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"	{"; strCode += L"\r\n";
	strCode += L"		D_API_ERR(0, TEXT(\"ReadDataValue\"), hr, TEXT(\"\"));"; strCode += L"\r\n";
	strCode += L"		LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"	}"; strCode += L"\r\n";

	strCode += L"	END_BLOCK(0);"; strCode += L"\r\n";

	strCode += L"	return hr;"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n"; strCode += L"\r\n";

	strCode += L"HRESULT "; strCode += StructName; strCode += L"::ReadFromBuffer( LPBYTE pBuffer, LONGLONG Length )"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	return READ_FROM_BUFFER(pBuffer, Length, this);"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n"; strCode += L"\r\n";

	strCode += L"HRESULT "; strCode += StructName; strCode += L"::ReadFromFile(LPCTSTR pFileName)"; strCode += L"\r\n";
	strCode += L"{"; strCode += L"\r\n";
	strCode += L"	return READ_FROM_FILE(pFileName, this);"; strCode += L"\r\n";
	strCode += L"}"; strCode += L"\r\n";
	strCode += L"////////////////////////////RRRRRR END//////////////////////////////////////////////"; strCode += L"\r\n"; strCode += L"\r\n";
}

VOID StreamConverter::READ_BASIC_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"ELSE_READ_SINGLE_FILED(hr, nvs, ";
	//strCode +=	FieldName;
	//strCode += L" , Name, ValueSize)"; strCode += L"\r\n";
	strCode += L"		else if (IS_FIELD_NAME("; strCode += FieldName; strCode += L", Name))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			ATLASSERT(sizeof("; strCode += FieldName; strCode += L") == ValueSize);"; strCode += L"\r\n";
	strCode += L"			hr = nvs.ReadValueData("; strCode += FieldName; strCode += L");"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"ReadValueData\"), hr, TEXT(\"%s\"), Name.c_str());"; strCode += L"\r\n";
	strCode += L"				break;"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID StreamConverter::READ_CSTRING_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"ELSE_READ_CSTRING_FIELD(hr, nvs, ";
	//strCode +=	FieldName;
	//strCode += L" , Name, ValueSize)"; strCode += L"\r\n";
	strCode += L"		else if (IS_FIELD_NAME("; strCode += FieldName; strCode += L", Name))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			LPTSTR pBuffer = "; strCode += FieldName; strCode += L".GetBuffer(ValueSize);"; strCode += L"\r\n";
	strCode += L"			hr = nvs.ReadValueData(pBuffer, ValueSize/sizeof(TCHAR));"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"ReadValueData\"), hr, TEXT(\"%s\"), Name.c_str());"; strCode += L"\r\n";
	strCode += L"				break;"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"			"; strCode += FieldName; strCode += L".ReleaseBuffer();"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID StreamConverter::READ_STDSTRING_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"ELSE_READ_STDSTRING_FIELD(hr, nvs, ";
	//strCode +=	FieldName;
	//strCode += L" , Name, ValueSize, szTempBuffer)"; strCode += L"\r\n";
	strCode += L"		else if (IS_FIELD_NAME("; strCode += FieldName; strCode += L", Name))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			ATLASSERT(sizeof(szTempBuffer) >= ValueSize);"; strCode += L"\r\n";
	strCode += L"			hr = nvs.ReadValueData(szTempBuffer, ValueSize/sizeof(TCHAR));"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"ReadValueData\"), hr, TEXT(\"%s\"), Name.c_str());"; strCode += L"\r\n";
	strCode += L"				break;"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"			"; strCode += FieldName; strCode += L" = szTempBuffer;"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID StreamConverter::READ_FIXED_ARRAY_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"ELSE_READ_FIXED_ARRAY_FILED(hr, nvs, ";
	//strCode +=	FieldName;
	//strCode += L" , Name, ValueSize)"; strCode += L"\r\n";
	strCode += L"		else if (IS_FIELD_NAME("; strCode += FieldName; strCode += L", Name))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			ATLASSERT(sizeof("; strCode += FieldName; strCode += L") == ValueSize);"; strCode += L"\r\n";
	strCode += L"			hr = nvs.ReadValueData("; strCode += FieldName; strCode += L", _countof("; strCode += FieldName; strCode += L"));"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"ReadValueData\"), hr, TEXT(\"%s\"), Name.c_str());"; strCode += L"\r\n";
	strCode += L"				break;"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID StreamConverter::READ_RAW_ARRAY_FIELD_CODE( CString & strCode, LPCTSTR FieldName, LPCTSTR Length )
{
	//strCode += L"ELSE_READ_RAW_ARRAY_FILED(hr, nvs, ";
	//strCode += 	FieldName;
	//strCode += L" , ";
	//strCode += 	Length;
	//strCode += L" , Name, ValueSize)"; strCode += L"\r\n";
	strCode += L"		else if (IS_FIELD_NAME("; strCode += FieldName; strCode += L", Name))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			//ATLASSERT(BufferLengthInBytes == ValueSize);"; strCode += L"\r\n";
	strCode += L"			hr = nvs.ReadValueBinaryData((BYTE*)("; strCode += FieldName; strCode += L"), ValueSize);"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"ReadValueData\"), hr, TEXT(\"%s\"), Name.c_str());"; strCode += L"\r\n";
	strCode += L"				break;"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID StreamConverter::READ_COMPLEX_CODE( CString & strCode, LPCTSTR FieldName )
{
	strCode += L"		else if (Name == FIELD_NAME( "; strCode +=FieldName;  strCode += L") && ValueSize == -1)"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			hr = "; strCode +=FieldName; strCode += L".ReadFrom(pFileDevice);"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"ReadFrom\"), hr, TEXT(\"FieldName=%s\"), "; strCode += FieldName; strCode += L");"; strCode += L"\r\n";
	strCode += L"				break;"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}


VOID StreamConverter::READ_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"ELSE_READ_VECTOR(hr, nvs, "; 
	//strCode += FieldName;
	//strCode += L" , Name, ValueSize)"; strCode += L"\r\n";
	//strCode += L"\r\n";
	strCode += L"		else if (IS_FIELD_NAME("; strCode +=FieldName; strCode += L", Name))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			hr = ReadVector("; strCode +=FieldName; strCode += L", Name, ValueSize, nvs);"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"WriteVector\"), hr, TEXT(\"%s\"), FIELD_NAME("; strCode +=FieldName; strCode += L"));"; strCode += L"\r\n";
	strCode += L"				LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}

VOID StreamConverter::READ_COMPLEX_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName )
{
	//strCode += L"ELSE_READ_COMPLEX_VECTOR(hr, pFileDevice, "; 
	//strCode += FieldName;
	//strCode += L" , Name, ValueSize)"; strCode += L"\r\n";
	//strCode += L"\r\n";
	strCode += L"		else if (IS_FIELD_NAME("; strCode +=FieldName; strCode += L", Name))"; strCode += L"\r\n";
	strCode += L"		{"; strCode += L"\r\n";
	strCode += L"			hr = ReadComplexVector("; strCode +=FieldName; strCode += L", Name, ValueSize, pFileDevice);"; strCode += L"\r\n";
	strCode += L"			if (FAILED(hr))"; strCode += L"\r\n";
	strCode += L"			{"; strCode += L"\r\n";
	strCode += L"				D_API_ERR(0, TEXT(\"WriteComplexVector\"), hr, TEXT(\"%s\"), FIELD_NAME("; strCode +=FieldName; strCode += L"));"; strCode += L"\r\n";
	strCode += L"				LEAVE_BLOCK(0);"; strCode += L"\r\n";
	strCode += L"			}"; strCode += L"\r\n";
	strCode += L"		}"; strCode += L"\r\n";
}



VOID StreamConverter::READ_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	//<!-- VECTOR_2D not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}

VOID StreamConverter::READ_COMPLEX_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	//<!-- COMPLEX_VECTOR_2D not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}


VOID StreamConverter::READ_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	//<!-- VECTOR_3D not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}

VOID StreamConverter::READ_COMPLEX_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	//<!-- COMPLEX_VECTOR_3D not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}


VOID StreamConverter::WRITE_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	//<!-- VECTOR_3D not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}

VOID StreamConverter::WRITE_COMPLEX_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName)
{
	strCode += L"	//<!-- COMPLEX_VECTOR_3D not supported "; strCode += FieldName; strCode += L" -->"; strCode += L"\r\n";
}