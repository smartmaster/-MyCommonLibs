// dllmain.h : Declaration of module class.

class CFreeModule : public ATL::CAtlDllModuleT< CFreeModule >
{
public :
	DECLARE_LIBID(LIBID_FreeLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FREE, "{01613859-58BC-4246-B1C5-5A1A72F0C03F}")
};

extern class CFreeModule _AtlModule;
