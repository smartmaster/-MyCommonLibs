#pragma once

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>

#include <map>
#include <deque>
using namespace std;

#include "DTraceLib.h"

//struct MY_TYPE
//{
//	HRESULT CreateMember();
//	HRESULT DestroyMember();
//};

template<typename MY_TYPE>
class CMTPool
{
private:
	HANDLE m_EventError; /***Incomming***/
	HANDLE m_EventAbort; /***Incomming***/
	INT m_ElementCount; /***Incomming***/

	HANDLE m_SemaphorePool; /***Internal to cleanup***/ //how many unallocated elements left in pool
	CComAutoCriticalSection m_CriricalSection; /***Internal***/
	map<MY_TYPE *, BOOL> m_ElementPool; /***Internal to cleanup***/
	//
private:
	MY_TYPE * AllocateNoLock()
	{
		MY_TYPE * p = NULL;
		for (map<MY_TYPE*, BOOL>::iterator iter = m_ElementPool.begin();
			iter != m_ElementPool.end();
			++ iter)
		{
			if (FALSE == iter->second)
			{
				p = const_cast<MY_TYPE *>(iter->first);
				iter->second = TRUE;
				break;
			}
		}
		return p;
	}
	//
public:
	CMTPool(HANDLE EventError = NULL, HANDLE EventAbort = NULL, INT ElementCount = 0) :
		m_EventError(EventError),
		m_EventAbort(EventAbort),
		m_ElementCount(ElementCount),
		m_SemaphorePool(NULL)
	{
	}

	VOID Init(HANDLE EventError, HANDLE EventAbort, INT ElementCount)
	{
		m_EventError = EventError;
		m_EventAbort = EventAbort;
		m_ElementCount = ElementCount;
	}

	//
	HRESULT CreateMember()
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		m_SemaphorePool = CreateSemaphore(
			NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
			m_ElementCount,//_In_      LONG lInitialCount,
			m_ElementCount,//_In_      LONG lMaximumCount,
			NULL//_In_opt_  LPCTSTR lpName
			);
		if (NULL == m_SemaphorePool)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr, LastError);
			DAPIERR(0, TEXT("CreateSemaphore"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		for (INT ii = 0; ii < m_ElementCount; ++ ii)
		{
			MY_TYPE * pElement = new MY_TYPE;
			hr = pElement->CreateMember();
			if (FAILED(hr))
			{
				DSETLASTSTATUS(hr, 0);
				DAPIERR(0, TEXT("ElementType::CreateMember"), hr, TEXT(""));
				delete pElement; pElement = NULL;
				LEAVE_BLOCK(0);
			}

			m_ElementPool.insert(make_pair(pElement, FALSE));
		}
		if (FAILED(hr))
		{
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	HRESULT DestroyMember()
	{
		HRESULT hr = S_OK;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		if (m_SemaphorePool)
		{
			CloseHandle(m_SemaphorePool); m_SemaphorePool = NULL;
		}

		//////////////////////////////////////////////////////////////////////////
		for (map<MY_TYPE*, BOOL>::iterator iter = m_ElementPool.begin();
			iter != m_ElementPool.end();
			++ iter)
		{
			MY_TYPE * p = const_cast<MY_TYPE *>(iter->first);
			p->DestroyMember();
			delete p; p = NULL;
		}
		m_ElementPool.clear();

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	HRESULT MTAllocate(MY_TYPE ** ppElement)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		HANDLE hWait[] = {m_EventError, m_EventAbort, m_SemaphorePool}; //do not change the order in array
		CONST DWORD dwMilliseconds = 1000 * 60 * 60; //one hour
		DWORD dwWait = WaitForMultipleObjects(
			_countof(hWait),//_In_  DWORD nCount,
			hWait,//_In_  const HANDLE *lpHandles,
			FALSE,//_In_  BOOL bWaitAll,
			dwMilliseconds//_In_  DWORD dwMilliseconds
			);
		if (WAIT_OBJECT_0 == dwWait)
		{
			hr = E_FAIL;
			DAPIERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("Error has occured"));
			LEAVE_BLOCK(0);
		}
		else if (WAIT_OBJECT_0 + 1 == dwWait)
		{
			hr = HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED);
			DSETLASTSTATUS(hr, ERROR_OPERATION_ABORTED);
			DAPIERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("Error operation aborted"));
			LEAVE_BLOCK(0);
		}
		else if (WAIT_OBJECT_0 + 2 == dwWait) //allocate
		{
			//////////////////////////////////////////////////////////////////////////
			CComCritSecLock<CComAutoCriticalSection> AutoLock(m_CriricalSection, FALSE);
			hr = AutoLock.Lock();
			if (FAILED(hr))
			{
				DSETLASTSTATUS(hr, hr);
				DAPIERR(0, TEXT("Lock"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			*ppElement = AllocateNoLock();
			if (NULL == *ppElement)
			{
				hr = E_FAIL;
				DSETLASTSTATUS(hr, -1);
				DAPIERR(0, TEXT("AllocateNoLock"), hr, TEXT("Unexpectd error: all the elements have been used up"));
				LEAVE_BLOCK(0);
			}
		}
		else if (WAIT_TIMEOUT == dwWait)
		{
			hr = E_FAIL;
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("wait timeout"));
			LEAVE_BLOCK(0);
		}
		else
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr, LastError);
			DAPIERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("wait failed"));
			LEAVE_BLOCK(0);
		}

		END_BLOCK(0);

		return hr;
	}


	HRESULT MTFree(MY_TYPE * pElement)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		CComCritSecLock<CComAutoCriticalSection> AutoLock(m_CriricalSection, FALSE);
		hr = AutoLock.Lock();
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr, hr);
			DAPIERR(0, TEXT("Lock"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		map<MY_TYPE*, BOOL>::iterator iter = m_ElementPool.find(pElement);
		if (m_ElementPool.end() == iter)
		{
			hr = E_FAIL;
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("MTFree"), hr, TEXT("Unexpected error: free a non-exiting element"));
			LEAVE_BLOCK(0);
		}
		else if (FALSE == iter->second)
		{
			hr = E_FAIL;
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("MTFree"), hr, TEXT("Unexpected error: free a non-allocated element"));
			LEAVE_BLOCK(0);
		}
		else
		{
			iter->second = FALSE;
			AutoLock.Unlock();
		}

		//////////////////////////////////////////////////////////////////////////
		BOOL bRet = ReleaseSemaphore(m_SemaphorePool, 1, NULL);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr, LastError);
			DAPIERR(0, TEXT("ReleaseSemaphore"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

};

template<typename MY_TYPE>
class CMTQueue
{
private:
	HANDLE m_EventError; /***Incomming***/
	HANDLE m_EventAbort; /***Incomming***/
	HANDLE m_EventProviderFinish; /***Incomming***/
	INT m_ElementCount; /***Incomming***/

	HANDLE m_SemaphoreQueue; /***Internal to cleanup***/ //how many elements left in queue
	CComAutoCriticalSection m_CriricalSection; /***Internal***/
	deque<MY_TYPE *> m_ElementQueue; /***Internal to cleanup***/

public:

	CMTQueue(HANDLE EventError = NULL, HANDLE EventAbort = NULL, HANDLE EventProviderFinish = NULL, INT ElementCount = 0) :
		m_EventError(EventError),
		m_EventAbort(EventAbort),
		m_EventProviderFinish(EventProviderFinish),
		m_ElementCount(ElementCount),
		m_SemaphoreQueue(NULL)
	{
	}

	VOID Init(HANDLE EventError, HANDLE EventAbort, HANDLE EventProviderFinish, INT ElementCount)
	{
		m_EventError = EventError;
		m_EventAbort = EventAbort;
		m_EventProviderFinish = EventProviderFinish;
		m_ElementCount = ElementCount;
	}

	HRESULT CreateMember()
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		m_SemaphoreQueue = CreateSemaphore(
			NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
			0,//_In_      LONG lInitialCount,
			m_ElementCount,//_In_      LONG lMaximumCount,
			NULL//_In_opt_  LPCTSTR lpName
			);
		if (NULL == m_SemaphoreQueue)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr, LastError);
			DAPIERR(0, TEXT("CreateSemaphore"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	HRESULT DestroyMember()
	{
		HRESULT hr = S_OK;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		if (m_SemaphoreQueue)
		{
			CloseHandle(m_SemaphoreQueue); m_SemaphoreQueue = NULL;
		}

		//////////////////////////////////////////////////////////////////////////
		m_ElementQueue.clear();

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	virtual ~CMTQueue()
	{
		DestroyMember();
	}

	HRESULT MTPushBack(MY_TYPE * p)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		CComCritSecLock<CComAutoCriticalSection> AutoLock(m_CriricalSection, FALSE);
		hr = AutoLock.Lock();
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr, hr);
			DAPIERR(0, TEXT("Lock"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		m_ElementQueue.push_back(p);
		AutoLock.Unlock();

		//////////////////////////////////////////////////////////////////////////
		BOOL bRet = ReleaseSemaphore(m_SemaphoreQueue, 1, NULL);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr, LastError);
			DAPIERR(0, TEXT("ReleaseSemaphore"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	HRESULT MTPopFront(MY_TYPE ** ppElement)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		HANDLE hWait[] = {m_EventError, m_EventAbort, m_SemaphoreQueue, m_EventProviderFinish}; //do not change the order in array, they are considerably arranged 
		CONST DWORD dwMilliseconds = 1000 * 60 * 60; //one hour
		DWORD dwWait = WaitForMultipleObjects(
			_countof(hWait),//_In_  DWORD nCount,
			hWait,//_In_  const HANDLE *lpHandles,
			FALSE,//_In_  BOOL bWaitAll,
			dwMilliseconds//_In_  DWORD dwMilliseconds
			);
		if (WAIT_OBJECT_0 == dwWait)
		{
			hr = E_FAIL;
			DAPIERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("Error has occured"));
			LEAVE_BLOCK(0);
		}
		else if (WAIT_OBJECT_0 + 1 == dwWait)
		{
			hr = HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED);
			DSETLASTSTATUS(hr, ERROR_OPERATION_ABORTED);
			DAPIERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("Error operation aborted"));
			LEAVE_BLOCK(0);
		}
		else if (WAIT_OBJECT_0 + 2 == dwWait) //pop front
		{
			//////////////////////////////////////////////////////////////////////////
			CComCritSecLock<CComAutoCriticalSection> AutoLock(m_CriricalSection, FALSE);
			hr = AutoLock.Lock();
			if (FAILED(hr))
			{
				DSETLASTSTATUS(hr, hr);
				DAPIERR(0, TEXT("Lock"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			if (m_ElementQueue.empty())
			{
				hr = E_FAIL;
				DSETLASTSTATUS(hr, -1);
				DAPIERR(0, TEXT("pop_front"), hr, TEXT("Unexpectd error: empty queue"));
				LEAVE_BLOCK(0);
			}
			else
			{
				*ppElement = m_ElementQueue.front();
				m_ElementQueue.pop_front();
			}
			
		}
		else if (WAIT_OBJECT_0 + 3 == dwWait)
		{
			hr = S_OK;
			DINFO(0, TEXT("Provider operation finished"));
			LEAVE_BLOCK(0);
		}
		else if (WAIT_TIMEOUT == dwWait)
		{
			hr = E_FAIL;
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("wait timeout"));
			LEAVE_BLOCK(0);
		}
		else
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr, LastError);
			DAPIERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("wait failed"));
			LEAVE_BLOCK(0);
		}

		END_BLOCK(0);

		return hr;
	}



};