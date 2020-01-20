#pragma once

////#include "stdafx.h"////

#include "..\PublicHeader\PublicDTraceLib.h"

#include "DiskHelper.h"

struct CSingleDiskInfo
{
	DWORD m_DiskNumber;
	DISK_GEOMETRY_EX * m_pDiskGeometryEx;
	DRIVE_LAYOUT_INFORMATION_EX * m_pDriveLayoutInformationEx;
	PARTITION_INFORMATION_EX m_PartitionInformationEx;
	GET_LENGTH_INFORMATION m_GetLengthInformation;
	STORAGE_DEVICE_DESCRIPTOR * m_pStorageDeviceDescriptor;
	STORAGE_ADAPTER_DESCRIPTOR * m_pStorageAdapterDescriptor;
	STORAGE_DEVICE_ID_DESCRIPTOR * m_pStorageDeviceIDDescriptor;
	STORAGE_DEVICE_ID_DESCRIPTOR * m_pStorageDeviceUniqueIDDescriptor;
	STORAGE_DEVICE_NUMBER m_StorageDeviceNumber;
	STORAGE_HOTPLUG_INFO m_StorageHotplugInfo;
	GET_MEDIA_TYPES * m_pMEDIA_TYPES;


	CSingleDiskInfo();
	~CSingleDiskInfo();

	INT Retrieve(DWORD dwDiskNumber, BOOL bBreakOnError);
	INT WriteToStorage(IStorage * pIStorage);
	INT ReadFromStorage(IStorage * pIStorage);

	

private: //******************************************
	CSingleDiskInfo(CONST CSingleDiskInfo&);
	CSingleDiskInfo& operator=(CONST CSingleDiskInfo&);
};

struct CAllDiskInfo
{
	//******************************************
	vector<CSingleDiskInfo*> m_pDiskInfos;

	//******************************************
	CAllDiskInfo();
	~CAllDiskInfo();
	VOID DestroyMember();

	//******************************************
	INT Retrieve(DWORD dwMaxDiskNumber);
	INT WriteToStorage(IStorage * pIStorage);
	INT ReadFromStorage(IStorage * pIStorage);

	CONST CSingleDiskInfo* FindDisk(ULONG ulDiskNumber);

private: //****************************************** not support deep copy
	CAllDiskInfo(CONST CAllDiskInfo&);
	CAllDiskInfo& operator=(CONST CAllDiskInfo&);

public:
	static VOID GetAllDiskNumbers(DWORD dwMaxDiskNumber, vector<DWORD> & DiskkNumbers);
};