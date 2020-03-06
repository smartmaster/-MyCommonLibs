#include "stdafx.h"
#include "MyVssSnapshotProp.h"
#include "..\CommonFunctions\MiscHelper.h"

static CONST LPCTSTR szStreamNamesOfMY_VSS_SNAPSHOT_PROP[] = 
{
	TEXT("m_SnapshotId"), //0
	TEXT("m_SnapshotSetId"), //1
	TEXT("m_lSnapshotsCount"), //2
	TEXT("m_pwszSnapshotDeviceObject"), //3
	TEXT("m_pwszOriginalVolumeName"), //4
	TEXT("m_pwszOriginatingMachine"), //5
	TEXT("m_pwszServiceMachine"), //6
	TEXT("m_pwszExposedName"), //7
	TEXT("m_pwszExposedPath"), //8
	TEXT("m_ProviderId"), //9
	TEXT("m_lSnapshotAttributes"), //10
	TEXT("m_tsCreationTimestamp"), //11
	TEXT("m_eStatus"), //12
};

VOID MY_VSS_SNAPSHOT_PROP::Copy( VSS_SNAPSHOT_PROP & VssSnapshotProp )
{
	m_SnapshotId = VssSnapshotProp.m_SnapshotId;
	m_SnapshotSetId = VssSnapshotProp.m_SnapshotSetId;
	m_lSnapshotsCount = VssSnapshotProp.m_lSnapshotsCount;
	m_pwszSnapshotDeviceObject = PTR_TO_STRING(VssSnapshotProp.m_pwszSnapshotDeviceObject);
	m_pwszOriginalVolumeName = PTR_TO_STRING(VssSnapshotProp.m_pwszOriginalVolumeName);
	m_pwszOriginatingMachine = PTR_TO_STRING(VssSnapshotProp.m_pwszOriginatingMachine);
	m_pwszServiceMachine = PTR_TO_STRING(VssSnapshotProp.m_pwszServiceMachine);
	m_pwszExposedName = PTR_TO_STRING(VssSnapshotProp.m_pwszExposedName);
	m_pwszExposedPath = PTR_TO_STRING(VssSnapshotProp.m_pwszExposedPath);
	m_ProviderId = VssSnapshotProp.m_ProviderId;
	m_lSnapshotAttributes = VssSnapshotProp.m_lSnapshotAttributes;
	m_tsCreationTimestamp = VssSnapshotProp.m_tsCreationTimestamp;
	m_eStatus = VssSnapshotProp.m_eStatus;
}

INT MY_VSS_SNAPSHOT_PROP::WriteToStorage( IStorage * pIStorage )
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfMY_VSS_SNAPSHOT_PROP); ++ii)
	{
		CComPtr<IStream> spIStream;
		hr = pIStorage->CreateStream(
			szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii],//[in]                 const WCHAR* pwcsName,
			STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
			nStatus = hr;
			break;
		}

		//******************************************
		CComVariant var;
		if (0 == ii)
		{
			ULONG cb = sizeof(m_SnapshotId);
			ULONG cbOut = 0;
			hr = spIStream->Write(&m_SnapshotId, cb, &cbOut);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (1 == ii)
		{
			ULONG cb = sizeof(m_SnapshotSetId);
			ULONG cbOut = 0;
			hr = spIStream->Write(&m_SnapshotSetId, cb, &cbOut);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (2 == ii)
		{
			var = m_lSnapshotsCount;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}

		}
		else if (3 == ii)
		{
			var = (LPCTSTR)m_pwszSnapshotDeviceObject;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (4 == ii)
		{
			var = (LPCTSTR)m_pwszOriginalVolumeName;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (5 == ii)
		{
			var = (LPCTSTR)m_pwszOriginatingMachine;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (6 == ii)
		{
			var = (LPCTSTR)m_pwszServiceMachine;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (7 == ii)
		{
			var = (LPCTSTR)m_pwszExposedName;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (8 == ii)
		{
			var = (LPCTSTR)m_pwszExposedPath;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (9 == ii)
		{
			ULONG cb = sizeof(m_ProviderId);
			ULONG cbOut = 0;
			hr = spIStream->Write(&m_ProviderId, cb, &cbOut);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (10 == ii)
		{
			var = m_lSnapshotAttributes;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (11 == ii)
		{
			var = (LONGLONG)m_tsCreationTimestamp;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (12 == ii)
		{
			var = (LONG)m_eStatus;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
	}

	return nStatus;
}

INT MY_VSS_SNAPSHOT_PROP::ReadFromStorage( IStorage * pIStorage )
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfMY_VSS_SNAPSHOT_PROP); ++ii)
	{
		CComPtr<IStream> spIStream;
		hr = pIStorage->OpenStream(
			szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii],//[in]                 const WCHAR* pwcsName,
			NULL,//[in]                 void* reserved1,
			STGM_READ | STGM_SHARE_EXCLUSIVE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("OpenStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
			nStatus = hr;
			break;
		}

		//******************************************
		CComVariant var;
		if (0 == ii)
		{
			ULONG cb = sizeof(m_SnapshotId);
			ULONG cbOut = 0;
			hr = spIStream->Read(&m_SnapshotId, cb, &cbOut);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (1 == ii)
		{
			ULONG cb = sizeof(m_SnapshotSetId);
			ULONG cbOut = 0;
			hr = spIStream->Read(&m_SnapshotSetId, cb, &cbOut);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (2 == ii)
		{
			//var = m_lSnapshotsCount;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
			m_lSnapshotsCount = var.lVal;
		}
		else if (3 == ii)
		{
			//var = (LPCTSTR)m_pwszSnapshotDeviceObject;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
			m_pwszSnapshotDeviceObject = var.bstrVal;
		}
		else if (4 == ii)
		{
			//var = (LPCTSTR)m_pwszOriginalVolumeName;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
			m_pwszOriginalVolumeName = var.bstrVal;
		}
		else if (5 == ii)
		{
			//var = (LPCTSTR)m_pwszOriginatingMachine;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
			m_pwszOriginatingMachine = var.bstrVal;
		}
		else if (6 == ii)
		{
			//var = (LPCTSTR)m_pwszServiceMachine;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
			m_pwszServiceMachine = var.bstrVal;
		}
		else if (7 == ii)
		{
			//var = (LPCTSTR)m_pwszExposedName;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
			m_pwszExposedName = var.bstrVal;
		}
		else if (8 == ii)
		{
			//var = (LPCTSTR)m_pwszExposedPath;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
			m_pwszExposedPath = var.bstrVal;
		}
		else if (9 == ii)
		{
			ULONG cb = sizeof(m_ProviderId);
			ULONG cbOut = 0;
			hr = spIStream->Read(&m_ProviderId, cb, &cbOut);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
		}
		else if (10 == ii)
		{
			//var = m_lSnapshotAttributes;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
			m_lSnapshotAttributes = var.lVal;
		}
		else if (11 == ii)
		{
			//var = (LONGLONG)m_tsCreationTimestamp;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
			m_tsCreationTimestamp = var.llVal;
		}
		else if (12 == ii)
		{
			//var = (LONG)m_eStatus;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfMY_VSS_SNAPSHOT_PROP[ii]));
				nStatus = hr;
				break;
			}
			m_eStatus = (VSS_SNAPSHOT_STATE)var.lVal;
		}
	}

	return nStatus;
}

INT MY_VSS_SNAPSHOT_PROP::ReadFromVectorRootStorage( IStorage * pIStorage, vector<MY_VSS_SNAPSHOT_PROP> & MyVssSnapshotProps )
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
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("OpenStream"), hr, TEXT("[StreamNames=%s]"),STG_ITEM_COUNT_NAME));
			nStatus = hr;
			break;
		}

		//******************************************
		CComVariant var;
		hr = var.ReadFromStream(spIStream);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStream"), hr, TEXT("StreamName=[%s]"), STG_ITEM_COUNT_NAME));
			break;
		}

		//******************************************
		ULONG nItemCount = var.ulVal;
		MyVssSnapshotProps.resize(nItemCount);
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
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("OpenStorage"), hr, TEXT("[%s]"), strStorageName.GetString()));
				break;
			}

			//******************************************
			nStatus = MyVssSnapshotProps[ii].ReadFromStorage(spIStorage);
			if (nStatus)
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStorage"), E_FAIL, TEXT("[index=%u]"), ii));
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

CONST MY_VSS_SNAPSHOT_PROP* MY_VSS_SNAPSHOT_PROP::FindByVulumeGuid( LPCTSTR pszVolGuid, CONST vector<MY_VSS_SNAPSHOT_PROP> & MyVssSnapshotProps )
{
	CONST MY_VSS_SNAPSHOT_PROP* pSnapshotProp = NULL;
	for (size_t ii=0; ii<MyVssSnapshotProps.size(); ++ii)
	{
		if (CMiscHelper::IsVolumeGuidMatch(pszVolGuid, MyVssSnapshotProps[ii].m_pwszOriginalVolumeName))
		{
			pSnapshotProp = &MyVssSnapshotProps[ii];
			break;
		}
	}
	return pSnapshotProp;
}