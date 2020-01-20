// MyObj.h : Declaration of the CMyObj

#pragma once
#include "resource.h"       // main symbols



#include "aaaioserver_i.h"



using namespace ATL;


// CMyObj

class ATL_NO_VTABLE CMyObj :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMyObj, &CLSID_MyObj>,
	public IMyObj
{
public:
	CMyObj()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYOBJ)


BEGIN_COM_MAP(CMyObj)
	COM_INTERFACE_ENTRY(IMyObj)
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
	

	virtual HRESULT STDMETHODCALLTYPE IO(
		/* [in] */ MyParams *input,
		/* [out] */ MyParams *output);
};

OBJECT_ENTRY_AUTO(__uuidof(MyObj), CMyObj)
