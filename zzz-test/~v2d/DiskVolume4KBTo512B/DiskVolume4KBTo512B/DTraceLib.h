#pragma once

#include <tchar.h>
#include <windows.h>

#include <atlstr.h>
using ATL::CString;


#if defined(EXPORTED_DEBUG_LOG_DLLAPI_76E26A7E_D764_4685_BFD9_1A40D6A0AFF1)
#define DEBUG_LOG_DLLAPI __declspec(dllexport)
#else
#define DEBUG_LOG_DLLAPI __declspec(dllimport)
#endif


//////////////////////////////////////////////////////////////////////////
#ifndef BEGIN_BLOCK
#define BEGIN_BLOCK(LevelNumber)		do{ int __abc_xyz_##LevelNumber(LevelNumber); 
#define LEAVE_BLOCK(LevelNumber)		{(__abc_xyz_##LevelNumber);} break;
#define END_BLOCK(LevelNumber)			{(__abc_xyz_##LevelNumber);} }while(0);
#endif
//////////////////////////////////////////////////////////////////////////


enum ENUM_LEVEL_TRACE
{
	LEVEL_TRACE_ERROR = 0,
	LEVEL_TRACE_WARNING = 1,
	LEVEL_TRACE_INFOMATION = 2,
	LEVEL_TRACE_VERBOSE = 9,
	LEVEL_TRACE_VERBOSE_99 = 99,
	LEVEL_TRACE_VERBOSE_999 = 999,
	LEVEL_TRACE_VERBOSE_MAX = 9999
};


//******************************************************************************
class DEBUG_LOG_DLLAPI CDTraceStart
{
private:
	LPCTSTR		m_szEnding;
	LPCTSTR		m_szFunction;
	LPCTSTR		m_szFile;
	INT			m_line;
	INT			m_nDebuglevel;

public:
	//******************************************************************************
	CDTraceStart(
		INT nDebuglevel, LPCTSTR szEnding0, 
		LPCTSTR szFunction0, LPCTSTR szFile0, INT Line0
		);
	~CDTraceStart();
};

//******************************************************************************
DEBUG_LOG_DLLAPI VOID WINAPI DTraceSetDebugSettings(INT nDebugLevel, BOOL bInXml);

DEBUG_LOG_DLLAPI VOID WINAPIV DTraceInfo(
	INT nDebugLevel, LPCTSTR szEnding, 
	LPCTSTR szFunction, LPCTSTR szFile, INT line, 
	LPCTSTR szFormat = TEXT(""), ...
	);

DEBUG_LOG_DLLAPI VOID WINAPIV DTraceAPIErr(
	INT nDebugLevel, LPCTSTR szEnding, 
	LPCTSTR szFunction, LPCTSTR szFile, INT line, 
	LPCTSTR szApi, DWORD dwErrorCode, 
	LPCTSTR szFormat = TEXT(""), ...
	);

DEBUG_LOG_DLLAPI VOID WINAPI DTraceSetLastStatus(DWORD dwStatusLow, LONG nStatusHigh = 0);
DEBUG_LOG_DLLAPI VOID WINAPI DTraceSetLastErrorMessage(LPCTSTR pFormat, ...);

DEBUG_LOG_DLLAPI DWORD WINAPI DTraceGetLastStatus(LONG * pnStatusHigh = NULL);
DEBUG_LOG_DLLAPI VOID WINAPI DTraceGetLastErrorMessage(CString & strLastErrorMessage);

DEBUG_LOG_DLLAPI VOID DTraceMessageBoxImpl(LPCTSTR szText, LPCTSTR szFunction, LPCTSTR szFile, INT nLine);
DEBUG_LOG_DLLAPI VOID DTraceDebugBreakImpl(LPCTSTR szText, LPCTSTR szFunction, LPCTSTR szFile, INT nLine);
DEBUG_LOG_DLLAPI VOID DTraceDebugSleepImpl(LPCTSTR szText, ULONG nCount, ULONG nSleepTime, LPCTSTR szFunction, LPCTSTR szFile, INT nLine);

//******************************************************************************
#define DRN			TEXT("\r\n")
#define DPOS		TEXT(__FUNCTION__), TEXT(__FILE__),  __LINE__
#define DRNPOS		DRN, DPOS

//////////////////////////////////////////////////////////////////////////
//<my> use below macros only
#define DSETTINGS(nDebugLevel, bInXml)						DTraceSetDebugSettings(nDebugLevel, bInXml)

#define DSTART(nDebugLevel)									CDTraceStart DTraceStartObj_A8A44E3550974ADD8DA30B7E1B6C2F0E(nDebugLevel, DRNPOS)

#define	DINFO(Level, pFormat, ...)							DTraceInfo(Level, DRNPOS, pFormat, __VA_ARGS__)
#define DAPIERR(Level, szApi, dwErrorCode, szFormat, ...)	DTraceAPIErr(Level, DRNPOS, szApi, dwErrorCode, szFormat, __VA_ARGS__)

#define DSETLASTSTATUS(dwStatusLow, nStatusHigh)			DTraceSetLastStatus(dwStatusLow, nStatusHigh)
#define DSETLASTERRORMESSAGE(szFormat, ...)					DTraceSetLastErrorMessage(szFormat, __VA_ARGS__)

#define DGETLASTSTATUS(pnStatusHigh)						DTraceGetLastStatus(pnStatusHigh)
#define DGETLASTERRORMESSAGE(strLastErrorMessage)			DTraceGetLastErrorMessage(strLastErrorMessage)

#define DMESSAGEBOX(szText)									DTraceMessageBoxImpl(szText, DPOS)
#define DBREAK(szText)										DTraceDebugBreakImpl(szText, DPOS)
#define DSLEEP(szText, nCount, nSleepTime)					DTraceDebugSleepImpl(szText, nCount, nSleepTime, DPOS)
//////////////////////////////////////////////////////////////////////////

#define _OLD_DAPIERR(x)			DTraceAPIErr x
#define _OLD_DINFO(x)			DTraceInfo x
#define _OLD_DSETLASTSTATUS		DTraceSetLastStatus
//////////////////////////////////////////////////////////////////////////

//******************************************
#if 0 //example
static DWORD WINAPI ThreadProc(LPVOID)
{
	DSETTINGS(LEVEL_TRACE_VERBOSE_MAX, FALSE);
	
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

static HRESULT CheckError()
{
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	BEGIN_BLOCK(987);
	//******************************************
	BOOL ___api_error = TRUE;
	LPCTSTR ___api_error_varargs = NULL;

	if (___api_error)
	{
		dwLastError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwLastError);
		DSETLASTSTATUS(hr, dwLastError);
		DAPIERR(0, TEXT("___api_error_name"), hr, TEXT("___api_error_format"), ___api_error_varargs);
		LEAVE_BLOCK(987);
	}
	//******************************************


	//******************************************
	HRESULT ___com_error = E_FAIL;
	LPCTSTR ___com_error_varargs = NULL;

	if (FAILED(___com_error))
	{
		DSETLASTSTATUS(___com_error, -119);
		DAPIERR(0, TEXT("___com_error_name"), ___com_error, TEXT("___com_error_format"), ___com_error_varargs);
		LEAVE_BLOCK(987);
	}
	//******************************************
	END_BLOCK(987);
	return hr;
}
#endif
//******************************************

