#pragma once

#include <tchar.h>
#include <windows.h>

#ifdef __cplusplus
#include <atlbase.h>
#include <atlstr.h>
#include <cstdio>
#else
#include <stdio.h>
#endif



#define  SML_LOG(format, ...) _ftprintf_s(stdout, format, __VA_ARGS__)
#define  SML_LOG_LINE(format, ...) _ftprintf_s(stdout,  format TEXT(" @[%s %d] ") TEXT("\r\n"), __VA_ARGS__, TEXT(__FUNCTION__), __LINE__)