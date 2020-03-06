#include "stdafx.h"
#include "RunAppServerHandler.h"


HRESULT CRunAppServerHandler::ProcessCommand_CMD_RUN_APP_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strXml = (LPCTSTR)(Body.m_pData);
	BODY_RUN_APP_REQ body_run_app_req;
	body_run_app_req.FromString(MethodString(BODY_RUN_APP_REQ), strXml);
	ULONG ExitCode = 0;
	hr = RunApp(
		body_run_app_req.m_Username.GetString(),//LPCTSTR Username, 
		body_run_app_req.m_Domain.GetString(),//LPCTSTR Domain, 
		body_run_app_req.m_Password.GetString(),//LPCTSTR Password, 
		body_run_app_req.m_CommandLine.GetString(),//LPCTSTR CommandLine, 
		body_run_app_req.m_CurDir.GetString(),//LPCTSTR CurDir, 
		body_run_app_req.m_TimeOut,//ULONG TimeOut, 
		body_run_app_req.m_TerminateOnTimeOut,//BOOL TerminateOnTimeOut, 
		ExitCode//ULONG & ExitCode
		);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT("RunApp"));
		D_API_ERR(0, TEXT("RunApp"), hr, TEXT("%s"), body_run_app_req.m_CommandLine.GetString());
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	BODY_RUN_APP_RSP body_run_app_rsp;
	body_run_app_rsp.m_ExitCode = ExitCode;
	strXml.Empty();
	body_run_app_rsp.ToString(MethodString(BODY_RUN_APP_RSP), strXml);
	hr = m_TempBody.Allocate(strXml.GetLength() * sizeof(TCHAR)+sizeof(TCHAR));
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT("Allocate"));
		D_API_ERR(0, TEXT(""), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CopyMemory(m_TempBody.m_pData, strXml.GetString(), m_TempBody.m_DataSize);
	m_TempHeader.Init();
	//m_TempHeader.m_HeaderSize;
	//m_TempHeader.m_HeaderVersion;
	m_TempHeader.m_CommandCode = CMD_RUN_APP_RSP;
	//m_TempHeader.m_FlagOrEnum;
	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
	//m_TempHeader.m_PayloadFileOffset;
	m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
	m_TempHeader.m_ServerHandler = Header.m_ServerHandler;

	hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CRunAppServerHandler::ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	CString strXml;
	m_StatusStatistic.ToString(MethodString(BODY_HANDLER_STATUS_STATISTIC_TOTAL_RSP), strXml);

	HRESULT hr = m_TempBody.Allocate(strXml.GetLength() * sizeof(TCHAR)+sizeof(TCHAR));
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT(""), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	CopyMemory(m_TempBody.m_pData, strXml.GetString(), m_TempBody.m_DataSize);

	m_TempHeader.Init();
	//m_TempHeader.m_HeaderSize;
	//m_TempHeader.m_HeaderVersion;
	m_TempHeader.m_CommandCode = CMD_HANDLER_STATUS_STATISTIC_TOTAL_RSP;
	//m_TempHeader.m_FlagOrEnum;
	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
	//m_TempHeader.m_PayloadFileOffset;
	m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
	m_TempHeader.m_ServerHandler = Header.m_ServerHandler;
	//m_TempHeader.m_Data64;

	hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	return hr;
}

CRunAppServerHandler::CRunAppServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body) :
m_Header(Header),
m_Body(Body),
m_RefCount(0)
{

}

HRESULT CRunAppServerHandler::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ATLASSERT(CMD_CREAE_RUN_APP_SESSION_REQ == m_Header.m_CommandCode);

	//////////////////////////////////////////////////////////////////////////
	hr = CreateInstanceCMultipleEventHelper(NS_MultipleEventHelper::bManualResetArr, _countof(NS_MultipleEventHelper::bManualResetArr), &m_spIMultipleEventHelper, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCMultipleEventHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CONST LONG INITIAL_SIZE = 1024 * 1024 * 2;
	hr = m_TempBody.Allocate(INITIAL_SIZE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("Size=%d"), INITIAL_SIZE);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CRunAppServerHandler::ProcessCommand(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (CMD_RUN_APP_REQ == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_RUN_APP_REQ(Header, Body, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			//m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("ProcessCommand_CMD_RUN_APP_REQ"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}
	else if (CMD_HANDLER_STATUS_STATISTIC_TOTAL_REQ == Header.m_CommandCode) //<sonmi01>2014-2-9 ###???
	{
		hr = ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_REQ(Header, Body, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			//m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_REQ"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}
	else
	{
		hr = E_INVALIDARG;
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("UnrecogneizedCommand"), hr, TEXT("CommandCode=%08x"), Header.m_CommandCode);
		//LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	if (FAILED(hr))
	{
		BODY_RESULT_RSP data_result_rsp;
		data_result_rsp.m_ErrorCode = hr;
		CString strXMl;
		data_result_rsp.ToString(MethodString(BODY_RESULT_RSP), strXMl);
		hr = m_TempBody.Allocate(strXMl.GetLength() * sizeof(TCHAR)+sizeof(TCHAR));
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
		CopyMemory(m_TempBody.m_pData, strXMl.GetString(), m_TempBody.m_DataSize);

		m_TempHeader.Init();
		m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
		m_TempHeader.m_ServerHandler = Header.m_ServerHandler;
		m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
		m_TempHeader.m_CommandCode = CMD_RESULT_RSP;

		hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}

	}

	return hr;
}

HRESULT CRunAppServerHandler::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_TempBody.Free();
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

DWORD CRunAppServerHandler::WaitAny(DWORD WaitMillSec)
{
	return m_spIMultipleEventHelper->WaitMultiple(FALSE, WaitMillSec);
}

HRESULT CRunAppServerHandler::Set(LONG Index)
{
	return m_spIMultipleEventHelper->Set(Index);
}

ULONGLONG CRunAppServerHandler::GetServerHandler()
{
	return NULL;
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CRunAppServerHandler::GetStatusStatistic()
{
	return &m_StatusStatistic;
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CRunAppServerHandler::GetStatusStatisticServer()
{
	return NULL;
}

HRESULT CRunAppServerHandler::Reset(LONG Index)
{
	return m_spIMultipleEventHelper->Reset(Index);
}

HRESULT CRunAppServerHandler::Wait(LONG Index, DWORD MillSec)
{
	return m_spIMultipleEventHelper->Wait(Index, MillSec);
}

DWORD CRunAppServerHandler::ResetAll()
{
	return m_spIMultipleEventHelper->ResetAll();
}



HRESULT CreateInstanceCRunAppServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppIDownloadHandler, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CRunAppServerHandler(Header, Body), ppIDownloadHandler, bCreateMember);
}
