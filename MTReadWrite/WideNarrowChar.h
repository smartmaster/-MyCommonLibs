#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <string>
#include <codecvt>



HRESULT Utf16To8String(CONST CStringW & strUtf16, CStringA & strUtf8);
HRESULT Utf8To16String(CONST CStringA & strUtf8, CStringW & strUtf16);


namespace SmartLib
{
	VOID Utf16To8StdString(CONST std::wstring & strUtf16, std::string & strUtf8);
	VOID Utf8To16StdString(CONST std::string & strUtf8, std::wstring & strUtf16);
} //namespace SmartLib