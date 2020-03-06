#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"
#include "ServiceManagerHelper.h"

//////////////////////////////////////////////////////////////////////////
HRESULT MyPulseService(CONST WCHAR* pName, INT nIndex)
{ 
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	// pulse the process with given index
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == 0) 
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("OpenSCManager"), hr, L"OpenSCManager failed, error code = %d", LastError);
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
		if (schService == 0) 
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("OpenService"), hr, L"OpenService failed, error code = %d", LastError);
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
					return S_OK;
				}
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("ControlService"), hr, L"ControlService failed, error code = %d", LastError);
			}
			else
			{
				D_API_ERR(0, TEXT("ControlService"), hr, L"Invalid argument to Pulse Process: %d", nIndex);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 
	}
	return hr;
}

HRESULT MyStopService(CONST WCHAR* pName)
{ 
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	// kill service with given name
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == 0) 
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("OpenSCManager"), hr, L"OpenSCManager failed, error code = %d", LastError);
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
		if (schService == 0) 
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("OpenService"), hr, L"OpenService failed, error code = %d", LastError);
		}
		else
		{
			// call ControlService to kill the given service
			SERVICE_STATUS status;
			if(ControlService(schService,SERVICE_CONTROL_STOP,&status))
			{
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager); 
				return S_OK;
			}
			else
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("ControlService"), hr, L"ControlService failed, error code = %d", LastError);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 
	}
	return hr;
}

HRESULT MyStartService(CONST WCHAR* pName, INT nArg, WCHAR** pArg)
{ 
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	// run service with given name
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == 0) 
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("OpenSCManager"), hr, L"OpenSCManager failed, error code = %d", LastError);
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
		if (schService == 0) 
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("OpenService"), hr, L"OpenService failed, error code = %d", LastError);
		}
		else
		{
			// call StartService to run the service
			if(StartService(schService,nArg,(CONST WCHAR**)pArg))
			{
				CloseServiceHandle(schService); 
				CloseServiceHandle(schSCManager); 
				return S_OK;
			}
			else
			{

				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("StartService"), hr, L"StartService failed, error code = %d", LastError);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager); 
	}
	return hr;
}



////////////////////////////////////////////////////////////////////// 
//
// Uninstall
//
HRESULT MyDeleteService(CONST WCHAR* pName)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager == 0) 
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("OpenSCManager"), hr, L"OpenSCManager failed, error code = %d", LastError);
	}
	else
	{
		SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_ALL_ACCESS);
		if (schService == 0) 
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("OpenService"), hr, L"OpenService failed, error code = %d", LastError);
		}
		else
		{
			if(!DeleteService(schService)) 
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("DeleteService"), hr, L"Failed to delete service %s, error code = %d", pName, LastError);
			}
			else 
			{
				D_INFO(0, L"Service %s removed",pName);
			}
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager);	
	}

	return hr;
}

////////////////////////////////////////////////////////////////////// 
//
// Install
//
HRESULT MyCreateService(CONST WCHAR* pPath, CONST WCHAR* pName, CONST WCHAR* lpDependencies)
{  
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE); 
	if (schSCManager == 0) 
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("OpenSCManager"), hr, L"OpenSCManager failed, error code = %d", LastError);
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
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("CreateService"), hr, L"Failed to create service %s, error code = %d", pName, LastError);
		}
		else
		{
			D_INFO(0, L"Service %s installed", pName);
			CloseServiceHandle(schService); 
		}
		CloseServiceHandle(schSCManager);
	}	

	return hr;
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
			D_API_ERR(0, TEXT("OpenSCManager"), hr, L"OpenSCManager failed with error [%d]", LastError);
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
			D_API_ERR(0, TEXT("OpenService"), hr, L"OpenService failed with error [%d]", LastError);
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
			D_API_ERR(0, TEXT("ChangeServiceConfig"), hr, L"ChangeServiceConfig start type [%s] failed with error [%d]", START_TYPE_NAME[StartType], LastError);
			break;
		}
		else
		{
			D_INFO(0, L"Service start type was changed to [%s] successfully.", START_TYPE_NAME[StartType]); 
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

HRESULT MyQueryService(LPCTSTR szSvcName, MyServiceConfig & svcConfig)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	SC_HANDLE schSCManager = nullptr;
	SC_HANDLE schService = nullptr;
	LPQUERY_SERVICE_CONFIG lpsc = nullptr;
	LPSERVICE_DESCRIPTION lpsd = nullptr;
	DWORD dwBytesNeeded = 0, cbBufSize = 0;
	
	do {
		// Get a handle to the SCM database. 

		schSCManager = OpenSCManager(
			NULL,                    // local computer
			NULL,                    // ServicesActive database 
			SC_MANAGER_ALL_ACCESS);  // full access rights 

		if (NULL == schSCManager)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("OpenSCManager"), hr, L"OpenSCManager failed (%d)", GetLastError());
			break;
		}

		// Get a handle to the service.

		schService = OpenService(
			schSCManager,          // SCM database 
			szSvcName,             // name of service 
			SERVICE_QUERY_CONFIG); // need query config access 
		if (schService == NULL)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("OpenService"), hr, L"OpenService failed (%d)", GetLastError());
			CloseServiceHandle(schSCManager);
			schSCManager = nullptr;
			break;
		}

		// Get the configuration information.

		if (!QueryServiceConfig(
			schService,
			NULL,
			0,
			&dwBytesNeeded))
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			if (ERROR_INSUFFICIENT_BUFFER == LastError)
			{
				cbBufSize = dwBytesNeeded;
				lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, cbBufSize);
			}
			else
			{
				D_API_ERR(0, TEXT("QueryServiceConfig"), hr, L"QueryServiceConfig failed (%d)", LastError);
				break;
			}
		}

		if (!QueryServiceConfig(
			schService,
			lpsc,
			cbBufSize,
			&dwBytesNeeded))
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("QueryServiceConfig"), hr, L"QueryServiceConfig failed (%d)", GetLastError());
			break;
		}
		else
		{
			hr = S_OK;
		}

		if (!QueryServiceConfig2(
			schService,
			SERVICE_CONFIG_DESCRIPTION,
			NULL,
			0,
			&dwBytesNeeded))
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			if (ERROR_INSUFFICIENT_BUFFER == LastError)
			{
				cbBufSize = dwBytesNeeded;
				lpsd = (LPSERVICE_DESCRIPTION)LocalAlloc(LMEM_FIXED, cbBufSize);
			}
			else
			{
				D_API_ERR(0, TEXT("QueryServiceConfig2"), hr, L"QueryServiceConfig2 failed (%d)", LastError);
				break;
			}
		}

		if (!QueryServiceConfig2(
			schService,
			SERVICE_CONFIG_DESCRIPTION,
			(LPBYTE)lpsd,
			cbBufSize,
			&dwBytesNeeded))
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("QueryServiceConfig2"), hr, L"QueryServiceConfig2 failed (%d)", GetLastError());
			break;
		}
		else
		{
			hr = S_OK;
		}

		// Print the configuration information.

		D_INFO(0, TEXT("%s configuration: "), szSvcName);
		svcConfig.szSvcName = szSvcName;
		D_INFO(0, TEXT("  Type: 0x%x"), lpsc->dwServiceType);
		svcConfig.dwServiceType = lpsc->dwServiceType;
		D_INFO(0, TEXT("  Start Type: 0x%x"), lpsc->dwStartType);
		svcConfig.dwStartType = lpsc->dwStartType;
		D_INFO(0, TEXT("  Error Control: 0x%x"), lpsc->dwErrorControl);
		svcConfig.dwErrorControl = lpsc->dwErrorControl;
		D_INFO(0, TEXT("  Binary path: %s"), lpsc->lpBinaryPathName);
		svcConfig.lpBinaryPathName = lpsc->lpBinaryPathName;
		D_INFO(0, TEXT("  Account: %s"), lpsc->lpServiceStartName);
		svcConfig.lpServiceStartName = lpsc->lpServiceStartName;

		if (lpsd->lpDescription != NULL && lstrcmp(lpsd->lpDescription, TEXT("")) != 0)
		{
			D_INFO(0, TEXT("  Description: %s"), lpsd->lpDescription);
			svcConfig.lpDescription = lpsd->lpDescription;
		}
			
		if (lpsc->lpLoadOrderGroup != NULL && lstrcmp(lpsc->lpLoadOrderGroup, TEXT("")) != 0)
		{
			D_INFO(0, TEXT("  Load order group: %s"), lpsc->lpLoadOrderGroup);
			svcConfig.lpLoadOrderGroup = lpsc->lpLoadOrderGroup;
		}
			
		if (lpsc->dwTagId != 0)
		{
			D_INFO(0, TEXT("  Tag ID: %d"), lpsc->dwTagId);
			svcConfig.dwTagId = lpsc->dwTagId;
		}
			
		if (lpsc->lpDependencies != NULL && lstrcmp(lpsc->lpDependencies, TEXT("")) != 0)
		{
			D_INFO(0, TEXT("  Dependencies: %s"), lpsc->lpDependencies);
			svcConfig.lpDependencies = lpsc->lpDependencies;
		}
			



		//////////////////////////////////////////////////////////////////////////
	} while (0);


	if (lpsc)
	{
		LocalFree(lpsc);
		lpsc = nullptr;
	}
	

	if (lpsd)
	{
		LocalFree(lpsd);
		lpsd = nullptr;
	}
	


	if (schService)
	{
		CloseServiceHandle(schService);
		schService = nullptr;
	}
	
	if (schSCManager)
	{
		CloseServiceHandle(schSCManager);
		schSCManager = nullptr;
	}
	

	return hr;


}

