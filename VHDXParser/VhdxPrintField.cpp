#include "stdafx.h"
#include "VhdxHelper.h"
#include "VhdxPrintField.h"



//////////////////////////////////////////////////////////////////////////
//template<>
VOID PrintField(LPCTSTR FiledName, UINT64 Val)
{
	D_INFO(0, TEXT("%s=%I64d(0x%016I64x)"), FiledName, Val, Val);
}

//template<>
VOID PrintField(LPCTSTR FiledName, INT64 Val)
{
	D_INFO(0, TEXT("%s=%I64d(0x%016I64x)"), FiledName, Val, Val);
}

//template<>
VOID PrintField(LPCTSTR FiledName, UINT32 Val)
{
	D_INFO(0, TEXT("%s=%d(0x%08x)"), FiledName, Val, Val);
}

//template<>
VOID PrintField(LPCTSTR FiledName, INT32 Val)
{
	D_INFO(0, TEXT("%s=%d(0x%08x)"), FiledName, Val, Val);
}

//template<>
VOID PrintField(LPCTSTR FiledName, UINT16 Val)
{
	D_INFO(0, TEXT("%s=%d(0x%08x)"), FiledName, Val, Val);
}

//template<>
VOID PrintField(LPCTSTR FiledName, INT16 Val)
{
	D_INFO(0, TEXT("%s=%d(0x%08x)"), FiledName, Val, Val);
}

//template<>
VOID PrintField(LPCTSTR FiledName, UINT8 Val)
{
	D_INFO(0, TEXT("%s=%d(0x%08x)"), FiledName, Val, Val);
}

//template<>
VOID PrintField(LPCTSTR FiledName, INT8 Val)
{
	D_INFO(0, TEXT("%s=%d(0x%08x)"), FiledName, Val, Val);
}

//template<>
VOID PrintField(LPCTSTR FiledName, CONST GUID & guid)
{
	TCHAR Buffer[64] = {0};
	D_INFO(0, TEXT("%s=%s"), FiledName, GuidToStringHelper(guid, Buffer, _countof(Buffer)));
}

//template<>
VOID PrintField(LPCTSTR FiledName, LPCTSTR str)
{
	D_INFO(0, TEXT("%s=%s"), FiledName, str);
}

