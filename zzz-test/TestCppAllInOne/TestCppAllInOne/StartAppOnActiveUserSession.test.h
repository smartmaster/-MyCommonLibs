#pragma once
#include "StartAppOnActiveUserSession.h"


namespace SmartLib
{
	class CStartAppOnActiveUserSessionTest
	{
	public:
		static void Case1()
		{
			PROCESS_INFORMATION PI{ 0 };
			LPCWSTR pCommandLine = TEXT("C:\\Windows\\System32\\notepad.exe F:\\notes-wix.txt");
			Smartlib::StartAppOnActiveUserSession::StartAppOnActiveSession(
				TEXT("administrator"),//IN LPCWSTR pUsername,
				TEXT("."),//IN LPCWSTR pDomain,
				TEXT("Master16516336"),//IN LPCWSTR pPassword,
				pCommandLine,//IN LPCWSTR pCommandLine,
				nullptr,//IN LPCWSTR pCurrentDirectory,
				&PI//,//OUT PPROCESS_INFORMATION pPI
			);
			::WaitForSingleObject(PI.hProcess, 300 * 1000);
			::CloseHandle(PI.hProcess);
			::CloseHandle(PI.hThread);
			ZeroMemory(&PI, sizeof(PROCESS_INFORMATION));
		}
		static void Case0()
		{
			PROCESS_INFORMATION PI{ 0 };
			LPCWSTR pCommandLine = TEXT("C:\\Windows\\System32\\notepad.exe F:\\notes-wix.txt");
			Smartlib::StartAppOnActiveUserSession::StartAppOnActiveSession(
				nullptr,//IN LPCWSTR pUsername,
				nullptr,//IN LPCWSTR pDomain,
				nullptr,//IN LPCWSTR pPassword,
				pCommandLine,//IN LPCWSTR pCommandLine,
				nullptr,//IN LPCWSTR pCurrentDirectory,
				&PI//,//OUT PPROCESS_INFORMATION pPI
			);
			::WaitForSingleObject(PI.hProcess, 300 * 1000);
			::CloseHandle(PI.hProcess);
			::CloseHandle(PI.hThread);
			ZeroMemory(&PI, sizeof(PROCESS_INFORMATION));
		}
	};
}