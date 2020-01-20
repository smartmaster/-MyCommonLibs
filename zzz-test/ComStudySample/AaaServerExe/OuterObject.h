// OuterObject.h : Declaration of the COuterObject

#pragma once
#include "resource.h"       // main symbols



#include "AaaServerExe_i.h"




using namespace ATL;


#define __DEBUG_BREAK 0

// COuterObject

#include "..\AaaServerDll\AaaServerDll_i.h"

class ATL_NO_VTABLE COuterObject :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<COuterObject, &CLSID_OuterObject>,
	public IOuterObject
{
public:
	COuterObject()
	{
		//D_START(0);
#if __DEBUG_BREAK
		DebugBreak();
#endif
	}

	virtual ~COuterObject()
	{
		//D_START(0);
#if __DEBUG_BREAK
		DebugBreak();
#endif
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_OUTEROBJECT)


	BEGIN_COM_MAP(COuterObject)
		COM_INTERFACE_ENTRY(IOuterObject)
		COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IInnerObject), m_pIUnknownIInnerObject) //###AGG 500
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	DECLARE_GET_CONTROLLING_UNKNOWN() //###AGG 200

	HRESULT FinalConstruct()
	{
		HRESULT hr = S_OK;
		BEGIN_BLOCK(0); //###AGG 300
		hr = CreateAggregatedIUnknown(__uuidof(InnerObject), &m_pIUnknownIInnerObject);
		if (FAILED(hr))
		{
			//_OLD_DSETLASTSTATUS(hr);
			//_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateInstance"), hr));
			break;
		}
		END_BLOCK(0);
		return hr;
	}

	void FinalRelease()
	{
		if (m_pIUnknownIInnerObject) //###AGG 400
		{
			CComPtr<IUnknown> spTemp;
			spTemp.Attach(m_pIUnknownIInnerObject);
			m_pIUnknownIInnerObject = NULL;
		}
	}

public:
	STDMETHOD(OutDynamicArray)(LONG* pLength, WCHAR ** ppBuffer);

private:
	IUnknown * m_pIUnknownIInnerObject{ nullptr }; //###AGG 100


private:
	HRESULT CreateAggregatedIUnknown(REFCLSID clsId, IUnknown ** ppobj)
	{
		CComPtr<IUnknown> spIUnknown;
		HRESULT hr = spIUnknown.CoCreateInstance(clsId, GetControllingUnknown(), CLSCTX_INPROC);
		if (FAILED(hr))
		{
			//_OLD_DSETLASTSTATUS(hr);
			//_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateInstance"), hr));
			//break;
		}
		else
		{
			*ppobj = spIUnknown.Detach();
		}
		return hr;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(OuterObject), COuterObject)
