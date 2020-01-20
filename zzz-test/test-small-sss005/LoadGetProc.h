#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#ifndef BEGIN_BLOCK

#define BEGIN_BLOCK(level_number)			do{	int	_abc_xyz##level_number(level_number);
#define LEAVE_BLOCK(level_number)			{(_abc_xyz##level_number);}	break;
#define END_BLOCK(level_number)				{(_abc_xyz##level_number);}	}while(0);

#endif



namespace SmartLib
{
	class LoadGetProc
	{
	public:
		static HRESULT Do(LPCTSTR dllName, LPCTSTR procName)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			HMODULE hmod = nullptr; /***CLEANUP***/
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			hmod = ::LoadLibrary(dllName);
			if (nullptr == hmod)
			{
				LastError = GetLastError();
				wprintf_s(L"LoadLibrary failed [dll=%s, LastError=%d]" L"\r\n", dllName, LastError);
				LEAVE_BLOCK(0);
			}
			wprintf_s(L"Successfully LoadLibrary [dll=%s]" L"\r\n", dllName);


			//////////////////////////////////////////////////////////////////////////
			CHAR procNameA[128] = { 0 };
			sprintf_s(procNameA, "%S", procName);
			FARPROC proc = ::GetProcAddress(hmod, procNameA);
			if (nullptr == proc)
			{
				LastError = GetLastError();
				wprintf_s(L"GetProcAddress failed [procNameA=%S, LastError=%d]" L"\r\n", procNameA, LastError);
				LEAVE_BLOCK(0);
			}
			wprintf_s(L"Successfully GetProcAddress [procNameA=%S]" L"\r\n", procNameA);

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			if (hmod)
			{
				::FreeLibrary(hmod);
				hmod = nullptr;
			}
			
			return hr;
			
		}
	};
}