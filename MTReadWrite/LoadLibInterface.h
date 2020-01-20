#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>



#include "..\PublicHeader\PublicSimpleUnknown.h"
#include "..\PublicHeader\PublicCodeBlock.h"



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
	//	virtual LPCTSTR WhoAmI() = 0;
	//	virtual HRESULT CreateMember() = 0;
	//	virtual HRESULT DestroyMember() = 0;
	//};
	virtual FARPROC GetProc(LPCTSTR pProcName) = 0;
};

HRESULT CreateInstanceCLoadLib( LPCTSTR pModuleName, ILoadLib ** ppILoadLib , BOOL bCreateMember);