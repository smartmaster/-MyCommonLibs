#pragma once


#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicMiscHelper.h"
#include "..\PublicHeader\PublicInterfaceSocketClient.h"

#include "CommandList.h"
#include "HPTHandler.h"
#include "HPTSendReceiverInterface.h"
#include "JobMainFlowbInterface.h"

class CServerJobMainFlow : public IJobMainFlow
{
private:
	LONG m_RefCount; /***INTERNAL***/
	SOCKET m_ConnectionSocket; /***INCOMMING***/ /***TO CLEANUP***/
	BOOL m_bUseSsl; /**INCOMMING**/

public:
	CServerJobMainFlow(SOCKET ConnectionSocket, BOOL bUseSsl);

	IMP_CLASS_DESTRUCTOR(CServerJobMainFlow);
	//struct IJobMainFlow : public ISimpleUnknown
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
	IMP_WHO_AM_I(TEXT("CServerJobMainFlow:IJobMainFlow"));

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();

	//	//};

	virtual HRESULT Main();
	//};

};

