// dllmain.h : Declaration of module class.

class CSingleModule : public ATL::CAtlDllModuleT< CSingleModule >
{
public :
	DECLARE_LIBID(LIBID_SingleLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SINGLE, "{46069B98-1F0C-4CC2-9943-23732F86898D}")
};

extern class CSingleModule _AtlModule;
