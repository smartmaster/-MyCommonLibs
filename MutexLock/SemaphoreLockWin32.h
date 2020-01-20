#pragma once

#include <tchar.h>
#include <windows.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlbase.h>
#include <atlstr.h>

#include "InterfaceMutexLock.h"

class CSemaphoreLockWin32 : public IProcLock
{
private:
	CString	m_ObjectName; /***INCOMMING***/
	LONG m_InitialCount; /***INCOMMING***/
	LONG m_MaximumCount; /***INCOMMING***/
	//DWORD	m_DesiredAccess; /***INCOMMING***/
	ULONG m_RefCount; /***INTERNAL***/
	HANDLE	m_Semaphore; /***INTERNAL TO CLEANUP***/

public:
	CSemaphoreLockWin32(LPCTSTR ObjectName, LONG lInitialCount, LONG lMaximumCount/*, DWORD DesiredAccess*/);

	IMP_CLASS_DESTRUCTOR(CSemaphoreLockWin32);

public:
	//struct  IMutextLock : public ISimpleUnknown
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
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//virtual LPCTSTR WhoAmI();
	IMP_WHO_AM_I(TEXT("CSemaphoreLockWin32:IProcLock"));

	//virtual HRESULT CreateMember();
	virtual HRESULT CreateMember();
	//virtual HRESULT DestroyMember();
	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT Lock(ULONG Timeout);
	virtual HRESULT Unlock();
	//};
};


