#pragma once

#include <windows.h>
#include "SmlCVector.h"

EXTERN_C_START



const BYTE* sml_memmem(const BYTE* buffer1, int len1, const BYTE* buffer2, int len2);
const BYTE* sml_memrmem(const BYTE* buffer1, int len1, const BYTE* buffer2, int len2);

#include <pshpack1.h>
#ifdef _WIN64
typedef struct  Sml_JmpTagPatern
{
	/*
00007FF6EADAC3B2 41 B9 44 44 44 44    mov         r9d,44444444h
00007FF6EADAC3B8 41 B8 63 63 63 63    mov         r8d,63636363h
00007FF6EADAC3BE BA 42 42 42 42       mov         edx,42424242h
00007FF6EADAC3C3 B9 61 61 61 61       mov         ecx,61616161h
00007FF6EADAC3C8 E8 DA D2 F9 FF       call        JmpTag (07FF6EAD496A7h)
	*/
	BYTE movr9d1;
	BYTE movr9d2;
	DWORD tag4;

	BYTE movr8d1;
	BYTE movr8d2;
	DWORD tag3;


	BYTE movedx1;
	DWORD tag2;

	BYTE movecx1;
	DWORD tag1;

	BYTE call;
	int relativeoffset;
} Sml_JmpTagPatern, * PSml_JmpTagPatern;
#else
typedef struct  Sml_JmpTagPatern
{
	/*
0088C6B4 68 62 62 62 62       push        62626262h
0088C6B9 68 6C 6C 6C 6C       push        6C6C6C6Ch
0088C6BE 68 6D 6D 6D 6D       push        6D6D6D6Dh
0088C6C3 68 73 73 73 73       push        73737373h
0088C6C8 E8 63 50 FF FF       call        _Sml_JmpTag@16 (0881730h)
	*/
	BYTE movr9d1;
	DWORD tag4;

	BYTE movr8d1;
	DWORD tag3;


	BYTE movedx1;
	DWORD tag2;

	BYTE movecx1;
	DWORD tag1;

	BYTE call;
	int relativeoffset;
} Sml_JmpTagPatern, * PSml_JmpTagPatern;
#endif
#include <poppack.h>

#define  SML_ssss 'ssss'
#define  SML_mmmm 'mmmm'
#define  SML_llll 'llll'
#define  SML_bbbb 'bbbb'
#define SML_JMPTAGPATERN_BEGIN	SML_ssss, SML_mmmm, SML_llll, SML_bbbb

#define SML_SSSS 'SSSS'
#define SML_MMMM 'MMMM'
#define SML_LLLL 'LLLL'
#define SML_EEEE 'EEEE'
#define SML_JMPTAGPATERN_END	SML_SSSS, SML_MMMM, SML_LLLL, SML_EEEE

void Sml_JmpTagPatern_Init(Sml_JmpTagPatern* obj, int tag1, int tag2, int tag3, int tag4);

#include <pshpack1.h>
#ifdef _WIN64

typedef struct Sml_AddStackPointer
{
	//0097C638 83 C4 10             add         esp,10h
	//BYTE addesp1;
	//BYTE addesp2;
	//BYTE offset;
	BYTE notused;
} Sml_AddStackPointer, * PSml_AddStackPointer;

#define  Sml_AddStackPointer_Size 0

#else
typedef struct Sml_AddStackPointer 
{
	//0097C638 83 C4 10             add         esp,10h
	BYTE addesp1;
	BYTE addesp2;
	BYTE offset;
} Sml_AddStackPointer, *PSml_AddStackPointer;

#define  Sml_AddStackPointer_Size sizeof(Sml_AddStackPointer) 
#endif
#include <poppack.h>

void Sml_AddStackPointer_Init(Sml_AddStackPointer* obj, BYTE offset);
BYTE* Sml_SetAddStackPointer(BYTE* target, BYTE offset);

#if defined(_WIN64)
__declspec(noinline)
int Sml_JmpTag(int tag1, int tag2, int tag3, int tag4);
#else
__declspec(noinline)
int __stdcall Sml_JmpTag(int tag1, int tag2, int tag3, int tag4);
#endif

#include <pshpack1.h>
typedef struct Sml_Jmp2Target
{
	//00007FF705CF8253 E9 18 0D 02 00       jmp         TestAsm(07FF705D18F70h)
	BYTE jmp1;
	int relativeoffset;
} Sml_Jmp2Target, *PSml_Jmp2Target;
#include <poppack.h>
void Sml_Jmp2Target_Init(Sml_Jmp2Target* obj, BYTE* src, BYTE* target);
void Sml_SetJmpFromTo(BYTE* from, BYTE* to);

__declspec(noinline)
void Sml_FindJmps(PSmlCVector vec_blocks, PSmlCVector vec_jmp);

__declspec(noinline)
void Sml_LinkAndRunCleanups(LONG localIniting, LONG volatile* globalInited, SmlCVector* vec_jmp, LPBYTE* pretddr);

__declspec(noinline)
BYTE* Sml_GetRetAddr(SmlCVector* vec_blocks, long initing_local, BYTE** pretaddr);

EXTERN_C_END