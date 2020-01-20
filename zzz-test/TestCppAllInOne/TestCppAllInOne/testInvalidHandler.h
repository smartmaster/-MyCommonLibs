#pragma once

// crt_set_invalid_parameter_handler.c
// compile with: /Zi /MTd
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>  // For _CrtSetReportMode


extern void ExceptionDump_SetExceptionHandler(BOOL bFullDump);

struct MyStruct
{
	static void myInvalidParameterHandler(const wchar_t* expression,
		const wchar_t* function,
		const wchar_t* file,
		unsigned int line,
		uintptr_t pReserved)
	{
		wprintf(L"Invalid parameter detected in function %s."
			L" File: %s Line: %d\n", function, file, line);
		wprintf(L"Expression: %s\n", expression);
		throw(L"Invalid parameters");
		//abort();
	}

	static  int main_case0()
	{
		//ExceptionDump_SetExceptionHandler(FALSE);

		try
		{
			char* formatString;

			_invalid_parameter_handler oldHandler, newHandler;
			newHandler = myInvalidParameterHandler;
			oldHandler = _set_invalid_parameter_handler(newHandler);

			// Disable the message box for assertions.
			_CrtSetReportMode(_CRT_ASSERT, 0);

			// Call printf_s with invalid parameters.

			formatString = NULL;
			printf(formatString);
		}
		catch (...)
		{
			printf("exception");
		}
		return 0;
	}
};