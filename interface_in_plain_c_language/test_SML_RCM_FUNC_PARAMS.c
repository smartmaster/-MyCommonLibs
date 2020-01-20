#include "stdafx.h"
#include "test_SML_RCM_FUNC_PARAMS.h"
#include "sml_resource_manager.h"


static void func_cleanup_i(LONGLONG x1, LONGLONG x2, LONGLONG x3)
{
	WCHAR * p1 = (WCHAR*)(x1);
	WCHAR * p2 = (WCHAR*)(x2);
	WCHAR * p3 = (WCHAR*)(x3);

	wprintf_s(L"[%s] [%s] [%s]" L"\r\n", p1, p2, p3);
}




void func_cleanup(struct MySomeParams * params)
{
	func_cleanup_i(params->x1, params->x2, params->x3);
}

void func_destroy_MyParams(struct MySomeParams * params)
{
	free((WCHAR*)(params->x1));
	free((WCHAR*)(params->x2));
	free((WCHAR*)(params->x3));
}

void SML_OBJ_FUNC_1(MySomeParams, Test)()
{
	SML_RESOURCE_INIT;

	struct MySomeParams * params = SML_ALLOC_PARAMS(sizeof(struct MySomeParams));
	params->x1 = (LONGLONG)_wcsdup(L"hello");
	params->x2 = (LONGLONG)_wcsdup(L"clang");
	params->x3 = (LONGLONG)_wcsdup(L"world");

	SML_RESOURCE_ADD_PARAMS(func_cleanup, params, func_destroy_MyParams);

	SML_RESOURCE_CLEANUP;
}