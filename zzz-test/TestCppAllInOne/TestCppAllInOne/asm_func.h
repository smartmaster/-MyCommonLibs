#pragma once


#include <windows.h>
#include "SmlCVector.h"

EXTERN_C_START

#if defined(_WIN64)
__declspec(noinline)
SML_DATA_PTR Sml_AsmGetRetAddr(PSmlCVector vec, int enabled, int reserved1, int reserved2);
//void Sml_PrepareJmpEnd(BYTE* src, BYTE* target, IN OUT volatile long* inited);

__declspec(noinline)
void Sml_AsmRunCleanup(BYTE* first, BYTE* last, IN OUT volatile long* inited);
#else
__declspec(noinline)
SML_DATA_PTR __stdcall Sml_AsmGetRetAddr(PSmlCVector vec, int enabled, int reserved1, int reserved2);

__declspec(noinline)
void __stdcall Sml_AsmRunCleanup(BYTE* first, BYTE* last, IN OUT volatile long* inited);
#endif

EXTERN_C_END

