#pragma once

#include <tchar.h>
#include <windows.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlbase.h>
#include <atlstr.h>

#include "InterfaceMutexLock.h"

class CMutexLockWin32 : public IProcLock
{
private:
	CString	m_ObjectName; /***INCOMMING***/
	//DWORD	m_DesiredAccess; /***INCOMMING***/
	ULONG m_RefCount; /***INTERNAL***/
	HANDLE	m_Mutex; /***INTERNAL TO CLEANUP***/

public:
	CMutexLockWin32(LPCTSTR ObjectName/*, DWORD DesiredAccess*/);
	IMP_CLASS_DESTRUCTOR(CMutexLockWin32);

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
	
	virtual LPCTSTR WhoAmI();
	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT Lock(ULONG Timeout);
	virtual HRESULT Unlock();
	//};
};


