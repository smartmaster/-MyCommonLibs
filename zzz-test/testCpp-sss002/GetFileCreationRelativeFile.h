#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "log.h"
#include "code_block.h"

namespace SmartLib
{
	class CGetFileCreationRelativeFile
	{
	public:
		//return value mean the file was created ### seconds ago
		static LONGLONG GetFileCreationRelativeFile(LPCTSTR fileName)
		{

			HRESULT hr = S_OK;
			DWORD LastError = 0;

			LONGLONG rv = 0L;
			HANDLE hfile = INVALID_HANDLE_VALUE; /***CLEANUP***/
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			hfile = ::CreateFile(
				fileName,//_In_     LPCTSTR               lpFileName,
				GENERIC_READ,//_In_     DWORD                 dwDesiredAccess,
				FILE_SHARE_READ,//_In_     DWORD                 dwShareMode,
				nullptr,//_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				OPEN_EXISTING,//_In_     DWORD                 dwCreationDisposition,
				0,//_In_     DWORD                 dwFlagsAndAttributes,
				nullptr//_In_opt_ HANDLE                hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == hfile)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LOG_LINE(TEXT("CreateFile failed, [fileName=%s, hr=0x%08x, LastError=%d]"), fileName, hr, LastError);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			FILETIME CreationTime = { 0 };
			FILETIME LastAccessTime = { 0 };
			FILETIME LastWriteTime = { 0 };
			BOOL bget = ::GetFileTime(
				hfile,//_In_      HANDLE     hFile,
				&CreationTime,//_Out_opt_ LPFILETIME lpCreationTime,
				&LastAccessTime,//_Out_opt_ LPFILETIME lpLastAccessTime,
				&LastWriteTime//_Out_opt_ LPFILETIME lpLastWriteTime
			);
			if (!bget)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LOG_LINE(TEXT("GetFileTime failed, [fileName=%s, hr=0x%08x, LastError=%d]"), fileName, hr, LastError);
				LEAVE_BLOCK(0);
			}

			SYSTEMTIME st = { 0 };
			::GetSystemTime(&st);
			FILETIME ft = { 0 };
			::SystemTimeToFileTime(&st, &ft);

			ULARGE_INTEGER liFile;
			liFile.LowPart = CreationTime.dwLowDateTime;
			liFile.HighPart = CreationTime.dwHighDateTime;

			ULARGE_INTEGER licurrent;
			licurrent.LowPart = ft.dwLowDateTime;
			licurrent.HighPart = ft.dwHighDateTime;

			CONST LONGLONG __second = 10'000'000LL;

			rv = (licurrent.QuadPart - liFile.QuadPart) / __second;
			LOG_LINE(TEXT("File was craeted %I64d ago [fileName=%s]"), rv, fileName);
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			/***CLEANUP***/
			if (hfile != INVALID_HANDLE_VALUE)
			{
				::CloseHandle(hfile);
				hfile = INVALID_HANDLE_VALUE;
			}


			return rv;
		}
	};
}