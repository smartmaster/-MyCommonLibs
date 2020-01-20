#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>



#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicSimpleUnknown.h"

#include "CommandList.h"
#include "EumCallbackInterface.h"

struct IJobMainFlow : public ISimpleUnknown
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

	virtual HRESULT Main() = 0;
};



//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCDowloadClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_CREATE_DOWNLOAD_SESSION_REQ & data_creae_download_session_req,
	IJobMainFlow ** ppIJobMainFlow, BOOL bCreateMember);
HRESULT CreateInstanceCUploadClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_CREATE_DOWNLOAD_SESSION_REQ & data_creae_download_session_req, 
	IJobMainFlow ** ppIJobMainFlow, BOOL bCreateMember);
HRESULT CreateInstanceCRunAppClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_RUN_APP_REQ & body_run_app_req, 
	IJobMainFlow ** ppIJobMainFlow, BOOL bCreateMember);
HRESULT CreateInstanceCEnumClient(IFileDevice * pSender, IFileDevice * pReceiver, IEumCallback * pEumCallback, CONST BODY_BEGIN_ENUM_FILE_REQ & body_begin_enum_file_req, 
	IJobMainFlow ** ppIJobMainFlow, BOOL bCreateMember);

//////////////////////////////////////////////////////////////////////////
//HRESULT CreateInstanceCDownloadServer(SOCKET ConnectionSocket, IJobMainFlow ** ppIJobMainFlow, BOOL bCreateMember);
HRESULT CreateThreadServerMain(SOCKET ConnectionSocket);
DWORD WINAPI ThreadProcServerMain(_In_  LPVOID lpParameter);