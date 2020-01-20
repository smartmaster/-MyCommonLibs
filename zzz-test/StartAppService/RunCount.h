#pragma once

#include <tchar.h>
#include <Windows.h>

#include <atlstr.h>

///*static*/ HRESULT __GetModulePath(HMODULE hMod, BOOL bDir, CString & strPath);
///*static*/ INT __ReadRestartCount(LPCTSTR pIniFile);
///*static*/ HRESULT __WriteRestartCount(LPCTSTR pIniFile, INT RestartCount);

INT DecrementRestartCount(INT DefaultRestartCount);