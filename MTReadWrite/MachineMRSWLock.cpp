#include "stdafx.h"
#include "MachineMRSWLock.h"

HRESULT MachineMRSWLock::CloseMember()
{
	m_result = E_FAIL;

	m_hevReadersCleared.Close();
	m_csReaderCount.Close();
	m_csWrite.Close();
	return S_OK;
}

MachineMRSWLock::MachineMRSWLock(LPCTSTR szBaseName) :
m_csWrite(nullptr),
m_csReaderCount(nullptr),
m_cReaders(nullptr, 0),
m_hevReadersCleared(nullptr),
m_result(E_FAIL)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;


	BEGIN_BLOCK(0);
	CString BaseNameWithSuffix = TEXT("Global\\1DEB10A4F8044BE6874FEA1DB0151357_");

	//////////////////////////////////////////////////////////////////////////
	m_csWrite.Attach(::CreateSemaphore(
		nullptr,//_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
		1,//_In_     LONG                  lInitialCount,
		1,//_In_     LONG                  lMaximumCount,
		(BaseNameWithSuffix + TEXT("csWrite_") + szBaseName)//_In_opt_ LPCTSTR               lpName
		));
	if (nullptr == m_csWrite)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateSemaphore"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_csReaderCount.Attach(::CreateSemaphore(
		nullptr,//_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
		1,//_In_     LONG                  lInitialCount,
		1,//_In_     LONG                  lMaximumCount,
		(BaseNameWithSuffix + TEXT("csReaderCount_") + szBaseName)//_In_opt_ LPCTSTR               lpName
		));
	if (nullptr == m_csReaderCount)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateSemaphore"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = m_cReaders.Create(BaseNameWithSuffix + TEXT("cReaders_") + szBaseName, 0);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Create"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	m_hevReadersCleared.Attach(::CreateEvent(
		nullptr,//_In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_     BOOL                  bManualReset,
		TRUE,//_In_     BOOL                  bInitialState,
		(BaseNameWithSuffix + TEXT("hevReadersCleared_") + szBaseName)//_In_opt_ LPCTSTR               lpName
		));
	if (nullptr == m_hevReadersCleared)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	if (SUCCEEDED(hr))
	{
		m_result = S_OK;
	}
	else
	{
		m_result = hr;
		CloseMember();
	}

	return;
}

MachineMRSWLock::~MachineMRSWLock()
{
	//WaitForSingleObject(m_hevReadersCleared, INFINITE);
	CloseMember();
}

HRESULT MachineMRSWLock::Result()
{
	return m_result;
}

void MachineMRSWLock::EnterReader(void)
{
	WaitForSingleObject(m_csWrite, INFINITE);
	WaitForSingleObject(m_csReaderCount, INFINITE);
	LONG & cReaders = m_cReaders;
	if (++cReaders == 1)
		ResetEvent(m_hevReadersCleared);
	ReleaseSemaphore(m_csReaderCount, 1, nullptr);
	ReleaseSemaphore(m_csWrite, 1, nullptr);
}

void MachineMRSWLock::LeaveReader(void)
{
	WaitForSingleObject(m_csReaderCount, INFINITE);
	LONG & cReaders = m_cReaders;
	if (--cReaders == 0)
		SetEvent(m_hevReadersCleared);
	ReleaseSemaphore(m_csReaderCount, 1, nullptr);
}

void MachineMRSWLock::EnterWriter(void)
{
	WaitForSingleObject(m_csWrite, INFINITE);
	WaitForSingleObject(m_hevReadersCleared, INFINITE);
}

void MachineMRSWLock::LeaveWriter(void)
{
	ReleaseSemaphore(m_csWrite, 1, nullptr);
}

HRESULT CRWLockProcessWin32::CreateMember()
{
	HRESULT hr = S_OK;
	m_pImpl.Attach(new MachineMRSWLock(m_name));
	if (nullptr == m_pImpl.m_p)
	{
		//D_API_ERR(0, TEXT("new"), hr, TEXT("..."));
		hr = E_OUTOFMEMORY;
	}

	if (FAILED(m_pImpl->Result()))
	{
		hr = m_pImpl->Result();
	}

	return hr;
}

HRESULT CRWLockProcessWin32::DestroyMember()
{
	m_pImpl.Free();
	return S_OK;
}

HRESULT CRWLockProcessWin32::Lock()
{
	m_pImpl->EnterWriter();
	return S_OK;
}

HRESULT CRWLockProcessWin32::Unlock()
{
	m_pImpl->LeaveWriter();
	return S_OK;
}

HRESULT CRWLockProcessWin32::SharedLock()
{
	m_pImpl->EnterReader();
	return S_OK;
}

HRESULT CRWLockProcessWin32::SharedUnlock()
{
	m_pImpl->LeaveReader();
	return S_OK;
}


HRESULT CreateInstacnceCRWLockProcessWin32(IThreadRWLock ** ppIRWLock, LPCTSTR name, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(/*CRWLock,*/ (new CRWLockProcessWin32(name)), ppIRWLock, bCreateMember);
}