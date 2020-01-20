#pragma once
#include <stdarg.h>
#include <iostream>

#include "log.h"
#include "StringOf.h"

namespace SmartLib
{
	struct TestArgs
	{
		template<typename T>
		static void PrintTypeArgs(int count, ...)
		{
			va_list list;
			va_start(list, count);
			const char * ptemp = SML_EXPANDED_MACRO_STRING(va_start(list, count));
			SML_LOG_LINE(TEXT("va_start - list=0x%p"), list);
			for (int ii = 0; ii < count; ++ii)
			{
				T param = va_arg(list, T);
				ptemp = SML_EXPANDED_MACRO_STRING(va_start(list, T));
				std::cout << "param=" << param << ", ";  SML_LOG_LINE(TEXT("list=0x%p"), list);
			}
			va_end(list);
			ptemp = SML_EXPANDED_MACRO_STRING(va_end(list));
			SML_LOG_LINE(TEXT("va_end - list=0x%p"), list);
		}


		static void Case0()
		{
			ULONGLONG val = (ULONGLONG)70368744177664;
			ULONGLONG val2 = (1ULL << 46);
			PrintTypeArgs<int>(3, 100, 200, 300);
			PrintTypeArgs<LONGLONG>(4, -100LL, -200LL, -300LL, -400LL);
		}
	};
}