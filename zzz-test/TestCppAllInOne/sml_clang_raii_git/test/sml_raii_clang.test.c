

#include "../src/sml_raii_clang.h"

#include "log.h"


void sml_raii_clang_test()
{
	SML_RAII_BLOCK_START(sml_raii_clang_test);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) p1 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(p1, 128, TEXT("1111111111111111111"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), p1);

	SML_RAII_START(sml_raii_clang_test, p1);
	if (p1)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), p1);
		free(p1);
		p1 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, p1);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) p2 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(p2, 128, TEXT("222222222222222222222222222"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), p2);

	SML_RAII_START(sml_raii_clang_test, p2);
	if (p2)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), p2);
		free(p2);
		p2 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, p2);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) p3 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(p3, 128, TEXT("33333333333333333333333"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), p3);

	SML_RAII_START(sml_raii_clang_test, p3);
	if (p3)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), p3);
		free(p3);
		p3 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, p3);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) p4 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(p4, 128, TEXT("44444444444444444444"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), p4);

	SML_RAII_START(sml_raii_clang_test, p4);
	if (p4)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), p4);
		free(p4);
		p4 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, p4);


	SML_RAII_VOLATILE(WCHAR*) p5 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(p5, 128, TEXT("555555555555555"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), p5);

	SML_RAII_START(sml_raii_clang_test, p5);
	if (p5)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), p5);
		free(p5);
		p5 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, p5);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) res1000 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(res1000, 128, TEXT("res1000"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), res1000);

	SML_RAII_START(sml_raii_clang_test, res1000);
	if (res1000)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), res1000);
		free(res1000);
		res1000 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, res1000);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) res2000 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(res2000, 128, TEXT("res2000"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), res2000);

	SML_RAII_START(sml_raii_clang_test, res2000);
	if (res2000)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), res2000);
		free(res2000);
		res2000 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, res2000);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) res3000 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(res3000, 128, TEXT("res3000"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), res3000);

	SML_RAII_START(sml_raii_clang_test, res3000);
	if (res3000)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), res3000);
		free(res3000);
		res3000 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, res3000);

	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) res4000 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(res4000, 128, TEXT("res4000"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), res4000);

	SML_RAII_START(sml_raii_clang_test, res4000);
	if (res4000)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), res4000);
		free(res4000);
		res4000 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, res4000);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) res5000 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(res5000, 128, TEXT("res5000"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), res5000);

	SML_RAII_START(sml_raii_clang_test, res5000);
	if (res5000)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), res5000);
		free(res5000);
		res5000 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, res5000);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) res6000 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(res6000, 128, TEXT("res6000"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), res6000);

	SML_RAII_START(sml_raii_clang_test, res6000);
	if (res6000)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), res6000);
		free(res6000);
		res6000 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, res6000);

	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) res7000 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(res7000, 128, TEXT("res7000"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), res7000);

	SML_RAII_START(sml_raii_clang_test, res7000);
	if (res7000)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), res7000);
		free(res7000);
		res7000 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, res7000);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) res8000 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(res8000, 128, TEXT("res8000"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), res8000);

	SML_RAII_START(sml_raii_clang_test, res8000);
	if (res8000)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), res8000);
		free(res8000);
		res8000 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, res8000);


	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) res9000 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(res9000, 128, TEXT("res9000"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), res9000);

	SML_RAII_START(sml_raii_clang_test, res9000);
	if (res9000)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), res9000);
		free(res9000);
		res9000 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, res9000);



	//////////////////////////////////////////////////////////////////////////
	SML_RAII_VOLATILE(WCHAR*) resa000 = calloc(128, sizeof(WCHAR));
	_tcscpy_s(resa000, 128, TEXT("resa000"));
	SML_LOG_LINE(TEXT("Allocated resource [%s]"), resa000);

	SML_RAII_START(sml_raii_clang_test, resa000);
	if (resa000)
	{
		SML_LOG_LINE(TEXT("To free resource [%s]"), resa000);
		free(resa000);
		resa000 = NULL;
	}
	SML_RAII_END(sml_raii_clang_test, resa000);


	SML_RAII_BLOCK_END(sml_raii_clang_test);
	SML_RAII_LABEL(sml_raii_clang_test, p1);
	SML_RAII_LABEL(sml_raii_clang_test, p2);
	SML_RAII_LABEL(sml_raii_clang_test, p3);
	SML_RAII_LABEL(sml_raii_clang_test, p4);
	SML_RAII_LABEL(sml_raii_clang_test, p5);
	SML_RAII_LABEL(sml_raii_clang_test, res1000);
	SML_RAII_LABEL(sml_raii_clang_test, res2000);
	SML_RAII_LABEL(sml_raii_clang_test, res3000);
	SML_RAII_LABEL(sml_raii_clang_test, res4000);
	SML_RAII_LABEL(sml_raii_clang_test, res5000);
	SML_RAII_LABEL(sml_raii_clang_test, res6000);
	SML_RAII_LABEL(sml_raii_clang_test, res7000);
	SML_RAII_LABEL(sml_raii_clang_test, res8000);
	SML_RAII_LABEL(sml_raii_clang_test, res9000);
	SML_RAII_LABEL(sml_raii_clang_test, resa000);
}


void sml_raii_clang_test1()
{

}

void sml_raii_clang_test2()
{

}
