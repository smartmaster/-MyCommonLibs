#pragma once

#include "LoadLibInterface.h"

class CLoadLib : public ILoadLib
{
	HMODULE			m_hMoudle;	/***Internal to cleanup***/
	CONST CString	m_ModuleName;	/***Incomming***/
	LONG			m_RefCount; /***Internal***/

public:
	CLoadLib(LPCTSTR pModuleName);

	IMP_CLASS_DESTRUCTOR(CLoadLib);


	//struct ILoadLib : public ISimpleUnknown
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
	virtual FARPROC GetProc(LPCTSTR pProcName);
	//};
	//
};
