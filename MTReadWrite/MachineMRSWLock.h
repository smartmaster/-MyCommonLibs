#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicInterfaceLock.h"

#include "sharedData.h"

/*********************
based on http://www.glennslayden.com/code/win32/reader-writer-lock
*************************/

class MachineMRSWLock
{
private:
	CHandle m_csWrite;
	CHandle m_csReaderCount;
	CSharedData<long> m_cReaders;
	CHandle m_hevReadersCleared;

	HRESULT m_result;

private:
	HRESULT CloseMember();

public:
	MachineMRSWLock(LPCTSTR szBaseName);

	~MachineMRSWLock();

	HRESULT Result();

	void EnterReader(void);

	void LeaveReader(void);

	void EnterWriter(void);

	void LeaveWriter(void);
};


class  CRWLockProcessWin32 : public IThreadRWLock
{
private:
	ULONG m_RefCount; /***INTERNAL***/
	CString m_name; /***INCOMMING***/
	CAutoPtr<MachineMRSWLock> m_pImpl; /***INTERNAL***/

public:
	CRWLockProcessWin32(LPCTSTR name) :
		m_RefCount(0)
	{
	}

	IMP_CLASS_DESTRUCTOR(CRWLockProcessWin32);


	//struct  IRWLock : public ILock
	//{
	//	//struct  ILock : public ISimpleUnknown
	//	//{
	//	//	//struct ISimpleUnknown : public IUnknown
	//	//	//{
	//	//	//	//	MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	//	IUnknown
	//	//	//	//	{
	//	//	//	//	public:
	//	//	//	//		BEGIN_INTERFACE
	//	//	//	//			virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//	//			/* [in] */ REFIID riid,
	//	//	//	//			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) = 0;
	//	//	//	//
	//	//	//	//			virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;
	//	//	//	//
	//	//	//	//			virtual ULONG STDMETHODCALLTYPE Release( void) = 0;
	//	//	//	//		END_INTERFACE
	//	//	//	//	};
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//virtual LPCTSTR WhoAmI();
	IMP_WHO_AM_I(TEXT("class  CRWLockProcessWin32 : public IThreadRWLock"));

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();

	//	//	//};

	virtual HRESULT Lock();

	virtual HRESULT Unlock();
	//	//};
	virtual HRESULT SharedLock();

	virtual HRESULT SharedUnlock();
	//};

private:

};

//API
HRESULT CreateInstacnceCRWLockProcessWin32(IThreadRWLock ** ppIRWLock, LPCTSTR name, BOOL bCreateMember);