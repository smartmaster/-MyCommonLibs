#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicSimpleUnknown.h"

struct IFileLock : public ISimpleUnknown
{
	//struct ISimpleUnknown : public IUnknown
	//{
	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	IUnknown
	//	//{
	//	//public:
	//	//	BEGIN_INTERFACE
	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//		/* [in] */ REFIID riid,
	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	END_INTERFACE
	//	//};

	//	virtual LPCTSTR WhoAmI() = 0;
	//	virtual HRESULT CreateMember() = 0;
	//	virtual HRESULT DestroyMember() = 0;
	//};

	virtual HRESULT Lock() = 0;
	virtual HRESULT SharedLock() = 0;

	virtual HRESULT LockEx(ULONG ulSleepInterval, ULONG ulRetryCount) = 0;
	virtual HRESULT SharedLockEx(ULONG ulSleepInterval, ULONG ulRetryCount) = 0;

	virtual HRESULT Unlock() = 0;
};

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstancCFileLock( LPCTSTR FileName, IFileLock ** ppObj, BOOL bCreateMember );
HRESULT CreateInstanceCFileLockMultiple( CONST vector<CString> & LockingFiles, IFileLock ** ppObj, BOOL bCreateMember );