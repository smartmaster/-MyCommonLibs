#include "stdafx.h"

#include "DownloadClientHandler.h"


HRESULT CDownloadClientHandler::ProcessCommand_CMD_CREAE_DOWNLOAD_SESSION_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

HRESULT CDownloadClientHandler::ProcessCommand_CMD_BEGIN_FILE_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strXml = (LPCTSTR)(Body.m_pData);
	BODY_BEGIN_FILE_RSP data_begin_file_rsp;
	data_begin_file_rsp.FromString(MethodString(BODY_BEGIN_FILE_RSP), strXml);

	D_INFO(0, TEXT("Begin file [%s, %s]"), data_begin_file_rsp.m_CilentFile.m_FileName.GetString(), data_begin_file_rsp.m_ServerFile.m_FileName.GetString());
	D_INFO(0, TEXT("Begin file [%s, %s]"), data_begin_file_rsp.m_CilentFile.m_MoreContext.GetString(), data_begin_file_rsp.m_ServerFile.m_MoreContext.GetString());
	//////////////////////////////////////////////////////////////////////////
	CString strParent;
	hr = CMiscHelper::GetStringParentByLevel(strParent, data_begin_file_rsp.m_CilentFile.m_FileName.GetString(), 1, TEXT('\\'));
	if (S_OK == hr)
	{
		if (!CMiscHelper::IsDirExists(strParent.GetString()))
		{
			hr = CMiscHelper::CreateDirectoryRecursively(strParent.GetString());
			if (FAILED(hr))
			{
				m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
				D_API_ERR(0, TEXT("CreateDirectoryRecursively"), hr, TEXT("%s"), strParent.GetString());
				LEAVE_BLOCK(0);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	ATLASSERT(NULL == m_spIFileDeviceCurrent.p);
	if (0 == data_begin_file_rsp.m_CilentFile.m_FileType)
	{
		hr = CreateWin32FileHelper(
			data_begin_file_rsp.m_CilentFile,//CONST BODY_FILE_DESCRITION & FileDesc,
			GENERIC_WRITE,//IN ULONG ulDesiredAccess,
			0,//IN ULONG ulShareMode,
			CREATE_ALWAYS,//IN ULONG ulCreationDisposition,
			0,//FILE_FLAG_NO_BUFFERING,//IN ULONG ulFlagsAndAttributes,
			&m_spIFileDeviceCurrent//IFileDevice ** ppIFileDevice
			);
		if (FAILED(hr))
		{
			m_StatusStatistic.m_Error.SetLastStatus(hr, TEXT(""));
			D_API_ERR(0, TEXT("CreateWin32FileHelper"), hr, TEXT("%s"), data_begin_file_rsp.m_CilentFile.m_FileName.GetString());
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		size_t index = -1;
		for (size_t ii = 0; ii < m_pffConfigs.Configs.size(); ++ii)
		{
			if (m_pffConfigs.Configs[ii].FileType == data_begin_file_rsp.m_CilentFile.m_FileType)
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
			D_API_ERR(0, TEXT("FindPluginFileType"), hr, TEXT("FileType=%d, "), data_begin_file_rsp.m_CilentFile.m_FileType);
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
		hr = spIPluginFileFactory->CreateFileHelper(data_begin_file_rsp.m_CilentFile, GENERIC_WRITE, 0, CREATE_ALWAYS, 0, &m_spIFileDeviceCurrent);
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

HRESULT CDownloadClientHandler::ProcessCommand_CMD_FILE_DATA_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

HRESULT CDownloadClientHandler::ProcessCommand_CMD_END_FILE_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strXml = (LPCTSTR)(Body.m_pData);
	BODY_END_FILE_RSP data_end_file_rsp;
	data_end_file_rsp.FromString(MethodString(BODY_END_FILE_RSP), strXml);

	D_INFO(0, TEXT("End file [%s, %s]"), data_end_file_rsp.m_CilentFile.m_FileName.GetString(), data_end_file_rsp.m_ServerFile.m_FileName.GetString());
	D_INFO(0, TEXT("End file [%s, %s]"), data_end_file_rsp.m_CilentFile.m_MoreContext.GetString(), data_end_file_rsp.m_ServerFile.m_MoreContext.GetString());

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

HRESULT CDownloadClientHandler::ProcessCommand_CMD_END_TRANSFER_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

HRESULT CDownloadClientHandler::ProcessCommand_CMD_END_SESSION_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

HRESULT CDownloadClientHandler::ProcessCommand_CMD_HANDLER_STATUS_STATISTIC_TOTAL_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

HRESULT CDownloadClientHandler::ProcessCommand_CMD_RESULT_RSP(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
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

CDownloadClientHandler::CDownloadClientHandler() :
m_RefCount(0),
m_ServerHandler(0),
m_bPeerError(FALSE)
{

}

HRESULT CDownloadClientHandler::CreateMember()
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

HRESULT CDownloadClientHandler::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_spIMultipleEventHelper.Release();

	m_spIFileDeviceCurrent.Release();

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//HRESULT CDownloadClientHandler::SetCurrentCommandFinished()
//{
//	return m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::SYNC_INDEX);
//}

DWORD CDownloadClientHandler::WaitAny(DWORD WaitMillSec)
{
	return m_spIMultipleEventHelper->WaitMultiple(FALSE, WaitMillSec);
}

ULONGLONG CDownloadClientHandler::GetServerHandler()
{
	return m_ServerHandler;
}

HRESULT CDownloadClientHandler::Set(LONG Index)
{
	return m_spIMultipleEventHelper->Set(Index);
}



HRESULT CDownloadClientHandler::ProcessCommand(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBufferResult, CCircularBuffer * pCircBufferRecieve)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (CMD_CREATE_DOWNLOAD_SESSION_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_CREAE_DOWNLOAD_SESSION_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
	}
	else if (CMD_BEGIN_FILE_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_BEGIN_FILE_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
	}
	else if (CMD_FILE_DATA_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_FILE_DATA_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
	}
	else if (CMD_END_FILE_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_END_FILE_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
	}
	else if (CMD_END_TRANSFER_RSP == Header.m_CommandCode)
	{
		hr = ProcessCommand_CMD_END_TRANSFER_RSP(Header, Body, pCircBufferResult, pCircBufferRecieve);
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

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CDownloadClientHandler::GetStatusStatistic()
{
	return &m_StatusStatistic;
}

CONST BODY_HANDLER_STATUS_STATISTIC_TOTAL * CDownloadClientHandler::GetStatusStatisticServer()
{
	return &m_StatusStatisticServer;
}

HRESULT CDownloadClientHandler::Reset(LONG Index)
{
	return m_spIMultipleEventHelper->Reset(Index);
}

HRESULT CDownloadClientHandler::Wait(LONG Index, DWORD MillSec)
{
	return m_spIMultipleEventHelper->Wait(Index, MillSec);
}

DWORD CDownloadClientHandler::ResetAll()
{
	return m_spIMultipleEventHelper->ResetAll();
}

HRESULT CDownloadClientHandler::GetPluginFileFactoryConfigs()
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

BOOL CDownloadClientHandler::IsPeerError()
{
	return m_bPeerError;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCDownloadClientHandler(IHPTHandler ** ppIHPTHandler, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new CDownloadClientHandler(), ppIHPTHandler, bCreateMember);
}
