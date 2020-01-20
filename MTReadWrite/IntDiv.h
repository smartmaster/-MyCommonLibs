#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

struct IntDiv
{
	//x/y = n.000d
	static VOID Div(LONGLONG x, LONGLONG y, LONG p, LONGLONG & n, LONGLONG & d);
	static VOID Div(LONGLONG x, LONGLONG y, LONG p, CString & str);

	static INT TestCases();
};
