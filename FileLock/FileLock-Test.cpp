// FileLock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"

#include "FileLockInterface.h"

int _tmain_test(int argc, _TCHAR* argv[])
{

	D_SETTINGS(9999, FALSE, TEXT(""), TEXT(""));

	{
		//CFileLock fileLock(NULL);
		//INT nStatus1 = fileLock.LockExclusive();
		//INT nStatus2 = fileLock.LockShared();
		//D_INFO(0, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2);
	}

	{
		//00
		CComPtr<IFileLock> fileLock1;
		CComPtr<IFileLock> fileLock2;
		CreateInstancCFileLock(NULL, &fileLock1, TRUE);
		CreateInstancCFileLock(NULL, &fileLock2, TRUE);

		INT nStatus1 = fileLock1->Lock();
		INT nStatus2 = fileLock2->Lock();

		fileLock2->Unlock();
		fileLock1->Unlock();
		D_INFO(0, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2);
	}

	{
		//01
		CComPtr<IFileLock> fileLock1;
		CComPtr<IFileLock> fileLock2;
		CreateInstancCFileLock(NULL, &fileLock1, TRUE);
		CreateInstancCFileLock(NULL, &fileLock2, TRUE);

		INT nStatus1 = fileLock1->Lock();
		INT nStatus2 = fileLock2->SharedLock();

		fileLock2->Unlock();
		fileLock1->Unlock();
		D_INFO(0, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2);
	}

	{
		//11
		CComPtr<IFileLock> fileLock1;
		CComPtr<IFileLock> fileLock2;
		CreateInstancCFileLock(NULL, &fileLock1, TRUE);
		CreateInstancCFileLock(NULL, &fileLock2, TRUE);

		INT nStatus1 = fileLock1->SharedLock();
		INT nStatus2 = fileLock2->SharedLock();

		fileLock2->Unlock();
		fileLock1->Unlock();
		D_INFO(0, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2);
	}

	{
		//10
		CComPtr<IFileLock> fileLock1;
		CComPtr<IFileLock> fileLock2;
		CreateInstancCFileLock(NULL, &fileLock1, TRUE);
		CreateInstancCFileLock(NULL, &fileLock2, TRUE);

		INT nStatus1 = fileLock1->SharedLock();
		INT nStatus2 = fileLock2->Lock();

		fileLock2->Unlock();
		fileLock1->Unlock();
		D_INFO(0, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2);
	}

	{
		//00

		vector<CString> LockingFiles;
		for (INT ii=0; ii<5; ++ii)
		{
			CString strFileLock;
			strFileLock.Format(TEXT("Hello-god-%08u.lock"), ii);
			LockingFiles.push_back(strFileLock);
		}
		CComPtr<IFileLock> fileLock1;
		CComPtr<IFileLock> fileLock2;
		CreateInstanceCFileLockMultiple(LockingFiles, &fileLock1, TRUE);
		CreateInstanceCFileLockMultiple(LockingFiles, &fileLock2, TRUE);

		INT nStatus1 = fileLock1->LockEx(1000, 3);
		INT nStatus2 = fileLock2->LockEx(1000, 3);

		fileLock2->Unlock();
		fileLock1->Unlock();
		D_INFO(0, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2);
	}

	{
		//01
		vector<CString> LockingFiles;
		for (INT ii=0; ii<5; ++ii)
		{
			CString strFileLock;
			strFileLock.Format(TEXT("Hello-god-%08u.lock"), ii);
			LockingFiles.push_back(strFileLock);
		}

		CComPtr<IFileLock> fileLock1;
		CComPtr<IFileLock> fileLock2;
		CreateInstanceCFileLockMultiple(LockingFiles, &fileLock1, TRUE);
		CreateInstanceCFileLockMultiple(LockingFiles, &fileLock2, TRUE);

		INT nStatus1 = fileLock1->LockEx(1000, 3);
		INT nStatus2 = fileLock2->SharedLockEx(1000, 3);

		fileLock2->Unlock();
		fileLock1->Unlock();
		D_INFO(0, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2);
	}

	{
		//11
		vector<CString> LockingFiles;
		for (INT ii=0; ii<5; ++ii)
		{
			CString strFileLock;
			strFileLock.Format(TEXT("Hello-god-%08u.lock"), ii);
			LockingFiles.push_back(strFileLock);
		}

		CComPtr<IFileLock> fileLock1;
		CComPtr<IFileLock> fileLock2;
		CreateInstanceCFileLockMultiple(LockingFiles, &fileLock1, TRUE);
		CreateInstanceCFileLockMultiple(LockingFiles, &fileLock2, TRUE);

		INT nStatus1 = fileLock1->SharedLockEx(1000, 3);
		INT nStatus2 = fileLock2->SharedLockEx(1000, 3);

		fileLock2->Unlock();
		fileLock1->Unlock();
		D_INFO(0, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2);
	}

	{
		//10
		vector<CString> LockingFiles;
		for (INT ii=0; ii<5; ++ii)
		{
			CString strFileLock;
			strFileLock.Format(TEXT("Hello-god-%08u.lock"), ii);
			LockingFiles.push_back(strFileLock);
		}

		CComPtr<IFileLock> fileLock1;
		CComPtr<IFileLock> fileLock2;
		CreateInstanceCFileLockMultiple(LockingFiles, &fileLock1, TRUE);
		CreateInstanceCFileLockMultiple(LockingFiles, &fileLock2, TRUE);

		INT nStatus1 = fileLock1->SharedLockEx(1000, 3);
		INT nStatus2 = fileLock2->LockEx(1000, 3);

		fileLock2->Unlock();
		fileLock1->Unlock();
		D_INFO(0, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2);
	}


	return 0;
}

