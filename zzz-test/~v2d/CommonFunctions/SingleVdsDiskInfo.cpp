#include "stdafx.h"
#include "SingleVdsDiskInfo.h"

static CONST LPCTSTR szStreamNamesOfCSingleVdsDiskInfo[] = 
{
	TEXT("m_id"),  //0
	TEXT("m_status"), //1  
	TEXT("m_ReserveMode"),  //2
	TEXT("m_health"),  //3
	TEXT("m_dwDeviceType"),  //4
	TEXT("m_dwMediaType"),  //5
	TEXT("m_ullSize"),  //6
	TEXT("m_ulBytesPerSector"), //  7
	TEXT("m_ulSectorsPerTrack"), //  8
	TEXT("m_ulTracksPerCylinder"), //  9
	TEXT("m_ulFlags"), //  10
	TEXT("m_BusType"), //  11
	TEXT("m_PartitionStyle"), //12  
	TEXT("m_DiskGuid"), //  13
	TEXT("m_strDiskAddress"), //14  
	TEXT("m_strName"), //  15
	TEXT("m_strFriendlyName"), //  16
	TEXT("m_strAdaptorName"), //  17
	TEXT("m_strDevicePath"), // 18
	TEXT("m_VDSDiskExtents"), // 19
};

INT CSingleVdsDiskInfo::WriteToStorage( IStorage * pIStorage )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfCSingleVdsDiskInfo); ++ii)
	{
		//******************************************
		CComPtr<IStream> spIStream;
		hr = pIStorage->CreateStream(
			szStreamNamesOfCSingleVdsDiskInfo[ii],//[in]                 const WCHAR* pwcsName,
			STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
			nStatus = hr;
			break;
		}

		CComVariant var;
		if (0 == ii) //******************************************
		{
			ULONG cb = sizeof(m_MyVDSDiskProp.m_id);
			ULONG cbOut = 0;
			hr = spIStream->Write(&m_MyVDSDiskProp.m_id, cb, &cbOut);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (1 == ii) //******************************************
		{
			var = (LONG)m_MyVDSDiskProp.m_status;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (2 == ii) //******************************************
		{
			var = (LONG)m_MyVDSDiskProp.m_ReserveMode;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (3 == ii) //******************************************
		{
			var = (LONG)m_MyVDSDiskProp.m_health;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (4 == ii) //******************************************
		{
			var = (ULONG)m_MyVDSDiskProp.m_dwDeviceType;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (5 == ii) //******************************************
		{
			var = (ULONG)m_MyVDSDiskProp.m_dwMediaType;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (6 == ii) //******************************************
		{
			var = m_MyVDSDiskProp.m_ullSize;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (7 == ii) //******************************************
		{
			var = m_MyVDSDiskProp.m_ulBytesPerSector;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (8 == ii) //******************************************
		{
			var = m_MyVDSDiskProp.m_ulSectorsPerTrack;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (9 == ii) //******************************************
		{
			var = m_MyVDSDiskProp.m_ulTracksPerCylinder;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (10 == ii) //******************************************
		{
			var = m_MyVDSDiskProp.m_ulFlags;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (11 == ii) //******************************************
		{
			var = (LONG)m_MyVDSDiskProp.m_BusType;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (12 == ii) //******************************************
		{
			var = (LONG)m_MyVDSDiskProp.m_PartitionStyle;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (13 == ii) //******************************************
		{
			ULONG cb = sizeof(m_MyVDSDiskProp.m_DiskGuid);
			ULONG cbWritten = 0;
			hr = spIStream->Write(&m_MyVDSDiskProp.m_DiskGuid, cb, &cbWritten);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (14 == ii) //******************************************
		{
			var = (LPCTSTR)m_MyVDSDiskProp.m_strDiskAddress;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (15 == ii) //******************************************
		{
			var = (LPCTSTR)m_MyVDSDiskProp.m_strName;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (16 == ii) //******************************************
		{
			var = (LPCTSTR)m_MyVDSDiskProp.m_strFriendlyName;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (17 == ii) //******************************************
		{
			var = (LPCTSTR)m_MyVDSDiskProp.m_strAdaptorName;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (18 == ii) //******************************************
		{
			var = (LPCTSTR)m_MyVDSDiskProp.m_strDevicePath;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (19 == ii) //******************************************
		{
			ULONG cb = sizeof(VDS_DISK_EXTENT) * m_VDSDiskExtents.size();
			ULONG cbWritten = 0;
			if (cb == 0 )
			{
				continue;
			}

			hr = spIStream->Write(&(m_VDSDiskExtents[0]), cb, &cbWritten);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
	}

	return nStatus;
}

INT CSingleVdsDiskInfo::ReadFromStorage( IStorage * pIStorage )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfCSingleVdsDiskInfo); ++ii)
	{
		//******************************************
		CComPtr<IStream> spIStream;
		hr = pIStorage->OpenStream(
			szStreamNamesOfCSingleVdsDiskInfo[ii],//[in]                 const WCHAR* pwcsName,
			NULL,//[in]                 void* reserved1,
			STGM_READ | STGM_SHARE_EXCLUSIVE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("OpenStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
			nStatus = hr;
			break;
		}

		CComVariant var;
		if (0 == ii) //******************************************
		{
			ULONG cb = sizeof(m_MyVDSDiskProp.m_id);
			ULONG cbOut = 0;
			hr = spIStream->Read(&m_MyVDSDiskProp.m_id, cb, &cbOut);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (1 == ii) //******************************************
		{
			//var = (LONG)m_MyVDSDiskProp.m_status;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_status = (VDS_DISK_STATUS)var.lVal;
		}
		else if (2 == ii) //******************************************
		{
			//var = (LONG)m_MyVDSDiskProp.m_ReserveMode;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_ReserveMode = (VDS_LUN_RESERVE_MODE)var.lVal;
		}
		else if (3 == ii) //******************************************
		{
			//var = (LONG)m_MyVDSDiskProp.m_health;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_health = (VDS_HEALTH)var.lVal;
		}
		else if (4 == ii) //******************************************
		{
			//var = (ULONG)m_MyVDSDiskProp.m_dwDeviceType;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_dwDeviceType = var.ulVal;
		}
		else if (5 == ii) //******************************************
		{
			//var = (ULONG)m_MyVDSDiskProp.m_dwMediaType;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_dwMediaType = var.ulVal;
		}
		else if (6 == ii) //******************************************
		{
			//var = m_MyVDSDiskProp.m_ullSize;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_ullSize = var.ullVal;
		}
		else if (7 == ii) //******************************************
		{
			//var = m_MyVDSDiskProp.m_ulBytesPerSector;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_ulBytesPerSector = var.ulVal;
		}
		else if (8 == ii) //******************************************
		{
			//var = m_MyVDSDiskProp.m_ulSectorsPerTrack;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_ulSectorsPerTrack = var.ulVal;
		}
		else if (9 == ii) //******************************************
		{
			//var = m_MyVDSDiskProp.m_ulTracksPerCylinder;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_ulTracksPerCylinder = var.ulVal;
		}
		else if (10 == ii) //******************************************
		{
			//var = m_MyVDSDiskProp.m_ulFlags;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_ulFlags = var.ulVal;
		}
		else if (11 == ii) //******************************************
		{
			//var = (LONG)m_MyVDSDiskProp.m_BusType;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_BusType = (VDS_STORAGE_BUS_TYPE)var.lVal;
		}
		else if (12 == ii) //******************************************
		{
			//var = (LONG)m_MyVDSDiskProp.m_PartitionStyle;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_PartitionStyle = (VDS_PARTITION_STYLE)var.lVal;
		}
		else if (13 == ii) //******************************************
		{
			ULONG cb = sizeof(m_MyVDSDiskProp.m_DiskGuid);
			ULONG cbRead = 0;
			hr = spIStream->Read(&m_MyVDSDiskProp.m_DiskGuid, cb, &cbRead);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
		else if (14 == ii) //******************************************
		{
			//var = (LPCTSTR)m_strDiskAddress;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_strDiskAddress = var.bstrVal;
		}
		else if (15 == ii) //******************************************
		{
			//var = (LPCTSTR)m_strName;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_strName = var.bstrVal;
		}
		else if (16 == ii) //******************************************
		{
			//var = (LPCTSTR)m_strFriendlyName;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_strFriendlyName = var.bstrVal;
		}
		else if (17 == ii) //******************************************
		{
			//var = (LPCTSTR)m_strAdaptorName;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_strAdaptorName = var.bstrVal;
		}
		else if (18 == ii) //******************************************
		{
			//var = (LPCTSTR)m_strDevicePath;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
			m_MyVDSDiskProp.m_strDevicePath = var.bstrVal;
		}
		else if (19 == ii) //******************************************
		{
			STATSTG statstg = {0};
			hr = spIStream->Stat(
				&statstg,//[out]                STATSTG* pstatstg,
				STATFLAG_NONAME//[in]                 DWORD grfStatFlag
				);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				nStatus = hr;
				break;
			}

			if (0 == statstg.cbSize.LowPart)
			{
				continue;
			}

			ULONG cbRead = 0;
			m_VDSDiskExtents.resize(statstg.cbSize.LowPart/sizeof(VDS_DISK_EXTENT));
			hr = spIStream->Read(&(m_VDSDiskExtents[0]), statstg.cbSize.LowPart, &cbRead);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsDiskInfo[ii]));
				break;
			}
		}
	}

	return nStatus;
}