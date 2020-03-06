// GetCurrentModuleName-dll-test.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#include "..\PublicHeader\PublicGetCurrentModuleName.h"
//#pragma comment(lib, "E:/MyProjects/MyCommonLibs/PublicLib/ReleaseX86/GetCurrentModuleName.lib")


int dll_name()
{
	LPCTSTR p1 = GetCurrentModuleName();
	LPCTSTR p2 = GetCurrentModuleName();
	LPCTSTR p3 = GetCurrentModuleName();
	LPCTSTR p4 = GetCurrentModuleName();
	return 0;
}