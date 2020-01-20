#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>



HRESULT Utf16To8String(CONST CStringW & strUtf16, CStringA & strUtf8);
HRESULT Utf8To16String(CONST CStringA & strUtf8, CStringW & strUtf16);