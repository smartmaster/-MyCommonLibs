#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

template<typename _Type>
ULONGLONG PtrToUll(_Type * p)
{
	return (ULONGLONG)(ULONG_PTR)(p);
}


template<typename _Type>
_Type* UllToPtr(ULONGLONG ull)
{
	return (_Type*)(ULONG_PTR)(ull);
}