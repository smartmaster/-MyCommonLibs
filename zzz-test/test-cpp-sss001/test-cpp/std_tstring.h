#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string>

#ifdef  _UNICODE
using std_tstring = std::wstring;
#else
using std_tstring = std::string;
#endif 
