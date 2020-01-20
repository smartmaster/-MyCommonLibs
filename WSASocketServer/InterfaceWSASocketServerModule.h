#pragma once

#include "..\PublicHeader\PublicSimpleUnknown.h"
#include "InterfaceWSADataBufferHandler.h"

struct IWSASocketServerModule : public ISimpleUnknown
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

	virtual HRESULT RunServer() = 0;
	virtual HRESULT EndServer() = 0;
};


//////////////////////////////////////////////////////////////////////////
//API
HRESULT CreateInstanceCWSASocketServerModule(LPCTSTR pPort, LONG ThreadCount, LONG BufferInitialSize, IDataBufferHandlerFactory * pIDataBufferHandlerFactory, BOOL bUseThreadSocket, IWSASocketServerModule ** ppServerModule, BOOL bCreateMember);