#include "stdafx.h"
#include "ServiceHandlerAndMain.h"
#include "IniDefine.h"
#include "DebugLog.h"
#include "StartAppOnActiveSession.h"
#include "CodeBlockDef.h"
#include "EncryptPassword.h"
#include "EncryptString.h"

// helper functions
BOOL StartProcess(INT nIndex) 
{ 
	// start a process with given index
	CONST INISectionProcess & IniSectionProcess = g_pIniFileBin->SectionProcessItems[nIndex];

	STARTUPINFO StartUpInfo = {0};
	StartUpInfo.cb= sizeof(STARTUPINFO);
	StartUpInfo.dwFlags = STARTF_USESHOWWINDOW;

	if(IniSectionProcess.INICommandLine.GetLength()  >  4)
	{
		BOOL bImpersonate = (IniSectionProcess.INIUserName.GetLength() > 0 && IniSectionProcess.INIPassword.GetLength() > 0);
		BOOL bUserInterface = (bImpersonate == FALSE) && 
			(IniSectionProcess.INIUserInterface[0] == L'y' || IniSectionProcess.INIUserInterface[0] == L'Y');
		
		// set the correct desktop for the process to be started
		WCHAR CurrentDesktopName[512] = {0};
		if(bUserInterface)
		{
			StartUpInfo.wShowWindow = SW_SHOW;
			StartUpInfo.lpDesktop = /*L"winsta0\\default"*/NULL;
		}
		else
		{
			HDESK hCurrentDesktop = GetThreadDesktop(GetCurrentThreadId());
			DWORD len = 0;
			GetUserObjectInformation(hCurrentDesktop,UOI_NAME,CurrentDesktopName,_countof(CurrentDesktopName),&len);
			StartUpInfo.wShowWindow = SW_HIDE;
			StartUpInfo.lpDesktop = (bImpersonate == FALSE)?CurrentDesktopName:L"";
		}	

		//////////////////////////////////////////////////////////////////////////
		if (IniSectionProcess.INIUserInterfaceOnActiveSession[0] == L'y' || IniSectionProcess.INIUserInterfaceOnActiveSession[0] == L'Y')
		{
			HRESULT hr = StartAppOnActiveSession(
				NULL,//IniSectionProcess.INIUserName.GetString(),
				NULL,//(IniSectionProcess.INIDomain.GetLength()? IniSectionProcess.INIDomain.GetString() : L"."),
				NULL,//IniSectionProcess.INIPassword.GetString(), 
				IniSectionProcess.INICommandLine.GetString(), 
				(IniSectionProcess.INICurrentDirectory.GetLength()? IniSectionProcess.INICurrentDirectory.GetString() : NULL),
				&g_pProcInfo[nIndex]);
			if (FAILED(hr))
			{
				DEBUG_LOG(L"Failed to start program [%s] on active session, result code = %d", 
					IniSectionProcess.INICommandLine.GetString(), hr); 
			}
			else
			{
				DEBUG_LOG(L"Start program [%s] on active session", 
					IniSectionProcess.INICommandLine.GetString()); 
			}

			return SUCCEEDED(hr);
		}
		else if(bImpersonate == FALSE)
		{
			// create the process
			if(CreateProcess(NULL,
				(WCHAR*)IniSectionProcess.INICommandLine.GetString(),
				NULL,NULL,TRUE,0,NULL, 
				(IniSectionProcess.INICurrentDirectory.GetLength()? IniSectionProcess.INICurrentDirectory.GetString() : NULL),
				&StartUpInfo,
				&g_pProcInfo[nIndex]))
			{
				DEBUG_LOG(L"Started program [%s], (%06d,%06d)(0x%04x,0x%04x)", 
					IniSectionProcess.INICommandLine.GetString(),
					g_pProcInfo[nIndex].dwProcessId, g_pProcInfo[nIndex].dwThreadId, 
					g_pProcInfo[nIndex].dwProcessId, g_pProcInfo[nIndex].dwThreadId); 

				Sleep(_wtoi(IniSectionProcess.INIPauseStart.GetString()));
				return TRUE;
			}
			else
			{
				DWORD LastError = GetLastError();
				DEBUG_LOG(L"Failed to start program [%s], error code = %d", 
					IniSectionProcess.INICommandLine.GetString(), LastError); 
				return FALSE;
			}
		}
		else
		{
			CString PlainUserPassword;
			if (L'y' == g_pIniFileBin->SectionSettings.INIEncryptionEnabled[0] || 
				L'Y' == g_pIniFileBin->SectionSettings.INIEncryptionEnabled[0])
			{
				CString StrEncPwd;
				GetEncPassword(StrEncPwd);
				wstring PlainText;
				DecryptString(StrEncPwd.GetString(), IniSectionProcess.INIPassword.GetString(), PlainText);
				PlainUserPassword = PlainText.c_str();
			}
			else
			{
				PlainUserPassword = IniSectionProcess.INIPassword.GetString();
			}
			HANDLE hToken = NULL;
			if(LogonUser(IniSectionProcess.INIUserName.GetString(),
				(IniSectionProcess.INIDomain.GetLength()? IniSectionProcess.INIDomain.GetString() : L"."),
				PlainUserPassword.GetString(), 
				/*LOGON32_LOGON_SERVICE*/LOGON32_LOGON_BATCH,
				LOGON32_PROVIDER_DEFAULT,
				&hToken))
			{
				if(CreateProcessAsUser(
					hToken,
					NULL,
					(WCHAR*)IniSectionProcess.INICommandLine.GetString(),
					NULL,NULL,TRUE,0,NULL, 
					(IniSectionProcess.INICurrentDirectory.GetLength()?IniSectionProcess.INICurrentDirectory.GetString():NULL),
					&StartUpInfo,
					&g_pProcInfo[nIndex]))
				{
					DEBUG_LOG(L"Started program [%s] as user [%s\\%s], (%06d,%06d)(0x%04x,0x%04x)", 
						IniSectionProcess.INICommandLine.GetString(), 
						IniSectionProcess.INIDomain.GetString(), IniSectionProcess.INIUserName.GetString(), 
						g_pProcInfo[nIndex].dwProcessId, g_pProcInfo[nIndex].dwThreadId, 
						g_pProcInfo[nIndex].dwProcessId, g_pProcInfo[nIndex].dwThreadId); 

					Sleep(_wtoi(IniSectionProcess.INIPauseStart.GetString()));
					return TRUE;
				}
				DWORD LastError = GetLastError();
				DEBUG_LOG(L"Failed to start program [%s] as user [%s], error code = %d", 
					IniSectionProcess.INICommandLine.GetString(), 
					IniSectionProcess.INIUserName.GetString(), 
					LastError); 
				return FALSE;
			}
			DWORD LastError = GetLastError();
			DEBUG_LOG(L"Failed to log on as user [%s], error code = %d", 
				IniSectionProcess.INIUserName.GetString(), 
				LastError); 
			return FALSE;
		}
	}
	else 
	{
		return FALSE;
	}
}



void EndProcess(INT nIndex) 
{	
	CONST INISectionProcess & SectionProcess = g_pIniFileBin->SectionProcessItems[nIndex];

	// end a program started by the service
	if(g_pProcInfo[nIndex].hProcess)
	{
		INT nPauseEnd = _wtoi(SectionProcess.INIPauseEnd.GetString());
		// post a WM_QUIT message first
		PostThreadMessage(g_pProcInfo[nIndex].dwThreadId,WM_QUIT,0,0);
		// sleep for a while so that the process has a chance to terminate itself
		::Sleep(nPauseEnd > 0? nPauseEnd : 50);
		// terminate the process by force
		TerminateProcess(g_pProcInfo[nIndex].hProcess,0);

		DEBUG_LOG(L"Stop process [%s] (%06d,%06d)(0x%04x,0x%04x)", 
			SectionProcess.INICommandLine.GetString(), 
			g_pProcInfo[nIndex].dwProcessId, 
			g_pProcInfo[nIndex].dwThreadId, 
			g_pProcInfo[nIndex].dwProcessId, 
			g_pProcInfo[nIndex].dwThreadId); 

		__try // close handles to avoid ERROR_NO_SYSTEM_RESOURCES
		{
			::CloseHandle(g_pProcInfo[nIndex].hThread);
			::CloseHandle(g_pProcInfo[nIndex].hProcess);
			ZeroMemory(&g_pProcInfo[nIndex], sizeof(PROCESS_INFORMATION));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}
}

DWORD WINAPI MonitorProcessWorker(LPVOID lpParameter)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	INT Index = (INT)(lpParameter);
	CONST INISectionProcess & SectionProcess = g_pIniFileBin->SectionProcessItems[Index];
	DWORD WaitResult = WaitForSingleObject(g_pProcInfo[Index].hProcess, INFINITE);
	if (WAIT_OBJECT_0 != WaitResult)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DEBUG_LOG(L"Failed to wait for process [%s] (%06d,%06d)(0x%04x,0x%04x), WaitResult=[%d], ErrorCode=[%d]", 
			SectionProcess.INICommandLine.GetString(), 
			g_pProcInfo[Index].dwProcessId, 
			g_pProcInfo[Index].dwThreadId, 
			g_pProcInfo[Index].dwProcessId, 
			g_pProcInfo[Index].dwThreadId,
			WaitResult, hr); 
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD dwExitCode = 0;
	BOOL bRet = GetExitCodeProcess(g_pProcInfo[Index].hProcess, &dwExitCode);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DEBUG_LOG(L"Failed to get process exit code [%s] (%06d,%06d)(0x%04x,0x%04x), ErrorCode=[%d]", 
			SectionProcess.INICommandLine.GetString(), 
			g_pProcInfo[Index].dwProcessId, 
			g_pProcInfo[Index].dwThreadId, 
			g_pProcInfo[Index].dwProcessId, 
			g_pProcInfo[Index].dwThreadId,
			hr); 
		break;
	}


	//////////////////////////////////////////////////////////////////////////
	if (0 == dwExitCode)
	{
		DEBUG_LOG(L"[++++++++++][%s]Process results (%06d,%06d)(0x%04x,0x%04x)", 
			SectionProcess.INICommandLine.GetString(), 
			g_pProcInfo[Index].dwProcessId, 
			g_pProcInfo[Index].dwThreadId, 
			g_pProcInfo[Index].dwProcessId, 
			g_pProcInfo[Index].dwThreadId
			); 
	}
	else
	{
		DEBUG_LOG(L"[----------][ExitCode=%d][%s]Process results (%06d,%06d)(0x%04x,0x%04x)", 
			dwExitCode,
			SectionProcess.INICommandLine.GetString(), 
			g_pProcInfo[Index].dwProcessId, 
			g_pProcInfo[Index].dwThreadId, 
			g_pProcInfo[Index].dwProcessId, 
			g_pProcInfo[Index].dwThreadId
			); 
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT MonitorProcess(INT Index)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST INISectionProcess & SectionProcess = g_pIniFileBin->SectionProcessItems[Index];
	if (NULL == g_pProcInfo[Index].hProcess)
	{
		break;
	}


	//////////////////////////////////////////////////////////////////////////
	DWORD ThreadId = 0;
	CHandle ahThread;
	ahThread.m_h = CreateThread(
		NULL,//__in_opt   LPSECURITY_ATTRIBUTES lpThreadAttributes,
		0,//__in       SIZE_T dwStackSize,
		MonitorProcessWorker,//__in       LPTHREAD_START_ROUTINE lpStartAddress,
		(LPVOID)(Index),//__in_opt   LPVOID lpParameter,
		0,//__in       DWORD dwCreationFlags,
		&ThreadId//__out_opt  LPDWORD lpThreadId
		);
	if (NULL == ahThread.m_h)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DEBUG_LOG(L"Failed to CreateThread MonitorProcessWorker for [%s] (%06d,%06d)(0x%04x,0x%04x), ErrorCode=[%d]", 
			SectionProcess.INICommandLine.GetString(), 
			g_pProcInfo[Index].dwProcessId, 
			g_pProcInfo[Index].dwThreadId, 
			g_pProcInfo[Index].dwProcessId, 
			g_pProcInfo[Index].dwThreadId,
			hr); 
		break;
	}
	END_BLOCK(0);

	return hr;
}