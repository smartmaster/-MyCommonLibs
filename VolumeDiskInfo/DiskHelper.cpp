#include "stdafx.h"
#include "DiskHelper.h"


INT CDiskHelper::IOCTL_DISK_GET_DRIVE_GEOMETRY_EX_Helper( IN HANDLE hDevice, OUT DISK_GEOMETRY_EX ** ppDiskGeometryEx, OUT DISK_DETECTION_INFO ** ppDiskDetectionInfo, OUT DISK_PARTITION_INFO ** ppDiskPartitionInfo )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		BOOL bRet = FALSE;
		//********************************************************************
		CONST INT BUFFER_SIZE = 8*1024;
		CHeapPtr<BYTE> apBuffer;
		bRet = apBuffer.Allocate(BUFFER_SIZE);
		ZeroMemory((BYTE*)(apBuffer), BUFFER_SIZE);
		if (!bRet)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), dwLastError, TEXT("[BUFFER_SIZE]=[%u]"), BUFFER_SIZE);
			nStatus = dwLastError;
			break;
		}

		//********************************************************************
		DWORD BytesReturned = 0;
		bRet = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,                 // handle to volume
			IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,//IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, // dwIoControlCode
			NULL,//NULL,                             // lpInBuffer
			0,//0,                                // nInBufferSize
			(BYTE*)(apBuffer),//(LPVOID) lpOutBuffer,             // output buffer
			BUFFER_SIZE,//(DWORD) nOutBufferSize,           // size of output buffer
			&BytesReturned,//(LPDWORD) lpBytesReturned,        // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped       // OVERLAPPED structure
			);
		if (!bRet)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_GET_DRIVE_GEOMETRY_EX"), dwLastError, TEXT("")); /*0002*/
			nStatus = dwLastError;
			break;
		}

		//********************************************************************
		*ppDiskGeometryEx = (DISK_GEOMETRY_EX*)apBuffer.Detach();
		if (ppDiskDetectionInfo)
		{
			*ppDiskDetectionInfo = DiskGeometryGetDetect((*ppDiskGeometryEx));
		}
		if (ppDiskPartitionInfo)
		{
			*ppDiskPartitionInfo = DiskGeometryGetPartition((*ppDiskGeometryEx));
		}

	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_DISK_GET_DRIVE_LAYOUT_EX_Helper( IN HANDLE hDevice, OUT DRIVE_LAYOUT_INFORMATION_EX ** ppDriveLayoutInformationEx )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		CONST INT DELTA = sizeof(PARTITION_INFORMATION_EX) * 16;
		DWORD dwBufferSize = sizeof(DRIVE_LAYOUT_INFORMATION_EX) + DELTA;
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(dwBufferSize);
		ZeroMemory((BYTE*)(apBuffer), dwBufferSize);
		do 
		{
			DWORD dwBytesReturned = 0;
			BOOL bDeviceIoControl = DeviceIoControl(
				hDevice,//(HANDLE) hDevice,              // handle to volume
				IOCTL_DISK_GET_DRIVE_LAYOUT_EX,//IOCTL_DISK_GET_DRIVE_LAYOUT_EX,  // dwIoControlCode
				NULL,//(LPVOID) lpInBuffer,           // input buffer
				0,//(DWORD) nInBufferSize,         // size of input buffer
				(BYTE*)apBuffer,//(LPVOID) lpOutBuffer,          // output buffer
				dwBufferSize,//(DWORD) nOutBufferSize,        // size of output buffer
				&dwBytesReturned,//(LPDWORD) lpBytesReturned,     // number of bytes returned
				NULL//(LPOVERLAPPED) lpOverlapped  // OVERLAPPED structure
				);
			if (bDeviceIoControl)
			{
				*ppDriveLayoutInformationEx = (DRIVE_LAYOUT_INFORMATION_EX*)(apBuffer.Detach());
				break;
			}
			else
			{
				dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
				if (ERROR_INSUFFICIENT_BUFFER != dwLastError)
				{
					D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_GET_DRIVE_LAYOUT_EX"), dwLastError, TEXT("")); /*0002*/
					nStatus = dwLastError;
					break;
				}

				dwBufferSize += DELTA;
				apBuffer.Reallocate(dwBufferSize);
				ZeroMemory((BYTE*)(apBuffer), dwBufferSize);
			}

		} while (TRUE);
		if (nStatus)
		{
			break;
		}

		//******************************************************************************
	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_DISK_GET_PARTITION_INFO_EX_Helper( HANDLE hDevice, OUT PARTITION_INFORMATION_EX * pPartitionInformationEx )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,           // handle to device
			IOCTL_DISK_GET_PARTITION_INFO_EX,//FSCTL_GET_NTFS_VOLUME_DATA, // dwIoControlCode
			NULL,//(LPVOID) lpInBuffer,        // lpInBuffer
			0,//(DWORD) nInBufferSize,      // size of input buffer
			pPartitionInformationEx,//(LPVOID) lpOutBuffer,       // output buffer
			sizeof(PARTITION_INFORMATION_EX),//(DWORD) nOutBufferSize,     // size of output buffer
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,  // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped // OVERLAPPED structure
			);
		if (!bDeviceIoControl)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_GET_PARTITION_INFO_EX"), dwLastError, TEXT("")); /*0002*/
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;
}

HANDLE CDiskHelper::CreateHandle( DWORD dwDiskNumber, DWORD dwDesiredAccess )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HANDLE hDevice = INVALID_HANDLE_VALUE;

	CONST TCHAR DISK_NAME[] = TEXT("\\\\?\\PhysicalDrive%u");

	do 
	{
		//******************************************************************************
		CString strDisk;
		strDisk.Format(DISK_NAME, dwDiskNumber);
		hDevice = CDiskHelper::CreateHandle(strDisk, dwDesiredAccess);
		if (INVALID_HANDLE_VALUE == hDevice)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			D_API_ERR(999, /*DRNPOS,*/ TEXT("CreateFile"), dwLastError, TEXT("[Disk]=[%s]"), strDisk.GetString());  /*0007*/
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return hDevice;
}

HANDLE CDiskHelper::CreateHandle( IN LPCTSTR szDiskNumber, IN DWORD dwDesiredAccess )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HANDLE hDevice = INVALID_HANDLE_VALUE;

	do 
	{
		//******************************************************************************
		hDevice = CreateFile(
			szDiskNumber,//__in      LPCTSTR lpFileName,
			dwDesiredAccess,//__in      DWORD dwDesiredAccess,
			FILE_SHARE_READ | FILE_SHARE_WRITE,//__in      DWORD dwShareMode,
			NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_EXISTING,//__in      DWORD dwCreationDisposition,
			0,//__in      DWORD dwFlagsAndAttributes,
			NULL//__in_opt  HANDLE hTemplateFile
			);
		if (INVALID_HANDLE_VALUE == hDevice)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			D_API_ERR(999, /*DRNPOS,*/ TEXT("CreateFile"), dwLastError, TEXT("[Disk]=[%s]"),szDiskNumber);
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return hDevice;
}

INT CDiskHelper::IOCTL_DISK_GET_LENGTH_INFO_Helper( IN HANDLE hDevice, OUT GET_LENGTH_INFORMATION * pGetLengthInformation )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,           // handle to device
			IOCTL_DISK_GET_LENGTH_INFO,//FSCTL_GET_NTFS_VOLUME_DATA, // dwIoControlCode
			NULL,//(LPVOID) lpInBuffer,        // lpInBuffer
			0,//(DWORD) nInBufferSize,      // size of input buffer
			pGetLengthInformation,//(LPVOID) lpOutBuffer,       // output buffer
			sizeof(GET_LENGTH_INFORMATION),//(DWORD) nOutBufferSize,     // size of output buffer
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,  // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped // OVERLAPPED structure
			);
		if (!bDeviceIoControl)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_GET_LENGTH_INFO"), dwLastError, TEXT("")); /*0002*/
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_DEVICE_Helper( IN HANDLE hDevice, OUT STORAGE_DEVICE_DESCRIPTOR ** ppStorageDeviceDescriptor )
{
	INT nStatus = 0;
	DWORD dwLastError= 0 ;

	do 
	{
		//******************************************************************************
		DWORD dwBytesReturned = 0;
		STORAGE_PROPERTY_QUERY spq;
		ZeroMemory(&spq, sizeof(STORAGE_PROPERTY_QUERY));
		spq.PropertyId = StorageDeviceProperty;
		spq.QueryType = PropertyStandardQuery;

		DWORD dwBufferSize = 4*1024;
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(dwBufferSize);
		ZeroMemory((BYTE*)apBuffer, dwBufferSize);

		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//__in          HANDLE hDevice,
			IOCTL_STORAGE_QUERY_PROPERTY,//__in          DWORD dwIoControlCode,
			&spq,//__in          LPVOID lpInBuffer,
			sizeof(STORAGE_PROPERTY_QUERY),//__in          DWORD nInBufferSize,
			(BYTE*)apBuffer,//__out         LPVOID lpOutBuffer,
			dwBufferSize,//__in          DWORD nOutBufferSize,
			&dwBytesReturned,//__out         LPDWORD lpBytesReturned,
			NULL//__in          LPOVERLAPPED lpOverlapped
			);
		if (!bDeviceIoControl)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			nStatus = dwLastError;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("STORAGE_DEVICE_DESCRIPTOR"), dwLastError, TEXT("")); /*0002*/
			break;
		}

		*ppStorageDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)(apBuffer.Detach());

	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_ADAPTER_Helper( HANDLE hDevice, STORAGE_ADAPTER_DESCRIPTOR ** ppStorageAdapterDescriptor )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		STORAGE_PROPERTY_QUERY spq;
		ZeroMemory(&spq, sizeof(STORAGE_PROPERTY_QUERY));
		spq.PropertyId = StorageAdapterProperty;
		spq.QueryType = PropertyStandardQuery;

		DWORD dwBufferSize = 4*1024;
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(dwBufferSize);
		ZeroMemory((BYTE*)apBuffer, dwBufferSize);

		DWORD dwBytesReturned = 0;

		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//__in          HANDLE hDevice,
			IOCTL_STORAGE_QUERY_PROPERTY,//__in          DWORD dwIoControlCode,
			&spq,//__in          LPVOID lpInBuffer,
			sizeof(STORAGE_PROPERTY_QUERY),//__in          DWORD nInBufferSize,
			(BYTE*)apBuffer,//__out         LPVOID lpOutBuffer,
			dwBufferSize,//__in          DWORD nOutBufferSize,
			&dwBytesReturned,//__out         LPDWORD lpBytesReturned,
			NULL//__in          LPOVERLAPPED lpOverlapped
			);
		if (!bDeviceIoControl)
		{
			dwLastError =GetLastError();
			D_SET_LAST_STATUS(dwLastError, -1);
			nStatus = dwLastError;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("STORAGE_ADAPTER_DESCRIPTOR"), dwLastError, TEXT("")); /*0002*/
			break;
		}

		*ppStorageAdapterDescriptor = (STORAGE_ADAPTER_DESCRIPTOR*)(apBuffer.Detach());

	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_UNIQUE_ID_Helper( HANDLE hDevice, STORAGE_DEVICE_ID_DESCRIPTOR ** ppStorageDeviceIDDescriptor )
{
	INT nStatus = 0;

	do 
	{
		//******************************************************************************
		STORAGE_PROPERTY_QUERY spq;
		ZeroMemory(&spq, sizeof(STORAGE_PROPERTY_QUERY));
		spq.PropertyId =  StorageDeviceUniqueIdProperty;
		spq.QueryType = PropertyStandardQuery;

		DWORD dwBufferSize = 1024*4;
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(dwBufferSize);
		ZeroMemory((BYTE*)apBuffer, dwBufferSize);

		DWORD dwBytesReturned = 0;

		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//__in          HANDLE hDevice,
			IOCTL_STORAGE_QUERY_PROPERTY,//__in          DWORD dwIoControlCode,
			&spq,//__in          LPVOID lpInBuffer,
			sizeof(STORAGE_PROPERTY_QUERY),//__in          DWORD nInBufferSize,
			(BYTE*)apBuffer,//__out         LPVOID lpOutBuffer,
			dwBufferSize,//__in          DWORD nOutBufferSize,
			&dwBytesReturned,//__out         LPDWORD lpBytesReturned,
			NULL//__in          LPOVERLAPPED lpOverlapped
			);
		if (!bDeviceIoControl)
		{
			DWORD dwLastError =GetLastError();
			D_SET_LAST_STATUS(dwLastError, -1);
			nStatus = dwLastError;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("STORAGE_DEVICE_ID_DESCRIPTOR"), dwLastError, TEXT("")); /*0002*/
			break;
		}

		*ppStorageDeviceIDDescriptor = (STORAGE_DEVICE_ID_DESCRIPTOR*)(apBuffer.Detach());

	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_STORAGE_GET_DEVICE_NUMBER_Helper( HANDLE hDevice, STORAGE_DEVICE_NUMBER * pStorageDeviceNumber )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,           // handle to device
			IOCTL_STORAGE_GET_DEVICE_NUMBER,//FSCTL_GET_NTFS_VOLUME_DATA, // dwIoControlCode
			NULL,//(LPVOID) lpInBuffer,        // lpInBuffer
			0,//(DWORD) nInBufferSize,      // size of input buffer
			pStorageDeviceNumber,//(LPVOID) lpOutBuffer,       // output buffer
			sizeof(STORAGE_DEVICE_NUMBER),//(DWORD) nOutBufferSize,     // size of output buffer
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,  // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped // OVERLAPPED structure
			);
		if (!bDeviceIoControl)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_STORAGE_GET_DEVICE_NUMBER"), dwLastError, TEXT("")); /*0002*/
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_STORAGE_GET_HOTPLUG_INFO_Helper( HANDLE hDevice, STORAGE_HOTPLUG_INFO * pStorageHotplugInfo )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,           // handle to device
			IOCTL_STORAGE_GET_HOTPLUG_INFO,//FSCTL_GET_NTFS_VOLUME_DATA, // dwIoControlCode
			NULL,//(LPVOID) lpInBuffer,        // lpInBuffer
			0,//(DWORD) nInBufferSize,      // size of input buffer
			pStorageHotplugInfo,//(LPVOID) lpOutBuffer,       // output buffer
			sizeof(STORAGE_HOTPLUG_INFO),//(DWORD) nOutBufferSize,     // size of output buffer
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,  // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped // OVERLAPPED structure
			);
		if (!bDeviceIoControl)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_STORAGE_GET_HOTPLUG_INFO"), dwLastError, TEXT("")); /*0002*/
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_STORAGE_GET_MEDIA_TYPES_EX_Helper( HANDLE hDevice, GET_MEDIA_TYPES ** ppMEDIA_TYPES )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************************************************
		CONST INT DELTA = sizeof(DEVICE_MEDIA_INFO) * 16;
		DWORD dwBufferSize = sizeof(GET_MEDIA_TYPES) + DELTA;
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(dwBufferSize);
		ZeroMemory((BYTE*)apBuffer, dwBufferSize);

		do 
		{
			DWORD dwBytesReturned = 0;

			BOOL bDeviceIoControl = DeviceIoControl(
				hDevice,//(HANDLE) hDevice,              // handle to volume
				IOCTL_STORAGE_GET_MEDIA_TYPES_EX,//FSCTL_GET_RETRIEVAL_POINTERS,  // dwIoControlCode
				NULL,//(LPVOID) lpInBuffer,           // input buffer
				0,//(DWORD) nInBufferSize,         // size of input buffer
				(BYTE*)apBuffer,//(LPVOID) lpOutBuffer,          // output buffer
				dwBufferSize,//(DWORD) nOutBufferSize,        // size of output buffer
				&dwBytesReturned,//(LPDWORD) lpBytesReturned,     // number of bytes returned
				NULL//(LPOVERLAPPED) lpOverlapped  // OVERLAPPED structure
				);
			if (bDeviceIoControl)
			{
				*ppMEDIA_TYPES = (GET_MEDIA_TYPES*)(apBuffer.Detach());
				break;
			}
			else
			{
				dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
				if (ERROR_INSUFFICIENT_BUFFER != dwLastError && ERROR_MORE_DATA != dwLastError)
				{
					D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_STORAGE_GET_MEDIA_TYPES_EX"), dwLastError, TEXT("")); /*0002*/
					nStatus = dwLastError;
					break;
				}

				dwBufferSize += DELTA;
				apBuffer.Reallocate(dwBufferSize);
				ZeroMemory((BYTE*)apBuffer, dwBufferSize);
			}

		} while (TRUE);
		if (nStatus)
		{
			break;
		}

		//******************************************************************************
	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_ID_Helper( HANDLE hDevice, STORAGE_DEVICE_ID_DESCRIPTOR ** ppStorageDeviceIDDescriptor )
{
	INT nStatus = 0;

	do 
	{
		//******************************************************************************
		STORAGE_PROPERTY_QUERY spq;
		ZeroMemory(&spq, sizeof(STORAGE_PROPERTY_QUERY));
		spq.PropertyId =  StorageDeviceIdProperty;
		spq.QueryType = PropertyStandardQuery;

		DWORD dwBufferSize = 1024*4;
		CHeapPtr<BYTE> apBuffer;
		apBuffer.Allocate(dwBufferSize);
		ZeroMemory((BYTE*)apBuffer, dwBufferSize);

		DWORD dwBytesReturned = 0;

		BOOL bDeviceIoControl = DeviceIoControl(
			hDevice,//__in          HANDLE hDevice,
			IOCTL_STORAGE_QUERY_PROPERTY,//__in          DWORD dwIoControlCode,
			&spq,//__in          LPVOID lpInBuffer,
			sizeof(STORAGE_PROPERTY_QUERY),//__in          DWORD nInBufferSize,
			(BYTE*)apBuffer,//__out         LPVOID lpOutBuffer,
			dwBufferSize,//__in          DWORD nOutBufferSize,
			&dwBytesReturned,//__out         LPDWORD lpBytesReturned,
			NULL//__in          LPOVERLAPPED lpOverlapped
			);
		if (!bDeviceIoControl)
		{
			DWORD dwLastError =GetLastError();
			D_SET_LAST_STATUS(dwLastError, -1);
			nStatus = dwLastError;
			D_API_ERR(999, /*DRNPOS,*/ TEXT("STORAGE_DEVICE_ID_DESCRIPTOR"), dwLastError, TEXT("")); /*0002*/
			break;
		}

		*ppStorageDeviceIDDescriptor = (STORAGE_DEVICE_ID_DESCRIPTOR*)(apBuffer.Detach());

	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_DISK_CREATE_DISK_GPT_Helper( HANDLE hDevice, GUID& diskGUID )
{
	INT nStatus = 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//********************************************************************
		CREATE_DISK CreateDisk;
		CreateDisk.PartitionStyle = PARTITION_STYLE_GPT;
		CreateDisk.Gpt.MaxPartitionCount = 128;
		hr = CoCreateGuid(&CreateDisk.Gpt.DiskId);
		if (FAILED(hr))
		{
			D_API_ERR(0, /*DRNPOS,*/ TEXT("CoCreateGuid"), hr, TEXT("")); /*0004*/
			nStatus = hr;
			break;
		}
		CopyMemory(&diskGUID, &CreateDisk.Gpt.DiskId, sizeof(GUID));

		//********************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bRet = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,              // handle to device
			IOCTL_DISK_CREATE_DISK,//IOCTL_DISK_CREATE_DISK,        // dwIoControlCode
			&CreateDisk,//(LPVOID) lpInBuffer,           // input buffer
			sizeof(CREATE_DISK),//(DWORD) nInBufferSize,         // size of input buffer
			NULL,//NULL,                          // lpOutBuffer
			0, //0,                             // nOutBufferSize
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,     // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped    // OVERLAPPED structure
			);
		if (!bRet)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_CREATE_DISK"), dwLastError, TEXT("")); /*0002*/
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::IOCTL_DISK_SET_DRIVE_LAYOUT_EX_Helper( HANDLE hDevice, PDRIVE_LAYOUT_INFORMATION_EX pDriveLayoutInformationEx )
{
	INT nStatus = 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//********************************************************************
		DWORD dwBytesReturned = 0;
		BOOL bRet = DeviceIoControl(
			hDevice,//(HANDLE) hDevice,              // handle to device
			IOCTL_DISK_SET_DRIVE_LAYOUT_EX,//IOCTL_DISK_CREATE_DISK,        // dwIoControlCode
			pDriveLayoutInformationEx,//(LPVOID) lpInBuffer,           // input buffer
			sizeof(DRIVE_LAYOUT_INFORMATION_EX) + (pDriveLayoutInformationEx->PartitionCount-1)*sizeof(PARTITION_INFORMATION_EX),//(DWORD) nInBufferSize,         // size of input buffer
			NULL,//NULL,                          // lpOutBuffer
			0, //0,                             // nOutBufferSize
			&dwBytesReturned,//(LPDWORD) lpBytesReturned,     // number of bytes returned
			NULL//(LPOVERLAPPED) lpOverlapped    // OVERLAPPED structure
			);
		if (!bRet)
		{
			dwLastError = GetLastError(); D_SET_LAST_STATUS(dwLastError, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_SET_DRIVE_LAYOUT_EX"), dwLastError, TEXT("")); /*0002*/
			nStatus = dwLastError;
			break;
		}

	} while (FALSE);

	return nStatus;
}

INT CDiskHelper::GetDriveLayoutFromTemplateGPT( IN PDRIVE_LAYOUT_INFORMATION_EX pDriveLayoutInformationTemplate, OUT PDRIVE_LAYOUT_INFORMATION_EX pDriveLayoutInformation, GUID& partitionId )
{
	CopyMemory(pDriveLayoutInformation, pDriveLayoutInformationTemplate, sizeof(DRIVE_LAYOUT_INFORMATION_EX));

	CONST LONGLONG STARTING_OFFSET_ALIGHN = ((LONGLONG)16)*1024*1024;
	CONST LONGLONG STARTING_RESERVED_SPACE = ((LONGLONG)8)*1024*1024;
	CONST LONGLONG ENDING_OFFSET_ALIGHN = ((LONGLONG)16)*1024*1024;
	CONST LONGLONG ENDING_RESERVED_SPACE = ((LONGLONG)8)*1024*1024;
	LONGLONG llStartingOffset = AtlAlignUp((pDriveLayoutInformationTemplate->Gpt.StartingUsableOffset.QuadPart + STARTING_RESERVED_SPACE), STARTING_OFFSET_ALIGHN);
	LONGLONG llEndingOffset = AtlAlignDown((pDriveLayoutInformationTemplate->Gpt.StartingUsableOffset.QuadPart + pDriveLayoutInformationTemplate->Gpt.UsableLength.QuadPart - ENDING_RESERVED_SPACE), ENDING_OFFSET_ALIGHN);

	pDriveLayoutInformation->PartitionCount = 1;
	pDriveLayoutInformation->PartitionEntry[0].PartitionStyle = (PARTITION_STYLE)(pDriveLayoutInformationTemplate->PartitionStyle);
	pDriveLayoutInformation->PartitionEntry[0].StartingOffset.QuadPart = llStartingOffset;
	pDriveLayoutInformation->PartitionEntry[0].PartitionLength.QuadPart = llEndingOffset - llStartingOffset;
	pDriveLayoutInformation->PartitionEntry[0].PartitionNumber = 1;

	CONST TCHAR szPUBasicDataGuid[] = TEXT("{ebd0a0a2-b9e5-4433-87c0-68b6b72699c7}");
	CONST TCHAR szPUBasicDataGuidName[] = TEXT("Basic data partition");
	CLSIDFromString(szPUBasicDataGuid, &pDriveLayoutInformation->PartitionEntry[0].Gpt.PartitionType);
	CoCreateGuid(&(pDriveLayoutInformation->PartitionEntry[0].Gpt.PartitionId));
	CopyMemory(&partitionId, &(pDriveLayoutInformation->PartitionEntry[0].Gpt.PartitionId), sizeof(GUID));
	_tcscpy_s(pDriveLayoutInformation->PartitionEntry[0].Gpt.Name, 
		_countof(pDriveLayoutInformation->PartitionEntry[0].Gpt.Name), 
		szPUBasicDataGuidName);

	return 0;
}

INT TestDisk( DWORD dwDiskNumber )
{
	D_START(0);
	INT nStatus = 0;

	do 
	{
		//********************************************************************
		HANDLE hDeviceRaw = CDiskHelper::CreateHandle(dwDiskNumber, GENERIC_READ);
		if (INVALID_HANDLE_VALUE == hDeviceRaw)
		{
			break;
		}
		CHandle hDevice(hDeviceRaw); hDeviceRaw = INVALID_HANDLE_VALUE;

		//********************************************
		STORAGE_DEVICE_ID_DESCRIPTOR * pStorageDeviceIDDescriptorM = NULL;
		nStatus = CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_ID_Helper( hDevice, &pStorageDeviceIDDescriptorM );
		{
			CHeapPtr<BYTE> apBufferTemp((BYTE*)pStorageDeviceIDDescriptorM);
			pStorageDeviceIDDescriptorM = NULL;
		}

		//********************************************************************
		DISK_GEOMETRY_EX * pDiskGeometryEx = NULL;
		DISK_DETECTION_INFO * pDiskDetectionInfo = NULL;
		DISK_PARTITION_INFO * pDiskPartitionInfo = NULL;
		nStatus = CDiskHelper::IOCTL_DISK_GET_DRIVE_GEOMETRY_EX_Helper(
			hDevice,//IN HANDLE hDevice, 
			&pDiskGeometryEx,//OUT DISK_GEOMETRY_EX ** ppDiskGeometryEx,
			&pDiskDetectionInfo,//OUT DISK_DETECTION_INFO ** ppDiskDetectionInfo,
			&pDiskPartitionInfo//OUT DISK_PARTITION_INFO ** ppDiskPartitionInfo
			);
		{
			CHeapPtr<BYTE> apBufferTemp((BYTE*)pDiskGeometryEx);
			pDiskGeometryEx = NULL;
			pDiskDetectionInfo = NULL;
			pDiskPartitionInfo = NULL;
		}

		//********************************************************************
		DRIVE_LAYOUT_INFORMATION_EX * pDriveLayoutInformationEx = NULL;
		nStatus = CDiskHelper::IOCTL_DISK_GET_DRIVE_LAYOUT_EX_Helper(
			hDevice,//IN HANDLE hDevice,
			&pDriveLayoutInformationEx//OUT DRIVE_LAYOUT_INFORMATION_EX ** ppDriveLayoutInformationEx
			);
		{
			CHeapPtr<BYTE> apBufferTemp((BYTE*)pDriveLayoutInformationEx);
			pDriveLayoutInformationEx = NULL;
		}

		//********************************************************************
		PARTITION_INFORMATION_EX PartitionInformationEx;
		ZeroMemory(&PartitionInformationEx, sizeof(PARTITION_INFORMATION_EX));
		nStatus = CDiskHelper::IOCTL_DISK_GET_PARTITION_INFO_EX_Helper(
			hDevice,//IN HANDLE hDevice, 
			&PartitionInformationEx//OUT PARTITION_INFORMATION_EX * pPartitionInformationEx
			);

		//********************************************
		GET_LENGTH_INFORMATION  GetLengthInformation = {0};
		nStatus = CDiskHelper::IOCTL_DISK_GET_LENGTH_INFO_Helper(
			hDevice,//IN HANDLE hDevice, 
			&GetLengthInformation//OUT GET_LENGTH_INFORMATION * pGetLengthInformation
			);

		//********************************************
		STORAGE_DEVICE_DESCRIPTOR * pStorageDeviceDescriptor = NULL;
		nStatus = CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_DEVICE_Helper( hDevice, &pStorageDeviceDescriptor);
		{
			CHeapPtr<BYTE> apBufferTemp((BYTE*)pStorageDeviceDescriptor);
			pStorageDeviceDescriptor = NULL;
		}

		//********************************************
		STORAGE_ADAPTER_DESCRIPTOR * pStorageAdapterDescriptor = NULL;
		nStatus = CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_ADAPTER_Helper( hDevice, &pStorageAdapterDescriptor );
		{
			CHeapPtr<BYTE> apBufferTemp((BYTE*)pStorageAdapterDescriptor);
			pStorageAdapterDescriptor = NULL;
		}

		//********************************************
		STORAGE_DEVICE_ID_DESCRIPTOR * pStorageDeviceIDDescriptor = NULL;
		nStatus = CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_UNIQUE_ID_Helper(
			hDevice,//HANDLE hDevice, 
			&pStorageDeviceIDDescriptor//STORAGE_DEVICE_ID_DESCRIPTOR ** ppStorageDeviceIDDescriptor
			);
		{
			CHeapPtr<BYTE> apBufferTemp((BYTE*)pStorageDeviceIDDescriptor);
			pStorageDeviceIDDescriptor = NULL;
		}

		//*******************************************
		STORAGE_DEVICE_NUMBER StorageDeviceNumber = {0};
		nStatus = CDiskHelper::IOCTL_STORAGE_GET_DEVICE_NUMBER_Helper(
			hDevice,//HANDLE hDevice, 
			&StorageDeviceNumber//STORAGE_DEVICE_NUMBER * pStorageDeviceNumber
			);

		//********************************************
		STORAGE_HOTPLUG_INFO StorageHotplugInfo = {0};
		nStatus = CDiskHelper::IOCTL_STORAGE_GET_HOTPLUG_INFO_Helper(
			hDevice,//HANDLE hDevice, 
			&StorageHotplugInfo//STORAGE_HOTPLUG_INFO * pStorageHotplugInfo
			);

		//********************************************
		GET_MEDIA_TYPES * pMEDIA_TYPES = NULL;
		nStatus = CDiskHelper::IOCTL_STORAGE_GET_MEDIA_TYPES_EX_Helper(
			hDevice,//HANDLE hDevice,  
			&pMEDIA_TYPES //GET_MEDIA_TYPES ** ppMEDIA_TYPES
			);
		{
			CHeapPtr<BYTE> apBufferTemp((BYTE*)pMEDIA_TYPES);
			pMEDIA_TYPES = NULL;
		}


		//********************************************************************
	} while (FALSE);

	return nStatus;
}

INT TestDiskMain( INT argc, _TCHAR* argv[] )
{
	CCmdLineTestDiskMain cmdLine;
	cmdLine.Parse(argc, argv);
	for (size_t i=0; i<cmdLine.m_vecDiskNumber.size(); ++i)
	{
		TestDisk(cmdLine.m_vecDiskNumber[i]);
	}
	return 0;
}
