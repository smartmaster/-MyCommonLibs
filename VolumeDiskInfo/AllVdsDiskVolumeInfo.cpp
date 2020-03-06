#include "stdafx.h"
#include "AllVdsDiskVolumeInfo.h"
#include "MiscHelper.h"

CAllVdsDiskVolumeInfo::CAllVdsDiskVolumeInfo()
{

}

CAllVdsDiskVolumeInfo::~CAllVdsDiskVolumeInfo()
{
	DestroyMember();
}

VOID CAllVdsDiskVolumeInfo::DestroyMember()
{
	for (size_t ii=0; ii<m_pVdsDiskInfos.size(); ++ii)
	{
		delete m_pVdsDiskInfos[ii];
		m_pVdsDiskInfos[ii]= NULL;
	}
	m_pVdsDiskInfos.clear();

	for (size_t ii=0; ii<m_pVdsVolumeInfos.size(); ++ii)
	{
		delete m_pVdsVolumeInfos[ii];
		m_pVdsVolumeInfos[ii]= NULL;
	}
	m_pVdsVolumeInfos.clear();
}

INT CAllVdsDiskVolumeInfo::Retrieve()
{
	INT nStatus = 0;
	CVDSService MyVdsService;
	nStatus = MyVdsService.Load();
	if (nStatus)
	{
		return nStatus;
	}

	vector<IVdsSwProvider*> VdsSwProviders;
	MyVdsService.QuerySoftwareProvides(VdsSwProviders);
	for (size_t ii=0; ii<VdsSwProviders.size(); ++ii)
	{
		CVdsSwProvider MySwProvider;
		MySwProvider.Initialize(VdsSwProviders[ii], FALSE);

		vector<IVdsPack*> vdsPacks;
		MySwProvider.QueryPacks(vdsPacks);
		for (size_t jj=0; jj<vdsPacks.size(); ++jj)
		{
			CVdsPack MyPack;
			MyPack.Initialize(vdsPacks[jj], FALSE);

			vector<IVdsDisk*> vdsDisks;
			MyPack.QueryDisks(vdsDisks);
			for (size_t kk=0; kk<vdsDisks.size(); ++kk)
			{
				CVdsDisk MyDisk;
				MyDisk.Initialize(vdsDisks[kk], FALSE);

				//******************************************
				CAutoPtr<CSingleVdsDiskInfo> apSingleVdsDiskInfo(new CSingleVdsDiskInfo);
				apSingleVdsDiskInfo->m_MyVDSDiskProp = MyDisk.m_MyVDSDiskProp;
				apSingleVdsDiskInfo->m_VDSDiskExtents = MyDisk.m_VDSDiskExtents;

				//******************************************
				m_pVdsDiskInfos.push_back(apSingleVdsDiskInfo.Detach());
			}

			//******************************************
			vector<IVdsVolume*> vdsVolumes;
			MyPack.QueryVolumes(vdsVolumes);
			for (size_t kk=0; kk<vdsVolumes.size(); ++kk)
			{
				CVdsVolume MyVolume;
				MyVolume.Initialize(vdsVolumes[kk], FALSE);

				//******************************************
				CAutoPtr<CSingleVdsVolumeInfo> apSingleVdsVolumeInfo(new CSingleVdsVolumeInfo);
				apSingleVdsVolumeInfo->m_MyVDSVolumeProp = MyVolume.m_MyVDSVolumeProp;
				apSingleVdsVolumeInfo->m_strVolumeGUID = MyVolume.m_strVolumeGUID;

				//******************************************
				vector<IVdsVolumePlex*> vdsPlexes;
				MyVolume.QueryPlexes(vdsPlexes);
				apSingleVdsVolumeInfo->m_VolumePlexes.resize(vdsPlexes.size());
				for (size_t nn=0; nn<vdsPlexes.size(); ++nn)
				{
					CVdsVolumePlex MyVolumePlex;
					MyVolumePlex.Initialize(vdsPlexes[nn], FALSE);

					//******************************************
					CSingleVdsVolumePlex & svvp = apSingleVdsVolumeInfo->m_VolumePlexes[nn];
					svvp.m_VDSVolumePlexProp = MyVolumePlex.m_VDSVolumePlexProp;
					svvp.m_VDSDiskExtents = MyVolumePlex.m_VDSDiskExtents;
				}

				//******************************************
				m_pVdsVolumeInfos.push_back(apSingleVdsVolumeInfo.Detach());
			}
		}
	}
	return nStatus;
}

INT CAllVdsDiskVolumeInfo::WriteVdsDiskToStorage( IStorage * pIStorage )
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
		CComVariant var = (ULONG)(m_pVdsDiskInfos.size());
		hr = var.WriteToStream(spIStream);
		if (FAILED(hr))
		{	
			D_SET_LAST_STATUS(hr, -1);
			nStatus = hr;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStream"), hr, TEXT("StreamName=[%s]"), STG_ITEM_COUNT_NAME); /*0003*/
			break;
		}

		//******************************************
		for (size_t ii=0; ii<m_pVdsDiskInfos.size(); ++ii)
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
			nStatus = m_pVdsDiskInfos[ii]->WriteToStorage(spIStorage);
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

INT CAllVdsDiskVolumeInfo::ReadVdsDiskFromStorage( IStorage * pIStorage )
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
			CAutoPtr<CSingleVdsDiskInfo> apSingleVdsDiskInfo(new CSingleVdsDiskInfo);
			nStatus = apSingleVdsDiskInfo->ReadFromStorage(spIStorage);
			if (nStatus)
			{
				D_API_ERR(0, /*DRNPOS,*/ TEXT("ReadFromStorage"), E_FAIL, TEXT("[index=%u]"), ii);  /*0008*/
				break;
			}
			m_pVdsDiskInfos.push_back(apSingleVdsDiskInfo.Detach());
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CAllVdsDiskVolumeInfo::WriteVdsVolumeToStorage( IStorage * pIStorage )
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
		CComVariant var = (ULONG)(m_pVdsVolumeInfos.size());
		hr = var.WriteToStream(spIStream);
		if (FAILED(hr))
		{	
			D_SET_LAST_STATUS(hr, -1);
			nStatus = hr;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStream"), hr, TEXT("StreamName=[%s]"), STG_ITEM_COUNT_NAME); /*0003*/
			break;
		}

		//******************************************
		for (size_t ii=0; ii<m_pVdsVolumeInfos.size(); ++ii)
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
			nStatus = m_pVdsVolumeInfos[ii]->WriteToStorage(spIStorage);
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

INT CAllVdsDiskVolumeInfo::ReadVdsVolumeFromStorage( IStorage * pIStorage )
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
			D_API_ERR(0, /*DRNPOS,*/ TEXT("OpenStream"), hr, TEXT("[StreamNames=%sH]"),STG_ITEM_COUNT_NAME); /*0003*/
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
			CAutoPtr<CSingleVdsVolumeInfo> apSingleVdsVolumeInfo(new CSingleVdsVolumeInfo);
			nStatus = apSingleVdsVolumeInfo->ReadFromStorage(spIStorage);
			if (nStatus)
			{
				D_API_ERR(0, /*DRNPOS,*/ TEXT("ReadFromStorage"), E_FAIL, TEXT("[index=%u]"), ii);  /*0008*/
				break;
			}
			m_pVdsVolumeInfos.push_back(apSingleVdsVolumeInfo.Detach());

		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

CONST CSingleVdsVolumeInfo* CAllVdsDiskVolumeInfo::FindVolume( LPCTSTR pszVolGuid )
{
	CSingleVdsVolumeInfo* pSingleVolumeInfo = NULL;
	for (size_t ii=0; ii<m_pVdsVolumeInfos.size(); ++ii)
	{
		if (CMiscHelper::IsVolumeGuidMatch(pszVolGuid, m_pVdsVolumeInfos[ii]->m_strVolumeGUID))
		{
			pSingleVolumeInfo = m_pVdsVolumeInfos[ii];
			break;
		}
	}
	return pSingleVolumeInfo;
}

CONST CSingleVdsDiskInfo* CAllVdsDiskVolumeInfo::FindDisk( ULONG ulDiskNumber )
{
	CONST CSingleVdsDiskInfo* pSingleDiskInfo = NULL;
	for (size_t ii=0; ii<m_pVdsDiskInfos.size(); ++ii)
	{
		if (CMiscHelper::IsDiskNumberMatch(ulDiskNumber, m_pVdsDiskInfos[ii]->m_MyVDSDiskProp.m_strName) )
		{
			pSingleDiskInfo = m_pVdsDiskInfos[ii];
			break;
		}
	}
	return pSingleDiskInfo;
}
