#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "ScopeExec.h"

namespace TestCpp
{
	class CTestCV
	{
	public:
		
	static	DWORD WINAPI ThreadProc(
			_In_  LPVOID lpParameter
		)
		{
			CONDITION_VARIABLE * cv = (CONDITION_VARIABLE*)lpParameter;
			WakeAllConditionVariable(cv);
			Sleep(10 * 1000);
			WakeAllConditionVariable(cv);
			return 0;
		}

		
		static void Case0()
		{
			CONDITION_VARIABLE cv;
			CRITICAL_SECTION cs;
			InitializeCriticalSectionAndSpinCount(&cs, 0x400);
			InitializeConditionVariable(&cv);
			
			DWORD threadId = -1;
			HANDLE ht = CreateThread(
				nullptr,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
				0,//_In_       SIZE_T dwStackSize,
				(LPTHREAD_START_ROUTINE)(ThreadProc),//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
				(&cv),//_In_opt_   LPVOID lpParameter,
				0,//_In_       DWORD dwCreationFlags,
				&threadId//_Out_opt_  LPDWORD lpThreadId
			);

			SmartLib::ScopeExec CScopeExit_ht(nullptr, 
				[&ht]() 
			{
				CloseHandle(ht);
				ht = nullptr;
			});


			EnterCriticalSection(&cs);
			Sleep(3 * 1000);
			SleepConditionVariableCS(&cv, &cs, INFINITE);
			SleepConditionVariableCS(&cv, &cs, INFINITE);
			LeaveCriticalSection(&cs);


			WaitForSingleObject(ht, INFINITE);
			//CloseHandle(ht);
		}
	};
}