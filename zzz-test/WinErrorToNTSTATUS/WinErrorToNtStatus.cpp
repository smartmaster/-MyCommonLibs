// WinErrorToNTSTATUS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>

#pragma  warning(disable:4005)
#include <ntstatus.h>
#pragma  warning(default:4005)

#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>

#include "wine-dlls-ntdll-error.h"

int _tmain(int argc, TCHAR** argv)
{
	//////////////////////////////////////////////////////////////////////////
	NTSTATUS status = SmlWinErrorToNtStatus(ERROR_SXS_FILE_HASH_MISSING);
	ATLASSERT(status == STATUS_SXS_FILE_HASH_MISSING);

	DWORD winError = SmlNtStatusToWinError(STATUS_SXS_FILE_HASH_MISSING);
	ATLASSERT(winError == ERROR_SXS_FILE_HASH_MISSING);


	//////////////////////////////////////////////////////////////////////////
	status = SmlWinErrorToNtStatus(ERROR_WAIT_2);
	ATLASSERT(status == STATUS_WAIT_2);


	winError = SmlNtStatusToWinError(STATUS_WAIT_2);
	ATLASSERT(winError == ERROR_WAIT_2);


	//////////////////////////////////////////////////////////////////////////
	status = SmlWinErrorToNtStatus(ERROR_GEN_FAILURE);
	ATLASSERT(status == STATUS_UNSUCCESSFUL);


	winError = SmlNtStatusToWinError(STATUS_UNSUCCESSFUL);
	ATLASSERT(winError == ERROR_GEN_FAILURE);



	//////////////////////////////////////////////////////////////////////////
	status = SmlWinErrorToNtStatus(-2);
	ATLASSERT(status == STATUS_UNSUCCESSFUL);
    return 0;
}

