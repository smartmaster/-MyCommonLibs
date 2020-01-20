#pragma once


#include "SingleVdsDiskInfo.h"

#include "SingleVdsVolumeInfoXml.h"

//
//m_id
//m_status
//m_ReserveMode
//m_health
//m_dwDeviceType
//m_dwMediaType
//m_ullSize
//m_ulBytesPerSector
//m_ulSectorsPerTrack
//m_ulTracksPerCylinder
//m_ulFlags
//m_BusType
//m_PartitionStyle
//m_DiskGuid
//m_strDiskAddress
//m_strName
//m_strFriendlyName
//m_strAdaptorName
//m_strDevicePath


struct MY_VDS_DISK_PROP_Xml
{
	SERIALIZE_BEGIN_STRUCT(MY_VDS_DISK_PROP_Xml, MY_VDS_DISK_PROP_Xml);
	/*VDS_OBJECT_ID*/ GUID m_id;								SERIALIZE_BASIC(m_id);
	/*VDS_DISK_STATUS*/ INT m_status;							SERIALIZE_BASIC(m_status);
	/*VDS_LUN_RESERVE_MODE*/ INT  m_ReserveMode;				SERIALIZE_BASIC(m_ReserveMode);
	/*VDS_HEALTH*/ INT  m_health;								SERIALIZE_BASIC(m_health);
	DWORD m_dwDeviceType;										SERIALIZE_BASIC(m_dwDeviceType);
	DWORD m_dwMediaType;										SERIALIZE_BASIC(m_dwMediaType);
	ULONGLONG m_ullSize;										SERIALIZE_BASIC(m_ullSize);
	ULONG m_ulBytesPerSector;									SERIALIZE_BASIC(m_ulBytesPerSector);
	ULONG m_ulSectorsPerTrack;									SERIALIZE_BASIC(m_ulSectorsPerTrack);
	ULONG m_ulTracksPerCylinder;								SERIALIZE_BASIC(m_ulTracksPerCylinder);
	ULONG m_ulFlags;											SERIALIZE_BASIC(m_ulFlags);
	/*VDS_STORAGE_BUS_TYPE*/ INT  m_BusType;					SERIALIZE_BASIC(m_BusType);
	/*VDS_PARTITION_STYLE*/ INT  m_PartitionStyle;				SERIALIZE_BASIC(m_PartitionStyle);
	GUID m_DiskGuid;											SERIALIZE_BASIC(m_DiskGuid);
	CString m_strDiskAddress;									SERIALIZE_BASIC(m_strDiskAddress);
	CString m_strName;											SERIALIZE_BASIC(m_strName);
	CString m_strFriendlyName;									SERIALIZE_BASIC(m_strFriendlyName);
	CString m_strAdaptorName;									SERIALIZE_BASIC(m_strAdaptorName);
	CString m_strDevicePath;									SERIALIZE_BASIC(m_strDevicePath);
	SERIALIZE_END_STRUCT(MY_VDS_DISK_PROP_Xml, MY_VDS_DISK_PROP_Xml);
	STRUCT_XML_SERIALIZER;


	MY_VDS_DISK_PROP_Xml()
	{
		ZeroMemory(this, FIELD_OFFSET(MY_VDS_DISK_PROP_Xml, m_strDiskAddress));
	}
};

namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST MY_VDS_DISK_PROP & src, MY_VDS_DISK_PROP_Xml & target)
	{
		target.m_id = src.m_id;
		target.m_status = src.m_status;
		target.m_ReserveMode = src.m_ReserveMode;
		target.m_health = src.m_health;
		target.m_dwDeviceType = src.m_dwDeviceType;
		target.m_dwMediaType = src.m_dwMediaType;
		target.m_ullSize = src.m_ullSize;
		target.m_ulBytesPerSector = src.m_ulBytesPerSector;
		target.m_ulSectorsPerTrack = src.m_ulSectorsPerTrack;
		target.m_ulTracksPerCylinder = src.m_ulTracksPerCylinder;
		target.m_ulFlags = src.m_ulFlags;
		target.m_BusType = src.m_BusType;
		target.m_PartitionStyle = src.m_PartitionStyle;
		target.m_DiskGuid = src.m_DiskGuid;
		target.m_strDiskAddress = src.m_strDiskAddress;
		target.m_strName = src.m_strName;
		target.m_strFriendlyName = src.m_strFriendlyName;
		target.m_strAdaptorName = src.m_strAdaptorName;
		target.m_strDevicePath = src.m_strDevicePath;
	}

	static void Xml2Bin(CONST MY_VDS_DISK_PROP_Xml & src, MY_VDS_DISK_PROP & target)
	{
		target.m_id = src.m_id;
		target.m_status = (VDS_DISK_STATUS)src.m_status;
		target.m_ReserveMode = (VDS_LUN_RESERVE_MODE)src.m_ReserveMode;
		target.m_health = (VDS_HEALTH)src.m_health;
		target.m_dwDeviceType = src.m_dwDeviceType;
		target.m_dwMediaType = src.m_dwMediaType;
		target.m_ullSize = src.m_ullSize;
		target.m_ulBytesPerSector = src.m_ulBytesPerSector;
		target.m_ulSectorsPerTrack = src.m_ulSectorsPerTrack;
		target.m_ulTracksPerCylinder = src.m_ulTracksPerCylinder;
		target.m_ulFlags = src.m_ulFlags;
		target.m_BusType = (VDS_STORAGE_BUS_TYPE)src.m_BusType;
		target.m_PartitionStyle = (VDS_PARTITION_STYLE)src.m_PartitionStyle;
		target.m_DiskGuid = src.m_DiskGuid;
		target.m_strDiskAddress = src.m_strDiskAddress;
		target.m_strName = src.m_strName;
		target.m_strFriendlyName = src.m_strFriendlyName;
		target.m_strAdaptorName = src.m_strAdaptorName;
		target.m_strDevicePath = src.m_strDevicePath;
	}
}




//m_MyVDSDiskProp
//m_VDSDiskExtents

struct CSingleVdsDiskInfoXml
{
	SERIALIZE_BEGIN_STRUCT(CSingleVdsDiskInfoXml, CSingleVdsDiskInfoXml);
	MY_VDS_DISK_PROP_Xml m_MyVDSDiskProp;				SERIALIZE_COMPLEX(m_MyVDSDiskProp);
	vector<VDS_DISK_EXTENT_Xml> m_VDSDiskExtents;		SERIALIZE_COMPLEX_VECTOR(m_VDSDiskExtents);
	SERIALIZE_END_STRUCT(CSingleVdsDiskInfoXml, CSingleVdsDiskInfoXml);
	STRUCT_XML_SERIALIZER;


	CSingleVdsDiskInfoXml()
	{
	}
};


namespace BinXmlConv_NS
{
	static VOID Bin2Xml(CONST CSingleVdsDiskInfo & src, CSingleVdsDiskInfoXml & target)
	{
		Bin2Xml(src.m_MyVDSDiskProp, target.m_MyVDSDiskProp);
		target.m_VDSDiskExtents.resize(src.m_VDSDiskExtents.size());
		for (size_t ii = 0; ii < target.m_VDSDiskExtents.size(); ++ ii)
		{
			Bin2Xml(src.m_VDSDiskExtents[ii], target.m_VDSDiskExtents[ii]);
		}
	}

	static void Xml2Bin(CONST CSingleVdsDiskInfoXml & src, CSingleVdsDiskInfo & target)
	{
		Xml2Bin(src.m_MyVDSDiskProp, target.m_MyVDSDiskProp);
		target.m_VDSDiskExtents.resize(src.m_VDSDiskExtents.size());
		for (size_t ii = 0; ii < target.m_VDSDiskExtents.size(); ++ii)
		{
			Xml2Bin(src.m_VDSDiskExtents[ii], target.m_VDSDiskExtents[ii]);
		}
	}
}