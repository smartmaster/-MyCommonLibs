#pragma once

#include <windows.h>
#include "SmlCVector.h"

EXTERN_C_START



const BYTE* sml_memmem(const BYTE* buffer1, int len1, const BYTE* buffer2, int len2);
const BYTE* sml_memrmem(const BYTE* buffer1, int len1, const BYTE* buffer2, int len2);

#include <pshpack1.h>
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
} Sml_JmpTagPatern, *PSml_JmpTagPatern;
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


#if defined(_WIN64)
void Sml_JmpTag(int tag1, int tag2, int tag3, int tag4);
#else
void __stdcall Sml_JmpTag(int tag1, int tag2, int tag3, int tag4);
#endif

#include <pshpack1.h>
typedef struct Sml_Jmp2Target
{
	//00007FF705CF8253 E9 18 0D 02 00       jmp         TestAsm(07FF705D18F70h)
	BYTE jmp1;
	int relativeoffset;
} Sml_Jmp2Target, *PSml_Jmp2Target;
#include <poppack.h>

void Sml_Jmp2TargetJmp2Target_Init(PSml_Jmp2Target obj, BYTE* src, BYTE * target);

//#include <pshpack1.h>
//typedef struct Sml_Jmp2End
//{
//	/*
//	00007FF71C737F62 FF 24 24             jmp         qword ptr [rsp]
//	00007FF71C737F65 FF 64 24 F8          jmp         qword ptr [rsp-8]
//	00007FF71C737F69 FF 64 24 F0          jmp         qword ptr [rsp-10h]
//	00007FF71C737F6D FF 64 24 E8          jmp         qword ptr [rsp-18h]
//	00007FF71C737F71 FF 64 24 E0          jmp         qword ptr [rsp-20h]
//	*/
//	//00007FF71C737F65 FF 64 24 F8          jmp         qword ptr [rsp-8]
//	BYTE jmp1;
//	BYTE jmp2;
//	BYTE jmp3;
//	BYTE Offset;
//} Sml_Jmp2End, *PSml_Jmp2End;
//#include <poppack.h>

//void Sml_Jmp2End_Init(Sml_Jmp2End* obj);

void Sml_PrepareCleanup(PSmlCVector vec, PSmlCVector vecjmp);

__declspec(noinline)
void Sml_PrepareJmpEnd(BYTE* src, BYTE* target, IN OUT volatile long* inited);

EXTERN_C_END