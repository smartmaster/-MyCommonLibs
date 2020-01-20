#pragma once

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicSerializeFieldDefine.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"


//////////////////////////////////////////////////////////////////////////
#define SEARCH_TAG L"SERIALIZE_"

//////////////////////////////////////////////////////////////////////////
#define SERIALIZE_TOKEN(x) SERIALIZE_TOKEN_##x
#define DEFINE_TOKEN(x) static CONST TCHAR SERIALIZE_TOKEN(x)[] = L#x

DEFINE_TOKEN(SERIALIZE_BEGIN_STRUCT);
DEFINE_TOKEN(SERIALIZE_END_STRUCT);

DEFINE_TOKEN(SERIALIZE_BASIC);
DEFINE_TOKEN(SERIALIZE_CSTRING);
DEFINE_TOKEN(SERIALIZE_STDSTRING);
DEFINE_TOKEN(SERIALIZE_FIXED_ARRAY);
DEFINE_TOKEN(SERIALIZE_RAW_ARRAY);
DEFINE_TOKEN(SERIALIZE_COMPLEX);
DEFINE_TOKEN(SERIALIZE_VECTOR);
DEFINE_TOKEN(SERIALIZE_COMPLEX_VECTOR);
DEFINE_TOKEN(SERIALIZE_VECTOR_2D);
DEFINE_TOKEN(SERIALIZE_COMPLEX_VECTOR_2D);
DEFINE_TOKEN(SERIALIZE_VECTOR_3D);
DEFINE_TOKEN(SERIALIZE_COMPLEX_VECTOR_3D);

struct  StreamConverter
{
	//////////////////////////////////////////////////////////////////////////
	static VOID WRITE_HEADER_CODE(CString & strCode, LPCTSTR StructName, LPCTSTR MethodName);

	static VOID WRITE_BASIC_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_CSTRING_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_STDSTRING_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_FIXED_ARRAY_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_RAW_ARRAY_FIELD_CODE(CString & strCode, LPCTSTR FieldName, LPCTSTR Length);
	static VOID WRITE_COMPLEX_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName );
	static VOID WRITE_COMPLEX_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName );

	static VOID WRITE_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName); //<sonmi01>2014-8-4 ###???
	static VOID WRITE_COMPLEX_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);

	//////////////////////////////////////////////////////////////////////////
	static VOID WRITE_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_COMPLEX_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	

	static VOID WRITE_ENDING_CODE(CString & strCode, LPCTSTR StructName, LPCTSTR MethodName);


	//////////////////////////////////////////////////////////////////////////
	static VOID READ_HEADER_CODE(CString & strCode, LPCTSTR StructName, LPCTSTR MethodName);

	static VOID READ_BASIC_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_CSTRING_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_STDSTRING_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_FIXED_ARRAY_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_RAW_ARRAY_FIELD_CODE(CString & strCode, LPCTSTR FieldName, LPCTSTR Length);
	static VOID READ_COMPLEX_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName );
	static VOID READ_COMPLEX_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName );

	static VOID READ_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_COMPLEX_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);

	//////////////////////////////////////////////////////////////////////////
	static VOID READ_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_COMPLEX_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);

	static VOID READ_ENDING_CODE(CString & strCode, LPCTSTR StructName, LPCTSTR MethodName);
};

struct  XmlStreamConverter
{
	//////////////////////////////////////////////////////////////////////////
	static VOID WRITE_HEADER_CODE(CString & strCode, LPCTSTR StructName, LPCTSTR MethodName);

	static VOID WRITE_BASIC_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_CSTRING_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_STDSTRING_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_FIXED_ARRAY_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_RAW_ARRAY_FIELD_CODE(CString & strCode, LPCTSTR FieldName, LPCTSTR Length);
	static VOID WRITE_COMPLEX_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName );
	static VOID WRITE_COMPLEX_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName );

	static VOID WRITE_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName); //<sonmi01>2014-8-4 ###???
	static VOID WRITE_COMPLEX_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);

	//////////////////////////////////////////////////////////////////////////
	static VOID WRITE_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID WRITE_COMPLEX_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);

	//////////////////////////////////////////////////////////////////////////
	static VOID WRITE_ENDING_CODE(CString & strCode, LPCTSTR StructName, LPCTSTR MethodName);


	//////////////////////////////////////////////////////////////////////////
	static VOID READ_HEADER_CODE(CString & strCode, LPCTSTR StructName, LPCTSTR MethodName);

	static VOID READ_BASIC_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_CSTRING_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_STDSTRING_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_FIXED_ARRAY_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_RAW_ARRAY_FIELD_CODE(CString & strCode, LPCTSTR FieldName, LPCTSTR Length);
	static VOID READ_COMPLEX_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName );
	static VOID READ_COMPLEX_VECTOR_FIELD_CODE( CString & strCode, LPCTSTR FieldName );

	static VOID READ_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_COMPLEX_VECTOR_2D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);

	//////////////////////////////////////////////////////////////////////////
	static VOID READ_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);
	static VOID READ_COMPLEX_VECTOR_3D_FIELD_CODE(CString & strCode, LPCTSTR FieldName);

	static VOID READ_ENDING_CODE(CString & strCode, LPCTSTR StructName, LPCTSTR MethodName);
};

//static HRESULT GetNameInBracket(CONST CString & strSource, IN OUT int & Start, vector<CString> & Tokens);

//HRESULT ParseStruct(CONST CString & strSource, IN OUT int & Start, OUT CString & strWriteCode, OUT CString & strReadCode);

//HRESULT ParseSource(CONST CString & strSource, CString & strWriteCode, CString & strReadCode);

HRESULT ParseSourceFile(LPCTSTR strInput, LPCTSTR strOutPut, BOOL bXml);