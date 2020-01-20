#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string>
using namespace std;

namespace TestCpp
{
	class CTimerWait
	{
	public:
		static void OneByOne(const WCHAR *pwESXServerName)
		{

			const DWORD dwTimeout = 30;
			wstring wtrMutexName = L"Global\\UDP_CreateSnapshotSequentially_183CB510-A7F1-41BC-9284-24A9EE2B4CA8";
			if (pwESXServerName != NULL && pwESXServerName[0] != 0)
				wtrMutexName.append(pwESXServerName);

			LARGE_INTEGER liDueTime;

			LONGLONG nano100 = -10'000'000LL;

			HANDLE hTimerEvent = CreateWaitableTimerW(NULL, FALSE, wtrMutexName.c_str());
			if (hTimerEvent != NULL)
			{
				if (GetLastError() == ERROR_SUCCESS)
				{
					//D2DDEBUGLOG(0, 0, TEXT(__FUNCTION__) TEXT("This is 1st backup job, setting waitable timer..."));
					liDueTime.QuadPart = 0;
					if (!SetWaitableTimer(hTimerEvent, &liDueTime, 0, NULL, NULL, 0))
					{
						//D2DDEBUGLOG(0, 0, TEXT(__FUNCTION__) TEXT("Faield to set waitable timer!"));
						return;
					}
				}
			}
			else
			{
				//D2DDEBUGLOG(0, 0, TEXT(__FUNCTION__) TEXT("Faield to create waitable timer!"));
				return;
			}

			if (WaitForSingleObject(hTimerEvent, INFINITE) == WAIT_OBJECT_0)
			{
				liDueTime.QuadPart = nano100 * dwTimeout;
				SetWaitableTimer(hTimerEvent, &liDueTime, 0, NULL, NULL, FALSE);
			}
		}
	};
}