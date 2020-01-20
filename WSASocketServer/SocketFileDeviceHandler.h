#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "SocketFileRPCDefines.h"


#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "InterfaceWSADataBufferHandler.h"

#include "ParameterStreamHelper.h"
#include "ParameterRPC.h"

class CSocketFileDeviceHandler : public IDataBufferHandler
{
private:
	LONG m_RefCount;
	CComPtr<IFileDevice> m_spIFileDevice;

public:
	CSocketFileDeviceHandler() :
		m_RefCount(0)
	{

	}
	IMP_CLASS_DESTRUCTOR(CSocketFileDeviceHandler);

public:
	//struct IDataBufferHandler : public ISimpleUnknown
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

	virtual LPCTSTR WhoAmI()
	{
		static CONST TCHAR WHO_AM_I[] = TEXT("CSocketFileDeviceHandler:IDataBufferHandler");
		return WHO_AM_I;
	}

	virtual HRESULT CreateMember()
	{
		return S_OK;
	}

	virtual HRESULT DestroyMember()
	{
		m_spIFileDevice.Release(); // in case client crash, it will be released by server to avoid leak
		return S_OK;
	}
	//};
	virtual HRESULT Process( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	);
	//};

private:
	virtual HRESULT ProcessCreateInstanceFileDeviceWin32( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	);
	virtual HRESULT ProcessSetPointerEx( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	);
	virtual HRESULT ProcessRead( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	);
	virtual HRESULT ProcessWrite( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	);
	virtual HRESULT ProcessGetFileSize( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	);
	virtual HRESULT ProcessReadAtOffsetHelper( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	);
	virtual HRESULT ProcessWriteAtOffsetHelper( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	);

	//DEFINE_PROCESS_METHOD_WITHOUT_PARAM(m_spIFileDevice, ., AddRef);
	DEFINE_PROCESS_METHOD_WITHOUT_PARAM_VOID(m_spIFileDevice, ., Release);
	DEFINE_PROCESS_METHOD_WITHOUT_PARAM_HR(m_spIFileDevice, ->, CreateMember);
	DEFINE_PROCESS_METHOD_WITHOUT_PARAM_HR(m_spIFileDevice, ->, DestroyMember);
	
};

HRESULT CreateInstanceCSocketFileDeviceHandler(IDataBufferHandler ** ppResponceHandler, BOOL bCreateMember);

class CSocketFileDeviceHandlerFactory : public IDataBufferHandlerFactory
{
public:
	//struct IDataBufferHandlerFactory : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//  //  //virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//  //  //	/* [in] */ REFIID riid,
	//  //  //	/* [iid_is][out] */ void **ppvObject);

	//  //  //virtual ULONG STDMETHODCALLTYPE AddRef( void);

	//  //  //virtual ULONG STDMETHODCALLTYPE Release( void);

	//	//	//	END_INTERFACE
	//	//	//};
	IMP_IUNKNOWN_METHODS(m_RefCount);

	virtual LPCTSTR WhoAmI();
	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};
	virtual HRESULT CreateInstance(IDataBufferHandler ** ppResponceHandler);
	//};

public:
	CSocketFileDeviceHandlerFactory();

	IMP_CLASS_DESTRUCTOR(CSocketFileDeviceHandlerFactory);
private:
	ULONG m_RefCount;
};

HRESULT CreateInstanceCSocketFileDeviceHandlerFactory(IDataBufferHandlerFactory ** ppResponceHandlerFactory, BOOL bCreateMember);