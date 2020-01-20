#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "..\PublicHeader\PublicSimpleUnknown.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"

#include "HPTHandler.h"
#include "HPTHeaderBody.h"

struct IHPTSendReceive : public ISimpleUnknown
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

	virtual HRESULT PutCommandToSend(CONST HPTHeader & Header, CONST HPTBody & Body) = 0;  //used by client //PutCommandToSend and IHPTHandler::Process must be syncronized if IHPTHandler::Process also put data into m_apCircBufferSend
	virtual HRESULT SetClientHandler(IHPTHandler * pClientHandler) = 0; //used by client

	virtual HRESULT Set(LONG WaitIndexe) = 0;
	virtual DWORD WaitToEnd(DWORD MillSec) = 0;
	virtual HRESULT StopProcess() = 0;
};


//////////////////////////////////////////////////////////////////////////
HRESULT PutCommmandHelper(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBuffer);
HRESULT GetCommmandHelper(HPTHeader & Header, HPTBody & Body, CCircularBuffer * pCircBuffer);


//////////////////////////////////////////////////////////////////////////
DWORD WINAPI SendReceiverMonitorProc(IHPTSendReceive * pIHPTSendReceive);
HANDLE CreateSendReceiverMonitorThread(IHPTSendReceive * pIHPTSendReceive);



//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCHPTSendReceive(
	IFileDevice * pSender,
	IFileDevice * pReceiver,
	BOOL bAttachSenderReceiver,
	IHPTServerHandlerFactory * pHPTNewHandlerFactory,
	BOOL bServer,
	IHPTSendReceive  ** ppIHPTSendReceive,
	BOOL bCreateMember);