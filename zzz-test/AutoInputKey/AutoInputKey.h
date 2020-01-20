#pragma once

#include <Windows.h>
#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>

#include <iostream>

#ifndef BEGIN_BLOCK
#define BEGIN_BLOCK(level_number)			do{	int	_abc_xyz##level_number(level_number);
#define LEAVE_BLOCK(level_number)			{(_abc_xyz##level_number);}	break;
#define END_BLOCK(level_number)				{(_abc_xyz##level_number);}	}while(0);
#endif

namespace SmartLib
{
	struct CPrint
	{
		template<typename T0, typename... TMore>
		static void Print(CONST T0 & val, CONST TMore & ... more)
		{
			std::wcout << val << L", ";
			Print(more...);
		}

		template<typename T0>
		static void Print(CONST T0 & val)
		{
			std::wcout << val << std::endl;
		}

		static void Print()
		{
			std::wcout << std::endl;
		}
	};

	struct CAutoKeyInput
	{
	private:
		static void MySleep(int sec)
		{
			for (int ii = 0; ii < sec; ++ii)
			{
				::Sleep(1000);
			}
		}

	private:
		static HRESULT PressKey(WORD vKey)
		{

			HRESULT hr = S_OK;
			DWORD lastError = 0;

			INPUT Inputs[2] = { 0 };
			ZeroMemory(Inputs, sizeof(Inputs));

			Inputs[0].type = INPUT_KEYBOARD;
			Inputs[0].ki.wVk = vKey;

			Inputs[1].type = INPUT_KEYBOARD;
			Inputs[1].ki.wVk = vKey;
			Inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;



			UINT nsi = SendInput(
				_countof(Inputs),//_In_ UINT    nInputs,
				Inputs,//_In_ LPINPUT pInputs,
				sizeof(INPUT)//_In_ int     cbSize
				);
			if (0 == nsi)
			{
				lastError = GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				CPrint::Print(L"error SendInput", lastError, hr);
			}
			return hr;
		}

	private:
		static HRESULT FindAndActivateChildWindow(
			HWND hParent,
			LPCTSTR childClass,
			LPCTSTR childWindow)
		{
			HRESULT hr = S_OK;
			DWORD lastError = 0;

			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			if (nullptr == childClass || 0 == childClass[0] || nullptr == childWindow || 0 == childWindow[0])
			{
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			LPCTSTR atom = 0;
			if (nullptr != childClass && TEXT('#') == childClass[0])
			{
				atom = (LPCTSTR)_tcstol(childClass + 1, nullptr, 10);
			}

			HWND hwnd = FindWindowEx(
				hParent,//_In_opt_ HWND    hwndParent,
				nullptr,//_In_opt_ HWND    hwndChildAfter,
				atom ? atom : childClass,//_In_opt_ LPCTSTR lpszClass,
				childWindow//_In_opt_ LPCTSTR lpszWindow
			);
			if (nullptr == hwnd)
			{
				lastError = GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				CPrint::Print(L"error FindWindowEx", childClass, lastError, hr);
				if (SUCCEEDED(hr))
				{
					hr = HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
				}
				LEAVE_BLOCK(0);
			}
			CPrint::Print(L"WIndow found!!!", childWindow, lastError, hr);

			//////////////////////////////////////////////////////////////////////////
			BOOL bpm = SendMessage(
				hwnd,//_In_opt_ HWND   hWnd,
				WM_SETFOCUS,//_In_     UINT   Msg,
				0,//_In_     WPARAM wParam,
				0//_In_     LPARAM lParam
			);
			if (!bpm)
			{
				lastError = GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				CPrint::Print(L"error FindWindowEx", childClass, lastError, hr);
				if (SUCCEEDED(hr))
				{
					hr = HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
				}
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

		static HRESULT FindAndActivateWindow(
			LPCTSTR lpszClass,
			LPCTSTR lpszWindow,
			LPCTSTR childClass,
			LPCTSTR childWindow
			)
		{
			HRESULT hr = S_OK;
			DWORD lastError = 0;

			LPCTSTR atom = 0;
			if (nullptr != lpszClass && TEXT('#') == lpszClass[0])
			{
				atom = (LPCTSTR)_tcstol(lpszClass + 1, nullptr, 10);
			}

			

			BEGIN_BLOCK(0);
			///////////////////////////////////////////////////////
			HWND hwnd = FindWindowEx(
				nullptr,//_In_opt_ HWND    hwndParent,
				nullptr,//_In_opt_ HWND    hwndChildAfter,
				atom? atom : lpszClass,//_In_opt_ LPCTSTR lpszClass,
				lpszWindow//_In_opt_ LPCTSTR lpszWindow
				);
			if (nullptr == hwnd)
			{
				lastError = GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				CPrint::Print(L"error FindWindowEx", lpszClass, lastError, hr);
				if (SUCCEEDED(hr))
				{
					hr = HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
				}
				LEAVE_BLOCK(0);
			}
			CPrint::Print(L"WIndow found!!!", lpszClass, lastError, hr);

			///////////////////////////////////////////////////////
			BOOL bShowWindowAsyncDebug = FALSE;
			BOOL bRet = FALSE;
			if (bShowWindowAsyncDebug)
			{
				bRet = ShowWindowAsync(
					hwnd,//_In_ HWND hWnd,
					SW_SHOWNORMAL//_In_ int  nCmdShow
					);
				::Sleep(3000);
			}
			else
			{
				bRet = ShowWindow(
					hwnd,//_In_ HWND hWnd,
					SW_SHOWNORMAL//_In_ int  nCmdShow
					);
			}
			if (!bRet)
			{
				lastError = GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				CPrint::Print(L"error ShowWindow", lastError, hr);
				//LEAVE_BLOCK(0);
			}
			


			bRet = BringWindowToTop(
				hwnd//_In_ HWND hWnd
				);
			if (!bRet)
			{
				lastError = GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				CPrint::Print(L"error BringWindowToTop", lastError, hr);
				//LEAVE_BLOCK(0);
			}


			bRet = SetForegroundWindow(
				hwnd//_In_ HWND hWnd
				);
			if (!bRet)
			{
				lastError = GetLastError();
				hr = HRESULT_FROM_WIN32(lastError);
				CPrint::Print(L"error BringWindowToTop", lastError, hr);
				//LEAVE_BLOCK(0);
			}


			FindAndActivateChildWindow(hwnd, childClass, childWindow);

			
			///////////////////////////////////////////////////////
			hr = PressKey(VK_RETURN);
			if (FAILED(hr))
			{
				CPrint::Print(L"error PressKey", hr);
				LEAVE_BLOCK(0);
			}


			END_BLOCK(0);

			return hr;
		}

	public:
		static void WorkerProc(
			LPCTSTR lpszClass,
			LPCTSTR lpszWindow,
			LPCTSTR childClass,
			LPCTSTR childWindow
			)
		{
			for (;;)
			{
				MySleep(5);
				HRESULT hr = FindAndActivateWindow(lpszClass, lpszWindow, childClass, childWindow);
				if (SUCCEEDED(hr))
				{
					//MySleep(30);
				}
			}
		}
	};
} //end namespace SmartLib