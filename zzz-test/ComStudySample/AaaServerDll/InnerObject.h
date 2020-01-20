// InnerObject.h : Declaration of the CInnerObject

#pragma once
#include "resource.h"       // main symbols



#include "AaaServerDll_i.h"


#define __DEBUG_BREAK 0


using namespace ATL;


// CInnerObject

class ATL_NO_VTABLE CInnerObject :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CInnerObject, &CLSID_InnerObject>,
	public IInnerObject
{
public:
	CInnerObject()
	{
		//D_START(0);
#if __DEBUG_BREAK
		DebugBreak();
#endif
	}

	virtual ~CInnerObject()
	{
		//D_START(0);
#if __DEBUG_BREAK
		DebugBreak();
#endif
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INNEROBJECT)


BEGIN_COM_MAP(CInnerObject)
	COM_INTERFACE_ENTRY(IInnerObject)
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



	STDMETHOD(OutDynamicArray)(LONG* pLength, WCHAR ** ppBuffer);
};

OBJECT_ENTRY_AUTO(__uuidof(InnerObject), CInnerObject)
