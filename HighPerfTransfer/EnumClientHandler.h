#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>


#include "HPTHandler.h"
#include "CommandList.h"
#include "EventHelper.h"
#include "CreateWin32FileHelper.h"
#include "EumCallbackInterface.h"



class CEnumClientHandler : public IHPTHandler
{
private:
	IEumCallback * m_pEumCallback; /***INCOMMING***/


	LONG m_RefCount; /***INTERNAL***/
	CComPtr<IMultipleEventHelper> m_spIMultipleEventHelper; /***INTERNAL***/ /***TO CLEANUP***/
	ULONGLONG m_ServerHandler; /***INTERNAL***/

	LONG m_FileCount; /***INTERNAL***/
	

	BODY_HANDLER_STATUS_STATISTIC_TOTAL m_StatusStatistic; /***INTERNAL***/
	BODY_HANDLER_STATUS_STATISTIC_TOTAL m_StatusStatisticServer; /***INTERNAL***/

	volatile BOOL m_bPeerError; /***INTERNAL***/ //set once and get many times, so it is to use a BOOL for performance 



private:
	HRESULT ProcessCommand_CMD_CREATE_ENUM_FILE_SESSION_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

	HRESULT ProcessCommand_CMD_ENUM_FILE_DATA_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

	HRESULT ProcessCommand_CMD_END_ENUM_FILE_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

	HRESULT ProcessCommand_CMD_END_SESSION_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

	HRESULT ProcessCommand_CMD_RESULT_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

	HRESULT ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);


public:
	CEnumClientHandler(IEumCallback * pEumCallback);

	IMP_CLASS_DESTRUCTOR(CEnumClientHandler);

	//struct IHPTHandler : public ISimpleUnknown
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
	IMP_WHO_AM_I(TEXT("CEnumClientHandler:IHPTHandler"));

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT ProcessCommand(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

	//virtual HRESULT SetCurrentCommandFinished();

	virtual DWORD WaitAny(DWORD WaitMillSec);

	virtual DWORD ResetAll();

	virtual HRESULT Set(LONG Index);

	virtual HRESULT Reset(LONG Index);

	virtual HRESULT Wait(LONG Index, DWORD MillSec);

	virtual ULONGLONG GetServerHandler();

	virtual CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * GetStatusStatistic();

	virtual CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * GetStatusStatisticServer();

	//<sonmi01>2015-3-12 ###???
	virtual BOOL IsPeerError();
	//};
};

HRESULT CreateInstanceCEnumClientHandler(IHPTHandler ** ppIHPTHandler, BOOL bCreateMember);