#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"



class CComInit
{
private:
	HRESULT m_hr{E_FAIL};

public:
	CComInit(DWORD dwCoInit = COINIT_MULTITHREADED)
	{
		m_hr = CoInitializeEx(NULL, dwCoInit);
	}

	~CComInit()
	{
		if (SUCCEEDED(m_hr))
		{
			CoUninitialize();
		}
	}

	HRESULT GetHRESULT()
	{
		return m_hr;
	}
};


namespace SmartLib
{
	template<typename ITYPE>
	HRESULT ComInterfaceToCookie(IN ITYPE * pObj, OUT DWORD & cookie)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		CComGITPtr<ITYPE> gitPtr;
		hr = gitPtr.Attach(pObj);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT(""), hr, TEXT("Attach"));
			LEAVE_BLOCK(0);
		}

		cookie = gitPtr.Detach();
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	template<typename ITYPE>
	HRESULT CookieToComInterface(IN DWORD cookie, OUT ITYPE ** ppObj)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		CComGITPtr<ITYPE> gitPtr;
		hr = gitPtr.Attach(cookie);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Attach"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = gitPtr.CopyTo(ppObj);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("CopyTo"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}
} //namespace SmartLib