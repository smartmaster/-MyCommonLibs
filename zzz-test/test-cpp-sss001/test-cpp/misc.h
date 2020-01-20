#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "DebugLog.h"

namespace SmartLib
{
	class CMisc
	{
	public:
		static	CString ConvertArg(CString str)
		{
			str.Replace(TEXT("\""), TEXT("\"\""));
			return str;
		}

		static void Case1()
		{
			TCHAR szCmdline[4096] = { 0 };
			swprintf_s(szCmdline, 4096, L"VixPurgeSqlLogs.exe \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
				ConvertArg(TEXT("m_strVix.G\"\"\"etString()")).GetString() ,
				ConvertArg(TEXT("m_stuESX\"\"\"Credentials.Username")).GetString(),
				ConvertArg(TEXT("m_stuESXCredenti\"\"\"als.Password")).GetString(),
				ConvertArg(TEXT("m_stuVMInfo.vmN\"\"\"ame")).GetString(),
				ConvertArg(TEXT("m_stuVM\"\"\"Info.vmVMX")).GetString() ,
				ConvertArg(TEXT("m_VMUs\"\"\"ername")).GetString(),
				ConvertArg(TEXT("m_VMPassword\"\"\"")).GetString(),
				ConvertArg(TEXT("\"\"\"wszInputFile")).GetString(),
				ConvertArg(TEXT("wszOutputFile\"\"\"")).GetString() 
			);
			LOG_LINE_T(TEXT("%s"), szCmdline);
		}


		static void Case0(int argc, TCHAR ** argv)
		{
			CString str = TEXT("aa\"\"\"bb");
			_ftprintf_s(stdout, L"%s" L"\r\n", str.GetString());
			str.Replace(TEXT("\""), TEXT("\"\""));
			_ftprintf_s(stdout, L"%s" L"\r\n", str.GetString());

			for (int ii = 0; ii < argc; ++ii)
			{
				_ftprintf_s(stdout, L"%s" L"\r\n", argv[ii]);
			}
			{
				TCHAR szCmdline[4096] = { 0 };
				LPCTSTR commandLine[] =
				{
					TEXT("10.98.236.3:443"),						//L"host",,//m_strVix.GetString()/*m_stuESXCredentials.ServerName*/,
					TEXT("backup-mgr@vsphere.local"),			//L"host-user",,//m_stuESXCredentials.Username,
					TEXT("xxx"),									//L"host-password",,//m_stuESXCredentials.Password,
					TEXT("hubud0160"),								//vmName,,//L"vmName",,//m_stuVMInfo.vmName, 
					TEXT("[Unity-2007] hubud0160/hubud0160.vmx"),	//L"vmVMX",,//m_stuVMInfo.vmVMX, 
					TEXT("ATREMA\\dtce-bkp"),						//L"m_VMUsername",,//m_VMUsername,
					TEXT("m_VMPassword"),										//L"m_VMPassword",,//m_VMPassword, 
					TEXT("C:\\Program Files (x86)\\Entity Framework Tools\\NuGet Packages\\EntityFramework.5.0.0\\wszInputFile"),										//wszInputFile,,//wszInputFile, 
					TEXT("C:\\Program Files (x86)\\Entity Framework Tools\\NuGet Packages\\EntityFramework.5.0.0\\wszOutputFile"),										//wszOutputFile,//wszOutputFile
				};
				const TCHAR * vmName = L"\"\" abc\"\"";
				const TCHAR * wszInputFile = L"C:\\Program Files (x86)\\Entity Framework Tools\\NuGet Packages\\EntityFramework.5.0.0\\wszInputFile";
				const TCHAR * wszOutputFile = L"C:\\Program Files (x86)\\Entity Framework Tools\\NuGet Packages\\EntityFramework.5.0.0\\wszOutputFile";
				swprintf_s(szCmdline, 4096, L"VixPurgeSqlLogs.exe \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
					commandLine[0],//L"host",,//m_strVix.GetString()/*m_stuESXCredentials.ServerName*/, 
					commandLine[1],//L"host-user",,//m_stuESXCredentials.Username, 
					commandLine[2],//L"host-password",,//m_stuESXCredentials.Password,
					commandLine[3],//vmName,,//L"vmName",,//m_stuVMInfo.vmName, 
					commandLine[4],//L"vmVMX",,//m_stuVMInfo.vmVMX, 
					commandLine[5],//L"m_VMUsername",,//m_VMUsername, 
					commandLine[6],//L"m_VMPassword",,//m_VMPassword, 
					commandLine[7],//wszInputFile,,//wszInputFile, 
					commandLine[8]//,//wszOutputFile,//wszOutputFile
				);
				_ftprintf_s(stdout, L"%s", szCmdline);
				_ftprintf_s(stdout, L"%s", L"\r\n");
			}

			{
				TCHAR szCmdline[4096] = { 0 };
				const TCHAR * vmName = L"\"\" abc\"\"";
				const TCHAR * wszInputFile = L"C:\\Program Files (x86)\\Entity Framework Tools\\NuGet Packages\\EntityFramework.5.0.0\\wszInputFile";
				const TCHAR * wszOutputFile = L"C:\\Program Files (x86)\\Entity Framework Tools\\NuGet Packages\\EntityFramework.5.0.0\\wszOutputFile";
				swprintf_s(szCmdline, 4096, L"VixPurgeSqlLogs.exe \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
					L"host",//m_strVix.GetString()/*m_stuESXCredentials.ServerName*/, 
					L"host-user",//m_stuESXCredentials.Username, 
					L"host-password",//m_stuESXCredentials.Password,
					vmName,//L"vmName",//m_stuVMInfo.vmName, 
					L"vmVMX",//m_stuVMInfo.vmVMX, 
					L"m_VMUsername",//m_VMUsername, 
					L"m_VMPassword",//m_VMPassword, 
					wszInputFile,//wszInputFile, 
					wszOutputFile//wszOutputFile
				);
				_ftprintf_s(stdout, L"%s", szCmdline);
				_ftprintf_s(stdout, L"%s", L"\r\n");
			}



			{
				TCHAR szCmdline[4096] = { 0 };
				const TCHAR * wszInputFile = L"C:\\Program Files (x86)\\Entity Framework Tools\\NuGet Packages\\EntityFramework.5.0.0\\wszInputFile";
				const TCHAR * wszOutputFile = L"C:\\Program Files (x86)\\Entity Framework Tools\\NuGet Packages\\EntityFramework.5.0.0\\wszOutputFile";
				swprintf_s(szCmdline, 4096, L"VixPurgeSqlLogs.exe \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
					L"host",//m_strVix.GetString()/*m_stuESXCredentials.ServerName*/, 
					L"host-user",//m_stuESXCredentials.Username, 
					L"host-password",//m_stuESXCredentials.Password,
					L"vmName",//m_stuVMInfo.vmName, 
					L"vmVMX",//m_stuVMInfo.vmVMX, 
					L"m_VMUsername",//m_VMUsername, 
					L"m_VMPassword",//m_VMPassword, 
					wszInputFile,//wszInputFile, 
					wszOutputFile//wszOutputFile
				);
				_ftprintf_s(stdout, L"%s", szCmdline);
				_ftprintf_s(stdout, L"%s", L"\r\n");
			}

			{
				TCHAR szCmdline[4096] = { 0 };
				const TCHAR * wszInputFile = L"C:\\Program Files (x86)\\Entity Framework Tools\\NuGet Packages\\EntityFramework.5.0.0\\wszInputFile";
				const TCHAR * wszOutputFile = L"C:\\Program Files (x86)\\Entity Framework Tools\\NuGet Packages\\EntityFramework.5.0.0\\wszOutputFile";
				swprintf_s(szCmdline, 4096, L"VixPurgeSqlLogs.exe \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
					L"10.98.236.3:443",//m_strVix.GetString()/*m_stuESXCredentials.ServerName*/, 
					L"backup-mgr@vsphere.local",//m_stuESXCredentials.Username, 
					L"xxx",//m_stuESXCredentials.Password,
					L"hubud0160/hubud0160.vmx ATREMA\\dtce-bkp cleartextpasswordhere C:\\Program",//m_stuVMInfo.vmName, 
					L"Files\\Arcserve\\Unified",//m_stuVMInfo.vmVMX, 
					L"Data",//m_VMUsername, 
					L"yyy",//m_VMPassword, 
					L"Files\\Arcserve\\Unified",//wszInputFile, 
					wszOutputFile//wszOutputFile
				);
				_ftprintf_s(stdout, L"%s", szCmdline);
				_ftprintf_s(stdout, L"%s", L"\r\n");
			}
		}
	};
}