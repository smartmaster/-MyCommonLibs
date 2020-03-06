#include "stdafx.h"

#include <fstream>
#include <string>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"

#include "ReadWriteStringFile.h"

HRESULT ReadSourceStringFile(LPCTSTR pSourceFile, CString & strSource)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	HANDLE hFile = CreateFile(
		pSourceFile,//_In_      LPCTSTR lpFileName,
		GENERIC_READ,//_In_      DWORD dwDesiredAccess,
		0,//_In_      DWORD dwShareMode,
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		NULL//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("%s"), pSourceFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CHandle ahFile(hFile); hFile = INVALID_HANDLE_VALUE;
	LARGE_INTEGER FileSize = { 0 };
	BOOL bRet = GetFileSizeEx(
		ahFile,//_In_   HANDLE hFile,
		&FileSize//_Out_  PLARGE_INTEGER lpFileSize
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("GetFileSizeEx"), hr, TEXT("%s"), pSourceFile);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	if (FileSize.QuadPart < 3)
	{
		hr = HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
		//D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("GetFileSizeEx"), hr, TEXT("%s"), pSourceFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////

	CHeapPtr<BYTE> apBuffer;
	apBuffer.Allocate(FileSize.LowPart + 512);
	ZeroMemory(apBuffer.m_pData, FileSize.LowPart + 512);
	DWORD NumberOfBytesRead = 0;
	bRet = ReadFile(
		ahFile,//_In_         HANDLE hFile,
		apBuffer.m_pData,//_Out_        LPVOID lpBuffer,
		FileSize.LowPart,//_In_         DWORD nNumberOfBytesToRead,
		&NumberOfBytesRead,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
		NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
		);

	INT iResult = 0;
	if (apBuffer.m_pData[0] == 0xff && apBuffer.m_pData[1] == 0xfe) //utf-16
	{
		//D_INFO(0, TEXT("%s is detected to be UNICODE"), pFile);
		strSource = (WCHAR*)(apBuffer.m_pData + 2);
	}
	else if (apBuffer.m_pData[0] == 0xef && apBuffer.m_pData[1] == 0xbb && apBuffer.m_pData[2] == 0xbf) //utf-8
	{
		//D_INFO(0, TEXT("%s is detected to be NON-UNICODE"), pFile);
		strSource = (CHAR*)(apBuffer.m_pData + 3);
	}
	else if (IsTextUnicode(apBuffer.m_pData, FileSize.LowPart, &iResult) && (iResult & IS_TEXT_UNICODE_STATISTICS))
	{
		strSource = (WCHAR*)(apBuffer.m_pData); //wchar
	}
	else
	{
		strSource = (CHAR*)(apBuffer.m_pData); //ansi
	}

	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("ReadFile"), hr, TEXT("%s"), pSourceFile);
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

HRESULT WriteTargetStringFile(LPCTSTR pTargetFile, CONST vector<CString> & strSource)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	HANDLE hFile = CreateFile(
		pTargetFile,//_In_      LPCTSTR lpFileName,
		GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
		0,//_In_      DWORD dwShareMode,
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		NULL//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("%s"), pTargetFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CHandle ahFile(hFile); hFile = INVALID_HANDLE_VALUE;

	DWORD NumberOfBytesRead = 0;
	DWORD NumberOfBytesWritten = 0;
	CONST BYTE ucIndicator[] = { 0xff, 0xfe };
	BOOL bRet = WriteFile(
		ahFile,//_In_         HANDLE hFile,
		ucIndicator,//_In_         LPCVOID lpBuffer,
		sizeof(ucIndicator),//_In_         DWORD nNumberOfBytesToWrite,
		&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
		NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("WriteFile"), hr, TEXT("%s"), pTargetFile);
		LEAVE_BLOCK(0);
	}

	for (size_t ii = 0; ii < strSource.size(); ++ii)
	{
		NumberOfBytesRead = 0;
		NumberOfBytesWritten = 0;
		bRet = WriteFile(
			ahFile,//_In_         HANDLE hFile,
			strSource[ii].GetString(),//_In_         LPCVOID lpBuffer,
			strSource[ii].GetLength() * sizeof(TCHAR),//(strSource[ii].GetLength() + 1) * sizeof(TCHAR),//_In_         DWORD nNumberOfBytesToWrite,
			&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
			NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
			);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("WriteFile"), hr, TEXT("%s"), pTargetFile);
			LEAVE_BLOCK(0);
		}
	}


	END_BLOCK(0);

	return hr;
}


namespace SmartLib
{
	HRESULT ReadStdFile(LPCTSTR fileNane, vector<wstring> & lines)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;
		
		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		std::wifstream wfin(fileNane);
		if (!wfin)
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT("wifstream"), hr, TEXT("cannot opne file [%s]"), fileNane);
			LEAVE_BLOCK(0);
		}

		for (;;)
		{
			std::wstring str;
			if (!std::getline(wfin, str))
			{
				break;
			}
		
			lines.push_back(str);
		}

		wfin.close();
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);
		
		return hr;			
	}


	HRESULT WriteStdFile(LPCTSTR fileName, const vector<wstring> & lines)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;
		
		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		std::wofstream wfout(fileName);
		if (!wfout)
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			hr = E_FAIL;
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("wofstream"), hr, TEXT("[%s]"), fileName);
			LEAVE_BLOCK(0);
		}


		for (CONST auto & line : lines)
		{
			wfout << line << std::endl;
		}
		wfout.close();

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);
		
		return hr;
	}
} //namespace SmartLib