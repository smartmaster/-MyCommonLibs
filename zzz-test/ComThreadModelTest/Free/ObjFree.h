// ObjFree.h : Declaration of the CObjFree

#pragma once
#include "resource.h"       // main symbols



#include "Free_i.h"



using namespace ATL;


// CObjFree

class ATL_NO_VTABLE CObjFree :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CObjFree, &CLSID_ObjFree>,
	public IObjFree
{
public:
	CObjFree()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_OBJFREE)


BEGIN_COM_MAP(CObjFree)
	COM_INTERFACE_ENTRY(IObjFree)
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

OBJECT_ENTRY_AUTO(__uuidof(ObjFree), CObjFree)
