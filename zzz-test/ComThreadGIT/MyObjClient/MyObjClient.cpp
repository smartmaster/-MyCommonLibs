// MyObjClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
using namespace std;


#include "..\..\..\PublicHeader\PublicCodeBlock.h"
#include "..\..\..\PublicHeader\PublicResourceAnnotation.h"
#include "..\..\..\PublicHeader\PublicComInit.h"
#include "../MyObj/MyObj_i.h"

#define PRINT_LINE(sss, ...) _ftprintf_s(stdout, sss TEXT("\r\n"), __VA_ARGS__)

namespace
{

#if 0
	//////////////////////////////////////////////////////////////////////////
	class CComInit
	{
	public:
		CComInit(DWORD dwCoInit)
		{
			CoInitializeEx(nullptr, dwCoInit);
		}

		~CComInit()
		{
			CoUninitialize();
		}
	};

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
			//D_API_ERR(0, TEXT(""), hr, TEXT("Attach"));
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
			//D_API_ERR(0, TEXT("Attach"), hr, TEXT(""));
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
			//D_API_ERR(0, TEXT("CopyTo"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);
		
		return hr;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	static DWORD WINAPI ThreadProc(
		_In_  LPVOID lpParameter
		)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		CComInit comInit(COINIT_MULTITHREADED);
		
		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		DWORD dwCokkie = (DWORD)(lpParameter);
		CComPtr<ISimpleObj> spISimpleObj;
#if 1
		SmartLib::CookieToComInterface(dwCokkie, &spISimpleObj);
#else
		CComGITPtr<ISimpleObj> gitPtr;
		hr = gitPtr.Attach(dwCokkie); RESOURCE_NEW_OWER(dwCokkie);

		//////////////////////////////////////////////////////////////////////////
		
		hr = gitPtr.CopyTo(&spISimpleObj);
		//gitPtr.Detach();
#endif

		
		//////////////////////////////////////////////////////////////////////////
		spISimpleObj->Method();
		
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);
		
		return hr;

	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	CComInit comInit(COINIT_MULTITHREADED);
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<ISimpleObj> spISimpleObj;
	hr = spISimpleObj.CoCreateInstance(__uuidof(SimpleObj));
	
	//////////////////////////////////////////////////////////////////////////
	LONG ThreadCount = 3;
	vector<HANDLE> hThreads(ThreadCount);
	auto_deleter_func(hThreads) = [](vector<HANDLE> * phThreads)
	{
		if (phThreads)
		{
			for (auto & var : *phThreads)
			{
				CloseHandle(var);
				var = nullptr;
			}
			phThreads->clear();
		}
	};
	auto_delete_obj(hThreads);
	for (size_t ii = 0; ii < hThreads.size(); ++ ii)
	{
		//////////////////////////////////////////////////////////////////////////
#if 1
		DWORD dwCookie = 0;
		SmartLib::ComInterfaceToCookie(spISimpleObj.p, dwCookie);
#else
		CComGITPtr<ISimpleObj> gitPtr;
		hr = gitPtr.Attach(spISimpleObj);
		DWORD dwCookie = gitPtr.Detach(); RESOURCE_CHANGE_OWNER(dwCookie);
#endif

		DWORD ThreadId = 0;
		hThreads[ii] = CreateThread(
			nullptr,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//_In_       SIZE_T dwStackSize,
			(LPTHREAD_START_ROUTINE)ThreadProc,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
			(LPVOID)dwCookie,//_In_opt_   LPVOID lpParameter,
			0,//_In_       DWORD dwCreationFlags,
			&ThreadId//_Out_opt_  LPDWORD lpThreadId
			);
	}

	//////////////////////////////////////////////////////////////////////////
	spISimpleObj->Method();

	//////////////////////////////////////////////////////////////////////////
	WaitForMultipleObjects(hThreads.size(), &hThreads[0], TRUE, INFINITE);
	//for (auto & hThread : hThreads)
	//{
	//	CloseHandle(hThread);
	//	hThread = nullptr;
	//}
	//hThreads.clear();
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

