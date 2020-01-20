// dllmain.h : Declaration of module class.

class CNeutralModule : public ATL::CAtlDllModuleT< CNeutralModule >
{
public :
	DECLARE_LIBID(LIBID_NeutralLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_NEUTRAL, "{565AE73A-2CC8-42FF-A5F6-35A12057269D}")
};

extern class CNeutralModule _AtlModule;
