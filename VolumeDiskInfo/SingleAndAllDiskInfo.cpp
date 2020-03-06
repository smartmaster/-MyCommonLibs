#include "stdafx.h"
#include "SingleAndAllDiskInfo.h"
#include "MiscHelper.h"

static CONST LPCTSTR szStreamNamesOfCSingleDiskInfo[] = 
{
	TEXT("m_DiskNumber"),
	TEXT("m_pDiskGeometryEx"),
	TEXT("m_pDriveLayoutInformationEx"),
	TEXT("m_PartitionInformationEx"),
	TEXT("m_GetLengthInformation"),
	TEXT("m_pStorageDeviceDescriptor"),
	TEXT("m_pStorageAdapterDescriptor"),
	TEXT("m_pStorageDeviceIDDescriptor"),
	TEXT("m_pStorageDeviceUniqIDDscr"),
	TEXT("m_StorageDeviceNumber"),
	TEXT("m_StorageHotplugInfo"),
	TEXT("m_pMEDIA_TYPES")
};

CSingleDiskInfo::CSingleDiskInfo()
{
	m_DiskNumber = -1;
	m_pDiskGeometryEx = NULL;
	m_pDriveLayoutInformationEx = NULL;
	ZeroMemory(&m_PartitionInformationEx, sizeof(PARTITION_INFORMATION_EX));;
	ZeroMemory(&m_GetLengthInformation, sizeof(GET_LENGTH_INFORMATION));
	m_pStorageDeviceDescriptor = NULL;
	m_pStorageAdapterDescriptor = NULL;
	m_pStorageDeviceIDDescriptor = NULL;
	m_pStorageDeviceUniqueIDDescriptor = NULL;
	ZeroMemory(&m_StorageDeviceNumber, sizeof(STORAGE_DEVICE_NUMBER));
	ZeroMemory(&m_StorageHotplugInfo, sizeof(STORAGE_HOTPLUG_INFO));
	m_pMEDIA_TYPES = NULL;
}

CSingleDiskInfo::~CSingleDiskInfo()
{
	m_DiskNumber = -1;

	{
		CHeapPtr<BYTE> apBufferCleanup((BYTE*)(m_pDiskGeometryEx));
		m_pDiskGeometryEx = NULL;
	}

	{
		CHeapPtr<BYTE> apBufferCleanup((BYTE*)(m_pDriveLayoutInformationEx ));
		m_pDriveLayoutInformationEx  = NULL;
	}

	ZeroMemory(&m_PartitionInformationEx, sizeof(PARTITION_INFORMATION_EX));;
	ZeroMemory(&m_GetLengthInformation, sizeof(GET_LENGTH_INFORMATION));

	{
		CHeapPtr<BYTE> apBufferCleanup((BYTE*)(m_pStorageDeviceDescriptor ));
		m_pStorageDeviceDescriptor  = NULL;
	}

	{
		CHeapPtr<BYTE> apBufferCleanup((BYTE*)(m_pStorageAdapterDescriptor ));
		m_pStorageAdapterDescriptor = NULL;
	}

	{
		CHeapPtr<BYTE> apBufferCleanup((BYTE*)(m_pStorageDeviceIDDescriptor ));
		m_pStorageDeviceIDDescriptor = NULL;
	}

	{
		CHeapPtr<BYTE> apBufferCleanup((BYTE*)(m_pStorageDeviceUniqueIDDescriptor ));
		m_pStorageDeviceUniqueIDDescriptor = NULL;
	}

	ZeroMemory(&m_StorageDeviceNumber, sizeof(STORAGE_DEVICE_NUMBER));
	ZeroMemory(&m_StorageHotplugInfo, sizeof(STORAGE_HOTPLUG_INFO));

	{
		CHeapPtr<BYTE> apBufferCleanup((BYTE*)(m_pMEDIA_TYPES ));
		m_pMEDIA_TYPES = NULL;
	}
}

INT CSingleDiskInfo::Retrieve( DWORD dwDiskNumber, BOOL bBreakOnError )
{
	D_START(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		m_DiskNumber = dwDiskNumber;

		//******************************************
		HANDLE hDeviceRaw = CDiskHelper::CreateHandle(
			m_DiskNumber,//IN DWORD dwDiskNumber, 
			GENERIC_READ//IN DWORD dwDesiredAccess
			);
		if (INVALID_HANDLE_VALUE == hDeviceRaw)
		{
			nStatus = E_FAIL;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("CreateHandle"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}
		CHandle hDevice(hDeviceRaw); hDeviceRaw = INVALID_HANDLE_VALUE;

		//******************************************
		DISK_DETECTION_INFO * pDiskDetectionInfo = NULL;
		DISK_PARTITION_INFO * pDiskPartitionInfo = NULL;
		nStatus = CDiskHelper::IOCTL_DISK_GET_DRIVE_GEOMETRY_EX_Helper(
			hDevice,//IN HANDLE hDevice, 
			&m_pDiskGeometryEx,//OUT DISK_GEOMETRY_EX ** ppDiskGeometryEx,
			&pDiskDetectionInfo,//OUT DISK_DETECTION_INFO ** ppDiskDetectionInfo,
			&pDiskPartitionInfo//OUT DISK_PARTITION_INFO ** ppDiskPartitionInfo
			);
		if (nStatus)
		{
			//nStatus = -1;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_GET_DRIVE_GEOMETRY_EX_Helper"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		//******************************************
		nStatus = CDiskHelper::IOCTL_DISK_GET_DRIVE_LAYOUT_EX_Helper(
			hDevice,//IN HANDLE hDevice,
			&m_pDriveLayoutInformationEx//OUT DRIVE_LAYOUT_INFORMATION_EX ** ppDriveLayoutInformationEx
			);
		if (nStatus)
		{
			//nStatus = -1;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_GET_DRIVE_LAYOUT_EX_Helper"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		//******************************************
		nStatus = CDiskHelper::IOCTL_DISK_GET_PARTITION_INFO_EX_Helper(
			hDevice,//IN HANDLE hDevice, 
			&m_PartitionInformationEx//OUT PARTITION_INFORMATION_EX * pPartitionInformationEx
			);
		if (nStatus)
		{
			//nStatus = -1;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_GET_PARTITION_INFO_EX_Helper"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		//******************************************
		nStatus = CDiskHelper::IOCTL_DISK_GET_LENGTH_INFO_Helper(
			hDevice,//IN HANDLE hDevice, 
			&m_GetLengthInformation//OUT GET_LENGTH_INFORMATION * pGetLengthInformation
			);
		if (nStatus)
		{
			//nStatus = -1;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_GET_LENGTH_INFO_Helper"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		//******************************************
		nStatus = CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_DEVICE_Helper(
			hDevice,//IN HANDLE hDevice, 
			&m_pStorageDeviceDescriptor//OUT STORAGE_DEVICE_DESCRIPTOR ** ppStorageDeviceDescriptor
			);
		if (nStatus)
		{
			//nStatus = -1;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_STORAGE_QUERY_PROPERTY_DEVICE_Helper"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		//******************************************
		nStatus = CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_ADAPTER_Helper(
			hDevice,//HANDLE hDevice, 
			&m_pStorageAdapterDescriptor//STORAGE_ADAPTER_DESCRIPTOR ** ppStorageAdapterDescriptor
			);
		if (nStatus)
		{
			//nStatus = -1;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_STORAGE_QUERY_PROPERTY_ADAPTER_Helper"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		//******************************************
		nStatus = CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_ID_Helper(
			hDevice,//HANDLE hDevice, 
			&m_pStorageDeviceIDDescriptor//STORAGE_DEVICE_ID_DESCRIPTOR ** ppStorageDeviceIDDescriptor
			);
		if (nStatus)
		{
			nStatus = 0; //always ignore this error
			D_API_ERR(999, /*DRNPOS,*/ TEXT("IOCTL_STORAGE_QUERY_PROPERTY_ID_Helper"), nStatus, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			//break;
		}

		//******************************************
		nStatus = CDiskHelper::IOCTL_STORAGE_QUERY_PROPERTY_UNIQUE_ID_Helper(
			hDevice,//HANDLE hDevice, 
			&m_pStorageDeviceUniqueIDDescriptor//STORAGE_DEVICE_ID_DESCRIPTOR ** ppStorageDeviceIDDescriptor
			);
		if (nStatus)
		{
			//nStatus = -1;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_STORAGE_QUERY_PROPERTY_UNIQUE_ID_Helper"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		//******************************************
		nStatus = CDiskHelper::IOCTL_STORAGE_GET_DEVICE_NUMBER_Helper(
			hDevice,//HANDLE hDevice, 
			&m_StorageDeviceNumber//STORAGE_DEVICE_NUMBER * pStorageDeviceNumber
			);
		if (nStatus)
		{
			//nStatus = -1;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_STORAGE_GET_DEVICE_NUMBER_Helper"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}


		//******************************************
		nStatus = CDiskHelper::IOCTL_STORAGE_GET_HOTPLUG_INFO_Helper(
			hDevice,//HANDLE hDevice, 
			&m_StorageHotplugInfo//STORAGE_HOTPLUG_INFO * pStorageHotplugInfo
			);
		if (nStatus)
		{
			//nStatus = -1;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_STORAGE_GET_HOTPLUG_INFO_Helper"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		//******************************************
		nStatus = CDiskHelper::IOCTL_STORAGE_GET_MEDIA_TYPES_EX_Helper(
			hDevice,//HANDLE hDevice,  
			&m_pMEDIA_TYPES//GET_MEDIA_TYPES ** ppMEDIA_TYPES
			);
		if (nStatus)
		{
			//nStatus = -1;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_STORAGE_GET_MEDIA_TYPES_EX_Helper"), E_FAIL, TEXT("m_DiskNumber=[%u]"), m_DiskNumber); /*0005*/
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CSingleDiskInfo::WriteToStorage( IStorage * pIStorage )
{
	D_START(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfCSingleDiskInfo); ++ii)
	{
		CComPtr<IStream> spIStream;
		hr = pIStorage->CreateStream(
			szStreamNamesOfCSingleDiskInfo[ii],//[in]                 const WCHAR* pwcsName,
			STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("CreateStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
			nStatus = hr;
			break;
		}

		//******************************************
		CComVariant var;
		if (0 == ii)
		{
			var = (ULONG)m_DiskNumber;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		else if (1 == ii)
		{
			ULONG cb = 0;
			if (m_pDiskGeometryEx)
			{
				cb = sizeof(DISK_GEOMETRY_EX);
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					m_pDiskGeometryEx,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (2 == ii)
		{
			ULONG cb = 0;
			if (m_pDriveLayoutInformationEx)
			{
				cb = sizeof(DRIVE_LAYOUT_INFORMATION_EX) + (m_pDriveLayoutInformationEx->PartitionCount - 1)*sizeof(PARTITION_INFORMATION_EX);
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					m_pDriveLayoutInformationEx,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (3 == ii)
		{
			ULONG cb = sizeof(m_PartitionInformationEx);
			ULONG cbWritten = 0;
			hr = spIStream->Write(
				&m_PartitionInformationEx,//[in]                 void const* pv,
				cb,//[in]                 ULONG cb,
				&cbWritten//[out]                ULONG* pcbWritten
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		else if (4 == ii)
		{
			ULONG cb = sizeof(m_GetLengthInformation);
			ULONG cbWritten = 0;
			hr = spIStream->Write(
				&m_GetLengthInformation,//[in]                 void const* pv,
				cb,//[in]                 ULONG cb,
				&cbWritten//[out]                ULONG* pcbWritten
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		else if (5 == ii)
		{
			ULONG cb = 0;
			if (m_pStorageDeviceDescriptor)
			{
				cb = sizeof(STORAGE_DEVICE_DESCRIPTOR);
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					m_pStorageDeviceDescriptor,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (6 == ii)
		{
			ULONG cb = 0;
			if (m_pStorageAdapterDescriptor)
			{
				cb = sizeof(STORAGE_ADAPTER_DESCRIPTOR);
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					m_pStorageAdapterDescriptor,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (7 == ii)
		{
			ULONG cb = 0;
			if (m_pStorageDeviceIDDescriptor)
			{
				cb = sizeof(STORAGE_DEVICE_ID_DESCRIPTOR);
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					m_pStorageDeviceIDDescriptor,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (8 == ii)
		{
			ULONG cb = 0;
			if (m_pStorageDeviceUniqueIDDescriptor)
			{
				cb = sizeof(STORAGE_DEVICE_ID_DESCRIPTOR);
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					m_pStorageDeviceUniqueIDDescriptor,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (9 == ii)
		{
			ULONG cb = 0;
			//if (m_StorageDeviceNumber)
			{
				cb = sizeof(STORAGE_DEVICE_NUMBER);
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					&m_StorageDeviceNumber,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (10 == ii)
		{
			ULONG cb = 0;
			//if (m_StorageHotplugInfo)
			{
				cb = sizeof(STORAGE_HOTPLUG_INFO);
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					&m_StorageHotplugInfo,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (11 == ii)
		{
			ULONG cb = 0;
			if (m_pMEDIA_TYPES)
			{
				cb = sizeof(GET_MEDIA_TYPES) + sizeof(DEVICE_MEDIA_INFO)*(m_pMEDIA_TYPES->MediaInfoCount - 1);
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					m_pMEDIA_TYPES,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
	}

	return nStatus;
}

INT CSingleDiskInfo::ReadFromStorage( IStorage * pIStorage )
{
	D_START(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfCSingleDiskInfo); ++ii)
	{
		CComPtr<IStream> spIStream;
		hr = pIStorage->OpenStream(
			szStreamNamesOfCSingleDiskInfo[ii],//[in]                 const WCHAR* pwcsName,
			NULL,//[in]                 void* reserved1,
			STGM_READ | STGM_SHARE_EXCLUSIVE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("OpenStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
			nStatus = hr;
			break;
		}

		//******************************************
		CComVariant var;
		if (0 == ii)
		{
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (VT_UI4 != var.vt)
			{
				D_SET_LAST_STATUS(E_INVALIDARG, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("VT_UI4 != var.vt"), E_INVALIDARG, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = E_INVALIDARG;
				break;
			}

			m_DiskNumber = var.ulVal;
		}
		else if (1 == ii)
		{
			ULONG cb = 0;
			STATSTG statstg = {0};
			hr = spIStream->Stat(
				&statstg,//[out]                STATSTG* pstatstg,
				STATFLAG_NONAME//[in]                 DWORD grfStatFlag
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (0 == statstg.cbSize.LowPart)
			{
				continue;
			}

			CHeapPtr<BYTE> apBuffer;
			BOOL bRet = apBuffer.Allocate(statstg.cbSize.LowPart);
			if (!bRet)
			{
				D_SET_LAST_STATUS(E_OUTOFMEMORY, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), E_OUTOFMEMORY, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = E_OUTOFMEMORY;
				break;
			}

			ULONG cbRead = 0;
			hr = spIStream->Read((BYTE*)apBuffer, statstg.cbSize.LowPart, &cbRead);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			m_pDiskGeometryEx = (DISK_GEOMETRY_EX*)apBuffer.Detach();
		}
		else if (2 == ii)
		{
			ULONG cb = 0;
			STATSTG statstg = {0};
			hr = spIStream->Stat(
				&statstg,//[out]                STATSTG* pstatstg,
				STATFLAG_NONAME//[in]                 DWORD grfStatFlag
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (0 == statstg.cbSize.LowPart)
			{
				continue;
			}

			CHeapPtr<BYTE> apBuffer;
			BOOL bRet = apBuffer.Allocate(statstg.cbSize.LowPart);
			if (!bRet)
			{
				D_SET_LAST_STATUS(E_OUTOFMEMORY, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), E_OUTOFMEMORY, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = E_OUTOFMEMORY;
				break;
			}

			ULONG cbRead = 0;
			hr = spIStream->Read((BYTE*)apBuffer, statstg.cbSize.LowPart, &cbRead);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			m_pDriveLayoutInformationEx = (DRIVE_LAYOUT_INFORMATION_EX*)apBuffer.Detach();
		}
		else if (3 == ii)
		{
			ULONG cb = sizeof(PARTITION_INFORMATION_EX);
			ULONG cbRead = 0;
			hr = spIStream->Read(
				&m_PartitionInformationEx,//[in]                 void const* pv,
				cb,//[in]                 ULONG cb,
				&cbRead//[out]                ULONG* pcbWritten
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		else if (4 == ii)
		{
			ULONG cb = sizeof(m_GetLengthInformation);
			ULONG cbRead = 0;
			hr = spIStream->Read(
				&m_GetLengthInformation,//[in]                 void const* pv,
				cb,//[in]                 ULONG cb,
				&cbRead//[out]                ULONG* pcbWritten
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		else if (5 == ii)
		{
			//if (m_pStorageDeviceDescriptor)
			ULONG cb = 0;
			STATSTG statstg = {0};
			hr = spIStream->Stat(
				&statstg,//[out]                STATSTG* pstatstg,
				STATFLAG_NONAME//[in]                 DWORD grfStatFlag
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (0 == statstg.cbSize.LowPart)
			{
				continue;
			}

			CHeapPtr<BYTE> apBuffer;
			BOOL bRet = apBuffer.Allocate(statstg.cbSize.LowPart);
			if (!bRet)
			{
				D_SET_LAST_STATUS(E_OUTOFMEMORY, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), E_OUTOFMEMORY, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = E_OUTOFMEMORY;
				break;
			}

			ULONG cbRead = 0;
			hr = spIStream->Read((BYTE*)apBuffer, statstg.cbSize.LowPart, &cbRead);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			m_pStorageDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)apBuffer.Detach();
		}
		else if (6 == ii)
		{
			//if (m_pStorageAdapterDescriptor)
			ULONG cb = 0;
			STATSTG statstg = {0};
			hr = spIStream->Stat(
				&statstg,//[out]                STATSTG* pstatstg,
				STATFLAG_NONAME//[in]                 DWORD grfStatFlag
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (0 == statstg.cbSize.LowPart)
			{
				continue;
			}

			CHeapPtr<BYTE> apBuffer;
			BOOL bRet = apBuffer.Allocate(statstg.cbSize.LowPart);
			if (!bRet)
			{
				D_SET_LAST_STATUS(E_OUTOFMEMORY, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), E_OUTOFMEMORY, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = E_OUTOFMEMORY;
				break;
			}

			ULONG cbRead = 0;
			hr = spIStream->Read((BYTE*)apBuffer, statstg.cbSize.LowPart, &cbRead);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			m_pStorageAdapterDescriptor = (STORAGE_ADAPTER_DESCRIPTOR*)apBuffer.Detach();
		}
		else if (7 == ii)
		{
			//if (m_pStorageDeviceIDDescriptor)
			ULONG cb = 0;
			STATSTG statstg = {0};
			hr = spIStream->Stat(
				&statstg,//[out]                STATSTG* pstatstg,
				STATFLAG_NONAME//[in]                 DWORD grfStatFlag
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (0 == statstg.cbSize.LowPart)
			{
				continue;
			}

			CHeapPtr<BYTE> apBuffer;
			BOOL bRet = apBuffer.Allocate(statstg.cbSize.LowPart);
			if (!bRet)
			{
				D_SET_LAST_STATUS(E_OUTOFMEMORY, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), E_OUTOFMEMORY, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = E_OUTOFMEMORY;
				break;
			}

			ULONG cbRead = 0;
			hr = spIStream->Read((BYTE*)apBuffer, statstg.cbSize.LowPart, &cbRead);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			m_pStorageDeviceIDDescriptor = (STORAGE_DEVICE_ID_DESCRIPTOR*)apBuffer.Detach();
		}
		else if (8 == ii)
		{
			//if (m_pStorageDeviceUniqueIDDescriptor)
			ULONG cb = 0;
			STATSTG statstg = {0};
			hr = spIStream->Stat(
				&statstg,//[out]                STATSTG* pstatstg,
				STATFLAG_NONAME//[in]                 DWORD grfStatFlag
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (0 == statstg.cbSize.LowPart)
			{
				continue;
			}

			CHeapPtr<BYTE> apBuffer;
			BOOL bRet = apBuffer.Allocate(statstg.cbSize.LowPart);
			if (!bRet)
			{
				D_SET_LAST_STATUS(E_OUTOFMEMORY, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), E_OUTOFMEMORY, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = E_OUTOFMEMORY;
				break;
			}

			ULONG cbRead = 0;
			hr = spIStream->Read((BYTE*)apBuffer, statstg.cbSize.LowPart, &cbRead);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			m_pStorageDeviceUniqueIDDescriptor = (STORAGE_DEVICE_ID_DESCRIPTOR*)apBuffer.Detach();
		}
		else if (9 == ii)
		{
			ULONG cb = 0;
			//if (m_StorageDeviceNumber)
			{
				cb = sizeof(m_StorageDeviceNumber);
				ULONG cbRead = 0;
				hr = spIStream->Read(
					&m_StorageDeviceNumber,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbRead//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (10 == ii)
		{
			ULONG cb = 0;
			//if (m_StorageHotplugInfo)
			{
				cb = sizeof(STORAGE_HOTPLUG_INFO);
				ULONG cbRead = 0;
				hr = spIStream->Read(
					&m_StorageHotplugInfo,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbRead//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (11 == ii)
		{
			//if (m_pMEDIA_TYPES)
			ULONG cb = 0;
			STATSTG statstg = {0};
			hr = spIStream->Stat(
				&statstg,//[out]                STATSTG* pstatstg,
				STATFLAG_NONAME//[in]                 DWORD grfStatFlag
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (0 == statstg.cbSize.LowPart)
			{
				continue;
			}

			CHeapPtr<BYTE> apBuffer;
			BOOL bRet = apBuffer.Allocate(statstg.cbSize.LowPart);
			if (!bRet)
			{
				D_SET_LAST_STATUS(E_OUTOFMEMORY, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), E_OUTOFMEMORY, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = E_OUTOFMEMORY;
				break;
			}

			ULONG cbRead = 0;
			hr = spIStream->Read((BYTE*)apBuffer, statstg.cbSize.LowPart, &cbRead);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleDiskInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			m_pMEDIA_TYPES = (GET_MEDIA_TYPES*)apBuffer.Detach();
		}
	}

	return nStatus;
}

CAllDiskInfo::CAllDiskInfo()
{

}

CAllDiskInfo::~CAllDiskInfo()
{
	DestroyMember();
}

VOID CAllDiskInfo::DestroyMember()
{
	for (size_t ii=0; ii<m_pDiskInfos.size(); ++ii)
	{
		delete m_pDiskInfos[ii];
		m_pDiskInfos[ii]= NULL;
	}
	m_pDiskInfos.clear();
}

VOID CAllDiskInfo::GetAllDiskNumbers( DWORD dwMaxDiskNumber, vector<DWORD> & DiskkNumbers )
{
	for (DWORD dn=0; dn<dwMaxDiskNumber; ++dn)
	{
		HANDLE hDisk = CDiskHelper::CreateHandle(
			dn,//IN DWORD dwDiskNumber, 
			GENERIC_READ//IN DWORD dwDesiredAccess
			);
		if (INVALID_HANDLE_VALUE != hDisk)
		{
			DiskkNumbers.push_back(dn);
			CloseHandle(hDisk); hDisk  = INVALID_HANDLE_VALUE;
		}
	}
}

INT CAllDiskInfo::Retrieve( DWORD dwMaxDiskNumber )
{
	INT nStatus = 0;

	vector<DWORD> DiskNumbers;
	GetAllDiskNumbers(dwMaxDiskNumber, DiskNumbers);

	for (size_t ii=0; ii<DiskNumbers.size(); ++ii)
	{
		DWORD dwDiskNumber = DiskNumbers[ii];

		CAutoPtr<CSingleDiskInfo> apSingleDiskInfo(new CSingleDiskInfo);
		nStatus = apSingleDiskInfo->Retrieve(dwDiskNumber, TRUE);
		if (nStatus)
		{
			D_API_ERR(0, /*DRNPOS,*/ TEXT("Retrieve"), E_FAIL, TEXT("[dwDiskNumber=%u]"), dwDiskNumber);
		}
		else 
		{
			m_pDiskInfos.push_back(apSingleDiskInfo.Detach());
		}
	}

	return nStatus;
}

INT CAllDiskInfo::WriteToStorage( IStorage * pIStorage )
{
	INT nStatus = 0;
	HRESULT hr = S_OK;

	
	

	do 
	{
		//******************************************
		CComPtr<IStream> spIStream;
		hr = pIStorage->CreateStream(
			STG_ITEM_COUNT_NAME,//[in]                 const WCHAR* pwcsName,
			STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("CreateStream"), hr, TEXT("StreamName=[%s]"), STG_ITEM_COUNT_NAME); /*0003*/
			nStatus = hr;
			break;
		}

		//******************************************
		CComVariant var = (ULONG)(m_pDiskInfos.size());
		hr = var.WriteToStream(spIStream);
		if (FAILED(hr))
		{	
			D_SET_LAST_STATUS(hr, -1);
			nStatus = hr;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStream"), hr, TEXT("StreamName=[%s]"), STG_ITEM_COUNT_NAME); /*0003*/
			break;
		}

		//******************************************
		for (size_t ii=0; ii<m_pDiskInfos.size(); ++ii)
		{
			//******************************************
			CString strStorageName;
			strStorageName.Format(STG_ITEM_NAME, ii);
			CComPtr<IStorage> spIStorage;
			hr = pIStorage->CreateStorage(
				strStorageName,//[in]                 const WCHAR* pwcsName,
				STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
				0,//[in]                 DWORD reserved1,
				0,//[in]                 DWORD reserved2,
				&spIStorage//[out]                IStorage** ppstg
				);
			if (FAILED(hr))
			{	
				D_SET_LAST_STATUS(hr, -1);
				nStatus = hr;
				D_API_ERR(0, /*DRNPOS,*/ TEXT("CreateStorage"), hr, TEXT("[%s]"), strStorageName.GetString());  /*0007*/
				break;
			}

			//******************************************
			nStatus = m_pDiskInfos[ii]->WriteToStorage(spIStorage);
			if (nStatus)
			{
				D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStorage"), E_FAIL, TEXT("[index=%u]"), ii);  /*0008*/
				break;
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

INT CAllDiskInfo::ReadFromStorage( IStorage * pIStorage )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	
	

	do 
	{
		//******************************************
		CComPtr<IStream> spIStream;
		hr = pIStorage->OpenStream(
			STG_ITEM_COUNT_NAME,//[in]                 const WCHAR* pwcsName,
			NULL,//[in]                 void* reserved1,
			STGM_READ | STGM_SHARE_EXCLUSIVE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("OpenStream"), hr, TEXT("[StreamNames=%s]"),STG_ITEM_COUNT_NAME); /*0003*/
			nStatus = hr;
			break;
		}

		//******************************************
		CComVariant var;
		hr = var.ReadFromStream(spIStream);
		if (FAILED(hr))
		{	
			D_SET_LAST_STATUS(hr, -1);
			nStatus = hr;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("ReadFromStream"), hr, TEXT("StreamName=[%s]"), STG_ITEM_COUNT_NAME); /*0003*/
			break;
		}

		//******************************************
		ULONG nItemCount = var.ulVal;
		for (size_t ii=0; ii<nItemCount; ++ii)
		{
			//******************************************
			CString strStorageName;
			strStorageName.Format(STG_ITEM_NAME, ii);
			CComPtr<IStorage> spIStorage;
			hr = pIStorage->OpenStorage(
				strStorageName,//[in]                 const WCHAR* pwcsName,
				NULL,//[in]                 IStorage* pstgPriority,
				STGM_READ | STGM_SHARE_EXCLUSIVE,//[in]                 DWORD grfMode,
				0,//[in]                 SNB snbExclude,
				0,//[in]                 DWORD reserved,
				&spIStorage//[out]                IStorage** ppstg
				);
			if (FAILED(hr))
			{	
				D_SET_LAST_STATUS(hr, -1);
				nStatus = hr;
				D_API_ERR(0, /*DRNPOS,*/ TEXT("OpenStorage"), hr, TEXT("[%s]"), strStorageName.GetString());  /*0007*/
				break;
			}

			//******************************************
			CAutoPtr<CSingleDiskInfo> apSingleDiskInfo(new CSingleDiskInfo);
			nStatus = apSingleDiskInfo->ReadFromStorage(spIStorage);
			if (nStatus)
			{
				D_API_ERR(0, /*DRNPOS,*/ TEXT("ReadFromStorage"), E_FAIL, TEXT("[index=%u]"), ii);  /*0008*/
				break;
			}
			m_pDiskInfos.push_back(apSingleDiskInfo.Detach());
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

CONST CSingleDiskInfo* CAllDiskInfo::FindDisk( ULONG ulDiskNumber )
{
	CONST CSingleDiskInfo* pSingleDiskInfo = NULL;
	for (size_t ii=0; ii<m_pDiskInfos.size(); ++ii)
	{
		if (ulDiskNumber == m_pDiskInfos[ii]->m_DiskNumber )
		{
			pSingleDiskInfo = m_pDiskInfos[ii];
			break;
		}
	}
	return pSingleDiskInfo;
}