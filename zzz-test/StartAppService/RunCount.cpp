#include "stdafx.h"
#include "DebugLog.h"

HRESULT __GetModulePath(HMODULE hMod, BOOL bDir, CString & strPath)
{
	HRESULT hr = S_OK;
	DWORD	LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST INT BUFFER_SZIE = 1024;
	LPTSTR pBuffer = strPath.GetBuffer(BUFFER_SZIE);
	DWORD dwRet = GetModuleFileName(
		hMod,//_In_opt_  HMODULE hModule,
		pBuffer,//_Out_     LPTSTR lpFilename,
		BUFFER_SZIE//_In_      DWORD nSize
		);
	if (0 == dwRet)
	{
		strPath.ReleaseBuffer();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DEBUG_LOG(TEXT("GetModuleFileName fail with 0x%08x"), hr);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	if (bDir)
	{
		pBuffer = _tcsrchr(pBuffer, TEXT('\\'));
		if (pBuffer)
		{
			pBuffer[1] = 0;
		}
	}
	strPath.ReleaseBuffer();
	END_BLOCK(0);
	
	return hr;
}

static CONST TCHAR __INI_EXT[] = TEXT(".RestartCount.INI");

static CONST TCHAR __SECTION_NAME[] = TEXT("Settings");
static CONST TCHAR __RESTART_COUNT[] = TEXT("RestartCount");

static CONST INT __MAX_RESTART_COUNT = 32767;
static CONST TCHAR __MAX_RESTART_COUNT_STRING[] = TEXT("32767");

static INT __ReadRestartCount(LPCTSTR pIniFile)
{
	TCHAR Buffer[128] = {0};
	DWORD dwRet = GetPrivateProfileString(
		__SECTION_NAME,//_In_   LPCTSTR lpAppName,
		__RESTART_COUNT,//_In_   LPCTSTR lpKeyName,
		__MAX_RESTART_COUNT_STRING,//_In_   LPCTSTR lpDefault,
		Buffer,//_Out_  LPTSTR lpReturnedString,
		_countof(Buffer),//_In_   DWORD nSize,
		pIniFile//_In_   LPCTSTR lpFileName
		);

	INT RestartCount = _tcstol(Buffer, NULL, 10);

	return RestartCount;
}

static HRESULT __WriteRestartCount(LPCTSTR pIniFile, INT RestartCount)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	CString strRestartCount;
	strRestartCount.Format(TEXT("%d"), RestartCount);
	BOOL bRet = WritePrivateProfileString(
		__SECTION_NAME,//_In_  LPCTSTR lpAppName,
		__RESTART_COUNT,//_In_  LPCTSTR lpKeyName,
		strRestartCount.GetString(),//_In_  LPCTSTR lpString,
		pIniFile//_In_  LPCTSTR lpFileName
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
	}

	return hr;
}

INT DecrementRestartCount(INT DefaultRestartCount)
{
	CString strIni;
	__GetModulePath(NULL, FALSE, strIni);
	strIni += __INI_EXT;

	INT RestartCount = __ReadRestartCount(strIni.GetString());
	if (__MAX_RESTART_COUNT == RestartCount)
	{
		RestartCount = DefaultRestartCount;
	}
	else if (RestartCount > 0)
	{
		--RestartCount;
	}
	else
	{
		RestartCount = 0;
	}

	__WriteRestartCount(strIni.GetString(), RestartCount);

	return RestartCount;
}