#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"

#include "MutexLockWin32.h"


CMutexLockWin32::CMutexLockWin32( LPCTSTR ObjectName/*, DWORD DesiredAccess*/ ) :
	m_ObjectName(ObjectName),
	//m_DesiredAccess(DesiredAccess),
	m_RefCount(0),
	m_Mutex(NULL)
{

}

LPCTSTR CMutexLockWin32::WhoAmI()
{
	static CONST TCHAR WHO_AM_I[] = TEXT("CMutexLockWin32:IProcLock");
	return WHO_AM_I;
}

HRESULT CMutexLockWin32::CreateMember()
{
	DWORD LastError = 0;
	HRESULT hr = S_OK;

	m_Mutex = CreateMutex(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpMutexAttributes,
		FALSE,//_In_      BOOL bInitialOwner,
		(m_ObjectName.GetLength()? m_ObjectName.GetString() : NULL)//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_Mutex)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateMutex"), hr, TEXT("Name=%s"), m_ObjectName.GetString());
	}

	return hr;
}

HRESULT CMutexLockWin32::DestroyMember()
{
	if (m_Mutex)
	{
		CloseHandle(m_Mutex);
		m_Mutex = NULL;
	}

	return S_OK;
}

HRESULT CMutexLockWin32::Lock( ULONG Timeout )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	DWORD WaitRet = WaitForSingleObject(
		m_Mutex,//_In_  HANDLE hHandle,
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

HRESULT CMutexLockWin32::Unlock()
{
	HRESULT hr = S_OK;
	DWORD	LastError = 0;

	BOOL bRet = ReleaseMutex(m_Mutex);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
	}

	return hr;
}



HRESULT CreateInstanceMutexLockWin32( LPCTSTR ObjectName, /*DWORD DesiredAccess, */IProcLock ** ppObj, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CMutexLockWin32,*/ (new CMutexLockWin32(ObjectName/*, DesiredAccess*/)), ppObj, bCreateMember);
	//HRESULT hr = S_OK;

	//CMutexLockWin32 * pMutexLockWin32 = new CMutexLockWin32(ObjectName, DesiredAccess);
	//if (NULL == pMutexLockWin32)
	//{
	//	return E_OUTOFMEMORY;
	//}

	//pMutexLockWin32->AddRef();

	//if (bCreateMember)
	//{
	//	hr = pMutexLockWin32->CreateMember();
	//	if (FAILED(hr))
	//	{
	//		pMutexLockWin32->Release();
	//		return hr;
	//	}
	//}

	//*ppObj = pMutexLockWin32;
	//return S_OK;
}
