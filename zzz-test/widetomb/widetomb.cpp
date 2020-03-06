// widetomb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "WideNarrowChar.h"

void test()
{
	WCHAR strW[] = L"Œ“a√«";

	CStringA strUtf8;
	Utf16To8String(strW, strUtf8);
	int size = strUtf8.GetLength();

	CStringW strUtf16;
	Utf8To16String(strUtf8, strUtf16);
	size = strUtf16.GetLength();

	return;
}


int _tmain(int argc, _TCHAR* argv[])
{
	test();
	return 0;
}

