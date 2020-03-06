#include "stdafx.h"
#include "SingleVdsVolumeInfo.h"
#include "MiscHelper.h"

static CONST LPCTSTR szStreamNamesOfCSingleVdsVolumePlex[] = 
{
	TEXT("id"), //0
	TEXT("type"), //1
	TEXT("status"), //2
	TEXT("health"), //3
	TEXT("TransitionState"), //4
	TEXT("ullSize"), //5
	TEXT("ulStripeSize"), //6
	TEXT("ulNumberOfMembers"), //7
	TEXT("m_VDSDiskExtents"), //8
};


static CONST LPCTSTR szStreamNamesOfCSingleVdsVolumeInfo[] = 
{
	TEXT("m_id"), //  0
	TEXT("m_type"), // 1  
	TEXT("m_status"), // 2  
	TEXT("m_health"), //  3
	TEXT("m_TransitionState"), //  4
	TEXT("m_ullSize"), //  5
	TEXT("m_ulFlags"), //  6
	TEXT("m_RecommendedFileSystemType"), //  7
	TEXT("m_strName"), // 8
	TEXT("m_strVolumeGUID"), // 9
};

static CONST LPCTSTR szStorageNamesOfCSingleVdsVolumeInfo[] = 
{
	TEXT("m_VolumePlexes"), // 0
};

INT CSingleVdsVolumePlex::WriteToStorage( IStorage * pIStorage )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfCSingleVdsVolumePlex); ++ii)
	{
		//******************************************
		CComPtr<IStream> spIStream;
		hr = pIStorage->CreateStream(
			szStreamNamesOfCSingleVdsVolumePlex[ii],//[in]                 const WCHAR* pwcsName,
			STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
			nStatus = hr;
			break;
		}

		CComVariant var;
		if (0 == ii) //******************************************
		{
			ULONG cb = sizeof(m_VDSVolumePlexProp.id);
			ULONG cbOut = 0;
			hr = spIStream->Write(&m_VDSVolumePlexProp.id, cb, &cbOut);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (1 == ii) //******************************************
		{
			var = (LONG)m_VDSVolumePlexProp.type;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (2 == ii) //******************************************
		{
			var = (LONG)m_VDSVolumePlexProp.status;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (3 == ii) //******************************************
		{
			var = (LONG)m_VDSVolumePlexProp.health;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (4 == ii) //******************************************
		{
			var = (LONG)m_VDSVolumePlexProp.TransitionState;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (5 == ii) //******************************************
		{
			var = m_VDSVolumePlexProp.ullSize;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (6 == ii) //******************************************
		{
			var = m_VDSVolumePlexProp.ulStripeSize;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (7 == ii) //******************************************
		{
			var = m_VDSVolumePlexProp.ulNumberOfMembers;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (8 == ii) //******************************************
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
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
	}

	return nStatus;
}

INT CSingleVdsVolumePlex::ReadFromStorage( IStorage * pIStorage )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfCSingleVdsVolumePlex); ++ii)
	{
		//******************************************
		CComPtr<IStream> spIStream;
		hr = pIStorage->OpenStream(
			szStreamNamesOfCSingleVdsVolumePlex[ii],//[in]                 const WCHAR* pwcsName,
			NULL,//[in]                 void* reserved1,
			STGM_READ | STGM_SHARE_EXCLUSIVE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("OpenStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
			nStatus = hr;
			break;
		}

		CComVariant var;
		if (0 == ii) //******************************************
		{
			ULONG cb = sizeof(m_VDSVolumePlexProp.id);
			ULONG cbOut = 0;
			hr = spIStream->Read(&m_VDSVolumePlexProp.id, cb, &cbOut);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (1 == ii) //******************************************
		{
			//var = (LONG)m_VDSVolumePlexProp.type;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_VDSVolumePlexProp.type = (VDS_VOLUME_PLEX_TYPE)var.lVal;
		}
		else if (2 == ii) //******************************************
		{
			//var = (LONG)m_VDSVolumePlexProp.status;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_VDSVolumePlexProp.status = (VDS_VOLUME_PLEX_STATUS)var.lVal; 
		}
		else if (3 == ii) //******************************************
		{
			//var = (LONG)m_VDSVolumePlexProp.health;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_VDSVolumePlexProp.health = (VDS_HEALTH)var.lVal;
		}
		else if (4 == ii) //******************************************
		{
			//var = (LONG)m_VDSVolumePlexProp.TransitionState;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_VDSVolumePlexProp.TransitionState = (VDS_TRANSITION_STATE)var.lVal;
		}
		else if (5 == ii) //******************************************
		{
			//var = m_VDSVolumePlexProp.ullSize;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_VDSVolumePlexProp.ullSize = var.ullVal;
		}
		else if (6 == ii) //******************************************
		{
			//var = m_VDSVolumePlexProp.ulStripeSize;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_VDSVolumePlexProp.ulStripeSize = var.ulVal;
		}
		else if (7 == ii) //******************************************
		{
			//var = m_VDSVolumePlexProp.ulNumberOfMembers;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_VDSVolumePlexProp.ulNumberOfMembers = var.ulVal;
		}
		else if (8 == ii) //******************************************
		{
			STATSTG statstg = {0};
			hr = spIStream->Stat(
				&statstg,//[out]                STATSTG* pstatstg,
				STATFLAG_NONAME//[in]                 DWORD grfStatFlag
				);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
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
				_OLD_DAPIERR((0, DRNPOS, TEXT("Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
	}

	return nStatus;
}

INT CSingleVdsVolumeInfo::WriteToStorage( IStorage * pIStorage )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfCSingleVdsVolumeInfo); ++ii)
	{
		//******************************************
		CComPtr<IStream> spIStream;
		hr = pIStorage->CreateStream(
			szStreamNamesOfCSingleVdsVolumeInfo[ii],//[in]                 const WCHAR* pwcsName,
			STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
			nStatus = hr;
			break;
		}

		CComVariant var;
		if (0 == ii) //******************************************
		{
			ULONG cb = sizeof(m_MyVDSVolumeProp.m_id);
			ULONG cbOut = 0;
			hr = spIStream->Write(&m_MyVDSVolumeProp.m_id, cb, &cbOut);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
		}
		else if (1 == ii) //******************************************
		{
			var = (LONG)m_MyVDSVolumeProp.m_type;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
		}
		else if (2 == ii) //******************************************
		{
			var = (LONG)m_MyVDSVolumeProp.m_status;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
		}
		else if (3 == ii) //******************************************
		{
			var = (LONG)m_MyVDSVolumeProp.m_health;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
		}
		else if (4 == ii) //******************************************
		{
			var = (LONG)m_MyVDSVolumeProp.m_TransitionState;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
		}
		else if (5 == ii) //******************************************
		{
			var = m_MyVDSVolumeProp.m_ullSize;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
		}
		else if (6 == ii) //******************************************
		{
			var = m_MyVDSVolumeProp.m_ulFlags;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (7 == ii) //******************************************
		{
			var = (LONG)m_MyVDSVolumeProp.m_RecommendedFileSystemType;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (8 == ii) //******************************************
		{
			var = (LPCTSTR)m_MyVDSVolumeProp.m_strName;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
		else if (9 == ii)
		{
			var = (LPCTSTR)m_strVolumeGUID;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
		}
	}

	//******************************************
	for (INT ii=0; ii<_countof(szStorageNamesOfCSingleVdsVolumeInfo); ++ii)
	{
		CComPtr<IStorage> spIStorage;
		hr = pIStorage->CreateStorage(
			szStorageNamesOfCSingleVdsVolumeInfo[ii],//[in]                 const WCHAR* pwcsName,
			STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spIStorage//[out]                IStorage** ppstg
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateStorage"), hr, TEXT("[%s]"), szStorageNamesOfCSingleVdsVolumeInfo[ii]));
			break;
		}

		if (0 == ii) //******************************************
		{
			nStatus = WriteVolumePlexesHelper(spIStorage, m_VolumePlexes);
			if (nStatus)
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteVolumePlexesHelper"), E_FAIL));
				break;
			}
		}
		//else if (1 == ii) //...
		//{
		//}
	}

	return nStatus;
}

INT CSingleVdsVolumeInfo::WriteVolumePlexesHelper( IStorage * pIStorage, vector<CSingleVdsVolumePlex> & VolumePlexes )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
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
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateStream"), hr, TEXT("StreamName=[%s]"), STG_ITEM_COUNT_NAME));
			nStatus = hr;
			break;
		}

		//******************************************
		CComVariant var = (ULONG)(VolumePlexes.size());
		hr = var.WriteToStream(spIStream);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("StreamName=[%s]"), STG_ITEM_COUNT_NAME));
			break;
		}

		//******************************************
		for (size_t ii=0; ii<VolumePlexes.size(); ++ii)
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
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("CreateStorage"), hr, TEXT("[%s]"), strStorageName.GetString()));
				break;
			}

			//******************************************
			nStatus = VolumePlexes[ii].WriteToStorage(spIStorage);
			if (nStatus)
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStorage"), E_FAIL, TEXT("[index=%u]"), ii));
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

INT CSingleVdsVolumeInfo::ReadVolumePlexesHelper( IStorage * pIStorage, vector<CSingleVdsVolumePlex> & VolumePlexes )
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
		VolumePlexes.resize(nItemCount);
		for (size_t ii=0; ii<VolumePlexes.size(); ++ii)
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
			nStatus = VolumePlexes[ii].ReadFromStorage(spIStorage);
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

INT CSingleVdsVolumeInfo::ReadFromStorage( IStorage * pIStorage )
{
	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfCSingleVdsVolumeInfo); ++ii)
	{
		//******************************************
		CComPtr<IStream> spIStream;
		hr = pIStorage->OpenStream(
			szStreamNamesOfCSingleVdsVolumeInfo[ii],//[in]                 const WCHAR* pwcsName,
			NULL,//[in]                 void* reserved1,
			STGM_READ | STGM_SHARE_EXCLUSIVE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("OpenStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
			nStatus = hr;
			break;
		}

		CComVariant var;
		if (0 == ii) //******************************************
		{
			ULONG cb = sizeof(m_MyVDSVolumeProp.m_id);
			ULONG cbOut = 0;
			hr = spIStream->Read(&m_MyVDSVolumeProp.m_id, cb, &cbOut);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
		}
		else if (1 == ii) //******************************************
		{
			//var = (LONG)m_MyVDSVolumeProp.m_type;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
			m_MyVDSVolumeProp.m_type = (VDS_VOLUME_TYPE)var.lVal;
		}
		else if (2 == ii) //******************************************
		{
			//var = (LONG)m_MyVDSVolumeProp.m_status;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
			m_MyVDSVolumeProp.m_status = (VDS_VOLUME_STATUS)var.lVal; 
		}
		else if (3 == ii) //******************************************
		{
			//var = (LONG)m_MyVDSVolumeProp.m_health;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
			m_MyVDSVolumeProp.m_health = (VDS_HEALTH)var.lVal;
		}
		else if (4 == ii) //******************************************
		{
			//var = (LONG)m_MyVDSVolumeProp.m_TransitionState;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
			m_MyVDSVolumeProp.m_TransitionState = (VDS_TRANSITION_STATE)var.lVal;
		}
		else if (5 == ii) //******************************************
		{
			//var = m_MyVDSVolumeProp.m_ullSize;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumeInfo[ii]));
				break;
			}
			m_MyVDSVolumeProp.m_ullSize = var.ullVal;
		}
		else if (6 == ii) //******************************************
		{
			//var = m_MyVDSVolumeProp.m_ulFlags;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_MyVDSVolumeProp.m_ulFlags = var.ulVal;
		}
		else if (7 == ii) //******************************************
		{
			//var = (LONG)m_MyVDSVolumeProp.m_RecommendedFileSystemType;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_MyVDSVolumeProp.m_RecommendedFileSystemType = (VDS_FILE_SYSTEM_TYPE)var.lVal;
		}
		else if (8 == ii) //******************************************
		{
			//var = (LPCTSTR)m_MyVDSVolumeProp.m_strName;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_MyVDSVolumeProp.m_strName = var.bstrVal;
		}
		else if (9 == ii)
		{
			//var = (LPCTSTR)m_strVolumeGUID;
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVdsVolumePlex[ii]));
				break;
			}
			m_strVolumeGUID = var.bstrVal;
		}
	}

	//******************************************
	for (INT ii=0; ii<_countof(szStorageNamesOfCSingleVdsVolumeInfo); ++ii)
	{
		CComPtr<IStorage> spIStorage;
		hr = pIStorage->OpenStorage(
			szStorageNamesOfCSingleVdsVolumeInfo[ii],//[in]                 const WCHAR* pwcsName,
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
			_OLD_DAPIERR((0, DRNPOS, TEXT("OpenStorage"), hr, TEXT("[%s]"), szStorageNamesOfCSingleVdsVolumeInfo[ii]));
			break;
		}

		if (0 == ii) //******************************************
		{
			nStatus = ReadVolumePlexesHelper(spIStorage, m_VolumePlexes);
			if (nStatus)
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteVolumePlexesHelper"), E_FAIL));
				break;
			}
		}
		//else if (1 == ii) //...
		//{
		//}
	}

	return nStatus;
}