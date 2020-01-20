#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "SocketRunAppRPCDefines.h"

#include "InterfaceWSADataBufferHandler.h"

class CRunAppHandler : public IDataBufferHandler
{
private:
	LONG m_RefCount;


public:
	CRunAppHandler();
	IMP_CLASS_DESTRUCTOR(CRunAppHandler);

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

	IMP_WHO_AM_I(TEXT("CRunAppHandler:IDataBufferHandler"));

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();
	//};
	virtual HRESULT Process( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	);
	//};

private:
	virtual HRESULT ProcessRunApp( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	);
};

HRESULT CreateInstanceRunAppHandler(IDataBufferHandler ** ppResponceHandler, BOOL bCreateMember);

class CRunAppHandlerFactory : public IDataBufferHandlerFactory
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

	IMP_WHO_AM_I(TEXT("CRunAppHandlerFactory:IDataBufferHandlerFactory"));

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};
	virtual HRESULT CreateInstance(IDataBufferHandler ** ppResponceHandler);
	//};

public:
	CRunAppHandlerFactory();

	IMP_CLASS_DESTRUCTOR(CRunAppHandlerFactory);
private:
	ULONG m_RefCount;
};

