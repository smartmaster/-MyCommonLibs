#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>



#define LOG(fmt, ...) _ftprintf_s(stdout, fmt, __VA_ARGS__)
#define LOG_LINE(fmt, ...) _ftprintf_s(stdout, fmt TEXT("\r\n"), __VA_ARGS__)


