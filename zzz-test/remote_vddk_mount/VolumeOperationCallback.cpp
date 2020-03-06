#include "stdafx.h"

#include <TlHelp32.h>
#include "VolumeOperationCallback.h"


DWORD WINAPI GetParentPID(PROCESSENTRY32& procentry);

HRESULT CVolumeChkdsk::Do(CONST vector<CString> & mountPoints, CONST vector<CString> & symbolLinks)
{
	for (CONST auto & var : mountPoints)
	{
		CHKDSK(var.GetString());
	}
	return S_OK;
}

HRESULT CVolumeChkdsk::CHKDSK(LPCTSTR volumePath)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////


	CONST LONG BUFFER_SIZE = 1024;
	CString str;
	LPTSTR pBuffer = str.GetBuffer(BUFFER_SIZE);
	GetModuleFileName(
		NULL,//_In_opt_  HMODULE hModule,
		pBuffer,//_Out_     LPTSTR lpFilename,
		BUFFER_SIZE//_In_      DWORD nSize
		);
	str.ReleaseBuffer();

	//cmd.exe /c "chkdsk m: >> rrr.txt"
	//cmd.exe /c \"chkdsk %s >> %s\"
	CString resultFile = str;
	resultFile += TEXT('.');
	resultFile += volumePath[0];

	PROCESSENTRY32 procentry = { 0 };
	DWORD ppid = GetParentPID(procentry);

	SYSTEMTIME SystemTime = { 0 };
	GetLocalTime(
		&SystemTime//_Out_  LPSYSTEMTIME lpSystemTime
		);
	resultFile.AppendFormat(TEXT(".%02d-%02d-%02d-%02d-%02d-%03d.ppid%d.log"),
		SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds, ppid);

	CString strVolume = volumePath;
	strVolume.TrimRight(TEXT("\\/"));

	CString strCmdLine;
	static CONST TCHAR CMD_LINE[] = TEXT("cmd.exe /c chkdsk %s >> \"%s\"");
	strCmdLine.Format(CMD_LINE, strVolume.GetString(), resultFile.GetString());

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));


	BOOL bCreateProcess = CreateProcess(NULL,   // No module name (use command line)
		strCmdLine.GetBuffer(),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi           // Pointer to PROCESS_INFORMATION structure
		);
	if (!bCreateProcess)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateProcess"), hr, TEXT("%s"), strCmdLine.GetString());
		LEAVE_BLOCK(0);
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}
