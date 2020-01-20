// VssLayerObject.h : Declaration of the CVssLayerObject

#pragma once
#include "resource.h"       // main symbols



#include "ServerVssLayer_i.h"

#include <vss.h>
#include <vswriter.h>
#include <vsbackup.h>
#include <vsmgmt.h>


using namespace ATL;


// CVssLayerObject

class ATL_NO_VTABLE CVssLayerObject :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CVssLayerObject, &CLSID_VssLayerObject>,
	public IVssLayerObject
{
public:
	CVssLayerObject()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_VSSLAYEROBJECT)


BEGIN_COM_MAP(CVssLayerObject)
	COM_INTERFACE_ENTRY(IVssLayerObject)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		CleanupIVssBackupComponents();
	}

private: //******************************************
	VOID CleanupIVssBackupComponents();
	CComGITPtr<IVssBackupComponents> m_spGITPtrIVssBackupComponents;

public:
	STDMETHOD(ShadowCopy)(SAFEARRAY *  psaArgvs, IStorage * pISorageXmls , IStorage * pISorageVssResults);
	STDMETHOD(VssBackupComponentsRelease)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(VssLayerObject), CVssLayerObject)
