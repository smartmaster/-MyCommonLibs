#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"

#include "SemaphoreLockWin32.h"


CSemaphoreLockWin32::CSemaphoreLockWin32(LPCTSTR ObjectName, LONG lInitialCount, LONG lMaximumCount/*, DWORD DesiredAccess*/) :
m_ObjectName(ObjectName),
m_InitialCount(lInitialCount),
m_MaximumCount(lMaximumCount),
//m_DesiredAccess(DesiredAccess),
m_RefCount(0),
m_Semaphore(NULL)
{

}

HRESULT CSemaphoreLockWin32::CreateMember()
{
	DWORD LastError = 0;
	HRESULT hr = S_OK;

	m_Semaphore = CreateSemaphore(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpMutexAttributes,
		m_InitialCount,
		m_MaximumCount,
		(m_ObjectName.GetLength() ? m_ObjectName.GetString() : NULL)//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_Semaphore)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateSemaphore"), hr, TEXT("Name=%s"), m_ObjectName.GetString());
	}

	return hr;
}

HRESULT CSemaphoreLockWin32::DestroyMember()
{
	if (m_Semaphore)
	{
		CloseHandle(m_Semaphore);
		m_Semaphore = NULL;
	}

	return S_OK;
}

HRESULT CSemaphoreLockWin32::Lock(ULONG Timeout)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	DWORD WaitRet = WaitForSingleObject(
		m_Semaphore,//_In_  HANDLE hHandle,
		Timeout//_In_  DWORD dwMilliseconds
		);
	if (WAIT_OBJECT_0 == WaitRet)
	{
		hr = S_OK;
	}
	else if (WAIT_TIMEOUT == WaitRet)
	{
		hr = HRESULT_FROM_WIN32(WAIT_TIMEOUT);
		D_SET_LAST_STATUS(hr, WaitRet);
		D_API_ERR(0, TEXT("WaitForSingleObject"), hr, TEXT("Name=%s"), m_ObjectName.GetString());
	}
	else
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("WaitForSingleObject"), hr, TEXT("Name=%s"), m_ObjectName.GetString());
	}

	return hr;
}

HRESULT CSemaphoreLockWin32::Unlock()
{
	HRESULT hr = S_OK;
	DWORD	LastError = 0;

	LONG PreviousCount = 0;
	BOOL bRet = ReleaseSemaphore(
		m_Semaphore,//_In_       HANDLE hSemaphore,
		1,//_In_       LONG lReleaseCount,
		&PreviousCount//_Out_opt_  LPLONG lpPreviousCount
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("ReleaseSemaphore"), hr, TEXT("Name=%s"), m_ObjectName.GetString());
	}

	return hr;
}



HRESULT CreateInstanceCSemaphoreLockWin32(LPCTSTR ObjectName, LONG lInitialCount, LONG lMaximumCount/*, DWORD DesiredAccess*/, IProcLock ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(/*CMutexLockWin32,*/ (new CSemaphoreLockWin32(ObjectName, lInitialCount, lMaximumCount/*, DWORD DesiredAccess*/)), ppObj, bCreateMember);
}
