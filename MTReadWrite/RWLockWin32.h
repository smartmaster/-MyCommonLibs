#pragma once

#include <tchar.h>
#include <windows.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicInterfaceLock.h"

//#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
//namespace DTRACELIB_N {
//#endif
namespace
{
	//
	// Multi-reader Single-writer concurrency base class for Win32
	//
	// (c) 1999-2003 by Glenn Slayden (glenn@glennslayden.com)
	//
	//
	//http://www.glennslayden.com/code/win32/reader-writer-lock
	//
	class MultiReaderSingleWriter
	{
	private:
		CRITICAL_SECTION m_csWrite;
		CRITICAL_SECTION m_csReaderCount;
		long m_cReaders;
		HANDLE m_hevReadersCleared;

	public:
		MultiReaderSingleWriter()
		{
			m_cReaders = 0;
			InitializeCriticalSection(&m_csWrite);
			InitializeCriticalSection(&m_csReaderCount);
			m_hevReadersCleared = CreateEvent(NULL, TRUE, TRUE, NULL);
		}

		~MultiReaderSingleWriter()
		{
			WaitForSingleObject(m_hevReadersCleared, INFINITE);
			CloseHandle(m_hevReadersCleared);
			DeleteCriticalSection(&m_csWrite);
			DeleteCriticalSection(&m_csReaderCount);
		}


		void EnterReader(void)
		{
			EnterCriticalSection(&m_csWrite);
			EnterCriticalSection(&m_csReaderCount);
			if (++m_cReaders == 1)
				ResetEvent(m_hevReadersCleared);
			LeaveCriticalSection(&m_csReaderCount);
			LeaveCriticalSection(&m_csWrite);
		}

		void LeaveReader(void)
		{
			EnterCriticalSection(&m_csReaderCount);
			if (--m_cReaders == 0)
				SetEvent(m_hevReadersCleared);
			LeaveCriticalSection(&m_csReaderCount);
		}

		void EnterWriter(void)
		{
			EnterCriticalSection(&m_csWrite);
			WaitForSingleObject(m_hevReadersCleared, INFINITE);
		}

		void LeaveWriter(void)
		{
			LeaveCriticalSection(&m_csWrite);
		}
	};
} //end namespace

class  CRWLockWin32 : public IThreadRWLock
{
private:
	ULONG m_RefCount; /***INTERNAL***/
	CAutoPtr<MultiReaderSingleWriter> m_pImpl;

public:
	CRWLockWin32();

	IMP_CLASS_DESTRUCTOR(CRWLockWin32);


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
	IMP_WHO_AM_I(TEXT("class  CRWLockWin32 : public IThreadRWLock"));

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


HRESULT CreateInstacnceCRWLockWin32(IThreadRWLock ** ppIRWLock, BOOL bCreateMember);

//#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
//};  //end namespace DTRACELIB_N
//#endif