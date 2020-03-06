#include "stdafx.h"

#include "ServiceHandlerAndMain.h"
#include "IniDefine.h"
#include "DebugLog.h"

#include "StartEndApp.h"

unsigned WINAPI RestartWorkerThread(void* pParam)
{
	INT nCheckProcessSeconds = 0;
	INT nCheckProcess = _wtoi(g_pIniFileBin->SectionSettings.INICheckProcessMinutes.GetString());

	if(nCheckProcess > 0) 
	{
		nCheckProcessSeconds = nCheckProcess*60;
	}
	else
	{
		nCheckProcessSeconds = _wtoi(g_pIniFileBin->SectionSettings.INICheckProcessSeconds.GetString());
	}

	while(nCheckProcessSeconds > 0)
	{
		if (NULL == g_hEventStop)
		{
			::Sleep(1000*nCheckProcessSeconds);
		}
		else
		{
			DWORD WaitRet = WaitForSingleObject(g_hEventStop, 1000*nCheckProcessSeconds);
			if (WAIT_OBJECT_0 == WaitRet)
			{
				CloseHandle(g_hEventStop);
				g_hEventStop = NULL;
				break; //end thread
			}
		}

		for(INT ii=0;ii < G_MAX_PROC_COUNT; ++ii)
		{
			if(g_pProcInfo[ii].hProcess)
			{
				CONST INISectionProcess & SectionProcess = g_pIniFileBin->SectionProcessItems[ii];
				if(SectionProcess.INIRestart[0] == L'Y' || SectionProcess.INIRestart[0] == L'y')
				{
					DWORD dwCode;
					if(::GetExitCodeProcess(g_pProcInfo[ii].hProcess, &dwCode))
					{
						if(dwCode != STILL_ACTIVE)
						{
							__try // close handles to avoid ERROR_NO_SYSTEM_RESOURCES
							{
								::Sleep(1000);
								::CloseHandle(g_pProcInfo[ii].hThread);
								::CloseHandle(g_pProcInfo[ii].hProcess);
								ZeroMemory(&g_pProcInfo[ii], sizeof(PROCESS_INFORMATION));
							}
							__except(EXCEPTION_EXECUTE_HANDLER)
							{
							}

							Sleep(1000);
							if(StartProcess(ii))
							{
								MonitorProcess(ii);
								DEBUG_LOG(L"Restarted process %d, [%s]", ii, SectionProcess.INICommandLine.GetString());
							}
						}
					}
					else
					{
						DWORD LastError = GetLastError();
						DEBUG_LOG(L"GetExitCodeProcess failed, error code = %d", LastError);
					}
				}
			}
		}
	}

	return 0;
}