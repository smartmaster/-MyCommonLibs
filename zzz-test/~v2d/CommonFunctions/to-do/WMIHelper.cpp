#include "stdafx.h"

#include <Wbemidl.h>

#include <iostream>
using namespace std;

#include "CodeBlockDef.h"
#include "WMIHelper.h"

HRESULT PrepareLocatorAndService(
									IWbemLocator ** ppLocator,
									IWbemServices ** ppService)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr =  CoInitializeSecurity(
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
	if (FAILED(hr))
	{
		cout << "Failed to initialize security. Error code = 0x" 
			<< hex << hr << endl;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	hr = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID *)ppLocator);
	if (FAILED(hr))
	{
		cout << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<< hex << hr << endl;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	hr = (**ppLocator).ConnectServer(
		CComBSTR(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (e.g. Kerberos)
		0,                       // Context object 
		ppService                    // pointer to IWbemServices proxy
		);
	if (FAILED(hr))
	{
		cout << "Could not connect. Error code = 0x" 
			<< hex << hr << endl;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	hr = CoSetProxyBlanket(
		*ppService,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);
	if (FAILED(hr))
	{
		cout << "Could not set proxy blanket. Error code = 0x" 
			<< hex << hr << endl;
		break;
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


HRESULT ExecQuery(IWbemServices *pService, LPCTSTR pQuery, vector<IWbemClassObject*> & VecObj)
{
	HRESULT hres = S_OK;

	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IEnumWbemClassObject> spEnumerator;
	hres = pService->ExecQuery(
		CComBSTR(L"WQL"), 
		CComBSTR(pQuery),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&spEnumerator);
	if (FAILED(hres))
	{
		cout << "Query for operating system name failed."
			<< " Error code = 0x" 
			<< hex << hres << endl;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;
	HRESULT hrEnum = S_OK;
	while (!!spEnumerator)
	{
		pclsObj = NULL;
		uReturn = 0;
		hrEnum = spEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if (0 == uReturn || FAILED(hrEnum))
		{
			break;
		}
		VecObj.push_back(pclsObj);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hres;
}


HRESULT GetObjProp(IWbemClassObject * pclsObj, LPCTSTR pName, CComVariant & VarProp)
{
	HRESULT hr = pclsObj->Get(pName, 0, &VarProp, 0, 0);
	return hr;
}

HRESULT ExecMethod( IWbemServices * pService, IWbemClassObject * pclsObj, LPCTSTR pClassName, LPCTSTR pMethodName, vector<CString> & ParamNames, vector<CComVariant> & ParamValues, CComVariant & ReturnValue )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spClass;
	hr = pService->GetObject(CComBSTR(pClassName), 0, NULL, &spClass, NULL);
	if (FAILED(hr))
	{
		cout << "Could not GetObject. Error code = 0x" 
			<< hex << hr << endl;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spInParamsDefinition;
	hr = spClass->GetMethod(CComBSTR(pMethodName), 0, &spInParamsDefinition, NULL);
	if (FAILED(hr))
	{
		cout << "Could not GetMethod. Error code = 0x" 
			<< hex << hr << endl;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spClassInstance;
	if (!!spInParamsDefinition)
	{
		hr = spInParamsDefinition->SpawnInstance(0, &spClassInstance);
		if (FAILED(hr))
		{
			cout << "Could not GetMethod. Error code = 0x" 
				<< hex << hr << endl;
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	ATLASSERT(ParamNames.size() == ParamValues.size());
	for (size_t ii = 0; ii < ParamNames.size(); ++ii)
	{
		hr = spClassInstance->Put(ParamNames[ii].GetString(), 0, &ParamValues[ii], 0);
		if (FAILED(hr))
		{
			cout << "Could not Put params. Error code = 0x" 
				<< hex << hr << endl;
			break;
		}
	}
	if (FAILED(hr))
	{
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	CComVariant VarPath;
	if (pclsObj)
	{
		hr = GetObjProp(pclsObj, TEXT("__PATH"), VarPath);
		if (FAILED(hr))
		{
			cout << "Could not object path. Error code = 0x" 
				<< hex << hr << endl;
			break;
		}
	}
	else
	{
		VarPath = pClassName;
	}


	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spOutParams;
	hr = pService->ExecMethod(VarPath.bstrVal, CComBSTR(pMethodName), 0, NULL, spClassInstance, &spOutParams, NULL);
	if (FAILED(hr))
	{
		cout << "Could not execute method. Error code = 0x" 
			<< hex << hr << endl;
		break;
	}


	//////////////////////////////////////////////////////////////////////////
	hr = spOutParams->Get(L"ReturnValue", 0, &ReturnValue, NULL, 0);
	if (FAILED(hr))
	{
		cout << "Could not Get ReturnValue. Error code = 0x" 
			<< hex << hr << endl;
		break;
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);


	return hr;
}
