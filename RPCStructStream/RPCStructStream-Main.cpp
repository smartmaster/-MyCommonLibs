// RPCStructStream.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "StructParser.h"


VOID NOP() {}

#define WriteLine(format, ...) _ftprintf(stdout, format L"\r\n", __VA_ARGS__)

VOID PrintUsage()
{
	WriteLine(L"Usage:");
	WriteLine(L"\t this.exe <xml|bin> <infile> [outfile]");
}

int _tmain(int argc, _TCHAR* argv[])
{
	D_SETTINGS(999, FALSE, TEXT(""), TEXT(""));
	if (argc < 3)
	{
		PrintUsage();
		return E_INVALIDARG;
	}
	BOOL bXml = (0 == _tcsicmp(TEXT("xml"), argv[1]));
	CString strIn = argv[2];
	CString strOut;
	if (argc > 3)
	{
		strOut = argv[3];
	}
	else
	{
		static CONST TCHAR EXT[] = TEXT(".Output.hpp");
		static CONST TCHAR XML_EXT[] = TEXT(".xml.Output.hpp");
		strOut = strIn;
		bXml? strOut.Append(XML_EXT) : strOut.Append(EXT);
	}
	ParseSourceFile(strIn, strOut, bXml);
	//CString strCode;

	//WRITE_HEADER_CODE(strCode, TEXT("some_func"));
	//WRITE_BASIC_FIELD_CODE(strCode, TEXT("SINGLE_FIELD"));
	//WRITE_CSTRING_FIELD_CODE(strCode, TEXT("CSTRING_FIELD"));
	//WRITE_STDSTRING_FIELD_CODE(strCode, TEXT("STDSTRING_FIELD"));
	//WRITE_FIXED_ARRAY_FIELD_CODE(strCode, TEXT("FIXED_ARRAY"));
	//WRITE_RAW_ARRAY_FIELD_CODE(strCode, TEXT("raw_ptr"), TEXT("get_size()"));
	//WRITE_COMPLEX_CODE(strCode, TEXT("sub_str"));
	//WRITE_ENDING_CODE(strCode);


	//READ_HEADER_CODE(strCode, TEXT("some_func"));
	//READ_BASIC_FIELD_CODE(strCode, TEXT("SINGLE_FIELD"));
	//READ_CSTRING_FIELD_CODE(strCode, TEXT("CSTRING_FIELD"));
	//READ_STDSTRING_FIELD_CODE(strCode, TEXT("STDSTRING_FIELD"));
	//READ_FIXED_ARRAY_FIELD_CODE(strCode, TEXT("FIXED_ARRAY"));
	//READ_RAW_ARRAY_FIELD_CODE(strCode, TEXT("raw_ptr"), TEXT("get_size()"));
	//READ_COMPLEX_CODE(strCode, TEXT("sub_str"));
	//READ_ENDING_CODE(strCode);

	//NOP();

	return 0;
}