// CleanupOldSetupFiles.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <tchar.h>

#include "StringVersionHelpers.h"
#include "FindOlderVersions.h"

int wmain(int argc, wchar_t ** argv)
{
	int testCase = 0;
	if (argc > 1)
	{
		testCase = wcstol(argv[1], nullptr, 10);
	}

	switch (testCase)
	{
	case 1: //this.exe 1 rootDir
	{
		SmartLib::CFindOlderVersions fov;
		fov.SetRoot(argv[2]);
		fov.Process();
		fwprintf_s(stdout, L"Process Y to cleanup" L"\r\n");
		int ch = getwchar();
		if (ch == L'Y' || ch == L'y')
		{
			fov.Cleanup();
		}
	}
	break;
	case 0:
	{
		std::vector<int> ver1;
		SmartLib::CStringVersionHelpers::StringToVersion(L"00123", ver1);

		std::vector<int> ver2;
		SmartLib::CStringVersionHelpers::StringToVersion(L"00123.00456.00789", ver2);

		std::vector<int> ver3;
		SmartLib::CStringVersionHelpers::StringToVersion(L"00123.00456.00789,9999", ver3);

		int cmp = SmartLib::CStringVersionHelpers::CompareVersion(ver1, ver2);
		std::wstring strVer1, strVer2;
		fwprintf_s(stdout, L"%s is %s than %s" L"\r\n", 
			SmartLib::CStringVersionHelpers::VersionToString(ver1, strVer1).c_str(),
			cmp > 0 ? L"greater" : L"less", 
			SmartLib::CStringVersionHelpers::VersionToString(ver2, strVer2).c_str());


		//const wchar_t * hello = SmartLib::CStringVersionHelpers::retSTring().c_str();
		//_ftprintf_s(stdout, L"%s %p" L"\r\n", hello, hello);

		//const wchar_t * hello1 = SmartLib::CStringVersionHelpers::retSTring().c_str();
		//_ftprintf_s(stdout, L"%s %p" L"\r\n", hello1, hello1);


		std::wstring filePattern1, fileVersion1;
		SmartLib::CStringVersionHelpers::ConvertFileName(L"Microsoft.Ancm.IISExpress.Msi,version=1.0.1972,chip=x64", filePattern1, fileVersion1);
		fwprintf_s(stdout, L"%s and %s" L"\r\n", filePattern1.c_str(), fileVersion1.c_str());

		std::wstring filePattern2, fileVersion2;
		SmartLib::CStringVersionHelpers::ConvertFileName(L"Microsoft.Ancm.IISExpress.Msi,chip=x64,version=1.0.1972", filePattern2, fileVersion2);
		fwprintf_s(stdout, L"%s and %s" L"\r\n", filePattern2.c_str(), fileVersion2.c_str());
		

	}
	break;
	default:
		break;
	}
    return 0;
}

