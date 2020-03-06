// WMITools.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CodeBlockDef.h"
#include "WMIHelper.h"

#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>

#include "AutoUtility.h"

# pragma comment(lib, "wbemuuid.lib")

int tmain_000(int argc, TCHAR **argv)
{
	HRESULT hres;

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hres))
	{
		cout << "Failed to initialize COM library. Error code = 0x" 
			<< hex << hres << endl;
		return 1;                  // Program has failed.
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------
	// Note: If you are using Windows 2000, you need to specify -
	// the default authentication credentials for a user by using
	// a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
	// parameter of CoInitializeSecurity ------------------------

	hres =  CoInitializeSecurity(
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


	if (FAILED(hres))
	{
		cout << "Failed to initialize security. Error code = 0x" 
			<< hex << hres << endl;
		CoUninitialize();
		return 1;                    // Program has failed.
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,             
		0, 
		CLSCTX_INPROC_SERVER, 
		IID_IWbemLocator, (LPVOID *) &pLoc);

	if (FAILED(hres))
	{
		cout << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<< hex << hres << endl;
		CoUninitialize();
		return 1;                 // Program has failed.
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (e.g. Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
		);

	if (FAILED(hres))
	{
		cout << "Could not connect. Error code = 0x" 
			<< hex << hres << endl;
		pLoc->Release();     
		CoUninitialize();
		return 1;                // Program has failed.
	}

	cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;


	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
		cout << "Could not set proxy blanket. Error code = 0x" 
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();     
		CoUninitialize();
		return 1;               // Program has failed.
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"), 
		bstr_t("SELECT * FROM Win32_OperatingSystem"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		cout << "Query for operating system name failed."
			<< " Error code = 0x" 
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, 
			&pclsObj, &uReturn);

		if(0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		// Get the value of the Name property
		hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
		wcout << " OS Name : " << vtProp.bstrVal << endl;
		VariantClear(&vtProp);

		pclsObj->Release();
	}

	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	//pclsObj->Release();
	CoUninitialize();

	return 0;   // Program successfully completed.

}


void TransformStr(TCHAR * pStr, INT Count, INT xx)
{
	for (LONG ii = 0; ii < Count; ++ii)
	{
		pStr[ii] += xx;
	}
}

int _tmain_001(int argc, _TCHAR* argv[])
{



	HRESULT hr = S_OK;
	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemLocator> spLoc;
	CComPtr<IWbemServices> spService;
	hr = PrepareLocatorAndService(&spLoc, &spService);
	if (FAILED(hr))
	{
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	vector<IWbemClassObject*> VecObj;
	hr = ExecQuery(spService, TEXT("Select * From Win32_NetworkAdapterConfiguration"), VecObj);
	if (FAILED(hr))
	{
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	CString strDevIndex;
	INT DevIndex = _tcstol(argv[1], NULL, 10);
	strDevIndex.Format(TEXT("[%08d]"), DevIndex);
	for (size_t ii = 0; ii < VecObj.size(); ++ii)
	{
		CComVariant VarProp;
		hr = GetObjProp(VecObj[ii],TEXT("Caption"), VarProp);
		_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), VarProp.bstrVal);

		CString strNIC = VarProp.bstrVal;
		if (-1 != strNIC.Find(strDevIndex))
		{
			LPCTSTR pClassName = L"Win32_NetworkAdapterConfiguration";
			LPCTSTR pMethodName = L"EnableDHCP";


			vector<CString> ParamNames;
			//vector<CString> ParamNames(2);
			//ParamNames[0] = TEXT("WINSPrimaryServer");
			//ParamNames[1] = TEXT("WINSSecondaryServer");

			vector<CComVariant> ParamValues;
			//vector<CComVariant> ParamValues(2);
			//ParamValues[0] = TEXT("155.35.78.100");
			//ParamValues[1] = TEXT("155.35.78.200");

			CComVariant ReturnValue;
			ExecMethod(spService, 
				VecObj[ii], 				
				pClassName,
				pMethodName,
				ParamNames,
				ParamValues,
				ReturnValue);
		}
	}


	for (size_t ii = 0; ii < VecObj.size(); ++ii)
	{
		VecObj[ii]->Release();
	}
	VecObj.clear();



	END_BLOCK(0);
	return 0;
}

int _tmain_002(int argc, TCHAR ** argv)
{
	BEGIN_BLOCK(0);
	vector<CString> vec;
	for (LONG ii = 0; ii < 5; ++ii)
	{
		TCHAR tmp[] = TEXT("AAAAAA");
		TransformStr(tmp, _countof(tmp) - 1, ii);
		vec.push_back(tmp);
	}

	//SAFEARRAY * psa = NULL;
	//StringVectorToSafeArray(vec, &psa);

	//CComSafeArray<BSTR> ssa;
	//ssa.Attach(psa);

	CComVariant var;
	StringVectorToVar(vec, var);
	END_BLOCK(0);

	return 0;
}


int _tmain_003(int argc, TCHAR ** argv)
{
	BEGIN_BLOCK(0);
	vector<SHORT> vec;
	for (LONG ii = 0; ii < 5; ++ii)
	{
		vec.push_back(ii);
	}


	CComVariant var;
	IntegerVectorToVar<SHORT>(vec, var);
	END_BLOCK(0);

	return 0;
}





int _tmain(int argc, _TCHAR* argv[])
{
	//if (argc < 2)
	//{
	//	printf("this.exe devIndex\r\n");
	//	return -1;
	//}
	CoInitializeEx(NULL ,COINIT_MULTITHREADED);
	//tmain_000(argc, argv);
	//_tmain_001(argc, argv);
	_tmain_002(argc, argv);
	//_tmain_003(argc, argv);
	CoUninitialize();
	return 0;
}

