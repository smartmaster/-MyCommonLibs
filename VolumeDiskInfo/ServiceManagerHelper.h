#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


HRESULT MyPulseService(CONST WCHAR* pName, INT nIndex) ;
HRESULT MyStopService(CONST WCHAR* pName);
HRESULT MyStartService(CONST WCHAR* pName, INT nArg, WCHAR** pArg);
HRESULT MyDeleteService(CONST WCHAR* pName);
HRESULT MyCreateService(CONST WCHAR* pPath, CONST WCHAR* pName, CONST WCHAR* lpDependencies);
HRESULT MyChangeServiceStartType(CONST WCHAR * szSvcName, DWORD StartType);

struct MyServiceConfig
{
	CString		szSvcName;
	DWORD		dwServiceType;
	DWORD		dwStartType;
	DWORD		dwErrorControl;
	CString		lpBinaryPathName;
	CString		lpServiceStartName;
	CString		lpDescription;
	CString		lpLoadOrderGroup;
	DWORD		dwTagId;
	CString		lpDependencies;

	MyServiceConfig() :
		dwServiceType(0),
		dwStartType(0),
		dwErrorControl(0),
		dwTagId(0)
	{
	}
};
HRESULT MyQueryService(LPCTSTR szSvcName, MyServiceConfig & svcConfig);
