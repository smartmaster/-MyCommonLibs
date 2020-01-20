#pragma once

#include "..\PublicHeader\PublicInterfaceSocketClient.h"

#include "HPTHandler.h"
#include "HPTSendReceiverInterface.h"
#include "CommandList.h"
#include "JobMainFlowbInterface.h"

#include "PluginFileFactoryConfig.h"
#include "PluginFileFactoryInterface.h"

class CDowloadClient : public IJobMainFlow
{
private:
	LONG m_RefCount; /***INTERNAL***/

	IFileDevice * m_pSender;  /***INCOMMING***/
	IFileDevice * m_pReceiver; /***INCOMMING***/
	BODY_CREATE_DOWNLOAD_SESSION_REQ m_data_creae_download_session_req; /***INCOMMING***/

	CComPtr<IHPTHandler> m_spIHPTHandlerClient; /***INTERNAL***/
	CComPtr<IHPTSendReceive> m_spIHPTSendReceive; /***INTERNAL***/
	HANDLE m_hSendReceiverMonitorThread; /***INTERNAL***/

	//PluginFileFactoryConfigs m_pffConfigs;

//private:
//	HRESULT GetPluginFileFactoryConfigs();

public:
	CDowloadClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_CREATE_DOWNLOAD_SESSION_REQ & data_creae_download_session_req);

	IMP_CLASS_DESTRUCTOR(CDowloadClient);

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
	IMP_WHO_AM_I(TEXT("CDowloadClient:IJobMainFlow"));

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();

	HRESULT Main();
	//};
};

HRESULT HPTDownloadClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_CREATE_DOWNLOAD_SESSION_REQ & data_creae_download_session_req);
HRESULT HPTDownloadClient(LPCTSTR pServer, LPCTSTR pPort, CONST BODY_CREATE_DOWNLOAD_SESSION_REQ & data_creae_download_session_req);
