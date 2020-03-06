#include "stdafx.h"

#include "TaskMonitorClass.h"

HRESULT CTaskMonitor::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_events[0] = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		FALSE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_events[0])
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

	m_events[1] = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_events[1])
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

	return hr;
}

HRESULT CTaskMonitor::DestroyMember()
{
	for (LONG ii = 0; ii < _countof(m_events); ++ii)
	{
		if (m_events[ii])
		{
			CloseHandle(m_events[ii]);
			m_events[ii] = NULL;
		}
	}


	if (m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	return S_OK;
}

HRESULT CTaskMonitor::Start()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	DWORD ThreadId = 0;
	ATLASSERT(NULL == m_hThread);
	m_hThread = CreateThread(
		NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
		0,//_In_       SIZE_T dwStackSize,
		(LPTHREAD_START_ROUTINE)(ThreadProc),//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
		(LPVOID)(this),//_In_opt_   LPVOID lpParameter,
		0,//_In_       DWORD dwCreationFlags,
		&ThreadId//_Out_opt_  LPDWORD lpThreadId
		);
	if (NULL == m_hThread)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateThread"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////

	return hr;
}

HRESULT CTaskMonitor::Stop()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = SetEvent(m_events[1]);
	if (!bRet)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("SetEvent"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	if (m_hThread)
	{
		ULONG __3min = 1000 * 60 * 3;
		DWORD dwWait = WaitForSingleObject(m_hThread, __3min);
		if (WAIT_OBJECT_0 != dwWait)
		{
			hr = HRESULT_FROM_WIN32(dwWait);
			D_API_ERR(0, TEXT("WaitForSingleObject"), hr, TEXT(""));
		}

		CloseHandle(m_hThread);
		m_hThread = NULL;
	}


	return hr;
}

HRESULT CTaskMonitor::SetTaskDone()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = SetEvent(m_events[0]);
	if (!bRet)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("SetEvent"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	return hr;
}

DWORD WINAPI CTaskMonitor::ThreadProc(_In_ LPVOID lpParameter)
{
	CTaskMonitor * pObj = (CTaskMonitor *)(lpParameter);
	return pObj->ThreadMethod();
}

HRESULT CTaskMonitor::ThreadMethod()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	DWORD dwWait = 0;
	for (;;)
	{
		dwWait = WaitForMultipleObjects(
			_countof(m_events),//_In_  DWORD nCount,
			m_events,//_In_  const HANDLE *lpHandles,
			FALSE,//_In_  BOOL bWaitAll,
			m_TimeOut//_In_  DWORD dwMilliseconds
			);
		if (WAIT_TIMEOUT == dwWait)
		{
			hr = HRESULT_FROM_WIN32(dwWait);
			D_API_ERR(0, TEXT("WAIT_TIMEOUT"), hr, TEXT("Hang decteted, end process"));
			ExitProcess(hr);
			break; //never goes here
		}
		else if (WAIT_FAILED == dwWait)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT(""));
			break;
		}
		else if (WAIT_OBJECT_0 == dwWait)
		{
			continue;
		}
		else if (WAIT_OBJECT_0 + 1 == dwWait)
		{
			D_INFO(0, TEXT("monitor end"));
			break;
		}
		else
		{
			hr = HRESULT_FROM_WIN32(dwWait);
			D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT(""));
			break;
		}
	}

	return hr;
}

CTaskMonitor::CTaskMonitor(ULONG TimeOut) :
m_TimeOut(TimeOut),
m_refCount(0),
m_hThread(NULL)
{
	ZeroMemory(m_events, sizeof(m_events));
}

HRESULT CTaskMonitor::Restart()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_hThread)
	{
		hr = Stop();
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Stop"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	for (LONG ii = 0; ii < _countof(m_events); ++ii)
	{
		BOOL bRet = ResetEvent(
			m_events[ii]//_In_  HANDLE hEvent
			);

		if (!bRet)
		{
			//LastError = WSAGetLastError();
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("ResetEvent"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = Start();
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Start"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCTaskMonitor(ULONG TimeOut, ITaskMonitor ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		new CTaskMonitor(TimeOut),//TY_CLASS * NEW_COBJECT, 
		ppObj,//TY_INTERFACE ** ppObj, 
		bCreateMember//BOOL bCreateMember
		);
}