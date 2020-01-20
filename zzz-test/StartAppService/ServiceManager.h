#pragma once

#include "stdafx.h"

BOOL MyPulseService(CONST WCHAR* pName, INT nIndex) ;
BOOL MyStopService(CONST WCHAR* pName);
BOOL MyStartService(CONST WCHAR* pName, INT nArg, WCHAR** pArg);
VOID MyDeleteService(CONST WCHAR* pName);
VOID MyCreateService( CONST WCHAR* pPath, CONST WCHAR* pName,  CONST WCHAR* lpDependencies);
HRESULT MyChangeServiceStartType(CONST WCHAR * szSvcName, DWORD StartType);