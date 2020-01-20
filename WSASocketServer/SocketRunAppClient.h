#pragma once

#include "InterfaceRunAppClient.h"



//////////////////////////////////////////////////////////////////////////
class  CRunAppClient : public IRunAppClient
{
private:
	LONG m_RefCount; /***Internal***/
	ISocketClient * m_pISocketClient; /***Incomming***/

	WSADataBufferT m_SendBuffer; /***Internal to cleanup***/
	WSADataBufferT m_RcvBuffer; /***Internal to cleanup***/
public:
	CRunAppClient(ISocketClient * pISocketClient, LONG MaxBinaryDataLen);

	IMP_CLASS_DESTRUCTOR(CRunAppClient);
public:
	//struct IRunAppClient : public ISimpleUnknown
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

	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("CRunAppClient:IRunAppClient"));

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};

	HRESULT RunApp(
		LPCTSTR Username, 
		LPCTSTR Domain, 
		LPCTSTR Password, 
		LPCTSTR CommandLine, 
		LPCTSTR CurDir, 
		ULONG TimeOut, 
		BOOL TerminateOnTimeOut,
		ULONG & ExitCode);

};