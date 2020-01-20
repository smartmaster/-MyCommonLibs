// dllmain.h : Declaration of module class.

class CAaaServerDllModule : public ATL::CAtlDllModuleT< CAaaServerDllModule >
{
public :
	DECLARE_LIBID(LIBID_AaaServerDllLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_AAASERVERDLL, "{72F2F23F-1053-49B8-B8D1-60854D4847DC}")

	CAaaServerDllModule()
	{
		//_OLD_DINFO((0, DRNPOS, TEXT("%s"), TEXT(__FUNCTION__)));
	}

	virtual ~CAaaServerDllModule()
	{
		//_OLD_DINFO((0, DRNPOS, TEXT("%s"), TEXT(__FUNCTION__)));
	}
};

extern class CAaaServerDllModule _AtlModule;
