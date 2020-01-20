#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>




#define LOG_W(str, ...) fwprintf_s(stdout, str, __VA_ARGS__)
#define LOG_LINE_W(str, ...) fwprintf_s(stdout, str L"\r\n", __VA_ARGS__)

#define LOG_A(str, ...) fprintf_s(stdout, str, __VA_ARGS__)
#define LOG_LINE_A(str, ...) fprintf_s(stdout, str "\r\n", __VA_ARGS__)



#ifdef _UNICODE
#define LOG_T LOG_W
#define LOG_LINE_T LOG_LINE_W
#else
#define LOG_T LOG_A
#define LOG_LINE_T LOG_LINE_A
#endif