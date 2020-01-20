

#include "sml_raii_clang.h"
#include "log.h"


void sml_raii_clang_test_nomacro()
{
	static SmlCVector volatile _0_sml_raii_vec_blocks;
	static SmlCVector volatile _0_sml_raii_vec_jmp;
	static long volatile _0_sml_raii_initing = 0;
	static long volatile _0_sml_raii_call_getaddr_size = 0;
	static long volatile _0_sml_raii_call_jmptag_size = 0;
	static long volatile _0_sml_raii_const_false = 0;
	static long volatile _0_sml_raii_reserved1 = 0xbbbb'bbbb;
	static long volatile _0_sml_raii_reserved2 = 0xcccc'cccc;
	static long volatile _0_sml_raii_prepared = 0;
	long volatile _0_sml_raii_initing_local = (_InterlockedCompareExchange(&_0_sml_raii_initing, 1, 0) == 0);
_0_sml_raii_block_start_label:
	if (_0_sml_raii_initing_local)
	{
		SmlCVector_Ctor(&_0_sml_raii_vec_blocks);
		SmlCVector_Ctor(&_0_sml_raii_vec_jmp);
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile p1 = calloc(128, sizeof(WCHAR));
	wcscpy_s(p1, 128, L"1111111111111111111");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", p1);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_1000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (p1)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", p1);
			free(p1);
			p1 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_1000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile p2 = calloc(128, sizeof(WCHAR));
	wcscpy_s(p2, 128, L"222222222222222222222222222");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", p2);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_2000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (p2)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", p2);
			free(p2);
			p2 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_2000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile p3 = calloc(128, sizeof(WCHAR));
	wcscpy_s(p3, 128, L"33333333333333333333333");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", p3);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_3000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (p3)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", p3);
			free(p3);
			p3 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_3000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile p4 = calloc(128, sizeof(WCHAR));
	wcscpy_s(p4, 128, L"44444444444444444444");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", p4);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_4000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (p4)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", p4);
			free(p4);
			p4 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_4000;
	}
	;
	WCHAR* volatile p5 = calloc(128, sizeof(WCHAR));
	wcscpy_s(p5, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", p5);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_5000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (p5)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", p5);
			free(p5);
			p5 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_5000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res1000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res1000, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res1000);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_6000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (res1000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", res1000);
			free(res1000);
			res1000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_6000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res2000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res2000, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res2000);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_7000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (res2000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", res2000);
			free(res2000);
			res2000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_7000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res3000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res3000, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res3000);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_8000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (res3000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", res3000);
			free(res3000);
			res3000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_8000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res4000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res4000, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res4000);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_9000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (res4000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", res4000);
			free(res4000);
			res4000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_9000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res5000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res5000, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res5000);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_A000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (res5000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", res5000);
			free(res5000);
			res5000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_A000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res6000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res6000, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res6000);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_B000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (res6000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", res6000);
			free(res6000);
			res6000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_B000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res7000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res7000, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res7000);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_C000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (res7000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", res7000);
			free(res7000);
			res7000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_C000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res8000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res8000, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res8000);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_D000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (res8000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", res8000);
			free(res8000);
			res8000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_D000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res9000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res9000, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res9000);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_E000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (res9000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", res9000);
			free(res9000);
			res9000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_E000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile resA000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(resA000, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", resA000);
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
_0_sml_raii_lable_F000:
	if (_0_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (resA000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", resA000);
			free(resA000);
			resA000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_0_sml_raii_vec_blocks, _0_sml_raii_initing_local, _0_sml_raii_reserved1, _0_sml_raii_reserved2);
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_F000;
	}
	;
	if (_0_sml_raii_initing_local)
	{
		Sml_PrepareCleanup(&_0_sml_raii_vec_blocks, &_0_sml_raii_vec_jmp);
	}
	if (0 == _0_sml_raii_initing_local)
	{
		while (0 == _0_sml_raii_prepared)
		{
			Sleep(0);
		}
	}
	{
		int _0_sml_raii_vec_jmp_size = SmlCVector_Size(&_0_sml_raii_vec_jmp);
		BYTE* _0_sml_raii_first = _0_sml_raii_vec_jmp_size == 0 ? ((void*)0) : (BYTE*)SmlCVector_Get(&_0_sml_raii_vec_jmp, 0);
		if (_0_sml_raii_first)
		{
			_0_sml_raii_first += sizeof(Sml_JmpTagPatern);
		}
		BYTE* _0_sml_raii_last = _0_sml_raii_vec_jmp_size == 0 ? ((void*)0) : (BYTE*)SmlCVector_Get(&_0_sml_raii_vec_jmp, _0_sml_raii_vec_jmp_size - 1);
		Sml_AsmRunCleanup(_0_sml_raii_first, _0_sml_raii_last, &_0_sml_raii_prepared);
	}
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_block_start_label;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_1000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_2000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_3000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_4000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_5000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_6000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_7000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_8000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_9000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_A000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_B000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_C000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_D000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_E000;
	}
	;
	if (_0_sml_raii_const_false)
	{
		goto _0_sml_raii_lable_F000;
	}
	;
}
void sml_raii_clang_test1_nomacro()
{
}
void sml_raii_clang_test2_nomacro()
{
}