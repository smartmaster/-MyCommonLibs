#pragma once

#include "InterfaceWSADataBufferHandler.h"


//////////////////////////////////////////////////////////////////////////
//a very simplest sample
class CEchoResponceHandler : public IDataBufferHandler
{
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
	virtual HRESULT Process( IN CONST WSADataBufferT & InBuffer, 	OUT WSADataBufferT & OutBuffer 	);
	//};

public:
	CEchoResponceHandler();

	IMP_CLASS_DESTRUCTOR(CEchoResponceHandler);

private:
	ULONG m_RefCount;
};

class CEchoResponceHandlerFactory : public IDataBufferHandlerFactory
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
	CEchoResponceHandlerFactory();

	IMP_CLASS_DESTRUCTOR(CEchoResponceHandlerFactory);
private:
	ULONG m_RefCount;
};
///////////////////////////////////////////