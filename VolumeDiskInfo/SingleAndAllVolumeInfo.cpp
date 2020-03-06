#include "stdafx.h"
#include "DiskHelper.h"
#include "SingleAndAllVolumeInfo.h"
#include "MiscHelper.h"

//******************************************
static CONST LPCTSTR szStreamNamesOfCSingleVolumeInfo[] = 
{
	TEXT("m_strVolumeGUID"),
	TEXT("m_Paths"),
	TEXT("m_MountPoint"),
	TEXT("m_GetVolumeInformation"),
	TEXT("m_NtfsVolumeDataBuffer"),
	TEXT("m_pVolumeDiskExtents"),
	TEXT("m_pVolumeBitmapBuffer"),
	TEXT("m_GetLengthInformation"),
};

#define PATH_SEPERATOR TEXT("*")

CSingleVolumeInfo::CSingleVolumeInfo()
{
	ZeroMemory(&m_GetVolumeInformation, sizeof(CGetVolumeInformation));
	ZeroMemory(&m_NtfsVolumeDataBuffer, sizeof(NTFS_VOLUME_DATA_BUFFER));
	m_pVolumeDiskExtents = NULL;
	m_pVolumeBitmapBuffer = NULL;
	m_GetLengthInformation.Length.QuadPart = 0;
}

CSingleVolumeInfo::~CSingleVolumeInfo()
{
	{
		CHeapPtr<BYTE> apBufferCleanup((BYTE*)m_pVolumeDiskExtents);
		m_pVolumeDiskExtents = NULL;
	}
	{
		CHeapPtr<BYTE> apBufferCleanup((BYTE*)m_pVolumeBitmapBuffer);
		m_pVolumeBitmapBuffer = NULL;
	}
}

INT CSingleVolumeInfo::WriteToStorage( IStorage * pIStorage )
{
	D_START(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfCSingleVolumeInfo); ++ii)
	{
		CComPtr<IStream> spIStream;
		hr = pIStorage->CreateStream(
			szStreamNamesOfCSingleVolumeInfo[ii],//[in]                 const WCHAR* pwcsName,
			STGM_WRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved1,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("CreateStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
			nStatus = hr;
			break;
		}

		//******************************************
		CComVariant var;
		if (0 == ii)
		{
			var = (LPCTSTR)m_strVolumeGUID;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		//******************************************
		else if (1 == ii)
		{
			CString strPaths;
			for (size_t jj=0; jj<m_Paths.size(); ++jj)
			{
				strPaths += m_Paths[jj];
				strPaths += PATH_SEPERATOR;
			}
			strPaths += PATH_SEPERATOR;

			var = (LPCTSTR)strPaths;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		//******************************************
		else if (2 == ii)
		{
			CString strMountPoint;
			for (size_t jj=0; jj<m_MountPoint.size(); ++jj)
			{
				strMountPoint += m_MountPoint[jj];
				strMountPoint += PATH_SEPERATOR;
			}
			strMountPoint += PATH_SEPERATOR;

			var = (LPCTSTR)strMountPoint;
			hr = var.WriteToStream(spIStream);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		//******************************************
		else if (3 == ii)
		{
			ULONG cbWritten = 0;
			hr = spIStream->Write(
				&m_GetVolumeInformation,//[in]                 void const* pv,
				sizeof(CGetVolumeInformation),//[in]                 ULONG cb,
				&cbWritten//[out]                ULONG* pcbWritten
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		//******************************************
		else if (4 == ii)
		{
			ULONG cbWritten = 0;
			hr = spIStream->Write(
				&m_NtfsVolumeDataBuffer,//[in]                 void const* pv,
				sizeof(NTFS_VOLUME_DATA_BUFFER),//[in]                 ULONG cb,
				&cbWritten//[out]                ULONG* pcbWritten
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		//******************************************
		else if (5 == ii)
		{
			ULONG cb = 0;
			if (m_pVolumeDiskExtents)
			{
				cb = sizeof(VOLUME_DISK_EXTENTS) + sizeof(DISK_EXTENT)*(m_pVolumeDiskExtents->NumberOfDiskExtents - 1);
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					m_pVolumeDiskExtents,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (6 == ii)
		{
			ULONG cb = 0;
			if (m_pVolumeBitmapBuffer)
			{
				cb = FIELD_OFFSET(VOLUME_BITMAP_BUFFER, Buffer) + AtlAlignUp(m_pVolumeBitmapBuffer->BitmapSize.QuadPart, 8)/8;
				ULONG cbWritten = 0;
				hr = spIStream->Write(
					m_pVolumeBitmapBuffer,//[in]                 void const* pv,
					cb,//[in]                 ULONG cb,
					&cbWritten//[out]                ULONG* pcbWritten
					);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
					nStatus = hr;
					break;
				}
			}
		}
		else if (7 == ii)
		{
			ULONG cb = sizeof(GET_LENGTH_INFORMATION);
			ULONG cbWritten = 0;
			hr = spIStream->Write(
				&m_GetLengthInformation,//[in]                 void const* pv,
				cb,//[in]                 ULONG cb,
				&cbWritten//[out]                ULONG* pcbWritten
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
	}

	return nStatus;
}

INT CSingleVolumeInfo::ReadFromStorage( IStorage * pIStorage )
{
	D_START(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	for (INT ii=0; ii<_countof(szStreamNamesOfCSingleVolumeInfo); ++ii)
	{
		CComPtr<IStream> spIStream;
		hr = pIStorage->OpenStream(
			szStreamNamesOfCSingleVolumeInfo[ii],//[in]                 const WCHAR* pwcsName,
			NULL,//[in]                 void* reserved1,
			STGM_READ | STGM_SHARE_EXCLUSIVE,//[in]                 DWORD grfMode,
			0,//[in]                 DWORD reserved2,
			&spIStream//[out]                IStream** ppstm
			);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("OpenStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
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
				D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (VT_BSTR != var.vt)
			{
				D_SET_LAST_STATUS(E_INVALIDARG, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("VT_BSTR != var.vt"), E_INVALIDARG, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = E_INVALIDARG;
				break;
			}

			m_strVolumeGUID = var.bstrVal;
		}
		//******************************************
		else if (1 == ii)
		{
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (VT_BSTR != var.vt)
			{
				D_SET_LAST_STATUS(E_INVALIDARG, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("VT_BSTR != var.vt"), E_INVALIDARG, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = E_INVALIDARG;
				break;
			}

			CString strPaths = var.bstrVal;
			INT iStart = 0;
			for (;;)
			{
				CString strPath = strPaths.Tokenize(PATH_SEPERATOR, iStart);
				if (-1 == iStart)
				{
					break;
				}
				m_Paths.push_back(strPath);
			}

		}
		//******************************************
		else if (2 == ii)
		{
			hr = var.ReadFromStream(spIStream);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("ReadFromStream"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			if (VT_BSTR != var.vt)
			{
				D_SET_LAST_STATUS(E_INVALIDARG, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("VT_BSTR != var.vt"), E_INVALIDARG, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = E_INVALIDARG;
				break;
			}

			CString strMountPoints = var.bstrVal;
			INT iStart = 0;
			for (;;)
			{
				CString strMountPoint = strMountPoints.Tokenize(PATH_SEPERATOR, iStart);
				if (-1 == iStart)
				{
					break;
				}
				m_MountPoint.push_back(strMountPoint);
			}
		}
		//******************************************
		else if (3 == ii)
		{
			ULONG cbRead = 0;
			hr = spIStream->Read(
				&m_GetVolumeInformation,//[in]                 void const* pv,
				sizeof(CGetVolumeInformation),//[in]                 ULONG cb,
				&cbRead//[out]                ULONG* pcbWritten
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		//******************************************
		else if (4 == ii)
		{
			ULONG cbRead = 0;
			hr = spIStream->Read(
				&m_NtfsVolumeDataBuffer,//[in]                 void const* pv,
				sizeof(NTFS_VOLUME_DATA_BUFFER),//[in]                 ULONG cb,
				&cbRead//[out]                ULONG* pcbWritten
				);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Write"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
		//******************************************
		else if (5 == ii)
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
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
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
				D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), E_OUTOFMEMORY, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = E_OUTOFMEMORY;
				break;
			}

			ULONG cbRead = 0;
			hr = spIStream->Read((BYTE*)apBuffer, statstg.cbSize.LowPart, &cbRead);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			m_pVolumeDiskExtents = (VOLUME_DISK_EXTENTS*)apBuffer.Detach();
		}
		else if (6 == ii) //******************************************
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
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Stat"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
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
				D_API_ERR(0, /*DRNPOS,*/ TEXT("Allocate"), E_OUTOFMEMORY, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = E_OUTOFMEMORY;
				break;
			}

			ULONG cbRead = 0;
			hr = spIStream->Read((BYTE*)apBuffer, statstg.cbSize.LowPart, &cbRead);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}

			m_pVolumeBitmapBuffer = (VOLUME_BITMAP_BUFFER*)apBuffer.Detach();
		}
		else if (7 == ii)
		{
			ULONG cbRead = 0;
			hr = spIStream->Read(&m_GetLengthInformation, sizeof(GET_LENGTH_INFORMATION), &cbRead);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, /*DRNPOS,*/ TEXT("spIStream->Read"), hr, TEXT("[StreamNames[%d]]=[%s]"), ii, szStreamNamesOfCSingleVolumeInfo[ii]); /*0001*/
				nStatus = hr;
				break;
			}
		}
	}

	return nStatus;
}

INT CSingleVolumeInfo::Retrieve( LPCTSTR szVolumeGUID, BOOL bGetVolumeBitmap, BOOL bBreakOnError)
{
	INT nStatus = 0;

	do 
	{
		m_strVolumeGUID = szVolumeGUID;

		nStatus = CVolumeHelper::GetVolumePathFromGuid(m_strVolumeGUID, m_Paths);
		if (nStatus)
		{
			D_API_ERR(999, /*DRNPOS,*/ TEXT("GetVolumePathFromGuid"), nStatus, TEXT("")); /*0006*/
			//nStatus = E_FAIL;
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		CVolumeHelper::CVolumeInfomation volumeInfo;
		nStatus = CVolumeHelper::GetVolumeInfo(m_strVolumeGUID, volumeInfo);
		if (nStatus)
		{
			D_API_ERR(999, /*DRNPOS,*/ TEXT("GetVolumeInfo"), nStatus, TEXT("")); /*0006*/
			//nStatus = E_FAIL;
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}
		_tcscpy_s(m_GetVolumeInformation.m_VolumeNameBuffer, _countof(m_GetVolumeInformation.m_VolumeNameBuffer), volumeInfo.m_VolumeNameBuffer.GetString());
		m_GetVolumeInformation.m_VolumeSerialNumber = volumeInfo.m_VolumeSerialNumber;
		m_GetVolumeInformation.m_MaximumComponentLength = volumeInfo.m_MaximumComponentLength;
		m_GetVolumeInformation.m_FileSystemFlags = volumeInfo.m_FileSystemFlags;
		_tcscpy_s(m_GetVolumeInformation.m_FileSystemNameBuffer, _countof(m_GetVolumeInformation.m_FileSystemNameBuffer), volumeInfo.m_FileSystemNameBuffer.GetString());


		//////////////////////////////////////////////////////////////////////////
		nStatus = CVolumeHelper::GetVolumeMountPoint(m_strVolumeGUID, m_MountPoint);
		if (nStatus)
		{
			D_API_ERR(999, /*DRNPOS,*/ TEXT("GetVolumeMountPoint"), nStatus, TEXT("")); /*0006*/
			nStatus = 0; //always ignore this error
		//	break;
		}

		
		//////////////////////////////////////////////////////////////////////////
		HANDLE hDevice = CVolumeHelper::CreateHandle(m_strVolumeGUID, GENERIC_READ);
		if (INVALID_HANDLE_VALUE == hDevice)
		{
			D_API_ERR(0, /*DRNPOS,*/ TEXT("CreateHandle"), E_FAIL, TEXT("")); /*0009*/
			nStatus = E_FAIL;
			break;
		}
		CHandle ahDevice(hDevice); hDevice = INVALID_HANDLE_VALUE;

		nStatus = CVolumeHelper::FSCTL_GET_NTFS_VOLUME_DATA_Helper(ahDevice, m_NtfsVolumeDataBuffer);
		if (nStatus)
		{
			D_API_ERR(0, /*DRNPOS,*/ TEXT("FSCTL_GET_NTFS_VOLUME_DATA_Helper"), E_FAIL, TEXT("")); /*0009*/
			nStatus = 0; //ignore error FAT
			//break;
		}

		nStatus = CVolumeHelper::IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS_Helper(ahDevice, &m_pVolumeDiskExtents);
		if (nStatus)
		{
			D_API_ERR(999, /*DRNPOS,*/ TEXT("IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS_Helper"), nStatus, TEXT("")); /*0006*/
			//nStatus = hr;
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}

		if (bGetVolumeBitmap) //******************************************
		{
			nStatus = CVolumeHelper::FSCTL_GET_VOLUME_BITMAP_Helper(ahDevice, &m_pVolumeBitmapBuffer);
			if (nStatus)
			{
				D_API_ERR(0, /*DRNPOS,*/ TEXT("FSCTL_GET_VOLUME_BITMAP_Helper"), E_FAIL, TEXT("")); /*0009*/
				//nStatus = hr;
				if (bBreakOnError)
				{
					break;
				}
				else
				{
					nStatus = 0;
				}
			}
		}

		nStatus = CDiskHelper::IOCTL_DISK_GET_LENGTH_INFO_Helper(ahDevice, &m_GetLengthInformation);
		if (nStatus)
		{
			D_API_ERR(0, /*DRNPOS,*/ TEXT("IOCTL_DISK_GET_LENGTH_INFO_Helper"), E_FAIL, TEXT("")); /*0009*/
			//nStatus = hr;
			if (bBreakOnError)
			{
				break;
			}
			else
			{
				nStatus = 0;
			}
		}


	} while (FALSE);

	return nStatus;
}

CAllVolumeInfo::CAllVolumeInfo()
{

}

CAllVolumeInfo::~CAllVolumeInfo()
{
	DestroyMember();
}

VOID CAllVolumeInfo::DestroyMember()
{
	for (size_t ii=0; ii<m_pVolumeInfos.size(); ++ii)
	{
		delete m_pVolumeInfos[ii];
		m_pVolumeInfos[ii]= NULL;
	}
	m_pVolumeInfos.clear();
}

INT CAllVolumeInfo::Retrieve( BOOL bGetVolumeBitmap )
{
	INT nStatus = 0;

	vector<CString> VolumeGuids;
	CVolumeHelper::GetAllVolumeGuid(VolumeGuids);

	for (size_t ii=0; ii<VolumeGuids.size(); ++ii)
	{
		CString & VolumeGuid = VolumeGuids[ii];

		CAutoPtr<CSingleVolumeInfo> apSingleVolumeInfo(new CSingleVolumeInfo);
		nStatus = apSingleVolumeInfo->Retrieve(VolumeGuid, bGetVolumeBitmap, TRUE);
		if (nStatus)
		{
			D_API_ERR(999, /*DRNPOS,*/ TEXT("Retrieve"), nStatus, TEXT("[VolumeGuid=%s]"), VolumeGuid.GetString());  /*0007*/
		}
		else 
		{
			m_pVolumeInfos.push_back(apSingleVolumeInfo.Detach());
		}
	}

	return nStatus;
}

INT CAllVolumeInfo::WriteToStorage( IStorage * pIStorage )
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
		CComVariant var = (ULONG)(m_pVolumeInfos.size());
		hr = var.WriteToStream(spIStream);
		if (FAILED(hr))
		{	
			D_SET_LAST_STATUS(hr, -1);
			nStatus = hr;
			D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteToStream"), hr, TEXT("StreamName=[%s]"), STG_ITEM_COUNT_NAME); /*0003*/
			break;
		}

		//******************************************
		for (size_t ii=0; ii<m_pVolumeInfos.size(); ++ii)
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
			nStatus = m_pVolumeInfos[ii]->WriteToStorage(spIStorage);
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

INT CAllVolumeInfo::ReadFromStorage( IStorage * pIStorage )
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
			CAutoPtr<CSingleVolumeInfo> apSingleVolumeInfo(new CSingleVolumeInfo);
			nStatus = apSingleVolumeInfo->ReadFromStorage(spIStorage);
			if (nStatus)
			{
				D_API_ERR(0, /*DRNPOS,*/ TEXT("ReadFromStorage"), E_FAIL, TEXT("[index=%u]"), ii);  /*0008*/
				break;
			}
			m_pVolumeInfos.push_back(apSingleVolumeInfo.Detach());

		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

CONST CSingleVolumeInfo* CAllVolumeInfo::FindVolumeByGuid( LPCTSTR pszVolGuid )
{
	CSingleVolumeInfo* pSingleVolumeInfo = NULL;
	for (size_t ii=0; ii<m_pVolumeInfos.size(); ++ii)
	{
		if (CMiscHelper::IsVolumeGuidMatch(pszVolGuid, m_pVolumeInfos[ii]->m_strVolumeGUID))
		{
			pSingleVolumeInfo = m_pVolumeInfos[ii];
			break;
		}
	}
	return pSingleVolumeInfo;
}

CONST CSingleVolumeInfo * CAllVolumeInfo::FindVolume( LPCTSTR pszName )
{
	CSingleVolumeInfo * pRetVolInfo = NULL;;
	for (size_t ii=0; ii<m_pVolumeInfos.size(); ++ii)
	{
		CSingleVolumeInfo * pSingleVolumeInfo = m_pVolumeInfos[ii];
		if (CMiscHelper::IsVolumeGuidMatch(pszName, pSingleVolumeInfo->m_strVolumeGUID))
		{
			pRetVolInfo = pSingleVolumeInfo;
			break;
		}
		else
		{
			CString strName = pszName;
			CMiscHelper::AppendTail(strName, TEXT('\\'));
			for (size_t jj=0; jj<pSingleVolumeInfo->m_Paths.size(); ++jj)
			{
				if (0 == strName.CompareNoCase(pSingleVolumeInfo->m_Paths[jj]))
				{
					pRetVolInfo = pSingleVolumeInfo;
					break;
				}
			}
			if (pRetVolInfo)
			{
				break;
			}
		}
	}

	return pRetVolInfo;
}