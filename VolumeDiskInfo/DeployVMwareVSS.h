#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


enum class GUEST_VSS_USE_METHOD
{
	UNKNOWN,
	DISKSHADOW,
	VSHADOW_32_WIN03,
	VSHADOW_64_WIN03,
	VSHADOW_32_VISTA,
	VSHADOW_64_VISTA,
};
GUEST_VSS_USE_METHOD DetectVssMethod();

//////////////////////////////////////////////////////////////////////////
HRESULT DeployVMwareVSS();
HRESULT UndeployVMwareVSS();
HRESULT CheckVMwareVssFiles(); //<sonmi01>2015-5-18 ###???