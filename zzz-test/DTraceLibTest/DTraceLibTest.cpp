// DTraceLibTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <lmerr.h>

#include <string>
using namespace std;

#include "..\..\PublicHeader\PublicDTraceLib.h"
#include "..\..\DTraceLib\ErrorResultStatus.h"


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

	D_SETTINGS(LEVEL_TRACE_VERBOSE_MAX, pCmdLine->bInXMl, pCmdLine->strPrefix.GetString(), pCmdLine->strFolder.GetString());

	D_START(LEVEL_TRACE_ERROR);

	D_INFO(LEVEL_TRACE_ERROR, TEXT("text='%s', number=%d, float=%f"), TEXT("hello..."), 1234, 5678.90);
	D_API_ERR(LEVEL_TRACE_ERROR, TEXT("FakeAPI"), E_OUTOFMEMORY, TEXT("text='%s', number=%d, float=%f"), TEXT("hello..."), 1234, 5678.90);

	D_SET_LAST_STATUS(ERROR_IO_PRIVILEGE_FAILED, -1);
	D_SET_LAST_ERROR_MESSAGE(TEXT("out of memory [%s, %d]"), TEXT("helloWorld"), 1234);

	for (int ii = 0; ii < 5; ++ ii)
	{
		D_API_ERR(0, TEXT("win32"), 2, TEXT(""));
		D_API_ERR(0, TEXT("win32hr"), HRESULT_FROM_WIN32(2), TEXT(""));
		D_API_ERR(0, TEXT("nerr"), NERR_BASE + 100, TEXT(""));
		D_API_ERR(0, TEXT("nerr_hr"), HRESULT_FROM_WIN32(NERR_BASE + 100), TEXT(""));
	}


	LONG hiStatusOut = 0;
	CString LastErrorMsg;
	DWORD dwLastStatus = D_GET_LAST_STATUS(&hiStatusOut);
	D_GET_LAST_ERROR_MESSAGE(LastErrorMsg);

	D_MESSAGE_BOX(TEXT("This is message box sample"));
	D_BREAK(TEXT("This is a break sample"));
	D_SLEEP(TEXT("This is a sleep sample"), 60, 1000);

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

int _tmain_ProcessGlobalData(int argc, _TCHAR* argv[])
{
	wstring Name = TEXT("1");
	HRESULT hr = SetProcessGlobalData(TEXT("1"), TEXT("aaaaaa"));
	hr = SetProcessGlobalData(TEXT("1"), TEXT("AAAAA"));

	Name = TEXT("2");
	hr = SetProcessGlobalData(TEXT("2"), TEXT("bbbbb"));

	TCHAR * pData = (TCHAR *)GetProcessGlobalData(TEXT("1"));
	pData = (TCHAR *)GetProcessGlobalData(TEXT("2"));
	pData = (TCHAR *)GetProcessGlobalData(TEXT("3"));


	pData = (TCHAR *)ClearProcessGlobalData(TEXT("1"));
	pData = (TCHAR *)ClearProcessGlobalData(TEXT("1"));
	pData = (TCHAR *)ClearProcessGlobalData(TEXT("2"));
	pData = (TCHAR *)ClearProcessGlobalData(TEXT("3"));


	return 0;

}


int _tmain_mt(int argc, _TCHAR* argv[])
{
	//return _tmain_ProcessGlobalData(argc, argv);

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


int _tmain(int argc, _TCHAR* argv[])
{
	//////////////////////////////////////////////////////////////////////////
	D_SETTINGS(
		999,//nDebugLevel, 
		TRUE,//bInXml, 
		TEXT("test---"),//strPrefix, 
		TEXT("")//strLogFolder
		);


	//////////////////////////////////////////////////////////////////////////
	ErrorResultStatus_NS::ErrorCollection errors;
	ERS_INIT_ADD_CURRENT_ERROR(errors, E_INVALIDARG, TEXT("E_INVALIDARG %d %s"), 123, TEXT("sss"));
	ERS_INIT_ADD_CURRENT_ERROR(errors, E_FAIL, TEXT("E_FAIL"));
	ERS_INIT_ADD_CURRENT_ERROR(errors, E_ABORT, TEXT("E_ABORT %s %I64d %s"), TEXT("hello"), 456LL, TEXT("world"));

	errors.ToFile(FIELD_NAME(ErrorResultStatus_NS:ErrorCollection), TEXT("hello.ErrorCollection.xml"));

	ErrorResultStatus_NS::ErrorCollection errorsCopy;
	errorsCopy.FromFile(FIELD_NAME(ErrorResultStatus_NS:ErrorCollection), TEXT("hello.ErrorCollection.xml"));


	//////////////////////////////////////////////////////////////////////////
	D_START(0);

	//////////////////////////////////////////////////////////////////////////
	D_INFO(0, TEXT("[%s, %I64d, %d, %.3f]"), TEXT("hello"), 1234LL, 5678, 9.123456);

	for (LONG ii = 0; ii < 1024; ++ ii)
	{
		D_API_ERR(0, TEXT("FakeAPI"), ERROR_INVALID_FUNCTION + ii, TEXT("[%s, %I64d, %d, %.3f]"), TEXT("hello"), 1234LL, 5678, 9.123456);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG StatusLow = 0;
	LONG StatusHi = 0;
	CString strLastErrorMessage;

	D_SET_LAST_STATUS(E_FAIL, ERROR_IO_PRIVILEGE_FAILED);
	D_SET_LAST_ERROR_MESSAGE(TEXT("[%s, %I64d, %d, %.3f]"), TEXT("hello"), 1234LL, 5678, 9.123456);

	StatusLow = D_GET_LAST_STATUS(&StatusHi);
	D_GET_LAST_ERROR_MESSAGE(strLastErrorMessage);

	D_SET_LAST_STATUS(E_UNEXPECTED, ERROR_BAD_REM_ADAP);
	D_SET_LAST_ERROR_MESSAGE(TEXT("[%s, %I64d, %d, %.3f]"), TEXT("hello new message"), 1234LL, 5678, 9.123456);

	StatusLow = D_GET_LAST_STATUS(&StatusHi);
	D_GET_LAST_ERROR_MESSAGE(strLastErrorMessage);

	//////////////////////////////////////////////////////////////////////////
	LONG FirstStatusLow = 0;
	LONG FirstStatusHi = 0;
	CString strFirstErrorMessage;

	D_SET_FISRT_STATUS(E_FAIL, ERROR_IO_PRIVILEGE_FAILED);
	D_SET_FISRT_ERROR_MESSAGE(TEXT("[%s, %I64d, %d, %.3f]"), TEXT("hello"), 1234LL, 5678, 9.123456);

	D_SET_FISRT_STATUS(E_UNEXPECTED, ERROR_BAD_REM_ADAP);
	D_SET_FISRT_ERROR_MESSAGE(TEXT("[%s, %I64d, %d, %.3f]"), TEXT("hello new message"), 1234LL, 5678, 9.123456);
	
	FirstStatusLow = D_GET_FIRST_STATUS(&FirstStatusHi);
	D_GET_FIRST_ERROR_MESSAGE(strFirstErrorMessage);

	D_RESET_FISRT_STATUS(0, 0);
	D_RESET_FISRT_ERROR_MESSAGE(TEXT(""));

	D_SET_FISRT_STATUS(E_UNEXPECTED, ERROR_BAD_REM_ADAP);
	D_SET_FISRT_ERROR_MESSAGE(TEXT("[%s, %I64d, %d, %.3f]"), TEXT("hello new message"), 1234LL, 5678, 9.123456);

	FirstStatusLow = D_GET_FIRST_STATUS(&FirstStatusHi);
	D_GET_FIRST_ERROR_MESSAGE(strFirstErrorMessage);

	//////////////////////////////////////////////////////////////////////////
	LONG errCount = D_API_ERR_COUNT();
	LONG code = 0;
	LPCTSTR str = nullptr;
	for (LONG ii = 0; ii < errCount; ++ ii)
	{
		code = D_API_ERR_CODE(ii);
		str = D_API_ERR_STRING(ii);
	}

	//////////////////////////////////////////////////////////////////////////
	CONST TCHAR __key[] = TEXT("key");
	HRESULT hr = D_SET_PGD(__key, new wstring(L"this is process global data"));

	wstring * p2 = new wstring(L"this is process global data #new");
	hr = D_SET_PGD(__key, p2);
	if (FAILED(hr))
	{
		delete p2;
		p2 = NULL;
	}

	wstring * pVal = (wstring *)D_GET_PGD(__key);

	wstring * pVal2 = (wstring *)D_CLEAR_PGD(__key);
	delete pVal2; pVal2 = NULL;

	wstring * pVal3 = (wstring *)D_GET_PGD(__key);
	//////////////////////////////////////////////////////////////////////////

	D_MESSAGE_BOX(TEXT("desc 1"));
	D_BREAK(TEXT("desc 2"));
	D_SLEEP(TEXT("desc 2"), -1, -1);

	//////////////////////////////////////////////////////////////////////////
	return 0;
}

