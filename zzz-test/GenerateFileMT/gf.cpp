// gf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include <vector>
using namespace std;

#include "..\..\PublicHeader\PublicCodeBlock.h"

#define DEBUGLOG(format, ...) _ftprintf_s(stdout, format TEXT("\r\n"), __VA_ARGS__)

HRESULT CrateFileWithSize(LPCTSTR FileName, LONGLONG FileSize)
{
	HRESULT hr = S_OK;

	HANDLE hFile = INVALID_HANDLE_VALUE;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	DWORD dwFlagsAndAttributes = 0;
	hFile = CreateFile(
		FileName,//_In_      LPCTSTR lpFileName,
		GENERIC_ALL,//_In_      DWORD dwDesiredAccess,
		FILE_SHARE_READ | FILE_SHARE_WRITE,//_In_      DWORD dwShareMode,
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
		dwFlagsAndAttributes,//_In_      DWORD dwFlagsAndAttributes,
		NULL//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		DEBUGLOG(TEXT("cannot create file [fileName=%s, hr=0x%08x]"), FileName, hr);
		LEAVE_BLOCK(0);
	}

#if 1
	//////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER liDistanceToMove;
	liDistanceToMove.QuadPart = FileSize;
	LARGE_INTEGER NewFilePointer = {0};
	BOOL bRet = SetFilePointerEx(
		hFile,//_In_       HANDLE hFile,
		liDistanceToMove,//_In_       LARGE_INTEGER liDistanceToMove,
		&NewFilePointer,//_Out_opt_  PLARGE_INTEGER lpNewFilePointer,
		FILE_BEGIN//_In_       DWORD dwMoveMethod
		);
	if (!bRet)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		DEBUGLOG(TEXT("SetFilePointerEx [fileName=%s, DistanceToMove=%I64d, hr=0x%08x]"), FileName, liDistanceToMove.QuadPart, hr);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	bRet = SetEndOfFile(hFile);
	if (!bRet)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		DEBUGLOG(TEXT("SetEndOfFile [fileName=%s, DistanceToMove=%I64d, hr=0x%08x]"), FileName, liDistanceToMove.QuadPart, hr);
		LEAVE_BLOCK(0);
	}
#endif

	bRet = SetFileValidData(
		hFile,//_In_  HANDLE hFile,
		FileSize//_In_  LONGLONG ValidDataLength
		);
	if (!bRet)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		DEBUGLOG(TEXT("SetFileValidData [fileName=%s, DistanceToMove=%I64d, hr=0x%08x]"), FileName, FileSize, hr);
		//LEAVE_BLOCK(0);
		hr = S_OK; //do not fail
	}

	//////////////////////////////////////////////////////////////////////////
	DEBUGLOG(TEXT("created file [fileName=%s, size=%i64d]"), FileName, FileSize);
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	if (INVALID_HANDLE_VALUE != hFile)
	{
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
	
	return hr;
}

VOID PrintUsage(TCHAR ** argv)
{
	DEBUGLOG(TEXT("Usage:"));
	DEBUGLOG(TEXT("%s -name file_name [-size file_size] [-count file_count] [-thread threadCount]"), TEXT("this.exe"));
}

BOOL EqaulSz(LPCTSTR str1, LPCTSTR str2)
{
	return 0 == _tcsicmp(str1, str2);
}

struct CmdLine
{
	CString m_FileName;
	LONGLONG m_FileSize;
	LONGLONG m_FileCount;
	LONG m_ThreadCount;

	BOOL parse(int argc, TCHAR ** argv)
	{
		BOOL bValid = FALSE;

		m_FileSize = 1024 * 1024;
		m_FileCount = 1;
		m_ThreadCount = 8;

		for (int ii = 1; ii < argc; ++ ii)
		{
			if (EqaulSz(TEXT("-name"), argv[ii]))
			{
				++ ii;
				if (ii >= argc)
				{
					break;
				}

				bValid = TRUE;
				m_FileName = argv[ii];
			}
			else if (EqaulSz(TEXT("-size"), argv[ii]))
			{
				++ ii;
				if (ii >= argc)
				{
					break;
				}

				m_FileSize = _tcstoi64(argv[ii], NULL, 0);
				if (m_FileSize <= 0)
				{
					m_FileSize = 1024 * 1024;
				}
			}
			else if (EqaulSz(TEXT("-count"), argv[ii]))
			{
				++ ii;
				if (ii >= argc)
				{
					break;
				}

				m_FileCount = _tcstoi64(argv[ii], NULL, 0);
				if (m_FileCount <= 0)
				{
					m_FileCount = 1;
				}
			}
			else if (EqaulSz(TEXT("-thread"), argv[ii]))
			{
				++ ii;
				if (ii >= argc)
				{
					break;
				}

				m_ThreadCount = _tcstol(argv[ii], NULL, 0);
				if (m_ThreadCount <= 0)
				{
					m_ThreadCount = 8;
				}

				if (m_ThreadCount > 64)
				{
					m_ThreadCount = 64;
				}
			}
		} //end for

		if (!bValid)
		{
			PrintUsage(argv);
		}

		return bValid;
	}
};

class  CMTCreateFileWithSize
{
private:
	LONGLONG m_FileCount; /***INCOMMING***/
	LONGLONG m_FileSize; /***INCOMMING***/
	CString m_strFileName; /***INCOMMING***/

	LONGLONG m_seq; /***INTERNAL***/
public:
	CMTCreateFileWithSize(LONGLONG FileCount, LONGLONG FileSize, LPCTSTR strFileName) :
		m_FileCount(FileCount),
		m_FileSize(FileSize),
		m_strFileName(strFileName),
		m_seq(0)
	{

	}
	~ CMTCreateFileWithSize()
	{

	}

private:
	HRESULT Proc()
	{
		HRESULT hr = S_OK;
		CString fileNameSeq;
		for (;;)
		{
			//////////////////////////////////////////////////////////////////////////
			LONGLONG sequence = InterlockedIncrement64(&m_seq);
			if (sequence > m_FileCount)
			{
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			fileNameSeq = m_strFileName;
			fileNameSeq.AppendFormat(TEXT("-%012I64d"), sequence);
			hr = CrateFileWithSize(fileNameSeq.GetString(), m_FileSize);
			if (FAILED(hr))
			{
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			CONST LONG StreamCount = 2;
			CONST LONGLONG StreamSize = 1024;
			for (LONG ii = 0; ii < StreamCount; ++ ii)
			{
				CString streamName = fileNameSeq;
				//streamName.AppendFormat(TEXT(":stream-%012d:$DATA"), ii);
				streamName.AppendFormat(TEXT(":stream-%012d"), ii);
				hr = CrateFileWithSize(streamName.GetString(), StreamSize);
				if (FAILED(hr))
				{
					break;
				}
			}
			if (FAILED(hr))
			{
				break;
			}
		}
		
		return hr;
	}

	static DWORD WINAPI ThreadProc(_In_  LPVOID lpParameter)
	{
		CMTCreateFileWithSize * pObj = (CMTCreateFileWithSize *)(lpParameter);
		return pObj->Proc();
	}

public:
	VOID Run(LONG ThreadCount)
	{
		vector<HANDLE> vecThread;

		//////////////////////////////////////////////////////////////////////////
		for (LONG ii = 0; ii < ThreadCount; ++ ii)
		{
			DWORD ThreadId = 0;
			HANDLE hThread = CreateThread(
				NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
				0,//_In_       SIZE_T dwStackSize,
				(LPTHREAD_START_ROUTINE)(ThreadProc),//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
				(LPVOID)(this),//_In_opt_   LPVOID lpParameter,
				0,//_In_       DWORD dwCreationFlags,
				&ThreadId//_Out_opt_  LPDWORD lpThreadId
				);
			if (NULL == ThreadId)
			{
				break;
			}

			vecThread.push_back(hThread);
		}

		//////////////////////////////////////////////////////////////////////////
		WaitForMultipleObjects(vecThread.size(), &vecThread[0], TRUE, INFINITE);

		//////////////////////////////////////////////////////////////////////////
		for (size_t ii = 0; ii < vecThread.size(); ++ ii)
		{
			CloseHandle(vecThread[ii]);
		}
		vecThread.clear();
	}

};


int _tmain(int argc, _TCHAR* argv[])
{
	CmdLine cmdLine;
	BOOL bValidCmdLine = cmdLine.parse(argc, argv);
	if (!bValidCmdLine)
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;

	CMTCreateFileWithSize obj(cmdLine.m_FileCount, cmdLine.m_FileSize, cmdLine.m_FileName);
	obj.Run(cmdLine.m_ThreadCount);

	//CString fileNameSeq;
	//for (LONGLONG ii = 0; ii < cmdLine.m_FileCount; ++ ii)
	//{
	//	fileNameSeq = cmdLine.m_FileName;
	//	fileNameSeq.AppendFormat(TEXT("-%012I64d"), ii);

	//	hr = CrateFileWithSize(fileNameSeq.GetString(), cmdLine.m_FileSize);
	//	if (FAILED(hr))
	//	{
	//		break;
	//	}
	//}

	return hr;
}

