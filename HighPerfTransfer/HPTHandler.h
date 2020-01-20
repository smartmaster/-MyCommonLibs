#pragma once

#include "..\CommonSources\SimpleUnknown.h"
#include "..\PublicHeader\PublicCircularBuffer.h"

#include "HPTHeaderBody.h"
#include "CommandList.h"
#include "EumCallbackInterface.h"


//////////////////////////////////////////////////////////////////////////
struct IHPTHandler : public ISimpleUnknown
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

	virtual HRESULT ProcessCommand(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve) = 0;
	//virtual HRESULT SetCurrentCommandFinished() = 0;
	virtual HRESULT Set(LONG Index) = 0;
	virtual HRESULT Reset(LONG Index) = 0;
	virtual HRESULT Wait(LONG Index, DWORD MillSec) = 0;
	virtual DWORD WaitAny(DWORD WaitMillSec) = 0;
	virtual DWORD ResetAll() = 0;

	virtual ULONGLONG GetServerHandler() = 0;

	virtual CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * GetStatusStatistic() = 0;
	virtual CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * GetStatusStatisticServer() = 0; //used by client only

	virtual BOOL IsPeerError() = 0; //<sonmi01>2015-3-12 ###???
};


struct IHPTServerHandlerFactory : public ISimpleUnknown
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

	virtual HRESULT CreateServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppNewIHPTHandler, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve) = 0;
	virtual CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * GetStatusStatistic() = 0;
};


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCDownloadServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppIDownloadHandler, BOOL bCreateMember);
HRESULT CreateInstanceCDownloadClientHandler(IHPTHandler ** ppIHPTHandler, BOOL bCreateMember);

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCUploadServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppIDownloadHandler, BOOL bCreateMember);
HRESULT CreateInstanceCUploadClientHandler(IHPTHandler ** ppIHPTHandler, BOOL bCreateMember);

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCRunAppServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppIDownloadHandler, BOOL bCreateMember);
HRESULT CreateInstanceCRunAppClientHandler(IHPTHandler ** ppIHPTHandler, BOOL bCreateMember);

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCEnumServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppIDownloadHandler, BOOL bCreateMember);
HRESULT CreateInstanceCEnumClientHandler(IEumCallback * pEumCallback, IHPTHandler ** ppIHPTHandler, BOOL bCreateMember);

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCHPTServerHandlerFactory(IHPTServerHandlerFactory ** ppIHPTServerHandlerFactory, BOOL bCreateMember);