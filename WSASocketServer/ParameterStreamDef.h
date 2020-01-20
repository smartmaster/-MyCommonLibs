#pragma once

#include <tchar.h>
#include <windows.h>

//////////////////////////////////////////////////////////////////////////
#define IS_STRING_EQUAL_SZ(str1, str2)						(0 == wcscmp(str1, str2))
#define IS_STRING_EQUAL(str1, str2)							IS_STRING_EQUAL_SZ(str1, str2.c_str())

#define FIELD_NAME(MyField)									(L#MyField)
#define IS_FIELD_NAME_SZ(MyField, MyName)					(IS_STRING_EQUAL_SZ(FIELD_NAME(MyField), MyName))
#define IS_FIELD_NAME(MyField, MyName)						IS_FIELD_NAME_SZ(MyField, MyName.c_str())

//#define STRUCT_FIELD_NAME(MyStructure, MyField)				(L#MyStructure L"::" L#MyField)
//#define IS_STRUCT_FIELD_NAME(MyStructure, MyField, MyName)	(IS_STRING_EQUAL(STRUCT_FIELD_NAME(MyStructure, MyField), MyName))


//////////////////////////////////////////////////////////////////////////
#define MethodString(x)			METHOD_##x
#define MethodStringDef(x)		static CONST TCHAR MethodString(x)[] = L#x
#define ParameterStruct(x)		Parameter##x

//////////////////////////////////////////////////////////////////////////
static CONST TCHAR METHOD_META_NAME[] = TEXT("Method");






