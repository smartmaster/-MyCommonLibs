#pragma once

#include<tchar.h>
#include <vector>
#include <string>

#include "CommonDef.h"

namespace SmartLib
{
	class CStringVersionHelpers
	{
	public:
		static void StringToVersion(const wchar_t * strVer, std::vector<int>& ver)
		{
			const wchar_t * start = strVer;
			wchar_t * end = nullptr;
			for (;;)
			{
				long lVal = wcstol(start, &end, 10);
				ver.push_back(lVal);
				if (*end == 0 || *end != L'.')
				{
					break;
				}
				start = ++end;
				end = nullptr;
			}
		}

		static int CompareVersion(std::vector<int> & ver1, std::vector<int> & ver2)
		{
			int diff = ver1.size() - ver2.size();
			if (diff > 0)
			{
				for (int ii = 0; ii < diff; ++ii)
				{
					ver2.push_back(0);
				}
			}
			else if (diff < 0)
			{
				diff = -diff;
				for (int ii = 0; ii < diff; ++ii)
				{
					ver1.push_back(0);
				}
			}

			for (size_t ii = 0; ii < ver1.size(); ii++)
			{
				if (ver1[ii] != ver2[ii])
				{
					return ver1[ii] - ver2[ii];
				}
			}

			return 0;
		}

	public:
		static const std::wstring & VersionToString(const std::vector<int>& ver, std::wstring & strVer)
		{
			wchar_t buffer[64] = { 0 };
			for (size_t ii = 0; ii < ver.size(); ii++)
			{
				buffer[0] = 0;
				strVer += _itow(ver[ii], buffer, 10);
				if (ii != ver.size() - 1)
				{
					strVer += L".";
				}
			}

			return strVer;
		}
		
		static int CompareStringVersion(const wchar_t * strVer1, const wchar_t * strVer2)
		{
			std::vector<int> ver1;
			StringToVersion(strVer1, ver1);

			std::vector<int> ver2;
			StringToVersion(strVer2, ver2);

			return CompareVersion(ver1, ver2);
		}

		//IN Microsoft.Ancm.IISExpress.Msi,version=1.0.1972,chip=x64
		//OUT Microsoft.Ancm.IISExpress.Msi,version=*,chip=x64
		//OUT 1.0.1972
		static void ConvertFileName(const wchar_t * fileName, std::wstring & filePattern, std::wstring & fileVersion)
		{
			static const wchar_t VER_IND[] = L",version=";
			const wchar_t * pstr = wcsstr(fileName, VER_IND);
			if (pstr != nullptr)
			{
				pstr += (_countof(VER_IND) - 1);

				filePattern.append(fileName, pstr - fileName);
				filePattern += L'*';

				while (isdigit(*pstr) || L'.' == *pstr)
				{
					fileVersion += *pstr;
					++ pstr;
				}

				filePattern += pstr;
			}
		}

		static int CompareFileNameVersion(const wchar_t * file1, const wchar_t * file2)
		{
			std::wstring filePattern1, fileVersion1;
			ConvertFileName(file1, filePattern1, fileVersion1);

			std::wstring filePattern2, fileVersion2;
			ConvertFileName(file2, filePattern2, fileVersion2);

			return ( 0 == wcsicmp(filePattern1.c_str(), filePattern2.c_str()) ?
				CompareStringVersion(fileVersion1.c_str(), fileVersion2.c_str()) :
				0);
		}
	};
} //namespace SmartLib