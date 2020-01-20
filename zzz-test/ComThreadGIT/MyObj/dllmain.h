// dllmain.h : Declaration of module class.

class CMyObjModule : public ATL::CAtlDllModuleT< CMyObjModule >
{
public :
	DECLARE_LIBID(LIBID_MyObjLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MYOBJ, "{D6903FCC-6A8D-4905-B308-89B4FDECCC52}")
};

extern class CMyObjModule _AtlModule;
