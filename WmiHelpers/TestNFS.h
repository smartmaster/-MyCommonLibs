#pragma once

#include "WmiHelper.h"

namespace SmartLib
{
	class TestNFS
	{
	public:
		static HRESULT CreateShare()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CComPtr<IWbemServices> spIWbemServices;
			hr = WMiHelper::ApiConnectService(nullptr, nullptr, nullptr, nullptr, TEXT("Root\\Microsoft\\Windows\\NFS"), &spIWbemServices);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ApiConnectService"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			vector<CString> namesIN
			{
				TEXT("Name"), 
				TEXT("Path"), 
				TEXT("NetworkName"),
				TEXT("Authentication"),
				TEXT("UnmappedUserAccess"),
				TEXT("AnonymousAccess"),
				TEXT("AnonymousUid"),
				TEXT("AnonymousGid"),
				TEXT("LanguageEncoding"),
				TEXT("AllowRootAccess"),
				TEXT("Permission"),
				TEXT("ClientPermission")
			};
			vector<CString> authentication{TEXT("default")};
			CComSafeArray<BSTR>  saAuthentication;
			WMiHelper::__VectorToSafeArray(authentication, saAuthentication);

			vector<CString>  names_MSFT_NfsSharePermission
			{
				TEXT("ClientName"),
				TEXT("Path"),
				TEXT("ClientType"),
				TEXT("Name"),
				TEXT("Permission"),
				TEXT("AllowRootAccess"),
				TEXT("LanguageEncoding")
			};
			vector<CComVariant>  vars_MSFT_NfsSharePermission
			{
				TEXT("All Machines"),//TEXT("ClientName"),
				TEXT("F:\\ssh-key"),//TEXT("Path"),
				TEXT("builtin"),//TEXT("ClientType"),
				TEXT("nfs_share"),//TEXT("Name"),
				TEXT("readwrite"),//TEXT("Permission"),
				true,//TEXT("AllowRootAccess"),
				TEXT("ansi")//TEXT("LanguageEncoding")
			};
			CComVariant  varOut_MSFT_NfsSharePermission;
			WMiHelper::ApiNewInstanceVar(
				spIWbemServices,//IN IWbemServices * pService,
				TEXT("MSFT_NfsSharePermission"),//IN LPCTSTR className,
				names_MSFT_NfsSharePermission,//IN CONST vector<CString> & names,
				vars_MSFT_NfsSharePermission,//IN vector<CComVariant> & vars,
				varOut_MSFT_NfsSharePermission//,//OUT CComVariant & varOut
			);
			vector<LPUNKNOWN> ClientPermission{ varOut_MSFT_NfsSharePermission.punkVal };
			CComSafeArray<LPUNKNOWN>  saClientPermission;
			WMiHelper::__VectorToSafeArray(ClientPermission, saClientPermission);
			CComVariant varClientPermission = saClientPermission;
			CComVariant varAuthentication = saAuthentication;
			vector<CComVariant> varsIN
			{
				TEXT("nfs_share"),			//TEXT("Name"),
				TEXT("F:\\ssh-key"),		//TEXT("Path"),
				TEXT(""),					//TEXT("NetworkName"),
				varAuthentication,			//TEXT("Authentication"),
				true,						//TEXT("UnmappedUserAccess"),
				true,						//TEXT("AnonymousAccess"),
				0,							//TEXT("AnonymousUid"),
				0,							//TEXT("AnonymousGid"),
				TEXT("ansi"),				//TEXT("LanguageEncoding"),
				true,						//TEXT("AllowRootAccess"),
				TEXT("readwrite"),			//TEXT("Permission"),
				varClientPermission			//TEXT("ClientPermission")
			};

			vector<CString> namesOut;
			vector<CComVariant> varsOut;
			hr = WMiHelper::ApiInvokeMethod(
				spIWbemServices,//IN IWbemServices * pService,
				nullptr,//IN IWbemClassObject * pInstance,	,//null to call class static method, non-null to call object method
				TEXT("MSFT_NfsServerTasks"),//IN LPCTSTR className,
				TEXT("CreateShare"),//IN LPCTSTR methodName,
				namesIN,//IN CONST vector<CString> & namesIN, ,//allocated by caller
				varsIN,//IN vector<CComVariant> & varsIN,	,//allocated by caller
				namesOut,//OUT vector<CString> & namesOut,		,//allocated by the function
				varsOut//,//OUT vector<CComVariant> & varsOut	,//allocated by the function
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

			for (CONST auto & item : varsOut)
			{
				vector<CString>  names;
				vector<CComVariant>  vars;
				WMiHelper::ApiGetAllProps(
					(IWbemClassObject *)item.punkVal,//IN IWbemClassObject * pInstance,
					names,//OUT vector<CString> & names,
					vars//,//OUT vector<CComVariant> & vars
				); //names and vars are allocated by the function;
			}
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
			
		}
	};
}