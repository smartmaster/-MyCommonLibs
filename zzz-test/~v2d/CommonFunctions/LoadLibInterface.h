#pragma once

#include "SimpleUnknown.h"

#include "CodeBlockDef.h"
#include "SimpleUnknown.h"


struct ILoadLib : public ISimpleUnknown
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

	//	virtual HRESULT CreateMember() = 0;
	//	virtual HRESULT DestroyMember() = 0;
	//};
	virtual FARPROC GetProc(LPCTSTR pProcName) = 0;
};

HRESULT CreateInstanceILoadLib(LPCTSTR pModuleName, BOOL bCreateMember, ILoadLib ** ppILoadLib);