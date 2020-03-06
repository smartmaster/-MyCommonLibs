// DTraceLibTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\DTraceLib\DTraceLib.h"

struct CmdLine
{
	BOOL bInXMl;
	CString strPrefix;
	CString strFolder;

	VOID Parae(INT argc, TCHAR ** argv)
	{
		for (int ii = 0; ii < argc; ii++)
		{
			if (0 == _tcsicmp(argv[ii], TEXT("-1")))
			{
				++ ii;
				bInXMl = (TEXT('y') == argv[ii][0] || TEXT('Y') == argv[ii][0]);
			}
			else if (0 == _tcsicmp(argv[ii], TEXT("-2")))
			{
				++ ii;
				strPrefix = argv[ii];
			}
			else if (0 == _tcsicmp(argv[ii], TEXT("-3")))
			{
				++ ii;
				strFolder = argv[ii];
			}
		}
	}
};

static DWORD WINAPI ThreadProc(LPVOID pv)
{
	CmdLine * pCmdLine = (CmdLine *)(pv);

	DSETTINGS(LEVEL_TRACE_VERBOSE_MAX, pCmdLine->bInXMl, pCmdLine->strPrefix.GetString(), pCmdLine->strFolder.GetString());

	DSTART(LEVEL_TRACE_ERROR);

	DINFO(LEVEL_TRACE_ERROR, TEXT("text='%s', number=%d, float=%f"), TEXT("hello..."), 1234, 5678.90);
	DAPIERR(LEVEL_TRACE_ERROR, TEXT("FakeAPI"), E_OUTOFMEMORY, TEXT("text='%s', number=%d, float=%f"), TEXT("hello..."), 1234, 5678.90);

	DSETLASTSTATUS(ERROR_IO_PRIVILEGE_FAILED, -1);
	DSETLASTERRORMESSAGE(TEXT("out of memory [%s, %d]"), TEXT("helloWorld"), 1234);


	LONG hiStatusOut = 0;
	CString LastErrorMsg;
	DWORD dwLastStatus = DGETLASTSTATUS(&hiStatusOut);
	DGETLASTERRORMESSAGE(LastErrorMsg);

	DMESSAGEBOX(TEXT("This is message box sample"));
	DBREAK(TEXT("This is a break sample"));
	DSLEEP(TEXT("This is a sleep sample"), 60, 1000);

	return 0;
}

VOID PrintUsage()
{
	_ftprintf(stdout, TEXT("Usage:") TEXT("\r\n"));
	_ftprintf(stdout, TEXT("\t") TEXT("this.exe -1 <y|n> -2 [LogFilePrefix] -3 [LogFolder]") TEXT("\r\n"));
	_ftprintf(stdout, TEXT("Example:") TEXT("\r\n"));
	_ftprintf(stdout, TEXT("\t") TEXT("this.exe -1 y") TEXT("\r\n"));
	_ftprintf(stdout, TEXT("\t") TEXT("this.exe -1 n") TEXT("\r\n"));
	_ftprintf(stdout, TEXT("\t") TEXT("this.exe -1 y -2 test_ -3 F:\\temp") TEXT("\r\n"));
	_ftprintf(stdout, TEXT("\t") TEXT("this.exe -1 n -3 F:\\temp") TEXT("\r\n"));
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		PrintUsage();
		return E_INVALIDARG;
	}

	CmdLine cmdLine;
	cmdLine.Parae(argc, argv);

	INT count = 20 * 1024;
	while (count)
	{
		ThreadProc((LPVOID)&cmdLine);
		-- count;
	}
	return 0;
}

