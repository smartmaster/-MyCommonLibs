#pragma once

#include <tchar.h>
#include <windows.h>

#include "InterfaceLock.h"

#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
namespace DTRACELIB_N {
#endif

class  CCiticalSectionLock : public IThreadRWLock
{
private:
	ULONG m_RefCount; /***INTERNAL***/

	CRITICAL_SECTION m_CS; /***INTERNAL TO CLEANUP***/
public:
	CCiticalSectionLock();
	IMP_CLASS_DESTRUCTOR(CCiticalSectionLock);

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

	virtual LPCTSTR WhoAmI();

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

#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
};  //end namespace DTRACELIB_N
#endif