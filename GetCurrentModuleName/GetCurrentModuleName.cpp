#include "stdafx.h"

LPCTSTR GetCurrentModuleName()
{
	static HMODULE s_hModule = NULL;
	static TCHAR s_ModuleName[MAX_PATH + MAX_PATH] = {0};
	static CComAutoCriticalSection s_CritSec;

	HRESULT hr = S_OK;

	if (NULL == s_hModule)
	{
		CComCritSecLock<CComAutoCriticalSection> Lock(s_CritSec, FALSE);
		Lock.Lock();
		if (NULL == s_hModule) //check again
		{
			MEMORY_BASIC_INFORMATION mbi  = {0};
			SIZE_T Size = VirtualQuery(
				(LPVOID)(GetCurrentModuleName),//_In_opt_  LPCVOID lpAddress,
				&mbi,//_Out_     PMEMORY_BASIC_INFORMATION lpBuffer,
				sizeof(MEMORY_BASIC_INFORMATION)//_In_      SIZE_T dwLength
				);
			if (Size)
			{
				HMODULE hModuleTemp = (HMODULE)(mbi.AllocationBase);

				//////////////////////////////////////////////////////////////////////////
				TCHAR ExeName[MAX_PATH] = {0};
				GetModuleFileName(
					NULL,//_In_opt_  HMODULE hModule,
					ExeName,//_Out_     LPTSTR lpFilename,
					_countof(ExeName)//_In_      DWORD nSize
					);
				LPCTSTR pExe = _tcsrchr(ExeName, TEXT('\\'));
				if (pExe)
				{
					++ pExe;
				}

				//////////////////////////////////////////////////////////////////////////
				TCHAR ModName[MAX_PATH] = {0};
				GetModuleFileName(
					hModuleTemp,//_In_opt_  HMODULE hModule,
					ModName,//_Out_     LPTSTR lpFilename,
					_countof(ModName)//_In_      DWORD nSize
					);
				LPCTSTR pMod = _tcsrchr(ModName, TEXT('\\'));
				if (pMod)
				{
					++ pMod;
				}

				//////////////////////////////////////////////////////////////////////////
				if (pExe)
				{
					_tcscat_s(s_ModuleName, pExe);
					_tcscat_s(s_ModuleName, TEXT("!"));
				}
				if (pMod)
				{
					_tcscat_s(s_ModuleName, pMod);
				}

				s_hModule = hModuleTemp; //it is the last step to assign s_hModule value
			}
		}
	}


	return s_ModuleName;
}