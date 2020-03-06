#include "stdafx.h"
#include "IniDefine.h"
#include "DebugLog.h"

#include "StartEndApp.h"
#include "ServiceHandlerAndMain.h"
#include "ServiceManager.h"
#include "UtililityAndHelper.h"
#include "StartAppOnActiveSession.h"
#include "RunCount.h"

//////////////////////////////////////////////////////////////////////////
static VOID EndAllProcessHelper();
static VOID CloseAllProcessHelper();

//////////////////////////////////////////////////////////////////////////
CAutoPtr<TINIFile> g_pIniFileBin; 
PROCESS_INFORMATION g_pProcInfo[G_MAX_PROC_COUNT] = {0};

SERVICE_STATUS          g_ServiceStatus = {0}; 
SERVICE_STATUS_HANDLE   g_hServiceStatusHandle = NULL; 

HANDLE g_hEventStop = NULL;

////////////////////////////////////////////////////////////////////// 
//
// This routine responds to events concerning your service, like start/stop
//
VOID WINAPI CAStartAppServiceHandler(DWORD fdwControl)
{
	switch(fdwControl) 
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		g_ServiceStatus.dwWin32ExitCode = 0; 
		g_ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
		g_ServiceStatus.dwCheckPoint    = 0; 
		g_ServiceStatus.dwWaitHint      = 0;

		//////////////////////////////////////////////////////////////////////////
		if (g_hEventStop) //end worker thread
		{
			SetEvent(g_hEventStop);
		}

		// terminate all processes started by this service before shutdown
		{
			if (g_pIniFileBin->SectionSettings.INIEndProcessOnServiceStop[0] == L'Y' || 
				g_pIniFileBin->SectionSettings.INIEndProcessOnServiceStop[0] == L'y')
			{
				EndAllProcessHelper();
			}
			else
			{
				CloseAllProcessHelper();
			}
					
			if (!SetServiceStatus(g_hServiceStatusHandle, &g_ServiceStatus))
			{ 
				DWORD LastError = GetLastError();
				DEBUG_LOG(L"SetServiceStatus failed, error code = %d", LastError);
			}
		}
		return; 
	case SERVICE_CONTROL_PAUSE:
		g_ServiceStatus.dwCurrentState = SERVICE_PAUSED; 
		break;
	case SERVICE_CONTROL_CONTINUE:
		g_ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default: 
		// pulse processes started by this service
		if(fdwControl >= 128 && fdwControl < 256)
		{
			INT nIndex = fdwControl & 0x7F;
			// pulse a single process
			if(nIndex >= 0 && nIndex < G_MAX_PROC_COUNT)
			{
				EndProcess(nIndex);
				StartProcess(nIndex);
			}
			// pulse all processes
			else if(nIndex == 127)
			{
				for(INT ii= G_MAX_PROC_COUNT-1; ii >= 0; --ii)
				{
					EndProcess(ii);
				}
				for(INT ii=0; ii < G_MAX_PROC_COUNT; ++ii)
				{
					StartProcess(ii);
				}
			}
		}
		else
		{
			DWORD LastError = GetLastError();
			DEBUG_LOG(L"Unrecognized opcode %d", fdwControl);
		}
	};
	if (!SetServiceStatus(g_hServiceStatusHandle,  &g_ServiceStatus)) 
	{ 
		DWORD LastError = GetLastError();
		DEBUG_LOG(L"SetServiceStatus failed, error code = %d", LastError);
	} 
}

extern HRESULT __GetModulePath(HMODULE hMod, BOOL bDir, CString & strPath);

BOOLEAN IsWsP2VDone() 
{
	CString wsP2VDoneLog;
	TCHAR* logSubPath = TEXT("WS_P2V\\ws_p2v.xml");
	__GetModulePath(NULL, TRUE, wsP2VDoneLog);
	wsP2VDoneLog += logSubPath;
	WIN32_FIND_DATA find_data = {0};
	HANDLE hFind = FindFirstFile(wsP2VDoneLog, &find_data);
    if (hFind != INVALID_HANDLE_VALUE)
    {
		DEBUG_LOG(L"The ws_p2v service has finished its job.");
        ::FindClose(hFind);
        return TRUE;
    }
	return FALSE;
}
////////////////////////////////////////////////////////////////////// 
//
// This routine gets used to start your service
//
VOID WINAPI CAStartAppServiceMain( DWORD dwArgc, LPTSTR *lpszArgv )
{
	DWORD   status = 0; 
	DWORD   specificError = 0xfffffff; 

	g_ServiceStatus.dwServiceType        = SERVICE_WIN32; 
	g_ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
	g_ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE; 
	g_ServiceStatus.dwWin32ExitCode      = 0; 
	g_ServiceStatus.dwServiceSpecificExitCode = 0; 
	g_ServiceStatus.dwCheckPoint         = 0; 
	g_ServiceStatus.dwWaitHint           = 0; 

	g_hServiceStatusHandle = RegisterServiceCtrlHandler(
		g_pIniFileBin->SectionSettings.INIServiceName.GetString(), 
		CAStartAppServiceHandler); 
	if (g_hServiceStatusHandle == 0) 
	{
		DWORD LastError = GetLastError();
		DEBUG_LOG(L"RegisterServiceCtrlHandler failed, error code = %d", LastError);
		return; 
	} 

	// Initialization complete - report running status 
	g_ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
	g_ServiceStatus.dwCheckPoint         = 0; 
	g_ServiceStatus.dwWaitHint           = 0;  
	if(!SetServiceStatus(g_hServiceStatusHandle, &g_ServiceStatus)) 
	{ 
		DWORD LastError = GetLastError();
		DEBUG_LOG(L"SetServiceStatus failed, error code = %d", LastError);
	}

	//////////////////////////////////////////////////////////////////////////
	//change service start type
	if (g_pIniFileBin->SectionSettings.INIChangeToStartType.GetLength())
	{
		DWORD StartType = TranslateStartType(g_pIniFileBin->SectionSettings.INIChangeToStartType.GetString());
		if (-1 != StartType)
		{
			if (0 == DecrementRestartCount(2) //<sonmi01>2012-9-24 #the service start type will not be changed until restart count reaches 0
				|| IsWsP2VDone()) // Or ws_p2v service finished its job.
			{
				HRESULT hr = MyChangeServiceStartType(g_pIniFileBin->SectionSettings.INIServiceName.GetString(), StartType);
				if (FAILED(hr))
				{
					DEBUG_LOG(L"Failed to change service %s start type to %d", 
						g_pIniFileBin->SectionSettings.INIServiceName.GetString(), StartType);
				}
				else
				{
					DEBUG_LOG(L"Change service %s start type to %d", 
						g_pIniFileBin->SectionSettings.INIServiceName.GetString(), StartType);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//start applications
	BOOL bStartProcessInOrder = (L'Y' == g_pIniFileBin->SectionSettings.INIStartProcessInOrder[0] || L'y' == g_pIniFileBin->SectionSettings.INIStartProcessInOrder[0]);
	DWORD WaitPreviousProcessTimeOut = _wtoi(g_pIniFileBin->SectionSettings.INIWaitPreviousProcessTimeOut.GetString());
	WaitPreviousProcessTimeOut *= 1000;
	for(INT ii=0;ii < G_MAX_PROC_COUNT; ++ii)
	{
		g_pProcInfo[ii].hProcess = 0;
		StartProcess(ii);
		MonitorProcess(ii);
		if (bStartProcessInOrder && g_pProcInfo[ii].hProcess)
		{
			CONST INISectionProcess & SectionProcess = g_pIniFileBin->SectionProcessItems[ii];
			DWORD Ret = WaitForSingleObject(g_pProcInfo[ii].hProcess,  WaitPreviousProcessTimeOut);
			if (WAIT_OBJECT_0 == Ret)
			{
				DEBUG_LOG(L"Process end and go on to create next one. [%s] (%06d,%06d)(0x%04x,0x%04x)", 
					SectionProcess.INICommandLine.GetString(), 
					g_pProcInfo[ii].dwProcessId, g_pProcInfo[ii].dwThreadId, 
					g_pProcInfo[ii].dwProcessId, g_pProcInfo[ii].dwThreadId); 
			}
			else
			{
				DEBUG_LOG(L"Process do not end in [%d] MilSec and go on to create next one. [%s] (%06d,%06d)(0x%04x,0x%04x)", 
					WaitPreviousProcessTimeOut,
					SectionProcess.INICommandLine.GetString(), 
					g_pProcInfo[ii].dwProcessId, g_pProcInfo[ii].dwThreadId, 
					g_pProcInfo[ii].dwProcessId, g_pProcInfo[ii].dwThreadId); 
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
static VOID EndAllProcessHelper()
{
	for(INT ii=G_MAX_PROC_COUNT-1;ii >= 0;--ii)
	{
		EndProcess(ii);
	}
}

static VOID CloseAllProcessHelper()
{
	for(INT ii=G_MAX_PROC_COUNT-1;ii >= 0;--ii)
	{
		if(g_pProcInfo[ii].hProcess)
		{
			CONST INISectionProcess & SectionProcess = g_pIniFileBin->SectionProcessItems[ii];

			DEBUG_LOG(L"Close process [%s] (%06d,%06d)(0x%04x,0x%04x)", 
				SectionProcess.INICommandLine.GetString(), 
				g_pProcInfo[ii].dwProcessId, g_pProcInfo[ii].dwThreadId, 
				g_pProcInfo[ii].dwProcessId, g_pProcInfo[ii].dwThreadId); 

			__try
			{
				CloseHandle(g_pProcInfo[ii].hProcess);
				CloseHandle(g_pProcInfo[ii].hThread);
				ZeroMemory(&g_pProcInfo[ii], sizeof(PROCESS_INFORMATION));
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
			}
		}
	}
}