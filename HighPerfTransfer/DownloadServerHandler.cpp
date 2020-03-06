#include "stdafx.h"

#include "..\PublicHeader\PublicStringValueConvert.h"
#include "..\PublicHeader\PublicMiscHelper.h"

#include "DownloadServerHandler.h"
#include "HPTSendReceiverInterface.h"
#include "CreateWin32FileHelper.h"
#include "CommandList.h"

CDownloadServerHandler::CDownloadServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body) :
m_Header(Header),
m_Body(Body),
m_RefCount(0)
{

}

HRESULT CDownloadServerHandler::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = GetPluginFileFactoryConfigs();
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetPluginFileFactoryConfigs"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	ATLASSERT(CMD_CREAE_DOWNLOAD_SESSION_REQ == m_Header.m_CommandCode);

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

HRESULT CDownloadServerHandler::DestroyMember()
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

HRESULT CDownloadServerHandler::ProcessCommand(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (CMD_BEGIN_TRANSFER_REQ == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_BEGIN_TRANSFER_REQ(Header, Body, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			//m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("ProcessCommand_CMD_BEGIN_TRANSFER_REQ"), hr, TEXT(""));
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
		hr = m_TempBody.Allocate(strXMl.GetLength() * sizeof(TCHAR) + sizeof(TCHAR));
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

//HRESULT CDownloadServerHandler::SetCurrentCommandFinished()
//{
//	return m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::SYNC_INDEX);
//}

DWORD CDownloadServerHandler::WaitAny(DWORD WaitMillSec)
{
	return m_spIMultipleEventHelper->WaitMultiple(FALSE, WaitMillSec);
}

HRESULT CDownloadServerHandler::ProcessCommand_CMD_BEGIN_TRANSFER_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	D_INFO(0, TEXT("ProcessCommand CMD_BEGIN_TRANSFER_REQ %08x"), Header.m_CommandCode);

	CString strXML = (LPTSTR)(m_Body.m_pData);
	m_ClientServerFiles.m_CilentFiles.resize(0);
	m_ClientServerFiles.m_ServerFiles.resize(0);
	m_ClientServerFiles.FromString(MethodString(BODY_BEGIN_TRANSFER_REQ), strXML);
	ATLASSERT(m_ClientServerFiles.m_CilentFiles.size() == m_ClientServerFiles.m_ServerFiles.size());

	//////////////////////////////////////////////////////////////////////////
	for (size_t ii = 0; ii < m_ClientServerFiles.m_ServerFiles.size(); ++ii)
	{
		hr = ProcessFile(Header, Body, ii, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ProcessWin32File"), hr, TEXT("%s"), m_ClientServerFiles.m_ServerFiles[ii].m_FileName.GetString());
			break;
		}
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	m_TempBody.Allocate(0);

	m_TempHeader.Init();
	m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
	m_TempHeader.m_ServerHandler = Header.m_ServerHandler;
	m_TempHeader.m_CommandCode = CMD_END_TRANSFER_RSP;

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

HRESULT CDownloadServerHandler::ProcessFile(CONST HPTHeader & Header, CONST HPTBody & Body, LONG FileIndex, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strXml;
	BODY_BEGIN_FILE_RSP data_begin_file_rsp;
	data_begin_file_rsp.m_CilentFile = m_ClientServerFiles.m_CilentFiles[FileIndex];
	data_begin_file_rsp.m_ServerFile = m_ClientServerFiles.m_ServerFiles[FileIndex];
	data_begin_file_rsp.ToString(MethodString(BODY_BEGIN_FILE_RSP), strXml);
	hr = m_TempBody.Allocate(strXml.GetLength() * sizeof(TCHAR)+sizeof(TCHAR));
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT("Allocate"));
		LEAVE_BLOCK(0);
	}
	CopyMemory(m_TempBody.m_pData, strXml.GetString(), m_TempBody.m_DataSize);

	m_TempHeader.Init();
	m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
	m_TempHeader.m_ServerHandler = Header.m_ServerHandler;
	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
	m_TempHeader.m_CommandCode = CMD_BEGIN_FILE_RSP;

	D_INFO(0, TEXT("Begin file [%s, %s]"), data_begin_file_rsp.m_CilentFile.m_FileName.GetString(), data_begin_file_rsp.m_ServerFile.m_FileName.GetString());
	D_INFO(0, TEXT("Begin file [%s, %s]"), data_begin_file_rsp.m_CilentFile.m_MoreContext.GetString(), data_begin_file_rsp.m_ServerFile.m_MoreContext.GetString());

	hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT("PutCommmandHelper"));
		D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

#if 1
	if (0 == data_begin_file_rsp.m_ServerFile.m_FileType)
	{
		hr = ProcessWin32FileHelper(data_begin_file_rsp, Header, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("ProcessWin32FileHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		hr = ProcessPluginFileHelper(data_begin_file_rsp, Header, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("ProcessPluginFileHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
#else
//	//////////////////////////////////////////////////////////////////////////
//	CComPtr<IFileDevice> spFileDevide;
//	hr = CreateWin32FileHelper(
//		data_begin_file_rsp.m_ServerFile,//BODY_FILE_DESCRITION & FileDesc, 
//		GENERIC_READ,//IN ULONG ulDesiredAccess, 
//		FILE_SHARE_READ,//IN ULONG ulShareMode, 
//		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
//		0,//FILE_FLAG_NO_BUFFERING,//IN ULONG ulFlagsAndAttributes, 
//		&spFileDevide//IFileDevice ** ppIFileDevice
//		);
//	if (FAILED(hr))
//	{
//		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
//		D_API_ERR(0, TEXT("CreateWin32FileHelper"), hr, TEXT("%s"), data_begin_file_rsp.m_ServerFile.m_FileName.GetString());
//		LEAVE_BLOCK(0);
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	LONGLONG FileSize = 0;
//	hr = spFileDevide->GetFileSize(&FileSize);
//	if (FAILED(hr))
//	{
//		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
//		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
//		LEAVE_BLOCK(0);
//	}
//
//#if 1
//	hr = ProcessBuffer(spFileDevide, 0LL, FileSize, Header, pCircBufferResult);
//	if (FAILED(hr))
//	{
//		//LastError = WSAGetLastError();
//		//LastError = GetLastError();
//		//hr = HRESULT_FROM_WIN32(LastError);
//		//D_SET_FISRT_STATUS(hr, LastError);
//		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
//		//D_SET_LAST_STATUS(hr, LastError);
//		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
//		D_API_ERR(0, TEXT("ProcessBuffer"), hr, TEXT(""));
//		LEAVE_BLOCK(0);
//	}
//#else
//	//CONST LONG __2M = 1024 * 1024 * 2;
//	//LONGLONG Offset = 0;
//	//LONG ToReadLength = 0;
//	//ULONG ReadLength = 0;
//	//while (FileSize)
//	//{
//	//	//////////////////////////////////////////////////////////////////////////
//	//	if (FileSize >= __2M)
//	//	{
//	//		ToReadLength = __2M;
//	//	}
//	//	else
//	//	{
//	//		ToReadLength = FileSize;
//	//	}
//
//	//	//////////////////////////////////////////////////////////////////////////
//	//	hr = m_TempBody.Allocate(ToReadLength);
//	//	if (FAILED(hr))
//	//	{
//	//		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
//	//		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
//	//		LEAVE_BLOCK(0);
//	//	}
//
//	//	//////////////////////////////////////////////////////////////////////////
//	//	ReadLength = 0;
//	//	hr = ReadAtOffsetHelper(spFileDevide, Offset, ToReadLength, &ReadLength, m_TempBody.m_pData);
//	//	if (FAILED(hr))
//	//	{
//	//		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
//	//		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
//	//		LEAVE_BLOCK(0);
//	//	}
//
//	//	//////////////////////////////////////////////////////////////////////////
//	//	m_TempBody.m_DataSize = ReadLength;
//
//	//	m_TempHeader.Init();
//	//	m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
//	//	m_TempHeader.m_ServerHandler = Header.m_ServerHandler;
//	//	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
//	//	m_TempHeader.m_PayloadFileOffset = Offset;
//	//	m_TempHeader.m_CommandCode = CMD_FILE_DATA_RSP;
//
//	//	hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
//	//	if (FAILED(hr))
//	//	{
//	//		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
//	//		D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
//	//		LEAVE_BLOCK(0);
//	//	}
//
//	//	//////////////////////////////////////////////////////////////////////////
//	//	Offset += ReadLength;
//	//	FileSize -= ReadLength;
//	//}
//	//if (FAILED(hr))
//	//{
//	//	D_API_ERR(0, TEXT("ReadLoop"), hr, TEXT(""));
//	//	LEAVE_BLOCK(0);
//	//}
//#endif
#endif
	

	//////////////////////////////////////////////////////////////////////////
	BODY_END_FILE_RSP & data_end_file_rsp = data_begin_file_rsp; //BODY_END_FILE_RSP and BODY_BEGIN_FILE_RSP are same
	strXml.Empty();
	data_end_file_rsp.ToString(MethodString(BODY_END_FILE_RSP), strXml);
	hr = m_TempBody.Allocate(strXml.GetLength() * sizeof(TCHAR)+sizeof(TCHAR));
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CopyMemory(m_TempBody.m_pData, strXml.GetString(), m_TempBody.m_DataSize);

	m_TempHeader.Init();
	m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
	m_TempHeader.m_ServerHandler = Header.m_ServerHandler;
	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
	m_TempHeader.m_CommandCode = CMD_END_FILE_RSP;

	D_INFO(0, TEXT("End file [%s, %s]"), data_end_file_rsp.m_CilentFile.m_FileName.GetString(), data_end_file_rsp.m_ServerFile.m_FileName.GetString());
	D_INFO(0, TEXT("End file [%s, %s]"), data_end_file_rsp.m_CilentFile.m_MoreContext.GetString(), data_end_file_rsp.m_ServerFile.m_MoreContext.GetString());

	hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

ULONGLONG CDownloadServerHandler::GetServerHandler()
{
	return NULL;
}

HRESULT CDownloadServerHandler::Set(LONG Index)
{
	return m_spIMultipleEventHelper->Set(Index);
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CDownloadServerHandler::GetStatusStatistic()
{
	return &m_StatusStatistic;
}

HRESULT CDownloadServerHandler::ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CDownloadServerHandler::GetStatusStatisticServer()
{
	return NULL;
}

HRESULT CDownloadServerHandler::Reset(LONG Index)
{
	return m_spIMultipleEventHelper->Reset(Index);
}

HRESULT CDownloadServerHandler::Wait(LONG Index, DWORD MillSec)
{
	return m_spIMultipleEventHelper->Wait(Index, MillSec);
}

DWORD CDownloadServerHandler::ResetAll()
{
	return m_spIMultipleEventHelper->ResetAll();
}

HRESULT CDownloadServerHandler::GetPluginFileFactoryConfigs()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST LONG BUFFER_SIZE = 1024;
	CString strCOnfig;
	LPTSTR pBuffer = strCOnfig.GetBuffer(BUFFER_SIZE);
	DWORD dwGMF = GetModuleFileName(
		nullptr,//_In_opt_  HMODULE hModule,
		pBuffer,//_Out_     LPTSTR lpFilename,
		BUFFER_SIZE//_In_      DWORD nSize
		);
	strCOnfig.ReleaseBuffer();
	if (0 == dwGMF)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetModuleFileName"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	static CONST TCHAR CONFIG_FILE_NAME[] = TEXT(".PluginFileFactoryConfigs.xml");
	strCOnfig += CONFIG_FILE_NAME;

	static CONST TCHAR ROOT_NAME[] = FIELD_NAME(PluginFileFactoryConfigs);
	HRESULT hrTemp = m_pffConfigs.FromFile(ROOT_NAME, strCOnfig.GetString());
	if (FAILED(hrTemp))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("FromFile"), hr, TEXT("%s"), strCOnfig.GetString());
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CDownloadServerHandler::ProcessBuffer(IFileDevice * pFileDevide, LONGLONG currentOffset, LONGLONG currentSize, CONST HPTHeader & Header, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST LONG __2M = 1024 * 1024 * 2;
	LONG ToReadLength = 0;
	ULONG ReadLength = 0;
	HPTHeader headerpeeked = { 0 };
	LONG peekedLen = 0;
	while (currentSize)
	{
		//////////////////////////////////////////////////////////////////////////
		peekedLen = 0; //<sonmi01>2015-3-12 ###???
		hr = pCircBufferRecieve->PeekNoLocked((BYTE*)&headerpeeked, sizeof(HPTHeader), peekedLen);
		if (SUCCEEDED(hr) &&
			peekedLen == sizeof(HPTHeader) &&
			Header.m_HeaderVersion == MAKEWORD(0, 0) &&
			Header.m_HeaderSize == sizeof(HPTHeader) &&
			Header.m_CommandCode == CMD_END_SESSION_REQ
			)
		{
			hr = HRESULT_FROM_WIN32(ERROR_CANCELLED);
			D_API_ERR(0, TEXT("CMD_END_SESSION_REQ"), hr, TEXT("client request CMD_END_SESSION_REQ"));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		if (currentSize < 0) //endless file
		{
			ToReadLength = __2M;
		}
		else if (currentSize >= __2M)
		{
			ToReadLength = __2M;
		}
		else
		{
			ToReadLength = currentSize;
		}

		//////////////////////////////////////////////////////////////////////////
		hr = m_TempBody.Allocate(ToReadLength);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		ReadLength = 0;
		hr = ReadAtOffsetHelper(pFileDevide, currentOffset, ToReadLength, &ReadLength, m_TempBody.m_pData);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		if (0 == ReadLength)
		{
			D_INFO(0, TEXT("No data read, assume file end. currentSize=%I64d"), currentSize);
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		m_TempBody.m_DataSize = ReadLength;

		m_TempHeader.Init();
		m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
		m_TempHeader.m_ServerHandler = Header.m_ServerHandler;
		m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
		m_TempHeader.m_PayloadFileOffset = currentOffset;
		m_TempHeader.m_CommandCode = CMD_FILE_DATA_RSP;

		hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		currentOffset += ReadLength;
		if (currentSize > 0)
		{
			currentSize -= ReadLength;
		}
	}
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadLoop"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CDownloadServerHandler::ProcessWin32FileHelper(CONST BODY_BEGIN_FILE_RSP & data_begin_file_rsp, CONST HPTHeader & Header, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spFileDevide;
	hr = CreateWin32FileHelper(
		data_begin_file_rsp.m_ServerFile,//BODY_FILE_DESCRITION & FileDesc, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//FILE_FLAG_NO_BUFFERING,//IN ULONG ulFlagsAndAttributes, 
		&spFileDevide//IFileDevice ** ppIFileDevice
		);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("CreateWin32FileHelper"), hr, TEXT("%s"), data_begin_file_rsp.m_ServerFile.m_FileName.GetString());
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG FileSize = 0;
	hr = spFileDevide->GetFileSize(&FileSize);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

#if 1
	hr = ProcessBuffer(spFileDevide, 0LL, FileSize, Header, pCircBufferResult, pCircBufferRecieve);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ProcessBuffer"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
#else
	//CONST LONG __2M = 1024 * 1024 * 2;
	//LONGLONG Offset = 0;
	//LONG ToReadLength = 0;
	//ULONG ReadLength = 0;
	//while (FileSize)
	//{
	//	//////////////////////////////////////////////////////////////////////////
	//	if (FileSize >= __2M)
	//	{
	//		ToReadLength = __2M;
	//	}
	//	else
	//	{
	//		ToReadLength = FileSize;
	//	}

	//	//////////////////////////////////////////////////////////////////////////
	//	hr = m_TempBody.Allocate(ToReadLength);
	//	if (FAILED(hr))
	//	{
	//		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
	//		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
	//		LEAVE_BLOCK(0);
	//	}

	//	//////////////////////////////////////////////////////////////////////////
	//	ReadLength = 0;
	//	hr = ReadAtOffsetHelper(spFileDevide, Offset, ToReadLength, &ReadLength, m_TempBody.m_pData);
	//	if (FAILED(hr))
	//	{
	//		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
	//		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
	//		LEAVE_BLOCK(0);
	//	}

	//	//////////////////////////////////////////////////////////////////////////
	//	m_TempBody.m_DataSize = ReadLength;

	//	m_TempHeader.Init();
	//	m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
	//	m_TempHeader.m_ServerHandler = Header.m_ServerHandler;
	//	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
	//	m_TempHeader.m_PayloadFileOffset = Offset;
	//	m_TempHeader.m_CommandCode = CMD_FILE_DATA_RSP;

	//	hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
	//	if (FAILED(hr))
	//	{
	//		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
	//		D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
	//		LEAVE_BLOCK(0);
	//	}

	//	//////////////////////////////////////////////////////////////////////////
	//	Offset += ReadLength;
	//	FileSize -= ReadLength;
	//}
	//if (FAILED(hr))
	//{
	//	D_API_ERR(0, TEXT("ReadLoop"), hr, TEXT(""));
	//	LEAVE_BLOCK(0);
	//}
#endif
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CDownloadServerHandler::ProcessPluginFileHelper(CONST BODY_BEGIN_FILE_RSP & data_begin_file_rsp, CONST HPTHeader & Header, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	size_t index = -1;
	for (size_t ii = 0; ii < m_pffConfigs.Configs.size(); ++ii)
	{
		if (m_pffConfigs.Configs[ii].FileType == data_begin_file_rsp.m_ServerFile.m_FileType)
		{
			index = ii;
			break;
		}
	}
	if (-1 == index)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = E_INVALIDARG;
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("FindPluginFileType"), hr, TEXT("FileType=%d, "), data_begin_file_rsp.m_ServerFile.m_FileType);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CONST PluginFileFactoryConfig & config = m_pffConfigs.Configs[index];
	CString strMoudleFullPath;
	if (config.IsRelativePath)
	{
		//D_API_ERR(0, TEXT("IsRelativePath"), E_INVALIDARG, TEXT(""));
		//ATLASSERT(FALSE);
		CONST LONG MOD_BUFFER_SIZE = 1024;
		CString strModuleExe;
		LPTSTR pBUffer = strModuleExe.GetBuffer(MOD_BUFFER_SIZE);
		DWORD dwGMF = GetModuleFileName(
			nullptr,//_In_opt_  HMODULE hModule,
			pBUffer,//_Out_     LPTSTR lpFilename,
			MOD_BUFFER_SIZE//_In_      DWORD nSize
			);
		strModuleExe.ReleaseBuffer();
		if (0 == dwGMF)
		{
			//LastError = WSAGetLastError();
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("GetModuleFileName"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		CMiscHelper::ConvertRelativeToAbsolutePath(strModuleExe, config.ModulePath, strMoudleFullPath);
		D_INFO(0, TEXT("RelativePath=%s, AbsoluetPath=%s"), config.ModulePath.GetString(), strMoudleFullPath.GetString());
	}
	else
	{
		strMoudleFullPath = config.ModulePath;
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IPluginFileFactory> spIPluginFileFactory;
	hr = CreateInstanceIPluginFileFactoryFromModule(strMoudleFullPath.GetString(), config.ApiName.GetString(), &spIPluginFileFactory);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("CreateInstanceIPluginFileFactoryFromModule"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDevice;
	hr = spIPluginFileFactory->CreateFileHelper(data_begin_file_rsp.m_ServerFile, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("CreateFileHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG fileSize = 0;
	hr = spIFileDevice->GetFileSize(&fileSize);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	BOOL bBitmapAvailable = FALSE;
	hr = spIPluginFileFactory->IsBitmapAvailable(spIFileDevice, bBitmapAvailable);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("IsBitmapAvailable"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	if (FALSE == bBitmapAvailable)
	{
		hr = ProcessBuffer(spIFileDevice, 0LL, fileSize, Header, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("ProcessBuffer"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	else //!(FALSE == bBitmapAvailable)
	{
		CONST LONGLONG SIZE_BYTES = 1024 * 1024 * 2; //2M
		CONST LONGLONG SIZE_BITS = SIZE_BYTES * 8;
		CVMemPtr<BYTE> bitmapBuffer;
		bitmapBuffer.Allocate(SIZE_BYTES);
		if (nullptr == bitmapBuffer.m_pData)
		{
			//LastError = WSAGetLastError();
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("Allocate"), hr, TEXT("SIZE_BYTES=%d"), SIZE_BYTES);
			LEAVE_BLOCK(0);
		}


		//////////////////////////////////////////////////////////////////////////
		CComPtr<IFileDeviceMem> spBitmapFile;
		hr = CreateInstanceCFileDeviceMem(bitmapBuffer.m_pData, SIZE_BYTES, &spBitmapFile, TRUE);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("CreateInstanceCFileDeviceMem"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		//LONGLONG totalBlockCount = 0;
		//hr = spIPluginFileFactory->BlockCount(spIFileDevice, totalBlockCount);
		//if (FAILED(hr))
		//{
		//	//LastError = WSAGetLastError();
		//	//LastError = GetLastError();
		//	//hr = HRESULT_FROM_WIN32(LastError);
		//	//D_SET_FISRT_STATUS(hr, LastError);
		//	//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//	//D_SET_LAST_STATUS(hr, LastError);
		//	//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		//	m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		//	D_API_ERR(0, TEXT("BlockCount"), hr, TEXT(""));
		//	LEAVE_BLOCK(0);
		//}

		//////////////////////////////////////////////////////////////////////////
		LONG BlockSize = 0;
		hr = spIPluginFileFactory->BlockSize(spIFileDevice, BlockSize);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("BlockSize"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		CONST LONGLONG DATA_SIZE = SIZE_BITS * BlockSize;
		LONGLONG offset = 0;
		LONGLONG currentSize = 0;
		HPTHeader headerpeeked = { 0 };
		LONG peekedLen = 0;
		while (fileSize)
		{
			//////////////////////////////////////////////////////////////////////////
			peekedLen = 0; //<sonmi01>2015-3-12 ###???
			hr = pCircBufferRecieve->PeekNoLocked((BYTE*)&headerpeeked, sizeof(HPTHeader), peekedLen);
			if (SUCCEEDED(hr) &&
				peekedLen == sizeof(HPTHeader) &&
				Header.m_HeaderVersion == MAKEWORD(0, 0) &&
				Header.m_HeaderSize == sizeof(HPTHeader) &&
				Header.m_CommandCode == CMD_END_SESSION_REQ
				)
			{
				hr = HRESULT_FROM_WIN32(ERROR_CANCELLED);
				D_API_ERR(0, TEXT("CMD_END_SESSION_REQ"), hr, TEXT("client request CMD_END_SESSION_REQ"));
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			if (fileSize > DATA_SIZE)
			{
				currentSize = DATA_SIZE;
			}
			else
			{
				currentSize = fileSize;
			}

			//////////////////////////////////////////////////////////////////////////
			LONGLONG newFP = 0;
			hr = spBitmapFile->SetPointerEx(0, &newFP, FILE_BEGIN);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
				D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			LONGLONG startBlockTmp = offset / BlockSize;
			LONGLONG blockCountTmp = currentSize / BlockSize;
			hr = spIPluginFileFactory->GetBlocksBitmap(spIFileDevice, startBlockTmp, blockCountTmp, spBitmapFile);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
				D_API_ERR(0, TEXT("GetBlocksBitmap"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			CSimpleBitArray bitArray(bitmapBuffer.m_pData, blockCountTmp);
			LONGLONG bitIndex = 0;
			while (bitIndex < bitArray.GetBitLength())
			{
				bitIndex = bitArray.FindFirst(1, bitIndex);
				LONGLONG nextBitIndex = bitArray.FindFirst(0, bitIndex);

				//////////////////////////////////////////////////////////////////////////
				LONGLONG bitCount = nextBitIndex - bitIndex;
				if (bitCount > 0)
				{
					LONGLONG bufferOffset = offset + bitIndex * BlockSize;
					LONGLONG bufferSize = bitCount * BlockSize;
					hr = ProcessBuffer(spIFileDevice, bufferOffset, bufferSize, Header, pCircBufferResult, pCircBufferRecieve);
					if (FAILED(hr))
					{
						//LastError = WSAGetLastError();
						//LastError = GetLastError();
						//hr = HRESULT_FROM_WIN32(LastError);
						//D_SET_FISRT_STATUS(hr, LastError);
						//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
						//D_SET_LAST_STATUS(hr, LastError);
						//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
						D_API_ERR(0, TEXT("ProcessBuffer"), hr, TEXT("bufferOffset=%I64d, bufferSize=%I64d"), bufferOffset, bufferSize);
						break;
					}
				}

				//////////////////////////////////////////////////////////////////////////
				bitIndex = nextBitIndex;
			}
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			offset += currentSize;
			fileSize -= currentSize;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CreateInstanceCDownloadServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppIDownloadHandler, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CDownloadServerHandler(Header, Body), ppIDownloadHandler, bCreateMember);
}
