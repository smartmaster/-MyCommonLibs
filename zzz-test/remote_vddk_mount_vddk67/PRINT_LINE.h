#pragma once

#ifndef PRINT_LINE_A
//////////////////////////////////////////////////////////////////////////
#define PRINT_LINE_A(x, ...) fprintf_s(stdout, x "\r\n", __VA_ARGS__)
#endif