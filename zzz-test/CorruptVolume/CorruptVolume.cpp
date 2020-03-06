// CorruptVolume.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include "..\..\PublicHeader\PublicCodeBlock.h"

#define  PRINT_LINE(str, ...) _tprintf_s(str TEXT("\r\n"), __VA_ARGS__)


HRESULT WriteAtOffset(HANDLE hFile, LONGLONG offset, LPCVOID buffer, LONG Len)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER liDistanceToMove = { 0 };
	LARGE_INTEGER NewFilePointer = { 0 };
	liDistanceToMove.QuadPart = offset;
	BOOL bret = SetFilePointerEx(
		hFile,//_In_       HANDLE hFile,
		liDistanceToMove,//_In_       LARGE_INTEGER liDistanceToMove,
		&NewFilePointer,//_Out_opt_  PLARGE_INTEGER lpNewFilePointer,
		FILE_BEGIN//_In_       DWORD dwMoveMethod
		);
	if (!bret)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		PRINT_LINE(TEXT("%s failed wih %d"), TEXT("SetFilePointerEx"), hr);
		LEAVE_BLOCK(0);
	}

	DWORD NumberOfBytesWritten = 0;
	bret = WriteFile(
		hFile,//_In_         HANDLE hFile,
		buffer,//_In_         LPCVOID lpBuffer,
		Len,//_In_         DWORD nNumberOfBytesToWrite,
		&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
		NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
		);
	if (!bret)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		PRINT_LINE(TEXT("%s failed wih %d"), TEXT("WriteFile"), hr);
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT CorruptVolume(
	LPCTSTR VolumeName, 
	LONGLONG FirstWriteOffset,
	LONGLONG EveryWriteInterval,
	LONG WriteLength
	)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strVolume;
	strVolume.Format(TEXT("\\\\?\\%s"), VolumeName);
	HANDLE hVolumeTemp = CreateFile(
		strVolume.GetString(),//_In_      LPCTSTR lpFileName,
		GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
		FILE_SHARE_READ | FILE_SHARE_WRITE,//_In_      DWORD dwShareMode,
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		NULL//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hVolumeTemp)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		PRINT_LINE(TEXT("%s failed wih %d"), TEXT("CreateFile"), hr);
		LEAVE_BLOCK(0);
	}
	CHandle ahVolume(hVolumeTemp); hVolumeTemp = INVALID_HANDLE_VALUE;


	CHeapPtr<BYTE> apBuffer;
	apBuffer.Allocate(WriteLength);
	memset(apBuffer.m_pData, 0xc, WriteLength);

	//////////////////////////////////////////////////////////////////////////
	hr = WriteAtOffset(ahVolume, FirstWriteOffset, apBuffer.m_pData, WriteLength);
	if (FAILED(hr))
	{
		//LEAVE_BLOCK(0);
	}
	else
	{
		PRINT_LINE(TEXT("write some data at %I64d, length=%d"), FirstWriteOffset, WriteLength);
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG loopOffset = EveryWriteInterval;
	for (;;)
	{
		hr = WriteAtOffset(ahVolume, loopOffset, apBuffer.m_pData, WriteLength);
		if (FAILED(hr))
		{
			break;
		}
		else
		{
			PRINT_LINE(TEXT("write some data at %I64d, length=%d"), loopOffset, WriteLength);
		}

		loopOffset += EveryWriteInterval;
	}

	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


VOID PrintHelp()
{
	PRINT_LINE(TEXT("\r\n this.exe <VolumeName> <FirstWriteOffset> <EveryWriteInterval> <WriteLength>"));
	PRINT_LINE(TEXT("\r\n Example:"));
	PRINT_LINE(TEXT("\t this.exe q: 0 0 0"));
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 5)
	{
		PrintHelp();
		return E_INVALIDARG;
	}

	CONST LONGLONG __KB = 1024;
	CONST LONGLONG __MB = 1024 * __KB;
	CONST LONGLONG __GB = 1024 * __MB;

	LPCTSTR VolumeName = argv[1];
	LONGLONG FirstWriteOffset = _tcstoll(argv[2], nullptr, 0);
	if (FirstWriteOffset == 0)
	{
		FirstWriteOffset = 100 * __MB;
	}
	LONGLONG EveryWriteInterval = _tcstoll(argv[3], nullptr, 0);
	if (EveryWriteInterval == 0)
	{
		EveryWriteInterval = __GB;
	}

	LONG WriteLength = _tcstol(argv[4], nullptr, 0);
	if (WriteLength == 0)
	{
		WriteLength = 16 * __KB;
	}
	HRESULT hr = CorruptVolume(VolumeName, FirstWriteOffset, EveryWriteInterval, WriteLength);
	return hr;
}

