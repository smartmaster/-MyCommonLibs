#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicFileDeviceInterface.h"

VOID NtfsEnumDirEx(LPCTSTR pszFullPath, BOOL bShowFullPath, LPCTSTR pszIs, LPCTSTR pszIsNot);
HRESULT GetVolumeMountPoins(LPCTSTR pszFullPath);
HRESULT TestFindAndParseFileRecord(LPCTSTR volumePath, LPCTSTR filePath);


VOID NtfsEnumAll(IFileDevice * volumeFileDevice);