#include "stdafx.h"
#include <InitGuid.h>
#include "VDSVolumeDiskHelper.h"
#include "VolumeHelper.h"
#include "MiscHelper.h"


VOID MY_VDS_VOLUME_PROP::Copy( CONST VDS_VOLUME_PROP & VDSVolumeProp )
{
	m_id = VDSVolumeProp.id;  
	m_type = VDSVolumeProp.type;  
	m_status = VDSVolumeProp.status;  
	m_health = VDSVolumeProp.health;  
	m_TransitionState = VDSVolumeProp.TransitionState;  
	m_ullSize = VDSVolumeProp.ullSize;  
	m_ulFlags = VDSVolumeProp.ulFlags;  
	m_RecommendedFileSystemType = VDSVolumeProp.RecommendedFileSystemType;  
	m_strName = PTR_TO_STRING(VDSVolumeProp.pwszName);
}

VOID MY_VDS_VOLUME_PROP::VDS_VOLUME_PROP_Free( VDS_VOLUME_PROP & VDSVolumeProp )
{
	{
		CComHeapPtr<WCHAR> apBufferCleanUp(VDSVolumeProp.pwszName);
		VDSVolumeProp.pwszName = NULL;
	}
}

VOID MY_VDS_DISK_PROP::Copy( VDS_DISK_PROP & VDSDiskProp )
{
	m_id = VDSDiskProp.id;  
	m_status = VDSDiskProp.status;   
	m_ReserveMode = VDSDiskProp.ReserveMode;   
	m_health = VDSDiskProp.health;   
	m_dwDeviceType = VDSDiskProp.dwDeviceType;   
	m_dwMediaType = VDSDiskProp.dwMediaType;   
	m_ullSize = VDSDiskProp.ullSize;   
	m_ulBytesPerSector = VDSDiskProp.ulBytesPerSector;   
	m_ulSectorsPerTrack = VDSDiskProp.ulSectorsPerTrack;   
	m_ulTracksPerCylinder = VDSDiskProp.ulTracksPerCylinder;   
	m_ulFlags = VDSDiskProp.ulFlags;   
	m_BusType = VDSDiskProp.BusType;   
	m_PartitionStyle = VDSDiskProp.PartitionStyle;   
	m_DiskGuid = VDSDiskProp.DiskGuid;   
	m_strDiskAddress = PTR_TO_STRING(VDSDiskProp.pwszDiskAddress);   
	m_strName = PTR_TO_STRING(VDSDiskProp.pwszName);   
	m_strFriendlyName = PTR_TO_STRING(VDSDiskProp.pwszFriendlyName);   
	m_strAdaptorName = PTR_TO_STRING(VDSDiskProp.pwszAdaptorName);   
	m_strDevicePath = PTR_TO_STRING(VDSDiskProp.pwszDevicePath);
}

VOID MY_VDS_DISK_PROP::VDS_DISK_PROP_Free( VDS_DISK_PROP & VDSDiskProp )
{
	{
		CComHeapPtr<WCHAR> apBufferCleanUp(VDSDiskProp.pwszDiskAddress);
		VDSDiskProp.pwszDiskAddress = NULL;
	}
	{
		CComHeapPtr<WCHAR> apBufferCleanUp(VDSDiskProp.pwszName);
		VDSDiskProp.pwszName = NULL;
	}
	{
		CComHeapPtr<WCHAR> apBufferCleanUp(VDSDiskProp.pwszFriendlyName);
		VDSDiskProp.pwszFriendlyName = NULL;
	}
	{
		CComHeapPtr<WCHAR> apBufferCleanUp(VDSDiskProp.pwszAdaptorName);
		VDSDiskProp.pwszAdaptorName = NULL;
	}
	{
		CComHeapPtr<WCHAR> apBufferCleanUp(VDSDiskProp.pwszDevicePath);
		VDSDiskProp.pwszDevicePath = NULL;
	}
}

INT CVDSService::Load()
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComPtr<IVdsServiceLoader> spIVdsServiceLoader;
		hr = CoCreateInstance(
			CLSID_VdsLoader,
			NULL,
			CLSCTX_LOCAL_SERVER,
			__uuidof(IVdsServiceLoader),
			(VOID**)&spIVdsServiceLoader
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateInstance"), hr));
			break;
		}

		//******************************************
		hr = spIVdsServiceLoader->LoadService(NULL, &m_pIVdsService);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("LoadService"), hr));
			break;
		}

		//******************************************
		hr = m_pIVdsService->WaitForServiceReady();
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("WaitForServiceReady"), hr));
			break;
		}

		//******************************************
		VDS_SERVICE_PROP VDSServiceProp = {0};
		hr = m_pIVdsService->GetProperties(&VDSServiceProp);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("GetProperties"), hr));
			break;
		}
		m_MyVDSServicEProp.Copy(VDSServiceProp);
		MY_VDS_SERVICE_PROP::VDS_SERVICE_PROP_Free(VDSServiceProp);


		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CVDSService::QuerySoftwareProvides( vector<IVdsSwProvider*> & swProvides )
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComPtr<IEnumVdsObject> spIEnumVdsObject;
		hr = m_pIVdsService->QueryProviders(
			VDS_QUERY_SOFTWARE_PROVIDERS,//[in]   DWORD masks,
			&spIEnumVdsObject//[out]  IEnumVdsObject** ppEnum
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("QueryProviders"), hr));
			break;
		}

		//******************************************
		for (;;)
		{
			CComPtr<IUnknown> spIUnknown;
			ULONG cFetched = 0;
			hr = spIEnumVdsObject->Next(
				1,//[in]   ULONG celt,
				&spIUnknown,//[out]  IUnknown** ppObjectArray,
				&cFetched//[out]  ULONG* pcFetched
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Next"), hr));
				break;
			}
			else if (S_FALSE == hr || cFetched != 1)
			{
				break;
			}
			else
			{
				CComPtr<IVdsSwProvider> spIVdsSwProvider;
				hr = spIUnknown.QueryInterface(&spIVdsSwProvider);
				if (FAILED(hr))
				{	
					_OLD_DSETLASTSTATUS(hr);
					nStatus = hr;
					_OLD_DAPIERR((0, DRNPOS, TEXT("QueryInterface(&spIVdsSwProvider)"), hr));
					break;
				}
				swProvides.push_back(spIVdsSwProvider.Detach());
			}
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CVDSService::QueryHardwareProvides( vector<IVdsHwProvider*> & hwProvides )
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComPtr<IEnumVdsObject> spIEnumVdsObject;
		hr = m_pIVdsService->QueryProviders(
			VDS_QUERY_HARDWARE_PROVIDERS,//[in]   DWORD masks,
			&spIEnumVdsObject//[out]  IEnumVdsObject** ppEnum
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("QueryProviders"), hr));
			break;
		}

		//******************************************
		for (;;)
		{
			CComPtr<IUnknown> spIUnknown;
			ULONG cFetched = 0;
			hr = spIEnumVdsObject->Next(
				1,//[in]   ULONG celt,
				&spIUnknown,//[out]  IUnknown** ppObjectArray,
				&cFetched//[out]  ULONG* pcFetched
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Next"), hr));
				break;
			}
			else if (S_FALSE == hr || cFetched != 1)
			{
				break;
			}
			else
			{
				CComPtr<IVdsHwProvider> spIVdsHwProvider;
				hr = spIUnknown.QueryInterface(&spIVdsHwProvider);
				if (FAILED(hr))
				{	
					_OLD_DSETLASTSTATUS(hr);
					nStatus = hr;
					_OLD_DAPIERR((0, DRNPOS, TEXT("QueryInterface(&spIVdsHwProvider)"), hr));
					break;
				}
				hwProvides.push_back(spIVdsHwProvider.Detach());
			}
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CVDSService::QueryVirtualDiskProvides( vector<IVdsVdProvider*> & vdProvides )
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComPtr<IEnumVdsObject> spIEnumVdsObject;
		hr = m_pIVdsService->QueryProviders(
			VDS_QUERY_VIRTUALDISK_PROVIDERS,//[in]   DWORD masks,
			&spIEnumVdsObject//[out]  IEnumVdsObject** ppEnum
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("QueryProviders"), hr));
			break;
		}

		//******************************************
		for (;;)
		{
			CComPtr<IUnknown> spIUnknown;
			ULONG cFetched = 0;
			hr = spIEnumVdsObject->Next(
				1,//[in]   ULONG celt,
				&spIUnknown,//[out]  IUnknown** ppObjectArray,
				&cFetched//[out]  ULONG* pcFetched
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Next"), hr));
				break;
			}
			else if (S_FALSE == hr || cFetched != 1)
			{
				break;
			}
			else
			{
				CComPtr<IVdsVdProvider> spIVdsVdProvider;
				hr = spIUnknown.QueryInterface(&spIVdsVdProvider);
				if (FAILED(hr))
				{	
					_OLD_DSETLASTSTATUS(hr);
					nStatus = hr;
					_OLD_DAPIERR((0, DRNPOS, TEXT("QueryInterface(&spIVdsVdProvider)"), hr));
					break;
				}
				vdProvides.push_back(spIVdsVdProvider.Detach());
			}
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

VOID MY_VDS_PROVIDER_PROP::Copy( VDS_PROVIDER_PROP & VDSProviderProp )
{
	m_id = VDSProviderProp.id;  
	m_pwszName = VDSProviderProp.pwszName;  
	m_guidVersionId = VDSProviderProp.guidVersionId;  
	m_pwszVersion = VDSProviderProp.pwszVersion;  
	m_type = VDSProviderProp.type;  
	m_ulFlags = VDSProviderProp.ulFlags;  
	m_ulStripeSizeFlags = VDSProviderProp.ulStripeSizeFlags;  
	m_sRebuildPriority = VDSProviderProp.sRebuildPriority;
}

VOID MY_VDS_PROVIDER_PROP::VDS_PROVIDER_PROP_Free( VDS_PROVIDER_PROP & VDSProviderProp )
{
	{
		CComHeapPtr<WCHAR> apBufferCleanUp(VDSProviderProp.pwszName);
		VDSProviderProp.pwszName = NULL;
	}
	{
		CComHeapPtr<WCHAR> apBufferCleanUp(VDSProviderProp.pwszVersion);
		VDSProviderProp.pwszVersion = NULL;
	}
}

INT CVdsSwProvider::Initialize( IVdsSwProvider * pIVdsSwProvider, BOOL bCopy )
{
	if (bCopy)
	{
		m_pIVdsSwProvider = pIVdsSwProvider;
	}
	else
	{
		m_pIVdsSwProvider.Attach(pIVdsSwProvider);
	}

	CComPtr<IVdsProvider> spIVdsProvider;
	HRESULT hr = m_pIVdsSwProvider.QueryInterface(&spIVdsProvider);
	if (SUCCEEDED(hr))
	{
		VDS_PROVIDER_PROP ProviderProp = {0};
		hr = spIVdsProvider->GetProperties(
			&ProviderProp//[out]  VDS_PROVIDER_PROP* pProviderProp
			);
		if (SUCCEEDED(hr))
		{
			m_MyVDSProviderProp.Copy(ProviderProp);
		}
		MY_VDS_PROVIDER_PROP::VDS_PROVIDER_PROP_Free(ProviderProp);
	}

	return 0;
}

INT CVdsSwProvider::QueryPacks( vector<IVdsPack*> & Packs )
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComPtr<IEnumVdsObject> spIEnumVdsObject;
		hr = m_pIVdsSwProvider->QueryPacks(
			&spIEnumVdsObject//[out]  IEnumVdsObject** ppEnum
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("QueryPacks"), hr));
			break;
		}

		//******************************************
		for (;;)
		{
			CComPtr<IUnknown> spIUnknown;
			ULONG cFetched = 0;
			hr = spIEnumVdsObject->Next(
				1,//[in]   ULONG celt,
				&spIUnknown,//[out]  IUnknown** ppObjectArray,
				&cFetched//[out]  ULONG* pcFetched
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Next"), hr));
				break;
			}
			else if (S_FALSE == hr || cFetched != 1)
			{
				break;
			}
			else
			{
				CComPtr<IVdsPack> spIVdsPack;
				hr = spIUnknown.QueryInterface(&spIVdsPack);
				if (FAILED(hr))
				{	
					_OLD_DSETLASTSTATUS(hr);
					nStatus = hr;
					_OLD_DAPIERR((0, DRNPOS, TEXT("QueryInterface(&spIVdsPack)"), hr));
					break;
				}
				Packs.push_back(spIVdsPack.Detach());
			}
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}


INT CVdsHwProvider::Initialize( IVdsHwProvider * pIVdsHwProvider, BOOL bCopy )
{
	if (bCopy)
	{
		m_pIVdsHwProvider = pIVdsHwProvider;
	}
	else
	{
		m_pIVdsHwProvider.Attach(pIVdsHwProvider);
	}

	CComPtr<IVdsProvider> spIVdsProvider;
	HRESULT hr = m_pIVdsHwProvider.QueryInterface(&spIVdsProvider);
	if (SUCCEEDED(hr))
	{
		VDS_PROVIDER_PROP ProviderProp = {0};
		hr = spIVdsProvider->GetProperties(
			&ProviderProp//[out]  VDS_PROVIDER_PROP* pProviderProp
			);
		if (SUCCEEDED(hr))
		{
			m_MyVDSProviderProp.Copy(ProviderProp);
		}
		MY_VDS_PROVIDER_PROP::VDS_PROVIDER_PROP_Free(ProviderProp);
	}

	return 0;
}

INT CVdsVdProvider::Initialize( IVdsVdProvider * pIVdsVdProvider, BOOL bCopy )
{
	if (bCopy)
	{
		m_pIVdsVdProvider = pIVdsVdProvider;
	}
	else
	{
		m_pIVdsVdProvider.Attach(pIVdsVdProvider);
	}

	CComPtr<IVdsProvider> spIVdsProvider;
	HRESULT hr = m_pIVdsVdProvider.QueryInterface(&spIVdsProvider);
	if (SUCCEEDED(hr))
	{
		VDS_PROVIDER_PROP ProviderProp = {0};
		hr = spIVdsProvider->GetProperties(
			&ProviderProp//[out]  VDS_PROVIDER_PROP* pProviderProp
			);
		if (SUCCEEDED(hr))
		{
			m_MyVDSProviderProp.Copy(ProviderProp);
		}
		MY_VDS_PROVIDER_PROP::VDS_PROVIDER_PROP_Free(ProviderProp);
	}

	return 0;
}

VOID MY_VDS_PACK_PROP::Copy( VDS_PACK_PROP & VDSPackProp )
{
	m_id = VDSPackProp.id;  
	m_pwszName = PTR_TO_STRING(VDSPackProp.pwszName);  
	m_status = VDSPackProp.status;  
	m_ulFlags = VDSPackProp.ulFlags;
}

VOID MY_VDS_PACK_PROP::VDS_PACK_PROP_Free( VDS_PACK_PROP & VDSPackProp )
{
	{
		CComHeapPtr<WCHAR> apBufferCleanup(VDSPackProp.pwszName);
		VDSPackProp.pwszName = NULL;
	}
}

INT CVdsPack::Initialize( IVdsPack * pIVdsPack, BOOL bCopy )
{
	if (bCopy)
	{
		m_pIVdsPack = pIVdsPack;
	}
	else
	{
		m_pIVdsPack.Attach(pIVdsPack);
	}

	VDS_PACK_PROP VDSPackProp = {0};
	HRESULT hr = m_pIVdsPack->GetProperties(
		&VDSPackProp//[out]  VDS_PROVIDER_PROP* pProviderProp
		);
	if (SUCCEEDED(hr))
	{
		m_MyVDSPackProp.Copy(VDSPackProp);
	}
	MY_VDS_PACK_PROP::VDS_PACK_PROP_Free(VDSPackProp);

	return 0;
}

INT CVdsPack::QueryDisks( vector<IVdsDisk*> & Disks )
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComPtr<IEnumVdsObject> spIEnumVdsObject;
		hr = m_pIVdsPack->QueryDisks(
			&spIEnumVdsObject//[out]  IEnumVdsObject** ppEnum
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("QueryDisks"), hr));
			break;
		}

		//******************************************
		for (;;)
		{
			CComPtr<IUnknown> spIUnknown;
			ULONG cFetched = 0;
			hr = spIEnumVdsObject->Next(
				1,//[in]   ULONG celt,
				&spIUnknown,//[out]  IUnknown** ppObjectArray,
				&cFetched//[out]  ULONG* pcFetched
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Next"), hr));
				break;
			}
			else if (S_FALSE == hr || cFetched != 1)
			{
				break;
			}
			else
			{
				CComPtr<IVdsDisk> spIVdsDisk;
				hr = spIUnknown.QueryInterface(&spIVdsDisk);
				if (FAILED(hr))
				{	
					_OLD_DSETLASTSTATUS(hr);
					nStatus = hr;
					_OLD_DAPIERR((0, DRNPOS, TEXT("QueryInterface(&spIVdsDisk)"), hr));
					break;
				}
				Disks.push_back(spIVdsDisk.Detach());
			}
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CVdsPack::QueryVolumes( vector<IVdsVolume*> & Volumes )
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComPtr<IEnumVdsObject> spIEnumVdsObject;
		hr = m_pIVdsPack->QueryVolumes(
			&spIEnumVdsObject//[out]  IEnumVdsObject** ppEnum
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("QueryVolumes"), hr));
			break;
		}

		//******************************************
		for (;;)
		{
			CComPtr<IUnknown> spIUnknown;
			ULONG cFetched = 0;
			hr = spIEnumVdsObject->Next(
				1,//[in]   ULONG celt,
				&spIUnknown,//[out]  IUnknown** ppObjectArray,
				&cFetched//[out]  ULONG* pcFetched
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Next"), hr));
				break;
			}
			else if (S_FALSE == hr || cFetched != 1)
			{
				break;
			}
			else
			{
				CComPtr<IVdsVolume> spIVdsVolume;
				hr = spIUnknown.QueryInterface(&spIVdsVolume);
				if (FAILED(hr))
				{	
					_OLD_DSETLASTSTATUS(hr);
					nStatus = hr;
					_OLD_DAPIERR((0, DRNPOS, TEXT("QueryInterface(&spIVdsDisk)"), hr));
					break;
				}
				Volumes.push_back(spIVdsVolume.Detach());
			}
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CVdsDisk::Initialize( IVdsDisk * pIVdsDisk, BOOL bCopy )
{
	INT nStatus = 0;

	if (bCopy)
	{
		m_pIVdsDisk = pIVdsDisk;
	}
	else
	{
		m_pIVdsDisk.Attach(pIVdsDisk);
	}

	VDS_DISK_PROP VDSDiskProp = {0};
	HRESULT hr = m_pIVdsDisk->GetProperties(
		&VDSDiskProp//[out]  VDS_PROVIDER_PROP* pProviderProp
		);
	if (SUCCEEDED(hr))
	{
		m_MyVDSDiskProp.Copy(VDSDiskProp);
	}
	MY_VDS_DISK_PROP::VDS_DISK_PROP_Free(VDSDiskProp);

	VDS_DISK_EXTENT* pExtentArray = NULL;
	LONG nNumberOfExtents = 0;
	hr = m_pIVdsDisk->QueryExtents(
		&pExtentArray,//[out]  VDS_DISK_EXTENT** ppExtentArray,
		&nNumberOfExtents//[out]  LONG* plNumberOfExtents
		);
	if (FAILED(hr))
	{	
		_OLD_DSETLASTSTATUS(hr);
		nStatus = hr;
		_OLD_DAPIERR((0, DRNPOS, TEXT("QueryExtents"), hr));
		//break;
	}
	for (LONG ii=0; ii<nNumberOfExtents; ++ii)
	{
		m_VDSDiskExtents.push_back(pExtentArray[ii]);
	}
	if (pExtentArray)
	{
		CoTaskMemFree(pExtentArray);
		pExtentArray = NULL;
	}

	return nStatus;
}

INT CVdsVolume::Initialize( IVdsVolume * pIVdsVolume, BOOL bCopy )
{
	if (bCopy)
	{
		m_pIVdsVolume = pIVdsVolume;
	}
	else
	{
		m_pIVdsVolume.Attach(pIVdsVolume);
	}

	VDS_VOLUME_PROP VDSVolumeProp = {0};
	HRESULT hr = m_pIVdsVolume->GetProperties(
		&VDSVolumeProp//[out]  VDS_PROVIDER_PROP* pProviderProp
		);
	if (SUCCEEDED(hr))
	{
		m_MyVDSVolumeProp.Copy(VDSVolumeProp);
		CString strRoot;
		CVolumeHelper::GetVolumeRootAndGuidFromPath(m_MyVDSVolumeProp.m_strName, strRoot, m_strVolumeGUID);
	}
	MY_VDS_VOLUME_PROP::VDS_VOLUME_PROP_Free(VDSVolumeProp);

	return 0;
}

INT CVdsVolume::QueryPlexes( vector<IVdsVolumePlex*> & vdsPlexes )
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComPtr<IEnumVdsObject> spIEnumVdsObject;
		hr = m_pIVdsVolume->QueryPlexes(
			&spIEnumVdsObject//[out]  IEnumVdsObject** ppEnum
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("QueryPlexes"), hr));
			break;
		}

		//******************************************
		for (;;)
		{
			CComPtr<IUnknown> spIUnknown;
			ULONG cFetched = 0;
			hr = spIEnumVdsObject->Next(
				1,//[in]   ULONG celt,
				&spIUnknown,//[out]  IUnknown** ppObjectArray,
				&cFetched//[out]  ULONG* pcFetched
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Next"), hr));
				break;
			}
			else if (S_FALSE == hr || cFetched != 1)
			{
				break;
			}
			else
			{
				CComPtr<IVdsVolumePlex> spIVdsVolumePlex;
				hr = spIUnknown.QueryInterface(&spIVdsVolumePlex);
				if (FAILED(hr))
				{	
					_OLD_DSETLASTSTATUS(hr);
					nStatus = hr;
					_OLD_DAPIERR((0, DRNPOS, TEXT("QueryInterface(&spIVdsVolumePlex)"), hr));
					break;
				}
				vdsPlexes.push_back(spIVdsVolumePlex.Detach());
			}
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

VOID MY_VDS_SERVICE_PROP::Copy( CONST VDS_SERVICE_PROP & VDSServiceProp )
{
	m_pwszVersion = VDSServiceProp.pwszVersion;  
	m_ulFlags = VDSServiceProp.ulFlags;
}

VOID MY_VDS_SERVICE_PROP::VDS_SERVICE_PROP_Free( VDS_SERVICE_PROP & VDSServiceProp )
{
	{
		CComHeapPtr<WCHAR> apBufferCleanup(VDSServiceProp.pwszVersion);
		VDSServiceProp.pwszVersion = NULL;
	}
}

INT CVdsVolumePlex::Initialize( IVdsVolumePlex * pIVdsVolumePlex, BOOL bCopy )
{
	INT nStatus = 0;

	if (bCopy)
	{
		m_pVdsVolumePlex = pIVdsVolumePlex;
	}
	else
	{
		m_pVdsVolumePlex.Attach(pIVdsVolumePlex);
	}

	HRESULT hr = m_pVdsVolumePlex->GetProperties(
		&m_VDSVolumePlexProp//[out]  VDS_PROVIDER_PROP* pProviderProp
		);
	if (FAILED(hr))
	{	
		_OLD_DSETLASTSTATUS(hr);
		nStatus = hr;
		_OLD_DAPIERR((0, DRNPOS, TEXT("GetProperties"), hr));
		//break;
	}

	if (0 == nStatus)
	{
		VDS_DISK_EXTENT* pExtentArray = NULL;
		LONG nNumberOfExtents = 0;
		hr = m_pVdsVolumePlex->QueryExtents(
			&pExtentArray,//[out]  VDS_DISK_EXTENT** ppExtentArray,
			&nNumberOfExtents//[out]  LONG* plNumberOfExtents
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("QueryExtents"), hr));
			//break;
		}
		else
		{
			for (LONG ii=0; ii<nNumberOfExtents; ++ii)
			{
				m_VDSDiskExtents.push_back(pExtentArray[ii]);
			}
			if (pExtentArray)
			{
				CoTaskMemFree(pExtentArray);
				pExtentArray = NULL;
			}
		}

	}

	return 0;
}