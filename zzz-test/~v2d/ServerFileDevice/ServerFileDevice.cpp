// ServerFileDevice.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "ServerFileDevice_i.h"


#include <stdio.h>

class CServerFileDeviceModule : public ATL::CAtlServiceModuleT< CServerFileDeviceModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_ServerFileDeviceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SERVERFILEDEVICE, "{A12FC85B-6E39-4DCB-86EC-C0403880D15F}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.

		return S_OK;
	}

	//******************************************
	CServerFileDeviceModule()
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
	virtual ~CServerFileDeviceModule()
	{
		if (m_bComInitialized)
		{
			UninitializeCom();
			m_bComInitialized = FALSE;
		}
	}
};

CServerFileDeviceModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
	LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

