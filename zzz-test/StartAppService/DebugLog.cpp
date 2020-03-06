#include "stdafx.h"
#include "ConstDef.h"

//////////////////////////////////////////////////////////////////////////
WCHAR g_pLogFile[G_BUFFER_SIZE+1] = {0};
static CComAutoCriticalSection g_LogLock;

//////////////////////////////////////////////////////////////////////////
static VOID DebugWriteString(CONST WCHAR* pMsg)
{
	// write error or other information into log file
	DWORD pid = GetCurrentProcessId();
	DWORD tid = GetCurrentThreadId();
	SYSTEMTIME oT = {0};
	FILE* pLog = NULL;

	g_LogLock.Lock();
	__try
	{
		::GetLocalTime(&oT);
		errno_t err = _wfopen_s(&pLog, g_pLogFile, L"a");
		if (0  ==  err)
		{
			fwprintf(pLog, L"%04d-%02d-%02d %02d:%02d:%02d:%03d\t(%06u,%06u)(0x%04x,0x%04x)\t%s\n", 
				oT.wYear, oT.wMonth, oT.wDay, 
				oT.wHour, oT.wMinute, oT.wSecond, oT.wMilliseconds, 
				pid, tid, pid, tid, 
				pMsg); 
			fclose(pLog); pLog = NULL;
		}
	} 
	__except(EXCEPTION_EXECUTE_HANDLER) 
	{
	}
	g_LogLock.Unlock();
}


VOID DebugWriteStringV(CONST WCHAR * pFormat, ...)
{
	CString str;

	// format and write the data you were given
	va_list args;
	va_start(args, pFormat);
	str.FormatV(pFormat, args);
	va_end(args);

	DebugWriteString(str.GetString());
	return;
}