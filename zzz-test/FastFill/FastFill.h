#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlsecurity.h>


#ifndef SML_BEGIN_BLOCK

#define SML_BEGIN_BLOCK(level_number)			do{	int	sml_1qaz_2wsx_3edc_4rfv_ ## level_number = (level_number) + (__LINE__) - (level_number);
#define SML_LEAVE_BLOCK(level_number)			{(sml_1qaz_2wsx_3edc_4rfv_ ## level_number);}	break;
#define SML_END_BLOCK(level_number)				{(sml_1qaz_2wsx_3edc_4rfv_ ## level_number);}	}while(0);


#define  BEGIN_BLOCK SML_BEGIN_BLOCK
#define  LEAVE_BLOCK SML_LEAVE_BLOCK
#define  END_BLOCK SML_END_BLOCK


#define LOG_LINE(format, ...) _ftprintf_s(stdout, format TEXT("\r\n"), __VA_ARGS__)

#endif


namespace SmartLib
{
	class FastFill
	{
	public:
		static HRESULT FastCreatFile(LPCTSTR fileName, LONGLONG size)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;


			HANDLE hFile = INVALID_HANDLE_VALUE; /***CLEANUP***/
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			hFile = ::CreateFile(
				fileName,//_In_      LPCTSTR lpFileName,
				GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
				0,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
				0,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == hFile)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LOG_LINE(TEXT("CreateFile failed [file=%s, error=%d, hr=0x%08x]"), fileName, LastError, hr);
				LEAVE_BLOCK(0);
			}

			LARGE_INTEGER liDistanceToMove;
			liDistanceToMove.QuadPart = size;
			LARGE_INTEGER NewFilePointer;
			NewFilePointer.QuadPart = 0LL;
			BOOL bret = ::SetFilePointerEx(
				hFile,//_In_       HANDLE hFile,
				liDistanceToMove,//_In_       LARGE_INTEGER liDistanceToMove,
				&NewFilePointer,//_Out_opt_  PLARGE_INTEGER lpNewFilePointer,
				FILE_BEGIN//,//_In_       DWORD dwMoveMethod
			);
			if (!bret)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LOG_LINE(TEXT("SetFilePointerEx failed [file=%s, size=%I64d, error=%d, hr=0x%08x]"), fileName, size, LastError, hr);
				LEAVE_BLOCK(0);
			}

			bret = ::SetEndOfFile(
				hFile
			);
			if (!bret)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LOG_LINE(TEXT("SetEndOfFile failed [file=%s, size=%I64d, error=%d, hr=0x%08x]"), fileName, size, LastError, hr);
				LEAVE_BLOCK(0);
			}


			bret =  ::SetFileValidData(
				hFile,//_In_  HANDLE hFile,
				size//,//_In_  LONGLONG ValidDataLength
			);
			if (!bret)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LOG_LINE(TEXT("SetFileValidData failed [file=%s, size=%I64d, error=%d, hr=0x%08x]"), fileName, size, LastError, hr);
				LEAVE_BLOCK(0);
			}
			 
			 			 						
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			if (INVALID_HANDLE_VALUE != hFile)
			{
				::CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;
			}
			
			return hr;
		}


		static void FastFillProc(LPCTSTR folder, LONGLONG startNumber, LONGLONG size)
		{
			
			CAccessToken accTok;
			CTokenPrivileges PreviousState;
			BOOL bRet = accTok.GetEffectiveToken(TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY);
			if (bRet)
			{
				bRet = accTok.EnablePrivilege(SE_MANAGE_VOLUME_NAME, &PreviousState);
				LOG_LINE(TEXT("EnablePrivilege %s, bRet=%d"), SE_MANAGE_VOLUME_NAME, bRet);
			}
			else
			{
				LOG_LINE(TEXT("EnablePrivilege failed %s, bRet=%d"), SE_MANAGE_VOLUME_NAME, bRet);
			}

			for (;;)
			{
				CString strfile = folder;
				if (strfile[strfile.GetLength() - 1] == TEXT('\\'))
				{
					strfile.AppendFormat(TEXT("%016I64d.ffdata"), startNumber);
				}
				else
				{
					strfile.AppendFormat(TEXT("\\%016I64d.ffdata"), startNumber);
				}

				HRESULT hr = FastCreatFile(strfile, size);
				if (FAILED(hr))
				{
					
					LOG_LINE(TEXT("FastCreatFile failed, end operation [file=%s, size=%I64d]"), strfile.GetString(), size);
					break;
				}

				++startNumber;
			}
		}
	};
}