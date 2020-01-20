#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "TaskMonitorInterface.h"

class CTaskMonitor : public ITaskMonitor
{
private:
	ULONG m_TimeOut;	/***INCOMMING***/ // millSec
	LONG m_refCount;	/***INTERNAL***/
	HANDLE m_events[2]; /***INTERNAL***/ /***CLEANUP***/
	HANDLE m_hThread; /***INTERNAL***/ /***CLEANUP***/
public:
	CTaskMonitor(ULONG TimeOut);

	IMP_CLASS_DESTRUCTOR(CTaskMonitor);

	//struct ITaskMonitor : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};
	IMP_IUNKNOWN_METHODS(m_refCount);

	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("class CTaskMonitor : public ITaskMonitor"));
	//	//	virtual HRESULT CreateMember() = 0;
	virtual HRESULT CreateMember();
	//	//	virtual HRESULT DestroyMember() = 0;
	virtual HRESULT DestroyMember();
	//	//};

	//	virtual HRESULT Start() = 0;
	virtual HRESULT Start();
	//	virtual HRESULT End() = 0;
	virtual HRESULT Stop();

	virtual HRESULT Restart();

	//	virtual HRESULT SetTaskDone() = 0;
	virtual HRESULT SetTaskDone();
	//};


private:
	static DWORD WINAPI ThreadProc(
		_In_  LPVOID lpParameter
		);


	HRESULT ThreadMethod();
};