#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"

#include "WmiHelper.h"


HRESULT test_storage_api_enum()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//////////////////////////////////////////////////////////////////////////
	//LPCTSTR ClassName = TEXT("MSFT_Disk");
	//LPCTSTR MethodName = TEXT("xxxx");

	static CONST LPCTSTR classNames[] =
	{
		TEXT("MSFT_VirtualDisk"),
		TEXT("msft_disk"),
		TEXT("msft_physicalDisk"),
		TEXT("msft_volume"),
		TEXT("msft_partition"),
		TEXT("msft_partitionToVolume"),
	};

	CString machine = TEXT("sonmi01-vm-w12r2u1");//TEXT("");
	CString user = TEXT("administrator");
	CString domain = machine; // TEXT("sonmi01-hpv12");
	CString password = TEXT("Master16516336");
	CString wmiRes = TEXT("Root\\Microsoft\\Windows\\Storage");
	COAUTHIDENTITY authid = { 0 };
	WMiHelper::__ComposeAuthId(authid, user, domain, password);
	//////////////////////////////////////////////////////////////////////////
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWbemServices> spService;
	hr= WMiHelper::ApiConnectService(
		machine,//IN LPCTSTR Machine,
		user,//IN LPCTSTR User,
		domain,//IN LPCTSTR Domain,
		password,//IN LPCTSTR Password,
		wmiRes,//IN LPCTSTR WmiNamespace,
		&spService//OUT IWbemServices ** ppService
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
		D_API_ERR(0, TEXT("ApiConnectService"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	for (LONG ii = 0; ii < _countof(classNames); ++ ii)
	{
		//////////////////////////////////////////////////////////////////////////
		vector<CComPtr<IWbemClassObject> > vecInstances;
		hr = WMiHelper::ApiEnumInstances(
			spService,//IN IWbemServices * pService,
			classNames[ii],//IN LPCTSTR className,
			NULL,//IN LPCTSTR whereClause,
			machine.GetLength() ? &authid : NULL,//IN COAUTHIDENTITY * AuthId,
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
			vector<CString>  names;
			vector<CComVariant>  vars;

			hr = WMiHelper::ApiGetAllProps(
				vecInstances[ii],//IN IWbemClassObject * pInstance,
				names,//OUT vector<CString> & names,
				vars//OUT vector<CComVariant> & vars
				);

			continue; //no effect 
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}