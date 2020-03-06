// CAStartAppService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IniDefine.h"
#include "DebugLog.h"

#include "StartEndApp.h"
#include "ServiceHandlerAndMain.h"
#include "ServiceManager.h"
#include "RestartWorker.h"
#include "UtililityAndHelper.h"


#ifndef ___UNIT_TEST___

////////////////////////////////////////////////////////////////////// 
//
// Main Entry Point
//
#define CMD_CREATE			L"-create"
#define CMD_START			L"-start"
#define CMD_STOP			L"-stop"
#define CMD_CHANGE			L"-change"
#define CMD_DELETE			L"-delete"
#define CMD_PULSE			L"-pulse"

INT _tmain(INT argc, _TCHAR* argv[])
{
	// initialize variables for .EXE, .INI, and .LOG file names
	WCHAR pExeFile[G_BUFFER_SIZE+1] = {0};
	WCHAR pInitFile[G_BUFFER_SIZE+1] = {0};
	WCHAR pModuleFile[G_BUFFER_SIZE+1] = {0};

	DWORD dwSize = GetModuleFileName(NULL,pModuleFile,G_BUFFER_SIZE);
	pModuleFile[dwSize] = 0;
	if(dwSize > 4 && pModuleFile[dwSize-4] == L'.')
	{
		swprintf_s(pExeFile,L"%s",pModuleFile);
		pModuleFile[dwSize-4] = 0;
		swprintf_s(pInitFile,L"%s.ini",pModuleFile);
		swprintf_s(g_pLogFile,L"%s.log",pModuleFile);
	}
	else
	{
		fwprintf(stderr, L"Invalid module file name: %s\r\n", pModuleFile);
		return E_INVALIDARG;
	}

	DEBUG_LOG(L"------------------------------------Start------------------------------------");
	DEBUG_LOG(L"Executable: %s", pExeFile);
	DEBUG_LOG(L"INI: %s", pInitFile);
	DEBUG_LOG(L"Log: %s", g_pLogFile);

	// read configuration from .ini file
	g_pIniFileBin.Attach(new TINIFile);
	ReadIniFile(pInitFile, *g_pIniFileBin);
	DEBUG_LOG(L"Service Name: %s", g_pIniFileBin->SectionSettings.INIServiceName.GetString());

	// create / install service if switch is "-create"
	if(argc == 2 && _wcsicmp(CMD_CREATE,argv[1]) == 0)
	{			
		LPTSTR lpDependencies = NULL; //<sonmi01>2012-8-8 ###???
		if (g_pIniFileBin->SectionSettings.INIDependencies.GetLength())
		{
			QuotedStringsToBuffer(g_pIniFileBin->SectionSettings.INIDependencies.GetString(), &lpDependencies);
		}

		MyCreateService(pExeFile, g_pIniFileBin->SectionSettings.INIServiceName.GetString(), lpDependencies);

		if (lpDependencies)
		{
			FreeQuotedStringsBuffer(lpDependencies);
			lpDependencies = NULL;
		}
	}
	// start the service or a service with given name
	else if(argc >= 2 && _wcsicmp(CMD_START,argv[1]) == 0)
	{
		LPCWSTR pTempSevicename = (argc >= 3? argv[2] : g_pIniFileBin->SectionSettings.INIServiceName.GetString());
		if(MyStartService(pTempSevicename, 
			(argc > 3? (argc-3): 0), 
			(argc > 3? &argv[3] : NULL)))
		{
			DEBUG_LOG(L"Ran service %s", argv[2]);
		}
		else
		{
			DEBUG_LOG(L"Failed to run service %s", argv[2]);
		}
	}
	// stop the service or a service with given name
	else if(argc >= 2 && _wcsicmp(CMD_STOP,argv[1]) == 0)
	{
		LPCWSTR pTempSevicename = (argc == 3? argv[2] : g_pIniFileBin->SectionSettings.INIServiceName.GetString());
		if(MyStopService(pTempSevicename))
		{
			DEBUG_LOG(L"Stopped service %s", pTempSevicename);
		}
		else
		{
			DEBUG_LOG(L"Failed to stop service %s", pTempSevicename);
		}
	}
	// change service start type - remember AMD (vs. Intel)
	//-a automatic
	//-m manual
	//-d disable
	else if (argc >= 2 && _wcsicmp(CMD_CHANGE, argv[1]) == 0)
	{
		DWORD StartType = SERVICE_DEMAND_START; //default
		if (argc > 2)
		{
			StartType = TranslateStartType(argv[2]);
			if (-1 == StartType)
			{
				StartType = SERVICE_DEMAND_START; //default
			}
		}
		if (FAILED(MyChangeServiceStartType(g_pIniFileBin->SectionSettings.INIServiceName.GetString(), StartType)))
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
	// delete / Uninstall service if switch is "-delete"
	else if(argc == 2 && _wcsicmp(CMD_DELETE,argv[1]) == 0)
	{
		MyDeleteService(g_pIniFileBin->SectionSettings.INIServiceName.GetString());
	}
	// pulse service if switch is "-pulse"
	else if(argc == 2 && _wcsicmp(CMD_PULSE,argv[1]) == 0)
	{			
		MyStopService(g_pIniFileBin->SectionSettings.INIServiceName.GetString());
		MyStartService(g_pIniFileBin->SectionSettings.INIServiceName.GetString(),0,NULL);
	}
	// pulse a specific application if the index is supplied
	else if(argc == 3 && _wcsicmp(CMD_PULSE,argv[1]) == 0)
	{
		INT nIndex = _wtoi(argv[2]);
		if(MyPulseService(g_pIniFileBin->SectionSettings.INIServiceName.GetString(), nIndex))
		{
			DEBUG_LOG(L"Pulsed process %d", nIndex);
		}
		else
		{
			DEBUG_LOG(L"Failed to pulse process %d", nIndex);
		}
	}
	
	// assume user is starting this service 
	else 
	{
		//for (int ii=0; ii<300; ++ii)
		//{
		//	Sleep(100); //for debug purpose
		//}

		// start a worker thread to check for dead programs (and restart if necessary)
		g_hEventStop = CreateEvent(NULL, TRUE, FALSE, NULL); //worker thread is owner of the handle and will close it
		if (NULL == g_hEventStop)
		{
			DWORD LastError = GetLastError();
			DEBUG_LOG(L"CreateEvent for worker thread failed, error code = %d", LastError);
		}

		unsigned threadid = 0;
		HANDLE hThread = (HANDLE)_beginthreadex( // NATIVE CODE
			NULL,//void *security,
			0,//unsigned stack_size,
			RestartWorkerThread,//unsigned ( __stdcall *start_address )( void * ),
			NULL,//void *arglist,
			0,//unsigned initflag,
			&threadid//unsigned *thrdaddr 
			);
		if(NULL == hThread)
		{
			DWORD LastError = GetLastError();
			DEBUG_LOG(L"_beginthreadex failed, error code = %d", LastError);
		}
		else
		{
			CloseHandle(hThread); hThread = NULL;
		}

		// pass dispatch table to service controller
		SERVICE_TABLE_ENTRY   DispatchTable[] = 
		{ 
			{g_pIniFileBin->SectionSettings.INIServiceName.GetBuffer(), CAStartAppServiceMain}, 
			{NULL, NULL}
		};

		if(!StartServiceCtrlDispatcher(DispatchTable))
		{
			SetEvent(g_hEventStop);
			DWORD LastError = GetLastError();
			DEBUG_LOG(L"StartServiceCtrlDispatcher failed, error code = %d", LastError);
		}

		// never get here unless the service is shutdown
		Sleep(3000);
	}

	DEBUG_LOG(L"------------------------------------End------------------------------------\n\n");
	return 0;
}

#endif //#ifdef ___UNIT_TEST___