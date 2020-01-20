#pragma once

#include "stdafx.h"

#include <vector>
using namespace std;

struct CDiskHelper
{
	static HANDLE CreateHandle(
		IN DWORD dwDiskNumber, 
		IN DWORD dwDesiredAccess
		);

	static HANDLE CreateHandle(
		IN LPCTSTR szDiskNumber, 
		IN DWORD dwDesiredAccess
		);

	static INT IOCTL_DISK_GET_DRIVE_GEOMETRY_EX_Helper(
		IN HANDLE hDevice, 
		OUT DISK_GEOMETRY_EX ** ppDiskGeometryEx,
		OUT DISK_DETECTION_INFO ** ppDiskDetectionInfo,
		OUT DISK_PARTITION_INFO ** ppDiskPartitionInfo
		);

	static INT IOCTL_DISK_GET_DRIVE_LAYOUT_EX_Helper(
		IN HANDLE hDevice,
		OUT DRIVE_LAYOUT_INFORMATION_EX ** ppDriveLayoutInformationEx
		);

	static INT IOCTL_DISK_GET_PARTITION_INFO_EX_Helper(
		IN HANDLE hDevice, 
		OUT PARTITION_INFORMATION_EX * pPartitionInformationEx
		);

	static INT IOCTL_DISK_GET_LENGTH_INFO_Helper(
		IN HANDLE hDevice, 
		OUT GET_LENGTH_INFORMATION * pGetLengthInformation
		);

	static INT IOCTL_STORAGE_QUERY_PROPERTY_DEVICE_Helper(
		IN HANDLE hDevice, 
		OUT STORAGE_DEVICE_DESCRIPTOR ** ppStorageDeviceDescriptor
		);

	static INT IOCTL_STORAGE_QUERY_PROPERTY_ADAPTER_Helper(
		HANDLE hDevice, 
		STORAGE_ADAPTER_DESCRIPTOR ** ppStorageAdapterDescriptor
		);

	static INT IOCTL_STORAGE_QUERY_PROPERTY_ID_Helper(
		HANDLE hDevice, 
		STORAGE_DEVICE_ID_DESCRIPTOR ** ppStorageDeviceIDDescriptor
		);

	static INT IOCTL_STORAGE_QUERY_PROPERTY_UNIQUE_ID_Helper(
		HANDLE hDevice, 
		STORAGE_DEVICE_ID_DESCRIPTOR ** ppStorageDeviceIDDescriptor
		);

	static INT IOCTL_STORAGE_GET_DEVICE_NUMBER_Helper(
		HANDLE hDevice, 
		STORAGE_DEVICE_NUMBER * pStorageDeviceNumber
		);

	static INT IOCTL_STORAGE_GET_HOTPLUG_INFO_Helper(
		HANDLE hDevice, 
		STORAGE_HOTPLUG_INFO * pStorageHotplugInfo
		);

	static INT IOCTL_STORAGE_GET_MEDIA_TYPES_EX_Helper(
		HANDLE hDevice,  
		GET_MEDIA_TYPES ** ppMEDIA_TYPES
		);

	static INT IOCTL_DISK_CREATE_DISK_GPT_Helper(HANDLE hDevice, GUID& diskGUID );

	static INT GetDriveLayoutFromTemplateGPT(
		IN PDRIVE_LAYOUT_INFORMATION_EX pDriveLayoutInformationTemplate,
		OUT PDRIVE_LAYOUT_INFORMATION_EX pDriveLayoutInformation,
		OUT GUID & partitionId 
		);

	static INT IOCTL_DISK_SET_DRIVE_LAYOUT_EX_Helper(
		HANDLE hDevice, 
		PDRIVE_LAYOUT_INFORMATION_EX pDriveLayoutInformationEx
		);
};

struct CCmdLineTestDiskMain
{
	vector<DWORD> m_vecDiskNumber;

	//********************************************************************
	CCmdLineTestDiskMain()
	{
	}

	static BOOL IsParamIndicator(TCHAR ch)
	{
		return (TEXT('-') == ch || TEXT('/') == ch);
	}

	//this.exe -disknumber <n> -disknumber <n> ...
	VOID Parse(INT argc, _TCHAR* argv[])
	{
		static LPCTSTR szCmdline[] = 
		{
			TEXT("DiskNumber"), //0
		};
		for (INT i=0; i<argc; ++i)
		{
			if (IsParamIndicator(argv[i][0]))
			{
				if (0 == _tcsicmp(szCmdline[0], &argv[i][1]) )
				{
					++i;
					DWORD dwDiskNumber = _tcstoul(argv[i], NULL, 10);
					m_vecDiskNumber.push_back(dwDiskNumber);
				}
			}
		}
	}
};

INT TestDisk(DWORD dwDiskNumber);
INT TestDiskMain(int argc, _TCHAR* argv[]);