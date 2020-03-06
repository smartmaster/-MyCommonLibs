// FileLock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "FileLockLib.h"
using namespace NSFileLock;

int _tmain_test(int argc, _TCHAR* argv[])
{
	DSETLEVEL(9999);

	{
		//CFileLock fileLock(NULL);
		//INT nStatus1 = fileLock.LockExclusive();
		//INT nStatus2 = fileLock.LockShared();
		//_OLD_DINFO((0, DRNPOS, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2));
	}

	{
		//00
		CFileLock fileLock1(NULL);
		CFileLock fileLock2(NULL);
		INT nStatus1 = fileLock1.LockExclusive();
		INT nStatus2 = fileLock2.LockExclusive();

		fileLock2.Unlock();
		fileLock1.Unlock();
		_OLD_DINFO((0, DRNPOS, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2));
	}

	{
		//01
		CFileLock fileLock1(NULL);
		CFileLock fileLock2(NULL);
		INT nStatus1 = fileLock1.LockExclusive();
		INT nStatus2 = fileLock2.LockShared();

		fileLock2.Unlock();
		fileLock1.Unlock();
		_OLD_DINFO((0, DRNPOS, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2));
	}

	{
		//11
		CFileLock fileLock1(NULL);
		CFileLock fileLock2(NULL);
		INT nStatus1 = fileLock1.LockShared();
		INT nStatus2 = fileLock2.LockShared();

		fileLock2.Unlock();
		fileLock1.Unlock();
		_OLD_DINFO((0, DRNPOS, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2));
	}

	{
		//10
		CFileLock fileLock1(NULL);
		CFileLock fileLock2(NULL);
		INT nStatus1 = fileLock1.LockShared();
		INT nStatus2 = fileLock2.LockExclusive();

		fileLock2.Unlock();
		fileLock1.Unlock();
		_OLD_DINFO((0, DRNPOS, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2));
	}

	{
		//00
		CFileLockMultiple fileLock1;
		CFileLockMultiple fileLock2;

		for (INT ii=0; ii<5; ++ii)
		{
			CString strFileLock;
			strFileLock.Format(TEXT("Hello-god-%08u.lock"), ii);
			fileLock1.AddLockFile(strFileLock);
			fileLock2.AddLockFile(strFileLock);
		}

		INT nStatus1 = fileLock1.LockExclusiveEx(1000, 3);
		INT nStatus2 = fileLock2.LockExclusiveEx(1000, 3);

		fileLock2.Unlock();
		fileLock1.Unlock();
		_OLD_DINFO((0, DRNPOS, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2));
	}

	{
		//01
		CFileLockMultiple fileLock1;
		CFileLockMultiple fileLock2;

		for (INT ii=0; ii<5; ++ii)
		{
			CString strFileLock;
			strFileLock.Format(TEXT("Hello-god-%08u.lock"), ii);
			fileLock1.AddLockFile(strFileLock);
			fileLock2.AddLockFile(strFileLock);
		}

		INT nStatus1 = fileLock1.LockExclusiveEx(1000, 3);
		INT nStatus2 = fileLock2.LockSharedEx(1000, 3);

		fileLock2.Unlock();
		fileLock1.Unlock();
		_OLD_DINFO((0, DRNPOS, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2));
	}

	{
		//11
		CFileLockMultiple fileLock1;
		CFileLockMultiple fileLock2;

		for (INT ii=0; ii<5; ++ii)
		{
			CString strFileLock;
			strFileLock.Format(TEXT("Hello-god-%08u.lock"), ii);
			fileLock1.AddLockFile(strFileLock);
			fileLock2.AddLockFile(strFileLock);
		}

		INT nStatus1 = fileLock1.LockSharedEx(1000, 3);
		INT nStatus2 = fileLock2.LockSharedEx(1000, 3);

		fileLock2.Unlock();
		fileLock1.Unlock();
		_OLD_DINFO((0, DRNPOS, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2));
	}

	{
		//10
		CFileLockMultiple fileLock1;
		CFileLockMultiple fileLock2;

		for (INT ii=0; ii<5; ++ii)
		{
			CString strFileLock;
			strFileLock.Format(TEXT("Hello-god-%08u.lock"), ii);
			fileLock1.AddLockFile(strFileLock);
			fileLock2.AddLockFile(strFileLock);
		}

		INT nStatus1 = fileLock1.LockSharedEx(1000, 3);
		INT nStatus2 = fileLock2.LockExclusiveEx(1000, 3);

		fileLock2.Unlock();
		fileLock1.Unlock();
		_OLD_DINFO((0, DRNPOS, TEXT("[nStatus1=%d, nStatus2=%d]"), nStatus1, nStatus2));
	}


	return 0;
}

