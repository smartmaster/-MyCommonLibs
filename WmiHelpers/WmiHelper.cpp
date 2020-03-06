#include "stdafx.h"
#include "WmiHelper.h"


VOID WMiHelper::__CombineDomainUser(CONST CString & Domain, CONST CString & User, CString & strDomainUser)
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

VOID WMiHelper::__SplitDomainUser(CONST CString & strDomainUser, CString & Domain, CString & User)
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

HRESULT WMiHelper::ApiConnectService(LPCTSTR Machine, LPCTSTR User, LPCTSTR Domain, LPCTSTR Password, LPCTSTR WmiNamespace, IWbemServices ** ppService)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	BOOL LocalConn = (NULL == Machine || 0 == Machine[0]);

	CString strFullNamespace;
	if (LocalConn)
	{
		strFullNamespace.Format(TEXT("\\\\.\\%s"), WmiNamespace);
	}
	else
	{
		strFullNamespace.Format(TEXT("\\\\%s\\%s"), Machine, WmiNamespace);
	}


	//////////////////////////////////////////////////////////////////////////
	if (LocalConn)
	{
		hr = CoInitializeSecurity(
			NULL,
			-1,                          // COM authentication
			NULL,                        // Authentication services
			NULL,                        // Reserved
			RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
			RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
			NULL,                        // Authentication info
			EOAC_NONE,                   // Additional capabilities
			NULL                         // Reserved
			);
	}
	else
	{
		hr = CoInitializeSecurity(
			NULL,
			-1,                          // COM authentication
			NULL,                        // Authentication services
			NULL,                        // Reserved
			RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
			RPC_C_IMP_LEVEL_IDENTIFY,    // Default Impersonation
			NULL,                        // Authentication info
			EOAC_NONE,                   // Additional capabilities
			NULL                         // Reserved
			);
	}
	if (FAILED(hr))
	{
		
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CoInitializeSecurity"), hr, TEXT(""));
		if (RPC_E_TOO_LATE != hr)
		{
			LEAVE_BLOCK(0);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemLocator> spLocator;
	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&spLocator);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CoCreateInstance"), hr, TEXT("CLSID_WbemLocator"));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CString strDomainUser;

	CComPtr<IWbemServices> spService;
	if (LocalConn)
	{
		hr = spLocator->ConnectServer(
			CComBSTR(strFullNamespace.GetString()),      // Object path of WMI namespace
			NULL,                    // User name. NULL = current user
			NULL,                    // User password. NULL = current
			0,                       // Locale. NULL indicates current
			NULL,                    // Security flags.
			0,                       // Authority (e.g. Kerberos)
			0,                       // Context object
			&spService                    // pointer to IWbemServices proxy
			);
	}
	else
	{
		__CombineDomainUser(Domain, User, strDomainUser);
		hr = spLocator->ConnectServer(
			CComBSTR(strFullNamespace.GetString()),  // Object path of WMI namespace
			CComBSTR(strDomainUser.GetString()),             // User name
			CComBSTR(Password),              // User password
			NULL,                // Locale
			NULL,                // Security flags
			NULL,				 // Authority
			NULL,                // Context object
			&spService                // IWbemServices proxy
			);
	}
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ConnectServer"), hr, TEXT("%s"), strFullNamespace.GetString());
		LEAVE_BLOCK(0);
	}

	if (LocalConn)
	{
		hr = CoSetProxyBlanket(
			spService,                        // Indicates the proxy to set
			RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
			NULL,                        // Server principal name
			RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
			RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
			NULL,                        // client identity
			EOAC_NONE                    // proxy capabilities
			);
	}
	else
	{
		// Create COAUTHIDENTITY that can be used for setting security on proxy
		CString strDomainTmp, strUserTmp, strPasswordTmp = Password;
		__SplitDomainUser(strDomainUser, strDomainTmp, strUserTmp);
		COAUTHIDENTITY authIdent;
		__ComposeAuthId(authIdent, strUserTmp, strDomainTmp, strPasswordTmp);

		hr = CoSetProxyBlanket(
			spService,                           // Indicates the proxy to set
			RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
			COLE_DEFAULT_PRINCIPAL,         // Server principal name
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx
			RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
			&authIdent,                       // client identity
			EOAC_NONE                       // proxy capabilities
			);
	}
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CoSetProxyBlanket"), hr, TEXT(""));
		if (RPC_E_TOO_LATE != hr)
		{
			LEAVE_BLOCK(0);
		}
	}

	*ppService = spService.Detach();
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::GetClass(IWbemServices * pService, LPCTSTR ClassName, IWbemClassObject ** ppClass)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = pService->GetObject(CComBSTR(ClassName), 0, NULL, ppClass, NULL);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetObject"), hr, TEXT("%s"), ClassName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::NewInstance(IWbemServices * pService, LPCTSTR ClassName, IWbemClassObject ** ppInstance)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spClass;
	hr = GetClass(pService, ClassName, &spClass);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ClassName"), hr, TEXT("%s"), ClassName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spClass->SpawnInstance(0, ppInstance);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("SpawnInstance"), hr, TEXT("%s"), ClassName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::ClassGetInstance(IWbemClassObject * pClass, LPCTSTR ClassName, IWbemClassObject ** ppInstance)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = pClass->SpawnInstance(0, ppInstance);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("SpawnInstance"), hr, TEXT("%s"), ClassName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::GetMethodIn(IWbemServices * pService, LPCTSTR ClassName, LPCTSTR MethodName, IWbemClassObject ** ppMethodInParam)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spClass;
	hr = GetClass(pService, ClassName, &spClass);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ServiceGetClass"), hr, TEXT("%s"), ClassName);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spClassMethodInParams;
	hr = spClass->GetMethod(MethodName, 0, &spClassMethodInParams, NULL);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetMethod"), hr, TEXT("%s"), MethodName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spClassMethodInParams->SpawnInstance(0, ppMethodInParam);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("SpawnInstance"), hr, TEXT("%s"), MethodName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::Put(IWbemClassObject * pInstance, LPCTSTR name, VARIANT * pvar)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	HRESULT hr = pInstance->Put(name, 0, pvar, 0);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Put"), hr, TEXT("%s"), name);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::Get(IWbemClassObject * pInstance, LPCTSTR name, VARIANT * pvar)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = pInstance->Get(name, 0, pvar, NULL, NULL);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Get"), hr, TEXT("%s"), name);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::Get(IWbemClassObject * pInstance, CONST vector<CString> & names, vector<CComVariant> & vars)
{
	HRESULT hr = S_OK;
	ATLASSERT(names.size() == vars.size());
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		hr = Get(pInstance, names[ii].GetString(), &vars[ii]);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Get"), hr, TEXT("names[ii].GetString()"));
			break;
		}

	}

	return hr;
}

HRESULT WMiHelper::Put(IWbemClassObject * pInstance, CONST vector<CString> & names, vector<CComVariant> & vars)
{
	HRESULT hr = S_OK;
	ATLASSERT(names.size() == vars.size());
	for (size_t ii = 0; ii < names.size(); ++ii)
	{
		hr = Put(pInstance, names[ii].GetString(), &vars[ii]);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Put"), hr, TEXT("names[ii].GetString()"));
			break;
		}

	}
	return hr;
}

HRESULT WMiHelper::GetPath(IWbemClassObject * pInstance, CString & strObjPath)
{
	HRESULT hr = S_OK;
	CComVariant var;
	hr = Get(pInstance, TEXT("__path"), &var);
	if (V_VT(&var) == VT_BSTR)
	{
		strObjPath = var.bstrVal;
	}
	return hr;
}

HRESULT WMiHelper::ExecMethod(IWbemServices * pService, LPCTSTR ClassOrPathname, LPCTSTR MethodName, IWbemClassObject * pMethodInParam, IWbemClassObject ** ppMethodOutParam)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	hr = pService->ExecMethod(
		CComBSTR(ClassOrPathname),///* [in] */ __RPC__in const BSTR strObjectPath,
		CComBSTR(MethodName),///* [in] */ __RPC__in const BSTR strMethodName,
		0,///* [in] */ long lFlags,
		NULL,///* [in] */ __RPC__in_opt IWbemContext *pCtx,
		pMethodInParam,///* [in] */ __RPC__in_opt IWbemClassObject *pInParams,
		ppMethodOutParam,///* [unique][in][out] */ __RPC__deref_opt_inout_opt IWbemClassObject **ppOutParams,
		NULL///* [unique][in][out] */ __RPC__deref_opt_inout_opt IWbemCallResult **ppCallResult
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ExecMethod"), hr, TEXT("%s.%s"), ClassOrPathname, MethodName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::ExecMethod(IWbemServices * pService, IWbemClassObject * pInstance, LPCTSTR MethodName, IWbemClassObject * pMethodInParam, IWbemClassObject ** ppMethodOutParam)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strPath;
	hr = GetPath(pInstance, strPath);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Get"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = ExecMethod(pService, strPath.GetString(), MethodName, pMethodInParam, ppMethodOutParam);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ExecMethod"), hr, TEXT("%s.%s"), strPath.GetString(), MethodName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::ApiGetAllProps(IWbemClassObject * pInstance, vector<CString> & names, vector<CComVariant> & vars) //names and vars are allocated by the function
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = pInstance->BeginEnumeration(
		0///* [in] */ long lEnumFlags
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("BeginEnumeration"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//vars.reserve(256);
	for (;;)
	{
		CComBSTR name;
		CComVariant var;
		hr = pInstance->Next(
			NULL,///* [in] */ long lFlags,
			&name,///* [unique][in][out] */ BSTR *strName,
			&var,///* [unique][in][out] */ VARIANT *pVal,
			NULL,///* [unique][in][out] */ CIMTYPE *pType,
			NULL///* [unique][in][out] */ long *plFlavor
			);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Next"), hr, TEXT(""));
			LEAVE_BLOCK(0);
			break;
		}
		else if (S_FALSE == hr)
		{
			break;
		}
		else if (NULL == (LPCTSTR)(name))
		{
			break;
		}

		names.push_back((LPCTSTR)(name));
		vars.push_back(var);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	HRESULT hrTemp = pInstance->EndEnumeration();
	if (FAILED(hrTemp))
	{
		hr = hrTemp;
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("EndEnumeration"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	return hr;
}

HRESULT WMiHelper::ApiGetAllProps(
	IN IWbemServices * pService,
	IN LPCTSTR className,
	OUT vector<CString> & names,
	OUT vector<CComVariant> & vars //names and vars are allocated by the function;
	)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spClass;
	hr = GetClass(pService, className, &spClass);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetClass"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = ApiGetAllProps(spClass.p, names, vars);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetAllMethods"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::ApiInvokeMethod(IN IWbemServices * pService, IN IWbemClassObject * pInstance, /*null to call class static method, non-null to call object method */ IN LPCTSTR className, IN LPCTSTR methodName, IN CONST vector<CString> & namesIN, /*allocated by caller */ IN vector<CComVariant> & varsIN, /*allocated by caller */ OUT vector<CString> & namesOut, /*allocated by the function */ OUT vector<CComVariant> & varsOut /*allocated by the function */)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spMethodIn;
	hr = GetMethodIn(
		pService,//IWbemServices * pService,
		className,//LPCTSTR ClassName,
		methodName,//LPCTSTR MethodName,
		&spMethodIn//IWbemClassObject ** ppMethodInParam
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetMethodIn"), hr, TEXT("%s.%s"), className, methodName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	if (spMethodIn.p)
	{
		hr = Put(spMethodIn.p, namesIN, varsIN);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Put"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	CComPtr<IWbemClassObject> spMethodOut;
	if (pInstance)
	{
		hr = ExecMethod(
			pService,//IWbemServices * pService,
			pInstance,//IWbemClassObject * pInstance,
			methodName,//LPCTSTR MethodName,
			spMethodIn.p,//IWbemClassObject * pMethodInParam,
			&spMethodOut//IWbemClassObject ** ppMethodOutParam
			);
	}
	else
	{
		hr = ExecMethod(
			pService,//IWbemServices * pService,
			className,//LPCTSTR ClassOrPathname,
			methodName,//LPCTSTR MethodName,
			spMethodIn.p,//IWbemClassObject * pMethodInParam,
			&spMethodOut//IWbemClassObject ** ppMethodOutParam
			);
	}
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ExecMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	if (spMethodOut.p)
	{
		hr = ApiGetAllProps(spMethodOut.p, namesOut, varsOut);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("GetAll"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::ApiNewInstanceVar(IWbemServices * pService, LPCTSTR className, CONST vector<CString> & names, vector<CComVariant> & vars, CComVariant & varOut)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spInstancePS;
	hr = NewInstance(pService, className, &spInstancePS);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetInstance"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = Put(spInstancePS, names, vars);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Put"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	varOut = (IUnknown*)(spInstancePS.p);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::ApiEnumInstances(
	IN IWbemServices * pService,
	IN LPCTSTR className,
	IN LPCTSTR whereClause,
	IN COAUTHIDENTITY * pAuthId,
	OUT vector<CComPtr<IWbemClassObject> > & vecInstances
	)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString strWql;
	strWql.Format(TEXT("SELECT * FROM %s"), className);
	if (whereClause && whereClause[0])
	{
		strWql += TEXT(" WHERE ");
		strWql += whereClause;
	}

	CComPtr<IEnumWbemClassObject> spEnumerator;
	hr = pService->ExecQuery(
		CComBSTR(L"WQL"),
		CComBSTR(strWql.GetString()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&spEnumerator
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ExecQuery"), hr, TEXT("%s"), strWql.GetString());
		LEAVE_BLOCK(0);
	}

	// Secure the enumerator proxy
	if (pAuthId)
	{

		hr = CoSetProxyBlanket(
			spEnumerator,                    // Indicates the proxy to set
			RPC_C_AUTHN_DEFAULT,            // RPC_C_AUTHN_xxx
			RPC_C_AUTHZ_DEFAULT,            // RPC_C_AUTHZ_xxx
			COLE_DEFAULT_PRINCIPAL,         // Server principal name
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  // RPC_C_AUTHN_LEVEL_xxx
			RPC_C_IMP_LEVEL_IMPERSONATE,    // RPC_C_IMP_LEVEL_xxx
			pAuthId,                       // client identity
			EOAC_NONE                       // proxy capabilities
			);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("CoSetProxyBlanket"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	while (spEnumerator.p)
	{
		CComPtr<IWbemClassObject> spInstance;
		ULONG uReturn = 0;
		hr = spEnumerator->Next(WBEM_INFINITE, 1, &spInstance, &uReturn);

		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Next"), hr, TEXT(""));
			break;
		}

		if (0 == uReturn || S_FALSE == hr)
		{
			break;
		}

		vecInstances.push_back(spInstance);
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::GetAllMethods(IN IWbemClassObject * pInstance, OUT vector<CString> & names, OUT vector<CComPtr<IWbemClassObject> > & vecParamsIn, OUT vector<CComPtr<IWbemClassObject> > & vecParamsOut) //names and vars are allocated by the function
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = pInstance->BeginMethodEnumeration(
		0///* [in] */ long lEnumFlags
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("BeginMethodEnumeration"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	for (;;)
	{
		CComBSTR name;
		CComPtr<IWbemClassObject> spIn;
		CComPtr<IWbemClassObject> spOut;
		hr = pInstance->NextMethod(
			0,///* [in] */ long lFlags,
			&name,///* [unique][in][out] */ BSTR *pstrName,
			&spIn,///* [unique][in][out] */ IWbemClassObject **ppInSignature,
			&spOut///* [unique][in][out] */ IWbemClassObject **ppOutSignature
			);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("NextMethod"), hr, TEXT(""));
			LEAVE_BLOCK(0);
			break;
		}
		else if (S_FALSE == hr)
		{
			break;
		}
		else if (NULL == (LPCTSTR)(name))
		{
			break;
		}

		names.push_back((LPCTSTR)(name));
		vecParamsIn.push_back(spIn);
		vecParamsOut.push_back(spOut);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	HRESULT hrTemp = pInstance->EndMethodEnumeration();
	if (FAILED(hrTemp))
	{
		hr = hrTemp;
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("EndMethodEnumeration"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	return hr;
}

HRESULT WMiHelper::ApiGetAllMethods(IN IWbemServices * pService, IN LPCTSTR className, OUT vector<CString> & names, OUT vector<CComPtr<IWbemClassObject> > & vecParamsIn, OUT vector<CComPtr<IWbemClassObject> > & vecParamsOut)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spClass;
	hr = GetClass(pService, className, &spClass);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetClass"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = GetAllMethods(spClass, names, vecParamsIn, vecParamsOut);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetAllMethods"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT WMiHelper::RegisterEvent(IWbemServices * pService, IWbemObjectSink * pMySink, LPCTSTR strQuery, IWbemObjectSink ** ppSinkStub)
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
		D_API_ERR(0, TEXT("CoCreateInstance"), hr, TEXT("IID_IUnsecuredApartment"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IUnknown> spStubUnk;
	hr = spUnsecApp->CreateObjectStub(pMySink, &spStubUnk);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateObjectStub"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = spStubUnk->QueryInterface(
		IID_IWbemObjectSink,
		(LPVOID*)ppSinkStub
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("QueryInterface"), hr, TEXT("IID_IWbemObjectSink"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = pService->ExecNotificationQueryAsync(
		CComBSTR("WQL"),
		CComBSTR(strQuery),
		0,//WBEM_FLAG_SEND_STATUS,
		NULL,
		*ppSinkStub
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ExecNotificationQueryAsync"), hr, TEXT("%s"), strQuery);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::ApiMonitorEvent(IN IWbemServices * pService, IN LPCTSTR className, IN IMyEeventHandler * pHandler, IN ULONG TimeOut)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<CMyEventSink> apMySink;
	apMySink = (new CMyEventSink(pHandler)); //add ref
	if (NULL == apMySink.p)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = E_OUTOFMEMORY;
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("new"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CString strQuery;
	CComPtr<IWbemObjectSink> spSinkStub;
	strQuery.Format(TEXT("SELECT * FROM %s"), className);
	hr = RegisterEvent(
		pService,//IWbemServices * pSvc,
		apMySink.p,//IWbemObjectSink * pMySink,
		strQuery.GetString(),//LPCTSTR strQuery,
		&spSinkStub//IWbemObjectSink ** ppSinkStub
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("RegisterEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = pHandler->Wait(TimeOut);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Handler->Wait"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}


	hr = pService->CancelAsyncCall(spSinkStub);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CancelAsyncCall"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::ApiStartMonitorEvent(
	IN IWbemServices * pService,
	IN LPCTSTR className,
	IN IMyEeventHandler * pHandler,
	OUT VOID ** ppObjectSink
)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<CMyEventSink> apMySink;
	apMySink = (new CMyEventSink(pHandler)); //add ref
	if (NULL == apMySink.p)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = E_OUTOFMEMORY;
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("new"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CString strQuery;
	CComPtr<IWbemObjectSink> spSinkStub;
	strQuery.Format(TEXT("SELECT * FROM %s"), className);
	hr = RegisterEvent(
		pService,//IWbemServices * pSvc,
		apMySink.p,//IWbemObjectSink * pMySink,
		strQuery.GetString(),//LPCTSTR strQuery,
		&spSinkStub//IWbemObjectSink ** ppSinkStub
	);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("RegisterEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	*ppObjectSink = spSinkStub.Detach();
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WMiHelper::ApiEndMonitorEvent(
	IN IWbemServices * pService,
	IN VOID * pObjectSink
)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	
	CComPtr<IWbemObjectSink> spSinkStub;
	spSinkStub.Attach((IWbemObjectSink*)(pObjectSink));

	hr = pService->CancelAsyncCall(spSinkStub);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CancelAsyncCall"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


VOID WMiHelper::__ComposeAuthId(COAUTHIDENTITY & authIdent, CString & strUserTmp, CString & strDomainTmp, CString & strPasswordTmp)
{
	memset(&authIdent, 0, sizeof(COAUTHIDENTITY));

	authIdent.PasswordLength = strPasswordTmp.GetLength();
	authIdent.Password = (USHORT*)strPasswordTmp.GetString();

	authIdent.User = (USHORT*)strUserTmp.GetString();
	authIdent.UserLength = strUserTmp.GetLength();

	authIdent.Domain = (USHORT*)strDomainTmp.GetString();;
	authIdent.DomainLength = strDomainTmp.GetLength();

	authIdent.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
}

//////////////////////////////////////////////////////////////////////////
WMiHelper::CMyEventSink::CMyEventSink(IMyEeventHandler * pHandler) :
m_lRef(0),
m_bDone(FALSE),
m_handler(pHandler)
{

}

WMiHelper::CMyEventSink::~CMyEventSink()
{
	m_bDone = TRUE;
}

ULONG STDMETHODCALLTYPE WMiHelper::CMyEventSink::AddRef()
{
	return InterlockedIncrement(&m_lRef);
}

ULONG STDMETHODCALLTYPE WMiHelper::CMyEventSink::Release()
{
	LONG lRef = InterlockedDecrement(&m_lRef);
	if (lRef == 0)
	{
		delete this;
	}
	return lRef;
}

HRESULT STDMETHODCALLTYPE WMiHelper::CMyEventSink::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
	{
		*ppv = (IWbemObjectSink *) this;
		AddRef();
		return WBEM_S_NO_ERROR;
	}
	else
	{
		return E_NOINTERFACE;
	}
}

HRESULT STDMETHODCALLTYPE WMiHelper::CMyEventSink::Indicate(LONG lObjectCount, IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray)
{
	return m_handler->Indicate(lObjectCount, apObjArray);
}

HRESULT STDMETHODCALLTYPE WMiHelper::CMyEventSink::SetStatus(/* [in] */ LONG lFlags, /* [in] */ HRESULT hResult, /* [in] */ BSTR strParam, /* [in] */ IWbemClassObject __RPC_FAR *pObjParam)
{
	if (lFlags == WBEM_STATUS_COMPLETE)
	{
		D_INFO(9999, TEXT("Call complete. hResult = 0x%08X"), hResult);
	}
	else if (lFlags == WBEM_STATUS_PROGRESS)
	{
		D_INFO(9999, TEXT("Call in progress"));
	}

	return WBEM_S_NO_ERROR;
}
