#include "stdafx.h"
#include "UploadServerHandler.h"
#include "CreateWin32FileHelper.h"


HRESULT CreateInstanceCUploadServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body, IHPTHandler ** ppIDownloadHandler, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CUploadServerHandler(Header, Body), ppIDownloadHandler, bCreateMember);
}

HRESULT CUploadServerHandler::ProcessCommand_CMD_BEGIN_FILE_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strXml = (LPCTSTR)(Body.m_pData);
	BODY_BEGIN_FILE_REQ body_begin_file_req;
	body_begin_file_req.FromString(MethodString(BODY_BEGIN_FILE_REQ), strXml);

	D_INFO(0, TEXT("Begin file [%s, %s]"), body_begin_file_req.m_CilentFile.m_FileName.GetString(), body_begin_file_req.m_ServerFile.m_FileName.GetString());
	D_INFO(0, TEXT("Begin file [%s, %s]"), body_begin_file_req.m_CilentFile.m_MoreContext.GetString(), body_begin_file_req.m_ServerFile.m_MoreContext.GetString());

	//////////////////////////////////////////////////////////////////////////
	CString strParent;
	hr = CMiscHelper::GetStringParentByLevel(strParent, body_begin_file_req.m_ServerFile.m_FileName.GetString(), 1, TEXT('\\'));
	if (S_OK == hr)
	{
		if (!CMiscHelper::IsDirExists(strParent.GetString()))
		{
			hr = CMiscHelper::CreateDirectoryRecursively(strParent.GetString());
			if (FAILED(hr))
			{
				m_StatusStatistic.m_Error.SetLastStatus(hr, strParent.GetString());
				D_API_ERR(0, TEXT("CreateDirectoryRecursively"), hr, TEXT("%s"), strParent.GetString());
				LEAVE_BLOCK(0);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	ATLASSERT(NULL == m_spIFileDeviceCurrent.p);

	if (0 == body_begin_file_req.m_ServerFile.m_FileType)
	{
		hr = CreateWin32FileHelper(
			body_begin_file_req.m_ServerFile,//CONST BODY_FILE_DESCRITION & FileDesc,
			GENERIC_WRITE,//IN ULONG ulDesiredAccess,
			0,//IN ULONG ulShareMode,
			CREATE_ALWAYS,//IN ULONG ulCreationDisposition,
			0,//FILE_FLAG_NO_BUFFERING,//IN ULONG ulFlagsAndAttributes,
			&m_spIFileDeviceCurrent//IFileDevice ** ppIFileDevice
			);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, body_begin_file_req.m_ServerFile.m_FileName.GetString());
			D_API_ERR(0, TEXT("CreateWin32FileHelper"), hr, TEXT("%s"), body_begin_file_req.m_ServerFile.m_FileName.GetString());
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		size_t index = -1;
		for (size_t ii = 0; ii < m_pffConfigs.Configs.size(); ++ii)
		{
			if (m_pffConfigs.Configs[ii].FileType == body_begin_file_req.m_ServerFile.m_FileType)
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
			D_API_ERR(0, TEXT("FindPluginFileType"), hr, TEXT("FileType=%d, "), body_begin_file_req.m_ServerFile.m_FileType);
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
		hr = spIPluginFileFactory->CreateFileHelper(body_begin_file_req.m_ServerFile, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &m_spIFileDeviceCurrent);
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
	}
	

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CUploadServerHandler::ProcessCommand_CMD_FILE_DATA_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ATLASSERT(Header.m_PayloadLength == Body.m_DataSize);

	ULONG NumberOfBytesWritten = 0;
	if (NULL == m_spIFileDeviceCurrent.p)
	{
		hr = E_HANDLE;
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	else
	{
		hr = WriteAtOffsetHelper(m_spIFileDeviceCurrent, Header.m_PayloadFileOffset, Body.m_DataSize, &NumberOfBytesWritten, Body.m_pData);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		if (Body.m_DataSize != NumberOfBytesWritten)
		{
			hr = HRESULT_FROM_WIN32(ERROR_INCORRECT_SIZE);
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0)
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CUploadServerHandler::ProcessCommand_CMD_END_FILE_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strXml = (LPCTSTR)(Body.m_pData);
	BODY_END_FILE_REQ body_end_file_req;
	body_end_file_req.FromString(MethodString(BODY_END_FILE_REQ), strXml);

	D_INFO(0, TEXT("End file [%s, %s]"), body_end_file_req.m_CilentFile.m_FileName.GetString(), body_end_file_req.m_ServerFile.m_FileName.GetString());
	D_INFO(0, TEXT("End file [%s, %s]"), body_end_file_req.m_CilentFile.m_MoreContext.GetString(), body_end_file_req.m_ServerFile.m_MoreContext.GetString());

	//ATLASSERT(m_spIFileDeviceCurrent.p);
	if (NULL == m_spIFileDeviceCurrent.p)
	{
		hr = E_HANDLE;
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
	}
	m_spIFileDeviceCurrent.Release();

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

CUploadServerHandler::CUploadServerHandler(CONST HPTHeader & Header, CONST HPTBody & Body) :
m_Header(Header),
m_Body(Body),
m_RefCount(0)
{

}

HRESULT CUploadServerHandler::CreateMember()
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
	ATLASSERT(CMD_CREATE_UPLOAD_SESSION_REQ == m_Header.m_CommandCode);

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

HRESULT CUploadServerHandler::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_spIMultipleEventHelper.Release();
	m_TempBody.Free();
	m_spIFileDeviceCurrent.Release();
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CUploadServerHandler::ProcessCommand(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (CMD_BEGIN_FILE_REQ == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_BEGIN_FILE_REQ(Header, Body, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ProcessCommand_ CMD_BEGIN_FILE_REQ"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}
	else if (CMD_FILE_DATA_REQ == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_FILE_DATA_REQ(Header, Body, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ProcessCommand_ CMD_FILE_DATA_REQ"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}
	else if (CMD_END_FILE_REQ == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_END_FILE_REQ(Header, Body, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ProcessCommand_ CMD_END_FILE_REQ"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}
	else if (CMD_END_UPLOAD_REQ == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_END_UPLOAD_REQ(Header, Body, pCircBufferResult, pCircBufferRecieve);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ProcessCommand_ CMD_END_UPLOAD_REQ"), hr, TEXT(""));
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

DWORD CUploadServerHandler::WaitAny(DWORD WaitMillSec)
{
	return m_spIMultipleEventHelper->WaitMultiple(FALSE, WaitMillSec);
}

HRESULT CUploadServerHandler::Set(LONG Index)
{
	return m_spIMultipleEventHelper->Set(Index);
}

ULONGLONG CUploadServerHandler::GetServerHandler()
{
	return NULL;
}

HRESULT CUploadServerHandler::ProcessCommand_CMD_END_UPLOAD_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	m_TempBody.Allocate(0);

	m_TempHeader.Init();
	m_TempHeader.m_ClientHandler = Header.m_ClientHandler;
	m_TempHeader.m_ServerHandler = Header.m_ServerHandler;
	m_TempHeader.m_CommandCode = CMD_END_UPLOAD_RSP;

	HRESULT hr = PutCommmandHelper(m_TempHeader, m_TempBody, pCircBufferResult);
	if (FAILED(hr))
	{
		m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
		D_API_ERR(0, TEXT("PutCommmandHelper"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	return hr;
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CUploadServerHandler::GetStatusStatistic()
{
	return &m_StatusStatistic;
}

HRESULT CUploadServerHandler::ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_REQ(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CUploadServerHandler::GetStatusStatisticServer()
{
	return NULL;
}

HRESULT CUploadServerHandler::Reset(LONG Index)
{
	return m_spIMultipleEventHelper->Reset(Index);
}

HRESULT CUploadServerHandler::Wait(LONG Index, DWORD MillSec)
{
	return m_spIMultipleEventHelper->Wait(Index, MillSec);
}

DWORD CUploadServerHandler::ResetAll()
{
	return m_spIMultipleEventHelper->ResetAll();
}

HRESULT CUploadServerHandler::GetPluginFileFactoryConfigs()
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
