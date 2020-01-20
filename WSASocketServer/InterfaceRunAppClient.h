#pragma once

#include "..\PublicHeader\PublicSimpleUnknown.h"
#include "..\PublicHeader\PublicInterfaceSocketClient.h"
#include "SocketRunAppRPCDefines.h"

struct IRunAppClient : public ISimpleUnknown
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

	virtual HRESULT RunApp(
		LPCTSTR Username, 
		LPCTSTR Domain, 
		LPCTSTR Password, 
		LPCTSTR CommandLine, 
		LPCTSTR CurDir, 
		ULONG TimeOut, 
		BOOL TerminateOnTimeOut,
		ULONG & ExitCode) = 0;
};


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCRunAppClient( ISocketClient * pISocketClient, 
									LONG MaxBinaryDataLen, 
									IRunAppClient ** ppObj, 
									BOOL bCreateMember );
