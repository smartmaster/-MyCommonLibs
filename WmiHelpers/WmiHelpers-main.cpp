// WmiHelpers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <ClusApi.h>
#include <fstream>
#include <codecvt>
#include <locale>

#include "WmiHelper.h"

#include "test_storage_api_enum.h"
#include "MyMsftStorage.h"
#include "MySystemInfo.h"
#include "TestNFS.h"
#include "MyTestSmb.h"
#include "SmlWmi.test.h"
#include "SmlWmiParser.test.h"
#include "test_generated_wmi_class.h"
#include "MyFormatCode.test.h"

#if 0
int _tmain_001(int argc, _TCHAR* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemServices> spService;
	hr = WMiHelper::ApiConnectService(
		argc > 1 ? argv[1] : NULL,//LPCTSTR Machine, 
		argc > 2 ? argv[2] : NULL,//LPCTSTR User, 
		argc > 3 ? argv[3] : NULL,//LPCTSTR Domain, 
		argc > 4 ? argv[4] : NULL,//LPCTSTR Password, 
		TEXT("root\\cimv2"),//argc > 5 ? argv[5] : NULL,//LPCTSTR WmiNamespace, 
		&spService//IWbemServices ** ppService
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
		D_API_ERR(0, TEXT("ConnectService"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spMethodInParam;
	hr = WMiHelper::GetMethodIn(
		spService,//IWbemServices * pService,
		TEXT("Win32_Process"),//LPCTSTR ClassName,
		TEXT("Create"),//LPCTSTR MethodName,
		&spMethodInParam//IWbemClassObject ** ppMethodInParam
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
		D_API_ERR(0, TEXT("GetMethodInParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	vector<CString> paramNames(3);
	paramNames[0] = TEXT("CommandLine");
	paramNames[1] = TEXT("CurrentDirectory");
	paramNames[2] = TEXT("ProcessStartupInformation");

	vector<CComVariant> params(paramNames.size());
	params[0] = TEXT("c:\\windows\\system32\\notepad.exe");
	params[1] = TEXT("c:\\windows\\system32\\");

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spInstancePS;
	hr = WMiHelper::NewInstance(spService, TEXT("Win32_ProcessStartup"), &spInstancePS);
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

	vector<CString> namesProcessStartup(3);
	namesProcessStartup[0] = TEXT("WinstationDesktop");
	namesProcessStartup[1] = TEXT("ShowWindow");
	namesProcessStartup[2] = TEXT("Title");

	vector<CComVariant> paramsProcessStartup(namesProcessStartup.size());
	paramsProcessStartup[0] = TEXT("");
	paramsProcessStartup[1] = (BYTE)(SW_SHOWMAXIMIZED);
	paramsProcessStartup[2] = TEXT("test wmi");

	hr = WMiHelper::Put(spInstancePS, namesProcessStartup, paramsProcessStartup);
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

	//////////////////////////////////////////////////////////////////////////
	params[2] = (IUnknown*)(spInstancePS.p);
	spInstancePS.Release();

	hr = WMiHelper::Put(spMethodInParam, paramNames, params);
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

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemClassObject> spMethodOut;
	hr = WMiHelper::ExecMethod(spService, TEXT("Win32_Process"), TEXT("Create"), spMethodInParam, &spMethodOut);
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

	vector<CString> namesOut;
	vector<CComVariant> varsOut;
	hr = WMiHelper::ApiGetAll(
		spMethodOut,//IWbemClassObject * pInstance,
		namesOut,//vector<CString> & names,
		varsOut//vector<CComVariant> & vars
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
		D_API_ERR(0, TEXT("GetAll"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	CoUninitialize();
	return 0;
}
#endif

int _tmain_test_delete_group(int argc, _TCHAR* argv[])
{
	//int _tmain_test_ps(int argc, _TCHAR* argv[])
	//select * from MSCluster_ResourceGroup

	CONST TCHAR wmiRes[] = TEXT("root\\mscluster");
	CONST TCHAR ClassName[] = TEXT("MSCluster_ResourceGroup");
	CONST TCHAR MethodName[] = TEXT("Create");

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CString user = TEXT("administrator");
	CString domain = TEXT("08r2cluster");
	CString password = TEXT("1qaz@WSX3edc$RFV");
	COAUTHIDENTITY authid = { 0 };
	WMiHelper::__ComposeAuthId(authid, user, domain, password);


	CComPtr<IWbemServices> spService;
	hr = WMiHelper::ApiConnectService(
		TEXT("lijyo02-08r2-1"),//argc > 1 ? argv[1] : NULL,,//LPCTSTR Machine, 
		user.GetString(),//TEXT("administrator"),//argc > 2 ? argv[2] : NULL,,//LPCTSTR User, 
		domain.GetString(),//TEXT("08r2cluster"),//argc > 3 ? argv[3] : NULL,,//LPCTSTR Domain, 
		password.GetString(),//TEXT("1qaz@WSX3edc$RFV"),//argc > 4 ? argv[4] : NULL,,//LPCTSTR Password, 
		wmiRes,//TEXT("root\\cimv2"),,//argc > 5 ? argv[5] : NULL,,//LPCTSTR WmiNamespace, 
		&spService//IWbemServices ** ppService
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
		D_API_ERR(0, TEXT("ConnectService"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	CString strWhere = TEXT("Name = 'sss'");
	vector<CComPtr<IWbemClassObject> > vecInstances;
	hr = WMiHelper::ApiEnumInstances(
		spService,//IN IWbemServices * pService,
		ClassName,//IN LPCTSTR className,
		strWhere.GetString(),//IN LPCTSTR whereClause,
		&authid,
		vecInstances//OUT vector<CComPtr<IWbemClassObject> > & vecInstances
	);
	if (0 == vecInstances.size())
	{
		LEAVE_BLOCK(0);
	}


	//vector<CString> namesIN;//allocated by caller
	//vector<CComVariant> varsIN;//allocated by callerhr = WMiHelper::ApiInvokeMethod(
	//vector<CString> namesOut;//allocated by the function	spService,//IN IWbemServices * pService,
	//vector<CComVariant>varsOut;//allocated by the function	vecInstances[0],//IN IWbemClassObject * pInstance,	,//null to call class static method, non-null to call object method
	//	ClassName,//IN LPCTSTR className,
	//	MethodName,//IN LPCTSTR methodName,
	//	,//IN CONST vector<CString> & namesIN, ,//allocated by caller
	//	,//IN vector<CComVariant> & varsIN,	,//allocated by caller
	//	,//OUT vector<CString> & namesOut,		,//allocated by the function
	//	,//OUT vector<CComVariant> & varsOut	,//allocated by the function
	//	);



	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}



int _tmain_test_ps(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	CONST TCHAR ClassName[] = TEXT("Win32_Process");
	CONST TCHAR MethodName[] = TEXT("Create");


	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////

	CString user = TEXT("administrator");
	CString domain = TEXT("08r2cluster");
	CString password = TEXT("1qaz@WSX3edc$RFV");
	COAUTHIDENTITY authid = { 0 };
	WMiHelper::__ComposeAuthId(authid, user, domain, password);


	CComPtr<IWbemServices> spService;
	hr = WMiHelper::ApiConnectService(
		TEXT("lijyo02-08r2-1"),//argc > 1 ? argv[1] : NULL,,//LPCTSTR Machine, 
		user.GetString(),//TEXT("administrator"),//argc > 2 ? argv[2] : NULL,,//LPCTSTR User, 
		domain.GetString(),//TEXT("08r2cluster"),//argc > 3 ? argv[3] : NULL,,//LPCTSTR Domain, 
		password.GetString(),//TEXT("1qaz@WSX3edc$RFV"),//argc > 4 ? argv[4] : NULL,,//LPCTSTR Password, 
		TEXT("root\\cimv2"),//TEXT("root\\cimv2"),,//argc > 5 ? argv[5] : NULL,,//LPCTSTR WmiNamespace, 
		&spService//IWbemServices ** ppService
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
		D_API_ERR(0, TEXT("ConnectService"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	CString strCommandLine;
	strCommandLine.Format(L"PowerShell.exe -noexit import-module failoverclusters ; Add-ClusterVirtualMachineRole -VirtualMachine \"%s\" -Name \"%s\"",
		TEXT("sss"), TEXT("sss"));

	vector<CString> paramNames(2);
	paramNames[0] = TEXT("CommandLine");
	paramNames[1] = TEXT("CurrentDirectory");

	vector<CComVariant> params(paramNames.size());
	params[0] = strCommandLine.GetString();
	params[1] = TEXT("c:\\windows\\system32\\");


	vector<CString> namesOut;
	vector<CComVariant> varsOut;
	hr = WMiHelper::ApiInvokeMethod(
		spService,//IN IWbemServices * pService,
		NULL,//IN IWbemClassObject * pInstance,	,//null to call class static method, non-null to call object method
		ClassName,//IN LPCTSTR className,
		MethodName,//IN LPCTSTR methodName,
		paramNames,//IN CONST vector<CString> & namesIN, ,//allocated by caller
		params,//IN vector<CComVariant> & varsIN,	,//allocated by caller
		namesOut,//OUT vector<CString> & namesOut,		,//allocated by the function
		varsOut//OUT vector<CComVariant> & varsOut	,//allocated by the function
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
		D_API_ERR(0, TEXT("ApiInvokeMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	CString strWhere;
	strWhere.Format(TEXT("%s = %d"), namesOut[10].GetString(), varsOut[10].ulVal);
	for (;;)
	{
		vector<CComPtr<IWbemClassObject> > vecInstances;
		WMiHelper::ApiEnumInstances(
			spService,//IN IWbemServices * pService,
			ClassName,//IN LPCTSTR className,
			strWhere.GetString(),//IN LPCTSTR whereClause,
			&authid,
			vecInstances//OUT vector<CComPtr<IWbemClassObject> > & vecInstances
		);

		if (0 == vecInstances.size())
		{
			break;
		}

		Sleep(5000);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


int _tmain_test_mscluster_cluster(int argc, _TCHAR* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////

	LPCTSTR ClassName = TEXT("MSCluster_Cluster");
	LPCTSTR MethodName = TEXT("ExecuteClusterControl");

	CString machine = TEXT("lijyo02-08r2-1");
	CString user = TEXT("administrator");
	CString domain = TEXT("08r2cluster");
	CString password = TEXT("1qaz@WSX3edc$RFV");
	CString wmiRes = TEXT("root\\mscluster");
	COAUTHIDENTITY authid = { 0 };
	WMiHelper::__ComposeAuthId(authid, user, domain, password);


	CComPtr<IWbemServices> spService;
	hr = WMiHelper::ApiConnectService(
		machine.GetString(),//argc > 1 ? argv[1] : NULL,,//LPCTSTR Machine, 
		user.GetString(),//TEXT("administrator"),//argc > 2 ? argv[2] : NULL,,//LPCTSTR User, 
		domain.GetString(),//TEXT("08r2cluster"),//argc > 3 ? argv[3] : NULL,,//LPCTSTR Domain, 
		password.GetString(),//TEXT("1qaz@WSX3edc$RFV"),//argc > 4 ? argv[4] : NULL,,//LPCTSTR Password, 
		wmiRes.GetString(),//TEXT("root\\cimv2"),,//argc > 5 ? argv[5] : NULL,,//LPCTSTR WmiNamespace, 
		&spService//IWbemServices ** ppService
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
		D_API_ERR(0, TEXT("ConnectService"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	vector<CComPtr<IWbemClassObject> > vecInstances;
	hr = WMiHelper::ApiEnumInstances(
		spService,//IN IWbemServices * pService,
		ClassName,//IN LPCTSTR className,
		NULL,//IN LPCTSTR whereClause,
		&authid,//IN COAUTHIDENTITY * AuthId,
		vecInstances//OUT vector<CComPtr<IWbemClassObject> > & vecInstances
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
		D_API_ERR(0, TEXT("ApiEnumInstances"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	vector<CString> namesIN(2);
	namesIN[0] = TEXT("ControlCode");
	namesIN[1] = TEXT("InputBuffer");


	vector<CComVariant> valuesIN(namesIN.size());
	valuesIN[0] = (INT32)(CLUSCTL_CLUSTER_GET_SHARED_VOLUME_ID);

	TCHAR rawPath[] = TEXT("C:\\ClusterStorage\\Volume3");
	vector<BYTE> vec(sizeof(rawPath));
	BYTE* pByte = (BYTE*)(rawPath);
	for (size_t ii = 0; ii < vec.size(); ii++)
	{
		vec[ii] = pByte[ii];
	}
	WMiHelper::__VectorToVar<BYTE, BYTE>(vec, valuesIN[1]);

	vector<CString> namesOut;
	vector<CComVariant> varsOut;
	hr = WMiHelper::ApiInvokeMethod(
		spService,//IN IWbemServices * pService,
		vecInstances[0],//IN IWbemClassObject * pInstance,	,//null to call class static method, non-null to call object method
		ClassName,//IN LPCTSTR className,
		MethodName,//IN LPCTSTR methodName,
		namesIN,//IN CONST vector<CString> & namesIN, ,//allocated by caller
		valuesIN,//IN vector<CComVariant> & varsIN,	,//allocated by caller
		namesOut,//OUT vector<CString> & namesOut,		,//allocated by the function
		varsOut//OUT vector<CComVariant> & varsOut	,//allocated by the function
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
		D_API_ERR(0, TEXT("ApiInvokeMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	vector<BYTE> vecOut;
	WMiHelper::__VarToVector<BYTE, BYTE>(varsOut[10], vecOut);
	CString strOut = (LPCTSTR)(BYTE*)(&vecOut[0]);


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	CoUninitialize();
	return 0;
}



int _tmain_test_sa(int argc, _TCHAR* argv[])
{
	{
		vector<LONG> vec0{ 1, 2, 3, 4 }; //C++ 11 
		CComSafeArray<LONG> sa0;
		WMiHelper::__VectorToSafeArray<LONG, LONG>(vec0, sa0);

		vector<LONG> vec1;
		WMiHelper::__SafeArrayToVector<LONG, LONG>(sa0, vec1);

		CComVariant var0;
		WMiHelper::__VectorToVar<LONG, LONG>(vec1, var0);

		vector<LONG> vec2;
		WMiHelper::__VarToVector<LONG, LONG>(var0, vec2);

		sa0.Destroy();
	}


	{
		vector<CString> vec0(4);
		CComSafeArray<BSTR> sa0;
		TCHAR raw[] = TEXT("axxxxxxx");
		for (size_t ii = 0; ii < vec0.size(); ++ii)
		{
			vec0[ii] = raw;
			++raw[0];
		}
		WMiHelper::__VectorToSafeArray<CString, BSTR>(vec0, sa0);

		vector<CString> vec1;
		WMiHelper::__SafeArrayToVector<CString, BSTR>(sa0, vec1);

		CComVariant var0;
		WMiHelper::__VectorToVar<CString, BSTR>(vec1, var0);

		vector<CString> vec2;
		WMiHelper::__VarToVector<CString, BSTR>(var0, vec2);

		sa0.Destroy();
	}


	return 0;
}

int _tmain_test_apis(int argc, _TCHAR* argv[])
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////

	{
		//_tmain_test_mscluster_cluster(argc, argv);
		//_tmain_test_sa(argc, argv);
		//_tmain_test_ps(argc, argv);
	}

	CComPtr<IWbemServices> spService;
	hr = WMiHelper::ApiConnectService(
		argc > 1 ? argv[1] : NULL,//LPCTSTR Machine, 
		argc > 2 ? argv[2] : NULL,//LPCTSTR User, 
		argc > 3 ? argv[3] : NULL,//LPCTSTR Domain, 
		argc > 4 ? argv[4] : NULL,//LPCTSTR Password, 
		TEXT("root\\cimv2"),//argc > 5 ? argv[5] : NULL,//LPCTSTR WmiNamespace, 
		&spService//IWbemServices ** ppService
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
		D_API_ERR(0, TEXT("ConnectService"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	class CMyEeventHandler : public IWbemObjectSink, public WMiHelper::IMyEeventHandler
	{
	private:
		LONG m_lRef{ 0 };
		bool bDone{ false };


	public:
		virtual ULONG STDMETHODCALLTYPE AddRef()
		{
			return InterlockedIncrement(&m_lRef);
		}

		virtual ULONG STDMETHODCALLTYPE Release()
		{
			LONG lRef = InterlockedDecrement(&m_lRef);
			if (lRef == 0)
				delete this;
			return lRef;
		}

		virtual HRESULT STDMETHODCALLTYPE
			QueryInterface(REFIID riid, void** ppv)
		{
			if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
			{
				*ppv = (IWbemObjectSink *)this;
				AddRef();
				return WBEM_S_NO_ERROR;
			}
			else return E_NOINTERFACE;

		}

		virtual HRESULT STDMETHODCALLTYPE Indicate(
			/* [in] */
			LONG lObjectCount,
			/* [size_is][in] */
			IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray
		)
		{
			for (LONG ii = 0; ii < lObjectCount; ++ii)
			{
				vector<CString> names;
				vector<CComVariant> vars;
				WMiHelper::ApiGetAllProps(apObjArray[ii], names, vars);
				continue;
			}
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE SetStatus(
			/* [in] */ LONG lFlags,
			/* [in] */ HRESULT hResult,
			/* [in] */ BSTR strParam,
			/* [in] */ IWbemClassObject __RPC_FAR *pObjParam
		)
		{
			return WBEM_S_NO_ERROR;
		}

		//	virtual HRESULT Wait(ULONG TimeOut) = 0;
		virtual HRESULT Wait(ULONG TimeOut)
		{
			for (LONG ii = 0; ii < TimeOut / 1000; ++ii)
			{
				Sleep(1000);
			}
			return S_OK;
		}
		//};
	};

	CComPtr<CMyEeventHandler> myHandler(new CMyEeventHandler);
	//myHandler->AddRef();
	LPCTSTR ProcessStop = TEXT("Win32_ProcessStopTrace");

	LPVOID sinkObj = nullptr;
	hr = WMiHelper::ApiStartMonitorEvent(spService, ProcessStop, myHandler.p, &sinkObj);


	//////////////////////////////////////////////////////////////////////////
	{
		vector<CString> namesTmp;
		vector<CComVariant> varsTmp;
		hr = WMiHelper::ApiGetAllProps(spService, TEXT("Win32_Process"), namesTmp, varsTmp);
		hr = -1;
	}

	//////////////////////////////////////////////////////////////////////////
	//for (size_t ii = 0; ii < vecInstances.size(); ++ii)
	{
		vector<CString> names;
		vector<CComPtr<IWbemClassObject> > vecParamsIn;
		vector<CComPtr<IWbemClassObject> > vecParamsOut;
		hr = WMiHelper::ApiGetAllMethods(
			spService,//IN IWbemClassObject * pInstance,
			TEXT("Win32_Process"),
			names,//OUT vector<CString> & names,
			vecParamsIn,//OUT vector<CComPtr<IWbemClassObject> > & vecParamsIn,
			vecParamsOut//OUT vector<CComPtr<IWbemClassObject> > & vecParamsOut
		);

		for (size_t jj = 0; jj < vecParamsIn.size(); ++jj)
		{
			vector<CString> namesInTmp;
			vector<CComVariant> varsInTmp;
			if (vecParamsIn[jj])
			{
				hr = WMiHelper::ApiGetAllProps(vecParamsIn[jj], namesInTmp, varsInTmp);
			}

			continue;
		}

		for (size_t jj = 0; jj < vecParamsOut.size(); ++jj)
		{
			vector<CString> namesOutTmp;
			vector<CComVariant> varsOutTmp;
			if (vecParamsOut[jj])
			{
				hr = WMiHelper::ApiGetAllProps(vecParamsOut[jj], namesOutTmp, varsOutTmp);
			}

			continue;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	vector<CString> paramNames(3);
	paramNames[0] = TEXT("CommandLine");
	paramNames[1] = TEXT("CurrentDirectory");
	paramNames[2] = TEXT("ProcessStartupInformation");

	vector<CComVariant> params(paramNames.size());
	params[0] = TEXT("c:\\windows\\system32\\notepad.exe");
	params[1] = TEXT("c:\\windows\\system32\\");


	//////////////////////////////////////////////////////////////////////////
	vector<CString> namesPsi(3);
	namesPsi[0] = TEXT("WinstationDesktop");
	namesPsi[1] = TEXT("ShowWindow");
	namesPsi[2] = TEXT("Title");

	vector<CComVariant> varsPsi(namesPsi.size());
	varsPsi[0] = TEXT("");
	varsPsi[1] = (BYTE)(SW_SHOWMAXIMIZED);
	varsPsi[2] = TEXT("test wmi");

	hr = WMiHelper::ApiNewInstanceVar(spService, TEXT("Win32_ProcessStartup"), namesPsi, varsPsi, params[2]);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ApiNewInstanceVar"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	vector<CString> namesOut;
	vector<CComVariant> varsOut;
	hr = WMiHelper::ApiInvokeMethod(
		spService,//IN IWbemServices * pService,
		NULL,//IN IWbemClassObject * pInstance,	,//null to call class static method, non-null to call object method
		TEXT("Win32_Process"),//IN LPCTSTR className,
		TEXT("Create"),//IN LPCTSTR methodName,
		paramNames,//IN CONST vector<CString> & namesIN, ,//allocated by caller
		params,//IN vector<CComVariant> & varsIN,	,//allocated by caller
		namesOut,//OUT vector<CString> & namesOut,		,//allocated by the function
		varsOut//OUT vector<CComVariant> & varsOut	,//allocated by the function
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
		D_API_ERR(0, TEXT("ApiInvokeMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	COAUTHIDENTITY authIdent;
	CString strUserTmp, strDomainTmp, strPasswordTmp;
	if (argc > 1)
	{
		//CString strDomainTmp, strUserTmp, strPasswordTmp = Password;
		//__SplitDomainUser(strDomainUser, strDomainTmp, strUserTmp);
		strUserTmp = argv[2];
		strDomainTmp = argv[3];
		strPasswordTmp = argv[4];
		WMiHelper::__ComposeAuthId(authIdent, strUserTmp, strDomainTmp, strPasswordTmp);
	}


	vector<CComPtr<IWbemClassObject> > vecInstances;
	hr = WMiHelper::ApiEnumInstances(
		spService,//IN IWbemServices * pService,
		TEXT("Win32_Process"),//IN LPCTSTR className,
		TEXT("CommandLine like '%%notepad.exe%%'"),//IN LPCTSTR whereClause,
		argc > 1 ? &authIdent : NULL,
		vecInstances//OUT vector<CComPtr<IWbemClassObject> > & vecInstances
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
		D_API_ERR(0, TEXT("ApiEnumInstances"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	for (size_t ii = 0; ii < vecInstances.size(); ++ii)
	{
		vector<CString> names;
		vector<CComVariant> vars;
		hr = WMiHelper::ApiGetAllProps(vecInstances[ii], names, vars);
		continue;
	}


	for (size_t ii = 0; ii < vecInstances.size(); ++ii)
	{
		vector<CString> namesIN(1);
		namesIN[0] = TEXT("Reason");

		vector<CComVariant> varsIN(namesIN.size());
		varsIN[0] = (UINT32)(0xcdefcdef);


		vector<CString> namesOut;
		vector<CComVariant> varsOut;
		hr = WMiHelper::ApiInvokeMethod(
			spService,//IN IWbemServices * pService,
			vecInstances[ii],//IN IWbemClassObject * pInstance,	,//null to call class static method, non-null to call object method
			TEXT("Win32_Process"),//IN LPCTSTR className,
			TEXT("Terminate"),//IN LPCTSTR methodName,
			namesIN,//IN CONST vector<CString> & namesIN, ,//allocated by caller
			varsIN,//IN vector<CComVariant> & varsIN,	,//allocated by caller
			namesOut,//OUT vector<CString> & namesOut,		,//allocated by the function
			varsOut//OUT vector<CComVariant> & varsOut	,//allocated by the function
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
			D_API_ERR(0, TEXT("ApiInvokeMethod"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}

	::Sleep(3000);
	WMiHelper::ApiEndMonitorEvent(spService, sinkObj);
	//myHandler.Free();
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	CoUninitialize();
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	D_SETTINGS(999, FALSE, NULL, NULL);

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//product functions
	bool product = false;

	LPCTSTR command = L"-wmiparse";
	D_INFO(0, TEXT("Usage: this.exe %s <wmi namespace> <class1> <class2> <class3>"), command);
	D_INFO(0, TEXT("sample: this.exe %s root\\cimv2 Win32_Process Win32_DiskDrive Win32_NetworkAdapter"), command);
	for (int ii = 0; ii < argc; ++ ii)
	{
		product = (0 == _tcsicmp(command, argv[ii]));
		if (product)
		{
			//SmartLib::counted_ptr<SmartLib::SmlWmi> wmi = SmartLib::counted_ptr<SmartLib::SmlWmi>::make(nullptr, nullptr, nullptr, nullptr, argv[ii + 1]);
			//wmi->Connect();
			SmartLib::SmlWmiParser parser{ argv[ii + 1] };
			for (int jj = ii + 2; jj < argc; ++ jj)
			{
				CString strcode;
				HRESULT hr = parser.Parse(/*argv[ii + 1], */argv[jj], strcode);
				if (SUCCEEDED(hr))
				{
					CString filename;
					const int buffer_size = 1024;
					LPTSTR buffer = filename.GetBuffer(buffer_size);
					::GetModuleFileName(nullptr, buffer, buffer_size);
					LPTSTR ptemp = _tcsrchr(buffer, TEXT('\\'));
					if (ptemp)
					{
						ptemp[1] = 0;
					}
					filename.ReleaseBuffer();
					filename += TEXT("sml_wmi_parser.");
					filename += argv[jj];
					filename += TEXT(".hpp");
					//std::wofstream of(filename);
					//of.imbue(std::locale(of.getloc(), new std::codecvt_utf8_utf16<WCHAR>()));
					//of << strcode.GetString();
					HANDLE hfile = ::CreateFile(
						filename.GetString(),//_In_      LPCTSTR lpFileName,
						GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
						0,//_In_      DWORD dwShareMode,
						nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
						CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
						0,//_In_      DWORD dwFlagsAndAttributes,
						nullptr//,//_In_opt_  HANDLE hTemplateFile
					);

					if (INVALID_HANDLE_VALUE != hfile)
					{
						BYTE bom[] = {0xff, 0xfe};
						DWORD NumberOfBytesWritten = 0;
						::WriteFile(
							hfile,//_In_ HANDLE hFile,
							bom,//_In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,
							sizeof(bom),//_In_ DWORD nNumberOfBytesToWrite,
							&NumberOfBytesWritten,//_Out_opt_ LPDWORD lpNumberOfBytesWritten,
							nullptr//,//_Inout_opt_ LPOVERLAPPED lpOverlapped
						);

						NumberOfBytesWritten = 0;
						::WriteFile(
							hfile,//_In_ HANDLE hFile,
							strcode.GetString(),//_In_reads_bytes_opt_(nNumberOfBytesToWrite) LPCVOID lpBuffer,
							strcode.GetLength() * sizeof(TCHAR),//_In_ DWORD nNumberOfBytesToWrite,
							&NumberOfBytesWritten,//_Out_opt_ LPDWORD lpNumberOfBytesWritten,
							nullptr//,//_Inout_opt_ LPOVERLAPPED lpOverlapped
						);

						::CloseHandle(hfile);
						hfile = INVALID_HANDLE_VALUE;
					}


				}
			}

			break;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	volatile int test_case = 7;
	if (product)
	{
		test_case = -1;
	}

	switch (test_case)
	{
	case 7:
	{
		SML_WMI_FROM_PARSER::SmlWmiParserTest::CaseVSS();
		SML_WMI_FROM_PARSER::SmlWmiParserTest::CaseStorage();
	}
	break;
	case 6:
	{
		SmartLib::MyFormatCodeTest::Case3(argv[1], argv[2]);
	}
	break;
	case 5:
	{
		SML_WMI_FROM_PARSER::SmlWmiParserTest::CaseTestGenerated();
		//SmartLib::SmlWmiParserTest::CaseTestGenerated();
		SmartLib::SmlWmiParserTest::Case0();
		SmartLib::SmlWmiTest::Case0();
	}
	break;
	case 4:
	{
		//SmartLib::SMbContraintTest::Casse1();
		SmartLib::SMbContraintTest::Casse0();
	}
	break;
	case 3:
	{
		SmartLib::TestNFS::CreateShare();
	}
	break;
	case 2:
	{
		_tmain_test_apis(argc, argv);
	}
	break;
	case 1:
	{
		LPCTSTR machine = argc > 1 ? argv[1] : nullptr;
		LPCTSTR user = argc > 2 ? argv[2] : nullptr;
		LPCTSTR domain = argc > 3 ? argv[3] : nullptr;
		LPCTSTR password = argc > 4 ? argv[4] : nullptr;
		My_MSFT_DisksPartitionsVolumes allStorages(machine, user, domain, password);
		allStorages.Retrieve();
		allStorages.ToFile(FIELD_NAME(My_MSFT_DisksPartitionsVolumes), TEXT("My_MSFT_DisksPartitionsVolumes.xml"));

		My_System_Info System_Info(machine, user, domain, password);
		System_Info.Retrieve();
		System_Info.ToFile(FIELD_NAME(My_System_Info), TEXT("My_System_Info.xml"));

	}
	break;

	case 0:
	{
		test_storage_api_enum();
	}
	break;
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);


	CoUninitialize();

	return hr;
}