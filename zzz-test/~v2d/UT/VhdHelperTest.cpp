#include "stdafx.h"
#include "..\CommonFunctions\VhdHelper.h"
#include "..\CommonFunctions\DiskHelper.h"

INT VirtualDiskTest( LPCTSTR szVHDPath, ULONG nSizeGB )
{
	INT nStatus = 0;

	BOOL bAttached = FALSE;
	CVhdHelper vhdHelper;

	do 
	{
		//********************************************************************
		ULONGLONG ullDiskSize = ((ULONGLONG)(nSizeGB))*1024*1024*1024;
		nStatus = vhdHelper.Create(
			szVHDPath,//LPCTSTR szPath, 
			VIRTUAL_DISK_ACCESS_ALL,//VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, 
			ullDiskSize,//ULONGLONG MaximumSize, 
			NULL//LPCTSTR ParentPath
			);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Create"), E_FAIL));
			break;
		}

		//********************************************************************
		ATTACH_VIRTUAL_DISK_FLAG AttachFlags = ATTACH_VIRTUAL_DISK_FLAG_NO_DRIVE_LETTER | ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME;
		nStatus = vhdHelper.Attach(AttachFlags);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Attach"), E_FAIL));
			break;
		}
		bAttached = TRUE;

		//********************************************************************
		DWORD dwDiskNumber = vhdHelper.FindUninitalizedVHD();
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
		vhdHelper.Dettach();
		bAttached = FALSE;
	}

	return nStatus;
}

INT VirtualDiskChildTest( LPCTSTR szVHDPath, LPCTSTR szParent )
{
	INT nStatus = 0;

	BOOL bAttached = FALSE;
	CVhdHelper vhdHelper;

	do 
	{
		//********************************************************************
		ULONGLONG ullDiskSize = 0;
		nStatus = vhdHelper.Create(
			szVHDPath,//LPCTSTR szPath, 
			VIRTUAL_DISK_ACCESS_ALL,//VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, 
			ullDiskSize,//ULONGLONG MaximumSize, 
			szParent//LPCTSTR ParentPath
			);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Create"), E_FAIL));
			break;
		}

		//********************************************************************
		ATTACH_VIRTUAL_DISK_FLAG AttachFlags = ATTACH_VIRTUAL_DISK_FLAG_NO_DRIVE_LETTER | ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME;
		nStatus = vhdHelper.Attach(AttachFlags);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Attach"), E_FAIL));
			break;
		}
		bAttached = TRUE;

		//********************************************************************
		DWORD dwDiskNumber = vhdHelper.FindUninitalizedVHD();
		if ((DWORD)(-1) == dwDiskNumber)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("FindUninitalizedVHD"), E_FAIL));
			nStatus = E_FAIL;
			break;
		}
		_ftprintf_s(stdout, TEXT("*** ") TEXT("[VHDPath, SizeGB, DiskNumber]=[%s, %u, %u]") TEXT(" ***\r\n"), szVHDPath, -1, dwDiskNumber);

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
		vhdHelper.Dettach();
		bAttached = FALSE;
	}

	return nStatus;
}


INT VirtualDiskTestVds( LPCTSTR szVHDPath, ULONG nSizeGB )
{
	INT nStatus = 0;

	BOOL bAttached = FALSE;
	CVdsVhdHelper vhdHelper;

	do 
	{
		//******************************************
		nStatus = vhdHelper.CreateMember();
		if (nStatus)
		{
			//dwLastError = GetLastError();
			//_OLD_DSETLASTSTATUS(dwLastError);
			//nStatus = ___api_error_status;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateMember"), nStatus));
			break;
		}

		//********************************************************************
		ULONGLONG ullDiskSize = ((ULONGLONG)(nSizeGB))*1024*1024*1024;
		nStatus = vhdHelper.Create(
			szVHDPath,//LPCTSTR szPath, 
			VIRTUAL_DISK_ACCESS_ALL,//VIRTUAL_DISK_ACCESS_MASK VirtualDiskAccessMask, 
			ullDiskSize,//ULONGLONG MaximumSize, 
			NULL//LPCTSTR ParentPath
			);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Create"), E_FAIL));
			break;
		}

		//********************************************************************
		ATTACH_VIRTUAL_DISK_FLAG AttachFlags = ATTACH_VIRTUAL_DISK_FLAG_NO_DRIVE_LETTER | ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME;
		nStatus = vhdHelper.Attach(AttachFlags);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("CVirtualDiskHelper::Attach"), E_FAIL));
			break;
		}
		bAttached = TRUE;

		//********************************************************************
		DWORD dwDiskNumber = vhdHelper.FindUninitalizedVHD();
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
		vhdHelper.Dettach();
		bAttached = FALSE;
	}

	return nStatus;
}