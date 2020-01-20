#pragma once

#include "..\PublicHeader\PublicSimpleUnknown.h"

#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
namespace DTRACELIB_N {
#endif

struct  IThreadLock : public ISimpleUnknown
{
	//struct ISimpleUnknown : public IUnknown
	//{
	//	//	MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	IUnknown
	//	//	{
	//	//	public:
	//	//		BEGIN_INTERFACE
	//	//			virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//			/* [in] */ REFIID riid,
	//	//			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) = 0;
	//	//
	//	//			virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;
	//	//
	//	//			virtual ULONG STDMETHODCALLTYPE Release( void) = 0;
	//	//		END_INTERFACE
	//	//	};

	//	virtual LPCTSTR WhoAmI() = 0;
	//	virtual HRESULT CreateMember() = 0;
	//	virtual HRESULT DestroyMember() = 0;
	//};

	virtual HRESULT Lock() = 0;
	virtual HRESULT Unlock() = 0;
};

struct  IThreadRWLock : public IThreadLock
{
	//struct  ILock : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//	MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//	{
	//	//	//	public:
	//	//	//		BEGIN_INTERFACE
	//	//	//			virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//			/* [in] */ REFIID riid,
	//	//	//			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) = 0;
	//	//	//
	//	//	//			virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;
	//	//	//
	//	//	//			virtual ULONG STDMETHODCALLTYPE Release( void) = 0;
	//	//	//		END_INTERFACE
	//	//	//	};

	//	//	virtual LPCTSTR WhoAmI() = 0;
	//	//	virtual HRESULT CreateMember() = 0;
	//	//	virtual HRESULT DestroyMember() = 0;
	//	//};

	//	virtual HRESULT Lock() = 0;
	//	virtual HRESULT Unlock() = 0;
	//};
	virtual HRESULT SharedLock() = 0;
	virtual HRESULT SharedUnlock() = 0;
};


//////////////////////////////////////////////////////////////////////////
// APIs go here
HRESULT CreateInstacnceSRWLock( IThreadRWLock ** ppIRWLock, BOOL bCreateMember );
HRESULT CreateInstacnceCriticalSectionLock(IThreadRWLock ** ppLock, BOOL bCreateMember);
HRESULT CreateInstacnceCRWLockWin32(IThreadRWLock ** ppIRWLock, BOOL bCreateMember);

#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
};  //end namespace DTRACELIB_N
#endif