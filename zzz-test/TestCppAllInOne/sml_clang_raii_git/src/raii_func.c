#include <assert.h>

#include "raii_func.h"
#include "SmlCVector.h"


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
#ifdef _WIN64
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
#else
{
	const BYTE PUSH = 0x68;
	obj->movr9d1 = PUSH;
	obj->tag4 = tag4;

	obj->movr8d1 = PUSH;
	obj->tag3 = tag3;

	obj->movedx1 = PUSH;
	obj->tag2 = tag2;

	obj->movecx1 = PUSH;
	obj->tag1 = tag1;

	obj->call = 0xE8;
	obj->relativeoffset = 0;
}
#endif


void Sml_Jmp2Target_Init(Sml_Jmp2Target* obj, BYTE* src, BYTE* target)
{
	obj->jmp1 = 0xE9;
	obj->relativeoffset = (int)(target - src - sizeof(Sml_Jmp2Target));
}

#if defined(_WIN64)
int Sml_JmpTag(int tag1, int tag2, int tag3, int tag4)
#else
int __stdcall Sml_JmpTag(int tag1, int tag2, int tag3, int tag4)
#endif
{
	static volatile int some_data = 0xabcd'5678;
	some_data += tag1;
	some_data += tag2;
	some_data += tag3;
	some_data += tag4;
	return some_data;
}


#ifdef _WIN64
void Sml_AddStackPointer_Init(Sml_AddStackPointer* obj, BYTE offset)
{
	//do nothing for win64
	//obj->addesp1 = 0x83;
	//obj->addesp2 = 0xc4;
	//obj->offset = offset;
}

BYTE* Sml_SetAddStackPointer(BYTE* target, BYTE offset)
{
	//do nothing for win64
	//Sml_AddStackPointer asp;
	//Sml_AddStackPointer_Init(&asp, offset);
	//CopyMemory(target, &asp, Sml_AddStackPointer_Size);
	return target + Sml_AddStackPointer_Size;
}
#else
void Sml_AddStackPointer_Init(Sml_AddStackPointer* obj, BYTE offset)
{
	obj->addesp1 = 0x83;
	obj->addesp2 = 0xc4;
	obj->offset = offset;
}

BYTE* Sml_SetAddStackPointer(BYTE* target, BYTE offset)
{
	Sml_AddStackPointer asp;
	Sml_AddStackPointer_Init(&asp, offset);
	CopyMemory(target, &asp, Sml_AddStackPointer_Size);
	return target + Sml_AddStackPointer_Size;
}
#endif // _WIN64

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


BYTE* Sml_FindPlaceholderPattern(int revserse, BYTE * buffer, int len, BYTE * func, Sml_JmpTagPatern * pattern)
#ifdef _WIN64
{

	//	/*
	//	00007FF69CC2319A BA 6D 6D 6D 6D       mov         edx,6D6D6D6Dh
	//	00007FF69CC2319F B9 73 73 73 73       mov         ecx,73737373h
	//	00007FF69CC231A4 41 B9 62 62 62 62    mov         r9d,62626262h
	//	00007FF69CC231AA 41 B8 6C 6C 6C 6C    mov         r8d,6C6C6C6Ch
	//	00007FF69CC231B0 E8 FB 30 FF FF       call        Sml_JmpTag (07FF69CC162B0h)
	//	*/

	BYTE* arr[4] = { 0 };
	int arrsize[4] = { 0 };
	arr[0] = revserse ? sml_memrmem(buffer, len, &pattern->movr9d1, 6) : sml_memmem(buffer, len, &pattern->movr9d1, 6);
	arr[1] = revserse ? sml_memrmem(buffer, len, &pattern->movr8d1, 6) : sml_memmem(buffer, len, &pattern->movr8d1, 6);
	arr[2] = revserse ? sml_memrmem(buffer, len, &pattern->movedx1, 5) : sml_memmem(buffer, len, &pattern->movedx1, 5);
	arr[3] = revserse ? sml_memrmem(buffer, len, &pattern->movecx1, 5) : sml_memmem(buffer, len, &pattern->movecx1, 5);

	qsort(arr, _countof(arr), sizeof(BYTE*), Sml_compare);

	BOOL ok = (arr[0] >= buffer && arr[3] >= buffer);

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
			BYTE* targetfunc = (BYTE*)(&callfunc->call) + 5 + callfunc->relativeoffset;
			ok = (targetfunc == func);
		}
	}

	return ok ? arr[0] : NULL;
}
#else
{
	//////////////////////////32 bit//////////////////////////////////////////////////////
	/*
0088C6B4 68 62 62 62 62       push        62626262h
0088C6B9 68 6C 6C 6C 6C       push        6C6C6C6Ch
0088C6BE 68 6D 6D 6D 6D       push        6D6D6D6Dh
0088C6C3 68 73 73 73 73       push        73737373h
0088C6C8 E8 63 50 FF FF       call        _Sml_JmpTag@16 (0881730h)
	*/

	BYTE* arr[4] = { 0 };
	int arrsize[4] = { 0 };
	arr[0] = revserse ? sml_memrmem(buffer, len, &pattern->movr9d1, 5) : sml_memmem(buffer, len, &pattern->movr9d1, 5);
	arr[1] = revserse ? sml_memrmem(buffer, len, &pattern->movr8d1, 5) : sml_memmem(buffer, len, &pattern->movr8d1, 5);
	arr[2] = revserse ? sml_memrmem(buffer, len, &pattern->movedx1, 5) : sml_memmem(buffer, len, &pattern->movedx1, 5);
	arr[3] = revserse ? sml_memrmem(buffer, len, &pattern->movecx1, 5) : sml_memmem(buffer, len, &pattern->movecx1, 5);

	qsort(arr, _countof(arr), sizeof(BYTE*), Sml_compare);

	BOOL ok = (arr[0] >= buffer && arr[3] >= buffer);

	if (ok)
	{
		int totalSize = 0;
		for (int ii = 0; ii < _countof(arr); ++ii)
		{
			arrsize[ii] = 1024 * 1024; //set a very large size
			switch (arr[ii][0])
			{
			case 0x68:
			{
				arrsize[ii] = 5;
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
			BYTE* targetfunc = (BYTE*)(&callfunc->call) + 5 + callfunc->relativeoffset;
			ok = (targetfunc == func);
		}
	}

	return ok ? arr[0] : NULL;
}
#endif // _WIN64



void Sml_FindJmps(PSmlCVector vec_blocks, PSmlCVector vec_jmp)
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

		BYTE * jmp1 = Sml_FindPlaceholderPattern(0, buffer1, bufferlen, Sml_JmpTag, &pattern1);
		BYTE * jmp2 = Sml_FindPlaceholderPattern(1, buffer1, bufferlen, Sml_JmpTag, &pattern2);

		assert(jmp1 && jmp2 && jmp1 < jmp2);

		memset(jmp1, 0xcc, sizeof(Sml_JmpTagPatern)); //all set 'int 3'
		memset(jmp2, 0xcc, sizeof(Sml_JmpTagPatern));


		SmlCVector_Push(vec_jmp, jmp1);
		SmlCVector_Push(vec_jmp, jmp2);
	}
}


void Sml_SetJmpFromTo(BYTE * from, BYTE * to)
{
	Sml_Jmp2Target j2t;
	Sml_Jmp2Target_Init(&j2t, from, to);
	memcpy(from, &j2t, sizeof(Sml_Jmp2Target));
}


void Sml_LinkAndRunCleanups(LONG localIniting, LONG volatile* globalInited, SmlCVector * vec_jmp, LPBYTE * pretddr)
{
	int vec_size = SmlCVector_Size(vec_jmp);
	BYTE** addrs = SmlCVector_Ptr(vec_jmp);
	assert(0 == (vec_size & 1) && addrs);

	BOOL initing = ((0 != localIniting) && (0 == *globalInited));
	if (initing)
	{
		for (int ii = 0; ii < vec_size - 3; ii += 2)
		{
			BYTE* src = addrs[ii + 1];
			BYTE* target = addrs[ii + 2];
			Sml_SetJmpFromTo(src, target + sizeof(Sml_JmpTagPatern)); //+ sizeof(Sml_JmpTagPatern) to skip nops
		}

		if (vec_size > 0)
		{
			BYTE* src = addrs[vec_size - 1];
			BYTE* target = *pretddr;
			Sml_SetJmpFromTo(src, target + Sml_AddStackPointer_Size);
		}
	}

	if (vec_size)
	{
		BYTE* target = addrs[0] + sizeof(Sml_JmpTagPatern) - Sml_AddStackPointer_Size;
		if (initing)
		{
			if (Sml_AddStackPointer_Size)
			{
				CopyMemory(target, *pretddr, Sml_AddStackPointer_Size);
				memset(*pretddr, 0xcc, Sml_AddStackPointer_Size); //set 'int 3' in case of any error
			}
		}
		*pretddr = target;
	}

	if (initing)
	{
		*globalInited = 1; //set global inited to true
	}
}


BYTE* Sml_GetRetAddr(SmlCVector * vec_blocks, long initing_local, BYTE * *pretaddr)
{
	BYTE* rc = *pretaddr + Sml_AddStackPointer_Size;
	if (initing_local)
	{
		SmlCVector_Push(vec_blocks, rc);
	}
	return rc;
}

