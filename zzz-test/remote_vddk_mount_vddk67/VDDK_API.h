#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\..\PublicHeader\PublicCodeBlock.h"
#include "PRINT_LINE.h"
#include "vixDiskLib.h"

class VDDK_API
{
private:
	
	HMODULE _vixDiskLib{ nullptr };
	BOOL _vixDiskLibLoaded{ FALSE };
public:
	/**
	* Free the connection details structure allocated during
	* VixDiskLib_GetConnectParams or VixDiskLib_AllocateConnectParams.
	* @param connectParams [out] Connection details to be free'ed.
	* @return None.
	*/
	typedef void(*VixDiskLib_FreeConnectParams_t)(VixDiskLibConnectParams* connectParams);
	VixDiskLib_FreeConnectParams_t VixDiskLib_FreeConnectParams{ nullptr };

	/**
	* Allocate the connection details structure
	* @return A pointer to the instance of connection details; NULL if error.
	*/
	typedef VixDiskLibConnectParams * (*VixDiskLib_AllocateConnectParams_t)();
	VixDiskLib_AllocateConnectParams_t VixDiskLib_AllocateConnectParams{ nullptr };

	HRESULT Init()
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;
		
		BEGIN_BLOCK(0);

		static const LPCTSTR s_moduleName  = TEXT("vixDiskLib.dll");
		//////////////////////////////////////////////////////////////////////////
		_vixDiskLib = ::GetModuleHandle(
			s_moduleName//_In_opt_ LPCTSTR lpModuleName
		);
		if (nullptr == _vixDiskLib)
		{
			_vixDiskLib = ::LoadLibrary(
				s_moduleName//_In_ LPCTSTR lpFileName
			);
			if (nullptr == _vixDiskLib)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				PRINT_LINE_A("LoadLibrary failed with hr=0x%08x, lastError=%d", hr, LastError);
				LEAVE_BLOCK(0);
			}
			_vixDiskLibLoaded = TRUE;
		}

		//////////////////////////////////////////////////////////////////////////
		VixDiskLib_FreeConnectParams = (VixDiskLib_FreeConnectParams_t)::GetProcAddress(_vixDiskLib, "VixDiskLib_FreeConnectParams");
		if (nullptr == VixDiskLib_FreeConnectParams)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			PRINT_LINE_A("GetProcAddress [%s] failed with hr=0x%08x, lastError=%d", "VixDiskLib_FreeConnectParams", hr, LastError);
		}


		VixDiskLib_AllocateConnectParams = (VixDiskLib_AllocateConnectParams_t)::GetProcAddress(_vixDiskLib, "VixDiskLib_AllocateConnectParams");
		if (nullptr == VixDiskLib_AllocateConnectParams)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			PRINT_LINE_A("GetProcAddress [%s] failed with hr=0x%08x, lastError=%d", "VixDiskLib_AllocateConnectParams", hr, LastError);
		}
		
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);
		
		return hr;
	}


	~VDDK_API()
	{
		if (_vixDiskLibLoaded)
		{
			::FreeLibrary(_vixDiskLib);
			_vixDiskLibLoaded = FALSE;
			_vixDiskLib = nullptr;
		}
	}
};