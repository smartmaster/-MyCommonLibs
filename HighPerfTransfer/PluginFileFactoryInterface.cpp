#include "stdafx.h"

#include "PluginFileFactoryInterface.h"

namespace
{
	static CONST CHAR CreateInstanceIPluginFileFactory_API_NAME[] = "CreateInstanceIPluginFileFactory";
	typedef HRESULT(*pfnt_CreateInstanceIPluginFileFactory)(IPluginFileFactory ** pp);
} //end namespace


HRESULT CreateInstanceIPluginFileFactoryFromModule(LPCTSTR modulePath, LPCTSTR apiName, IPluginFileFactory ** pp)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	HMODULE hModule = GetModuleHandle(
		modulePath//_In_opt_  LPCTSTR lpModuleName
		);
	if (nullptr == hModule)
	{
		LastError = GetLastError();
		D_API_ERR(0, TEXT("GetModuleHandle"), LastError, TEXT(""));
		hModule = LoadLibrary(modulePath);
		if (nullptr == hModule)
		{
			//LastError = WSAGetLastError();
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("LoadLibrary"), hr, TEXT("%s"), modulePath);
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CStringA apiNameA(apiName);
	pfnt_CreateInstanceIPluginFileFactory pfn_CreateInstanceIPluginFileFactory = (pfnt_CreateInstanceIPluginFileFactory)GetProcAddress(
		hModule,//_In_  HMODULE hModule,
		apiNameA.GetLength() ? apiNameA.GetString() : CreateInstanceIPluginFileFactory_API_NAME //_In_  LPCSTR lpProcName
		);
	if (nullptr == pfn_CreateInstanceIPluginFileFactory)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetProcAddress"), hr, TEXT("%s"), apiName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = pfn_CreateInstanceIPluginFileFactory(pp);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("pfn_CreateInstanceIPluginFileFactory"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}
