#pragma once

#include <vector>
using std::vector;

namespace NSFileLock
{
	class CFileLock
	{
	private:
		CString m_strFileName;
		HANDLE m_hFile;

	private:
		VOID DestroyMember();

	public:
		CFileLock(LPCTSTR szFileName);
		~CFileLock();

		HRESULT LockExclusive();
		HRESULT LockShared();
		
		HRESULT LockExclusiveEx(ULONG ulSleepInterval, ULONG ulRetryCount);
		HRESULT LockSharedEx(ULONG ulSleepInterval, ULONG ulRetryCount);
		
		HRESULT Unlock();
	};


	class CFileLockMultiple
	{
	private:
		vector<CFileLock*> m_Locks;
		vector<CString> m_FileNames;

	private:
		VOID DestroyMember();

	public:
		CFileLockMultiple();
		~CFileLockMultiple();

		VOID AddLockFile(LPCTSTR szFileName);

		HRESULT LockExclusive();
		HRESULT LockShared();

		HRESULT LockExclusiveEx(ULONG ulSleepInterval, ULONG ulRetryCount);
		HRESULT LockSharedEx(ULONG ulSleepInterval, ULONG ulRetryCount);

		HRESULT Unlock();
	};
}