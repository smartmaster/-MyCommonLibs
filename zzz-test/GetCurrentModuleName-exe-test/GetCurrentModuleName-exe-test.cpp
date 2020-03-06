// GetCurrentModuleName-exe-test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\PublicHeader\PublicGetCurrentModuleName.h"
#pragma comment(lib, "E:/MyProjects/MyCommonLibs/PublicLib/ReleaseX86/GetCurrentModuleName.lib")

int dll_name();
#pragma comment(lib, "E:/MyProjects/MyCommonLibs/GetCurrentModuleName-dll-test/Debug/GetCurrentModuleName-dll-test.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	LPCTSTR p1 = GetCurrentModuleName();
	LPCTSTR p2 = GetCurrentModuleName();
	LPCTSTR p3 = GetCurrentModuleName();
	LPCTSTR p4 = GetCurrentModuleName();
	dll_name();
	return 0;
}
