// DTraceLib.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "DTraceLib.h"
#include "ThreadLocalData.h"

namespace { //begin anonymous namespace

#define DETAIL_FORMAT1_BASE						TEXT("%04d-%02d%02d-%02d%02d%02d-%03d %04d.%04d(%04x.%04x)")
#define DETAIL_FORMAT1_NOTAG					DETAIL_FORMAT1_BASE TEXT("\t")
#define DETAIL_FORMAT1							TEXT("<!--") DETAIL_FORMAT1_BASE TEXT("-->\t")
#define DETAIL_TEXT1(SystemTime, pid, tid) 		SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond, SystemTime.wMilliseconds, pid, tid, pid, tid

#define DETAIL_FORMAT2_NOTAG					TEXT("[%s][%s][%d]")
#define DETAIL_FORMAT2							TEXT("<!--") TEXT("[%s][%s][%d]") TEXT("-->")
#define DETAIL_TEXT2(szFunction, szFile, line)	szFunction, szFile, line

#define TAG_INFO_START_NOTAG					TEXT("")
#define TAG_INFO_END_NOTAG						TEXT("\t")
#define TAG_INFO_START							TEXT("<I>")
#define TAG_INFO_END							TEXT("</I>\t")

#define TAG_ERROR_START_NOTAG					TEXT("[ERROR]")
#define TAG_ERROR_END_NOTAG						TEXT("\t")
#define TAG_ERROR_START							TEXT("<E>")
#define TAG_ERROR_END							TEXT("</E>\t")

#define TAG_LOG_ROOT_START_NOTAG				TEXT("START LOGGING\r\n")
#define TAG_LOG_ROOT_END_NOTAG					TEXT("END LOGGING")
#define TAG_LOG_ROOT_START						TEXT("<LogRoot>\r\n")
#define TAG_LOG_ROOT_END						TEXT("</LogRoot>")

CONST SIZE_T DTRACE_BUFFSZ = 2048;

//////////////////////////////////////////////////////////////////////////
static VOID AppendBackSlash(CString & str)
{
	str.TrimRight(TEXT("\\/"));
	str += TEXT('\\');;
}

static VOID RemoveRN(LPTSTR lpMsgBuf, DWORD dwMax)
{
	DWORD i = 0;
	while( lpMsgBuf && lpMsgBuf[i] && i<dwMax)
	{
		if( lpMsgBuf[i]==TEXT('\n') || lpMsgBuf[i]==TEXT('\r') ) 
		{ 
			lpMsgBuf[i] = 0; 
			break;
		}
		++i;
	}
}

static VOID WINAPI ReviseFunctionName(CString & strMessage)
{
	strMessage.Replace(TEXT("::~"), TEXT("..0"));
	strMessage.Replace(TEXT("::"), TEXT(".."));
}

static CTlsData * WINAPI GetTlsDataHelper()
{
	CTlsData * pTlsData = CTlsData::GetData(); 
	if (NULL == pTlsData)
	{
		CTlsData::LoadData(NULL);
		pTlsData = GetTlsDataHelper(); 
	}
	return pTlsData;
}

//////////////////////////////////////////////////////////////////////////
class CDTraceLogger 
{
private:
	IN INT m_nDebugLevel;
	IN BOOL m_bInXml;
	IN CString m_strPrefix;
	IN CString m_strLogFolder;

	ULONG m_FileSize; /***INTERNAL***/
	CString m_strFileName; /***INTERNAL***/
	HANDLE m_fout; /***INTERNAL TO CLEANUP***/
	CRITICAL_SECTION m_cs; /***INTERNAL TO CLEANUP***/

public:
	//////////////////////////////////////////////////////////////////////////
	CDTraceLogger(INT nDebugLevel, BOOL bInXml, LPCTSTR strPrefix, LPCTSTR strLogFolder);
	~CDTraceLogger();

	//////////////////////////////////////////////////////////////////////////
	VOID CreateMember();
	
	//////////////////////////////////////////////////////////////////////////
	INT GetDebugLevel();
	BOOL IsInXml();

	//////////////////////////////////////////////////////////////////////////
	BOOL WriteBuffer(LPCVOID buffer, DWORD size);
	BOOL WINAPIV WriteString(LPCTSTR szFormat, ...);

private:
	//////////////////////////////////////////////////////////////////////////
	BOOL WriteBufferNoLock( LPCVOID buffer, DWORD size );

	VOID GenerateNewLogFile(BOOL bFirstTime);
	VOID ResetNewLogFile();

	VOID DestroyMemeber();
};

//////////////////////////////////////////////////////////////////////////
class  CSIDTraceLogger 
{
public:
	static CDTraceLogger * CreateInstance(INT nDebugLevel, BOOL bInXml, LPCTSTR strPrefix, LPCTSTR strLogFolder);
	static CDTraceLogger * GetInstance();

private:
	static CAutoPtr<CDTraceLogger> s_pDTraceLogger;
	static CComAutoCriticalSection s_CriticalSection;
};

CAutoPtr<CDTraceLogger> CSIDTraceLogger::s_pDTraceLogger;
CComAutoCriticalSection CSIDTraceLogger::s_CriticalSection;

CDTraceLogger * CSIDTraceLogger::CreateInstance( INT nDebugLevel, BOOL bInXml, LPCTSTR strPrefix, LPCTSTR strLogFolder )
{
	if (!s_pDTraceLogger)
	{
		CComCritSecLock<CComAutoCriticalSection> Lock(s_CriticalSection, TRUE);
		if (!s_pDTraceLogger)
		{
			s_pDTraceLogger.Attach(new CDTraceLogger(nDebugLevel, bInXml, strPrefix, strLogFolder));
			s_pDTraceLogger->CreateMember();
		}
	}
	return s_pDTraceLogger;
}

CDTraceLogger * CSIDTraceLogger::GetInstance()
{
	if (!s_pDTraceLogger)
	{
		CComCritSecLock<CComAutoCriticalSection> Lock(s_CriticalSection, TRUE);
		return s_pDTraceLogger;
	}
	return s_pDTraceLogger;
}

#define SILOGGER	CSIDTraceLogger::GetInstance()

//////////////////////////////////////////////////////////////////////////
VOID CDTraceLogger::CreateMember()
{
	m_FileSize = 0;
	
	BOOL bRet = InitializeCriticalSectionAndSpinCount(&m_cs, 0x80001000);
	EnterCriticalSection(&m_cs);
	GenerateNewLogFile(TRUE);
	LeaveCriticalSection(&m_cs);
}


VOID CDTraceLogger::DestroyMemeber()
{
	if (m_bInXml)
	{
		static CONST TCHAR szLogRoot[] = TAG_LOG_ROOT_END;
		WriteBuffer(szLogRoot, sizeof(szLogRoot)-sizeof(TCHAR));
	}
	else
	{
		static CONST TCHAR szLogRoot[] = TAG_LOG_ROOT_END_NOTAG;
		WriteBuffer(szLogRoot, sizeof(szLogRoot)-sizeof(TCHAR));
	}


	FlushFileBuffers(m_fout);

	BOOL bDeleteEmptyLogFile = FALSE;
	LARGE_INTEGER liFileSize = {0};
	if (GetFileSizeEx(m_fout, &liFileSize))
	{
		if (m_bInXml)
		{
			static CONST TCHAR szEmptyLog[] = TAG_LOG_ROOT_START TEXT("1234567890ABCDEF") TAG_LOG_ROOT_END;
			bDeleteEmptyLogFile = (liFileSize.QuadPart < sizeof(szEmptyLog));
		}
		else
		{
			static CONST TCHAR szEmptyLog[] = TAG_LOG_ROOT_START_NOTAG TEXT("1234567890ABCDEF") TAG_LOG_ROOT_END_NOTAG;
			bDeleteEmptyLogFile = (liFileSize.QuadPart < sizeof(szEmptyLog));
		}
	}

	CloseHandle(m_fout);
	m_fout = INVALID_HANDLE_VALUE;

	if (bDeleteEmptyLogFile)
	{
		::DeleteFile(m_strFileName.GetString());
		m_strFileName.Empty();
	}

	DeleteCriticalSection(&m_cs);
}

VOID CDTraceLogger::ResetNewLogFile()
{
	//////////////////////////////////////////////////////////////////////////
	if (m_bInXml)
	{
		static CONST TCHAR szLogRootEnd[] = TAG_LOG_ROOT_END;
		WriteBufferNoLock(szLogRootEnd, sizeof(szLogRootEnd)-sizeof(TCHAR));
	}
	else
	{
		static CONST TCHAR szLogRootEnd[] = TAG_LOG_ROOT_END_NOTAG;
		WriteBufferNoLock(szLogRootEnd, sizeof(szLogRootEnd)-sizeof(TCHAR));
	}
	
	FlushFileBuffers(m_fout);

	CloseHandle(m_fout);
	m_fout = INVALID_HANDLE_VALUE;

	//////////////////////////////////////////////////////////////////////////
	m_FileSize = 0;
	GenerateNewLogFile(FALSE);
}

//////////////////////////////////////////////////////////////////////////
CDTraceLogger::	CDTraceLogger(INT nDebugLevel, BOOL bInXml, LPCTSTR strPrefix, LPCTSTR strLogFolder) :
m_nDebugLevel(nDebugLevel),
m_bInXml(bInXml),
m_strPrefix(strPrefix),
m_strLogFolder(strLogFolder),
m_FileSize(0),
m_fout(INVALID_HANDLE_VALUE)
{
}

//////////////////////////////////////////////////////////////////////////
CDTraceLogger::~CDTraceLogger()
{
	DestroyMemeber();
}

//////////////////////////////////////////////////////////////////////////
INT CDTraceLogger::GetDebugLevel()
{
	return m_nDebugLevel;
}

//////////////////////////////////////////////////////////////////////////
BOOL CDTraceLogger::WriteBuffer( LPCVOID buffer, DWORD size )
{
	static CONST ULONG MAX_FILE_SIZE = 10 * 1024 * 1024 + 3 * 1024;

	DWORD dwNumberOfBytesWritten = 0;
	EnterCriticalSection(&m_cs);
	BOOL bWriteFile = WriteFile(
		m_fout, //__in          HANDLE hFile,
		buffer,//__in          LPCVOID lpBuffer,
		size,//__in          DWORD nNumberOfBytesToWrite,
		&dwNumberOfBytesWritten,//__out         LPDWORD lpNumberOfBytesWritten,
		NULL//__in          LPOVERLAPPED lpOverlapped
		);

	m_FileSize += dwNumberOfBytesWritten;
	if (m_FileSize >= MAX_FILE_SIZE)
	{
		m_FileSize = 0;
		ResetNewLogFile();
	}

	LeaveCriticalSection(&m_cs);
	return bWriteFile;
}

//////////////////////////////////////////////////////////////////////////
BOOL CDTraceLogger::WriteBufferNoLock( LPCVOID buffer, DWORD size )
{
	DWORD dwNumberOfBytesWritten = 0;
	BOOL bWriteFile = WriteFile(
		m_fout, //__in          HANDLE hFile,
		buffer,//__in          LPCVOID lpBuffer,
		size,//__in          DWORD nNumberOfBytesToWrite,
		&dwNumberOfBytesWritten,//__out         LPDWORD lpNumberOfBytesWritten,
		NULL//__in          LPOVERLAPPED lpOverlapped
		);
	return bWriteFile;
}

//////////////////////////////////////////////////////////////////////////
BOOL WINAPIV CDTraceLogger::WriteString(LPCTSTR szFormat, ...)
{
	CTlsData * pTlsData = GetTlsDataHelper();

	va_list args;
	va_start(args, szFormat);
	pTlsData->m_strMessage.FormatV(szFormat, args);
	va_end(args);
	return WriteBuffer((LPCVOID)pTlsData->m_strMessage.GetString(), pTlsData->m_strMessage.GetLength()*sizeof(TCHAR));		
}

VOID CDTraceLogger::GenerateNewLogFile( BOOL bFirstTime )
{
	LPTSTR pBuffer = m_strFileName.GetBuffer(DTRACE_BUFFSZ);
	GetModuleFileName(
		NULL,//__in          HMODULE hModule,
		pBuffer,//__out         LPTSTR lpFilename,
		DTRACE_BUFFSZ//__in          DWORD nSize
		);

	LPTSTR pTemp = _tcsrchr(pBuffer, TEXT('\\'));
	CString strExeName = pTemp + 1;

	pTemp[1] = 0;
	m_strFileName.ReleaseBuffer();
	m_strFileName += TEXT("logs\\");


	CONST INT MIN_PATH_LEN = sizeof("C:\\1");
	if (m_strLogFolder.GetLength() >= MIN_PATH_LEN)
	{
		AppendBackSlash(m_strLogFolder);
		if (bFirstTime)
		{
			CreateDirectory(m_strLogFolder.GetString(), NULL);
		}
		m_strFileName = m_strLogFolder;
	}
	else
	{
		if (bFirstTime)
		{
			CreateDirectory(m_strFileName.GetString(), NULL);
		}	
	}

	if (m_strPrefix.GetLength())
	{
		m_strFileName += m_strPrefix;
	}
	m_strFileName += strExeName;

	SYSTEMTIME SystemTime = {0};
	GetLocalTime(
		&SystemTime//__out         LPSYSTEMTIME lpSystemTime
		);

	if (m_bInXml)
	{
		m_strFileName.AppendFormat(
			TEXT(".%04d-%02d%02d-%02d%02d%02d-%03d.%04d.log.xml"),//LPCTSTR pszFormat,
			SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, 
			SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond,
			SystemTime.wMilliseconds,
			GetCurrentProcessId()//...
			);
	}
	else
	{
		m_strFileName.AppendFormat(
			TEXT(".%04d-%02d%02d-%02d%02d%02d-%03d.%04d.log"),//LPCTSTR pszFormat,
			SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, 
			SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond,
			SystemTime.wMilliseconds,
			GetCurrentProcessId()//...
			);
	}
	

	m_fout = CreateFile(
		m_strFileName.GetString(),//__in          LPCTSTR lpFileName,
		GENERIC_WRITE,//__in          DWORD dwDesiredAccess,
		FILE_SHARE_READ,//__in          DWORD dwShareMode,
		NULL,//__in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_ALWAYS,//__in          DWORD dwCreationDisposition,
		0,//__in          DWORD dwFlagsAndAttributes,
		NULL//__in          HANDLE hTemplateFile
		);	

	CONST BYTE ucIndicator[] = {0xff, 0xfe};
	WriteBufferNoLock(ucIndicator, sizeof(ucIndicator));
	//static CONST TCHAR szXmlHeader[] = TEXT("<?xml version='1.0' encoding='UTF-16'?>\n");
	//Write(szXmlHeader);

	if (m_bInXml)
	{
		static CONST TCHAR szLogRoot[] = TAG_LOG_ROOT_START;
		WriteBufferNoLock(szLogRoot, sizeof(szLogRoot)-sizeof(TCHAR));
	} 
	else
	{
		static CONST TCHAR szLogRoot[] = TAG_LOG_ROOT_START_NOTAG;
		WriteBufferNoLock(szLogRoot, sizeof(szLogRoot)-sizeof(TCHAR));
	}

}

BOOL CDTraceLogger::IsInXml()
{
	return m_bInXml;
}

} //end anonymous namespace

//////////////////////////////////////////////////////////////////////////
CDTraceStart::CDTraceStart(INT nDebuglevel, LPCTSTR szEnding0, 
						   LPCTSTR szFunction0, LPCTSTR szFile0, INT Line0
						   ) :
m_szEnding(szEnding0),
m_szFunction(szFunction0),
m_szFile(szFile0),
m_line(Line0),
m_nDebuglevel(nDebuglevel)
{
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		if (NULL == SILOGGER)
		{
			break;
		}

		if (m_nDebuglevel > SILOGGER->GetDebugLevel())
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		CTlsData * pTlsData = GetTlsDataHelper();

		SYSTEMTIME SystemTime;
		GetLocalTime(
			&SystemTime//__out         LPSYSTEMTIME lpSystemTime
			);
		if (SILOGGER->IsInXml())
		{
			pTlsData->m_strMessage.Format(
				DETAIL_FORMAT1 TEXT("<%s>\t") DETAIL_FORMAT2 TEXT("%s"),
				DETAIL_TEXT1(SystemTime, pTlsData->m_dwProcessID, pTlsData->m_dwThreadID), 
				m_szFunction,
				DETAIL_TEXT2(m_szFunction, m_szFile, m_line),
				m_szEnding//...
				);
			ReviseFunctionName( pTlsData->m_strMessage );
		}
		else
		{
			pTlsData->m_strMessage.Format(
				DETAIL_FORMAT1_NOTAG TEXT("Enter %s\t") DETAIL_FORMAT2_NOTAG TEXT("%s"),
				DETAIL_TEXT1(SystemTime, pTlsData->m_dwProcessID, pTlsData->m_dwThreadID), 
				m_szFunction,
				DETAIL_TEXT2(m_szFunction, m_szFile, m_line),
				m_szEnding//...
				);
		}

		OutputDebugString( pTlsData->m_strMessage.GetString() );

		SILOGGER->WriteBuffer(
			pTlsData->m_strMessage.GetString(), 
			pTlsData->m_strMessage.GetLength()*sizeof(TCHAR)
			);

	} while (FALSE);

}

//////////////////////////////////////////////////////////////////////////
CDTraceStart::~CDTraceStart()
{
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		if (NULL == SILOGGER)
		{
			break;
		}

		if (m_nDebuglevel > SILOGGER->GetDebugLevel())
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		CTlsData * pTlsData = GetTlsDataHelper();

		SYSTEMTIME SystemTime;
		GetLocalTime(
			&SystemTime//__out         LPSYSTEMTIME lpSystemTime
			);

		if (SILOGGER->IsInXml())
		{
			pTlsData->m_strMessage.Format(
				DETAIL_FORMAT1 TEXT("</%s>\t") DETAIL_FORMAT2 TEXT("%s"),
				DETAIL_TEXT1(SystemTime, pTlsData->m_dwProcessID, pTlsData->m_dwThreadID), 
				m_szFunction,
				DETAIL_TEXT2(m_szFunction, m_szFile, m_line),
				m_szEnding//...
				);
			ReviseFunctionName( pTlsData->m_strMessage );
		} 
		else
		{
			pTlsData->m_strMessage.Format(
				DETAIL_FORMAT1_NOTAG TEXT("Leave %s\t") DETAIL_FORMAT2_NOTAG TEXT("%s"),
				DETAIL_TEXT1(SystemTime, pTlsData->m_dwProcessID, pTlsData->m_dwThreadID), 
				m_szFunction,
				DETAIL_TEXT2(m_szFunction, m_szFile, m_line),
				m_szEnding//...
				);
		}
		
		OutputDebugString( pTlsData->m_strMessage.GetString() );

		SILOGGER->WriteBuffer(
			pTlsData->m_strMessage.GetString(), 
			pTlsData->m_strMessage.GetLength()*sizeof(TCHAR)
			);
	} while (FALSE);

}

//////////////////////////////////////////////////////////////////////////
VOID WINAPI DTraceSetDebugSettings( INT nDebugLevel, BOOL bInXml, LPCTSTR strPrefix, LPCTSTR strLogFolder)
{
	CSIDTraceLogger::CreateInstance(nDebugLevel, bInXml, strPrefix, strLogFolder);
}

//////////////////////////////////////////////////////////////////////////
VOID WINAPIV DTraceInfo(INT nDebugLevel, LPCTSTR szEnding, 
						LPCTSTR szFunction, LPCTSTR szFile, INT line, 
						LPCTSTR szFormat, ...
						)
{
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		if (NULL == SILOGGER)
		{
			break;
		}

		if (nDebugLevel > SILOGGER->GetDebugLevel())
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		CTlsData * pTlsData = GetTlsDataHelper(); 

		SYSTEMTIME SystemTime;
		GetLocalTime(
			&SystemTime//__out         LPSYSTEMTIME lpSystemTime
			);

		if (SILOGGER->IsInXml())
		{
			pTlsData->m_strMessage.Format(
				DETAIL_FORMAT1 TAG_INFO_START, 
				DETAIL_TEXT1(SystemTime, pTlsData->m_dwProcessID, pTlsData->m_dwThreadID)
				);
		} 
		else
		{
			pTlsData->m_strMessage.Format(
				DETAIL_FORMAT1_NOTAG TAG_INFO_START_NOTAG, 
				DETAIL_TEXT1(SystemTime, pTlsData->m_dwProcessID, pTlsData->m_dwThreadID)
				);
		}


		va_list args;
		va_start( args, szFormat );
		pTlsData->m_strHelper.FormatV(szFormat, args);
		va_end( args );

		if (SILOGGER->IsInXml())
		{
			pTlsData->m_strHelper += TAG_INFO_END;
		}
		else
		{
			pTlsData->m_strHelper += TAG_INFO_END_NOTAG;
		}
		
		pTlsData->m_strMessage += pTlsData->m_strHelper;

		if (SILOGGER->IsInXml())
		{
			pTlsData->m_strHelper.Format(DETAIL_FORMAT2, DETAIL_TEXT2(szFunction, szFile, line));
		} 
		else
		{
			pTlsData->m_strHelper.Format(DETAIL_FORMAT2_NOTAG, DETAIL_TEXT2(szFunction, szFile, line));
		}
		
		pTlsData->m_strMessage += pTlsData->m_strHelper;
		pTlsData->m_strMessage += szEnding;

		OutputDebugString(pTlsData->m_strMessage.GetString());
		SILOGGER->WriteBuffer(
			pTlsData->m_strMessage.GetString(), 
			pTlsData->m_strMessage.GetLength()*sizeof(TCHAR)
			);

	} while (FALSE);
}

//////////////////////////////////////////////////////////////////////////
VOID WINAPIV DTraceAPIErr(INT nDebugLevel, LPCTSTR szEnding, 
						  LPCTSTR szFunction, LPCTSTR szFile, INT line, 
						  LPCTSTR szApi, DWORD dwErrorCode, 
						  LPCTSTR szFormat, ...
						  ) 
{
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		if (NULL == SILOGGER)
		{
			break;
		}

		if (nDebugLevel > SILOGGER->GetDebugLevel())
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		CTlsData * pTlsData = GetTlsDataHelper(); 

		LPTSTR lpMsgBuf = NULL;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dwErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		RemoveRN(lpMsgBuf, DTRACE_BUFFSZ);

		SYSTEMTIME SystemTime;
		GetLocalTime(
			&SystemTime//__out         LPSYSTEMTIME lpSystemTime
			);

		va_list args;
		va_start(args, szFormat);
		pTlsData->m_strHelper.FormatV(szFormat, args);
		va_end(args);

		if( INT(dwErrorCode) >= 0)
		{
			if (SILOGGER->IsInXml())
			{
				pTlsData->m_strMessage.Format(
					DETAIL_FORMAT1 TAG_ERROR_START TEXT("[%s][%d][%s][%s]") TAG_ERROR_END DETAIL_FORMAT2 TEXT("%s"), 
					DETAIL_TEXT1(SystemTime, pTlsData->m_dwProcessID, pTlsData->m_dwThreadID),
					szApi, dwErrorCode, lpMsgBuf, pTlsData->m_strHelper.GetString(),
					DETAIL_TEXT2(szFunction, szFile, line),
					szEnding
					);
			} 
			else
			{
				pTlsData->m_strMessage.Format(
					DETAIL_FORMAT1_NOTAG TAG_ERROR_START_NOTAG TEXT("[%s][%d][%s][%s]") TAG_ERROR_END_NOTAG DETAIL_FORMAT2_NOTAG TEXT("%s"), 
					DETAIL_TEXT1(SystemTime, pTlsData->m_dwProcessID, pTlsData->m_dwThreadID),
					szApi, dwErrorCode, lpMsgBuf, pTlsData->m_strHelper.GetString(),
					DETAIL_TEXT2(szFunction, szFile, line),
					szEnding
					);
			}

		}
		else 
		{
			if (SILOGGER->IsInXml())
			{
				pTlsData->m_strMessage.Format(
					DETAIL_FORMAT1 TAG_ERROR_START TEXT("[%s][0x%08x][%s][%s]") TAG_ERROR_END DETAIL_FORMAT2 TEXT("%s"), 
					DETAIL_TEXT1(SystemTime, pTlsData->m_dwProcessID, pTlsData->m_dwThreadID),
					szApi, dwErrorCode, lpMsgBuf, pTlsData->m_strHelper.GetString(),
					DETAIL_TEXT2(szFunction, szFile, line),
					szEnding
					);
			} 
			else
			{
				pTlsData->m_strMessage.Format(
					DETAIL_FORMAT1_NOTAG TAG_ERROR_START_NOTAG TEXT("[%s][0x%08x][%s][%s]") TAG_ERROR_END_NOTAG DETAIL_FORMAT2_NOTAG TEXT("%s"), 
					DETAIL_TEXT1(SystemTime, pTlsData->m_dwProcessID, pTlsData->m_dwThreadID),
					szApi, dwErrorCode, lpMsgBuf, pTlsData->m_strHelper.GetString(),
					DETAIL_TEXT2(szFunction, szFile, line),
					szEnding
					);
			}
			
		}

		LocalFree(lpMsgBuf);

		OutputDebugString(pTlsData->m_strMessage.GetString());
		SILOGGER->WriteBuffer(
			pTlsData->m_strMessage.GetString(), 
			pTlsData->m_strMessage.GetLength()*sizeof(TCHAR)
			);

	} while (FALSE);
}



VOID WINAPI DTraceSetLastStatus(DWORD dwStatusLow, LONG nStatusHigh)
{
	CTlsData * pTlsData = GetTlsDataHelper(); 
	pTlsData->m_liLastStatus.LowPart = dwStatusLow;
	pTlsData->m_liLastStatus.HighPart = nStatusHigh;
}


//<sonmi01>2012-8-29 ###???
VOID WINAPI DTraceSetLastErrorMessage(LPCTSTR szFormat, ...)
{
	CTlsData * pTlsData = GetTlsDataHelper(); 
	va_list args;
	va_start(args, szFormat);
	pTlsData->m_strLastErrorMessage.FormatV(szFormat, args);
	va_end(args);
}

DWORD WINAPI DTraceGetLastStatus(LONG * pnStatusHigh)
{
	CTlsData * pTlsData = GetTlsDataHelper(); 
	if (pnStatusHigh)
	{
		*pnStatusHigh = pTlsData->m_liLastStatus.HighPart;
	}
	return pTlsData->m_liLastStatus.LowPart;
}

//<sonmi01>2012-8-29 ###???
VOID WINAPI DTraceGetLastErrorMessage(CString & strLastErrorMessage)
{
	CTlsData * pTlsData = GetTlsDataHelper(); 
	strLastErrorMessage = pTlsData->m_strLastErrorMessage;
}

//////////////////////////////////////////////////////////////////////////
static BOOL FindDebugMe()
{
	BOOL bFound = FALSE;

	TCHAR szFullPath[MAX_PATH*2] = {0};
	DWORD dwGetModuleFileName = GetModuleFileName(
		NULL,//__in          HMODULE hModule,
		szFullPath,//__out         LPTSTR lpFilename,
		_countof(szFullPath)//__in          DWORD nSize
		);
	if (dwGetModuleFileName)
	{
		CONST TCHAR		szFileName[] = TEXT(".DebugMe");
		HANDLE			hFileFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA	FindFileData = {0};

		_tcscat_s(          
			szFullPath,//LPTSTR pszDest,
			_countof(szFullPath),//size_t cchDest,
			szFileName//LPCTSTR pszSrc
			);

		hFileFind = FindFirstFile(
			szFullPath,//__in          LPCTSTR lpFileName,
			&FindFileData//__out         LPWIN32_FIND_DATA lpFindFileData
			);
		bFound = (INVALID_HANDLE_VALUE!=hFileFind);
		FindClose(hFileFind); hFileFind = INVALID_HANDLE_VALUE;
	}

	return bFound;
}

VOID DTraceMessageBoxImpl(LPCTSTR szText, LPCTSTR szFunction, LPCTSTR szFile, INT nLine)
{
	BOOL bFound = FindDebugMe();

	if( bFound)
	{
		TCHAR szMsgText[1024] = TEXT("Do you want to generate BreakPoint exception?\r\n\r\n");
		//static CONST TCHAR szPosFormat[] = TEXT("Process=%d(0x%04x), Thread=%d(0x%04x), Function='%s', File='%s', Line=%d\r\n\r\n");
		static CONST TCHAR szPosFormat[] = TEXT("0x%04x.0x%04x(%d.%d) '%s;%s;%d' \r\n\r\n");

		TCHAR szPos[512] = {0};
		INT id = 0;
		DWORD pid = 0;
		DWORD tid =0;

		pid = GetCurrentProcessId();
		tid = GetCurrentThreadId();
		_stprintf_s(szPos, _countof(szPos), szPosFormat, 
			pid, tid, pid, tid, 
			szFunction, szFile, nLine);
		_tcscat_s(szMsgText, _countof(szMsgText), szPos);
		_tcscat_s(szMsgText, _countof(szMsgText), szText);
		id = MessageBox(NULL, szMsgText, TEXT("DebugMe"), MB_SERVICE_NOTIFICATION|MB_YESNO);
		if (IDYES == id)
		{
			DebugBreak();
		}
	}

}

VOID DTraceDebugBreakImpl(LPCTSTR szText, LPCTSTR szFunction, LPCTSTR szFile, INT nLine)
{
	BOOL bFound = FindDebugMe();

	if( bFound)
	{
		DebugBreak();
	}
}

VOID DTraceDebugSleepImpl(LPCTSTR szText, ULONG nCount, ULONG nSleepTime, LPCTSTR szFunction, LPCTSTR szFile, INT nLine)
{
	BOOL bFound = FindDebugMe();
	if( bFound)
	{
		if (0 == nCount || (ULONG)(-1) == nCount || 0 == nSleepTime || (ULONG)(-1) == nSleepTime)
		{
			for (;;)
			{
				Sleep(2000);
			}
		}
		else
		{
			for (ULONG ii=0; ii<nCount; ++ii)
			{
				Sleep(nSleepTime);
			}
		}
	}
}
