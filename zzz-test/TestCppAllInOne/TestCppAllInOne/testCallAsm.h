#pragma once

#include <windows.h>
#include "log.h"

extern "C" void TestAsm(LPCVOID p0);

namespace SmartLib
{
	class TestCallAsm
	{
	public:
		static LPCVOID RetSame(volatile LPCVOID p0)
		{
			return p0;
		}
		static void Case0()
		{
			__try
			{
				TestAsm(TestCallAsm::Case0);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				SML_LOG_LINE(TEXT("exception occured and expected"));
			}
			
		}
	};
}