#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <WtsApi32.h>
#include <atlstr.h>
#include <atlsecurity.h>
#include <atlcoll.h>

#include "CodeBlock.h"
#include "log.h"


namespace Smartlib
{
	class StartAppOnActiveUserSession
	{
		private:
		static HRESULT FindProcessOnSession(IN LPCWSTR pExeName, IN DWORD SessionID, OUT DWORD& ProcessID)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			PWTS_PROCESS_INFO_EX  pProcessInfo = nullptr; /***CLEANUP***/
			DWORD Count = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			ProcessID = -1;
			DWORD Level = 1;
			BOOL ok = ::WTSEnumerateProcessesEx(
				WTS_CURRENT_SERVER_HANDLE,//_In_     HANDLE hServer,
				&Level,//_Inout_  DWORD * pLevel,
				SessionID,//_In_     DWORD SessionID,
				(LPTSTR*)& pProcessInfo,//_Out_    LPTSTR * ppProcessInfo,
				&Count//,//_Out_    DWORD * pCount
			);
			if (!ok)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSEnumerateProcesses"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			for (int ii = 0; ii < Count; ++ii)
			{
				if (0 == _tcsicmp(pExeName, pProcessInfo[ii].pProcessName))
				{
					ProcessID = pProcessInfo[ii].ProcessId;
					break;
				}
			}

			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			if (pProcessInfo)
			{
				::WTSFreeMemoryEx(WTSTypeProcessInfoLevel1, pProcessInfo, Count);
				pProcessInfo = nullptr;
			}
			
			return hr;
		}

		static HRESULT DuplicateProcessToken(IN DWORD ProcessID, OUT HANDLE& hProcessTokenDup)
		{

			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			
			//////////////////////////////////////////////////////////////////////////
			HANDLE hProcessRaw = OpenProcess(MAXIMUM_ALLOWED, FALSE, ProcessID);
			if (NULL == hProcessRaw)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(L"Failed to open process with error code [%u]", LastError);
				break;
			}
			CHandle ahProcess(hProcessRaw); hProcessRaw = NULL;

			//////////////////////////////////////////////////////////////////////////
			HANDLE hTempProcessToken = NULL;
			BOOL bRet = OpenProcessToken(
				ahProcess,
				TOKEN_ADJUST_PRIVILEGES |
				TOKEN_QUERY |
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
				SML_LOG_LINE(L"Failed to OpenProcessToken with error code [%u]", LastError);
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
				SML_LOG_LINE(L"Failed to DuplicateTokenEx with error code [%u]", LastError);
				break;
			}

			hProcessTokenDup = hTempTokenDup;
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;

			
		}


		static HRESULT AdjustProcessToken(IN HANDLE hToken, IN LPCWSTR pPrivilegeName, IN DWORD SessID)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			BOOL bRet = SetTokenInformation(hToken, TokenSessionId, (VOID*)& SessID, sizeof(DWORD));
			if (!bRet)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(L"Failed to Set TokenSessionId [%u] with error code [%u]", SessID, LastError);
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			TOKEN_PRIVILEGES TokenPriv = { 0 };
			LUID PrivLuid = { 0 };
			bRet = LookupPrivilegeValue(NULL, pPrivilegeName, &PrivLuid);
			if (!bRet)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(L"Failed to Set LookupPrivilegeValue [%s] with error code [%u]", pPrivilegeName, LastError);
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
				SML_LOG_LINE(L"Failed to Set AdjustTokenPrivileges [%s] with error code [%u]", pPrivilegeName, LastError);
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

		static HRESULT StartAppAsUser(HANDLE hToken, LPWSTR pCommandLine, LPCWSTR pCurrentDirectory, PPROCESS_INFORMATION pPI)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			PROCESS_INFORMATION ProcInfo = { 0 };

			STARTUPINFO StartupInfo = { 0 };
			StartupInfo.cb = sizeof(STARTUPINFO);
			StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
			StartupInfo.wShowWindow = SW_SHOW;
			StartupInfo.lpDesktop = NULL/*L"winsta0\\default"*/; //<sonmi01>2012-7-18 ###???

			LPVOID pEnv = NULL;
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
				SML_LOG_LINE(L"Failed to CreateProcessAsUser [%s] with error code [%u]", pCommandLine, LastError);
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
				SML_LOG_LINE(TEXT("WTSEnumerateSessions failed with [0x%08x]"), hr);
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


		public:
			static HRESULT StartAppOnActiveSession(
				IN LPCWSTR pUsername,
				IN LPCWSTR pDomain,
				IN LPCWSTR pPassword,
				IN LPCWSTR pCommandLine,
				IN LPCWSTR pCurrentDirectory,
				OUT PPROCESS_INFORMATION pPI)
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;
				
				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				CAccessToken at;
				bool atok = at.GetEffectiveToken(TOKEN_ALL_ACCESS);
				if (!atok)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("GetEffectiveToken"), LastError, hr);
				}
				else
				{
					CAtlArray< LPCTSTR > arr;
					arr.Add(SE_TCB_NAME);
					arr.Add(SE_DEBUG_NAME);
					arr.Add(SE_ASSIGNPRIMARYTOKEN_NAME);
					arr.Add(SE_ACTIVATE_AS_USER_CAPABILITY);
					arr.Add(SE_SESSION_IMPERSONATION_CAPABILITY);
					atok = at.EnablePrivileges(arr);
					if (!atok)
					{
						LastError = ::GetLastError();
						hr = HRESULT_FROM_WIN32(LastError);
						SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("EnablePrivileges"), LastError, hr);
					}
				}


				//////////////////////////////////////////////////////////////////////////
				//DWORD SessID = WTSGetActiveConsoleSessionId();
				DWORD SessID = GetActiveSessionID();
				if (-1 == SessID)
				{
					LastError = GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(L"WTSGetActiveConsoleSessionId failed with error code [%u]", LastError);
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
					SML_LOG_LINE(L"Use user and password");
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
						SML_LOG_LINE(L"Failed to log on as user [%s\\%s], error code = %d",
							pDomain,
							pUsername,
							LastError);
					}
				}
				else
				{
					SML_LOG_LINE(L"Use winlogon token");
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

				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);
				
				

				return hr;
			}



			static HRESULT Pause(LPCWSTR pCommandLine = TEXT("C:\\Windows\\System32\\notepad.exe"), DWORD millSec = 300 * 1000)
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;
				PROCESS_INFORMATION PI{ 0 }; /***CLEANUP***/

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				
				hr = Smartlib::StartAppOnActiveUserSession::StartAppOnActiveSession(
					nullptr,//IN LPCWSTR pUsername,
					nullptr,//IN LPCWSTR pDomain,
					nullptr,//IN LPCWSTR pPassword,
					pCommandLine,//IN LPCWSTR pCommandLine,
					nullptr,//IN LPCWSTR pCurrentDirectory,
					&PI//,//OUT PPROCESS_INFORMATION pPI
				);
				if (FAILED(hr))
				{
					LEAVE_BLOCK(0);
				}


				if (WAIT_TIMEOUT == ::WaitForSingleObject(PI.hProcess, millSec))
				{
					::TerminateProcess(PI.hProcess, ERROR_TIMEOUT);
				}
				
				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);


				if (PI.hThread)
				{
					::CloseHandle(PI.hThread);
				}
				if (PI.hProcess)
				{
					::CloseHandle(PI.hProcess);
				}
				ZeroMemory(&PI, sizeof(PROCESS_INFORMATION));
				
				return hr;

			}

	};
}