// dllmain.h : Declaration of module class.

class CApartmentModule : public ATL::CAtlDllModuleT< CApartmentModule >
{
public :
	DECLARE_LIBID(LIBID_ApartmentLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_APARTMENT, "{E4DAAF42-CB43-4CC2-84CA-50311A1274B4}")
};

extern class CApartmentModule _AtlModule;
