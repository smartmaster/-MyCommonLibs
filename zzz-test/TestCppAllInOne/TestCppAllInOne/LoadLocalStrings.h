#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <string>
#include <fstream>
#include <locale>
#include <codecvt>
#include <map>

#include "CodeBlock.h"
#include "../MUI/MUI.h"

namespace SmartLib
{
	class CLoadLocalStrings
	{
	public:

		static void Case0(LPCWSTR filename)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			std::map<int, std::wstring> map =
			{
				{ ::GetSystemDefaultLangID(), L"Default" },
				{0x404, L"Chinese TW"},
				{0x407, L"German"},
				{0x409, L"English"},
				{0x40A, L"Spanish"},
				{0x40C, L"French"},
				{0x410, L"Italian"},
				{0x411, L"Japanese"},
				{0x412, L"Korean"},
				{0x416, L"Po BR"},
				{0x804, L"Chinese"},
			};

			LPCTSTR mui_dll = TEXT("mui.dll");
			HMODULE hModule = GetModuleHandle(mui_dll);
			if (nullptr == hModule)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				hModule = ::LoadLibraryEx(mui_dll, nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
				if (nullptr == hModule)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					LEAVE_BLOCK(0);
				}
			}

			//map.insert({ ::GetSystemDefaultLangID(), L"Default" });
			std::wofstream of(filename);
			of.imbue(std::locale(of.getloc(), new std::codecvt_utf8_utf16<WCHAR>()));
			for (auto pair : map)
			{
				of << "--------" << pair.first << "++++++++" << pair.second << "--------" << std::endl;
				Case_proc(hModule, of, pair.first);
				of << std::endl;
			}
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			//return hr;
		}

		static void Case_proc(HMODULE hModule, std::wofstream & of, LANGID langid)
		{
			//std::wofstream of(filename);
			//of.imbue(std::locale(of.getloc(), new std::codecvt_utf8_utf16<WCHAR>()));
			//LANGID langid = ::GetSystemDefaultLangID();

			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
#if 0
			LPCTSTR mui_dll = TEXT("mui.dll");
			HMODULE hModule = GetModuleHandle(mui_dll);
			if (nullptr == hModule)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				hModule = ::LoadLibraryEx(mui_dll, nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
				if (nullptr == hModule)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					LEAVE_BLOCK(0);
				}
			}
#endif

			for (int ii = HBKSTUB_CBT_SERVICE_NAME; ii <= HBKVM_IC_SERVICE_HELP_RESULT_FAILED_UNINSTALL; ++ii)
			{
				std::wstring str;
				MyLoadString(hModule, ii, langid, str);
				of << str.c_str() << std::endl;
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			//return hr;

			
		}

		static void RemoveRN(LPWSTR str)
		{
			if (str)
			{
				while (*str)
				{
					if (L'\r' == *str || L'\n' == *str)
					{
						*str = 0;
						break;
					}
					++str;
				}
			}
		}

		static HRESULT MyLoadString(HMODULE hModule, int resid, int langid, std::wstring & str)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
#if 0
			LPCTSTR mui_dll = TEXT("mui.dll");
			HMODULE hModule = GetModuleHandle(mui_dll);
			if (nullptr == hModule)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				hModule = ::LoadLibraryEx(mui_dll, nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
				if (nullptr == hModule)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					LEAVE_BLOCK(0);
				}
			}
#endif

			LPWSTR pszStr = nullptr;
			DWORD chcount = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE, hModule, resid, langid, (LPWSTR)&pszStr, 0, NULL);
			if (0 == chcount)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);

				//try English
				hr = S_OK;
				chcount = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE, hModule, resid, 0x409, (LPWSTR)&pszStr, 0, NULL);
				if (0 == chcount)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					LEAVE_BLOCK(0);
				}
			}

			//RemoveRN(pszStr);
			str = pszStr;
			::LocalFree(pszStr);
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}
	};
}