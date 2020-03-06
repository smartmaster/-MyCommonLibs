// GetCurrentModuleName.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "GetCurrentModuleName.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CString strModule = GetCurrentModuleName();
	CString strModule1 = GetCurrentModuleName();
	CString strModule2 = GetCurrentModuleName();
	CString strModule3 = GetCurrentModuleName();
	CString strModule4 = GetCurrentModuleName();
	return 0;
}

