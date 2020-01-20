#pragma once



#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicInterfaceSocketClient.h"

#include "HPTHandler.h"
#include "HPTSendReceiverInterface.h"
#include "CommandList.h"
#include "JobMainFlowbInterface.h"
#include "PluginFileFactoryConfig.h"
#include "PluginFileFactoryInterface.h"

class CUploadClient : public IJobMainFlow
{
private:
	LONG m_RefCount; /***INTERNAL***/

	IFileDevice * m_pSender;  /***INCOMMING***/
	IFileDevice * m_pReceiver; /***INCOMMING***/
	BODY_CREATE_UPLOAD_SESSION_REQ m_ClientServerFiles; /***INCOMMING***/

	CComPtr<IHPTHandler> m_spIHPTHandlerClient; /***INTERNAL***/
	CComPtr<IHPTSendReceive> m_spIHPTSendReceive; /***INTERNAL***/
	HANDLE m_hSendReceiverMonitorThread; /***INTERNAL***/

	HPTHeader m_TempHeader; /***INTERNAL***/
	HPTBody m_TempBody; /***INTERNAL***/

	PluginFileFactoryConfigs m_pffConfigs;

	

private:
	HRESULT ProcessFile(ULONGLONG ServerHandler, LONG FileIndex);

	//<sonmi01>2015-3-7 ###???
	HRESULT ProcessWin32FileHelper(CONST BODY_BEGIN_FILE_REQ & body_begin_file_req, ULONGLONG ServerHandler);
	HRESULT ProcessBuffer(IFileDevice * spFileDevide, ULONGLONG ServerHandler, LONGLONG Offset, LONGLONG FileSize);

	HRESULT GetPluginFileFactoryConfigs();
	HRESULT ProcessPluginFileHelper(CONST BODY_BEGIN_FILE_REQ & body_begin_file_req, ULONGLONG ServerHandler);


public:
	CUploadClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_CREATE_UPLOAD_SESSION_REQ & body_creae_upload_session_req);

	IMP_CLASS_DESTRUCTOR(CUploadClient);

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
	IMP_WHO_AM_I(TEXT("CUploadClient:IJobMainFlow"));

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();

	HRESULT Main();
	//};
};

HRESULT HPTUploadClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_CREATE_UPLOAD_SESSION_REQ & data_creae_upload_session_req);
HRESULT HPTUploadClient(LPCTSTR pServer, LPCTSTR pPort, CONST BODY_CREATE_UPLOAD_SESSION_REQ & data_creae_upload_session_req);