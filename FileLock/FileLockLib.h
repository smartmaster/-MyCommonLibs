#pragma once

#include <vector>
using std::vector;

#include "FileLockInterface.h"


class CFileLock : public IFileLock
{
private:
	LONG m_RefCount; /***Internal***/
	CString m_strFileName; /***Incomming***/
	HANDLE m_hFile; /***Internal to cleanup***/

	//private:
	//	HRESULT DestroyMember();
	//
public:
	CFileLock(LPCTSTR szFileName);
	IMP_CLASS_DESTRUCTOR(CFileLock);

	//struct IFileLock : public ISimpleUnknown
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

	virtual LPCTSTR WhoAmI()
	{
		static CONST TCHAR WHO_AM_I[] = TEXT("CFileLock:IFileLock");
		return WHO_AM_I;
	}
	virtual HRESULT CreateMember()
	{
		return S_OK;
	}
	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT Lock();
	virtual HRESULT SharedLock();

	virtual HRESULT LockEx(ULONG ulSleepInterval, ULONG ulRetryCount);
	virtual HRESULT SharedLockEx(ULONG ulSleepInterval, ULONG ulRetryCount);

	virtual HRESULT Unlock();
	//};
};

class CFileLockMultiple : public IFileLock
{
private:
	LONG m_RefCount;
	vector<CFileLock*> m_Locks;
	vector<CString> m_FileNames;

public:
	CFileLockMultiple(CONST vector<CString> & LockingFiles);
	IMP_CLASS_DESTRUCTOR(CFileLockMultiple);

	//struct IFileLock : public ISimpleUnknown
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

	virtual LPCTSTR WhoAmI()
	{
		static CONST TCHAR WHO_AM_I[] = TEXT("CFileLockMultiple:IFileLock");
		return WHO_AM_I;
	}
	virtual HRESULT CreateMember()
	{
		return S_OK;
	}
	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT Lock();
	virtual HRESULT SharedLock();

	virtual HRESULT LockEx(ULONG ulSleepInterval, ULONG ulRetryCount);
	virtual HRESULT SharedLockEx(ULONG ulSleepInterval, ULONG ulRetryCount);

	virtual HRESULT Unlock();
	//};
};
