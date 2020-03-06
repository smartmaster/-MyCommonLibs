#include "stdafx.h"

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "DuplicateSocket.h"

HRESULT DuplicateSocketToTargetProcess(IN LPCTSTR pCommandLine, IN OUT SOCKET & ConnectionSocket)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	PROCESS_INFORMATION ProcInfo; /***TO CLEANUP***/
	ZeroMemory(&ProcInfo, sizeof(ProcInfo));

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	STARTUPINFO StartInfo;
	ZeroMemory(&StartInfo, sizeof(StartInfo));
	StartInfo.cb = sizeof(StartInfo);

	CString strTemp = pCommandLine;
	BOOL bCreateProcess = CreateProcess(
		NULL,//_In_opt_     LPCTSTR lpApplicationName,
		strTemp.GetBuffer(),//_Inout_opt_  LPTSTR lpCommandLine,
		NULL,//_In_opt_     LPSECURITY_ATTRIBUTES lpProcessAttributes,
		NULL,//_In_opt_     LPSECURITY_ATTRIBUTES lpThreadAttributes,
		FALSE,//_In_         BOOL bInheritHandles,
		0,//_In_         DWORD dwCreationFlags,
		NULL,//_In_opt_     LPVOID lpEnvironment,
		NULL,//_In_opt_     LPCTSTR lpCurrentDirectory,
		&StartInfo,//_In_         LPSTARTUPINFO lpStartupInfo,
		&ProcInfo//_Out_        LPPROCESS_INFORMATION lpProcessInformation
		);
	if (!bCreateProcess)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("bCreateProcess"), hr, TEXT("%s"), strTemp.GetString());
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	strTemp.Format(DUP_SOCK_EVENT_NAME, ProcInfo.dwProcessId);
	HANDLE hEvent = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		strTemp.GetString()//_In_opt_  LPCTSTR lpName
		);
	if (NULL == hEvent)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CHandle ahEvent(hEvent); hEvent = NULL;

	//////////////////////////////////////////////////////////////////////////
	CONST DWORD dwMilliseconds = 1000 * 60 * 3;
	HANDLE hEvents[] = { ahEvent.m_h, ProcInfo.hProcess};
	DWORD dwWait = WaitForMultipleObjects(
		_countof(hEvents),//_In_  DWORD nCount,
		hEvents,//_In_  const HANDLE *lpHandles,
		FALSE,//_In_  BOOL bWaitAll,
		dwMilliseconds//_In_  DWORD dwMilliseconds
		);
	if (WAIT_OBJECT_0 != dwWait)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("WaitForSingleObject=%d"), dwWait);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	strTemp.Format(DUP_SOCK_PIPE_NAME, ProcInfo.dwProcessId);
	HANDLE hPipe = CreateFile(
		strTemp.GetString(),//_In_      LPCTSTR lpFileName,
		GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
		0,//_In_      DWORD dwShareMode,
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		NULL//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hPipe)
	{
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("%s"), strTemp.GetString());
		LEAVE_BLOCK(0);
	}
	CHandle ahPipe(hPipe); hPipe = INVALID_HANDLE_VALUE;

	//////////////////////////////////////////////////////////////////////////
	WSAPROTOCOL_INFO ProtocolInfo = { 0 };
	int nWSADuplicateSocket = WSADuplicateSocket(
		ConnectionSocket,//_In_   SOCKET s,
		ProcInfo.dwProcessId,//_In_   DWORD dwProcessId,
		&ProtocolInfo//_Out_  LPWSAPROTOCOL_INFO lpProtocolInfo
		);
	if (SOCKET_ERROR == nWSADuplicateSocket)
	{
		LastError = WSAGetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("WSADuplicateSocket"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	DWORD NumberOfBytesWritten = 0;
	BOOL bWriteFile = WriteFile(
		ahPipe,//_In_         HANDLE hFile,
		&ProtocolInfo,//_In_         LPCVOID lpBuffer,
		sizeof(WSAPROTOCOL_INFO),//_In_         DWORD nNumberOfBytesToWrite,
		&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
		NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
		);
	if (!bWriteFile)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("WriteFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (sizeof(WSAPROTOCOL_INFO) != NumberOfBytesWritten)
	{
		hr = HRESULT_FROM_WIN32(ERROR_INCORRECT_SIZE);
		D_API_ERR(0, TEXT("WriteFile"), hr, TEXT("NumberOfBytesToWrite=%d, NumberOfBytesWritten=%d"), sizeof(WSAPROTOCOL_INFO), NumberOfBytesWritten);
		LEAVE_BLOCK(0);
	}

	BOOL bFlushFileBuffers = FlushFileBuffers(ahPipe);
	if (!bFlushFileBuffers)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("FlushFileBuffers"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	ahPipe.Close();
	ahEvent.Close();
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	if (ProcInfo.hProcess)
	{
		CloseHandle(ProcInfo.hProcess);
		ProcInfo.hProcess = NULL;
	}

	if (ProcInfo.hThread)
	{
		CloseHandle(ProcInfo.hThread);
		ProcInfo.hThread = NULL;
	}

	if (SUCCEEDED(hr))
	{
		closesocket(ConnectionSocket);
		ConnectionSocket = INVALID_SOCKET;
	}

	return hr;
}

HRESULT TargetProcessMain(pfnSocketProc_t pfnSocketProc)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	DWORD dwPid = GetCurrentProcessId();

	CString strTemp;
	strTemp.Format(DUP_SOCK_EVENT_NAME, dwPid);
	HANDLE hEvent = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		strTemp.GetString()//_In_opt_  LPCTSTR lpName
		);
	if (NULL == hEvent)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CHandle ahEvent(hEvent); hEvent = NULL;


	//////////////////////////////////////////////////////////////////////////
	strTemp.Format(DUP_SOCK_PIPE_NAME, dwPid);
	HANDLE hPipe = CreateNamedPipe(
		strTemp.GetString(),//_In_      LPCTSTR lpName,
		PIPE_ACCESS_INBOUND | FILE_FLAG_FIRST_PIPE_INSTANCE,//_In_      DWORD dwOpenMode,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,//_In_      DWORD dwPipeMode,
		PIPE_UNLIMITED_INSTANCES,//_In_      DWORD nMaxInstances,
		sizeof(WSAPROTOCOL_INFO) * 2,//_In_      DWORD nOutBufferSize,
		sizeof(WSAPROTOCOL_INFO) * 2,//_In_      DWORD nInBufferSize,
		0,//_In_      DWORD nDefaultTimeOut,
		NULL//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes
		);
	SetEvent(ahEvent);
	
	if (INVALID_HANDLE_VALUE == hPipe)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateNamedPipe"), hr, TEXT("%s"), strTemp.GetString());
		LEAVE_BLOCK(0);
	}
	CHandle ahPipe(hPipe); hPipe = INVALID_HANDLE_VALUE;

	//////////////////////////////////////////////////////////////////////////
	BOOL fConnected = ConnectNamedPipe(ahPipe, NULL);
	if (!fConnected)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		if (ERROR_PIPE_CONNECTED != LastError)
		{
			D_API_ERR(0, TEXT("ConnectNamedPipe"), hr, TEXT("%s"), strTemp.GetString());
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	WSAPROTOCOL_INFO protocol_info = { 0 };
	DWORD NumberOfBytesRead = 0;
	BOOL bReadFile = ReadFile(
		ahPipe,//_In_         HANDLE hFile,
		&protocol_info,//_Out_        LPVOID lpBuffer,
		sizeof(WSAPROTOCOL_INFO),//_In_         DWORD nNumberOfBytesToRead,
		&NumberOfBytesRead,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
		NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
		);

	FlushFileBuffers(ahPipe);
	DisconnectNamedPipe(ahPipe);
	ahPipe.Close();
	ahEvent.Close();

	if (!bReadFile)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("ReadFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (sizeof(WSAPROTOCOL_INFO) != NumberOfBytesRead)
	{
		hr = HRESULT_FROM_WIN32(ERROR_INCORRECT_SIZE);
		D_API_ERR(0, TEXT("ReadFile"), hr, TEXT("%d, %d"), sizeof(WSAPROTOCOL_INFO), NumberOfBytesRead);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	SOCKET connectionSocket = WSASocket(
		FROM_PROTOCOL_INFO,//_In_  int af,
		FROM_PROTOCOL_INFO,//_In_  int type,
		FROM_PROTOCOL_INFO,//_In_  int protocol,
		&protocol_info,//_In_  LPWSAPROTOCOL_INFO lpProtocolInfo,
		0,//_In_  GROUP g,
		0//_In_  DWORD dwFlags
		);
	if (INVALID_SOCKET == connectionSocket)
	{
		LastError = WSAGetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("WSASocket"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = pfnSocketProc((LPVOID)(connectionSocket)); //socket will be closed by pfnSocketProc
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("pfnSocketProc"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

