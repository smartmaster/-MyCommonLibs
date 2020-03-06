#include "stdafx.h"
#include "RunAppClientHandler.h"


HRESULT CRunAppClientHandler::ProcessCommand_CMD_CREAE_RUN_APP_SESSION_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

HRESULT CRunAppClientHandler::ProcessCommand_CMD_RUN_APP_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	//CMD_RESULT_RSP
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	BODY_RUN_APP_RSP body_run_app_rsp;
	CString strXml = (LPCTSTR)(Body.m_pData);
	body_run_app_rsp.FromString(MethodString(BODY_RUN_APP_RSP), strXml);

	D_INFO(0, TEXT("Remote process ExitCode=%d"), body_run_app_rsp.m_ExitCode);

	//////////////////////////////////////////////////////////////////////////
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

HRESULT CRunAppClientHandler::ProcessCommand_CMD_END_SESSION_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

HRESULT CRunAppClientHandler::ProcessCommand_CMD_RESULT_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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
		m_bPeerError = TRUE;
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

HRESULT CRunAppClientHandler::ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

CRunAppClientHandler::CRunAppClientHandler() :
m_RefCount(0),
m_ServerHandler(0),
m_bPeerError(FALSE)
{

}

HRESULT CRunAppClientHandler::CreateMember()
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

HRESULT CRunAppClientHandler::DestroyMember()
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

HRESULT CRunAppClientHandler::ProcessCommand(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (CMD_CREAE_RUN_APP_SESSION_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_CREAE_RUN_APP_SESSION_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
	}
	else if (CMD_RUN_APP_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_RUN_APP_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
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

DWORD CRunAppClientHandler::WaitAny(DWORD WaitMillSec)
{
	return m_spIMultipleEventHelper->WaitMultiple(FALSE, WaitMillSec);
}

HRESULT CRunAppClientHandler::Set(LONG Index)
{
	return m_spIMultipleEventHelper->Set(Index);
}

ULONGLONG CRunAppClientHandler::GetServerHandler()
{
	return m_ServerHandler;
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CRunAppClientHandler::GetStatusStatistic()
{
	return &m_StatusStatistic;
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CRunAppClientHandler::GetStatusStatisticServer()
{
	return &m_StatusStatisticServer;
}

HRESULT CRunAppClientHandler::Reset(LONG Index)
{
	return m_spIMultipleEventHelper->Reset(Index);
}

HRESULT CRunAppClientHandler::Wait(LONG Index, DWORD MillSec)
{
	return m_spIMultipleEventHelper->Wait(Index, MillSec);
}

DWORD CRunAppClientHandler::ResetAll()
{
	return m_spIMultipleEventHelper->ResetAll();
}

BOOL CRunAppClientHandler::IsPeerError()
{
	return m_bPeerError;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCRunAppClientHandler(IHPTHandler ** ppIHPTHandler, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CRunAppClientHandler(), ppIHPTHandler, bCreateMember);
}