#include "stdafx.h"

#include "DebugLog.h"

//////////////////////////////////////////////////////////////////////////
BOOL MyPulseService( CONST WCHAR* pName, INT nIndex )
{ 
	// pulse the process with given index
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == 0) 
	{
		DWORD LastError = GetLastError();
		DEBUG_LOG(L"OpenSCManager failed, error code = %d", LastError);
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
		if (schService == 0) 
		{
			DWORD LastError = GetLastError();
			DEBUG_LOG(L"OpenService failed, error code = %d", LastError); 
		}
		else
		{
			// call ControlService to invoke handler
			SERVICE_STATUS status;
			if(nIndex >= 0 && nIndex < 128)
			{
				if(ControlService(schService,(nIndex | 0x80),&status))
				{
					CloseServiceHandle(schService); 
					CloseServiceHandle(schSCManager); 
					return TRUE;
				}
				DWORD LastError = GetLastError();
				DEBUG_LOG(L"ControlService failed, error code = %d", LastError); 
			}
			else
			{
				DEBUG_LOG(L"Invalid argument to Pulse Process: %d", nIndex); 
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 
	}
	return FALSE;
}

BOOL MyStopService( CONST WCHAR* pName )
{ 
	// kill service with given name
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == 0) 
	{
		DWORD LastError = GetLastError();
		DEBUG_LOG(L"OpenSCManager failed, error code = %d", LastError);
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
		if (schService == 0) 
		{
			DWORD LastError = GetLastError();
			DEBUG_LOG(L"OpenService failed, error code = %d", LastError);
		}
		else
		{
			// call ControlService to kill the given service
			SERVICE_STATUS status;
			if(ControlService(schService,SERVICE_CONTROL_STOP,&status))
			{
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager); 
				return TRUE;
			}
			else
			{
				DWORD LastError = GetLastError();
				DEBUG_LOG(L"ControlService failed, error code = %d", LastError);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 
	}
	return FALSE;
}

BOOL MyStartService( CONST WCHAR* pName, INT nArg, WCHAR** pArg )
{ 
	// run service with given name
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == 0) 
	{
		DWORD LastError = GetLastError();
		DEBUG_LOG(L"OpenSCManager failed, error code = %d", LastError);
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
		if (schService == 0) 
		{
			DWORD LastError = GetLastError();
			DEBUG_LOG(L"OpenService failed, error code = %d", LastError);
		}
		else
		{
			// call StartService to run the service
			if(StartService(schService,nArg,(CONST WCHAR**)pArg))
			{
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager); 
				return TRUE;
			}
			else
			{
				DWORD LastError = GetLastError();
				DEBUG_LOG(L"StartService failed, error code = %d", LastError);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 
	}
	return FALSE;
}



////////////////////////////////////////////////////////////////////// 
//
// Uninstall
//
VOID MyDeleteService( CONST WCHAR* pName )
{
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == 0) 
	{
		DWORD LastError = GetLastError();
		DEBUG_LOG(L"OpenSCManager failed, error code = %d", LastError);
	}
	else
	{
		SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
		if (schService == 0) 
		{
			DWORD LastError = GetLastError();
			DEBUG_LOG(L"OpenService failed, error code = %d", LastError);
		}
		else
		{
			if(!DeleteService(schService)) 
			{
				DWORD LastError = GetLastError();
				DEBUG_LOG(L"Failed to delete service %s, error code = %d", pName, LastError);
			}
			else 
			{
				DEBUG_LOG(L"Service %s removed",pName);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager);	
	}
}

////////////////////////////////////////////////////////////////////// 
//
// Install
//
VOID MyCreateService( CONST WCHAR* pPath, CONST WCHAR* pName,  CONST WCHAR* lpDependencies)
{  
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE); 
	if (schSCManager == 0) 
	{
		DWORD LastError = GetLastError();
		DEBUG_LOG(L"OpenSCManager failed, error code = %d", LastError);
	}
	else
	{
		SC_HANDLE schService = CreateService
			( 
			schSCManager,	/* SCManager database      */ 
			pName,			/* name of service         */ 
			pName,			/* service name to display */ 
			SERVICE_ALL_ACCESS,        /* desired access          */ 
			SERVICE_WIN32_OWN_PROCESS/*|SERVICE_INTERACTIVE_PROCESS*/, /* service type            */ 
			SERVICE_AUTO_START,      /* start type              */ 
			SERVICE_ERROR_NORMAL,      /* error control type      */ 
			pPath,			/* service's binary        */ 
			NULL,                      /* no load ordering group  */ 
			NULL,                      /* no tag identifier       */ 
			lpDependencies,            /* no dependencies         */ 
			NULL,                      /* LocalSystem account     */ 
			NULL
			);                     /* no password             */ 
		if (schService == 0) 
		{
			DWORD LastError =  GetLastError();
			DEBUG_LOG(L"Failed to create service %s, error code = %d", pName, LastError);
		}
		else
		{
			DEBUG_LOG(L"Service %s installed", pName);
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager);
	}	
}


////////////////////////////////////////////////////////////////////// 
//
// change service start type
//
HRESULT MyChangeServiceStartType(CONST WCHAR * szSvcName, DWORD StartType)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	SC_HANDLE schSCManager = NULL; //TO CLEANUP
	SC_HANDLE schService = NULL; //TO CLEANUP


	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// Get a handle to the SCM database. 
		schSCManager = OpenSCManager( 
			NULL,                    // local computer
			NULL,                    // ServicesActive database 
			SC_MANAGER_ALL_ACCESS);  // full access rights 
		if (NULL == schSCManager) 
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"OpenSCManager failed with error [%d]", LastError);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		// Get a handle to the service.
		schService = OpenService( 
			schSCManager,            // SCM database 
			szSvcName,               // name of service 
			SERVICE_CHANGE_CONFIG);  // need change config access 
		if (schService == NULL)
		{ 
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"OpenService failed with error [%d]", LastError); 
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		// Change the service start type.
		CONST static LPCWSTR START_TYPE_NAME[] = 
		{
			L"SERVICE_BOOT_START",
			L"SERVICE_SYSTEM_START",
			L"SERVICE_AUTO_START",
			L"SERVICE_DEMAND_START",
			L"SERVICE_DISABLED",
		};
		if (!ChangeServiceConfig( 
			schService,        // handle of service 
			SERVICE_NO_CHANGE, // service type: no change 
			StartType,			// service start type 
			SERVICE_NO_CHANGE, // error control: no change 
			NULL,              // binary path: no change 
			NULL,              // load order group: no change 
			NULL,              // tag ID: no change 
			NULL,              // dependencies: no change 
			NULL,              // account name: no change 
			NULL,              // password: no change 
			NULL))            // display name: no change
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DEBUG_LOG(L"ChangeServiceConfig start type [%s] failed with error [%d]", START_TYPE_NAME[StartType], LastError); 
			break;
		}
		else
		{
			DEBUG_LOG(L"Service start type was changed to [%s] successfully.", START_TYPE_NAME[StartType]); 
		}
		

		//////////////////////////////////////////////////////////////////////////
	} while (FALSE);

	//////////////////////////////////////////////////////////////////////////
	if (schService)
	{
		CloseServiceHandle(schService); schService = NULL;
	}
	if (schSCManager)
	{
		CloseServiceHandle(schSCManager); schSCManager = NULL;
	}

	return hr;
}

