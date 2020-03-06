#include "stdafx.h"
#include "EnumServerHandler.h"


CEnumServerHandlerFileOp::CEnumServerHandlerFileOp(CONST BODY_BEGIN_ENUM_FILE_REQ & body_begin_enum_file_req, CCircularBuffer * pCircBufferResult, BODY_HANDLER_STATUS_STATISTIC_TOTAL & StatusStatistic, ULONGLONG ClientHandler, ULONGLONG ServerHandler) :
m_body_begin_enum_file_req(body_begin_enum_file_req),
m_pCircBufferResult(pCircBufferResult),
m_StatusStatistic(StatusStatistic),
m_ClientHandler(ClientHandler),
m_ServerHandler(ServerHandler)
{
	m_body_enum_file_data_rsp.m_Files.reserve(S_RESERVE_COUNT);
}

CEnumServerHandlerFileOp::~CEnumServerHandlerFileOp()
{
	m_TempBody.Free();
	m_body_enum_file_data_rsp.m_Files.clear();
}

INT CEnumServerHandlerFileOp::Operate(CONST CString & strFile)
{
	HRESULT hr = S_OK;

	//////////////////////////////////////////////////////////////////////////
	BODY_FILE_DESCRITION body_file_descrition;
	if (m_body_begin_enum_file_req.m_NotFilePattern.GetLength())
	{
		LPCTSTR pszFileName = strFile.GetString();
		INT index = strFile.ReverseFind(TEXT('\\'));
		if (-1 != index)
		{
			pszFileName += (index + 1);
		}
		if (!CMiscHelper::MatchPattern(pszFileName, m_body_begin_enum_file_req.m_NotFilePattern.GetString()))
		{
			body_file_descrition.m_FileName = strFile;
			m_body_enum_file_data_rsp.m_Files.push_back(body_file_descrition);
		}
	}
	else
	{
		body_file_descrition.m_FileName = strFile;
		m_body_enum_file_data_rsp.m_Files.push_back(body_file_descrition);
	}

	//////////////////////////////////////////////////////////////////////////
	if (m_body_enum_file_data_rsp.m_Files.size() >= S_RESERVE_COUNT)
	{
		CString strXml;
		m_body_enum_file_data_rsp.ToString(MethodString(BODY_ENUM_FILE_DATA_RSP), strXml);
		hr = m_TempBody.Allocate(strXml.GetLength() * sizeof(TCHAR)+sizeof(TCHAR));
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT(""), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
		CopyMemory(m_TempBody.m_pData, strXml.GetString(), m_TempBody.m_DataSize);

		m_TempHeader.Init();
		//m_TempHeader.m_HeaderSize = vvvv;
		//m_TempHeader.m_HeaderVersion = vvvv;
		m_TempHeader.m_CommandCode = CMD_ENUM_FILE_DATA_RSP;
		//m_TempHeader.m_Flags = vvvv;
		//m_TempHeader.m_Enum = vvvv;
		//m_TempHeader.m_MoreData = vvvv;
		m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
		//m_TempHeader.m_PayloadFileOffset = vvvv;
		m_TempHeader.m_ClientHandler = m_ClientHandler;
		m_TempHeader.m_ServerHandler = m_ServerHandler;

		hr = PutCommmandHelper(m_TempHeader, m_TempBody, m_pCircBufferResult);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}

		m_body_enum_file_data_rsp.m_Files.resize(0);
		m_body_enum_file_data_rsp.m_Files.reserve(S_RESERVE_COUNT);
	}

	return hr;
}

CONST BODY_ENUM_FILE_DATA_RSP & CEnumServerHandlerFileOp::get_BODY_ENUM_FILE_DATA_RSP()
{
	return m_body_enum_file_data_rsp;
}

HRESULT CEnumServerHandler::ProcessCommand_CMD_BEGIN_ENUM_FILE_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	D_INFO(0, TEXT("ProcessCommand CMD_BEGIN_ENUM_FILE_REQ %08x"), Header.m_CommandCode);

	CString strXML = (LPTSTR)(m_Body.m_pData);
	m_body_begin_enum_file_req.FromString(MethodString(BODY_BEGIN_ENUM_FILE_REQ), strXML);

	//////////////////////////////////////////////////////////////////////////
	CEnumServerHandlerFileOp EnumServerHandlerFileOp(
		m_body_begin_enum_file_req,//CONST BODY_BEGIN_ENUM_FILE_REQ & body_begin_enum_file_req,
		pCircBufferResult,//CCircularBuffer * pCircBufferResult,
		m_StatusStatistic,//BODY_HANDLER_STATUS_STATISTIC_TOTAL & StatusStatistic,
		Header.m_ClientHandler,//ULONGLONG ClientHandler,
		Header.m_ServerHandler//ULONGLONG ServerHandler
		);
	if (m_body_begin_enum_file_req.m_bRecursively)
	{
		hr = EnumDirectoryFileTree(
			m_body_begin_enum_file_req.m_Directory.GetString(),//LPCTSTR szDirectory, 
			m_body_begin_enum_file_req.m_FilePattern.GetString(),//LPCTSTR szFileSpec, 
			NULL,//IEnumFolderFileOperation * pFolderOperation, 
			&EnumServerHandlerFileOp//IEnumFolderFileOperation * pFileOperation
			);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("EnumDirectoryFileTree"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}
	else
	{
		hr = EnumDirectoryFileFlat(
			m_body_begin_enum_file_req.m_Directory.GetString(),//LPCTSTR szDirectory, 
			m_body_begin_enum_file_req.m_FilePattern.GetString(),//LPCTSTR szFileSpec, 
			&EnumServerHandlerFileOp//IEnumFolderFileOperation * pFileOperation
			);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("EnumDirectoryFileTree"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CONST BODY_ENUM_FILE_DATA_RSP & body_enum_file_data_rsp = EnumServerHandlerFileOp.get_BODY_ENUM_FILE_DATA_RSP();
	if (body_enum_file_data_rsp.m_Files.size()) //send remaining enumerated files
	{
		strXML.Empty();
		body_enum_file_data_rsp.ToString(MethodString(BODY_ENUM_FILE_DATA_RSP), strXML);
		hr = m_TempBody.Allocate(strXML.GetLength() * sizeof(TCHAR)+sizeof(TCHAR));
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT(""), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
		CopyMemory(m_TempBody.m_pData, strXML.GetString(), m_TempBody.m_DataSize);

		m_TempHeader.Init();
		//m_TempHeader.m_HeaderSize = vvvv;
		//m_TempHeader.m_HeaderVersion = vvvv;
		m_TempHeader.m_CommandCode = CMD_ENUM_FILE_DATA_RSP;
		//m_TempHeader.m_Flags = vvvv;
		//m_TempHeader.m_Enum = vvvv;
		//m_TempHeader.m_MoreData = vvvv;
		m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
		//m_TempHeader.m_PayloadFileOffset = vvvv;
		m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
		m_TempHeader.m_ServerHandler = Header.m_ServerHandler;

		hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	m_TempBody.Allocate(0);

	m_TempHeader.Init();
	m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
	m_TempHeader.m_ServerHandler = Header.m_ServerHandler;
	m_TempHeader.m_CommandCode = CMD_END_ENUM_FILE_RSP;

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

HRESULT CEnumServerHandler::ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

HRESULT CEnumServerHandler::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ATLASSERT(CMD_CREATE_ENUM_FILE_SESSION_REQ == m_Header.m_CommandCode);

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

HRESULT CEnumServerHandler::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_spIMultipleEventHelper.Release();
	m_TempBody.Free();
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CEnumServerHandler::ProcessCommand(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (CMD_BEGIN_ENUM_FILE_REQ == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_BEGIN_ENUM_FILE_REQ(Header, Body, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			//m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("ProcessCommand_CMD_BEGIN_ENUM_FILE_REQ"), hr, TEXT(""));
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

DWORD CEnumServerHandler::WaitAny(DWORD WaitMillSec)
{
	return m_spIMultipleEventHelper->WaitMultiple(FALSE, WaitMillSec);
}

DWORD CEnumServerHandler::ResetAll()
{
	return m_spIMultipleEventHelper->ResetAll();
}

HRESULT CEnumServerHandler::Set(LONG Index)
{
	return m_spIMultipleEventHelper->Set(Index);
}

HRESULT CEnumServerHandler::Reset(LONG Index)
{
	return m_spIMultipleEventHelper->Reset(Index);
}

HRESULT CEnumServerHandler::Wait(LONG Index, DWORD MillSec)
{
	return m_spIMultipleEventHelper->Wait(Index, MillSec);
}

ULONGLONG CEnumServerHandler::GetServerHandler()
{
	return NULL;
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CEnumServerHandler::GetStatusStatistic()
{
	return &m_StatusStatistic;
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CEnumServerHandler::GetStatusStatisticServer()
{
	return NULL;
}

HRESULT CreateInstanceCEnumServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppIDownloadHandler, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CEnumServerHandler(Header, Body), ppIDownloadHandler, bCreateMember);
}
