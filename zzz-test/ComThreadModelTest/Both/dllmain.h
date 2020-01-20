// dllmain.h : Declaration of module class.

class CBothModule : public ATL::CAtlDllModuleT< CBothModule >
{
public :
	DECLARE_LIBID(LIBID_BothLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BOTH, "{A8247D1A-4B61-42C0-BF06-B1D82AA84890}")
};

extern class CBothModule _AtlModule;
