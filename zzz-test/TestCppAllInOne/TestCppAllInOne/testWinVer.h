#pragma once

#include <Windows.h>
#include <VersionHelpers.h>

#include "log.h"

namespace SmartLib
{
	//_NT_TARGET_VERSION = $(_NT_TARGET_VERSION_LATEST);

	class TestWinver
	{
		//static LPCSTR name_GetVersionExW{ "GetVersionExW" };
		typedef BOOL(WINAPI* pfnGetVersionExW)(
			_Inout_ LPOSVERSIONINFO lpVersionInfo
			);

		//typedef LONG NTSTATUS, * PNTSTATUS;
		//

		
#ifdef _WIN32
		static	bool  GetWinNTOSVersion(PRTL_OSVERSIONINFOEXW osvi)
		{
			typedef NTSTATUS(WINAPI * pfnRtlGetVersion)(PRTL_OSVERSIONINFOEXW);

			bool rc = false;
			HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
			if (hMod)
			{
				pfnRtlGetVersion pRtlGetVersion = (pfnRtlGetVersion)::GetProcAddress(hMod, "RtlGetVersion");
				if (pRtlGetVersion != nullptr)
				{
					osvi->dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
					rc = (0 == pRtlGetVersion(osvi));
				}
			}
			return rc;
		}
#endif

	public:
		static void Case0()
		{
			DWORD dwMajorVersion = 6;
			DWORD dwMinorVersion = 2;
			DWORD build = 0;

			RTL_OSVERSIONINFOEXW osvi{ 0 };
			GetWinNTOSVersion(&osvi);

			build = 9100;
			bool bb = IsWindowsBuildEqualOrGreater(dwMajorVersion, dwMinorVersion, build);
			SML_LOG_LINE(TEXT("IsWindowsBuildEqualOrGreater %d.%d.%d [%d]"), dwMajorVersion, dwMinorVersion, build, bb);

			build = 9200;
			bb = IsWindowsBuildEqualOrGreater(dwMajorVersion, dwMinorVersion, build);
			SML_LOG_LINE(TEXT("IsWindowsBuildEqualOrGreater %d.%d.%d [%d]"), dwMajorVersion, dwMinorVersion, build, bb);

			build = 9300;
			bb = IsWindowsBuildEqualOrGreater(dwMajorVersion, dwMinorVersion, build);
			SML_LOG_LINE(TEXT("IsWindowsBuildEqualOrGreater %d.%d.%d [%d]"), dwMajorVersion, dwMinorVersion, build, bb);

			build = 10000;
			bb = IsWindowsBuildEqualOrGreater(dwMajorVersion, dwMinorVersion, build);
			SML_LOG_LINE(TEXT("IsWindowsBuildEqualOrGreater %d.%d.%d [%d]"), dwMajorVersion, dwMinorVersion, build, bb);


			bb = IsWindows10OrGreater();
			SML_LOG_LINE(TEXT("IsWindows10OrGreater [%d]"), bb);

			//OSVERSIONINFOEX verinfo{ 0 };
			//verinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			//BOOL b3 = ::GetVersionEx(
			//	(LPOSVERSIONINFO)&verinfo//_Inout_ LPOSVERSIONINFO lpVersionInfo
			//);

			pfnGetVersionExW pGetVersionExW = (pfnGetVersionExW)::GetProcAddress(::GetModuleHandle(TEXT("Kernel32.dll")), "GetVersionExW");
			OSVERSIONINFOEX verinfo{ 0 };
			verinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			BOOL b5 = pGetVersionExW(
				(LPOSVERSIONINFO)& verinfo//_Inout_ LPOSVERSIONINFO lpVersionInfo
			);
			SML_LOG_LINE(TEXT("windows version %d.%d.%d"), verinfo.dwMajorVersion, verinfo.dwMinorVersion, verinfo.dwBuildNumber);

			return;
		}

		static BOOL IsWindowsBuildEqualOrGreater(DWORD dwMajorVersion, DWORD dwMinorVersion, DWORD dwBuildNumber)
		{
			OSVERSIONINFOEX osvi{ 0 };
			ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

			osvi.dwMajorVersion = dwMajorVersion;
			osvi.dwMinorVersion = dwMinorVersion;
			osvi.dwBuildNumber = dwBuildNumber;

			DWORDLONG dwlConditionMask = 0;
			VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
			VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);
			VER_SET_CONDITION(dwlConditionMask, VER_BUILDNUMBER, VER_GREATER_EQUAL);

			return VerifyVersionInfo(
				&osvi,
				VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER,
				dwlConditionMask);
		}
	};
}