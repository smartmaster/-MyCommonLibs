#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicWideNarrowChar.h"

#include "ReadWriteTextFile.h"

//////////////////////////////////////////////////////////////////////////
//read unicode source files only
/*static*/ HRESULT ReadSourceFile(LPCTSTR pFile, CString & strSource)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	HANDLE hFile = CreateFile(
		pFile,//_In_      LPCTSTR lpFileName,
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
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("%s"), pFile);
		LEAVE_BLOCK(0);
	}

	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceCFileDeviceHandle(
		hFile,//IN HANDLE Handle,
		TRUE,//IN BOOL bAttach,
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceHandle"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = ReadSourceStream(spIFileDevice, strSource);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ReadSourceFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	
#if 0
	////////////////////////////////////////////////////////////////////////////
	//CHandle ahFile(hFile); hFile = INVALID_HANDLE_VALUE;
	//LARGE_INTEGER FileSize = { 0 };
	//BOOL bRet = GetFileSizeEx(
	//	ahFile,//_In_   HANDLE hFile,
	//	&FileSize//_Out_  PLARGE_INTEGER lpFileSize
	//	);
	//if (!bRet)
	//{
	//	LastError = GetLastError();
	//	hr = HRESULT_FROM_WIN32(LastError);
	//	D_SET_LAST_STATUS(hr, LastError);
	//	D_API_ERR(0, TEXT("GetFileSizeEx"), hr, TEXT("%s"), pFile);
	//	LEAVE_BLOCK(0);
	//}

	////////////////////////////////////////////////////////////////////////////
	//CHeapPtr<BYTE> apBuffer;
	//apBuffer.Allocate(FileSize.LowPart + 512);
	//ZeroMemory(apBuffer.m_pData, FileSize.LowPart + 512);
	//DWORD NumberOfBytesRead = 0;
	//bRet = ReadFile(
	//	ahFile,//_In_         HANDLE hFile,
	//	apBuffer.m_pData,//_Out_        LPVOID lpBuffer,
	//	FileSize.LowPart,//_In_         DWORD nNumberOfBytesToRead,
	//	&NumberOfBytesRead,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
	//	NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
	//	);
	//if (!bRet)
	//{
	//	LastError = GetLastError();
	//	hr = HRESULT_FROM_WIN32(LastError);
	//	D_SET_LAST_STATUS(hr, LastError);
	//	D_API_ERR(0, TEXT("ReadFile"), hr, TEXT("%s"), pFile);
	//	LEAVE_BLOCK(0);
	//}

	////INT iResult = 0;
	////if ((IsTextUnicode(apBuffer.m_pData, FileSize, &iResult) && (iResult & IS_TEXT_UNICODE_STATISTICS)) ||
	////	(apBuffer.m_pData[0] == 0xff && apBuffer.m_pData[1] == 0xfe))
	//if (apBuffer.m_pData[0] == 0xff && apBuffer.m_pData[1] == 0xfe)
	//{
	//	D_INFO(0, TEXT("%s is detetced to be utf-16LE"), pFile);
	//	strSource = (WCHAR*)(apBuffer.m_pData + 2);
	//}
	//else if (apBuffer.m_pData[0] == 0xef && apBuffer.m_pData[1] == 0xbb && apBuffer.m_pData[2] == 0xbf)
	//{
	//	D_INFO(0, TEXT("%s is detetced to be utf-8"), pFile);
	//	strSource = (CHAR*)(apBuffer.m_pData + 3);
	//}
	//else
	//{
	//	D_INFO(0, TEXT("%s is assumed to be ASCII"), pFile);
	//	strSource = (CHAR*)(apBuffer.m_pData);
	//	if (strSource.GetLength() == 0)
	//	{
	//		D_INFO(0, TEXT("%s is assumed to be WIDE CHAR"), pFile);
	//		strSource = (WCHAR*)(apBuffer.m_pData);
	//	}
	//}
#endif

	END_BLOCK(0);

	return hr;

}

HRESULT ReadSourceStream(IFileDevice * pFile, CString & strSource)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);

	LONGLONG FileSize = { 0 };
	hr = pFile->GetFileSize(
		&FileSize//_Out_  PLARGE_INTEGER lpFileSize
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("GetFileSizeEx"), hr, TEXT("%p"), pFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CHeapPtr<BYTE> apBuffer;
	apBuffer.Allocate(FileSize + 512);
	ZeroMemory(apBuffer.m_pData, FileSize + 512);
	DWORD NumberOfBytesRead = 0;
	hr = pFile->Read(
		FileSize,//_In_         DWORD nNumberOfBytesToRead,
		&NumberOfBytesRead,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
		apBuffer.m_pData//_Out_        LPVOID lpBuffer,
		);
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("ReadFile"), hr, TEXT("%p"), pFile);
		LEAVE_BLOCK(0);
	}

	//INT iResult = 0;
	//if ((IsTextUnicode(apBuffer.m_pData, FileSize, &iResult) && (iResult & IS_TEXT_UNICODE_STATISTICS)) ||
	//	(apBuffer.m_pData[0] == 0xff && apBuffer.m_pData[1] == 0xfe))
	if (apBuffer.m_pData[0] == 0xff && apBuffer.m_pData[1] == 0xfe)
	{
		D_INFO(0, TEXT("%p is detetced to be utf-16LE"), pFile);
		strSource = (WCHAR*)(apBuffer.m_pData + 2);
	}
	else if (apBuffer.m_pData[0] == 0xef && apBuffer.m_pData[1] == 0xbb && apBuffer.m_pData[2] == 0xbf)
	{
		D_INFO(0, TEXT("%p is detetced to be utf-8"), pFile);
		CStringA strSourceA = (CHAR*)(apBuffer.m_pData + 3);
		hr = Utf8To16String(strSourceA, strSource);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Utf8To16String"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		D_INFO(0, TEXT("%p is assumed to be ASCII"), pFile);
		strSource = (CHAR*)(apBuffer.m_pData);
		if (strSource.GetLength() == 1 && FileSize > 1)
		{
			D_INFO(0, TEXT("%p is assumed to be WIDE CHAR"), pFile);
			strSource = (WCHAR*)(apBuffer.m_pData);
		}
	}

	END_BLOCK(0);

	return hr;
}

//write unicode source files only
/*static*/ HRESULT WriteSourceFile(LPCTSTR pFile, CONST vector<CString> & strSource, E_WRITESOURCEFILE e_writesourcefile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	HANDLE hFile = CreateFile(
		pFile,//_In_      LPCTSTR lpFileName,
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
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("%s"), pFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spIFileDevice;
	hr = CreateInstanceCFileDeviceHandle(
		hFile,//IN HANDLE Handle,
		TRUE,//IN BOOL bAttach,
		&spIFileDevice,//OUT IFileDevice ** ppFileDevice,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceHandle"), hr, TEXT("%s"), pFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = WriteSourceStream(spIFileDevice, strSource, e_writesourcefile);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("WriteSourceFile"), hr, TEXT("%s"), pFile);
		LEAVE_BLOCK(0);
	}

#if 0
	////////////////////////////////////////////////////////////////////////////
	//CHandle ahFile(hFile); hFile = INVALID_HANDLE_VALUE;

	//DWORD NumberOfBytesRead = 0;
	//DWORD NumberOfBytesWritten = 0;

	//BOOL bRet = FALSE;
	//if (bAsUnicode)
	//{
	//	CONST BYTE ucIndicator[] = { 0xff, 0xfe };
	//	bRet = WriteFile(
	//		ahFile,//_In_         HANDLE hFile,
	//		ucIndicator,//_In_         LPCVOID lpBuffer,
	//		sizeof(ucIndicator),//_In_         DWORD nNumberOfBytesToWrite,
	//		&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
	//		NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
	//		);
	//	if (!bRet)
	//	{
	//		LastError = GetLastError();
	//		hr = HRESULT_FROM_WIN32(LastError);
	//		D_SET_LAST_STATUS(hr, LastError);
	//		D_API_ERR(0, TEXT("WriteFile"), hr, TEXT("%s"), pFile);
	//		LEAVE_BLOCK(0);
	//	}
	//}

	//for (size_t ii = 0; ii < strSource.size(); ++ii)
	//{
	//	NumberOfBytesRead = 0;
	//	NumberOfBytesWritten = 0;
	//	if (bAsUnicode)
	//	{
	//		bRet = WriteFile(
	//			ahFile,//_In_         HANDLE hFile,
	//			strSource[ii].GetString(),//_In_         LPCVOID lpBuffer,
	//			strSource[ii].GetLength() * sizeof(TCHAR),//(strSource[ii].GetLength() + 1) * sizeof(TCHAR),//_In_         DWORD nNumberOfBytesToWrite,
	//			&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
	//			NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
	//			);
	//	}
	//	else
	//	{
	//		CStringA strUft8;
	//		hr = Utf16To8String(strSource[ii], strUft8);
	//		if (FAILED(hr))
	//		{
	//			//LastError = WSAGetLastError();
	//			//LastError = GetLastError();
	//			//hr = HRESULT_FROM_WIN32(LastError);
	//			//D_SET_FISRT_STATUS(hr, LastError);
	//			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
	//			//D_SET_LAST_STATUS(hr, LastError);
	//			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
	//			D_API_ERR(0, TEXT("Utf16To8String"), hr, TEXT(""));
	//			LEAVE_BLOCK(0);
	//		}

	//		bRet = WriteFile(
	//			ahFile,//_In_         HANDLE hFile,
	//			strUft8.GetString(),//_In_         LPCVOID lpBuffer,
	//			strUft8.GetLength(),//(strSource[ii].GetLength() + 1) * sizeof(TCHAR),//_In_         DWORD nNumberOfBytesToWrite,
	//			&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
	//			NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
	//			);
	//	}

	//	if (!bRet)
	//	{
	//		LastError = GetLastError();
	//		hr = HRESULT_FROM_WIN32(LastError);
	//		D_SET_LAST_STATUS(hr, LastError);
	//		D_API_ERR(0, TEXT("WriteFile"), hr, TEXT("%s"), pFile);
	//		LEAVE_BLOCK(0);
	//	}
	//}
#endif

	END_BLOCK(0);

	return hr;
}

HRESULT WriteSourceStream(IFileDevice * pFile, CONST vector<CString> & strSource, E_WRITESOURCEFILE e_writesourcefile)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	
	//////////////////////////////////////////////////////////////////////////
	DWORD NumberOfBytesRead = 0;
	DWORD NumberOfBytesWritten = 0;

	if (E_WRITESOURCEFILE::E_UTF16 == e_writesourcefile)
	{
		CONST BYTE ucIndicator[] = { 0xff, 0xfe };
		hr = pFile->Write(
			sizeof(ucIndicator),//_In_         DWORD nNumberOfBytesToWrite,
			&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
			ucIndicator//_In_         LPCVOID lpBuffer,
			);
	}
	else if (E_WRITESOURCEFILE::E_UTF8 == e_writesourcefile)
	{
		CONST BYTE ucIndicator[] = { 0xef, 0xbb, 0xbf };
		hr = pFile->Write(
			sizeof(ucIndicator),//_In_         DWORD nNumberOfBytesToWrite,
			&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
			ucIndicator//_In_         LPCVOID lpBuffer,
			);
	}
	if (FAILED(hr))
	{
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("WriteFile"), hr, TEXT("%p"), pFile);
		LEAVE_BLOCK(0);
	}

	for (size_t ii = 0; ii < strSource.size(); ++ii)
	{
		NumberOfBytesRead = 0;
		NumberOfBytesWritten = 0;
		if (E_WRITESOURCEFILE::E_UTF16 == e_writesourcefile)
		{
			hr = pFile->Write(
				strSource[ii].GetLength() * sizeof(TCHAR),//(strSource[ii].GetLength() + 1) * sizeof(TCHAR),//_In_         DWORD nNumberOfBytesToWrite,
				&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
				(CONST BYTE *)strSource[ii].GetString()//_In_         LPCVOID lpBuffer,
				);
		}
		else
		{
			CStringA strUft8;
			hr = Utf16To8String(strSource[ii], strUft8);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("Utf16To8String"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			hr = pFile->Write(
				strUft8.GetLength(),//(strSource[ii].GetLength() + 1) * sizeof(TCHAR),//_In_         DWORD nNumberOfBytesToWrite,
				&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
				(CONST BYTE*)strUft8.GetString()//_In_         LPCVOID lpBuffer,
				);
		}

		if (FAILED(hr))
		{
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("WriteFile"), hr, TEXT("%p"), pFile);
			LEAVE_BLOCK(0);
		}
	}


	END_BLOCK(0);

	return hr;
}
