#pragma once

#ifdef _WIN32

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>



namespace SmartLib
{

	class WinTest
	{
	public:
		static void CaseInterlocked()
		{
			static volatile long target = 1;
			long orgianl = InterlockedCompareExchange(&target, 0, 1);
			orgianl = InterlockedCompareExchange(&target, 0, 1);
			InterlockedIncrement(&target);
		}
	};
}


#endif // _WIN32
