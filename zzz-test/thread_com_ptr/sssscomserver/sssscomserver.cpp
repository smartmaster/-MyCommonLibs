// sssscomserver.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "sssscomserver_i.h"


using namespace ATL;


class CsssscomserverModule : public ATL::CAtlExeModuleT< CsssscomserverModule >
{
public :
	DECLARE_LIBID(LIBID_sssscomserverLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SSSSCOMSERVER, "{CA39B111-048C-42FF-9137-2FAB756153F9}")
	};

CsssscomserverModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

