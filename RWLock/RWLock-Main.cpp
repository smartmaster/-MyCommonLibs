// RWLock.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

#include "InterfaceLock.h"

DWORD WINAPI ThreadProc_Lock(
	_In_  LPVOID lpParameter
	)
{
	CComPtr<IThreadRWLock> spIRWLock;
	spIRWLock.Attach((IThreadRWLock*)lpParameter);
	spIRWLock->Lock();
	for (int ii = 0; ii < 5; ++ii)
	{
		Sleep(1000);
	}
	spIRWLock->Unlock();

	return 0;
}

DWORD WINAPI ThreadProc_SharedLock(
	_In_  LPVOID lpParameter
	)
{
	CComPtr<IThreadRWLock> spIRWLock;
	spIRWLock.Attach((IThreadRWLock*)lpParameter);
	spIRWLock->SharedLock();
	for (int ii = 0; ii < 5; ++ii)
	{
		Sleep(1000);
	}
	spIRWLock->SharedUnlock();

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CComPtr<IThreadRWLock> spIRWLock;
	int testCase = 2;

	switch (testCase)
	{
	case 0:
		CreateInstacnceSRWLock(&spIRWLock, TRUE);
		break;
	case  1:
		CreateInstacnceCriticalSectionLock(&spIRWLock, TRUE);
		break;
	//case 2:
	//	CreateInstacnceCRWLockWin32(&spIRWLock, TRUE);
	//	break;
	}


	LPCTSTR pObjectName = spIRWLock->WhoAmI();

	{
		spIRWLock->Lock();

		CComPtr<IThreadRWLock> spThreadLock = spIRWLock;
		HANDLE hThread = CreateThread(
			NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//_In_       SIZE_T dwStackSize,
			ThreadProc_Lock,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
			spThreadLock.Detach(),//_In_opt_   LPVOID lpParameter,
			0,//_In_       DWORD dwCreationFlags,
			NULL//_Out_opt_  LPDWORD lpThreadId
			);

		for (int ii = 0; ii < 5; ++ii)
		{
			Sleep(1000);
		}

		spIRWLock->Unlock();

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread); hThread = NULL;
	}

	{
		spIRWLock->Lock();

		CComPtr<IThreadRWLock> spThreadLock = spIRWLock;
		HANDLE hThread = CreateThread(
			NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//_In_       SIZE_T dwStackSize,
			ThreadProc_SharedLock,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
			spThreadLock.Detach(),//_In_opt_   LPVOID lpParameter,
			0,//_In_       DWORD dwCreationFlags,
			NULL//_Out_opt_  LPDWORD lpThreadId
			);

		for (int ii = 0; ii < 5; ++ii)
		{
			Sleep(1000);
		}

		spIRWLock->Unlock();

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread); hThread = NULL;
	}


	{
		spIRWLock->SharedLock();

		CComPtr<IThreadRWLock> spThreadLock = spIRWLock;
		HANDLE hThread = CreateThread(
			NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//_In_       SIZE_T dwStackSize,
			ThreadProc_Lock,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
			spThreadLock.Detach(),//_In_opt_   LPVOID lpParameter,
			0,//_In_       DWORD dwCreationFlags,
			NULL//_Out_opt_  LPDWORD lpThreadId
			);

		for (int ii = 0; ii < 5; ++ii)
		{
			Sleep(1000);
		}

		spIRWLock->SharedUnlock();

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread); hThread = NULL;

	}

	{
		spIRWLock->SharedLock();

		CComPtr<IThreadRWLock> spThreadLock = spIRWLock;
		HANDLE hThread = CreateThread(
			NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//_In_       SIZE_T dwStackSize,
			ThreadProc_SharedLock,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
			spThreadLock.Detach(),//_In_opt_   LPVOID lpParameter,
			0,//_In_       DWORD dwCreationFlags,
			NULL//_Out_opt_  LPDWORD lpThreadId
			);

		for (int ii = 0; ii < 5; ++ii)
		{
			Sleep(1000);
		}

		spIRWLock->SharedUnlock();

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread); hThread = NULL;

	}

	//spIRWLock->Lock();
	//spIRWLock->Lock();
	//spIRWLock->Unlock();
	//spIRWLock->Unlock();

	//spIRWLock->Lock();
	//spIRWLock->SharedLock();
	//spIRWLock->SharedUnlock();
	//spIRWLock->Unlock();

	//spIRWLock->SharedLock();
	//spIRWLock->Lock();
	//spIRWLock->Unlock();
	//spIRWLock->SharedUnlock();

	spIRWLock->SharedLock();
	spIRWLock->SharedLock();
	spIRWLock->SharedUnlock();
	spIRWLock->SharedUnlock();



	return 0;
}

