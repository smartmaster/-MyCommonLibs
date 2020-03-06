#include <windows.h>
#include <msi.h>
#include <msiquery.h>

#pragma comment(linker, "/EXPORT:CheckPID=_CheckPID@4")

extern "C" UINT __stdcall CheckPID (MSIHANDLE hInstall) {
  char Pid[MAX_PATH];
  DWORD PidLen = MAX_PATH;

  MsiGetProperty (hInstall, "PIDKEY", Pid, &PidLen);
  MsiSetProperty (hInstall, "PIDACCEPTED", Pid[0] == '1' ? "1" : "0");
  return ERROR_SUCCESS;
} // CheckPID
