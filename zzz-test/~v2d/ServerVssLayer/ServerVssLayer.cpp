// ServerVssLayer.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "ServerVssLayer_i.h"


#include <stdio.h>

class CServerVssLayerModule : public ATL::CAtlServiceModuleT< CServerVssLayerModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_ServerVssLayerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SERVERVSSLAYER, "{7697431C-E530-47FE-B67B-CA5871E549FF}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.

		return S_OK;
	}

	//******************************************
	CServerVssLayerModule()
	{
		HRESULT hr = InitializeCom();
		if (FAILED(hr))
		{
			// Ignore RPC_E_CHANGED_MODE if CLR is loaded. Error is due to CLR initializing
			// COM and InitializeCOM trying to initialize COM with different flags.
			if (hr != RPC_E_CHANGED_MODE || GetModuleHandle(_T("Mscoree.dll")) == NULL)
			{
				ATLASSERT(0);
				ATL::CAtlBaseModule::m_bInitFailed = true;
				return;
			}
		}
		else
		{
			m_bComInitialized = true;
		}
	}

	//******************************************
	virtual ~CServerVssLayerModule()
	{
		if (m_bComInitialized)
		{
			UninitializeCom();
			m_bComInitialized = FALSE;
		}
	}
};

CServerVssLayerModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
	LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	HRESULT hr = CoInitializeSecurity(
		NULL,//PSECURITY_DESCRIPTOR pVoid,
		-1,//LONG cAuthSvc,
		NULL,//SOLE_AUTHENTICATION_SERVICE * asAuthSvc, 
		NULL,//void * pReserved1,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,//DWORD dwAuthnLevel,
		RPC_C_IMP_LEVEL_IMPERSONATE,//DWORD dwImpLevel,
		NULL,//SOLE_AUTHENTICATION_LIST * pAuthList,
		EOAC_NONE,//DWORD dwCapabilities,
		NULL//void * pReserved3
		);
	if (FAILED(hr))
	{	
		_OLD_DSETLASTSTATUS(hr);
		//nStatus = hr;
		_OLD_DAPIERR((0, DRNPOS, TEXT("CoInitializeSecurity"), hr));
		//break;
	}
	return _AtlModule.WinMain(nShowCmd);
}

