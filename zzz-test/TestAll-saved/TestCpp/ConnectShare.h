#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlsecurity.h>

#include "log.h"
#include "CommonDef.h"


namespace TestCpp
{
	class CShareManager
	{
	public:
		//administrator . Master16516336 "" \\sonmi01-000-win12r2\x64 sonmi01-000-win12r2\administrator 1324
		//administrator . Master16516336 "" \\sonmi01-000-win12r2\x64 sonmi01-000-win12r2\hbbuadmin c@123456
		static DWORD Connect(LPCTSTR localName, LPCTSTR remoteName, LPCTSTR user, LPCTSTR password)
		{
			NETRESOURCE nr;
			ZeroMemory(&nr, sizeof(nr));

			nr.dwType = RESOURCETYPE_ANY;
			nr.lpLocalName = localName && localName[0] ? (LPTSTR)localName : nullptr;
			nr.lpRemoteName = (LPTSTR)remoteName;
			nr.lpProvider = NULL;

			DWORD res = WNetAddConnection2(&nr, password, user, FALSE);

			if (ERROR_SUCCESS == res)
			{
				LOG_LINE(TEXT("successfully connected to [%s] with user name [%s]"), remoteName, user);
			}
			else
			{
				LOG_LINE(TEXT("failed to connect to [%s] with user name [%s], the error code is [%d]"), remoteName, user, res);
			}

			return res;
		}


		static DWORD Disconnect(LPCTSTR remoteName)
		{
			DWORD res = WNetCancelConnection2(remoteName,
				CONNECT_UPDATE_PROFILE, // remove connection from profile 
				TRUE);
			if (ERROR_SUCCESS == res)
			{
				LOG_LINE(TEXT("successfully disconnected from [%s]"), remoteName);
			}
			else
			{
				LOG_LINE(TEXT("failed to disconnect from [%s], the error code is [%d]"), remoteName, res);
			}

			return res;
		}


		//////////////////////////////////////////////////////////////////////////
		static HANDLE LogonAndImpersonateUser(LPTSTR user, LPTSTR domain, LPTSTR password)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			HANDLE hToken = nullptr;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			LOG_LINE(TEXT("before LogonUser"));
			GetTokenInfoProcess();
			BOOL blu = LogonUser(
				user,//_In_     LPTSTR  lpszUsername,
				domain,//_In_opt_ LPTSTR  lpszDomain,
				password,//_In_opt_ LPTSTR  lpszPassword,
				LOGON32_LOGON_BATCH,//_In_     DWORD   dwLogonType,
				LOGON32_PROVIDER_DEFAULT,//_In_     DWORD   dwLogonProvider,
				&hToken//_Out_    PHANDLE phToken
			);
			if (!blu)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LOG_LINE(TEXT("LogonUser failed with error %d"), LastError);
				LEAVE_BLOCK(0);
			}


			LOG_LINE(TEXT("before ImpersonateLoggedOnUser"));
			GetTokenInfoProcess();

			BOOL bilo = ImpersonateLoggedOnUser(
				hToken//_In_ HANDLE hToken
			);
			if (!bilo)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LOG_LINE(TEXT("ImpersonateLoggedOnUser failed with error %d"), LastError);
				LEAVE_BLOCK(0);
			}


			LOG_LINE(TEXT("after ImpersonateLoggedOnUser"));
			GetTokenInfoProcess();
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);


			if (FAILED(hr))
			{
				CloseHandle(hToken);
				hToken = nullptr;
			}

			return hToken;
		}


		static HRESULT LogonAndImpersonateUserCleanup(HANDLE htoken)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			LOG_LINE(TEXT("before RevertToSelf"));
			GetTokenInfoProcess();

			bool brts = RevertToSelf();
			if (!brts)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				LOG_LINE(TEXT("RevertToSelf failed with error %d"), LastError);
				//LEAVE_BLOCK(0);
			}


			LOG_LINE(TEXT("after RevertToSelf"));
			GetTokenInfoProcess();

			CloseHandle(htoken);
			htoken = nullptr;
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}//static HRESULT LogonAndImpersonateUserCleanup(HANDLE htoken)



		 //administrator . Master16516336 "" \\sonmi01-000-win12r2\x64 sonmi01-000-win12r2\administrator 1324
		 //administrator . Master16516336 "" \\sonmi01-000-win12r2\x64 sonmi01-000-win12r2\hbbuadmin c@123456
		static void Case0(LPTSTR myuser, LPTSTR mydomain, LPTSTR mypassword,
			LPCTSTR localShareName, LPCTSTR remoteShareName, LPCTSTR remoteShareUser, LPCTSTR remoteSharePassword
		)
		{
			HANDLE htoken = LogonAndImpersonateUser(myuser, mydomain, mypassword);
			Connect(localShareName, remoteShareName, remoteShareUser, remoteSharePassword);
			LOG_LINE(TEXT("Press any key to continue..."));
			_getch();
			Disconnect(remoteShareName);
			LogonAndImpersonateUserCleanup(htoken);
		}//static void Case0




		static HRESULT GetTokenInfo(HANDLE htoken)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			DWORD                  ReturnLength = 0;
			DWORD SessionId = -1;
			BOOL bgti = GetTokenInformation(
				htoken,//_In_      HANDLE                  TokenHandle,
				TokenSessionId,//_In_      TOKEN_INFORMATION_CLASS TokenInformationClass,
				&SessionId,//_Out_opt_ LPVOID                  TokenInformation,
				sizeof(SessionId),//_In_      DWORD                   TokenInformationLength,
				&ReturnLength//_Out_     PDWORD                  ReturnLength
			);

			TOKEN_STATISTICS ts = { 0 };
			bgti = GetTokenInformation(
				htoken,//_In_      HANDLE                  TokenHandle,
				TokenStatistics,//_In_      TOKEN_INFORMATION_CLASS TokenInformationClass,
				&ts,//_Out_opt_ LPVOID                  TokenInformation,
				sizeof(TOKEN_STATISTICS),//_In_      DWORD                   TokenInformationLength,
				&ReturnLength//_Out_     PDWORD                  ReturnLength
			);

			//BOOL WINAPI GetTokenInformation(
			//	_In_      HANDLE                  TokenHandle,
			//	_In_      TOKEN_INFORMATION_CLASS TokenInformationClass,
			//	_Out_opt_ LPVOID                  TokenInformation,
			//	_In_      DWORD                   TokenInformationLength,
			//	_Out_     PDWORD                  ReturnLength
			//);

			LOG_LINE(TEXT("sessionId=%d, Logon sessionId=%d"), SessionId, ts.AuthenticationId.LowPart);

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

		static void GetTokenInfoProcess()
		{
			//HANDLE TokenHandle = nullptr;
			//OpenProcessToken(
			//	GetCurrentProcess(),//_In_  HANDLE  ProcessHandle,
			//	TOKEN_QUERY,//_In_  DWORD   DesiredAccess,
			//	&TokenHandle//_Out_ PHANDLE TokenHandle
			//);
			CAccessToken at;
			at.GetEffectiveToken(TOKEN_QUERY);

			//GetTokenInfo(at.GetHandle());

			LUID LogonSessionId = { 0 };
			at.GetLogonSessionId(
				&LogonSessionId//LUID* pluid
			);


			CSid sid;
			at.GetLogonSid(&sid);

			DWORD wtsid = -1;
			at.GetTerminalServicesSessionId(&wtsid);


			LOG_LINE(TEXT("TerminalServicesSessionId=%d, LogonSessionId=%d, LogonSessionSId=%s\\%s"), wtsid, LogonSessionId, sid.Domain(), sid.AccountName());

		}


	}; //class CShareManager
}//namespace TestCpp