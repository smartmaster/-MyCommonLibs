#pragma once

#include "stdafx.h"
#include "ConstDef.h"

extern WCHAR				g_pLogFile[G_BUFFER_SIZE+1];

VOID DebugWriteStringV(CONST WCHAR * pFormat, ...);

#define DEBUG_LOG(pFormat, ...)		DebugWriteStringV(pFormat, __VA_ARGS__)