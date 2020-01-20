#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <stdlib.h>

namespace SmartLib
{
	class CTestApacheCrash
	{
	public:
		static BOOL apxAddToPathW(/*APXHANDLE hPool, */LPCWSTR szAdd)
		{

			typedef int (*WPUTENV)(
				const wchar_t* envstring
			);
			LPWSTR wsAdd;
			DWORD  rc;
			DWORD  al;
			HMODULE hmodUcrt;
			WPUTENV wputenv_ucrt = (WPUTENV)(void*)(1);

			rc = GetEnvironmentVariableW(L"PATH", NULL, 0);
			if (rc == 0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND)
				return FALSE;
			al = 5 + lstrlenW(szAdd) + 1;
			//if (!(wsAdd = apxPoolAlloc(hPool, (al + rc) * sizeof(WCHAR))))
			if (!(wsAdd = (LPWSTR)::malloc((al + rc) * sizeof(WCHAR))))
				return FALSE;
			lstrcpyW(wsAdd, L"PATH=");
			lstrcatW(wsAdd, szAdd);
			lstrcatW(wsAdd, L";");
			if (GetEnvironmentVariableW(L"PATH", wsAdd + al, rc) != rc - 1) {
				//apxLogWrite(APXLOG_MARK_SYSERR);
				::free(wsAdd);
				return FALSE;
			}

			hmodUcrt = LoadLibraryExA("ucrtbase.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
			if (hmodUcrt != NULL) {
				wputenv_ucrt = (WPUTENV)GetProcAddress(hmodUcrt, "_wputenv");
			}

			SetEnvironmentVariableW(L"PATH", wsAdd + 5);
			_wputenv(wsAdd);
			if (wputenv_ucrt != NULL) {
				wputenv_ucrt(wsAdd);
			}

			::free(wsAdd);
			return TRUE;
		}
	};
}