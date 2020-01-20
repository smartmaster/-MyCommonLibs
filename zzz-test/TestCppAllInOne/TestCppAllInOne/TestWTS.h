#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlsecurity.h>
#include <Wtsapi32.h>
#include <aclapi.h>

#include <vector>

#include "CodeBlock.h"
#include "log.h"

namespace Smartlib
{
	class CTestWTS
	{
	public:
		static HRESULT Case7_WinSec()
		{
			DWORD dwRes, dwDisposition;
			PSID pEveryoneSID = NULL, pAdminSID = NULL;
			PACL pACL = NULL;
			PSECURITY_DESCRIPTOR pSD = NULL;
			EXPLICIT_ACCESS ea[2];
			SID_IDENTIFIER_AUTHORITY SIDAuthWorld =
				SECURITY_WORLD_SID_AUTHORITY;
			SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
			SECURITY_ATTRIBUTES sa;
			LONG lRes;
			HKEY hkSub = NULL;

			// Create a well-known SID for the Everyone group.

			if (!AllocateAndInitializeSid(&SIDAuthWorld, 1,
				SECURITY_WORLD_RID,
				0, 0, 0, 0, 0, 0, 0,
				&pEveryoneSID))
			{
				_tprintf(_T("AllocateAndInitializeSid Error %u\n"), GetLastError());
				goto Cleanup;
			}

			// Initialize an EXPLICIT_ACCESS structure for an ACE.

			// The ACE will allow Everyone read access to the key.

			ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
			ea[0].grfAccessPermissions = KEY_READ;
			ea[0].grfAccessMode = SET_ACCESS;
			ea[0].grfInheritance = NO_INHERITANCE;
			ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
			ea[0].Trustee.ptstrName = (LPTSTR)pEveryoneSID;

			// Create a SID for the BUILTIN\Administrators group.

			if (!AllocateAndInitializeSid(&SIDAuthNT, 2,
				SECURITY_BUILTIN_DOMAIN_RID,
				DOMAIN_ALIAS_RID_ADMINS,
				0, 0, 0, 0, 0, 0,
				&pAdminSID))
			{
				_tprintf(_T("AllocateAndInitializeSid Error %u\n"), GetLastError());
				goto Cleanup;
			}

			// Initialize an EXPLICIT_ACCESS structure for an ACE.

			// The ACE will allow the Administrators group full access to

			// the key.

			ea[1].grfAccessPermissions = KEY_ALL_ACCESS;
			ea[1].grfAccessMode = SET_ACCESS;
			ea[1].grfInheritance = NO_INHERITANCE;
			ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
			ea[1].Trustee.ptstrName = (LPTSTR)pAdminSID;

			// Create a new ACL that contains the new ACEs.

			dwRes = SetEntriesInAcl(2, ea, NULL, &pACL);
			if (ERROR_SUCCESS != dwRes)
			{
				_tprintf(_T("SetEntriesInAcl Error %u\n"), GetLastError());
				goto Cleanup;
			}

			// Initialize a security descriptor.  

			pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,
				SECURITY_DESCRIPTOR_MIN_LENGTH);
			if (NULL == pSD)
			{
				_tprintf(_T("LocalAlloc Error %u\n"), GetLastError());
				goto Cleanup;
			}

			if (!InitializeSecurityDescriptor(pSD,
				SECURITY_DESCRIPTOR_REVISION))
			{
				_tprintf(_T("InitializeSecurityDescriptor Error %u\n"),
					GetLastError());
				goto Cleanup;
			}

			// Add the ACL to the security descriptor. 

			if (!SetSecurityDescriptorDacl(pSD,
				TRUE,     // bDaclPresent flag   

				pACL,
				FALSE))   // not a default DACL 

			{
				_tprintf(_T("SetSecurityDescriptorDacl Error %u\n"),
					GetLastError());
				goto Cleanup;
			}

			// Initialize a security attributes structure.

			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = pSD;
			sa.bInheritHandle = FALSE;

			// Use the security attributes to set the security descriptor 

			// when you create a key.

			//lRes = RegCreateKeyEx(HKEY_CURRENT_USER, _T("mykey"), 0, _T(""), 0,
			//	KEY_READ | KEY_WRITE, &sa, &hkSub, &dwDisposition);
			//_tprintf(_T("RegCreateKeyEx result %u\n"), lRes);

		Cleanup:

			if (pEveryoneSID)
				FreeSid(pEveryoneSID);
			if (pAdminSID)
				FreeSid(pAdminSID);
			if (pACL)
				LocalFree(pACL);
			if (pSD)
				LocalFree(pSD);
			if (hkSub)
				RegCloseKey(hkSub);

			return S_OK;

		}
		static HRESULT Case6_WTSGetters()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			DWORD acid = ::WTSGetActiveConsoleSessionId();
			SML_LOG_LINE(TEXT("WTSGetActiveConsoleSessionId=[%d]"), acid);


			//////////////////////////////////////////////////////////////////////////
			ULONG SessionId = -1;
			BOOL ok = ::WTSGetChildSessionId(
				&SessionId//_Out_  ULONG * pSessionId
			);
			if (!ok)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSGetChildSessionId"), LastError, hr);
			}
			SML_LOG_LINE(TEXT("WTSGetChildSessionId=[%d]"), SessionId);

			//////////////////////////////////////////////////////////////////////////


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}
		static HRESULT Case5_WTSEnumerateSessionsEx()
		{
			using namespace std;
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			PWTS_SESSION_INFO_1  pResult = nullptr;
			DWORD Level = 1;
			DWORD Count = 0;
			BOOL ok = ::WTSEnumerateSessionsEx(
				WTS_CURRENT_SERVER_HANDLE,//_In_     HANDLE hServer,
				&Level,//_Inout_  DWORD * pLevel,
				0,//_In_     DWORD Filter,
				&pResult,//_Out_    PWTS_SESSION_INFO_1 * ppSessionInfo,
				&Count//,//_Out_    DWORD * pCount
			);
			if (!ok)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSEnumerateSessionsEx"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			for (int ii = 0; ii < Count; ++ii)
			{
				SML_LOG_LINE(TEXT("ExecEnvId=[%d]\t SessionId=[%d]\t State=[%d]\t pSessionName=[%s]\t pHostName=[%s]\t pUserName=[%s]\t pDomainName=[%s]\t pFarmName=[%s]\t"), 
					pResult[ii].ExecEnvId,
					pResult[ii].SessionId, 
					pResult[ii].State, 
					pResult[ii].pSessionName,
					pResult[ii].pHostName,
					pResult[ii].pUserName,
					pResult[ii].pDomainName,
					pResult[ii].pFarmName
					);
			}

			::WTSFreeMemoryEx(WTSTypeSessionInfoLevel1, pResult, Count);
			pResult = nullptr;

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}

		static HRESULT Case4_WTSEnumerateSessions()
		{
			using namespace std;
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			PWTS_SESSION_INFO  pResult = nullptr;
			DWORD Level = 1;
			DWORD Count = 0;
			BOOL ok = ::WTSEnumerateSessions(
				WTS_CURRENT_SERVER_HANDLE,//_In_  LPTSTR pDomainName,
				0,//_In_  DWORD Reserved,
				1,//_In_  DWORD Version,
				&pResult,//PWTS_SERVER_INFO * ppServerInfo,
				&Count//DWORD * pCount
			);
			if (!ok)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSEnumerateSessions"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			for (int ii = 0; ii < Count; ++ii)
			{
				SML_LOG_LINE(TEXT("SessionId=[%d]\t State=[%d]\t WinStationName=[%s]\t"), pResult[ii].SessionId, pResult[ii].State, pResult[ii].pWinStationName);
			}

			::WTSFreeMemory(pResult);
			pResult = nullptr;

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}


		static HRESULT Case3_WTSEnumerateServers()
		{
			using namespace std;
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			PWTS_SERVER_INFO  pResult = nullptr;
			DWORD Level = 1;
			DWORD Count = 0;
			TCHAR domain[] = TEXT("arcserve.com");
			BOOL ok = ::WTSEnumerateServers(
				domain,//_In_  LPTSTR pDomainName,
				0,//_In_  DWORD Reserved,
				1,//_In_  DWORD Version,
				&pResult,//PWTS_SERVER_INFO * ppServerInfo,
				&Count//DWORD * pCount
			);
			if (!ok)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSEnumerateServers"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			for (int ii = 0; ii < Count; ++ii)
			{
				SML_LOG_LINE(TEXT("ServerName=[%s]"), pResult[ii].pServerName);
			}

			::WTSFreeMemory(pResult);
			pResult = nullptr;

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}


		static HRESULT Case2_WTSEnumerateProcessesEx()
		{
			using namespace std;
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			PWTS_PROCESS_INFO_EX  pProcessInfo = nullptr;
			DWORD Level = 1;
			DWORD Count = 0;
			BOOL ok = ::WTSEnumerateProcessesEx(
				WTS_CURRENT_SERVER_HANDLE,//_In_     HANDLE hServer,
				&Level,//_Inout_  DWORD * pLevel,
				WTS_ANY_SESSION,//_In_     DWORD SessionID,
				(LPTSTR*)&pProcessInfo,//_Out_    LPTSTR * ppProcessInfo,
				&Count//,//_Out_    DWORD * pCount
			);
			if (!ok)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSEnumerateProcesses"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			for (int ii = 0; ii < Count; ++ii)
			{
				if (pProcessInfo[ii].pUserSid)
				{

					CSid sid; //(pProcessInfo[ii].pUserSid);
					sid.LoadAccount((const SID*)pProcessInfo[ii].pUserSid);
					SML_LOG_LINE(TEXT("[%d]\t\t[%d]\t\t[%s]\t\t\t\t[%s\\%s]"), pProcessInfo[ii].ProcessId, pProcessInfo[ii].SessionId, pProcessInfo[ii].pProcessName, sid.Domain(), sid.AccountName());
				}
				else
				{
					SML_LOG_LINE(TEXT("[%d]\t\t[%d]\t\t[%s]"), pProcessInfo[ii].ProcessId, pProcessInfo[ii].SessionId, pProcessInfo[ii].pProcessName);
				}
			}

			WTSFreeMemoryEx(WTSTypeProcessInfoLevel1, pProcessInfo, Count);
			pProcessInfo = nullptr;

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}

		static HRESULT Case1_WTSEnumerateProcesses()
		{
			using namespace std;
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			PWTS_PROCESS_INFO pProcessInfo = nullptr;
			DWORD Count = 0;
			BOOL ok = ::WTSEnumerateProcesses(
				WTS_CURRENT_SERVER_HANDLE,//_In_   HANDLE hServer,
				0,//_In_   DWORD Reserved,
				1,//_In_   DWORD Version,
				&pProcessInfo,//_Out_  PWTS_PROCESS_INFO * ppProcessInfo,
				&Count//_Out_  DWORD * pCount
			);
			if (!ok)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSEnumerateProcesses"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			for (int ii = 0; ii < Count; ++ ii)
			{
				if (pProcessInfo[ii].pUserSid)
				{

					CSid sid; //(pProcessInfo[ii].pUserSid);
					sid.LoadAccount((const SID*)pProcessInfo[ii].pUserSid);
					SML_LOG_LINE(TEXT("[%d]\t\t[%d]\t\t[%s]\t\t[%s\\%s]"), pProcessInfo[ii].ProcessId, pProcessInfo[ii].SessionId, pProcessInfo[ii].pProcessName, sid.Domain(), sid.AccountName());
				}
				else
				{
					SML_LOG_LINE(TEXT("[%d]\t\t[%d]\t\t[%s]"), pProcessInfo[ii].ProcessId, pProcessInfo[ii].SessionId, pProcessInfo[ii].pProcessName);
				}
			}

			WTSFreeMemory(pProcessInfo);
			pProcessInfo = nullptr;

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}


		static HRESULT Case0_WTSEnumerateListeners()
		{
			using namespace std;
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			DWORD count = 0;
			BOOL ok = ::WTSEnumerateListeners(
				WTS_CURRENT_SERVER_HANDLE,//_In_       HANDLE  hServer,
				nullptr,//_In_       PVOID  pReserved,
				0,//_In_       DWORD  Reserved,
				nullptr,//_Out_opt_  PWTSLISTENERNAME  pListeners,
				&count//_Inout_    DWORD * pCount
			);
			if (!ok)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				if (ERROR_INSUFFICIENT_BUFFER != LastError)
				{
					SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSEnumerateListeners"), LastError, hr);
					LEAVE_BLOCK(0);
				}
			}


			vector<WTSLISTENERNAME> names{ count };
			ok = ::WTSEnumerateListeners(
				WTS_CURRENT_SERVER_HANDLE,//_In_       HANDLE  hServer,
				nullptr,//_In_       PVOID  pReserved,
				0,//_In_       DWORD  Reserved,
				&names[0],//_Out_opt_  PWTSLISTENERNAME  pListeners,
				&count//_Inout_    DWORD * pCount
			);
			if (!ok)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSEnumerateListeners"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			for (const auto& item : names)
			{
				SML_LOG_LINE(TEXT("Listeners: %s"), item);
			}


			for (const auto& item : names)
			{
				DWORD nLengthNeeded = 0;
				ok = ::WTSGetListenerSecurity(
					WTS_CURRENT_SERVER_HANDLE,//_In_       HANDLE hServer,
					nullptr,//_In_       PVOID pReserved,
					0,//_In_       DWORD Reserved,
					(LPTSTR)item,//_In_       LPTSTR pListenerName,
					DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION,//_In_       SECURITY_INFORMATION SecurityInformation,
					nullptr,//_Out_opt_  PSECURITY_DESCRIPTOR pSecurityDescriptor,
					0,//_In_       DWORD nLength,
					&nLengthNeeded//,//_Out_      LPDWORD lpnLengthNeeded
				);
				if (!ok)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					if (ERROR_INSUFFICIENT_BUFFER != LastError)
					{
						SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSGetListenerSecurity"), LastError, hr);
						continue;
					}
				}

				vector<BYTE> sdbuffer;
				sdbuffer.resize(nLengthNeeded);
				ok = ::WTSGetListenerSecurity(
					WTS_CURRENT_SERVER_HANDLE,//_In_       HANDLE hServer,
					nullptr,//_In_       PVOID pReserved,
					0,//_In_       DWORD Reserved,
					(LPTSTR)item,//_In_       LPTSTR pListenerName,
					DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION,//_In_       SECURITY_INFORMATION SecurityInformation,
					&sdbuffer[0],//_Out_opt_  PSECURITY_DESCRIPTOR pSecurityDescriptor,
					sdbuffer.size(),//_In_       DWORD nLength,
					&nLengthNeeded//,//_Out_      LPDWORD lpnLengthNeeded
				);
				if (!ok)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("[%s] failed with LastError=[%d], hr=[0x%08x]"), TEXT("WTSGetListenerSecurity"), LastError, hr);
					continue;
				}

				SECURITY_DESCRIPTOR* psd = (SECURITY_DESCRIPTOR*)&sdbuffer[0];
				CSecurityDesc sd(*psd);
				CString sdstr;
				sd.ToString(&sdstr);
				SML_LOG_LINE(TEXT("WTSListener=[%s]\t SECURITY_DESCRIPTOR=[%s]\t "), item, sdstr.GetString());
			}
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

	};
}