#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "CommandList.h"

HRESULT CreateWin32FileHelper(
	CONST BODY_FILE_DESCRITION & FileDesc,
	IN ULONG ulDesiredAccess,
	IN ULONG ulShareMode,
	IN ULONG ulCreationDisposition,
	IN ULONG ulFlagsAndAttributes, 
	IFileDevice ** ppIFileDevice
	);
