#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "log.h"

namespace SmartLib
{
	class CBstrConv
	{
	public:
		static void Case0()
		{
			CComBSTR bstr;
			LPCTSTR ptr = (LPCTSTR)(bstr);
			if (nullptr == ptr)
			{
				SML_LOG_LINE(TEXT("got null pointer"));
			}
			else if (0 == ptr[0])
			{
				SML_LOG_LINE(TEXT("got empty string"));
			}
			else
			{
				SML_LOG_LINE(TEXT("got non-empty string"));
			}

			WCHAR str[16] = { 0 };
			//_tcscpy_s(str, ptr);
		}
	};
}