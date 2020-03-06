#include "stdafx.h"

#include "..\PublicHeader\PublicSimpleBitArray.h"
#include "..\PublicHeader\PublicMiscHelper.h"

#include "UploadClient.h"
#include "EventHelper.h"
#include "HPTHelper.h"
#include "CreateWin32FileHelper.h"



CUploadClient::CUploadClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_CREATE_UPLOAD_SESSION_REQ & body_creae_upload_session_req) :
m_RefCount(0),
m_pSender(pSender),
m_pReceiver(pReceiver),
m_ClientServerFiles(body_creae_upload_session_req),
m_hSendReceiverMonitorThread(NULL)
{

}

HRESULT CUploadClient::CreateMember()
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
	CONST LONG INITIAL_SIZE = 1024 * 1024 * 2;
	hr = m_TempBody.Allocate(INITIAL_SIZE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("Size=%d"), INITIAL_SIZE);
		LEAVE_BLOCK(0);
	}

	hr = CreateInstanceCUploadClientHandler(&m_spIHPTHandlerClient, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCUploadClientHandler"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = CreateInstanceCHPTSendReceive(
		m_pSender,//IFileDevice * pSender,
		m_pReceiver,//IFileDevice * pReceiver,
		FALSE,//BOOL bAttachSenderReceiver,
		NULL,//IHPTServerHandlerFactory * pHPTNewHandlerFactory,
		FALSE,//BOOL bServer,
		&m_spIHPTSendReceive,//IHPTSendReceive  ** ppIHPTSendReceive,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCHPTSendReceive"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_spIHPTSendReceive->SetClientHandler(m_spIHPTHandlerClient.p);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("IHPTSendReceive->SetClientHandler"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	m_hSendReceiverMonitorThread = CreateSendReceiverMonitorThread(m_spIHPTSendReceive.p);
	if (NULL == m_hSendReceiverMonitorThread)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT(""), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CUploadClient::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_spIHPTSendReceive.Release();
	m_spIHPTHandlerClient.Release();
	m_TempBody.Free();
	if (m_hSendReceiverMonitorThread)
	{
		CloseHandle(m_hSendReceiverMonitorThread);
		m_hSendReceiverMonitorThread = NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CUploadClient::Main()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	DWORD WaitIndex = -1;

	HRESULT hrFile = S_OK;

	CONST LONG __15min = 1000 * 60 * 15;
	CONST LONG __30min = 1000 * 60 * 30;
	CONST LONG __60min = 1000 * 60 * 60;

	BOOL bReceiveServerStat = FALSE;

	//////////////////////////////////////////////////////////////////////////
	HPTHeader HeaderTemp;
	HPTBody BodyTemp;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST LONG __2M = 1024 * 1024 * 2;
	hr = BodyTemp.Allocate(__2M);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CString strXml;

	hr = BodyTemp.Allocate(0);

	HeaderTemp.Init();
	//HeaderTemp.m_HeaderSize;
	//HeaderTemp.m_HeaderVersion;
	HeaderTemp.m_CommandCode = CMD_CREATE_UPLOAD_SESSION_REQ;
	//HeaderTemp.m_FlagOrEnum;
	HeaderTemp.m_PayloadLength = BodyTemp.m_DataSize;
	//HeaderTemp.m_PayloadFileOffset;
	HeaderTemp.m_ClientHandler = PtrToUll<IHPTHandler>(m_spIHPTHandlerClient.p);
	//HeaderTemp.m_ServerHandler;
	//HeaderTemp.m_Data64;

	hr = m_spIHPTSendReceive->PutCommandToSend(HeaderTemp, BodyTemp);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("PutCommandToSend"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	WaitIndex = m_spIHPTHandlerClient->WaitAny(__15min);
	if (NS_MultipleEventHelper::SYNC_INDEX != WaitIndex - WAIT_OBJECT_0)
	{
		//HRESULT hrTmp = m_spIHPTHandlerClient->GetStatusStatistic()->m_Error.m_LastStatus;
		D_API_ERR(0, TEXT("Wait"), hr, TEXT("WaitIndex=%d"), WaitIndex);
		LEAVE_BLOCK(0);
	}

	ULONGLONG ServerHandlerTemp = m_spIHPTHandlerClient->GetServerHandler();
	ATLASSERT(ServerHandlerTemp);

	//////////////////////////////////////////////////////////////////////////
	for (size_t ii = 0; ii < m_ClientServerFiles.m_CilentFiles.size(); ++ ii)
	{
		hr = ProcessFile(ServerHandlerTemp, ii);
		if (FAILED(hr))
		{
			hrFile = hr;
			D_API_ERR(0, TEXT("ProcessWin32File"), hr, TEXT(""));
			break;
		}
	}
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT(""), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	BodyTemp.Allocate(0);

	HeaderTemp.Init();
	//HeaderTemp.m_HeaderSize;
	//HeaderTemp.m_HeaderVersion;
	HeaderTemp.m_CommandCode = CMD_END_UPLOAD_REQ;
	//HeaderTemp.m_FlagOrEnum;
	//HeaderTemp.m_PayloadLength;
	//HeaderTemp.m_PayloadFileOffset;
	HeaderTemp.m_ClientHandler = PtrToUll<IHPTHandler>(m_spIHPTHandlerClient.p);;
	HeaderTemp.m_ServerHandler = ServerHandlerTemp;
	//HeaderTemp.m_Data64;

	hr = m_spIHPTSendReceive->PutCommandToSend(HeaderTemp, BodyTemp);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("PutCommandToSend"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	WaitIndex = m_spIHPTHandlerClient->WaitAny(__15min);
	if (NS_MultipleEventHelper::SYNC_INDEX != WaitIndex - WAIT_OBJECT_0)
	{
		//HRESULT hrTmp = m_spIHPTHandlerClient->GetStatusStatistic()->m_Error.m_LastStatus;
		D_API_ERR(0, TEXT("Wait"), hr, TEXT("WaitIndex=%d"), WaitIndex);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	BodyTemp.Allocate(0); //<sonmi01>2014-2-9 ###???

	HeaderTemp.Init();
	//HeaderTemp.m_HeaderSize;
	//HeaderTemp.m_HeaderVersion;
	HeaderTemp.m_CommandCode = CMD_HANDLER_STATUS_STATISTIC_TOTAL_REQ;
	//HeaderTemp.m_FlagOrEnum;
	//HeaderTemp.m_PayloadLength;
	//HeaderTemp.m_PayloadFileOffset;
	HeaderTemp.m_ClientHandler = PtrToUll<IHPTHandler>(m_spIHPTHandlerClient.p);;
	HeaderTemp.m_ServerHandler = ServerHandlerTemp;
	//HeaderTemp.m_Data64;

	hr = m_spIHPTSendReceive->PutCommandToSend(HeaderTemp, BodyTemp);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("PutCommandToSend"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	WaitIndex = m_spIHPTHandlerClient->WaitAny(__15min);
	if (NS_MultipleEventHelper::SYNC_INDEX != WaitIndex - WAIT_OBJECT_0)
	{
		//HRESULT hrTmp = m_spIHPTHandlerClient->GetStatusStatistic()->m_Error.m_LastStatus;
		D_API_ERR(0, TEXT("Wait"), hr, TEXT("WaitIndex=%d"), WaitIndex);
		LEAVE_BLOCK(0);
	}
	//m_hSendReceiverMonitorThread(NULL)
	bReceiveServerStat = TRUE;

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	BodyTemp.Allocate(0);

	HeaderTemp.Init();
	//HeaderTemp.m_HeaderSize;
	//HeaderTemp.m_HeaderVersion;
	HeaderTemp.m_CommandCode = CMD_END_SESSION_REQ;
	//HeaderTemp.m_FlagOrEnum;
	//HeaderTemp.m_PayloadLength;
	//HeaderTemp.m_PayloadFileOffset;
	HeaderTemp.m_ClientHandler = PtrToUll<IHPTHandler>(m_spIHPTHandlerClient.p);
	//HeaderTemp.m_ServerHandler;
	//HeaderTemp.m_Data64;

	//hr = m_spIHPTHandlerClient->ResetAll();
	//if (FAILED(hr))
	//{
	//	D_API_ERR(0, TEXT("ResetAll"), hr, TEXT(""));
	//	//LEAVE_BLOCK(0);
	//}
	for (LONG ii = 0; ii <= NS_MultipleEventHelper::DONE_INDEX; ++ii) //<sonmi01>2014-3-28 ###???
	{
		HRESULT hrTemp = m_spIHPTHandlerClient->Reset(ii);
		if (FAILED(hrTemp))
		{
			hr = hrTemp;
			D_API_ERR(0, TEXT("Reset"), hr, TEXT(""));
		}
	}


	hr = m_spIHPTSendReceive->PutCommandToSend(HeaderTemp, BodyTemp);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("PutCommandToSend"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	WaitIndex = m_spIHPTHandlerClient->WaitAny(__15min);
	if (NS_MultipleEventHelper::SYNC_INDEX != WaitIndex - WAIT_OBJECT_0)
	{
		//HRESULT hrTmp = m_spIHPTHandlerClient->GetStatusStatistic()->m_Error.m_LastStatus;
		D_API_ERR(0, TEXT("Wait"), hr, TEXT("WaitIndex=%d"), WaitIndex);
		//LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	hr = m_spIHPTSendReceive->Set(FAILED(hr) ? NS_MultipleEventHelper::ERROR_INDEX : NS_MultipleEventHelper::DONE_INDEX);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("IHPTSendReceive->Set"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}


	DWORD dwWait = WaitForSingleObject(m_hSendReceiverMonitorThread, __30min);
	if (WAIT_OBJECT_0 == dwWait)
	{
		D_INFO(0, TEXT("SendReceiverMonitorThread ended"));
	}
	else if (WAIT_TIMEOUT == dwWait)
	{
		TerminateThread(m_hSendReceiverMonitorThread, E_FAIL);
		D_INFO(0, TEXT("SendReceiverMonitorThread terminated"));
	}
	else
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("WaitForSingleObject"), hr, TEXT(""));
	}


	//////////////////////////////////////////////////////////////////////////
	HRESULT FinalStatus = S_OK;
	if (FAILED(hr))
	{
		FinalStatus = hr;
		D_API_ERR(0, TEXT("Main"), FinalStatus, TEXT(""));
	}
	else if (FAILED(hrFile))
	{
		FinalStatus = hrFile;
		D_API_ERR(0, TEXT("File"), FinalStatus, TEXT(""));
	}
	else if (FAILED(m_spIHPTHandlerClient->GetStatusStatistic()->m_Error.m_LastStatus))
	{
		FinalStatus = m_spIHPTHandlerClient->GetStatusStatistic()->m_Error.m_LastStatus;
		D_API_ERR(0, TEXT("ClientHandler"), FinalStatus, TEXT(""));
	}
	else
	{
		if (!bReceiveServerStat)
		{
			FinalStatus = E_FAIL;
			D_API_ERR(0, TEXT("FinalStatus not received"), FinalStatus, TEXT(""));
		}
		if (FAILED(m_spIHPTHandlerClient->GetStatusStatisticServer()->m_Error.m_LastStatus))
		{
			FinalStatus = m_spIHPTHandlerClient->GetStatusStatisticServer()->m_Error.m_LastStatus;
			D_API_ERR(0, TEXT("ServerHandler"), FinalStatus, TEXT(""));
		}
	}
	//////////////////////////////////////////////////////////////////////////

	return FinalStatus;
}

HRESULT CUploadClient::ProcessFile(ULONGLONG ServerHandler, LONG FileIndex)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strXml;
	BODY_BEGIN_FILE_REQ body_begin_file_req;
	body_begin_file_req.m_CilentFile = m_ClientServerFiles.m_CilentFiles[FileIndex];
	body_begin_file_req.m_ServerFile = m_ClientServerFiles.m_ServerFiles[FileIndex];
	body_begin_file_req.ToString(MethodString(BODY_BEGIN_FILE_REQ), strXml);
	hr = m_TempBody.Allocate(strXml.GetLength() * sizeof(TCHAR)+sizeof(TCHAR));
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	CopyMemory(m_TempBody.m_pData, strXml.GetString(), m_TempBody.m_DataSize);

	m_TempHeader.Init();
	m_TempHeader.m_ClientHandler = PtrToUll<IHPTHandler>(m_spIHPTHandlerClient.p);
	m_TempHeader.m_ServerHandler = ServerHandler;
	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
	m_TempHeader.m_CommandCode = CMD_BEGIN_FILE_REQ;

	D_INFO(0, TEXT("Begin file [%s, %s]"), body_begin_file_req.m_CilentFile.m_FileName.GetString(), body_begin_file_req.m_ServerFile.m_FileName.GetString());
	D_INFO(0, TEXT("Begin file [%s, %s]"), body_begin_file_req.m_CilentFile.m_MoreContext.GetString(), body_begin_file_req.m_ServerFile.m_MoreContext.GetString());

	hr = m_spIHPTSendReceive->PutCommandToSend(m_TempHeader, m_TempBody);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("PutCommandToSend"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

#if 1
	if (0 == body_begin_file_req.m_CilentFile.m_FileType) //process win32 file
	{
		hr = ProcessWin32FileHelper(body_begin_file_req, ServerHandler);
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
	else //process plug-in file
	{
		hr = ProcessPluginFileHelper(body_begin_file_req, ServerHandler);
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
	////////////////////////////////////////////////////////////////////////////
	//CComPtr<IFileDevice> spFileDevide;
	//hr = CreateWin32FileHelper(
	//	body_begin_file_req.m_CilentFile,//BODY_FILE_DESCRITION & FileDesc, 
	//	GENERIC_READ,//IN ULONG ulDesiredAccess, 
	//	FILE_SHARE_READ,//IN ULONG ulShareMode, 
	//	OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
	//	0,//FILE_FLAG_NO_BUFFERING,//IN ULONG ulFlagsAndAttributes, 
	//	&spFileDevide//IFileDevice ** ppIFileDevice
	//	);
	//if (FAILED(hr))
	//{
	//	D_API_ERR(0, TEXT("CreateWin32FileHelper"), hr, TEXT("%s"), body_begin_file_req.m_CilentFile.m_FileName.GetString());
	//	LEAVE_BLOCK(0);
	//}

	////////////////////////////////////////////////////////////////////////////
	//LONGLONG FileSize = 0;
	//hr = spFileDevide->GetFileSize(&FileSize);
	//if (FAILED(hr))
	//{
	//	D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
	//	LEAVE_BLOCK(0);
	//}

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
	//		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
	//		LEAVE_BLOCK(0);
	//	}

	//	//////////////////////////////////////////////////////////////////////////
	//	ReadLength = 0;
	//	hr = ReadAtOffsetHelper(spFileDevide, Offset, ToReadLength, &ReadLength, m_TempBody.m_pData);
	//	if (FAILED(hr))
	//	{
	//		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
	//		LEAVE_BLOCK(0);
	//	}

	//	//////////////////////////////////////////////////////////////////////////
	//	m_TempBody.m_DataSize = ReadLength;

	//	m_TempHeader.Init();
	//	m_TempHeader.m_ClientHandler = PtrToUll<IHPTHandler>(m_spIHPTHandlerClient.p);
	//	m_TempHeader.m_ServerHandler = ServerHandler;
	//	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
	//	m_TempHeader.m_PayloadFileOffset = Offset;
	//	m_TempHeader.m_CommandCode = CMD_FILE_DATA_REQ;



	//	hr = m_spIHPTSendReceive->PutCommandToSend(m_TempHeader, m_TempBody);
	//	if (FAILED(hr))
	//	{
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
	BODY_END_FILE_REQ & data_end_file_req = body_begin_file_req; //BODY_END_FILE_RSP and BODY_BEGIN_FILE_RSP are same
	strXml.Empty();
	data_end_file_req.ToString(MethodString(BODY_END_FILE_REQ), strXml);
	hr = m_TempBody.Allocate(strXml.GetLength() * sizeof(TCHAR)+sizeof(TCHAR));
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	CopyMemory(m_TempBody.m_pData, strXml.GetString(), m_TempBody.m_DataSize);

	m_TempHeader.Init();
	m_TempHeader.m_ClientHandler = PtrToUll<IHPTHandler>(m_spIHPTHandlerClient.p);
	m_TempHeader.m_ServerHandler = ServerHandler;
	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
	m_TempHeader.m_CommandCode = CMD_END_FILE_REQ;

	D_INFO(0, TEXT("End file [%s, %s]"), data_end_file_req.m_CilentFile.m_FileName.GetString(), data_end_file_req.m_ServerFile.m_FileName.GetString());
	D_INFO(0, TEXT("End file [%s, %s]"), data_end_file_req.m_CilentFile.m_MoreContext.GetString(), data_end_file_req.m_ServerFile.m_MoreContext.GetString());

	hr = m_spIHPTSendReceive->PutCommandToSend(m_TempHeader, m_TempBody);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("PutCommandToSend"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//<sonmi01>2015-3-7 ###???
HRESULT CUploadClient::ProcessWin32FileHelper(CONST BODY_BEGIN_FILE_REQ & body_begin_file_req, ULONGLONG ServerHandler)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spFileDevide;
	hr = CreateWin32FileHelper(
		body_begin_file_req.m_CilentFile,//BODY_FILE_DESCRITION & FileDesc, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		FILE_SHARE_READ,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		0,//FILE_FLAG_NO_BUFFERING,//IN ULONG ulFlagsAndAttributes, 
		&spFileDevide//IFileDevice ** ppIFileDevice
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateWin32FileHelper"), hr, TEXT("%s"), body_begin_file_req.m_CilentFile.m_FileName.GetString());
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG FileSize = 0;
	hr = spFileDevide->GetFileSize(&FileSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

#if 1
	hr = ProcessBuffer(spFileDevide, ServerHandler, 0LL, FileSize);
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
	//		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
	//		LEAVE_BLOCK(0);
	//	}

	//	//////////////////////////////////////////////////////////////////////////
	//	ReadLength = 0;
	//	hr = ReadAtOffsetHelper(spFileDevide, Offset, ToReadLength, &ReadLength, m_TempBody.m_pData);
	//	if (FAILED(hr))
	//	{
	//		D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT(""));
	//		LEAVE_BLOCK(0);
	//	}

	//	//////////////////////////////////////////////////////////////////////////
	//	m_TempBody.m_DataSize = ReadLength;

	//	m_TempHeader.Init();
	//	m_TempHeader.m_ClientHandler = PtrToUll<IHPTHandler>(m_spIHPTHandlerClient.p);
	//	m_TempHeader.m_ServerHandler = ServerHandler;
	//	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
	//	m_TempHeader.m_PayloadFileOffset = Offset;
	//	m_TempHeader.m_CommandCode = CMD_FILE_DATA_REQ;



	//	hr = m_spIHPTSendReceive->PutCommandToSend(m_TempHeader, m_TempBody);
	//	if (FAILED(hr))
	//	{
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

HRESULT CUploadClient::ProcessBuffer(IFileDevice * pFileDevide, ULONGLONG ServerHandler, LONGLONG currentOffset, LONGLONG currentSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST LONG __2M = 1024 * 1024 * 2;
	LONG ToReadLength = 0;
	ULONG ReadLength = 0;
	while (currentSize)
	{
		//////////////////////////////////////////////////////////////////////////
		if (m_spIHPTHandlerClient->IsPeerError())
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(ERROR_CANCELLED);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("PeerError"), hr, TEXT(""));
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
			D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		ReadLength = 0;
		hr = ReadAtOffsetHelper(pFileDevide, currentOffset, ToReadLength, &ReadLength, m_TempBody.m_pData);
		if (FAILED(hr))
		{
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
		m_TempHeader.m_ClientHandler = PtrToUll<IHPTHandler>(m_spIHPTHandlerClient.p);
		m_TempHeader.m_ServerHandler = ServerHandler;
		m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
		m_TempHeader.m_PayloadFileOffset = currentOffset;
		m_TempHeader.m_CommandCode = CMD_FILE_DATA_REQ;



		hr = m_spIHPTSendReceive->PutCommandToSend(m_TempHeader, m_TempBody);
		if (FAILED(hr))
		{
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

HRESULT CUploadClient::GetPluginFileFactoryConfigs()
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

HRESULT CUploadClient::ProcessPluginFileHelper(CONST BODY_BEGIN_FILE_REQ & body_begin_file_req, ULONGLONG ServerHandler)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	size_t index = -1;
	for (size_t ii = 0; ii < m_pffConfigs.Configs.size(); ++ii)
	{
		if (m_pffConfigs.Configs[ii].FileType == body_begin_file_req.m_CilentFile.m_FileType)
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
		D_API_ERR(0, TEXT("FindPluginFileType"), hr, TEXT("FileType=%d, "), body_begin_file_req.m_CilentFile.m_FileType);
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
		D_API_ERR(0, TEXT("CreateInstanceIPluginFileFactoryFromModule"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDevice;
	hr = spIPluginFileFactory->CreateFileHelper(body_begin_file_req.m_CilentFile, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
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
		D_API_ERR(0, TEXT("IsBitmapAvailable"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	
	//////////////////////////////////////////////////////////////////////////
	if (FALSE == bBitmapAvailable)
	{
		hr = ProcessBuffer(spIFileDevice, ServerHandler, 0LL, fileSize);
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
			D_API_ERR(0, TEXT("BlockSize"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		CONST LONGLONG DATA_SIZE = SIZE_BITS * BlockSize;
		LONGLONG offset = 0;
		LONGLONG currentSize = 0;
		while (fileSize)
		{
			//////////////////////////////////////////////////////////////////////////
			if (m_spIHPTHandlerClient->IsPeerError())
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(ERROR_CANCELLED);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("PeerError"), hr, TEXT(""));
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
					hr = ProcessBuffer(spIFileDevice, ServerHandler, bufferOffset, bufferSize);
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


HRESULT CreateInstanceCUploadClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_CREATE_DOWNLOAD_SESSION_REQ & data_creae_download_session_req, IJobMainFlow ** ppIJobMainFlow, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		new CUploadClient(pSender, pReceiver, data_creae_download_session_req),
		ppIJobMainFlow,
		bCreateMember
		);
}

HRESULT HPTUploadClient(LPCTSTR pServer, LPCTSTR pPort, CONST BODY_CREATE_UPLOAD_SESSION_REQ & data_creae_upload_session_req)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<ISocketClient> spISocketClient;
	hr = CreateInstanceCSocketClient(pServer, pPort, &spISocketClient, FALSE, FALSE, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCSocketClient"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = HPTUploadClient(spISocketClient.p, spISocketClient.p, data_creae_upload_session_req);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("HPTUploadClient"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


HRESULT HPTUploadClient(IFileDevice * pSender, IFileDevice * pReceiver, CONST BODY_CREATE_UPLOAD_SESSION_REQ & data_creae_upload_session_req)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IJobMainFlow> spIJobMainFlow;
	hr = CreateInstanceCUploadClient(
		pSender,//IFileDevice * pSender, 
		pReceiver,//IFileDevice * pReceiver, 
		data_creae_upload_session_req,//CONST BODY_CREATE_DOWNLOAD_SESSION_REQ & data_creae_download_session_req,
		&spIJobMainFlow,//IJobMainFlow ** ppIJobMainFlow, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDowloadClient"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = spIJobMainFlow->Main();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("IJobMainFlow->Main"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

