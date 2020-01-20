#pragma once


#include <windows.h>
#include "SmlCVector.h"

EXTERN_C_START

#if defined(_WIN64)
__declspec(noinline)
SML_DATA_PTR Sml_AsmGetRetAddr(PSmlCVector vec, int enabled, int reserved1, int reserved2);

__declspec(noinline)
void Sml_AsmLinkAndRunCleanups(LONG localIniting, LONG volatile* globalInited, SmlCVector* vec_jmp);
#else
__declspec(noinline)
SML_DATA_PTR /*__stdcall*/ Sml_AsmGetRetAddr(PSmlCVector vec, int enabled, int reserved1, int reserved2);

__declspec(noinline)
void /*__stdcall*/ Sml_AsmLinkAndRunCleanups(LONG localIniting, LONG volatile* globalInited, SmlCVector* vec_jmp);
#endif

EXTERN_C_END

