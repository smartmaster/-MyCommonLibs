#include "stdafx.h"

#include "..\ServerDiskVolumeInfo\ServerDiskVolumeInfo_i.h"
#include "..\ServerFileDevice\ServerFileDevice_i.h"
#include "..\ServerVssLayer\ServerVssLayer_i.h"

#include "..\CommonFunctions\MyStgHelper.h"
#include "..\CommonFunctions\SingleDiskInfo.h"
#include "..\CommonFunctions\SingleVolumeInfo.h"
#include "..\CommonFunctions\AllVdsDiskVolumeInfo.h"

HRESULT AllDiskVolumeInformationTest()
{
	HRESULT hr = S_OK;

	do 
	{
		CComPtr<IDiskVolumeInfoObject> spIAllDiskVolumeInformation;
		hr = spIAllDiskVolumeInformation.CoCreateInstance(__uuidof(DiskVolumeInfoObject));
		if (FAILED(hr))
		{	
			break;
		}

		{
			CComPtr<ILockBytes> spILockBytes;
			CComPtr<IStorage> spIStorage;
			DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
			hr = CMyStgHelper::CreateMemoryStorage(&spILockBytes, &spIStorage,  grfMode);
			if (SUCCEEDED(hr))
			{
				hr = spIAllDiskVolumeInformation->WriteDiskInfo(spIStorage);
			}

			if (SUCCEEDED(hr))
			{
				CAllDiskInfo MyAllDiskInfo;
				MyAllDiskInfo.ReadFromStorage(spIStorage);
			}
		}

		{
			TCHAR szFileName[] = TEXT("bbb.stg");
			CComPtr<IStorage> spIStorage;
			DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE;
			hr = CMyStgHelper::CreateFileStorage(szFileName, &spIStorage,  grfMode);
			if (SUCCEEDED(hr))
			{
				hr = spIAllDiskVolumeInformation->WriteVolumeInfo(spIStorage, FALSE);
			}

			if (SUCCEEDED(hr))
			{
				CAllVolumeInfo MyAllVolumeInfo;
				MyAllVolumeInfo.ReadFromStorage(spIStorage);
			}
		}

		{
			TCHAR szFileName[] = TEXT("bbb.stg");
			CComPtr<IStorage> spIStorage;
			DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
			hr = CMyStgHelper::OpenFileStorage(szFileName, &spIStorage,  grfMode);
			if (SUCCEEDED(hr))
			{
				CAllVolumeInfo MyAllVolumeInfo;
				MyAllVolumeInfo.ReadFromStorage(spIStorage);
			}
		}

		{
			TCHAR szFileNameDisk[] = TEXT("ccc.stg");
			CComPtr<IStorage> spIStorageDisk;
			DWORD grfModeDisk = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_DELETEONRELEASE;
			hr = CMyStgHelper::CreateFileStorage(szFileNameDisk, &spIStorageDisk,  grfModeDisk);

			TCHAR szFileName[] = TEXT("ddd.stg");
			CComPtr<IStorage> spIStorage;
			DWORD grfMode = STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_DELETEONRELEASE;
			hr = CMyStgHelper::CreateFileStorage(szFileName, &spIStorage,  grfMode);

			if (SUCCEEDED(hr))
			{
				hr = spIAllDiskVolumeInformation->WriteVdsDiskVolumeInfo(spIStorageDisk, spIStorage);
			}

			if (SUCCEEDED(hr))
			{
				CAllVdsDiskVolumeInfo MyAllVdsDiskVolumeInfo;
				MyAllVdsDiskVolumeInfo.ReadVdsDiskFromStorage(spIStorageDisk);
				MyAllVdsDiskVolumeInfo.ReadVdsVolumeFromStorage(spIStorage);
			}
		}

	} while (FALSE);
	
	return hr;
}