#pragma once

#include <tchar.h>
#include <windows.h>

#include "sml_interface_common.h"


struct MySomeParams
{
	LONGLONG x1;
	LONGLONG x2;
	LONGLONG x3;
};

void func_cleanup(struct MySomeParams * params);
void func_destroy_MyParams(struct MySomeParams * params);

void SML_OBJ_FUNC_1(MySomeParams, Test)();