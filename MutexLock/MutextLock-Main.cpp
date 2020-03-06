// MutextLock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "InterfaceMutexLock.h"

int _tmain_InterfaceMutexLock(int argc, _TCHAR* argv[])
{
	CComPtr<IProcLock> spIMutextLock;
	HRESULT hr = CreateInstanceMutexLockWin32(TEXT("Global\\MUTEX_RVCM_SNAPSHOT_0F16ED27_0004_41CE_BCB0_1502ED6490EA"), /*MUTEX_ALL_ACCESS, */&spIMutextLock, TRUE);

	CComPtr<IProcLock> spIMutextLock2;
	hr = CreateInstanceMutexLockWin32(TEXT("Global\\MUTEX_RVCM_SNAPSHOT_0F16ED27_0004_41CE_BCB0_1502ED6490EA"), /*MUTEX_ALL_ACCESS, */&spIMutextLock2, TRUE);
	//if (NULL != spIMutextLock)
	//{
	//	spIMutextLock2 = spIMutextLock;
	//}
	


	hr = spIMutextLock->Lock(1000 * 60 * 15);
	//hr = spIMutextLock2->Lock(3000);
	for (;;)
	{
		Sleep(1000);
	}

	hr = spIMutextLock->Unlock();

	spIMutextLock2.Release();
	spIMutextLock.Release();
	return 0;
}


int _tmain_InterfaceSemaphore(int argc, _TCHAR* argv[])
{
	CComPtr<IProcLock> spSemLock;
	HRESULT hr = CreateInstanceCSemaphoreLockWin32(TEXT("Global\\MUTEX_RVCM_SNAPSHOT_0F16ED27_0004_41CE_BCB0_1502ED6490EA"), 1, 1/*, DWORD DesiredAccess*/, &spSemLock, TRUE);

	CComPtr<IProcLock> spSemLock2;
	hr = CreateInstanceCSemaphoreLockWin32(TEXT("Global\\MUTEX_RVCM_SNAPSHOT_0F16ED27_0004_41CE_BCB0_1502ED6490EA"), 1, 1/*, DWORD DesiredAccess*/, &spSemLock2, TRUE);
	//if (NULL != spIMutextLock)
	//{
	//	spIMutextLock2 = spIMutextLock;
	//}



	hr = spSemLock->Lock(1000 * 60 * 15);
	//hr = spIMutextLock2->Lock(3000);
	for (;;)
	{
		Sleep(1000);
	}

	hr = spSemLock->Unlock();

	spSemLock2.Release();
	spSemLock.Release();
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	return _tmain_InterfaceSemaphore(argc, argv);
	return _tmain_InterfaceMutexLock(argc, argv);
}