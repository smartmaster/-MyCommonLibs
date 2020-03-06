#include "stdafx.h"
#include "FileLockLib.h"


namespace NSFileLock
{
	static CONST TCHAR SZ_DEFAULT_LOCK_FILE_NAME[] = TEXT("{6AC24833-B9F5-4f0d-93C2-3E6DBB7E79B2}.lock");

	VOID CFileLock::DestroyMember()
	{
		if (INVALID_HANDLE_VALUE != m_hFile)
		{
			CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
		}
	}

	CFileLock::CFileLock( LPCTSTR szFileName ) :
	m_hFile(INVALID_HANDLE_VALUE)
	{
		if (szFileName && szFileName[0])
		{
			m_strFileName = szFileName;
		}
		else
		{
			m_strFileName = SZ_DEFAULT_LOCK_FILE_NAME;
		}
	}

	CFileLock::~CFileLock()
	{
		DestroyMember();
	}

	HRESULT CFileLock::LockExclusive()
	{
		HRESULT hr = S_OK;
		DWORD dwLastError = 0;

		ATLASSERT(INVALID_HANDLE_VALUE == m_hFile);

		m_hFile = CreateFile(
			m_strFileName,//__in      LPCTSTR lpFileName,
			GENERIC_WRITE,//__in      DWORD dwDesiredAccess,
			FILE_SHARE_DELETE,//__in      DWORD dwShareMode,
			NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_ALWAYS,//__in      DWORD dwCreationDisposition,
			FILE_ATTRIBUTE_HIDDEN|FILE_FLAG_DELETE_ON_CLOSE,//__in      DWORD dwFlagsAndAttributes,
			NULL//__in_opt  HANDLE hTemplateFile
			);
		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((999, DRNPOS, TEXT("CreateFile"), hr, TEXT("[FileName=%s]"), m_strFileName.GetString()));
		}
		return hr;
	}

	HRESULT CFileLock::LockShared()
	{
		HRESULT hr = S_OK;
		DWORD dwLastError = 0;

		ATLASSERT(INVALID_HANDLE_VALUE == m_hFile);

		m_hFile = CreateFile(
			m_strFileName,//__in      LPCTSTR lpFileName,
			GENERIC_READ,//__in      DWORD dwDesiredAccess,
			FILE_SHARE_READ|FILE_SHARE_DELETE,//__in      DWORD dwShareMode,
			NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_ALWAYS,//__in      DWORD dwCreationDisposition,
			FILE_ATTRIBUTE_HIDDEN|FILE_FLAG_DELETE_ON_CLOSE,//__in      DWORD dwFlagsAndAttributes,
			NULL//__in_opt  HANDLE hTemplateFile
			);
		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((999, DRNPOS, TEXT("CreateFile"), hr, TEXT("[FileName=%s]"), m_strFileName.GetString()));
		}
		return hr;
	}

	HRESULT CFileLock::Unlock()
	{
		DestroyMember();
		return S_OK;
	}

	HRESULT CFileLock::LockExclusiveEx( ULONG ulSleepInterval, ULONG ulRetryCount )
	{
		HRESULT hr = E_FAIL;

		if (0 == ulSleepInterval || (ULONG)(-1) == ulSleepInterval)
		{
			ulSleepInterval = 3*1000;
		}
		if (0 == ulRetryCount || (ULONG)(-1) == ulRetryCount)
		{
			ulRetryCount = 5;
		}

		for (ULONG ii=0; ii<ulRetryCount; ++ii)
		{
			Unlock();
			hr = LockExclusive();
			if (SUCCEEDED(hr))
			{
				break;
			}
			else
			{
				Sleep(ulSleepInterval);
			}
		}
		return hr;
	}

	HRESULT CFileLock::LockSharedEx( ULONG ulSleepInterval, ULONG ulRetryCount )
	{
		HRESULT hr = E_FAIL;

		if (0 == ulSleepInterval || (ULONG)(-1) == ulSleepInterval)
		{
			ulSleepInterval = 3*1000;
		}
		if (0 == ulRetryCount || (ULONG)(-1) == ulRetryCount)
		{
			ulRetryCount = 5;
		}

		for (ULONG ii=0; ii<ulRetryCount; ++ii)
		{
			Unlock();
			hr = LockShared();
			if (SUCCEEDED(hr))
			{
				break;
			}
			else
			{
				Sleep(ulSleepInterval);
			}
		}
		return hr;
	}

	VOID CFileLockMultiple::DestroyMember()
	{
		for (size_t ii=0; ii<m_Locks.size(); ++ii)
		{
			if (m_Locks[ii])
			{
				m_Locks[ii]->Unlock();
				delete m_Locks[ii];
				m_Locks[ii] = NULL;
			}
		}
		m_Locks.clear();
	}

	CFileLockMultiple::CFileLockMultiple()
	{

	}

	CFileLockMultiple::~CFileLockMultiple()
	{
		DestroyMember();
	}

	VOID CFileLockMultiple::AddLockFile( LPCTSTR szFileName )
	{
		m_FileNames.push_back(CString(szFileName));
	}

	HRESULT CFileLockMultiple::LockExclusive()
	{
		HRESULT hr = S_OK;

		ATLASSERT(0 == m_Locks.size());

		for (size_t ii=0; ii<m_FileNames.size(); ++ii)
		{
			CAutoPtr<CFileLock> apFileLock(new CFileLock(m_FileNames[ii]));
			hr = apFileLock->LockExclusive();
			if (SUCCEEDED(hr))
			{
				m_Locks.push_back(apFileLock.Detach());
			}
			else
			{
				break;
			}
		}

		return hr;
	}

	HRESULT CFileLockMultiple::LockShared()
	{
		HRESULT hr = 0;

		ATLASSERT(0 == m_Locks.size());

		for (size_t ii=0; ii<m_FileNames.size(); ++ii)
		{
			CAutoPtr<CFileLock> apFileLock(new CFileLock(m_FileNames[ii]));
			hr = apFileLock->LockShared();
			if (SUCCEEDED(hr))
			{
				m_Locks.push_back(apFileLock.Detach());
			}
			else
			{
				break;
			}
		}

		return hr;
	}

	HRESULT CFileLockMultiple::Unlock()
	{
		DestroyMember();
		return S_OK;
	}

	HRESULT CFileLockMultiple::LockExclusiveEx( ULONG ulSleepInterval, ULONG ulRetryCount )
	{
		HRESULT hr = E_FAIL;

		if (0 == ulSleepInterval || (ULONG)(-1) == ulSleepInterval)
		{
			ulSleepInterval = 3*1000;
		}
		if (0 == ulRetryCount || (ULONG)(-1) == ulRetryCount)
		{
			ulRetryCount = 5;
		}

		for (ULONG ii=0; ii<ulRetryCount; ++ii)
		{
			Unlock();
			hr = LockExclusive();
			if (SUCCEEDED(hr))
			{
				break;
			}
			else
			{
				Sleep(ulSleepInterval);
			}
		}
		return hr;
	}

	HRESULT CFileLockMultiple::LockSharedEx( ULONG ulSleepInterval, ULONG ulRetryCount )
	{
		HRESULT hr = E_FAIL;

		if (0 == ulSleepInterval || (ULONG)(-1) == ulSleepInterval)
		{
			ulSleepInterval = 3*1000;
		}
		if (0 == ulRetryCount || (ULONG)(-1) == ulRetryCount)
		{
			ulRetryCount = 5;
		}

		for (ULONG ii=0; ii<ulRetryCount; ++ii)
		{
			Unlock();
			hr = LockShared();
			if (SUCCEEDED(hr))
			{
				break;
			}
			else
			{
				Sleep(ulSleepInterval);
			}
		}
		return hr;
	}

} //end NSFileLock