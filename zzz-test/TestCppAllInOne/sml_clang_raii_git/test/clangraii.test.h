#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "../src/asm_func.h"
#include "../src/raii_func.h"
#include "log.h"

namespace SmartLib
{
	class CLangRaiiTest
	{
	public:
		static void Case2_loop()
		{
			for (int ii = 0; ii < 2; ++ ii)
			{
				Case1();
			}
		}
		static void Case1()
		{
			static SmlCVector sml_raii_vec_blocks;
			static SmlCVector sml_raii_vec_jmp;
			static volatile long sml_raii_initing = 0;
			static volatile long sml_raii_call_getaddr_size = 0;
			static volatile long sml_raii_call_jmptag_size = 0;
			static volatile long sml_raii_const_false = 0;
			static volatile long sml_raii_reserved1 = 0xbbbb'bbbb;
			static volatile long sml_raii_reserved2 = 0xcccc'cccc;
			static volatile long sml_raii_prepared = 0;
			sml_raii_block_start_label:
			volatile long sml_raii_initing_local = (InterlockedCompareExchange(&sml_raii_initing, 1, 0) == 0); //only one thread can modify the code
			if (sml_raii_initing_local)
			{
				SmlCVector_Ctor(&sml_raii_vec_blocks);
				SmlCVector_Ctor(&sml_raii_vec_jmp);
			}

			//////////////////////////////////////////////////////////////////////////
			HANDLE file1 = CreateFile(NULL, 0, 0, NULL, 0, 0, NULL);
			
			TCHAR* mem1 = new TCHAR[128];
			_tcscpy_s(mem1, 128, TEXT("1111111111"));

			Sml_AsmGetRetAddr(&sml_raii_vec_blocks, sml_raii_initing_local, sml_raii_reserved1, sml_raii_reserved2);
			sml_raii_lable_1:
			if (sml_raii_const_false)
			{
				Sml_JmpTag(SML_JMPTAGPATERN_BEGIN);
				SML_LOG_LINE(TEXT("cleanup resource 1"));

				SML_LOG_LINE(TEXT("cleanup memory %s"), mem1);
				delete mem1;
				mem1 = nullptr;

				if (file1 != INVALID_HANDLE_VALUE)
				{
					CloseHandle(file1);
				}
				Sml_JmpTag(SML_JMPTAGPATERN_END);
			}
			Sml_AsmGetRetAddr(&sml_raii_vec_blocks, sml_raii_initing_local, sml_raii_reserved1, sml_raii_reserved2);
			if (sml_raii_const_false)
			{
				goto sml_raii_lable_1;
			}
			

			//////////////////////////////////////////////////////////////////////////
			HANDLE file2 = CreateFile(NULL, 0, 0, NULL, 0, 0, NULL);

			TCHAR* mem2 = new TCHAR[128];
			_tcscpy_s(mem2, 128, TEXT("2222222222222"));

			Sml_AsmGetRetAddr(&sml_raii_vec_blocks, sml_raii_initing_local, sml_raii_reserved1, sml_raii_reserved2);
		sml_raii_lable_2:
			if (sml_raii_const_false)
			{
				
				Sml_JmpTag(SML_JMPTAGPATERN_BEGIN);
				SML_LOG_LINE(TEXT("cleanup resource 2"));


				SML_LOG_LINE(TEXT("cleanup memory %s"), mem2);
				delete mem2;
				mem2 = nullptr;

				if (file2 != INVALID_HANDLE_VALUE)
				{
					CloseHandle(file2);
				}
				Sml_JmpTag(SML_JMPTAGPATERN_END);
			}
			Sml_AsmGetRetAddr(&sml_raii_vec_blocks, sml_raii_initing_local, sml_raii_reserved1, sml_raii_reserved2);
			if (sml_raii_const_false)
			{
				goto sml_raii_lable_2;
			}

			//////////////////////////////////////////////////////////////////////////
			HANDLE file3 = CreateFile(NULL, 0, 0, NULL, 0, 0, NULL);

			TCHAR* mem3 = new TCHAR[128];
			_tcscpy_s(mem3, 128, TEXT("33333333333333333"));


			Sml_AsmGetRetAddr(&sml_raii_vec_blocks, sml_raii_initing_local, sml_raii_reserved1, sml_raii_reserved2);
			sml_raii_lable_3:
			if (sml_raii_const_false)
			{
				Sml_JmpTag(SML_JMPTAGPATERN_BEGIN);
				SML_LOG_LINE(TEXT("cleanup resource 3"));

				SML_LOG_LINE(TEXT("cleanup memory %s"), mem3);
				delete mem3;
				mem3 = nullptr;

				if (file3 != INVALID_HANDLE_VALUE)
				{
					CloseHandle(file3);
				}
				Sml_JmpTag(SML_JMPTAGPATERN_END);
			}
			Sml_AsmGetRetAddr(&sml_raii_vec_blocks, sml_raii_initing_local, sml_raii_reserved1, sml_raii_reserved2);
			if (sml_raii_const_false)
			{
				goto sml_raii_lable_3;
			}

			if (sml_raii_initing_local)
			{
				Sml_FindJmps(&sml_raii_vec_blocks, &sml_raii_vec_jmp);
			}
			if (!sml_raii_initing_local)
			{
				while (sml_raii_prepared == 0)
				{
					Sleep(0);
				}
			}
			{
				int sml_raii_vec_jmp_size = SmlCVector_Size(&sml_raii_vec_jmp);
				BYTE* sml_raii_first = sml_raii_vec_jmp_size == 0? NULL : (BYTE*)SmlCVector_Get(&sml_raii_vec_jmp, 0);
				if (sml_raii_first)
				{
					sml_raii_first += sizeof(Sml_JmpTagPatern);
				}
				BYTE* sml_raii_last = sml_raii_vec_jmp_size == 0 ? NULL : (BYTE*)SmlCVector_Get(&sml_raii_vec_jmp, sml_raii_vec_jmp_size - 1);
				Sml_AsmLinkAndRunCleanups(sml_raii_initing_local, &sml_raii_prepared, &sml_raii_vec_jmp);
			}
			if (sml_raii_const_false)
			{
				goto sml_raii_block_start_label;
			}
			if (sml_raii_const_false)
			{
				goto sml_raii_lable_1;
			}
			if (sml_raii_const_false)
			{
				goto sml_raii_lable_2;
			}
			if (sml_raii_const_false)
			{
				goto sml_raii_lable_3;
			}
			

			SML_LOG_LINE(TEXT("return herer after cleanup"));

			return;
		}
		static void Case0()
		{
			SmlCVector vec;
			SmlCVector_Ctor(&vec);
			Sml_AsmGetRetAddr(&vec, 1, 0xbbbb'bbbb, 0xcccc'cccc);
			Sml_AsmGetRetAddr(&vec, 1, 0xbbbb'bbbb, 0xcccc'cccc);
			Sml_AsmGetRetAddr(&vec, 1, 0xbbbb'bbbb, 0xcccc'cccc);
			Sml_AsmGetRetAddr(&vec, 1, 0xbbbb'bbbb, 0xcccc'cccc);
			Sml_AsmGetRetAddr(&vec, 1, 0xbbbb'bbbb, 0xcccc'cccc);
			Sml_AsmGetRetAddr(&vec, 0, 0xbbbb'bbbb, 0xcccc'cccc);
			Sml_AsmGetRetAddr(&vec, 0, 0xbbbb'bbbb, 0xcccc'cccc);
			Sml_AsmGetRetAddr(&vec, 0, 0xbbbb'bbbb, 0xcccc'cccc);
			Sml_AsmGetRetAddr(&vec, 0, 0xbbbb'bbbb, 0xcccc'cccc);

			SML_DATA_PTR p0 = SmlCVector_Get(&vec, 0);
			SML_DATA_PTR p1 = SmlCVector_Get(&vec, 1);
			int funcsize = (ULONG_PTR)p1 - (ULONG_PTR)p0;
			SmlCVector_Dtor(&vec);
		}
	};
}