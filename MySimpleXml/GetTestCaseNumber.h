#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


VOID GetTestCaseNumber(int argc, TCHAR ** argv, int & testCase, LPCTSTR strCase = TEXT("-case"));
VOID GetTestCaseNumberPos(int argc, TCHAR ** argv, int & testCase, int pos = 1);