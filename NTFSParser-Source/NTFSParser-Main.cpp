// NTFSParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicGetTestCaseNumber.h"

#include "NtfsTest.h"


int _tmain(int argc, _TCHAR* argv[])
{
	D_SETTINGS(0, FALSE, TEXT(""), TEXT(""));

	int testCase = 0;
	GetTestCaseNumber(argc, argv, testCase, TEXT("-case"));

	switch (testCase)
	{
	case  2:
	{
		TestFindAndParseFileRecord(
			argv[1],//LPCTSTR volumePath, 
			argv[2]//LPCTSTR filePath
			);

	}
	break;
	case  1:
	{
		LPCTSTR pszFullPath = argv[1];
		GetVolumeMountPoins(pszFullPath);
	}
	break;
	case 0:
	{
		if (argc < 5)
		{
			D_INFO(0, TEXT("this.exe <pszFullPath> <bShowFullPath> <pszIs> <pszIsNot>"));
			return E_INVALIDARG;
		}

		LONG bShowFullPath = _tcstol(argv[2], NULL, 0);
		NtfsEnumDirEx(
			argv[1],//LPCTSTR pszFullPath, 
			bShowFullPath,//BOOL bShowFullPath, 
			argv[3],//LPCTSTR pszIs, 
			argv[4]//LPCTSTR pszIsNot
			);
	}
	break;
	}


	return 0;
}

