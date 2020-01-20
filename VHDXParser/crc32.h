#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

UINT32 crc32c(UINT32 crc, const BYTE *data, unsigned int length);