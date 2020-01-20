#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>


INT BuildFilePath(LPCTSTR VSInstallPath, LPCTSTR sourceRootDir, LPCTSTR libDir, CONST std::vector<CString> & excludedDir);
