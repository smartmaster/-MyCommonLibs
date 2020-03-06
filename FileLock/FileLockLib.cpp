#include "stdafx.h"

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "FileLockLib.h"


//////////////////////////////////////////////////////////////////////////
CFileLock::CFileLock( LPCTSTR szFileName ) :
	m_hFile(INVALID_HANDLE_VALUE),
	m_RefCount(0)
{
	if (szFileName && szFileName[0])
	{
		m_strFileName = szFileName;
	}
	else
	{
		static CONST TCHAR DEFAULT_LOCK_FILE_NAME[] = TEXT("{6AC24833-B9F5-4f0d-93C2-3E6DBB7E79B2}.lock");
		m_strFileName = DEFAULT_LOCK_FILE_NAME;
	}
}

HRESULT CFileLock::DestroyMember()
{
	Unlock();
	return S_OK;
}

HRESULT CFileLock::Unlock()
{
	if (INVALID_HANDLE_VALUE != m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
	return S_OK;
}



HRESULT CFileLock::Lock()
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
		D_SET_LAST_STATUS(hr, dwLastError);
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("[FileName=%s]"), m_strFileName.GetString());
	}
	return hr;
}

HRESULT CFileLock::SharedLock()
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
		D_SET_LAST_STATUS(hr, dwLastError);
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("[FileName=%s]"), m_strFileName.GetString());
	}
	return hr;
}



HRESULT CFileLock::LockEx( ULONG ulSleepInterval, ULONG ulRetryCount )
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
		hr = Lock();
		if (SUCCEEDED(hr))
		{
			break;
		}
		else
		{
			if (HRESULT_FROM_WIN32(ERROR_SHARING_VIOLATION) == hr)
			{
				Sleep(ulSleepInterval);
			}
			else
			{
				break;
			}
		}
	}
	return hr;
}

HRESULT CFileLock::SharedLockEx( ULONG ulSleepInterval, ULONG ulRetryCount )
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
		hr = SharedLock();
		if (SUCCEEDED(hr))
		{
			break;
		}
		else
		{
			if (HRESULT_FROM_WIN32(ERROR_SHARING_VIOLATION) == hr)
			{
				Sleep(ulSleepInterval);
			}
			else
			{
				break;
			}
		}
	}
	return hr;
}


//////////////////////////////////////////////////////////////////////////
CFileLockMultiple::CFileLockMultiple(CONST vector<CString> & LockingFiles) :
	m_RefCount(0),
	m_FileNames(LockingFiles)
{
}


HRESULT CFileLockMultiple::DestroyMember()
{
	Unlock();
	return S_OK;
}

HRESULT CFileLockMultiple::Unlock()
{
	for (int ii = m_Locks.size() - 1; ii >= 0; --ii)
	{
		if (m_Locks[ii])
		{
			m_Locks[ii]->Unlock();
			delete m_Locks[ii];
			m_Locks[ii] = NULL;
		}
	}
	m_Locks.clear();
	return S_OK;
}


HRESULT CFileLockMultiple::Lock()
{
	HRESULT hr = S_OK;

	ATLASSERT(0 == m_Locks.size());

	for (size_t ii=0; ii<m_FileNames.size(); ++ii)
	{
		CAutoPtr<CFileLock> apFileLock(new CFileLock(m_FileNames[ii]));
		hr = apFileLock->Lock();
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

HRESULT CFileLockMultiple::SharedLock()
{
	HRESULT hr = 0;

	ATLASSERT(0 == m_Locks.size());

	for (size_t ii=0; ii<m_FileNames.size(); ++ii)
	{
		CAutoPtr<CFileLock> apFileLock(new CFileLock(m_FileNames[ii]));
		hr = apFileLock->SharedLock();
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


HRESULT CFileLockMultiple::LockEx( ULONG ulSleepInterval, ULONG ulRetryCount )
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
		hr = Lock();
		if (SUCCEEDED(hr))
		{
			break;
		}
		else
		{
			if (HRESULT_FROM_WIN32(ERROR_SHARING_VIOLATION) == hr)
			{
				Sleep(ulSleepInterval);
			}
			else
			{
				break;
			}
		}
	}
	return hr;
}

HRESULT CFileLockMultiple::SharedLockEx( ULONG ulSleepInterval, ULONG ulRetryCount )
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
		hr = SharedLock();
		if (SUCCEEDED(hr))
		{
			break;
		}
		else
		{
			if (HRESULT_FROM_WIN32(ERROR_SHARING_VIOLATION) == hr)
			{
				Sleep(ulSleepInterval);
			}
			else
			{
				break;
			}
		}
	}
	return hr;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstancCFileLock( LPCTSTR FileName, IFileLock ** ppObj, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CFileLock,*/ (new CFileLock(FileName)), ppObj, bCreateMember);
	//HRESULT hr = S_OK;
	//BEGIN_BLOCK(0);
	////////////////////////////////////////////////////////////////////////////
	//CFileLock * pCFileLock = new CFileLock(FileName);
	//if (NULL == pCFileLock)
	//{
	//	hr = E_OUTOFMEMORY;
	//	LEAVE_BLOCK(0);
	//}

	////////////////////////////////////////////////////////////////////////////
	//pCFileLock->AddRef();
	//if (bCreateMember)
	//{
	//	hr = pCFileLock->CreateMember();
	//	if (FAILED(hr))
	//	{
	//		pCFileLock->Release();
	//		LEAVE_BLOCK(0);
	//	}
	//}

	////////////////////////////////////////////////////////////////////////////
	//*ppObj  = pCFileLock;
	//END_BLOCK(0);

	//return hr;
}


HRESULT CreateInstanceCFileLockMultiple( CONST vector<CString> & LockingFiles, IFileLock ** ppObj, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CFileLockMultiple,*/ (new CFileLockMultiple(LockingFiles)), ppObj, bCreateMember );
	//HRESULT hr = S_OK;
	//BEGIN_BLOCK(0);
	////////////////////////////////////////////////////////////////////////////
	//CFileLockMultiple * pCFileLockMultiple = new CFileLockMultiple(LockingFiles);
	//if (NULL == pCFileLockMultiple)
	//{
	//	hr = E_OUTOFMEMORY;
	//	LEAVE_BLOCK(0);
	//}

	////////////////////////////////////////////////////////////////////////////
	//pCFileLockMultiple->AddRef();
	//if (bCreateMember)
	//{
	//	hr = pCFileLockMultiple->CreateMember();
	//	if (FAILED(hr))
	//	{
	//		pCFileLockMultiple->Release();
	//		LEAVE_BLOCK(0);
	//	}
	//}

	////////////////////////////////////////////////////////////////////////////
	//*ppObj  = pCFileLockMultiple;
	//END_BLOCK(0);

	//return hr;
}

