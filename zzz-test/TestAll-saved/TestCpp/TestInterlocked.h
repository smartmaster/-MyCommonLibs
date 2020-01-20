#pragma once

#include <tchar.h>
#include <windows.h>


namespace TestCpp
{
	class TestInterlocked
	{
	public:
		static void Case0()
		{
			LONG target = 100;
			LONG old = target;
			int rv = ::InterlockedIncrement(&target);
			_ftprintf_s(stdout, TEXT("%s - old=%d, target=%d, rv=%d") TEXT("\r\n"), TEXT("InterlockedIncrement"), old, target, rv);


			old = target;
			rv = ::InterlockedDecrement(&target);
			_ftprintf_s(stdout, TEXT("%s - old=%d, target=%d, rv=%d") TEXT("\r\n"), TEXT("InterlockedDecrement"), old, target, rv);


			old = target;
			rv = ::InterlockedCompareExchange(&target, old + 100, old);
			_ftprintf_s(stdout, TEXT("%s - old=%d, target=%d, rv=%d") TEXT("\r\n"), TEXT("InterlockedCompareExchange"), old, target, rv);


			old = target;
			rv = ::InterlockedCompareExchange(&target, old + 100, old + 579846);
			_ftprintf_s(stdout, TEXT("%s - old=%d, target=%d, rv=%d") TEXT("\r\n"), TEXT("InterlockedCompareExchange"), old, target, rv);



			old = target;
			rv = ::InterlockedAdd(&target, 10000);
			_ftprintf_s(stdout, TEXT("%s - old=%d, target=%d, rv=%d") TEXT("\r\n"), TEXT("InterlockedAdd"), old, target, rv);

			old = target;
			rv = ::InterlockedAdd(&target, -10000);
			_ftprintf_s(stdout, TEXT("%s - old=%d, target=%d, rv=%d") TEXT("\r\n"), TEXT("InterlockedAdd"), old, target, rv);


		}

		static void Case1(LONG volatile * target)
		{
			LONG old;
			LONG cur;
			do 
			{
				old = *target;
				cur = ::InterlockedCompareExchange(target, old + 1, old);
			} while (old != cur);
		}
	};
}