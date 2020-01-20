#pragma once

#include "stdafx.h"
#include "ConstDef.h"
#include "IniDefine.h"

//////////////////////////////////////////////////////////////////////////
extern CAutoPtr<TINIFile>		g_pIniFileBin; //<sonmi01>2012-7-16 ###???
extern PROCESS_INFORMATION		g_pProcInfo[G_MAX_PROC_COUNT];
extern SERVICE_STATUS			g_ServiceStatus; 
extern SERVICE_STATUS_HANDLE	g_hServiceStatusHandle; 
extern HANDLE					g_hEventStop;


//////////////////////////////////////////////////////////////////////////
VOID WINAPI CAStartAppServiceMain( DWORD dwArgc, LPTSTR *lpszArgv );
VOID WINAPI CAStartAppServiceHandler( DWORD fdwControl );
