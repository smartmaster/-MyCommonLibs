#include "stdafx.h"
#include "RunApp.h"

#include <vector>
using namespace std;

#include <atlsecurity.h>

HRESULT EnableProcessPriviledges(CONST vector<LPCTSTR> & Priviledges)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = FALSE;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CAccessToken Token;
	bRet = Token.GetProcessToken(TOKEN_ALL_ACCESS);
	//bRet = Token.GetEffectiveToken(TOKEN_ALL_ACCESS);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("GetProcessToken"), hr, TEXT("TOKEN_ALL_ACCESS"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CAtlArray<LPCTSTR> ArrPriviledges;
	for (size_t ii = 0; ii < Priviledges.size(); ++ ii)
	{
		ArrPriviledges.Add(Priviledges[ii]);
	}

	bRet = Token.EnablePrivileges(ArrPriviledges, NULL);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("EnablePrivileges"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT EnableProcessAllPriviledges()
{
	vector<LPCTSTR> Priviledges;
	Priviledges.push_back(SE_CREATE_TOKEN_NAME);
	Priviledges.push_back(SE_ASSIGNPRIMARYTOKEN_NAME);
	Priviledges.push_back(SE_LOCK_MEMORY_NAME);
	Priviledges.push_back(SE_INCREASE_QUOTA_NAME);
	Priviledges.push_back(SE_UNSOLICITED_INPUT_NAME);
	Priviledges.push_back(SE_MACHINE_ACCOUNT_NAME);
	Priviledges.push_back(SE_TCB_NAME);
	Priviledges.push_back(SE_SECURITY_NAME);
	Priviledges.push_back(SE_TAKE_OWNERSHIP_NAME);
	Priviledges.push_back(SE_LOAD_DRIVER_NAME);
	Priviledges.push_back(SE_SYSTEM_PROFILE_NAME);
	Priviledges.push_back(SE_SYSTEMTIME_NAME);
	Priviledges.push_back(SE_PROF_SINGLE_PROCESS_NAME);
	Priviledges.push_back(SE_INC_BASE_PRIORITY_NAME);
	Priviledges.push_back(SE_CREATE_PAGEFILE_NAME);
	Priviledges.push_back(SE_CREATE_PERMANENT_NAME);
	Priviledges.push_back(SE_BACKUP_NAME);
	Priviledges.push_back(SE_RESTORE_NAME);
	Priviledges.push_back(SE_SHUTDOWN_NAME);
	Priviledges.push_back(SE_DEBUG_NAME);
	Priviledges.push_back(SE_AUDIT_NAME);
	Priviledges.push_back(SE_SYSTEM_ENVIRONMENT_NAME);
	Priviledges.push_back(SE_CHANGE_NOTIFY_NAME);
	Priviledges.push_back(SE_REMOTE_SHUTDOWN_NAME);
	Priviledges.push_back(SE_UNDOCK_NAME);
	Priviledges.push_back(SE_SYNC_AGENT_NAME);
	Priviledges.push_back(SE_ENABLE_DELEGATION_NAME);
	Priviledges.push_back(SE_MANAGE_VOLUME_NAME);
	Priviledges.push_back(SE_IMPERSONATE_NAME);
	Priviledges.push_back(SE_CREATE_GLOBAL_NAME);
	Priviledges.push_back(SE_TRUSTED_CREDMAN_ACCESS_NAME);
	Priviledges.push_back(SE_RELABEL_NAME);
	Priviledges.push_back(SE_INC_WORKING_SET_NAME);
	Priviledges.push_back(SE_TIME_ZONE_NAME);
	Priviledges.push_back(SE_CREATE_SYMBOLIC_LINK_NAME);
	HRESULT hr = EnableProcessPriviledges(Priviledges);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("EnableProcessPriviledges"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}
	return hr;
}

HRESULT CRunAppWin32::CreateMember()
{
	return S_OK;
}

HRESULT CRunAppWin32::DestroyMember()
{
	if (m_hToken)
	{
		CloseHandle(m_hToken);
		m_hToken = NULL;
	}

	if (m_ProcInfo.hThread)
	{
		CloseHandle(m_ProcInfo.hThread);
		m_ProcInfo.hThread = NULL;
	}

	if (m_ProcInfo.hProcess)
	{
		CloseHandle(m_ProcInfo.hProcess);
		m_ProcInfo.hProcess = NULL;
	}

	return S_OK;
}

HRESULT CRunAppWin32::Start()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = FALSE;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_Username.GetLength() && m_Password.GetLength())
	{
		CHandle ahTokenTmp;
		bRet = LogonUser(
			m_Username.GetBuffer(),//_In_      LPTSTR lpszUsername,
			(m_Domain.GetLength()? m_Domain.GetBuffer() : NULL),//_In_opt_  LPTSTR lpszDomain,
			m_Password.GetBuffer(),//_In_opt_  LPTSTR lpszPassword,
			LOGON32_LOGON_BATCH,//_In_      DWORD dwLogonType,
			LOGON32_PROVIDER_DEFAULT,//_In_      DWORD dwLogonProvider,
			&ahTokenTmp.m_h//_Out_     PHANDLE phToken
			);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("LogonUser"), hr, TEXT("Username=%s, Domain=%s"), m_Username.GetString(), m_Domain.GetString());
			LEAVE_BLOCK(0);
		}


		bRet = DuplicateTokenEx(
			ahTokenTmp,//_In_      HANDLE hExistingToken,
			MAXIMUM_ALLOWED,//_In_      DWORD dwDesiredAccess,
			NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpTokenAttributes,
			SecurityDelegation,//_In_      SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,
			TokenPrimary,//_In_      TOKEN_TYPE TokenType,
			&m_hToken//_Out_     PHANDLE phNewToken
			);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("DuplicateTokenEx"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb= sizeof(STARTUPINFO);

	BOOL bCreateProcessAsUser = FALSE;
	if (m_hToken)
	{
		//hr = EnableProcessAllPriviledges();
		//if (FAILED(hr))
		//{
		//	D_API_ERR(0, TEXT("EnableProcessAllPriviledges"), hr, TEXT(""));
		//	//LEAVE_BLOCK(0); do not break on failure
		//}

		//bRet = ImpersonateLoggedOnUser(
		//	m_hToken//_In_  HANDLE hToken
		//	);
		//if (!bRet)
		//{
		//	LastError = GetLastError();
		//	hr = HRESULT_FROM_WIN32(LastError);
		//	D_API_ERR(0, TEXT("ImpersonateLoggedOnUser"), hr, TEXT(""));
		//	LEAVE_BLOCK(0);
		//}


		bRet = CreateProcessAsUser(
			m_hToken,//_In_opt_     HANDLE hToken,
			NULL,//_In_opt_     LPCTSTR lpApplicationName,
			m_CommandLine.GetBuffer(),//_Inout_opt_  LPTSTR lpCommandLine,
			NULL,//_In_opt_     LPSECURITY_ATTRIBUTES lpProcessAttributes,
			NULL,//_In_opt_     LPSECURITY_ATTRIBUTES lpThreadAttributes,
			FALSE,//_In_         BOOL bInheritHandles,
			NORMAL_PRIORITY_CLASS,//_In_         DWORD dwCreationFlags,
			NULL,//_In_opt_     LPVOID lpEnvironment,
			(m_CurDir.GetLength()? m_CurDir.GetString() : NULL),//_In_opt_     LPCTSTR lpCurrentDirectory,
			&si,//_In_         LPSTARTUPINFO lpStartupInfo,
			&m_ProcInfo//_Out_        LPPROCESS_INFORMATION lpProcessInformation
			);

		//RevertToSelf();

		if (!bRet)
		{
			bCreateProcessAsUser = FALSE;
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("CreateProcessAsUser"), hr, TEXT("CommandLine=%s, CurDir=%s, Username=%s, Domain=%s"), m_CommandLine.GetString(), m_CurDir.GetString(), m_Username.GetString(), m_Domain.GetString());
			//LEAVE_BLOCK(0);
		}
		else
		{
			bCreateProcessAsUser = TRUE;
		}
	}
	
	if (!bCreateProcessAsUser)
	{
		bRet = CreateProcess(
			NULL,//_In_opt_     LPCTSTR lpApplicationName,
			m_CommandLine.GetBuffer(),//_Inout_opt_  LPTSTR lpCommandLine,
			NULL,//_In_opt_     LPSECURITY_ATTRIBUTES lpProcessAttributes,
			NULL,//_In_opt_     LPSECURITY_ATTRIBUTES lpThreadAttributes,
			FALSE,//_In_         BOOL bInheritHandles,
			NORMAL_PRIORITY_CLASS,//_In_         DWORD dwCreationFlags,
			NULL,//_In_opt_     LPVOID lpEnvironment,
			(m_CurDir.GetLength()? m_CurDir.GetString() : NULL),//_In_opt_     LPCTSTR lpCurrentDirectory,
			&si,//_In_         LPSTARTUPINFO lpStartupInfo,
			&m_ProcInfo//_Out_        LPPROCESS_INFORMATION lpProcessInformation
			);

		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("CreateProcess"), hr, TEXT("CommandLine=%s, CurDir=%s"), m_CommandLine.GetString(), m_CurDir.GetString());
			LEAVE_BLOCK(0);
		}
		else
		{
			hr = S_OK;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CRunAppWin32::Wait( ULONG MillSec )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	DWORD dwRet = WaitForSingleObject(m_ProcInfo.hProcess, MillSec);
	if (WAIT_OBJECT_0 == dwRet)
	{
		hr = S_OK;
		D_INFO(0, TEXT("Application ended [%s]"), m_CommandLine.GetString());
	}
	else if (WAIT_TIMEOUT == dwRet)
	{
		hr = HRESULT_FROM_WIN32(WAIT_TIMEOUT);
		D_INFO(0, TEXT("Application time out in %u MillSec [%s]"), MillSec, m_CommandLine.GetString());
	}
	else
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("WaitForSingleObject"), hr, TEXT("Result=%u"), dwRet);
	}

	return hr;
}

HRESULT CRunAppWin32::Terminate()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = TerminateProcess(
		m_ProcInfo.hProcess,//_In_  HANDLE hProcess,
		ERROR_PROCESS_ABORTED//_In_  UINT uExitCode
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("TerminateProcess"), hr, TEXT("CommandLine=%s, Process=0x%p"), m_CommandLine.GetString(), m_ProcInfo.hProcess);
	}

	return hr;
}

CRunAppWin32::CRunAppWin32( LPCTSTR Username, LPCTSTR Domain, LPCTSTR Password, LPCTSTR CommandLine, LPCTSTR CurDir ) :
	m_Username(Username),
	m_Domain(Domain),
	m_Password(Password),
	m_CommandLine(CommandLine),
	m_CurDir(CurDir),
	m_RefCount(0),
	m_hToken(NULL)
{
	ZeroMemory(&m_ProcInfo, sizeof(m_ProcInfo));
}

HRESULT CRunAppWin32::GetExitCode( ULONG & ExitCode )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = GetExitCodeProcess(m_ProcInfo.hProcess, &ExitCode);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("GetExitCodeProcess"), hr, TEXT(""));
	}

	return hr;
}


HRESULT CreateInstanceCRunAppWin32( LPCTSTR Username, LPCTSTR Domain, LPCTSTR Password, LPCTSTR CommandLine, LPCTSTR CurDir, IRunApp ** ppObj, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CRunAppWin32,*/
		new CRunAppWin32(Username, Domain, Password, CommandLine, CurDir),
		ppObj,
		bCreateMember);
}

HRESULT RunApp(LPCTSTR Username, LPCTSTR Domain, LPCTSTR Password, LPCTSTR CommandLine, LPCTSTR CurDir, ULONG TimeOut, BOOL TerminateOnTimeOut, ULONG & ExitCode)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IRunApp> spIRunApp;
	hr = CreateInstanceCRunAppWin32( Username, Domain, Password, CommandLine, CurDir, &spIRunApp, TRUE );
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCRunAppWin32"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spIRunApp->Start();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Start"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	if (TimeOut)
	{
		hr = spIRunApp->Wait(TimeOut);
		D_API_ERR(0, TEXT("Wait"), hr, TEXT("TimeOut=%u"), TimeOut);
		if (S_OK != hr)
		{
			if (TerminateOnTimeOut)
			{
				hr = spIRunApp->Terminate();
				D_API_ERR(0, TEXT("Terminate"), hr, TEXT(""));
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spIRunApp->GetExitCode(ExitCode);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetExitCode"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}