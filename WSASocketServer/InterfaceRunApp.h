#pragma once

#include "..\PublicHeader\PublicSimpleUnknown.h"

struct IRunApp : public ISimpleUnknown
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

	virtual HRESULT Start() = 0;
	virtual HRESULT Wait(ULONG MillSec) = 0;
	virtual HRESULT Terminate() = 0;
	virtual HRESULT GetExitCode(ULONG & ExitCode) = 0;
};

HRESULT CreateInstanceCRunAppWin32( LPCTSTR Username, LPCTSTR Domain, LPCTSTR Password, LPCTSTR CommandLine, LPCTSTR CurDir, IRunApp ** ppObj, BOOL bCreateMember );


HRESULT RunApp(LPCTSTR Username, LPCTSTR Domain, LPCTSTR Password, LPCTSTR CommandLine, LPCTSTR CurDir, ULONG TimeOut, BOOL TerminateOnTimeOut, ULONG & ExitCode);