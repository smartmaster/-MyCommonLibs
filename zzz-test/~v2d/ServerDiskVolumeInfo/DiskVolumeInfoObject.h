// DiskVolumeInfoObject.h : Declaration of the CDiskVolumeInfoObject

#pragma once
#include "resource.h"       // main symbols



#include "ServerDiskVolumeInfo_i.h"



using namespace ATL;


// CDiskVolumeInfoObject

class ATL_NO_VTABLE CDiskVolumeInfoObject :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDiskVolumeInfoObject, &CLSID_DiskVolumeInfoObject>,
	public IDiskVolumeInfoObject
{
public:
	CDiskVolumeInfoObject()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DISKVOLUMEINFOOBJECT)


BEGIN_COM_MAP(CDiskVolumeInfoObject)
	COM_INTERFACE_ENTRY(IDiskVolumeInfoObject)
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



	STDMETHOD(WriteDiskInfo)(IStorage* pIStorage);
	STDMETHOD(WriteVolumeInfo)(IStorage* pIStorage ,  LONG bGetVolumeBitmap);
	STDMETHOD(WriteVdsDiskVolumeInfo)(IStorage* pIStorageDisk ,  IStorage* pIStorageVolume);
	STDMETHOD(WriteSingleDiskInfo)(ULONG ulDiskNumber ,  IStorage* pIStorage , LONG bBreakOnError);
	STDMETHOD(WriteSingleVolumeInfo)(BSTR bstrVolume , LONG bGetVolumeBitmap , IStorage* pIStorage , LONG bBreakOnError);
};

OBJECT_ENTRY_AUTO(__uuidof(DiskVolumeInfoObject), CDiskVolumeInfoObject)
