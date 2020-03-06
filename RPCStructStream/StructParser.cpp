#include "stdafx.h"

#include "..\PublicHeader\PublicWideNarrowChar.h"
#include "StructParser.h"
#include "ReadWriteTextFile.h"

INT GetMatchedPos(CONST CString & str, INT Pos, CONST TCHAR Left = TEXT('('), CONST TCHAR Right = TEXT(')'))
{
	ATLASSERT((-1 != Pos) && (Pos < str.GetLength()) && (Left == str[Pos]));
	INT Count = 0;
	while (Pos < str.GetLength())
	{
		if (Left == str[Pos])
		{
			++ Count;
		}
		else if (Right == str[Pos])
		{
			-- Count;
			if (0 == Count)
			{
				break;
			}
		}

		++ Pos;
	}

	if (Pos >= str.GetLength())
	{
		Pos = -1;
	}

	return Pos;
}


HRESULT GetNextField( CONST CString & strSource, IN OUT int & Start, CString & TokenField, vector<CString> & Tokens )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	Start = strSource.Find(SEARCH_TAG, Start);
	if (-1 == Start)
	{
		hr = S_OK;
		D_INFO(0, TEXT("No more SEARCH_TAG found, end pasrsing"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	INT End = strSource.Find(TEXT('('), Start);
	if (-1 == End)
	{
		Start = -1;
		hr = E_FAIL;
		D_INFO(0, TEXT("No more ( found, end pasrsing"));
		LEAVE_BLOCK(0);
	}

	TokenField = strSource.Mid(Start, End - Start);
	TokenField.Trim(TEXT(' '));
	if (TokenField.GetLength() == 0)
	{
		hr = E_FAIL;
		D_INFO(0, TEXT("No TokenField found, end pasrsing"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	Start = End;
	End = GetMatchedPos(strSource, Start);//strSource.Find(TEXT(')'), Start);
	if (-1 == End)
	{
		Start = -1;
		hr = E_FAIL;
		D_INFO(0, TEXT("No more ) found, end pasrsing"));
		LEAVE_BLOCK(0);
	}

	//CString strSub = strSource.Mid(Start, End - Start +1);
	CString strSub = strSource.Mid(Start + 1, End - Start - 1);
	int StartSub = 0;
	//static CONST TCHAR DELIMITERS[] = TEXT(" \t,()");
	static CONST TCHAR DELIMITERS[] = TEXT(",");
	for (;;)
	{
		CString token = strSub.Tokenize(DELIMITERS, StartSub);
		token.Trim(TEXT(' '));
		if (token.GetLength())
		{
			Tokens.push_back(token);
		}
		else if (-1 == StartSub)
		{
			break;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	if (Tokens.size() == 0)
	{
		hr = E_FAIL;
		D_INFO(0, TEXT("No tokens found, end pasrsing"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	Start = End;

	END_BLOCK(0);

	return hr;
}

template<typename T_CONVERTER>
HRESULT ParseStruct( CONST CString & strSource, IN OUT int & Start, OUT CString & strWriteCode, OUT CString & strReadCode )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////

	for (;;)
	{
		CString TokenField;
		vector<CString> TokenInBracket;
		hr = GetNextField(strSource, Start, TokenField, TokenInBracket);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("GetNextField"), hr, TEXT(""));
			break;
		}

		if (TokenField == SERIALIZE_TOKEN(SERIALIZE_BASIC))
		{
			T_CONVERTER::WRITE_BASIC_FIELD_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_BASIC_FIELD_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_CSTRING))
		{
			T_CONVERTER::WRITE_CSTRING_FIELD_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_CSTRING_FIELD_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_STDSTRING))
		{
			T_CONVERTER::WRITE_STDSTRING_FIELD_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_STDSTRING_FIELD_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_FIXED_ARRAY))
		{
			T_CONVERTER::WRITE_FIXED_ARRAY_FIELD_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_FIXED_ARRAY_FIELD_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_RAW_ARRAY))
		{
			T_CONVERTER::WRITE_RAW_ARRAY_FIELD_CODE(strWriteCode, TokenInBracket[0], TokenInBracket[1]);
			T_CONVERTER::READ_RAW_ARRAY_FIELD_CODE(strReadCode, TokenInBracket[0], TokenInBracket[1]);
			D_INFO(0, TEXT("%s(%s, %s)"), TokenField.GetString(), TokenInBracket[0].GetString(), TokenInBracket[1].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_COMPLEX))
		{
			T_CONVERTER::WRITE_COMPLEX_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_COMPLEX_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_VECTOR))
		{
			T_CONVERTER::WRITE_VECTOR_FIELD_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_VECTOR_FIELD_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_VECTOR_2D)) //<sonmi01>2014-8-4 ###???
		{
			T_CONVERTER::WRITE_VECTOR_2D_FIELD_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_VECTOR_2D_FIELD_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_VECTOR_3D)) //<sonmi01>2014-8-4 ###???
		{
			T_CONVERTER::WRITE_VECTOR_3D_FIELD_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_VECTOR_3D_FIELD_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_COMPLEX_VECTOR))
		{
			T_CONVERTER::WRITE_COMPLEX_VECTOR_FIELD_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_COMPLEX_VECTOR_FIELD_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_COMPLEX_VECTOR_2D)) //<sonmi01>2014-8-4 ###???
		{
			T_CONVERTER::WRITE_COMPLEX_VECTOR_2D_FIELD_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_COMPLEX_VECTOR_2D_FIELD_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_COMPLEX_VECTOR_3D)) //<sonmi01>2014-8-4 ###???
		{
			T_CONVERTER::WRITE_COMPLEX_VECTOR_3D_FIELD_CODE(strWriteCode, TokenInBracket[0]);
			T_CONVERTER::READ_COMPLEX_VECTOR_3D_FIELD_CODE(strReadCode, TokenInBracket[0]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_BEGIN_STRUCT))
		{
			T_CONVERTER::WRITE_HEADER_CODE(strWriteCode, TokenInBracket[0], TokenInBracket[1]);
			T_CONVERTER::READ_HEADER_CODE(strReadCode, TokenInBracket[0], TokenInBracket[1]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
		}
		else if (TokenField == SERIALIZE_TOKEN(SERIALIZE_END_STRUCT))
		{
			T_CONVERTER::WRITE_ENDING_CODE(strWriteCode, TokenInBracket[0], TokenInBracket[1]);
			T_CONVERTER::READ_ENDING_CODE(strReadCode, TokenInBracket[0], TokenInBracket[1]);
			D_INFO(0, TEXT("%s(%s)"), TokenField.GetString(), TokenInBracket[0].GetString());
			hr = S_OK;
			break;
		}
		else
		{
			D_INFO(0, TEXT("Unrecognized Token Field %s"), TokenField.GetString());
			hr = E_FAIL;
			break;
		}
	}	

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

template<typename T_CONVERTER>
HRESULT ParseSource( CONST CString & strSource, CString & strWriteCode, CString & strReadCode )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	INT Start = 0;

	BEGIN_BLOCK(0);
	while (Start != -1 && SUCCEEDED(hr))
	{
		hr = ParseStruct<T_CONVERTER>(strSource, Start, strWriteCode, strReadCode);
	}
	END_BLOCK(0);

	return hr;
}

HRESULT ParseSourceFile(LPCTSTR strInput, LPCTSTR strOutPut, BOOL bXml)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strSource;
	hr = ReadSourceFile(strInput, strSource);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadSourceFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	vector<CString> OutPut(2);
	if (bXml)
	{
		hr = ParseSource<XmlStreamConverter>(strSource, OutPut[0], OutPut[1]);
	}
	else
	{
		hr = ParseSource<StreamConverter>(strSource, OutPut[0], OutPut[1]);
	}
	
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ParseSource"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	hr = WriteSourceFile(strOutPut, OutPut, E_WRITESOURCEFILE::E_UTF8_NO_TAG);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("WriteSourceFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);
	
	return hr;
}

