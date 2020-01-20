#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "log.h"


namespace SmartLib
{
	struct MyStruct
	{
		int ii1;
		double dd;
		int arr[100];
		char arr2[200];
	};


	class test_array_in_strut_addr
	{
	public:
		static void Case0()
		{
			MyStruct * p = new MyStruct();

			ULONG_PTR p1 = (ULONG_PTR)(PVOID)(p->arr);
			ULONG_PTR p2 = (ULONG_PTR)(PVOID)(&(p->arr[0]));
			ULONG_PTR p3 = (ULONG_PTR)(PVOID)(&(p->arr));

			LOG_LINE(TEXT("p1=%I64x, p2=%I64x, equal? %d"), (ULONGLONG)p1, (ULONGLONG)p2, (int)(p1 == p2));
			LOG_LINE(TEXT("p1=%I64x, p3=%I64x, equal? %d"), (ULONGLONG)p1, (ULONGLONG)p3, (int)(p1 == p3));
		}

	};

	
}