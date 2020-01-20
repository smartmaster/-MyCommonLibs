#pragma once

////#include "stdafx.h"////

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicDTraceLib.h"

#include <vds.h>

struct MY_VDS_SERVICE_PROP 
{  
	CString m_pwszVersion;  
	ULONG m_ulFlags;

	VOID Copy(CONST VDS_SERVICE_PROP & VDSServiceProp );
	static VOID VDS_SERVICE_PROP_Free(VDS_SERVICE_PROP & VDSServiceProp );
};

struct CVDSService
{
	CComPtr<IVdsService> m_pIVdsService;
	MY_VDS_SERVICE_PROP m_MyVDSServicEProp;

	//******************************************
	INT Load();
	INT QuerySoftwareProvides(vector<IVdsSwProvider*> & swProvides);
	INT QueryHardwareProvides(vector<IVdsHwProvider*> & hwProvides);
	INT QueryVirtualDiskProvides(vector<IVdsVdProvider*> & vdProvides);
};


struct MY_VDS_PROVIDER_PROP 
{  
	VDS_OBJECT_ID m_id;  
	CString m_pwszName;  
	GUID m_guidVersionId;  
	CString m_pwszVersion;  
	VDS_PROVIDER_TYPE m_type;  
	ULONG m_ulFlags;  
	ULONG m_ulStripeSizeFlags;  
	SHORT m_sRebuildPriority;

	VOID Copy(VDS_PROVIDER_PROP & VDSProviderProp );
	static VOID VDS_PROVIDER_PROP_Free(VDS_PROVIDER_PROP & VDSProviderProp );
};

struct CVdsSwProvider
{
	CComPtr<IVdsSwProvider> m_pIVdsSwProvider;
	MY_VDS_PROVIDER_PROP m_MyVDSProviderProp;

	INT Initialize(IVdsSwProvider * pIVdsSwProvider, BOOL bCopy);
	INT QueryPacks(vector<IVdsPack*> & Packs);
};

struct MY_VDS_PACK_PROP 
{  
	VDS_OBJECT_ID m_id;  
	CString m_pwszName;  
	VDS_PACK_STATUS m_status;  
	ULONG m_ulFlags;

	VOID Copy(VDS_PACK_PROP & VDSPackProp);
	static VOID VDS_PACK_PROP_Free(VDS_PACK_PROP & VDSPackProp);
} ;

struct CVdsPack
{
	CComPtr<IVdsPack> m_pIVdsPack;
	MY_VDS_PACK_PROP m_MyVDSPackProp;

	INT Initialize(IVdsPack * pIVdsPack, BOOL bCopy);
	INT QueryDisks(vector<IVdsDisk*> & Disks);
	INT QueryVolumes(vector<IVdsVolume*> & Volumes);
};


struct MY_VDS_DISK_PROP 
{  
	VDS_OBJECT_ID m_id;  
	VDS_DISK_STATUS m_status;  
	VDS_LUN_RESERVE_MODE m_ReserveMode;  
	VDS_HEALTH m_health;  
	DWORD m_dwDeviceType;  
	DWORD m_dwMediaType;  
	ULONGLONG m_ullSize;  
	ULONG m_ulBytesPerSector;  
	ULONG m_ulSectorsPerTrack;  
	ULONG m_ulTracksPerCylinder;  
	ULONG m_ulFlags;  
	VDS_STORAGE_BUS_TYPE m_BusType;  
	VDS_PARTITION_STYLE m_PartitionStyle;  
	union {    
		DWORD m_dwSignature;    
		GUID m_DiskGuid;  
	};  
	CString m_strDiskAddress;  
	CString m_strName;  
	CString m_strFriendlyName;  
	CString m_strAdaptorName;  
	CString m_strDevicePath;

	VOID Copy(VDS_DISK_PROP & VDSDiskProp );
	static VOID VDS_DISK_PROP_Free(VDS_DISK_PROP & VDSDiskProp );
};

struct CVdsDisk
{
	CComPtr<IVdsDisk> m_pIVdsDisk;
	MY_VDS_DISK_PROP m_MyVDSDiskProp;
	vector<VDS_DISK_EXTENT> m_VDSDiskExtents;

	INT Initialize(IVdsDisk * pIVdsDisk, BOOL bCopy);
};

struct MY_VDS_VOLUME_PROP 
{  
	VDS_OBJECT_ID m_id;  
	VDS_VOLUME_TYPE m_type;  
	VDS_VOLUME_STATUS m_status;  
	VDS_HEALTH m_health;  
	VDS_TRANSITION_STATE m_TransitionState;  
	ULONGLONG m_ullSize;  
	ULONG m_ulFlags;  
	VDS_FILE_SYSTEM_TYPE m_RecommendedFileSystemType;  
	CString m_strName;

	VOID Copy(CONST VDS_VOLUME_PROP & VDSVolumeProp);
	static VOID VDS_VOLUME_PROP_Free(VDS_VOLUME_PROP & VDSVolumeProp);
};

struct CVdsVolume
{
	CComPtr<IVdsVolume> m_pIVdsVolume;
	MY_VDS_VOLUME_PROP m_MyVDSVolumeProp;
	CString m_strVolumeGUID;

	INT Initialize(IVdsVolume * pIVdsVolume, BOOL bCopy);
	INT QueryPlexes(vector<IVdsVolumePlex*> & vdsPlexes);
};

struct CVdsVolumePlex
{
	CComPtr<IVdsVolumePlex> m_pVdsVolumePlex;
	VDS_VOLUME_PLEX_PROP m_VDSVolumePlexProp;
	vector<VDS_DISK_EXTENT> m_VDSDiskExtents;

	INT Initialize(IVdsVolumePlex * pIVdsVolumePlex, BOOL bCopy);
};

struct CVdsHwProvider
{
	CComPtr<IVdsHwProvider> m_pIVdsHwProvider;
	MY_VDS_PROVIDER_PROP m_MyVDSProviderProp;

	INT Initialize(IVdsHwProvider * pIVdsHwProvider, BOOL bCopy);
};

struct CVdsVdProvider
{
	CComPtr<IVdsVdProvider> m_pIVdsVdProvider;
	MY_VDS_PROVIDER_PROP m_MyVDSProviderProp;

	INT Initialize(IVdsVdProvider * pIVdsVdProvider, BOOL bCopy);
};