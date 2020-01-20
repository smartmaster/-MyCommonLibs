#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"

#include "..\PublicHeader\PublicCircularBuffer.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicVMemPtr.h"

#include "HPTHandler.h"
#include "HPTSendReceiverInterface.h"
#include "EventHelper.h"

class CHPTSendReceive : public IHPTSendReceive
{
private:
	IFileDevice * m_pSender; /***INCOMMING***/ /***INTERNAL***/ /***TO CLEANUP***/
	IFileDevice * m_pReceiver; /***INCOMMING***/ /***INTERNAL***/ /***TO CLEANUP***/
	IHPTServerHandlerFactory * m_pHPTServerHandlerFactory; /***INCOMMING***/ //used by server only to create server handler
	IHPTHandler * m_pHPTServerHandler; /***INTERNAL***/ /***TO CLEANUP***/ //used by server only, copy of server handler in header, often created by m_pHPTHandlerDefault
	IHPTHandler * m_pHPTClientHandler; /***INCOMMING***/ //used by client only
	BOOL m_bServer; /***INCOMMING***/
	BOOL m_bAttachSenderReceiver; /***INCOMMING***/

	CVMemPtr<BYTE> m_apBufferSend; /***INTERNAL***/ /***TO CLEANUP***/
	CAutoPtr<CCircularBuffer> m_apCircBufferSend;  /***INTERNAL***//***TO CLEANUP***/
	CVMemPtr<BYTE> m_apBufferReceive; /***INTERNAL***/ /***TO CLEANUP***/
	CAutoPtr<CCircularBuffer> m_apCircBufferReceive; /***INTERNAL***//***TO CLEANUP***/
	HANDLE m_hThread[3]; /***INTERNAL***//***TO CLEANUP***/

	LONG m_RefCount; /***INTERNAL***/
	CComPtr<IMultipleEventHelper> m_spIMultipleEventHelper; /***INTERNAL***//***TO CLEANUP***/

	static CONST LONG S_BUFFER_SIZE = 8 * 1024 * 1024;

private:
	static DWORD WINAPI ThreadReceiverProc(LPVOID lpParameter);
	HRESULT ReceiverProc();

	static DWORD WINAPI ThreadHandlerProc(LPVOID lpParameter);
	HRESULT HandlerProc();

	static DWORD WINAPI ThreadSenderProc(LPVOID lpParameter);
	HRESULT SenderProc();

public:
	CHPTSendReceive(IFileDevice * pSender, IFileDevice * pReceiver, BOOL bAttachSenderReceiver, IHPTServerHandlerFactory * pHPTNewHandlerFactory, BOOL bServer);

	IMP_CLASS_DESTRUCTOR(CHPTSendReceive);

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
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("CHPTSendReceive:IHPTSendReceive"));

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();


	virtual HRESULT PutCommandToSend(CONST HPTHeader & Header, CONST HPTBody & Body); //PutCommandToSend and IHPTHandler::Process must be syncronized if IHPTHandler::Process also put data into m_apCircBufferSend
	virtual HRESULT SetClientHandler(IHPTHandler * pClientHandler);
	
	virtual HRESULT Set(LONG WaitIndexe);
	virtual DWORD WaitToEnd(DWORD MillSec);
	virtual HRESULT StopProcess();
	
	//};
		
};

////////////////////////////////////////////////////////////////////////////
//HRESULT PutCommmandHelper(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBuffer);
//HRESULT GetCommmandHelper(HPTHeader & Header, HPTBody & Body, CCircularBuffer * pCircBuffer);


