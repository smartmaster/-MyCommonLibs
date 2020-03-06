#include "stdafx.h"

// EventSink.cpp
#include "ProcessStartEventSink.h"
#include "WmiObjectWrapper.h"


ULONG ProcessStartEventSink::AddRef()
{
	return InterlockedIncrement(&m_lRef);
}

ULONG ProcessStartEventSink::Release()
{
	LONG lRef = InterlockedDecrement(&m_lRef);
	if(lRef == 0)
		delete this;
	return lRef;
}

HRESULT ProcessStartEventSink::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
	{
		*ppv = (IWbemObjectSink *) this;
		AddRef();
		return WBEM_S_NO_ERROR;
	}
	else return E_NOINTERFACE;
}


HRESULT ProcessStartEventSink::Indicate(long lObjectCount,
	IWbemClassObject **apObjArray)
{
	HRESULT hres = S_OK;

	wprintf_s(L"ProcessStart Event occurred" L"\r\n");
	for (int ii = 0; ii < lObjectCount; ii++)
	{
		CWmiObjectWrapper WmiObject(apObjArray[ii]);
		LONG ParentProcessID = WmiObject.GetLongProperty(TEXT("ParentProcessID"));
		LONG ProcessID = WmiObject.GetLongProperty(TEXT("ProcessID"));
		wstring ProcessName = WmiObject.GetStringProperty(TEXT("ProcessName"));
		LONG SessionID = WmiObject.GetLongProperty(TEXT("SessionID"));
		wprintf_s(L"ParentProcessID=%04d, ProcessID=%04d, SessionID=%04d, ProcessName=%s" L"\r\n", ParentProcessID, ProcessID, SessionID, ProcessName.c_str());
	}
	wprintf_s(L"ProcessStart Event done" L"\r\n\r\n\r\n");

	return WBEM_S_NO_ERROR;
}

HRESULT ProcessStartEventSink::SetStatus(
	/* [in] */ LONG lFlags,
	/* [in] */ HRESULT hResult,
	/* [in] */ BSTR strParam,
	/* [in] */ IWbemClassObject __RPC_FAR *pObjParam
	)
{
	if(lFlags == WBEM_STATUS_COMPLETE)
	{
		wprintf_s(L"Call complete. hResult = 0x%X\n", hResult);
	}
	else if(lFlags == WBEM_STATUS_PROGRESS)
	{
		wprintf_s(L"Call in progress.\n");
	}

	return WBEM_S_NO_ERROR;
}    // end of EventSink.cpp

HRESULT ProcessStartEventSink::CreateInstance( IWbemObjectSink ** ppOut )
{
	ProcessStartEventSink * pObj = new ProcessStartEventSink;
	pObj->AddRef();
	*ppOut = pObj;
	pObj = NULL;
	return S_OK;
}
