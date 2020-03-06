#include "stdafx.h"
#include "UploadClientHandler.h"




HRESULT CUploadClientHandler::ProcessCommand_CMD_CREATE_UPLOAD_SESSION_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_ServerHandler = Header.m_ServerHandler;

	if (0 == m_ServerHandler)
	{
		hr = E_POINTER;
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("ServerHandler"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::SYNC_INDEX);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT(""), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CUploadClientHandler::ProcessCommand_CMD_END_UPLOAD_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::SYNC_INDEX);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("IMultipleEventHelper->Set"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}
	return hr;
}

HRESULT CUploadClientHandler::ProcessCommand_CMD_END_SESSION_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::SYNC_INDEX);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("IMultipleEventHelper->Set"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}
	return hr;
}

HRESULT CUploadClientHandler::ProcessCommand_CMD_RESULT_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	//CMD_RESULT_RSP
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	BODY_RESULT_RSP data_result_rsp;
	CString strXml = (LPCTSTR)(Body.m_pData);
	data_result_rsp.FromString(MethodString(BODY_RESULT_RSP), strXml);

	if (FAILED(data_result_rsp.m_ErrorCode))
	{
		m_bPeerError = TRUE; //<sonmi01>2015-3-12 ###???
		D_INFO(0, TEXT("PeerError=0x%08x"), data_result_rsp.m_ErrorCode);
		m_StatusStatistic.m_Error.SetLastStatus(data_result_rsp.m_ErrorCode, TEXT(""));
		hr = m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::ERROR_INDEX);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("IMultipleEventHelper->Set"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

CUploadClientHandler::CUploadClientHandler() :
m_RefCount(0),
m_ServerHandler(0),
m_bPeerError(FALSE)
{

}

HRESULT CUploadClientHandler::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = CreateInstanceCMultipleEventHelper(NS_MultipleEventHelper::bManualResetArr, _countof(NS_MultipleEventHelper::bManualResetArr), &m_spIMultipleEventHelper, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCMultipleEventHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CUploadClientHandler::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_spIMultipleEventHelper.Release();

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

DWORD CUploadClientHandler::WaitAny(DWORD WaitMillSec)
{
	return m_spIMultipleEventHelper->WaitMultiple(FALSE, WaitMillSec);
}

HRESULT CUploadClientHandler::Set(LONG Index)
{
	return m_spIMultipleEventHelper->Set(Index);
}

ULONGLONG CUploadClientHandler::GetServerHandler()
{
	return m_ServerHandler;
}

HRESULT CUploadClientHandler::ProcessCommand(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (CMD_CREATE_UPLOAD_SESSION_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_CREATE_UPLOAD_SESSION_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
	}
	else if (CMD_END_UPLOAD_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_END_UPLOAD_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
	}
	else if (CMD_END_SESSION_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_END_SESSION_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
	}
	else if (CMD_RESULT_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_RESULT_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
	}
	else if (CMD_HANDLER_STATUS_STATISTIC_TOTAL_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
	}
	else
	{
		hr = E_INVALIDARG;
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT(""), hr, TEXT("Unrecognized command code [%d]"), Header.m_CommandCode);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	if (FAILED(hr))
	{
		hr = m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::ERROR_INDEX);
		D_API_ERR(0, TEXT("CDownloadClientHandler::ProcessCommand"), hr, TEXT("client handler error"));
		//LEAVE_BLOCK(0);
	}

	return hr;
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CUploadClientHandler::GetStatusStatistic()
{
	return &m_StatusStatistic;
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CUploadClientHandler::GetStatusStatisticServer()
{
	return &m_StatusStatisticServer;
}

HRESULT CUploadClientHandler::ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	CString strXml = (LPCTSTR)(Body.m_pData);
	m_StatusStatisticServer.FromString(MethodString(BODY_HANDLER_STATUS_STATISTIC_TOTAL_RSP), strXml);

	HRESULT hr = m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::SYNC_INDEX);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("IMultipleEventHelper->Set"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}
	return hr;
}

HRESULT CUploadClientHandler::Reset(LONG Index)
{
	return m_spIMultipleEventHelper->Reset(Index);
}

HRESULT CUploadClientHandler::Wait(LONG Index, DWORD MillSec)
{
	return m_spIMultipleEventHelper->Wait(Index, MillSec);
}

DWORD CUploadClientHandler::ResetAll()
{
	return m_spIMultipleEventHelper->ResetAll();
}

//<sonmi01>2015-3-12 ###???
BOOL CUploadClientHandler::IsPeerError()
{
	return m_bPeerError;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCUploadClientHandler(IHPTHandler ** ppIHPTHandler, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CUploadClientHandler(), ppIHPTHandler, bCreateMember);
}
