#include "stdafx.h"
#include "ProcessEndEventSink.h"
#include "ProcessStartEventSink.h"



#ifndef BEGIN_BLOCK
#define BEGIN_BLOCK(level_number)			do{	int	_abc_xyz##level_number(level_number);
#define LEAVE_BLOCK(level_number)			{(_abc_xyz##level_number);}	break;
#define END_BLOCK(level_number)				{(_abc_xyz##level_number);}	}while(0);
#endif

//////////////////////////////////////////////////////////////////////////
namespace 
{

class CInitCOM
{
public:
	CInitCOM(DWORD dwCoInit)
	{
		CoInitializeEx(NULL, dwCoInit);
	}

	~CInitCOM()
	{
		CoUninitialize();
	}
};

} //end namespace

static VOID __CombineDomainUser(CONST CString & Domain, CONST CString & User, CString & strDomainUser)
{
	if (User.GetLength())
	{
		if (Domain.GetLength())
		{
			strDomainUser = Domain;
			strDomainUser += TEXT("\\");
			strDomainUser += User;
		}
		else
		{
			strDomainUser = TEXT(".");
			strDomainUser += TEXT("\\");
			strDomainUser += User;
		}
	}
}

static VOID __SplitDomainUser(CONST CString & strDomainUser, CString & Domain, CString & User)
{
	int index = strDomainUser.Find(TEXT('\\'));
	if (-1 != index)
	{
		Domain = strDomainUser.Mid(0, index);
		User = strDomainUser.Mid(index + 1);
	}
	else
	{
		index = index = strDomainUser.Find(TEXT('@'));
		if (-1 != index)
		{
			Domain = strDomainUser.Mid(index + 1);
			User = strDomainUser.Mid(0, index);
		}
		else
		{
			Domain = TEXT(".");
			User = strDomainUser;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
static HRESULT CreateIWbemServices(
	CONST CString & Machine,
	CONST CString & Domain,
	CONST CString & User,
	CONST CString & Password,
	CONST CString & NetworkResource,
	IWbemServices ** ppIWbemServicesOut
	)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemLocator> spLoc;
	hr = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, 
		(LPVOID*)&spLoc
		);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("CoCreateInstance"), hr, TEXT("IID_IWbemLocator"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemServices> spSvc;
	CString strFullNetworkResource;
	if (Machine.GetLength())
	{
		strFullNetworkResource.Format(TEXT("\\\\%s\\%s"), Machine.GetString(), NetworkResource.GetString());
	}
	else
	{
		strFullNetworkResource = NetworkResource;
	}

	CString strDomainUser;
	__CombineDomainUser(Domain, User, strDomainUser);

	hr = spLoc->ConnectServer( 
		CComBSTR(strFullNetworkResource.GetString()),///* [in] */ const BSTR strNetworkResource,
		strDomainUser.GetLength()? CComBSTR(strDomainUser.GetString()) : NULL,///* [in] */ const BSTR strUser,
		strDomainUser.GetLength() && Password.GetLength()? CComBSTR(Password.GetString()) : NULL,///* [in] */ const BSTR strPassword,
		NULL,///* [in] */ const BSTR strLocale,
		0,///* [in] */ long lSecurityFlags,
		NULL,///* [in] */ const BSTR strAuthority,
		NULL,///* [in] */ IWbemContext *pCtx,
		&spSvc///* [out] */ IWbemServices **ppNamespace
		);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("ConnectServer"), hr, TEXT("ROOT\\CIMV2"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	RPC_AUTH_IDENTITY_HANDLE  pAuthInfo = NULL;

	COAUTHIDENTITY AuthId = {0};
	AuthId.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
	if (strDomainUser.GetLength())
	{
		AuthId.User = (USHORT*)User.GetString();
		AuthId.UserLength = User.GetLength();
		AuthId.Domain = Domain.GetLength()? (USHORT*)Domain.GetString() : (USHORT*)TEXT(".");
		AuthId.DomainLength = Domain.GetLength()? Domain.GetLength() : 1;
		AuthId.Password = (USHORT*)Password.GetString();
		AuthId.PasswordLength = Password.GetLength();
		pAuthInfo = (RPC_AUTH_IDENTITY_HANDLE)(&AuthId);
	}

	hr = CoSetProxyBlanket(
		spSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx 
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx 
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		pAuthInfo,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("CoSetProxyBlanket"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	*ppIWbemServicesOut = spSvc.Detach();
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

//////////////////////////////////////////////////////////////////////////
static HRESULT RegisterEvent(
	IWbemServices * pSvc, 
	IWbemObjectSink * pMySink, 
	LPCTSTR strQuery,
	IWbemObjectSink ** ppSinkStub
	)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IUnsecuredApartment> spUnsecApp;
	hr = CoCreateInstance(
		CLSID_UnsecuredApartment, 
		NULL, 
		CLSCTX_LOCAL_SERVER, 
		IID_IUnsecuredApartment, 
		(LPVOID*)&spUnsecApp
		);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("CoCreateInstance"), hr, TEXT("IID_IUnsecuredApartment"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IUnknown> spStubUnk; 
	hr = spUnsecApp->CreateObjectStub(pMySink, &spStubUnk);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("CreateObjectStub"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spStubUnk->QueryInterface(
		IID_IWbemObjectSink,
		(LPVOID*)ppSinkStub
		);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("QueryInterface"), hr, TEXT("IID_IWbemObjectSink"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = pSvc->ExecNotificationQueryAsync(
		CComBSTR("WQL"), 
		CComBSTR(strQuery), 
		WBEM_FLAG_SEND_STATUS, 
		NULL, 
		*ppSinkStub
		);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("ExecNotificationQueryAsync"), hr, TEXT("%s"), strQuery);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
static CONST TCHAR EVENT_NAME[] = TEXT("Global\\WMIMonitor_2303D61EB69042F1A82DBCD284BF8C0A_%d");
static HANDLE g_hStopEvent = NULL;
static BOOL CtrlHandler( DWORD fdwCtrlType )
{
	switch (fdwCtrlType)
	{
	case  CTRL_C_EVENT:
	case  CTRL_CLOSE_EVENT:
	case  CTRL_BREAK_EVENT:
	case  CTRL_LOGOFF_EVENT:
	case  CTRL_SHUTDOWN_EVENT:
		SetEvent(g_hStopEvent);
		return TRUE; //do not pass to the next control handler function
	default:
		return FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
int _tmain_WMIProcessMonitor(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	CInitCOM InitCOM(COINIT_MULTITHREADED);
	

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	DWORD CurrentId = GetCurrentProcessId();
	CString strEvent;
	strEvent.Format(EVENT_NAME, CurrentId);
	g_hStopEvent = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		strEvent.GetString()//_In_opt_  LPCTSTR lpName
		);
	if (NULL == g_hStopEvent )
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	BOOL bSetConsoleCtrlHandler = SetConsoleCtrlHandler(
		(PHANDLER_ROUTINE)(CtrlHandler),//_In_opt_  PHANDLER_ROUTINE HandlerRoutine,
		TRUE//_In_      BOOL Add
		);
	if (!bSetConsoleCtrlHandler)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_API_ERR(0, TEXT("SetConsoleCtrlHandler"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = CoInitializeSecurity(
		NULL, 
		-1,                          // COM negotiates service
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("CoInitializeSecurity"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemServices> spSvc;
	hr = CreateIWbemServices(
		argc > 1? argv[1] : TEXT(""),//CONST CString & Machine,
		argc > 2? argv[2] : TEXT(""),////CONST CString & Domain,
		argc > 3? argv[3] : TEXT(""),//CONST CString & User,
		argc > 4? argv[4] : TEXT(""),//CONST CString & Password,
		TEXT("ROOT\\CIMV2"),//CONST CString & NetworkResource,
		&spSvc//IWbemServices ** ppIWbemServicesOut
		);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("CreateIWbemServices"), hr, TEXT("ROOT\\CIMV2"));
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemObjectSink> spMySinkEnd;
	ProcessEndEventSink::CreateInstance( &spMySinkEnd );
	
	CComPtr<IWbemObjectSink> spSinkEndStub;
	hr = RegisterEvent(
		spSvc,//IWbemServices * pSvc, 
		spMySinkEnd,//IWbemObjectSink * pObjectSinkNew, 
		TEXT("SELECT * FROM Win32_ProcessStopTrace"),//LPCTSTR strQuery,
		&spSinkEndStub//IWbemObjectSink ** ppObjectSinkOut
		);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("RegisterEvent"), hr, TEXT("SELECT * FROM Win32_ProcessStopTrace"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemObjectSink> spMySinkStart;
	ProcessStartEventSink::CreateInstance( &spMySinkStart );

	CComPtr<IWbemObjectSink> spSinkStartStub;
	hr = RegisterEvent(
		spSvc,//IWbemServices * pSvc, 
		spMySinkStart,//IWbemObjectSink * pObjectSinkNew, 
		TEXT("SELECT * FROM Win32_ProcessStartTrace"),//LPCTSTR strQuery,
		&spSinkStartStub//IWbemObjectSink ** ppObjectSinkOut
		);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("RegisterEvent"), hr, TEXT("SELECT * FROM Win32_ProcessStopTrace"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD dwWait = 0;
	CONST DWORD WAIR_INTERVAL = 3000;
	for (;;)
	{
		dwWait = WaitForSingleObject(g_hStopEvent, WAIR_INTERVAL);
		if (WAIT_OBJECT_0 == dwWait)
		{
			break;
		}
		else if (WAIT_TIMEOUT == dwWait)
		{
			continue;
		}
		else
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_API_ERR(0, TEXT(""), hr, TEXT(""));
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spSvc->CancelAsyncCall(spSinkEndStub);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("CancelAsyncCall"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	hr = spSvc->CancelAsyncCall(spSinkStartStub);
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT("CancelAsyncCall"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}


	//for (;;)
	//{
	//	Sleep(1000);
	//}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	if (g_hStopEvent)
	{
		CloseHandle(g_hStopEvent);
		g_hStopEvent = NULL;
	}
	
	return hr;
}

