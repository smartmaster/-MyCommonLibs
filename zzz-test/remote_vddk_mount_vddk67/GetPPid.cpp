#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <TlHelp32.h>


DWORD WINAPI GetParentPID(PROCESSENTRY32& procentry)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	// Get the first process' information.
	memset(&procentry, 0, sizeof(PROCESSENTRY32));
	procentry.dwSize = sizeof(PROCESSENTRY32);

	DWORD ppid = 0;
	DWORD  crtpid = GetCurrentProcessId();
	BOOL bContinue = Process32First(hSnapShot, &procentry);

	if (bContinue)
	{
		do
		{
			if (crtpid == procentry.th32ProcessID)
			{
				ppid = procentry.th32ParentProcessID;
				break;
			}

			memset(&procentry, 0, sizeof(PROCESSENTRY32));
			procentry.dwSize = sizeof(PROCESSENTRY32);

		} while (Process32Next(hSnapShot, &procentry));

	}
	
	CloseHandle(hSnapShot); hSnapShot = INVALID_HANDLE_VALUE;

	return ppid;
}