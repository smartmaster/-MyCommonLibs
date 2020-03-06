// aaa-io-server.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "aaaioserver_i.h"


using namespace ATL;

#include <stdio.h>

class CaaaioserverModule : public ATL::CAtlServiceModuleT< CaaaioserverModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_aaaioserverLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_AAAIOSERVER, "{EF114D04-6C44-441C-8C27-4B9B72D31578}")
		HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropriate Non NULL Security Descriptor.

		return S_OK;
	}
	};

CaaaioserverModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

