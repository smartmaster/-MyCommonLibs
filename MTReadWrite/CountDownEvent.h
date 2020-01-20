#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <time.h>

#include <vector>



namespace SmartLib
{
	class CCountDownEvent
	{
	private:
		LONG _count{ 0 };	/***INTERNAL***/
		HANDLE _event{ nullptr }; /***CLEANUP***/

	public:
		CCountDownEvent(LONG count) :
			_count(count)
		{
			_event = CreateEvent(
				nullptr,//_In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
				FALSE,//_In_     BOOL                  bManualReset,
				FALSE,//_In_     BOOL                  bInitialState,
				nullptr//_In_opt_ LPCTSTR               lpName
			);
		}

		~CCountDownEvent()
		{
			CloseHandle(_event);
			_event = nullptr;
		}
		

		void Signal()
		{
			if (0 == InterlockedDecrement(&_count))
			{
				SetEvent(_event);
			}
		}


		void Wait()
		{
			WaitForSingleObject(_event, INFINITE);
		}

		void Reset(int count)
		{
			_count = count;
			ResetEvent(_event);
		}
	};



	class CCountDownEventTest
	{
	public:

		static DWORD WINAPI ThreadProc(
			_In_ LPVOID lpParameter
		)
		{
			CCountDownEvent * cde = (CCountDownEvent*)lpParameter;
			Sleep(100 + rand() % 500);
			_ftprintf_s(stdout, TEXT("Thread %08d ended") TEXT("\r\n"), GetCurrentThreadId());
			cde->Signal();
			return 0;
		}
		static void Case0()
		{
			srand(time(nullptr));
			const int count = 10;
			CCountDownEvent cde(count);
			std::vector<HANDLE> handles(count);
			for (int ii = 0; ii < count; ++ ii)
			{
				DWORD                ThreadId = -1;
				handles[ii] = CreateThread(
					nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
					0,//_In_      SIZE_T                 dwStackSize,
					ThreadProc,//_In_      LPTHREAD_START_ROUTINE lpStartAddress,
					(LPVOID)&cde,//_In_opt_  LPVOID                 lpParameter,
					0,//_In_      DWORD                  dwCreationFlags,
					&ThreadId//_Out_opt_ LPDWORD                lpThreadId
				); 
			}

			cde.Wait();
			WaitForMultipleObjects(handles.size(), &handles[0], TRUE, INFINITE);
			for (auto & h : handles)
			{
				CloseHandle(h);
				h = nullptr;
			}
		}
	};
}