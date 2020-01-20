#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

namespace SmartLib
{
	class testsprintf
	{
	public:
		static void Case0()
		{
			CString str;
			LPCTSTR psz = str.GetString();
			char rootDirectory[256] = { 0 };
			sprintf_s(rootDirectory, _countof(rootDirectory),
				"\\\\%S\\RAW", str.GetString());
			printf(rootDirectory);
		}
	};
}