#pragma once


#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicInterfaceSocketClient.h"

#include "HPTHandler.h"
#include "HPTSendReceiverInterface.h"
#include "CommandList.h"
#include "JobMainFlowbInterface.h"

class CRunAppClient : public IJobMainFlow
{
private:
	LONG m_RefCount; /***INTERNAL***/

	IFileDevice * m_pSender;  /***INCOMMING***/
	IFileDevice * m_pReceiver; /***INCOMMING***/
	BODY_RUN_APP_REQ m_body_run_app_req; /***INCOMMING***/

	CComPtr<IHPTHandler> m_spIHPTHandlerClient; /***INTERNAL***/
	CComPtr<IHPTSendReceive> m_spIHPTSendReceive; /***INTERNAL***/
	HANDLE m_hSendReceiverMonitorThread; /***INTERNAL***/

	HPTHeader m_TempHeader; /***INTERNAL***/
	HPTBody m_TempBody; /***INTERNAL***/

public:
	CRunAppClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_RUN_APP_REQ & body_run_app_req);

	IMP_CLASS_DESTRUCTOR(CRunAppClient);

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
	IMP_WHO_AM_I(TEXT("CRunAppClient:IJobMainFlow"));

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();

	HRESULT Main();
	//};
};


//HRESULT CreateInstanceCRunAppClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_RUN_APP_REQ & body_run_app_req, IJobMainFlow ** ppIJobMainFlow, BOOL bCreateMember);

HRESULT HPTRunAppClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_RUN_APP_REQ & body_run_app_req);
HRESULT HPTRunAppClient(LPCTSTR pServer, LPCTSTR pPort, CONST BODY_RUN_APP_REQ & body_run_app_req);