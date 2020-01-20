#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "..\PublicHeader\PublicSimpleBitArray.h"

#include "HPTHandler.h"
#include "CommandList.h"
#include "EventHelper.h"

#include "PluginFileFactoryConfig.h"
#include "PluginFileFactoryInterface.h"


class CDownloadServerHandler : public IHPTHandler
{
private:
	CONST HPTHeader & m_Header; /***INCOMMING***/
	CONST HPTBody & m_Body; /***INCOMMING***/
	LONG m_RefCount; /***INTERNAL***/
	BODY_CREATE_DOWNLOAD_SESSION_REQ m_ClientServerFiles; /***INTERNAL***/
	CComPtr<IMultipleEventHelper> m_spIMultipleEventHelper; /***INTERNAL***//***TO CLEANUP***/

	HPTHeader m_TempHeader; /***INTERNAL***/
	HPTBody m_TempBody; /***INTERNAL***/

	BODY_HANDLER_STATUS_STATISTIC_TOTAL m_StatusStatistic; /***INTERNAL***/

	PluginFileFactoryConfigs m_pffConfigs;

private:
	HRESULT ProcessCommand_CMD_BEGIN_TRANSFER_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);
	HRESULT ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

	HRESULT ProcessFile(CONST HPTHeader & Header, CONST HPTBody & Body, LONG FileIndex, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);
	HRESULT ProcessWin32FileHelper(CONST BODY_BEGIN_FILE_RSP  & data_begin_file_rsp, CONST HPTHeader & Header, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);
	HRESULT ProcessPluginFileHelper(CONST BODY_BEGIN_FILE_RSP  & data_begin_file_rsp, CONST HPTHeader & Header, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);
	HRESULT ProcessBuffer(IFileDevice * pFileDevide, LONGLONG currentOffset, LONGLONG currentSize, CONST HPTHeader & Header, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve);

	HRESULT GetPluginFileFactoryConfigs();
public:
	CDownloadServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body);

	IMP_CLASS_DESTRUCTOR(CDownloadServerHandler);

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
//HRESULT CreateInstanceCDownloadHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppIDownloadHandler, BOOL bCreateMember);