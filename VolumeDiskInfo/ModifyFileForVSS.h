#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

HRESULT ModifyFileForDiskShadow(LPCTSTR tagetFile);
HRESULT ModifyFileForVShadow(LPCTSTR tagetFile);