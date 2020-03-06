#include "stdafx.h"


#include <tchar.h>
#include <strsafe.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlsecurity.h>

#include <stdlib.h>

namespace
{
	static const LPCTSTR CUSTOM_ACTION_DATA = TEXT("CustomActionData");
	static const LPCTSTR GLOBAL_ATOM_REBOOT = TEXT("ZETTA_ARCSERVE_UDP_DRIVER_GLOBAL_ATOM_REBOOT");

	static LPCTSTR GetPidTidUser(LPTSTR buffer, int bufferSize)
	{
		CAccessToken at;
		at.GetEffectiveToken(TOKEN_QUERY);
		CSid sid;
		at.GetUser(&sid);

		static const LPCTSTR format =
			TEXT("GetCurrentProcessId=[%d]") TEXT("\r\n")
			TEXT("GetCurrentThreadId=[%d]") TEXT("\r\n")
			TEXT("User=[%s\\%s]") TEXT("\r\n");
		_stprintf_s(buffer, bufferSize, format, ::GetCurrentProcessId(), ::GetCurrentThreadId(), sid.Domain(), sid.AccountName());
		return buffer;
	}
};

UINT __stdcall CustomActionRebootProp(
	MSIHANDLE hInstall
)
{
	TCHAR message[128] = { 0 };
	::MessageBox(nullptr, GetPidTidUser(message, _countof(message)), TEXT(__FUNCTION__), MB_OK);

	HRESULT hr = S_OK;
	UINT er = ERROR_SUCCESS;

	hr = WcaInitialize(hInstall, "CustomActionRebootProp");
	ExitOnFailure(hr, "Failed to initialize");

	WcaLog(LOGMSG_STANDARD, "Initialized.");

	// TODO: Add your custom action code here.
	//////////////////////////////////////////////////////////////////////////
	TCHAR buffer[512] = { 0 };
	DWORD buffer_len = _countof(buffer);
	DWORD msierr = ::MsiGetProperty(hInstall, TEXT("SmlRebootNeeded"), buffer, &buffer_len);

	//////////////////////////////////////////////////////////////////////////
	_tcscpy_s(buffer, TEXT("1"));
	msierr = ::MsiSetProperty(hInstall, TEXT("SmlRebootNeeded"), buffer);

	//////////////////////////////////////////////////////////////////////////
	buffer_len = _countof(buffer);
	buffer[0] = 0;
	msierr = ::MsiGetProperty(hInstall, TEXT("SmlRebootNeeded"), buffer, &buffer_len);

	ATOM atomReboot = ::GlobalFindAtom(GLOBAL_ATOM_REBOOT);
	if(0 != atomReboot)
	{
		::GlobalDeleteAtom(atomReboot);
		_stprintf_s(buffer, TEXT("%d"), ERROR_SUCCESS_REBOOT_REQUIRED);
		msierr = ::MsiSetProperty(hInstall, TEXT("SmlRebootNeeded"), buffer);

		buffer_len = _countof(buffer);
		buffer[0] = 0;
		msierr = ::MsiGetProperty(hInstall, TEXT("SmlRebootNeeded"), buffer, &buffer_len);
	}


LExit:
	er = SUCCEEDED(hr) ? ERROR_SUCCESS : ERROR_INSTALL_FAILURE;
	return WcaFinalize(er);
}

UINT __stdcall CustomActionReboot(
	MSIHANDLE hInstall
)
{

	TCHAR message[128] = { 0 };
	::MessageBox(nullptr, GetPidTidUser(message, _countof(message)), TEXT(__FUNCTION__), MB_OK);

	HRESULT hr = S_OK;
	UINT er = ERROR_SUCCESS;

	hr = WcaInitialize(hInstall, "CustomAction1");
	ExitOnFailure(hr, "Failed to initialize");

	WcaLog(LOGMSG_STANDARD, "Initialized.");

	// TODO: Add your custom action code here.
	TCHAR buffer[512] = { 0 };
	DWORD buffer_len = _countof(buffer);
	DWORD msierr = ::MsiGetProperty(hInstall, CUSTOM_ACTION_DATA, buffer, &buffer_len);

	_tcscpy_s(buffer, TEXT("1"));
	msierr = ::MsiSetProperty(hInstall, CUSTOM_ACTION_DATA, buffer);


	buffer_len = _countof(buffer);
	msierr = ::MsiGetProperty(hInstall, CUSTOM_ACTION_DATA, buffer, &buffer_len);
	LPCTSTR choise =
		TEXT("Do you want to set reboot flag in deffered action?") TEXT("\r\n")
		TEXT("\t") TEXT("YES - to reboot") TEXT("\r\n")
		TEXT("\t") TEXT("NO - not to reboot and continue installation") TEXT("\r\n")
		TEXT("\t") TEXT("CANCELL - to make an installation failure") TEXT("\r\n");
	int answer = ::MessageBox(nullptr, choise, TEXT(__FUNCTION__), MB_YESNOCANCEL);
	switch (answer)
	{
	case IDYES:
		::GlobalAddAtom(GLOBAL_ATOM_REBOOT);
		break;
	case IDNO:
		hr = S_OK;
		break;
	case IDCANCEL:
		hr = E_FAIL;
		break;
	}
	

LExit:
	er = SUCCEEDED(hr) ? ERROR_SUCCESS : ERROR_INSTALL_FAILURE;
	return WcaFinalize(er);
}


UINT __stdcall CustomAction1(
	MSIHANDLE hInstall
	)
{

	TCHAR message[128] = { 0 };
	::MessageBox(nullptr, GetPidTidUser(message, _countof(message)), TEXT(__FUNCTION__), MB_OK);

	HRESULT hr = S_OK;
	UINT er = ERROR_SUCCESS;

	hr = WcaInitialize(hInstall, "CustomAction1");
	ExitOnFailure(hr, "Failed to initialize");

	WcaLog(LOGMSG_STANDARD, "Initialized.");

	// TODO: Add your custom action code here.
	TCHAR buffer[512] = { 0 };
	DWORD buffer_len = _countof(buffer);
	::MsiGetProperty(hInstall, CUSTOM_ACTION_DATA, buffer, &buffer_len);


LExit:
	er = SUCCEEDED(hr) ? ERROR_SUCCESS : ERROR_INSTALL_FAILURE;
	return WcaFinalize(er);
}


// DllMain - Initialize and cleanup WiX custom action utils.
extern "C" BOOL WINAPI DllMain(
	__in HINSTANCE hInst,
	__in ULONG ulReason,
	__in LPVOID
	)
{
	switch(ulReason)
	{
	case DLL_PROCESS_ATTACH:
		WcaGlobalInitialize(hInst);
		break;

	case DLL_PROCESS_DETACH:
		WcaGlobalFinalize();
		break;
	}

	return TRUE;
}
