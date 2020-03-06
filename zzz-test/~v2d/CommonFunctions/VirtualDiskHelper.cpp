#include "stdafx.h"

#include "VirtualDiskHelper.h"
#include <InitGuid.h>
#include "DiskHelper.h"

#if 1

#pragma comment(lib, "Virtdisk.lib")

DEFINE_GUID(VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT_COPY, 0xec984aec, 0xa0f9, 0x47e9, 0x90, 0x1f, 0x71, 0x41, 0x5a, 0x66, 0x34, 0x5b);

VOID CVirtualDiskHelper::InitializeCreateVirtualDiskParameters( PCREATE_VIRTUAL_DISK_PARAMETERS Parameter, ULONGLONG MaximumSize, LPCTSTR ParentPath, LPCTSTR SourcePath )
{
	ZeroMemory(Parameter,sizeof(CREATE_VIRTUAL_DISK_PARAMETERS));
	Parameter->Version = CREATE_VIRTUAL_DISK_VERSION_1;
	ZeroMemory(&Parameter->Version1.UniqueId, sizeof(GUID));
	Parameter->Version1.MaximumSize = MaximumSize;
	Parameter->Version1.BlockSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_BLOCK_SIZE;
	Parameter->Version1.SectorSizeInBytes = CREATE_VIRTUAL_DISK_PARAMETERS_DEFAULT_SECTOR_SIZE;
	Parameter->Version1.ParentPath = ParentPath;
	Parameter->Version1.SourcePath = SourcePath;
}

INT CVirtualDiskHelper::Create( LPCTSTR szPath, VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, PCREATE_VIRTUAL_DISK_PARAMETERS Parameters, OUT PHANDLE pHandle )
{
	VIRTUAL_STORAGE_TYPE VirtualStorageType;
	VirtualStorageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHD;
	VirtualStorageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT_COPY;
	DWORD dwStatus = ::CreateVirtualDisk(
		&VirtualStorageType,//__in   PVIRTUAL_STORAGE_TYPE VirtualStorageType,
		szPath,//__in   PCWSTR Path,
		VirtualDiskAccessMask,//__in   VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask,
		NULL,//__in   PSECURITY_DESCRIPTOR SecurityDescriptor,
		CREATE_VIRTUAL_DISK_FLAG_NONE ,//__in   CREATE_VIRTUAL_DISK_FLAG Flags,
		0,//__in   ULONG ProviderSpecificFlags,
		Parameters,//__in   PCREATE_VIRTUAL_DISK_PARAMETERS Parameters,
		NULL,//__in   LPOVERLAPPED Overlapped,
		pHandle//__out  PHANDLE Handle
		);
	if (dwStatus)
	{
		_OLD_DAPIERR((0, DRNPOS, TEXT("CreateVirtualDisk"), dwStatus));
	}
	return (INT)dwStatus;
}

INT CVirtualDiskHelper::Attach( HANDLE hVhd, ATTACH_VIRTUAL_DISK_FLAG AttachFlags)
{
	ATTACH_VIRTUAL_DISK_PARAMETERS AttachVirtualDiskParameters;
	ZeroMemory(&AttachVirtualDiskParameters, sizeof(ATTACH_VIRTUAL_DISK_PARAMETERS));
	AttachVirtualDiskParameters.Version = ATTACH_VIRTUAL_DISK_VERSION_1;
	DWORD dwStatus = ::AttachVirtualDisk(
		hVhd,//__in  HANDLE VirtualDiskHandle,
		NULL,//__in  PSECURITY_DESCRIPTOR SecurityDescriptor,
		AttachFlags,//__in  ATTACH_VIRTUAL_DISK_FLAG Flags,
		0,//__in  ULONG ProviderSpecificFlags,
		&AttachVirtualDiskParameters,//__in  PATTACH_VIRTUAL_DISK_PARAMETERS Parameters,
		NULL//__in  LPOVERLAPPED Overlapped
		);
	if (dwStatus)
	{
		_OLD_DAPIERR((0, DRNPOS, TEXT("AttachVirtualDisk"), dwStatus));
	}
	return (INT)dwStatus;
}

BOOL CVirtualDiskHelper::IsUninitalizedVHD( DWORD dwDiskNumber )
{
	INT  nStatus = 0;
	BOOL bRet = FALSE;

	do 
	{
		//********************************************************************
		CHandle hDevice(CDiskHelper::CreateHandle(dwDiskNumber, GENERIC_READ));
		if (nStatus)
		{
			break;
		}

		//********************************************************************
		DRIVE_LAYOUT_INFORMATION_EX * pDriveLayoutInformationEx = NULL;
		nStatus = CDiskHelper::IOCTL_DISK_GET_DRIVE_LAYOUT_EX_Helper(
			hDevice,//IN HANDLE hDevice,
			&pDriveLayoutInformationEx//OUT DRIVE_LAYOUT_INFORMATION_EX ** ppDriveLayoutInformationEx
			);
		CHeapPtr<BYTE> apBufferTempDriveLayoutInformationEx((BYTE*)pDriveLayoutInformationEx);
		if (nStatus || NULL == pDriveLayoutInformationEx || 0 != pDriveLayoutInformationEx->PartitionCount)
		{
			break;
		}

		//********************************************************************
		STORAGE_DEVICE_DESCRIPTOR * pStorageDeviceDescriptor = NULL;
		nStatus = CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_DEVICE_Helper(
			hDevice,//IN HANDLE hDevice, 
			&pStorageDeviceDescriptor//OUT STORAGE_DEVICE_DESCRIPTOR ** ppStorageDeviceDescriptor
			);
		CHeapPtr<BYTE> apBufferTempStorageDeviceDescriptor((BYTE*)pStorageDeviceDescriptor);
		if (nStatus || NULL == pStorageDeviceDescriptor || BusTypeFileBackedVirtual != pStorageDeviceDescriptor->BusType)
		{
			break;
		}

		//********************************************************************
		bRet = TRUE;

	} while (FALSE);

	return bRet;
}

DWORD CVirtualDiskHelper::FindUninitalizedVHD()
{
	DWORD dwDiskNumber = (DWORD)(-1);
	CONST DWORD MAX_DISK_NUMBER = 256;
	for (DWORD i=0; i<MAX_DISK_NUMBER; ++i)
	{
		if (IsUninitalizedVHD(i))
		{
			dwDiskNumber = i;
			break;
		}
	}
	return dwDiskNumber;
}

INT CVirtualDiskHelper::Dettach( HANDLE hVhd )
{
	INT nStatus = 0;
	DWORD dwErrorCode = DetachVirtualDisk(
		hVhd,//__in  HANDLE VirtualDiskHandle,
		DETACH_VIRTUAL_DISK_FLAG_NONE,//__in  DETACH_VIRTUAL_DISK_FLAG Flags,
		0//__in  ULONG ProviderSpecificFlags
		);
	if (ERROR_SUCCESS != dwErrorCode)
	{
		_OLD_DSETLASTSTATUS(dwErrorCode);
		nStatus = dwErrorCode;
		_OLD_DAPIERR((0, DRNPOS, TEXT("DetachVirtualDisk"), dwErrorCode));
	}

	return nStatus;
}
INT VirtualDiskMain( LPCTSTR szVHDPath, ULONG nSizeGB )
{
	INT nStatus = 0;

	CHandle hVhd;
	BOOL bAttached = FALSE;

	do 
	{
		//********************************************************************
		CREATE_VIRTUAL_DISK_PARAMETERS CreateVirtualDiskParameters;
		ULONGLONG ullDiskSize = ((ULONGLONG)(nSizeGB))*1024*1024*1024;
		CVirtualDiskHelper::InitializeCreateVirtualDiskParameters(
			&CreateVirtualDiskParameters,
			ullDiskSize,
			NULL,
			NULL
			);

		//********************************************************************
		HANDLE hVhdRaw = NULL;
		nStatus = CVirtualDiskHelper::Create(
			szVHDPath,//LPCTSTR szPath,  
			VIRTUAL_DISK_ACCESS_ALL,//VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask,
			&CreateVirtualDiskParameters,//PCREATE_VIRTUAL_DISK_PARAMETERS Parameters,
			&hVhdRaw//OUT PHANDLE pHandle
			);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Create"), E_FAIL));
			break;
		}
		hVhd.Attach(hVhdRaw); hVhdRaw = NULL;

		//********************************************************************
		ATTACH_VIRTUAL_DISK_FLAG AttachFlags = ATTACH_VIRTUAL_DISK_FLAG_NO_DRIVE_LETTER | ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME;
		nStatus = CVirtualDiskHelper::Attach( hVhd, AttachFlags);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Attach"), E_FAIL));
			break;
		}
		bAttached = TRUE;

		//********************************************************************
		DWORD dwDiskNumber = CVirtualDiskHelper::FindUninitalizedVHD();
		if ((DWORD)(-1) == dwDiskNumber)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("FindUninitalizedVHD"), E_FAIL));
			nStatus = E_FAIL;
			break;
		}
		_ftprintf_s(stdout, TEXT("*** ") TEXT("[VHDPath, SizeGB, DiskNumber]=[%s, %u, %u]") TEXT(" ***\r\n"), szVHDPath, nSizeGB, dwDiskNumber);

		//********************************************************************
		CHandle hDevice(CDiskHelper::CreateHandle(dwDiskNumber, GENERIC_ALL));
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateHandle"), E_FAIL));
			break;
		}

		//********************************************************************
		GUID diskId = {0};
		nStatus = CDiskHelper::IOCTL_DISK_CREATE_DISK_GPT_Helper(hDevice, diskId);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("IOCTL_DISK_CREATE_DISK_GPT_Helper"), E_FAIL));
			break;
		}

		//********************************************************************
		DRIVE_LAYOUT_INFORMATION_EX * pDriveLayoutInformationEx = NULL;
		
		nStatus = CDiskHelper::IOCTL_DISK_GET_DRIVE_LAYOUT_EX_Helper(
			hDevice,//IN HANDLE hDevice,
			&pDriveLayoutInformationEx//OUT DRIVE_LAYOUT_INFORMATION_EX ** ppDriveLayoutInformationEx
			);

		//********************************************************************
		DRIVE_LAYOUT_INFORMATION_EX DriveLayoutInformation =  {0};
		GUID partitionId = {0};
		nStatus = CDiskHelper::GetDriveLayoutFromTemplateGPT(pDriveLayoutInformationEx, &DriveLayoutInformation, partitionId);
		nStatus = CDiskHelper::IOCTL_DISK_SET_DRIVE_LAYOUT_EX_Helper(hDevice, &DriveLayoutInformation);

		DRIVE_LAYOUT_INFORMATION_EX * pDriveLayoutInformationExTest = NULL;
		nStatus = CDiskHelper::IOCTL_DISK_GET_DRIVE_LAYOUT_EX_Helper(
			hDevice,//IN HANDLE hDevice,
			&pDriveLayoutInformationExTest//OUT DRIVE_LAYOUT_INFORMATION_EX ** ppDriveLayoutInformationEx
			);

		//********************************************************************
		{
			//cleanup
			CHeapPtr<BYTE> apBufferTemp;
			apBufferTemp.Attach((BYTE*)pDriveLayoutInformationExTest);
			apBufferTemp.Attach((BYTE*)pDriveLayoutInformationEx);
			pDriveLayoutInformationExTest = NULL;
			pDriveLayoutInformationEx = NULL;
		}

	} while (FALSE);


	//******************************************
	if (bAttached)
	{
		CVirtualDiskHelper::Dettach(hVhd);
		bAttached = FALSE;
	}

	return nStatus;
}

INT AppVirtualDiskMain( INT argc, _TCHAR* argv[] )
{
	CCmdLineTestVirtualDiskHelper cmdLine;
	cmdLine.Parse(argc, argv);
	return VirtualDiskMain(cmdLine.m_strVHDPath, cmdLine.m_SizeGB);
}

static BOOL IsUninitalizedVHD(DWORD dwDiskNumber)
{
	INT  nStatus = 0;
	BOOL bRet = FALSE;
	
	do 
	{
		//********************************************************************
		CHandle hDevice(CDiskHelper::CreateHandle(dwDiskNumber, GENERIC_READ));
		if (nStatus)
		{
			break;
		}

		//********************************************************************
		DRIVE_LAYOUT_INFORMATION_EX * pDriveLayoutInformationEx = NULL;
		nStatus = CDiskHelper::IOCTL_DISK_GET_DRIVE_LAYOUT_EX_Helper(
			hDevice,//IN HANDLE hDevice,
			&pDriveLayoutInformationEx//OUT DRIVE_LAYOUT_INFORMATION_EX ** ppDriveLayoutInformationEx
			);
		CHeapPtr<BYTE> apBufferTempDriveLayoutInformationEx((BYTE*)pDriveLayoutInformationEx);
		if (nStatus || NULL == pDriveLayoutInformationEx || 0 != pDriveLayoutInformationEx->PartitionCount)
		{
			break;
		}

		//********************************************************************
		STORAGE_DEVICE_DESCRIPTOR * pStorageDeviceDescriptor = NULL;
		nStatus = CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_DEVICE_Helper(
			hDevice,//IN HANDLE hDevice, 
			&pStorageDeviceDescriptor//OUT STORAGE_DEVICE_DESCRIPTOR ** ppStorageDeviceDescriptor
			);
		CHeapPtr<BYTE> apBufferTempStorageDeviceDescriptor((BYTE*)pStorageDeviceDescriptor);
		if (nStatus || NULL == pStorageDeviceDescriptor || BusTypeFileBackedVirtual != pStorageDeviceDescriptor->BusType)
		{
			break;
		}

		//********************************************************************
		bRet = TRUE;

	} while (FALSE);

	return bRet;
}

static DWORD FindUninitalizedVHD()
{
	DWORD dwDiskNumber = (DWORD)(-1);
	CONST DWORD MAX_DISK_NUMBER = 256;
	for (DWORD i=0; i<MAX_DISK_NUMBER; ++i)
	{
		if (IsUninitalizedVHD(i))
		{
			dwDiskNumber = i;
			break;
		}
	}
	return dwDiskNumber;
}

#endif