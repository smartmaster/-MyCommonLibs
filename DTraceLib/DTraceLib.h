#pragma once

#include <tchar.h>
#include <windows.h>

#include <atlstr.h>
using ATL::CString;
//
//#include <string>
//using std::wstring;

#include "..\PublicHeader\PublicGetCurrentModuleName.h"

#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
#define __DECLSPEC_DLLAPI __declspec(dllexport)
#pragma message("Export Debug Trace API ...")
#else
#define __DECLSPEC_DLLAPI __declspec(dllimport)
#pragma message("Import Debug Trace API ...")
#endif


//////////////////////////////////////////////////////////////////////////
//#ifndef BEGIN_BLOCK
//#define BEGIN_BLOCK(LevelNumber)		do{ int __abc_xyz_##LevelNumber(LevelNumber); 
//#define LEAVE_BLOCK(LevelNumber)		{(__abc_xyz_##LevelNumber);} break;
//#define END_BLOCK(LevelNumber)			{(__abc_xyz_##LevelNumber);} }while(0);
//#endif
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


//////////////////////////////////////////////////////////////////////////
class __DECLSPEC_DLLAPI CDTraceStart
{
private:
	LPCTSTR		m_szEnding;
	LPCTSTR		m_szCloseEnding;
	LPCTSTR		m_szCurrentModule;
	LPCTSTR		m_szFunction;
	LPCTSTR		m_szFile;
	INT			m_line;
	INT			m_nDebuglevel;

public:
	//////////////////////////////////////////////////////////////////////////
	CDTraceStart(
		INT nDebuglevel, LPCTSTR szEnding0, LPCTSTR	szCloseEnding0, 
		LPCTSTR szCurrentModule0,
		LPCTSTR szFunction0, LPCTSTR szFile0, INT Line0
		);
	~CDTraceStart();
};

//////////////////////////////////////////////////////////////////////////
__DECLSPEC_DLLAPI VOID WINAPI DTraceSetDebugSettings(INT nDebugLevel, BOOL bInXml, LPCTSTR strPrefix, LPCTSTR strLogFolder, BOOL bDeleteOnClose);


__DECLSPEC_DLLAPI VOID WINAPIV DTraceRaw(INT nDebugLevel, LPCTSTR szEnding, 
					   LPCTSTR szFormat, ...
					   );

__DECLSPEC_DLLAPI VOID WINAPIV DTraceInfo(INT nDebugLevel, 
										LPCTSTR szEnding, 
										LPCTSTR szFunction, LPCTSTR szFile, INT line, 
										LPCTSTR szCurrentModule,
										LPCTSTR szFormat, ...
										);

__DECLSPEC_DLLAPI VOID WINAPIV DTraceAPIErr(INT nDebugLevel, LPCTSTR szEnding, 
											LPCTSTR szFunction, LPCTSTR szFile, INT line, 
											LPCTSTR szApi, DWORD dwErrorCode, 
											LPCTSTR szCurrentModule,
											LPCTSTR szFormat, ...
											) ;

__DECLSPEC_DLLAPI LONG WINAPI DTraceAPIErrQueueCount();
__DECLSPEC_DLLAPI LONG WINAPI DTraceAPIErrQueueCode(LONG index);
__DECLSPEC_DLLAPI LPCTSTR WINAPI DTraceAPIErrQueueString(LONG index);

//////////////////////////////////////////////////////////////////////////
__DECLSPEC_DLLAPI VOID WINAPI DTraceSetLastStatus(DWORD dwStatusLow, LONG nStatusHigh = 0);
__DECLSPEC_DLLAPI VOID WINAPI DTraceSetLastErrorMessage(LPCTSTR pFormat, ...);

__DECLSPEC_DLLAPI DWORD WINAPI DTraceGetLastStatus(LONG * pnStatusHigh = NULL);
__DECLSPEC_DLLAPI VOID WINAPI DTraceGetLastErrorMessage(CString & strLastErrorMessage);

//////////////////////////////////////////////////////////////////////////
__DECLSPEC_DLLAPI VOID WINAPI DTraceSetFirstStatus(DWORD dwStatusLow, LONG nStatusHigh = 0);
__DECLSPEC_DLLAPI VOID WINAPI DTraceSetFirstErrorMessage(LPCTSTR pFormat, ...);

__DECLSPEC_DLLAPI VOID WINAPI DTraceResetFirstStatus(DWORD dwStatusLow, LONG nStatusHigh);
__DECLSPEC_DLLAPI VOID WINAPI DTraceResetFirstErrorMessage(LPCTSTR szFormat, ...);

__DECLSPEC_DLLAPI DWORD WINAPI DTraceGetFirstStatus(LONG * pnStatusHigh);
__DECLSPEC_DLLAPI VOID WINAPI DTraceGetFirstErrorMessage(CString & strLastErrorMessage);

//////////////////////////////////////////////////////////////////////////
__DECLSPEC_DLLAPI VOID DTraceMessageBoxImpl(LPCTSTR szText, LPCTSTR szFunction, LPCTSTR szFile, INT nLine);
__DECLSPEC_DLLAPI VOID DTraceDebugBreakImpl(LPCTSTR szText, LPCTSTR szFunction, LPCTSTR szFile, INT nLine);
__DECLSPEC_DLLAPI VOID DTraceDebugSleepImpl(LPCTSTR szText, ULONG nCount, ULONG nSleepTime, LPCTSTR szFunction, LPCTSTR szFile, INT nLine);

//////////////////////////////////////////////////////////////////////////
__DECLSPEC_DLLAPI HRESULT SetProcessGlobalData(LPCTSTR Name, LPVOID pData);
__DECLSPEC_DLLAPI LPVOID GetProcessGlobalData(LPCTSTR Name);
__DECLSPEC_DLLAPI LPVOID ClearProcessGlobalData(LPCTSTR Name);


//////////////////////////////////////////////////////////////////////////
#define DRN			TEXT("\r\n")
#define DPOS		TEXT(__FUNCTION__), TEXT(__FILE__),  __LINE__
#define DRNPOS		DRN, DPOS
//#define DSPACE		TEXT("\t")
//#define DSPACEPOS	DSPACE, DPOS
//
//#define DMODULE(Level)	DTraceRaw(Level, DRN, TEXT("<!--%s-->"), GetCurrentModuleName())

//////////////////////////////////////////////////////////////////////////
//<my> BEGIN using below new macros only
#define D_SETTINGS(nDebugLevel, bInXml, strPrefix, strLogFolder)						DTraceSetDebugSettings(nDebugLevel, bInXml, strPrefix, strLogFolder, FALSE)
#define D_SETTINGS_EX(nDebugLevel, bInXml, strPrefix, strLogFolder, bDeleteOnClose)		DTraceSetDebugSettings(nDebugLevel, bInXml, strPrefix, strLogFolder, bDeleteOnClose)

//////////////////////////////////////////////////////////////////////////
#define D_START(nDebugLevel)											CDTraceStart DTraceStartObj_A8A44E3550974ADD8DA30B7E1B6C2F0E(nDebugLevel, DRN, DRN, GetCurrentModuleName(), DPOS)
#define D_INFO(Level, pFormat, ...)										DTraceInfo(Level, DRNPOS, GetCurrentModuleName(), pFormat, __VA_ARGS__)
#define D_API_ERR(Level, szApi, dwErrorCode, szFormat, ...)				DTraceAPIErr(Level, DRNPOS, szApi, dwErrorCode, GetCurrentModuleName(), szFormat, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////
#define D_API_ERR_COUNT()			DTraceAPIErrQueueCount();
#define D_API_ERR_CODE(index)		DTraceAPIErrQueueCode(index)
#define D_API_ERR_STRING(index)		DTraceAPIErrQueueString(index)

//////////////////////////////////////////////////////////////////////////
#define D_SET_LAST_STATUS(dwStatusLow, nStatusHigh)						DTraceSetLastStatus(dwStatusLow, nStatusHigh)
#define D_SET_LAST_ERROR_MESSAGE(szFormat, ...)							DTraceSetLastErrorMessage(szFormat, __VA_ARGS__)

#define D_GET_LAST_STATUS(pnStatusHigh)									DTraceGetLastStatus(pnStatusHigh)
#define D_GET_LAST_ERROR_MESSAGE(strLastErrorMessage)					DTraceGetLastErrorMessage(strLastErrorMessage)

//////////////////////////////////////////////////////////////////////////
#define D_SET_FISRT_STATUS(dwStatusLow, nStatusHigh)					DTraceSetFirstStatus(dwStatusLow, nStatusHigh)
#define D_SET_FISRT_ERROR_MESSAGE(szFormat, ...)						DTraceSetFirstErrorMessage(szFormat, __VA_ARGS__)

#define D_RESET_FISRT_STATUS(dwStatusLow, nStatusHigh)					DTraceResetFirstStatus(dwStatusLow, nStatusHigh)
#define D_RESET_FISRT_ERROR_MESSAGE(szFormat, ...)						DTraceResetFirstErrorMessage(szFormat, __VA_ARGS__)

#define D_GET_FIRST_STATUS(pnStatusHigh)								DTraceGetFirstStatus(pnStatusHigh)
#define D_GET_FIRST_ERROR_MESSAGE(strLastErrorMessage)					DTraceGetFirstErrorMessage(strLastErrorMessage)


//////////////////////////////////////////////////////////////////////////
#define D_SET_PGD(Name, pData)											SetProcessGlobalData(Name, pData)
#define D_GET_PGD(Name)													GetProcessGlobalData(Name)
#define D_CLEAR_PGD(Name)												ClearProcessGlobalData(Name)

//////////////////////////////////////////////////////////////////////////
#define D_MESSAGE_BOX(szText)											DTraceMessageBoxImpl(szText, DPOS)
#define D_BREAK(szText)													DTraceDebugBreakImpl(szText, DPOS)
#define D_SLEEP(szText, nCount, nSleepTime)								DTraceDebugSleepImpl(szText, nCount, nSleepTime, DPOS)

//</my> END using macros

//////////////////////////////////////////////////////////////////////////
#if 0 //example

int _tmain_example(int argc, _TCHAR* argv[])
{
	//////////////////////////////////////////////////////////////////////////
	D_SETTINGS(
		999,//nDebugLevel, 
		TRUE,//bInXml, 
		TEXT("test---"),//strPrefix, 
		TEXT("")//strLogFolder
		);

	//////////////////////////////////////////////////////////////////////////
	D_START(0);

	//////////////////////////////////////////////////////////////////////////
	D_INFO(0, TEXT("[%s, %I64d, %d, %.3f]"), TEXT("hello"), 1234LL, 5678, 9.123456);
	D_API_ERR(0, TEXT("FakeAPI"), ERROR_BAD_PROVIDER, TEXT("[%s, %I64d, %d, %.3f]"), TEXT("hello"), 1234LL, 5678, 9.123456);

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


static HRESULT CheckErrorExample()
{
	if (FAILED(hr)zzzzzz)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT(""), hr, TEXT("")zzzzzz);
		LEAVE_BLOCK(0);
	}

	if (!zzzzzz)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT(""), hr, TEXT("")zzzzzz);
		LEAVE_BLOCK(0);
	}


	if (!zzzzzz)
	{
		LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT(""), hr, TEXT("")zzzzzz);
		LEAVE_BLOCK(0);
	}
}

static HRESULT CheckErrorExample()
{
	if (FAILED(hr)zzzzzz)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT(""), hr, TEXT("")zzzzzz);
		LEAVE_BLOCK(0);
	}

	if (!zzzzzz)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT(""), hr, TEXT("")zzzzzz);
		LEAVE_BLOCK(0);
	}


	if (!zzzzzz)
	{
		LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT(""), hr, TEXT("")zzzzzz);
		LEAVE_BLOCK(0);
	}
}

static DWORD WINAPI ThreadProc(LPVOID bInXml)
{
	D_SETTINGS(LEVEL_TRACE_VERBOSE_MAX, (NULL != bInXml), TEXT("F:\\temp"));

	D_START(LEVEL_TRACE_ERROR);

	D_INFO(LEVEL_TRACE_ERROR, TEXT("text='%s', number=%d, float=%f"), TEXT("hello..."), 1234, 5678.90);
	D_API_ERR(LEVEL_TRACE_ERROR, TEXT("FakeAPI"), E_OUTOFMEMORY, TEXT("text='%s', number=%d, float=%f"), TEXT("hello..."), 1234, 5678.90);

	D_SET_LAST_STATUS(ERROR_IO_PRIVILEGE_FAILED, -1);
	D_SET_LAST_ERROR_MESSAGE(TEXT("out of memory [%s, %d]"), TEXT("helloWorld"), 1234);


	LONG hiStatusOut = 0;
	CString LastErrorMsg;
	DWORD dwLastStatus = D_GET_LAST_STATUS(&hiStatusOut);
	D_GET_LAST_ERROR_MESSAGE(LastErrorMsg);

	D_MESSAGE_BOX(TEXT("This is message box sample"));
	D_BREAK(TEXT("This is a break sample"));
	D_SLEEP(TEXT("This is a sleep sample"), 60, 1000);

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
		D_SET_LAST_STATUS(hr, dwLastError);
		D_API_ERR(0, TEXT("___api_error_name"), hr, TEXT("___api_error_format"), ___api_error_varargs);
		LEAVE_BLOCK(987);
	}
	//******************************************


	//******************************************
	HRESULT ___com_error = E_FAIL;
	LPCTSTR ___com_error_varargs = NULL;

	if (FAILED(___com_error))
	{
		D_SET_LAST_STATUS(___com_error, -119);
		D_API_ERR(0, TEXT("___com_error_name"), ___com_error, TEXT("___com_error_format"), ___com_error_varargs);
		LEAVE_BLOCK(987);
	}
	//******************************************
	END_BLOCK(987);
	return hr;
}
#endif
//******************************************




