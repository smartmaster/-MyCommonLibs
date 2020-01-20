#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


void DllFunc(int iCommand, const void * iBuffer, int iSize, int * oCommand, void * oBuffer, int oSize);
