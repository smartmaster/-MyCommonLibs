#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>


#include "..\PublicHeader\PublicDTraceLib.h"

//////////////////////////////////////////////////////////////////////////
#define PRINT_FIELD(fieldName) PrintField(L#fieldName, fieldName)

//////////////////////////////////////////////////////////////////////////
VOID PrintField(LPCTSTR FiledName, UINT64 Val);
VOID PrintField(LPCTSTR FiledName, INT64 Val);
VOID PrintField(LPCTSTR FiledName, UINT32 Val);
VOID PrintField(LPCTSTR FiledName, INT32 Val);
VOID PrintField(LPCTSTR FiledName, UINT16 Val);
VOID PrintField(LPCTSTR FiledName, INT16 Val);
VOID PrintField(LPCTSTR FiledName, UINT8 Val);
VOID PrintField(LPCTSTR FiledName, INT8 Val);
VOID PrintField(LPCTSTR FiledName, CONST GUID & guid);
VOID PrintField(LPCTSTR FiledName, LPCTSTR str);

