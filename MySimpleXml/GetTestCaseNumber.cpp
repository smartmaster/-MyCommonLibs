#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

VOID GetTestCaseNumber(int argc, TCHAR ** argv, int & testCase, LPCTSTR strCase = TEXT("-case"))
{
	for (int ii = 0; ii < argc; ++ ii)
	{
		if (0 == _tcsicmp(strCase, argv[ii]))
		{
			if (ii + 1 < argc)
			{
				testCase = _tcstol(argv[ii + 1], nullptr, 0);
			}
			break;
		}
	}
}

VOID GetTestCaseNumberPos(int argc, TCHAR ** argv, int & testCase, int pos = 1)
{
	if (pos < argc)
	{
		testCase = _tcstol(argv[pos], nullptr, 0);
	}
}