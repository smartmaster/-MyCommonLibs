#include <assert.h>

#include "raii_func.h"
#include "SmlCVector.h"

//static int mymemcmp(const BYTE* buffer11, const  BYTE* buffer22, int size)
//{
//	int ret = 0;
//	BYTE xx = buffer11[0] - buffer22[0];
//	for (int ii = 0; ii < size; ii++)
//	{
//		ret = (int)(buffer11[ii]) - (int)(buffer22[ii]);
//		if (ret)
//		{
//			break;
//		}
//	}
//	return ret;
//}

const BYTE* sml_memmem(const BYTE* buffer1, int len1, const BYTE* buffer2, int len2)
{
	const BYTE* ret = NULL;
	for (int ii = 0; ii <= len1 - len2; ++ii)
	{
		if (0 == memcmp(buffer1 + ii, buffer2, len2))
		{
			ret = buffer1 + ii;
			break;
		}
	}
	return ret;
}


const BYTE* sml_memrmem(const BYTE* buffer1, int len1, const BYTE* buffer2, int len2)
{
	const BYTE* ret = NULL;
	for (int ii = len1 - len2; ii >= 0; --ii)
	{
		if (0 == memcmp(buffer1 + ii, buffer2, len2))
		{
			ret = buffer1 + ii;
			break;
		}
	}
	return ret;
}


void Sml_JmpTagPatern_Init(Sml_JmpTagPatern* obj, int tag1, int tag2, int tag3, int tag4)
{
	obj->movr9d1 = 0x41;
	obj->movr9d2 = 0xB9;
	obj->tag4 = tag4;

	obj->movr8d1 = 0x41;
	obj->movr8d2 = 0xB8;
	obj->tag3 = tag3;

	obj->movedx1 = 0xBA;
	obj->tag2 = tag2;

	obj->movecx1 = 0xB9;
	obj->tag1 = tag1;

	obj->call = 0xE8;
	obj->relativeoffset = 0;
}

void Sml_Jmp2TargetJmp2Target_Init(Sml_Jmp2Target* obj, BYTE* src, BYTE* target)
{
	obj->jmp1 = 0xE9;
	obj->relativeoffset = (int)(target - src - sizeof(Sml_Jmp2Target));
}

//void Sml_Jmp2End_Init(Sml_Jmp2End* obj)
//{
//	obj->jmp1 = 0xFF;
//	obj->jmp2 = 0x64;
//	obj->jmp3 = 0x24;
//	obj->Offset = 0xF8;
//}
#pragma optimize( "", off )
#if defined(_WIN64)
void Sml_JmpTag(int tag1, int tag2, int tag3, int tag4)
#else
void __stdcall Sml_JmpTag(int tag1, int tag2, int tag3, int tag4)
#endif
{
	static volatile int some_data = 0xabcd'5678;
	some_data += tag1;
	some_data += tag2;
	some_data += tag3;
	some_data += tag4;
	return some_data;
}
#pragma optimize( "", on )

static void Sml_ReadWriteBuffer(const BYTE * buffer, int size)
{
	DWORD dw = 0;
	int ii = 0;
	for (; ii < size / sizeof(DWORD); ii += sizeof(DWORD))
	{
		CopyMemory(&dw, buffer + ii, sizeof(DWORD));
		CopyMemory(buffer + ii, &dw, sizeof(DWORD));
	}

	if (size % sizeof(DWORD))
	{
		CopyMemory(&dw, buffer + ii, size % sizeof(DWORD));
		CopyMemory(buffer + ii, &dw, size % sizeof(DWORD));
	}
}


int Sml_compare(BYTE * *arg1, BYTE * *arg2)
{
	return (int)((*arg1) - (*arg2));
}

//static BYTE* Sml_FindPattern(int revserse, BYTE* buffer, int len, BYTE* func, int tag1, int tag2, int tag3, int tag4)
//{
//
//	/*
//	00007FF69CC2319A BA 6D 6D 6D 6D       mov         edx,6D6D6D6Dh
//	00007FF69CC2319F B9 73 73 73 73       mov         ecx,73737373h
//	00007FF69CC231A4 41 B9 62 62 62 62    mov         r9d,62626262h
//	00007FF69CC231AA 41 B8 6C 6C 6C 6C    mov         r8d,6C6C6C6Ch
//	00007FF69CC231B0 E8 FB 30 FF FF       call        Sml_JmpTag (07FF69CC162B0h)
//	*/
//	BYTE* arr[4] = { 0 };
//	arr[0] = revserse ? sml_memrmem(buffer, len, &tag1, sizeof(DWORD)) : sml_memmem(buffer, len, &tag1, sizeof(DWORD));
//	arr[1] = revserse ? sml_memrmem(buffer, len, &tag2, sizeof(DWORD)) : sml_memmem(buffer, len, &tag2, sizeof(DWORD));
//	arr[2] = revserse ? sml_memrmem(buffer, len, &tag3, sizeof(DWORD)) : sml_memmem(buffer, len, &tag3, sizeof(DWORD));
//	arr[3] = revserse ? sml_memrmem(buffer, len, &tag4, sizeof(DWORD)) : sml_memmem(buffer, len, &tag4, sizeof(DWORD));
//
//	qsort(arr, _countof(arr), sizeof(BYTE*), Sml_compare);
//
//	BYTE* ret = NULL;
//	
//	BOOL ok = TRUE;
//	if (ok)
//	{
//		ok = (arr[0] >= buffer);
//	}
//
//	int diff = 0;
//	if (ok)
//	{
//		int patternSize = (int)(arr[3] - arr[0]) + sizeof(DWORD) + sizeof(Sml_Jmp2Target);
//		diff = sizeof(Sml_JmpTagPatern) - patternSize;
//		ok = (1 == diff || 2 == diff);
//	}
//
//	BYTE* targetfunc = NULL;
//	if (ok)
//	{
//		Sml_Jmp2Target* callfunc = (Sml_Jmp2Target*)(arr[3] + sizeof(DWORD));
//		ok = (callfunc->jmp1 == 0xE8);
//		if (ok)
//		{
//			targetfunc = (BYTE*)callfunc + sizeof(Sml_Jmp2Target) + callfunc->relativeoffset;
//		}
//	}
//
//	if (ok)
//	{
//		ok = (targetfunc == func);
//	}
//
//	if (ok)
//	{
//		ret = arr[0] - diff;
//	}
//
//	return ret;
//}


BYTE* Sml_FindPlaceholderPattern(int revserse, BYTE * buffer, int len, BYTE* func, Sml_JmpTagPatern * pattern)
{
	BYTE* arr[4] = { 0 };
	int arrsize[4] = { 0 };
	arr[0] = revserse ? sml_memrmem(buffer, len, &pattern->movr9d1, 6) : sml_memmem(buffer, len, &pattern->movr9d1, 6);
	arr[1] = revserse ? sml_memrmem(buffer, len, &pattern->movr8d1, 6) : sml_memmem(buffer, len, &pattern->movr8d1, 6);
	arr[2] = revserse ? sml_memrmem(buffer, len, &pattern->movedx1, 5) : sml_memmem(buffer, len, &pattern->movedx1, 5);
	arr[3] = revserse ? sml_memrmem(buffer, len, &pattern->movecx1, 5) : sml_memmem(buffer, len, &pattern->movecx1, 5);

	qsort(arr, _countof(arr), sizeof(BYTE*), Sml_compare);

	BOOL ok = (arr[0] >= buffer);

	if (ok)
	{
		int totalSize = 0;
		for (int ii = 0; ii < _countof(arr); ++ii)
		{
			arrsize[ii] = 1024 * 1024; //set a very large size
			switch (arr[ii][0])
			{
			case 0xBA:
			{
				arrsize[ii] = 5;
			}
			break;
			case 0xB9:
			{
				arrsize[ii] = 5;
			}
			break;
			case 0x41:
			{
				switch (arr[ii][1])
				{
				case 0xB8:
				{
					arrsize[ii] = 6;
				}
				break;
				case 0xB9:
				{
					arrsize[ii] = 6;
				}
				}
			}
			}

			totalSize += arrsize[ii];
		}

		if (ok)
		{
			ok = (totalSize == FIELD_OFFSET(Sml_JmpTagPatern, call));
		}


	}
	
	if (ok)
	{
		for (int ii = 0; ii < _countof(arr) - 1; ++ii)
		{
			if (arr[ii] + arrsize[ii] != arr[ii + 1])
			{
				ok = FALSE;
				break;
			}
		}
	}
	

	if (ok)
	{
		Sml_JmpTagPatern* callfunc = arr[0];
		ok = (callfunc->call == 0xE8);
		if (ok)
		{
			BYTE * targetfunc = (BYTE*)(&callfunc->call) + 5 + callfunc->relativeoffset;
			ok = (targetfunc == func);
		}
	}

	return ok ? arr[0] : NULL;
}

void Sml_PrepareCleanup(PSmlCVector vec_blocks, PSmlCVector vec_jmp)
{
	int vecsize = SmlCVector_Size(vec_blocks);
	assert(0 == (vecsize & 1)); //vecsize is even

	//2019-4-26 //!!@@##
	Sml_JmpTagPatern pattern1;
	Sml_JmpTagPatern_Init(&pattern1, SML_JMPTAGPATERN_BEGIN);

	Sml_JmpTagPatern pattern2;
	Sml_JmpTagPatern_Init(&pattern2, SML_JMPTAGPATERN_END);


	for (int ii = vecsize - 1; ii >= 0; ii -= 2)
	{
		BYTE* buffer1 = SmlCVector_Get(vec_blocks, ii - 1);
		BYTE * buffer2 = SmlCVector_Get(vec_blocks, ii);
		int bufferlen = buffer2 - buffer1;

		DWORD OldProtect = 0;
		DWORD NewProtect = PAGE_EXECUTE_READWRITE;
		BOOL ok = VirtualProtect(
			buffer1,//_In_   LPVOID lpAddress,
			bufferlen,//_In_   SIZE_T dwSize,
			NewProtect,//_In_   DWORD flNewProtect,
			&OldProtect//,//_Out_  PDWORD lpflOldProtect
		);

		Sml_ReadWriteBuffer(buffer1, bufferlen);

		//2019-4-26 //!!@@##
		//BYTE* jmp1 = sml_memmem(buffer1, bufferlen, &pattern1, sizeof(Sml_JmpTagPatern) - sizeof(DWORD));
		//BYTE* jmp2 = sml_memrmem(buffer1, bufferlen, &pattern2, sizeof(Sml_JmpTagPatern) - sizeof(DWORD));
		//BYTE * jmp1 = Sml_FindPattern(0, buffer1, bufferlen, Sml_JmpTag, SML_JMPTAGPATERN_BEGIN);
		//BYTE * jmp2 = Sml_FindPattern(1, buffer1, bufferlen, Sml_JmpTag, SML_JMPTAGPATERN_END);

		BYTE* jmp1 = Sml_FindPlaceholderPattern(0, buffer1, bufferlen, Sml_JmpTag, &pattern1);
		BYTE* jmp2 = Sml_FindPlaceholderPattern(1, buffer1, bufferlen, Sml_JmpTag, &pattern2);

		assert(jmp1 && jmp2 && jmp1 < jmp2);


		SmlCVector_Push(vec_jmp, jmp1);
		SmlCVector_Push(vec_jmp, jmp2);
	}

	vecsize = SmlCVector_Size(vec_jmp);
	for (int ii = 0; ii < vecsize; ii += 2)
	{
		BYTE* jmp1 = SmlCVector_Get(vec_jmp, ii);
		BYTE* jmp2 = SmlCVector_Get(vec_jmp, ii + 1);
		int bufferlen = jmp2 - jmp1;


		memset(jmp1, 0x90, sizeof(Sml_JmpTagPatern)); //all set nop
		memset(jmp2, 0x90, sizeof(Sml_JmpTagPatern));

		Sml_Jmp2Target j2t;

		if (ii + 2 != vecsize) //do not process the last pair now
		{
			BYTE* target = SmlCVector_Get(vec_jmp, ii + 2);
			Sml_Jmp2TargetJmp2Target_Init(&j2t, jmp2, target + sizeof(Sml_JmpTagPatern)); //+ sizeof(Sml_JmpTagPatern) to skip nops
			memcpy(jmp2, &j2t, sizeof(Sml_Jmp2Target));
		}
		//else
		//{
		//	Sml_Jmp2End j2end;
		//	Sml_Jmp2End_Init(&j2end);
		//	memcpy(jmp2, &j2end, sizeof(Sml_Jmp2End));
		//}
	}
}


void Sml_PrepareJmpEnd(BYTE * src, BYTE * target, IN OUT volatile long* inited)
{
	if (0 == *inited)
	{
		if (src && target)
		{
			Sml_Jmp2Target j2t;
			Sml_Jmp2TargetJmp2Target_Init(&j2t, src, target);
			memcpy(src, &j2t, sizeof(Sml_Jmp2Target));
		}
		*inited = 1;
	}
}