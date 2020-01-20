// SimpleObj.h : Declaration of the CSimpleObj

#pragma once
#include "resource.h"       // main symbols

#include <atlstr.h>

#include "MyObj_i.h"

using namespace ATL;

// CSimpleObj

class ATL_NO_VTABLE CSimpleObj :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSimpleObj, &CLSID_SimpleObj>,
	public ISimpleObj
{
public:
	CSimpleObj() :
		m_ProtectedData(0)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SIMPLEOBJ)


BEGIN_COM_MAP(CSimpleObj)
	COM_INTERFACE_ENTRY(ISimpleObj)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

private:
	ULONG m_ProtectedData;
	CString m_ProtectedStr;

public:



	STDMETHOD(Method)();
};

OBJECT_ENTRY_AUTO(__uuidof(SimpleObj), CSimpleObj)
