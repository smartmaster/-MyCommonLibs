// ObjNeutral.h : Declaration of the CObjNeutral

#pragma once
#include "resource.h"       // main symbols



#include "Neutral_i.h"


#ifdef _WIN32_WCE
#error "Neutral-threaded COM objects are not supported on Windows CE."
#endif

using namespace ATL;


// CObjNeutral

class ATL_NO_VTABLE CObjNeutral :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CObjNeutral, &CLSID_ObjNeutral>,
	public IObjNeutral
{
public:
	CObjNeutral()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_OBJNEUTRAL)


BEGIN_COM_MAP(CObjNeutral)
	COM_INTERFACE_ENTRY(IObjNeutral)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:



	STDMETHOD(Print)();
};

OBJECT_ENTRY_AUTO(__uuidof(ObjNeutral), CObjNeutral)
