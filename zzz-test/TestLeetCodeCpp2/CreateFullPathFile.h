#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <iostream>
using namespace std;

#include <cstdio>

#include "codeBlock.h"

namespace SmartLib
{
	class CreateFullPathFile
	{
	public:
		static void LogError(LPCWSTR message, ...)
		{
			va_list args;
			va_start(args, message);
			vwprintf_s(message, args);
			va_end(args);
		}

		static HRESULT CreateFullDirectory(LPCTSTR fullPathFile)
		{

			HRESULT hr = S_OK;
			DWORD lastError = ERROR_SUCCESS;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			TCHAR szVolumePathName[MAX_PATH + MAX_PATH] = { 0 };
			BOOL gvpn =  ::GetVolumePathName(
				fullPathFile,//_In_  LPCTSTR lpszFileName,
				szVolumePathName,//_Out_ LPTSTR  lpszVolumePathName,
				_countof(szVolumePathName)//_In_  DWORD   cchBufferLength
			);
			if (!gvpn)
			{
				lastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				LogError(L"GetVolumePathName, hr=0x%08x, lastError=%d, [path=%s]", hr, lastError, fullPathFile);
				LEAVE_BLOCK(0);
			}

			auto p1 = fullPathFile;
			auto p2 = szVolumePathName;
			while (*p2 != 0)
			{
				++p1;
				++p2;
			}

			for (;;)
			{
				if (*p1 == 0)
				{
					break;
				}

				while (*p1 != TEXT('\\') && *p1 != TEXT('/') && *p1 != 0)
				{
					*p2++ = *p1++;
				}

				bool cd = ::CreateDirectory(szVolumePathName, nullptr);
				if (!cd)
				{
					lastError = ::GetLastError();
					if (lastError != ERROR_ALREADY_EXISTS)
					{
						hr = HRESULT_FROM_WIN32(lastError);
						LogError(L"CreateDirectory, hr=0x%08x, lastError=%d, [path=%s]", hr, lastError, szVolumePathName);
						break;
					}
				}

				if (*p1 != 0)
				{
					*p2++ = *p1++;
				}
				
			}
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}




			END_BLOCK(0);


			return hr;
		}

		static HRESULT CreateFullDirectoryFile(LPCTSTR dir, LPCTSTR file)
		{
			HRESULT hr = S_OK;
			DWORD lastError = ERROR_SUCCESS;
			BEGIN_BLOCK(0);

			hr = CreateFullDirectory(dir);
			if (FAILED(hr))
			{
				LogError(L"CreateFullDirectory, hr=0x%08x, lastError=%d, [path=%s]", hr, lastError, dir);
				LEAVE_BLOCK(0);
			}

			TCHAR fullDirFile[MAX_PATH + MAX_PATH] = { 0 };
			_tcscpy_s(fullDirFile, dir);
			_tcscat_s(fullDirFile, TEXT("\\"));
			_tcscat_s(fullDirFile, file);
			HANDLE hFile =  ::CreateFile(
				fullDirFile,//_In_     LPCTSTR               lpFileName,
				GENERIC_ALL,//_In_     DWORD                 dwDesiredAccess,
				0,//_In_     DWORD                 dwShareMode,
				nullptr,//_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				CREATE_ALWAYS,//_In_     DWORD                 dwCreationDisposition,
				0,//_In_     DWORD                 dwFlagsAndAttributes,
				nullptr//,//_In_opt_ HANDLE                hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == hFile)
			{
				lastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				LogError(L"CreateFile, hr=0x%08x, lastError=%d, [path=%s]", hr, lastError, fullDirFile);
				LEAVE_BLOCK(0);
			}

			char data[] = "1234567890";
			DWORD      NumberOfBytesWritten = 0;
			BOOL wf = ::WriteFile(
				hFile,//_In_        HANDLE       hFile,
				data,//_In_        LPCVOID      lpBuffer,
				sizeof(data) - 1,//_In_        DWORD        nNumberOfBytesToWrite,
				&NumberOfBytesWritten,//_Out_opt_   LPDWORD      lpNumberOfBytesWritten,
				nullptr//,//_Inout_opt_ LPOVERLAPPED lpOverlapped
			);
			if (!wf)
			{
				lastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				LogError(L"WriteFile, hr=0x%08x, lastError=%d", hr, lastError);
				//LEAVE_BLOCK(0);
			}
			
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;

			END_BLOCK(0);

			return hr;
		}


		
	};


	class CreateFullPathFileTest
	{
	public:
		static void Case0()
		{
			//TCHAR fullDir[] = TEXT("d:\\vol-f\\zzz-temp-fff\\Logs 11-04-2018\\111\\222\\333\\");
			TCHAR fullDir[] = TEXT("\\\\?\\zzz\\Volume{2a941544-0083-11e6-80c6-90b11c900c8c}\\zzz-temp-fff\\Logs 11-04-2018\\111\\222\\333\\444");
			//TCHAR fullDir[] = TEXT("\\\\songmin-9010\\f$\\zzz-temp-fff\\Logs 11-04-2018\\111\\222\\333\\444");
			//TCHAR fullDir[] = TEXT("\\\\?\\UNC\\songmin-9010\\f$\\zzz-temp-fff\\Logs 11-04-2018\\ttt\\111\\222\\333\\444");
			//CreateFullPathFile::CreateFullDirectory(fullDir);

			TCHAR file[] = TEXT("sss.txt");
			CreateFullPathFile::CreateFullDirectoryFile(fullDir, file);

		}


		static void Case1(int argc, TCHAR ** argv)
		{
			if (argc < 3)
			{
				CreateFullPathFile::LogError(L"<this.exe> <dir name> <file name>" L"\r\n");
				return;
			}

			CreateFullPathFile::CreateFullDirectoryFile(argv[1], argv[2]);
		}
	};
}