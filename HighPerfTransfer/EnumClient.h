#pragma once


#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicInterfaceSocketClient.h"

#include "HPTHandler.h"
#include "HPTSendReceiverInterface.h"
#include "CommandList.h"
#include "JobMainFlowbInterface.h"
#include "HPTHelper.h"
#include "EventHelper.h"
#include "EumCallbackInterface.h"

class CEnumClient : public IJobMainFlow
{
private:
	LONG m_RefCount; /***INTERNAL***/

	IFileDevice * m_pSender;  /***INCOMMING***/
	IFileDevice * m_pReceiver; /***INCOMMING***/
	IEumCallback * m_pEumCallback; /***INCOMMING***/
	BODY_BEGIN_ENUM_FILE_REQ m_body_begin_enum_file_req; /***INCOMMING***/


	CComPtr<IHPTHandler> m_spIHPTHandlerClient; /***INTERNAL***/
	CComPtr<IHPTSendReceive> m_spIHPTSendReceive; /***INTERNAL***/
	HANDLE m_hSendReceiverMonitorThread; /***INTERNAL***/

public:
	CEnumClient(IFileDevice * pSender, IFileDevice * pReceiver, IEumCallback * pEumCallback, CONST BODY_BEGIN_ENUM_FILE_REQ & body_begin_enum_file_req);

	IMP_CLASS_DESTRUCTOR(CEnumClient);

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
	IMP_WHO_AM_I(TEXT("CEnumClient:IJobMainFlow"));

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();

	HRESULT Main();
	//};
};

HRESULT CreateInstanceCEnumClient(IFileDevice * pSender, IFileDevice * pReceiver, IEumCallback * pEumCallback, CONST BODY_BEGIN_ENUM_FILE_REQ & body_begin_enum_file_req, IJobMainFlow ** ppIJobMainFlow, BOOL bCreateMember);

HRESULT HPTEnumClient(IFileDevice * pSender, IFileDevice * pReceiver, IEumCallback * pEumCallback, CONST BODY_BEGIN_ENUM_FILE_REQ & body_begin_enum_file_req);
HRESULT HPTEnumClient(LPCTSTR pServer, LPCTSTR pPort, IEumCallback * pEumCallback, CONST BODY_BEGIN_ENUM_FILE_REQ & body_begin_enum_file_req);