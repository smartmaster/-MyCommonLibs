#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atlsafe.h>


#include <Wbemidl.h>


#include <vector>
#include <map>
using namespace std;

//////////////////////////////////////////////////////////////////////////
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"


/*

\b(\w+?)(\s+?)(\w+?);(.*?)


\b(CString)(\s+?)(\w+?);(.*?)
WMI_GET_PROP_STRING($3) //


\b(.*?INT\d*?|BOOL)(\s+?)(\w+?);(.*?)
WMI_GET_PROP_INT($3) //


\b(SYSTEMTIME)(\s+?)(\w+?);(.*?)
WMI_GET_PROP_SYSTEMTIME($3) //


\b(.*?vector<.+?>)(\s+?)(\w+?);(.*?)
WMI_GET_PROP_ARRAY(xxx, xxx, $3)  //


//////////////////////////////////////////////////////////////////////////
\b(\w+?)(\s+?)(\w+?);(.*?)


\b(CString)(\s+?)(\w+?);(.*?)
WMI_GET_PROP_STRING($3) //


\b(.*?INT\d*?|BOOL)(\s+?)(\w+?);(.*?)
WMI_GET_PROP_INT($3) //


\b(SYSTEMTIME)(\s+?)(\w+?);(.*?)
WMI_GET_PROP_SYSTEMTIME($3) //


\b(.*?vector<.+?>)(\s+?)(\w+?);(.*?)
WMI_GET_PROP_ARRAY(xxx, xxx, $3)  //

*/

#define  WMI_GET_PROP_STRING(field)  \
else if (0 == names[ii].CompareNoCase(FIELD_NAME(field))) \
{ \
	field = vars[ii].bstrVal; \
}


#define  WMI_GET_PROP_INT(field)  \
else if (0 == names[ii].CompareNoCase(FIELD_NAME(field))) \
{ \
	field = vars[ii].llVal; \
}


#define  WMI_GET_PROP_SYSTEMTIME(field)  \
else if (0 == names[ii].CompareNoCase(FIELD_NAME(field))) \
{ \
	VariantTimeToSystemTime(vars[ii].date, &(field)); \
}


#define  WMI_GET_PROP_ARRAY(tcpp, tcom, field)  \
else if (0 == names[ii].CompareNoCase(FIELD_NAME(field))) \
{ \
	if(nullptr != vars[ii].parray) \
	{ \
		CComSafeArray<tcom> sa(vars[ii].parray); \
		WMiHelper::__SafeArrayToVector<tcpp, tcom>(sa, field); \
	} \
}

//////////////////////////////////////////////////////////////////////////
struct WMiHelper
{
public:
	////API start//////////////////////////////////////////////////////////////////////
	// WmiNamespace - relative path of WMI name space, example root\vimv2, cannot be \root\cimv2 (with preceding slash) or \\machine\root\cimv2
	static HRESULT ApiConnectService(
		IN LPCTSTR Machine,
		IN LPCTSTR User,
		IN LPCTSTR Domain,
		IN LPCTSTR Password,
		IN LPCTSTR WmiNamespace,
		OUT IWbemServices ** ppService
		);

	static HRESULT ApiEnumInstances(
		IN IWbemServices * pService,
		IN LPCTSTR className,
		IN LPCTSTR whereClause,
		IN COAUTHIDENTITY * pAuthId,
		OUT vector<CComPtr<IWbemClassObject> > & vecInstances
		);

	static HRESULT ApiEnumAssociators(
		IN IWbemServices* pService,
		IN LPCTSTR parentClass,
		IN LPCTSTR propName,
		IN LPCTSTR propValue,
		IN LPCTSTR AssocClass,
		IN COAUTHIDENTITY* pAuthId,
		OUT vector<CComPtr<IWbemClassObject> >& vecInstances
	)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		//////////////////////////////////////////////////////////////////////////
		BEGIN_BLOCK(0);
		CString strWql;
		if(propName && propName[0] && propValue && propValue[0])
		{
			strWql.Format(TEXT("Associators of {%s.%s=%s} where AssocClass=%s"), parentClass, propName, propValue, AssocClass);
		}
		else
		{
			strWql.Format(TEXT("Associators of {%s} where AssocClass=%s"), parentClass, AssocClass);
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

	static HRESULT ApiGetAllProps(
		IN IWbemClassObject * pInstance,
		OUT vector<CString> & names,
		OUT vector<CComVariant> & vars
		); //names and vars are allocated by the function;

	static HRESULT ApiGetAllProps(
		IN IWbemClassObject* pInstance,
		OUT map<CString, CComVariant>& namevars
	) 
	{
		vector<CString> names;
		vector<CComVariant> vars;
		HRESULT hr = ApiGetAllProps(
			pInstance,//IN IWbemClassObject * pInstance,
			names,//OUT vector<CString> & names,
			vars//,//OUT vector<CComVariant> & vars
		);
		if (SUCCEEDED(hr))
		{
			for (int ii = 0; ii < names.size(); ii++)
			{
				namevars.insert({names[ii], vars[ii]});
			}
		}

		return hr;
	}

	static HRESULT ApiGetAllProps(
		IN IWbemServices * pService,
		IN LPCTSTR className,
		OUT vector<CString> & names,
		OUT vector<CComVariant> & vars //names and vars are allocated by the function;
		);

	static HRESULT ApiGetAllProps(
		IN IWbemServices* pService,
		IN LPCTSTR className,
		OUT map<CString, CComVariant>& namevars
	)
	{
		vector<CString> names;
		vector<CComVariant> vars;
		HRESULT hr = ApiGetAllProps(
			pService,//IN IWbemServices* pService,
			className,//IN LPCTSTR className,
			names,//OUT vector<CString>& names,
			vars//,//OUT vector<CComVariant>& vars ,//names and vars are allocated by the function;
		);
		if (SUCCEEDED(hr))
		{
			for (int ii = 0; ii < names.size(); ii++)
			{
				namevars.insert({ names[ii], vars[ii] });
			}
		}

		return hr;
	}

	static HRESULT ApiGetAllMethods(
		IN IWbemServices * pService,
		IN LPCTSTR className,
		OUT vector<CString> & names,
		OUT vector<CComPtr<IWbemClassObject> > & vecParamsIn,
		OUT vector<CComPtr<IWbemClassObject> > & vecParamsOut
		); //names and vars are allocated by the function;
	

	struct MethodInfo
	{
		CString _name;
		CComPtr<IWbemClassObject> _input;
		CComPtr<IWbemClassObject> _output;

		MethodInfo(LPCTSTR name, IWbemClassObject* input, IWbemClassObject* output):
			_name(name),
			_input(input),
			_output(output)
		{
		}
	};

	static HRESULT ApiGetAllMethods(
		IN IWbemServices* pService,
		IN LPCTSTR className,
		vector<MethodInfo>& methods
	)
	{
		vector<CString> names;
		vector<CComPtr<IWbemClassObject> > vecParamsIn;
		vector<CComPtr<IWbemClassObject> > vecParamsOut;
		HRESULT hr = ApiGetAllMethods(
			pService,//IN IWbemServices * pService,
			className,//IN LPCTSTR className,
			names,//OUT vector<CString> & names,
			vecParamsIn,//OUT vector<CComPtr<IWbemClassObject> > & vecParamsIn,
			vecParamsOut//,//OUT vector<CComPtr<IWbemClassObject> > & vecParamsOut
		); //names and vars are allocated by the function;
		if (SUCCEEDED(hr))
		{
			for (int ii = 0; ii < names.size(); ii++)
			{
				methods.push_back(MethodInfo(names[ii].GetString(), vecParamsIn[ii], vecParamsOut[ii]));
			}
		}
		return hr;
	}


	static HRESULT ApiNewInstanceVar(
		IN IWbemServices * pService,
		IN LPCTSTR className,
		IN CONST vector<CString> & names,
		IN vector<CComVariant> & vars,
		OUT CComVariant & varOut
		);

	static HRESULT ApiNewInstanceVar(
		IN IWbemServices* pService,
		IN LPCTSTR className,
		const map<CString, CComVariant> & namevars,
		OUT CComVariant& varOut
	)
	{
		vector<CString> names;
		vector<CComVariant> vars;
		for (const auto kv : namevars)
		{
			names.push_back(kv.first);
			vars.push_back(kv.second);
		}
		return ApiNewInstanceVar(
			pService,//IN IWbemServices * pService,
			className,//IN LPCTSTR className,
			names,//IN CONST vector<CString> & names,
			vars,//IN vector<CComVariant> & vars,
			varOut//,//OUT CComVariant & varOut
		);
	}

	static HRESULT ApiInvokeMethod(
		IN IWbemServices * pService,
		IN IWbemClassObject * pInstance,	//null to call class static method, non-null to call object method
		IN LPCTSTR className,
		IN LPCTSTR methodName,
		IN CONST vector<CString> & namesIN, //allocated by caller
		IN vector<CComVariant> & varsIN,	//allocated by caller
		OUT vector<CString> & namesOut,		//allocated by the function
		OUT vector<CComVariant> & varsOut	//allocated by the function
		);


	static HRESULT ApiInvokeMethod(
		IN IWbemServices* pService,
		IN IWbemClassObject* pInstance,	//null to call class static method, non-null to call object method
		IN LPCTSTR className,
		IN LPCTSTR methodName,
		const map<CString, CComVariant>& inparams, 
		map<CString, CComVariant>& outs
	)
	{
		vector<CString> namesIN;
		vector<CComVariant> varsIN;
		for (const auto kv : inparams)
		{
			namesIN.push_back(kv.first);
			varsIN.push_back(kv.second);
		}

		vector<CString> namesOut;
		vector<CComVariant> varsOut;
		HRESULT hr = ApiInvokeMethod(
			pService,//IN IWbemServices * pService,
			pInstance,//IN IWbemClassObject * pInstance,	,//null to call class static method, non-null to call object method
			className,//IN LPCTSTR className,
			methodName,//IN LPCTSTR methodName,
			namesIN,//IN CONST vector<CString> & namesIN, ,//allocated by caller
			varsIN,//IN vector<CComVariant> & varsIN,	,//allocated by caller
			namesOut,//OUT vector<CString> & namesOut,		,//allocated by the function
			varsOut//,//OUT vector<CComVariant> & varsOut	,//allocated by the function
		);
		if (SUCCEEDED(hr))
		{
			for (int ii = 0; ii < namesOut.size(); ii++)
			{
				outs.insert({ namesOut[ii], varsOut[ii] });
			}
		}

		return hr;
	}

	//////////////////////////////////////////////////////////////////////////
	struct IMyEeventHandler
	{
		virtual HRESULT STDMETHODCALLTYPE Indicate(
			/* [in] */
			LONG lObjectCount,
			/* [size_is][in] */
			IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray
		) = 0;
		virtual HRESULT Wait(ULONG TimeOut) = 0; // can wait for a certain fixed timeout or Indicate() set a signal on an interesting WMI event and then wait() return
	};

	static HRESULT ApiMonitorEvent(
		IN IWbemServices * pService,
		IN LPCTSTR className,
		IN IMyEeventHandler * pHandler,
		IN ULONG TimeOut
		);


	static HRESULT ApiStartMonitorEvent(
		IN IWbemServices * pService,
		IN LPCTSTR className,
		IN IMyEeventHandler * pHandler,
		OUT VOID ** ppObjectSink
	);

	static HRESULT ApiEndMonitorEvent(
		IN IWbemServices * pService,
		IN VOID * pObjectSink
	);

	////API end//////////////////////////////////////////////////////////////////////

public:
	//////////////////////////////////////////////////////////////////////////
	static VOID __CombineDomainUser(CONST CString & Domain, CONST CString & User, CString & strDomainUser);
	static VOID __SplitDomainUser(CONST CString & strDomainUser, CString & Domain, CString & User);
	static VOID __ComposeAuthId(COAUTHIDENTITY & authIdent, CString & strUserTmp, CString & strDomainTmp, CString & strPasswordTmp);

	template<typename TCPP, typename TCOM>
	static HRESULT __VectorToSafeArray(CONST vector<TCPP> & vec, CComSafeArray<TCOM> & sa)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;
		
		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		hr = sa.Create(vec.size());
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Create"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		for (size_t ii = 0; ii < vec.size(); ++ ii)
		{
			sa.SetAt(ii, vec[ii], TRUE);
		}
		
		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);
		
		return hr;
	}
	
	template<>
	static HRESULT __VectorToSafeArray<CString, BSTR>(CONST vector<CString> & vec, CComSafeArray<BSTR> & sa)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		hr = sa.Create(vec.size());
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Create"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		for (size_t ii = 0; ii < vec.size(); ++ii)
		{
			sa.SetAt(ii, vec[ii].AllocSysString(), FALSE);
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}
	
	template<typename TCPP, typename TCOM = TCPP>
	static HRESULT __SafeArrayToVector(CONST CComSafeArray<TCOM> & sa, vector<TCPP> & vec)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		vec.resize(sa.GetCount());

		for (size_t ii = 0; ii < vec.size(); ++ii)
		{
			vec[ii] = sa.GetAt(ii);
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	template<typename TCPP, typename TCOM>
	static HRESULT __VectorToVar(CONST vector<TCPP>& vec, CComVariant& var)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		CComSafeArray<TCOM> sa;
		hr = __VectorToSafeArray<TCPP, TCOM>(vec, sa);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("__VectorToSafeArray"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		var = sa;

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	template<typename TCPP, typename TCOM>
	static HRESULT __VarToVector(CComVariant & var, vector<TCPP> & vec)
	{
		HRESULT hr = S_OK;
		DWORD LastError = 0;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		CComSafeArray<TCOM> sa;
		sa = var.parray;

		hr = __SafeArrayToVector<TCPP, TCOM>(sa, vec);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("__SafeArrayToVector"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		return hr;
	}

	
private:
	//////////////////////////////////////////////////////////////////////////
	static HRESULT GetClass(
		IWbemServices * pService, 
		LPCTSTR ClassName, 
		IWbemClassObject ** ppClass
		);
	static HRESULT NewInstance(
		IWbemServices * pService, 
		LPCTSTR ClassName, 
		IWbemClassObject ** ppInstance
		);

	//////////////////////////////////////////////////////////////////////////
	static HRESULT GetMethodIn(
		IWbemServices * pService, 
		LPCTSTR ClassName, 
		LPCTSTR MethodName, 
		IWbemClassObject ** ppMethodInParam
		);

	static HRESULT ExecMethod(
		IWbemServices * pService, 
		LPCTSTR ClassOrPathname, 
		LPCTSTR MethodName, 
		IWbemClassObject * pMethodInParam, 
		IWbemClassObject ** ppMethodOutParam
		);

	static HRESULT ExecMethod(
		IWbemServices * pService, 
		IWbemClassObject * pInstance, 
		LPCTSTR MethodName, 
		IWbemClassObject * pMethodInParam, 
		IWbemClassObject ** ppMethodOutParam
		);

	//////////////////////////////////////////////////////////////////////////
	static HRESULT Put(
		IWbemClassObject * pInstance, 
		LPCTSTR name, 
		VARIANT * pvar
		);
	static HRESULT Put(
		IWbemClassObject * pInstance, 
		CONST vector<CString> & names, 
		vector<CComVariant> & vars
		); // names and vars are pre-allocated by caller

	//////////////////////////////////////////////////////////////////////////
	static HRESULT Get(
		IWbemClassObject * pInstance, 
		LPCTSTR name, 
		VARIANT * pvar
		);
	static HRESULT Get(
		IWbemClassObject * pInstance, 
		CONST vector<CString> & names, 
		vector<CComVariant> & vars
		); // names and vars are pre-allocated by caller

	static HRESULT GetPath(
		IWbemClassObject * pInstance, 
		CString & strObjPath
		);

	static HRESULT GetAllMethods(
		IN IWbemClassObject * pInstance,
		OUT vector<CString> & names,
		OUT vector<CComPtr<IWbemClassObject> > & vecParamsIn,
		OUT vector<CComPtr<IWbemClassObject> > & vecParamsOut
		); //names and vars are allocated by the function;


	//////////////////////////////////////////////////////////////////////////
	static HRESULT RegisterEvent(
		IWbemServices * pSvc,
		IWbemObjectSink * pMySink,
		LPCTSTR strQuery,
		IWbemObjectSink ** ppSinkStub
		);

private:
	//////////////////////////////////////////////////////////////////////////
	static HRESULT ClassGetInstance(
		IWbemClassObject * pClass, 
		LPCTSTR ClassName, 
		IWbemClassObject ** ppInstance
		);


private:
	//////////////////////////////////////////////////////////////////////////
	class CMyEventSink : public IWbemObjectSink
	{
		LONG m_lRef;			/***INCOMMING***/
		BOOL m_bDone;			/***INCOMMING***/
		IMyEeventHandler * m_handler;	/***INCOMMING***/

	public:
		CMyEventSink(IMyEeventHandler * pHandler);

		~CMyEventSink();

		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);

		virtual HRESULT STDMETHODCALLTYPE Indicate(
			LONG lObjectCount,
			IWbemClassObject __RPC_FAR *__RPC_FAR *apObjArray
			);
		virtual HRESULT STDMETHODCALLTYPE SetStatus(
			/* [in] */ LONG lFlags,
			/* [in] */ HRESULT hResult,
			/* [in] */ BSTR strParam,
			/* [in] */ IWbemClassObject __RPC_FAR *pObjParam
			);
	};

};