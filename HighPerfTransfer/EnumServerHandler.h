#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>


#include "..\PublicHeader\PublicEnumWinFile.h"
#include "..\PublicHeader\PublicMiscHelper.h"

#include "HPTSendReceiver.h"
#include "HPTHandler.h"
#include "CommandList.h"
#include "EventHelper.h"


class CEnumServerHandlerFileOp : public IEnumFolderFileOperation
{
private:
	CONST BODY_BEGIN_ENUM_FILE_REQ & m_body_begin_enum_file_req; /***INCOMMING***/
	CCircularBuffer * m_pCircBufferResult; /***INCOMMING***/
	BODY_HANDLER_STATUS_STATISTIC_TOTAL & m_StatusStatistic; /***INCOMMING***/
	ULONGLONG m_ClientHandler; /***INCOMMING***/
	ULONGLONG m_ServerHandler; /***INCOMMING***/

	BODY_ENUM_FILE_DATA_RSP m_body_enum_file_data_rsp; /***INTERNAL***/
	HPTHeader m_TempHeader; /***INTERNAL***/
	HPTBody m_TempBody; /***INTERNAL***/ /***TO CLEANUP***/

private:
	static CONST LONG S_RESERVE_COUNT = 1024;

public:
	CEnumServerHandlerFileOp(
		CONST BODY_BEGIN_ENUM_FILE_REQ & body_begin_enum_file_req, 
		CCircularBuffer * pCircBufferResult, 
		BODY_HANDLER_STATUS_STATISTIC_TOTAL & StatusStatistic,
		ULONGLONG ClientHandler,
		ULONGLONG ServerHandler
		);

	virtual ~CEnumServerHandlerFileOp();
	//struct IEnumFolderFileOperation
	//{
	//	virtual INT Operate(CONST CString & strFile) = 0;
	virtual INT Operate(CONST CString & strFile);

	CONST BODY_ENUM_FILE_DATA_RSP & get_BODY_ENUM_FILE_DATA_RSP();
	//};

private:

};
class CEnumServerHandler : public IHPTHandler
{
private:
	CONST HPTHeader & m_Header; /***INCOMMING***/
	CONST HPTBody & m_Body; /***INCOMMING***/
	LONG m_RefCount; /***INTERNAL***/
	BODY_BEGIN_ENUM_FILE_REQ m_body_begin_enum_file_req; /***INTERNAL***/
	CComPtr<IMultipleEventHelper> m_spIMultipleEventHelper; /***INTERNAL***//***TO CLEANUP***/

	HPTHeader m_TempHeader; /***INTERNAL***/
	HPTBody m_TempBody; /***INTERNAL***/

	BODY_HANDLER_STATUS_STATISTIC_TOTAL m_StatusStatistic; /***INTERNAL***/


private:
	HRESULT ProcessCommand_CMD_BEGIN_ENUM_FILE_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

	HRESULT ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

public:
	CEnumServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body) :
		m_Header(Header),
		m_Body(Body),
		m_RefCount(0)
	{

	}

	IMP_CLASS_DESTRUCTOR(CEnumServerHandler);

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
	IMP_WHO_AM_I(TEXT("CDownloadHandler:IHPTHandler"));

	virtual HRESULT CreateMember();

	HRESULT DestroyMember();
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
	virtual BOOL IsPeerError() { return FALSE; }
	//};
};

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCEnumServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppIDownloadHandler, BOOL bCreateMember);