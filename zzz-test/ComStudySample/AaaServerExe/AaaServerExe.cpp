// AaaServerExe.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "AaaServerExe_i.h"


using namespace ATL;

#include <stdio.h>

class CAaaServerExeModule : public ATL::CAtlServiceModuleT< CAaaServerExeModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_AaaServerExeLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_AAASERVEREXE, "{716CE8E8-86C4-4550-B837-19EAB0BEF173}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropriate Non NULL Security Descriptor.
		CSecurityDescriptor sd;
		sd.InitializeFromThreadToken();

		HRESULT hr = CoInitializeSecurity(
			NULL,//_In_opt_  PSECURITY_DESCRIPTOR pSecDesc,
			-1,//_In_      LONG cAuthSvc,
			NULL,//_In_opt_  SOLE_AUTHENTICATION_SERVICE *asAuthSvc,
			NULL,//_In_opt_  void *pReserved1,
			RPC_C_AUTHN_LEVEL_NONE,//_In_      DWORD dwAuthnLevel,
			RPC_C_IMP_LEVEL_IDENTIFY,//_In_      DWORD dwImpLevel,
			NULL,//_In_opt_  void *pAuthList,
			EOAC_NONE,//_In_      DWORD dwCapabilities,
			NULL//_In_opt_  void *pReserved3
			);

		return S_OK;
	}

	CAaaServerExeModule()
	{
		//_OLD_DINFO((0, DRNPOS, TEXT("%s"), TEXT(__FUNCTION__)));
	}

	virtual ~CAaaServerExeModule()
	{
		//_OLD_DINFO((0, DRNPOS, TEXT("%s"), TEXT(__FUNCTION__)));
	}
};

CAaaServerExeModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

