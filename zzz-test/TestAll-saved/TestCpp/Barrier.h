#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>



#include <time.h>
#include <stdlib.h>


#include <memory>
#include <vector>

namespace SmartLib
{
	class CBarrier
	{
	private:
		CONST LONG _totalCount;
		LONG _count;
		CRITICAL_SECTION _lock;
		CONDITION_VARIABLE _cv;

	public:
		CBarrier(LONG totalCount) :
			_totalCount(totalCount),
			_count(totalCount)
		{
			InitializeCriticalSectionAndSpinCount(&_lock, 0x400);
			InitializeConditionVariable(&_cv);
		}

		void StopToWait()
		{
			bool wakeOthers = false;
			EnterCriticalSection(&_lock);
			--_count;

			if (0 == _count)
			{
				_count = _totalCount;
				wakeOthers = true;
			}
			else
			{
				SleepConditionVariableCS(&_cv, &_lock, INFINITE);
			}
			LeaveCriticalSection(&_lock);

			if (wakeOthers)
			{
				WakeAllConditionVariable(&_cv);
			}
		}
	};


	class CBarrierTest
	{
	private:
		std::unique_ptr<CBarrier> _barrier;
		const int _count;

	public:
		CBarrierTest(int count) :
			_count(count)
		{
			srand((unsigned)time(nullptr));
			_barrier = std::make_unique<CBarrier>(_count);
		}


		void Print()
		{
			for (int ii = 0; ii < 17; ++ ii)
			{
				_ftprintf_s(stdout, TEXT("%d "), ii);
				Sleep(100 + rand() % 500);
				_barrier->StopToWait();
			}
		}

		static DWORD WINAPI ThreadProc(
			_In_ LPVOID lpParameter
		)
		{
			CBarrierTest * obj = (CBarrierTest*)lpParameter;
			obj->Print();
			return 0;
		}

		static void Case0()
		{
			const int count = 5;
			std::unique_ptr<CBarrierTest> obj = std::make_unique<CBarrierTest>(count);
			std::vector<HANDLE> handles(5);
			for (int ii = 0; ii < count; ++ii)
			{
				DWORD                ThreadId = 0;
				handles[ii] = CreateThread(
					nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
					0,//_In_      SIZE_T                 dwStackSize,
					(LPTHREAD_START_ROUTINE)(CBarrierTest::ThreadProc),//_In_      LPTHREAD_START_ROUTINE lpStartAddress,
					obj.get(),//_In_opt_  LPVOID                 lpParameter,
					0,//_In_      DWORD                  dwCreationFlags,
					&ThreadId//_Out_opt_ LPDWORD                lpThreadId
				);
			}

			WaitForMultipleObjects(handles.size(), &handles[0], true, INFINITE);
			for (HANDLE & h : handles)
			{
				CloseHandle(h);
				h = nullptr;
			}
		}

	};
}