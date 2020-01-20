#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <immintrin.h>
#include <map>
#include <utility>

#include "log.h"

namespace TestCpp
{
	class SimpleSpinLock
	{
		const LONG Free = 0;
		const LONG Busy = 1;

	private:
		volatile LONG _state{ Free };
	public:
		void Lock()
		{
			while (InterlockedCompareExchange(&_state, Busy, Free) == Busy)
			{
#if 1
				do 
				{
					_mm_pause();

				} while (_state == Busy);
#endif
			}
		}


		void Unlock()
		{
			InterlockedExchange(&_state, Free);
		}
	};


	class SimpleSpinLockTest
	{
	private:
		SimpleSpinLock _lock;
		std::map<int, int> _map;

		DWORD ThreadMethod()
		{
			for (int ii = 0; ii < 10000; ++ ii)
			{
				_lock.Lock();
				if (_map.find(ii) == _map.end())
				{
					_map.insert(std::pair<int, int>(ii, ii));
				}
				//::Sleep(50);
				LOG_LINE(TEXT("Thread %d inserted value %d"), GetCurrentThreadId(), ii);
				_lock.Unlock();
			}
			return 0;
		}

		static DWORD WINAPI ThreadProc(
			_In_ LPVOID lpParameter
		)
		{
			SimpleSpinLockTest * obj = (SimpleSpinLockTest*)(lpParameter);
			return obj->ThreadMethod();
		}


	public:
		static void Case0()
		{
			SimpleSpinLockTest test;
			HANDLE hThread[2] = { 0 };
			for (int ii = 0; ii < _countof(hThread); ++ ii)
			{
				DWORD ThreadId = 0;
				hThread[ii] = ::CreateThread(
					nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
					0,//_In_      SIZE_T                 dwStackSize,
					ThreadProc,//_In_      LPTHREAD_START_ROUTINE lpStartAddress,
					&test,//_In_opt_  LPVOID                 lpParameter,
					0,//_In_      DWORD                  dwCreationFlags,
					&ThreadId//_Out_opt_ LPDWORD                lpThreadId
				);
			}


			WaitForMultipleObjects(_countof(hThread), hThread, TRUE, INFINITE);
		}
	};
}