#include "stdafx.h"

#include <TlHelp32.h>
#include <Userenv.h>
#include <WtsApi32.h>

#include "CodeBlockDef.h"
#include "DebugLog.h"

HRESULT FindProcessOnSession(IN LPCWSTR pExeName, IN DWORD SessionID, OUT DWORD & ProcessID)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		HANDLE hSnapRaw = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapRaw)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"Failed to get process snapshot with error code [%u]", LastError);
			break;
		}
		CHandle ahSnap(hSnapRaw); hSnapRaw = INVALID_HANDLE_VALUE;

		//////////////////////////////////////////////////////////////////////////
		PROCESSENTRY32 ProcEntry = {0};
		ProcEntry.dwSize = sizeof(PROCESSENTRY32);
		BOOL bRet = Process32First(
			ahSnap,//__in     HANDLE hSnapshot,
			&ProcEntry//__inout  LPPROCESSENTRY32 lppe
			);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"Failed to start enumerating process with error code [%u]", LastError);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		do 
		{
			DWORD TempSessID = -1;
			if (0 == _wcsicmp(ProcEntry.szExeFile, pExeName))
			{
				TempSessID = -1;
				if (ProcessIdToSessionId(ProcEntry.th32ProcessID, &TempSessID) && TempSessID == SessionID)
				{
					hr = S_OK;
					ProcessID = ProcEntry.th32ProcessID;
					DEBUG_LOG(L"Found process [%s] on session [%u], process ID is [%d]", ProcEntry.szExeFile, SessionID, ProcessID);
					break;
				}
			}
		} while (Process32Next(ahSnap, &ProcEntry));

		//////////////////////////////////////////////////////////////////////////
	} while (FALSE);

	return hr;
}


HRESULT DuplicateProcessToken(IN DWORD ProcessID, OUT HANDLE & hProcessTokenDup)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		HANDLE hProcessRaw = OpenProcess(MAXIMUM_ALLOWED, FALSE, ProcessID);
		if (NULL == hProcessRaw)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"Failed to open process with error code [%u]", LastError);
			break;
		}
		CHandle ahProcess(hProcessRaw); hProcessRaw = NULL;

		//////////////////////////////////////////////////////////////////////////
		HANDLE hTempProcessToken = NULL;
		BOOL bRet = OpenProcessToken(
			ahProcess,
			TOKEN_ADJUST_PRIVILEGES |
			TOKEN_QUERY	|
			TOKEN_DUPLICATE |
			TOKEN_ASSIGN_PRIMARY |
			TOKEN_ADJUST_SESSIONID |
			TOKEN_READ |
			TOKEN_WRITE,
			&hTempProcessToken);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"Failed to OpenProcessToken with error code [%u]", LastError);
			break;
		}
		CHandle ahTempProcessToken(hTempProcessToken); hTempProcessToken = NULL;

		//////////////////////////////////////////////////////////////////////////
		HANDLE hTempTokenDup = NULL;
		bRet = DuplicateTokenEx(
			ahTempProcessToken,//__in      HANDLE hExistingToken,
			MAXIMUM_ALLOWED,//__in      DWORD dwDesiredAccess,
			NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpTokenAttributes,
			SecurityIdentification,//__in      SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,
			TokenPrimary,//__in      TOKEN_TYPE TokenType,
			&hTempTokenDup//__out     PHANDLE phNewToken
			);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"Failed to DuplicateTokenEx with error code [%u]", LastError);
			break;
		}

		hProcessTokenDup = hTempTokenDup;
		//////////////////////////////////////////////////////////////////////////
	} while (FALSE);

	return hr;
}


HRESULT AdjustProcessToken(IN HANDLE hToken, IN LPCWSTR pPrivilegeName, IN DWORD SessID)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		BOOL bRet = SetTokenInformation(hToken,TokenSessionId, (VOID*)&SessID, sizeof(DWORD));
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"Failed to Set TokenSessionId [%u] with error code [%u]", SessID, LastError);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		TOKEN_PRIVILEGES TokenPriv = {0};
		LUID PrivLuid = {0};
		bRet = LookupPrivilegeValue(NULL, pPrivilegeName, &PrivLuid);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"Failed to Set LookupPrivilegeValue [%s] with error code [%u]", pPrivilegeName, LastError);
			break;
		}
		TokenPriv.PrivilegeCount = 1;
		TokenPriv.Privileges[0].Luid = PrivLuid;
		TokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		//////////////////////////////////////////////////////////////////////////
		bRet = AdjustTokenPrivileges(
			hToken,//__in       HANDLE TokenHandle,
			FALSE,//__in       BOOL DisableAllPrivileges,
			&TokenPriv,//__in_opt   PTOKEN_PRIVILEGES NewState,
			sizeof(TOKEN_PRIVILEGES),//__in       DWORD BufferLength,
			NULL,//__out_opt  PTOKEN_PRIVILEGES PreviousState,
			NULL//__out_opt  PDWORD ReturnLength
			);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"Failed to Set AdjustTokenPrivileges [%s] with error code [%u]", pPrivilegeName, LastError);
			break;
		}


		//////////////////////////////////////////////////////////////////////////
	} while (FALSE);

	return hr;
}

HRESULT StartAppAsUser(HANDLE hToken, LPWSTR pCommandLine, LPCWSTR pCurrentDirectory, PPROCESS_INFORMATION pPI)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	PROCESS_INFORMATION ProcInfo = {0};

	STARTUPINFO StartupInfo = {0};
	StartupInfo.cb= sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_SHOW;
	StartupInfo.lpDesktop = NULL/*L"winsta0\\default"*/; //<sonmi01>2012-7-18 ###???

	LPVOID pEnv =NULL;
	DWORD dwCreationFlags = 0 /*NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE*/;
	//if(CreateEnvironmentBlock(&pEnv, hToken, TRUE))
	//{
	//	dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
	//}
	//else
	//{
	//	pEnv = NULL;
	//}

	BOOL bRet = CreateProcessAsUser(
		hToken,//__in_opt     HANDLE hToken,
		NULL,//__in_opt     LPCTSTR lpApplicationName,
		pCommandLine,//__inout_opt  LPTSTR lpCommandLine,
		NULL,//__in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
		NULL,//__in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
		FALSE,//__in         BOOL bInheritHandles,
		dwCreationFlags,//__in         DWORD dwCreationFlags,
		pEnv,//__in_opt     LPVOID lpEnvironment,
		pCurrentDirectory,//__in_opt     LPCTSTR lpCurrentDirectory,
		&StartupInfo,//__in         LPSTARTUPINFO lpStartupInfo,
		&ProcInfo//__out        LPPROCESS_INFORMATION lpProcessInformation
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DEBUG_LOG(L"Failed to CreateProcessAsUser [%s] with error code [%u]", pCommandLine, LastError);
	}
	else
	{
		if (NULL == pPI)
		{
			CloseHandle(ProcInfo.hProcess);
			CloseHandle(ProcInfo.hThread);
		}
		else
		{
			*pPI = ProcInfo;
		}
	}

	return hr;
}

static BOOL IsEmptyString(LPCTSTR pStr)
{
	return (NULL == pStr || 0 == pStr[0]);
}

static DWORD GetActiveSessionID()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	PWTS_SESSION_INFO pSessionInfo = NULL;
	DWORD Count = 0;
	DWORD SessID = -1;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	BOOL bRet = WTSEnumerateSessions(
		WTS_CURRENT_SERVER_HANDLE,//__in   HANDLE hServer,
		0,//__in   DWORD Reserved,
		1,//__in   DWORD Version,
		&pSessionInfo,//__out  PWTS_SESSION_INFO *ppSessionInfo,
		&Count//__out  DWORD *pCount
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DEBUG_LOG(TEXT("WTSEnumerateSessions failed with [0x%08x]"), hr);
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	for (DWORD ii = 0; ii < Count; ++ii)
	{
		if (WTSActive == pSessionInfo[ii].State)
		{
			SessID = pSessionInfo[ii].SessionId;
			break;
		}
	}

	END_BLOCK(0);


	if (pSessionInfo)
	{
		WTSFreeMemory(pSessionInfo); pSessionInfo = NULL;
	}
	

	return SessID;
}

HRESULT StartAppOnActiveSession(
								IN LPCWSTR pUsername,
								IN LPCWSTR pDomain,
								IN LPCWSTR pPassword,
								IN LPCWSTR pCommandLine,
								IN LPCWSTR pCurrentDirectory,
								OUT PPROCESS_INFORMATION pPI)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	do 
	{
		//DWORD SessID = WTSGetActiveConsoleSessionId();
		DWORD SessID = GetActiveSessionID();
		if (-1 == SessID)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"WTSGetActiveConsoleSessionId failed with error code [%u]", LastError);
			break;
		}

		CONST WCHAR WINLOGIN_NAME[] = L"winlogon.exe";
		DWORD ProcessID = -1;
		hr = FindProcessOnSession(WINLOGIN_NAME, SessID, ProcessID);
		if (FAILED(hr))
		{
			break;
		}

		HANDLE hTempDup = NULL;
		if (!IsEmptyString(pUsername) && !IsEmptyString(pPassword))
		{
			DEBUG_LOG(L"Use user and password");
			BOOL bRet = LogonUser(pUsername,
				pDomain,
				pPassword, 
				/*LOGON32_LOGON_INTERACTIVE*//*LOGON32_LOGON_SERVICE*/LOGON32_LOGON_BATCH,
				LOGON32_PROVIDER_DEFAULT,
				&hTempDup);
			if (!bRet)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError); 
				DEBUG_LOG(L"Failed to log on as user [%s\\%s], error code = %d", 
					pDomain,
					pUsername, 
					LastError); 
			}
		}
		else
		{
			DEBUG_LOG(L"Use winlogon token");
			hr = DuplicateProcessToken(ProcessID, hTempDup);
		}
		
		if (FAILED(hr))
		{
			break;
		}
		CHandle ahProcessTokenDup(hTempDup); hTempDup = NULL;

		
		hr = AdjustProcessToken(ahProcessTokenDup, SE_DEBUG_NAME, SessID);
		if (FAILED(hr))
		{
			break;
		}
				

		CString strCmdLine = pCommandLine;

		//{
		//	PROCESS_INFORMATION PITemp = { 0 };
		//	StartAppAsUser(nullptr, strCmdLine.GetBuffer(), pCurrentDirectory, &PITemp);
		//}

		hr = StartAppAsUser(ahProcessTokenDup, strCmdLine.GetBuffer(), pCurrentDirectory, pPI);
		if (FAILED(hr))
		{
			break;
		}

	} while (FALSE);

	return hr;
}