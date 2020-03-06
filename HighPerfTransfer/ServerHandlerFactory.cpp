#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "HPTSendReceiverInterface.h"
#include "ServerHandlerFactory.h"
#include "CommandList.h"
#include "HPTHelper.h"

CHPTServerHandlerFactory::CHPTServerHandlerFactory() :
m_RefCount(0)
{

}

HRESULT CHPTServerHandlerFactory::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST LONG __2M = 1024 * 1024 * 2;
	hr = m_TempBody.Allocate(__2M);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("Size=%d"), __2M);
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT CHPTServerHandlerFactory::DestroyMember()
{
	return S_OK;
}

HRESULT CHPTServerHandlerFactory::CreateServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppNewIHPTHandler, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (CMD_CREAE_DOWNLOAD_SESSION_REQ == Header.m_CommandCode)
	{
		D_INFO(0, TEXT("Received CMD_CREAE_DOWNLOAD_SESSION_REQ"));
		hr = CreateInstanceCDownloadServerHandler(Header, Body, ppNewIHPTHandler, TRUE);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT("CreateInstanceCDownloadServerHandler"));
			D_API_ERR(0, TEXT(""), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		else
		{
			m_TempBody.Allocate(0);

			m_TempHeader.Init();
			m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
			m_TempHeader.m_ServerHandler = PtrToUll<IHPTHandler>(*ppNewIHPTHandler);
			m_TempHeader.m_CommandCode = CMD_CREATE_DOWNLOAD_SESSION_RSP;

			hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
			if (FAILED(hr))
			{
				m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
				D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
				//LEAVE_BLOCK(0);
			}
		}
	}
	else if (CMD_CREATE_UPLOAD_SESSION_REQ == Header.m_CommandCode)
	{
		D_INFO(0, TEXT("Received CMD_CREATE_UPLOAD_SESSION_REQ"));
		hr = CreateInstanceCUploadServerHandler(Header, Body, ppNewIHPTHandler, TRUE);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT(""), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		else
		{
			m_TempBody.Allocate(0);

			m_TempHeader.Init();
			m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
			m_TempHeader.m_ServerHandler = PtrToUll<IHPTHandler>(*ppNewIHPTHandler);
			m_TempHeader.m_CommandCode = CMD_CREATE_UPLOAD_SESSION_RSP;

			hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
			if (FAILED(hr))
			{
				m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
				D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
				//LEAVE_BLOCK(0);
			}
		}
	}
	else if (CMD_CREAE_RUN_APP_SESSION_REQ == Header.m_CommandCode)
	{
		D_INFO(0, TEXT("Received CMD_CREAE_RUN_APP_SESSION_REQ"));
		hr = CreateInstanceCRunAppServerHandler(Header, Body, ppNewIHPTHandler, TRUE);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT(""), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		else
		{
			m_TempBody.Allocate(0);

			m_TempHeader.Init();
			m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
			m_TempHeader.m_ServerHandler = PtrToUll<IHPTHandler>(*ppNewIHPTHandler);
			m_TempHeader.m_CommandCode = CMD_CREAE_RUN_APP_SESSION_RSP;

			hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
			if (FAILED(hr))
			{
				m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
				D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
				//LEAVE_BLOCK(0);
			}
		}
	}
	else if (CMD_CREATE_ENUM_FILE_SESSION_REQ == Header.m_CommandCode)
	{
		D_INFO(0, TEXT("Received CMD_CREATE_ENUM_FILE_SESSION_REQ"));
		hr = CreateInstanceCEnumServerHandler(Header, Body, ppNewIHPTHandler, TRUE);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT(""), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		else
		{
			m_TempBody.Allocate(0);

			m_TempHeader.Init();
			m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
			m_TempHeader.m_ServerHandler = PtrToUll<IHPTHandler>(*ppNewIHPTHandler);
			m_TempHeader.m_CommandCode = CMD_CREATE_ENUM_FILE_SESSION_RSP;

			hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
			if (FAILED(hr))
			{
				m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
				D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
				//LEAVE_BLOCK(0);
			}
		}
	}
	else if (CMD_END_SESSION_REQ == Header.m_CommandCode)
	{
		D_INFO(0, TEXT("Received CMD_END_SESSION_REQ"));
		ATLASSERT(NULL == Header.m_ServerHandler);

		m_TempBody.Allocate(0);

		m_TempHeader.Init();
		//m_TempHeader.m_HeaderSize;
		//m_TempHeader.m_HeaderVersion;
		m_TempHeader.m_CommandCode = CMD_END_SESSION_RSP;
		//m_TempHeader.m_FlagOrEnum;
		//m_TempHeader.m_PayloadLength;
		//m_TempHeader.m_PayloadFileOffset;
		m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
		m_TempHeader.m_ServerHandler = NULL;
		//m_TempHeader.m_Data64;

		hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}
	else
	{
		hr = E_INVALIDARG;
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("UnrecogneizedCommand"), hr, TEXT("CommandCode=%08x"), Header.m_CommandCode);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

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
		m_TempHeader.m_ServerHandler = NULL;
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

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CHPTServerHandlerFactory::GetStatusStatistic()
{
	return &m_StatusStatistic;
}

HRESULT CreateInstanceCHPTServerHandlerFactory(IHPTServerHandlerFactory ** ppIHPTServerHandlerFactory, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CHPTServerHandlerFactory(), ppIHPTServerHandlerFactory, bCreateMember);
}
