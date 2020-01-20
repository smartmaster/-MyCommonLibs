#pragma once

#include <stdio.h>


#define LOG_LINE(format, ...) fwprintf(stdout, format L"\r\n", __VA_ARGS__)
#define LOG(format, ...) fwprintf(stdout, format, __VA_ARGS__)

#define LOG_LINE_A(format, ...) fprintf(stdout, format "\r\n", __VA_ARGS__)
#define LOG_A(format, ...) fprintf(stdout, format, __VA_ARGS__)