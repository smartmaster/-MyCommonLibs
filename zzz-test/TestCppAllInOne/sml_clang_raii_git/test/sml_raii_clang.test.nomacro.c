
#include "../src/sml_raii_clang.h"
#include "log.h"

void sml_raii_clang_test_nomacro()
{
	static SmlCVector volatile _sml_raii_clang_test_sml_raii_vec_blocks;
	static SmlCVector volatile _sml_raii_clang_test_sml_raii_vec_jmp;
	static long volatile _sml_raii_clang_test_sml_raii_const_false = 0;
	static long volatile _sml_raii_clang_test_sml_raii_initing = 0;
	static long volatile _sml_raii_clang_test_sml_raii_ready = 0;
	const long _sml_raii_clang_test_sml_raii_reserved1 = 0xbbbb'bbbb;
	const long _sml_raii_clang_test_sml_raii_reserved2 = 0xcccc'cccc;
	long volatile _sml_raii_clang_test_sml_raii_initing_local = (_InterlockedCompareExchange(&_sml_raii_clang_test_sml_raii_initing, 1, 0) == 0);
_sml_raii_clang_test_sml_raii_block_start_label:
	if (_sml_raii_clang_test_sml_raii_initing_local)
	{
		SmlCVector_Ctor(&_sml_raii_clang_test_sml_raii_vec_blocks);
		SmlCVector_Ctor(&_sml_raii_clang_test_sml_raii_vec_jmp);
	};
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile p1 = calloc(128, sizeof(WCHAR));
	wcscpy_s(p1, 128, L"1111111111111111111");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", p1);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_p1:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_p1;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile p2 = calloc(128, sizeof(WCHAR));
	wcscpy_s(p2, 128, L"222222222222222222222222222");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", p2);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_p2:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_p2;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile p3 = calloc(128, sizeof(WCHAR));
	wcscpy_s(p3, 128, L"33333333333333333333333");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", p3);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_p3:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_p3;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile p4 = calloc(128, sizeof(WCHAR));
	wcscpy_s(p4, 128, L"44444444444444444444");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", p4);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_p4:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_p4;
	}
	;
	WCHAR* volatile p5 = calloc(128, sizeof(WCHAR));
	wcscpy_s(p5, 128, L"555555555555555");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", p5);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_p5:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_p5;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res1000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res1000, 128, L"res1000");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res1000);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_res1000:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res1000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res2000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res2000, 128, L"res2000");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res2000);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_res2000:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res2000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res3000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res3000, 128, L"res3000");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res3000);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_res3000:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res3000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res4000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res4000, 128, L"res4000");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res4000);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_res4000:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res4000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res5000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res5000, 128, L"res5000");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res5000);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_res5000:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res5000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res6000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res6000, 128, L"res6000");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res6000);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_res6000:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res6000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res7000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res7000, 128, L"res7000");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res7000);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_res7000:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res7000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res8000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res8000, 128, L"res8000");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res8000);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_res8000:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res8000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile res9000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(res9000, 128, L"res9000");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", res9000);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_res9000:
	if (_sml_raii_clang_test_sml_raii_const_false)
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
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res9000;
	}
	;
	//////////////////////////////////////////////////////////////////////////
	WCHAR* volatile resa000 = calloc(128, sizeof(WCHAR));
	wcscpy_s(resa000, 128, L"resa000");
	fwprintf_s((__acrt_iob_func(1)), L"Allocated resource [%s]" L"\r\n", resa000);
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
_sml_raii_clang_test_sml_raii_lable_resa000:
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		Sml_JmpTag('ssss', 'mmmm', 'llll', 'bbbb');
		;
		if (resa000)
		{
			fwprintf_s((__acrt_iob_func(1)), L"To free resource [%s]" L"\r\n", resa000);
			free(resa000);
			resa000 = ((void*)0);
		}
		Sml_JmpTag('SSSS', 'MMMM', 'LLLL', 'EEEE');
	}
	Sml_AsmGetRetAddr(&_sml_raii_clang_test_sml_raii_vec_blocks, _sml_raii_clang_test_sml_raii_initing_local, _sml_raii_clang_test_sml_raii_reserved1, _sml_raii_clang_test_sml_raii_reserved2);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_resa000;
	}
	;
	if (0 == _sml_raii_clang_test_sml_raii_initing_local)
	{
		while (0 == _sml_raii_clang_test_sml_raii_ready)
		{
			Sleep(0);
		}
	}
	if (_sml_raii_clang_test_sml_raii_initing_local)
	{
		Sml_FindJmps(&_sml_raii_clang_test_sml_raii_vec_blocks, &_sml_raii_clang_test_sml_raii_vec_jmp);
	}
	Sml_AsmLinkAndRunCleanups(_sml_raii_clang_test_sml_raii_initing_local, &_sml_raii_clang_test_sml_raii_ready, &_sml_raii_clang_test_sml_raii_vec_jmp);
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		printf("Sml_JmpTag result is %d" "\r\n", Sml_JmpTag('RRRR', 'aaaa', 'IIII', 'iiii'));
		goto _sml_raii_clang_test_sml_raii_block_start_label;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_p1;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_p2;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_p3;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_p4;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_p5;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res1000;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res2000;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res3000;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res4000;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res5000;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res6000;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res7000;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res8000;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_res9000;
	}
	;
	if (_sml_raii_clang_test_sml_raii_const_false)
	{
		goto _sml_raii_clang_test_sml_raii_lable_resa000;
	}
	;
}
void sml_raii_clang_test1_nomacro()
{
}
void sml_raii_clang_test2_nomacro()
{
}