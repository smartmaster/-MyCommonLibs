#pragma once

#include <windows.h>

EXTERN_C
NTSTATUS WINAPI SmlWinErrorToNtStatus(DWORD winError);

EXTERN_C
DWORD WINAPI SmlNtStatusToWinError(NTSTATUS status);