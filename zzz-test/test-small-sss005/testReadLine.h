#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <vector>
#include <locale>
#include <codecvt>

using namespace std;

class CTestReadLine
{
public:
	 
	

	 static void ReadLines(LPCWSTR fileName,
		const string & localeName,
		function<bool(const wstring &)> includeFilter,
		function<bool(const wstring &)> excludeFilter,
		vector<wstring> & lines
	)
	{

		wifstream inFile(fileName);
		if (localeName.size())
		{
			locale loc(localeName);
			inFile.imbue(loc);
		}
		else
		{
			inFile.imbue(std::locale(inFile.getloc(), new std::codecvt_utf8_utf16<wchar_t>()));
			//inFile.imbue(std::locale(inFile.getloc(), new std::codecvt_utf8<wchar_t>()));
			//inFile.imbue(std::locale(inFile.getloc(), new std::codecvt_utf16<wchar_t>()));
		}
		locale floc = inFile.getloc();
		cout << floc.name() << endl;
		if (inFile)
		{
			wstring line;
			while (getline(inFile, line))
			{
				bool bInclude = true;
				if (includeFilter)
				{
					bInclude = includeFilter(line);
				}

				bool bExclude = false;
				if (excludeFilter)
				{
					bExclude = excludeFilter(line);
				}

				if (bInclude && !bExclude)
				{
					lines.push_back(line);
					//wcout << line << endl;
				}
			}
		}

	}


	 static void Case0(int argc, WCHAR ** argv)
	 {
		 auto bInclude = [](const wstring & str)
		 {
			 return wcsstr(str.c_str(), L"Exception: Client received SOAP Fault from server:");
		 };

		 auto bExclude = [](const wstring & str)
		 {
			 return false;
		 };
		 vector<wstring> lines;
		 CStringA strA = argv[2];
		 ReadLines(argv[1], strA.GetString(), bInclude, bExclude, lines);
	 }
};