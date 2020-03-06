#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"

#include "EventHelper.h"


CSimpleEventHelper::CSimpleEventHelper(BOOL bManualReset)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	m_hEvent = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		bManualReset,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_hEvent)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
	}
}

BOOL CSimpleEventHelper::IsValid() CONST
{
	return (NULL != m_hEvent);
}

CSimpleEventHelper::~CSimpleEventHelper()
{
	if (m_hEvent)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
}

HRESULT CSimpleEventHelper::Set()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	BOOL bSetEvent = SetEvent(
		m_hEvent//_In_  HANDLE hEvent
		);
	if (!bSetEvent)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("SetEvent"), hr, TEXT(""));
	}

	//////////////////////////////////////////////////////////////////////////

	return hr;
}

HRESULT CSimpleEventHelper::Reset()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	BOOL bResetEvent = ResetEvent(
		m_hEvent//_In_  HANDLE hEvent
		);
	if (!bResetEvent)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("ResetEvent"), hr, TEXT(""));
	}

	//////////////////////////////////////////////////////////////////////////

	return hr;
}

DWORD CSimpleEventHelper::Wait(DWORD dwMillSec)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////

	DWORD dwWaitForSingleObject = WaitForSingleObject(
		m_hEvent,//_In_  HANDLE hHandle,
		dwMillSec//_In_  DWORD dwMilliseconds
		);
	if (dwWaitForSingleObject != WAIT_OBJECT_0)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("bWaitForSingleObject"), hr, TEXT("WaitResult=%d"), dwWaitForSingleObject);
	}

	//////////////////////////////////////////////////////////////////////////

	return dwWaitForSingleObject;
}



CMultipleEventHelper::CMultipleEventHelper(CONST BOOL * pbManualReset, LONG Count) :
m_RefCount(0)
{
	m_bManualResetVector.reserve(Count);
	for (LONG ii = 0; ii < Count; ++ii)
	{
		m_bManualResetVector.push_back(pbManualReset[ii]);
	}

	m_hEvents.reserve(Count);
}

HRESULT CMultipleEventHelper::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	HANDLE hTemp = NULL;
	for (size_t ii = 0; ii < m_bManualResetVector.size(); ++ii)
	{
		hTemp = CreateEvent(
			NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
			m_bManualResetVector[ii],//_In_      BOOL bManualReset,
			FALSE,//_In_      BOOL bInitialState,
			NULL//_In_opt_  LPCTSTR lpName
			);
		if (NULL == hTemp)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
			break;
		}

		m_hEvents.push_back(hTemp);
	}
	//////////////////////////////////////////////////////////////////////////

	return hr;
}

HRESULT CMultipleEventHelper::DestroyMember()
{
	//////////////////////////////////////////////////////////////////////////
	for (size_t ii = 0; ii < m_hEvents.size(); ++ii)
	{
		if (m_hEvents[ii])
		{
			CloseHandle(m_hEvents[ii]);
			m_hEvents[ii] = NULL;
		}
	}
	m_hEvents.clear();

	//////////////////////////////////////////////////////////////////////////
	return S_OK;
}

HRESULT CMultipleEventHelper::Set(LONG Index)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	BOOL bSetEvent = SetEvent(
		m_hEvents[Index]//_In_  HANDLE hEvent
		);
	if (!bSetEvent)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("SetEvent"), hr, TEXT(""));
	}

	//////////////////////////////////////////////////////////////////////////

	return hr;
}

HRESULT CMultipleEventHelper::Reset(LONG Index)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	BOOL bResetEvent = ResetEvent(
		m_hEvents[Index]//_In_  HANDLE hEvent
		);
	if (!bResetEvent)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("ResetEvent"), hr, TEXT(""));
	}

	//////////////////////////////////////////////////////////////////////////

	return hr;
}

DWORD CMultipleEventHelper::Wait(LONG Index, DWORD MillSec)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	DWORD dwWaitForSingleObject = WaitForSingleObject(
		m_hEvents[Index],//_In_  HANDLE hHandle,
		MillSec//_In_  DWORD dwMilliseconds
		);
	if (dwWaitForSingleObject != WAIT_OBJECT_0)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("bWaitForSingleObject"), hr, TEXT("WaitResult=%d"), dwWaitForSingleObject);
	}
	//////////////////////////////////////////////////////////////////////////

	return dwWaitForSingleObject;
}

DWORD CMultipleEventHelper::WaitMultiple(BOOL bWaitAll, DWORD MillSec)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	DWORD dwWaitForMultipleObjects = WaitForMultipleObjects(
		m_hEvents.size(),//_In_  DWORD nCount,
		&(m_hEvents[0]),//_In_  const HANDLE *lpHandles,
		bWaitAll,//_In_  BOOL bWaitAll,
		MillSec//_In_  DWORD dwMilliseconds
		);
	if (!(dwWaitForMultipleObjects >= WAIT_OBJECT_0 && (dwWaitForMultipleObjects < WAIT_OBJECT_0 + m_hEvents.size())))
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("WaitResult=%d"), dwWaitForMultipleObjects);
	}

	//////////////////////////////////////////////////////////////////////////

	return dwWaitForMultipleObjects;
}

LONG CMultipleEventHelper::GetEventCount()
{
	return m_hEvents.size();
}

HRESULT CMultipleEventHelper::SetAll()
{
	HRESULT hr = S_OK;
	HRESULT hrTemp = S_OK;
	for (size_t ii = 0; ii < m_hEvents.size(); ++ii)
	{
		hrTemp = Set(ii);
		if (FAILED(hrTemp))
		{
			hr = hrTemp;
		}
	}
	return hr;
}

HRESULT CMultipleEventHelper::ResetAll()
{
	HRESULT hr = S_OK;
	HRESULT hrTemp = S_OK;
	for (size_t ii = 0; ii < m_hEvents.size(); ++ii)
	{
		hrTemp = Reset(ii);
		if (FAILED(hrTemp))
		{
			hr = hrTemp;
		}
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCMultipleEventHelper(CONST BOOL * pbManualReset, LONG Count, IMultipleEventHelper ** ppIMultipleEventHelper, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT((new CMultipleEventHelper(pbManualReset, Count)), ppIMultipleEventHelper, bCreateMember);
}
