// DiskVolumeInfoObject.cpp : Implementation of CDiskVolumeInfoObject

#include "stdafx.h"
#include "DiskVolumeInfoObject.h"


// CDiskVolumeInfoObject
#include "..\CommonFunctions\SingleDiskInfo.h"
#include "..\CommonFunctions\SingleVolumeInfo.h"
#include "..\CommonFunctions\AllVdsDiskVolumeInfo.h"



STDMETHODIMP CDiskVolumeInfoObject::WriteDiskInfo(IStorage* pIStorage)
{
	// TODO: Add your implementation code here
	ObjectLock lock(this);

	DSTART(999);

	INT nStatus = 0;
	HRESULT hr = S_OK;

	CAllDiskInfo MyAllDiskInfo;
	CONST DWORD MAX_DISK_NUMBER = 256;
	MyAllDiskInfo.Retrieve(MAX_DISK_NUMBER);

	nStatus = MyAllDiskInfo.WriteToStorage(pIStorage);
	if (nStatus)
	{
		_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStorage"), E_FAIL));
		hr = E_FAIL;
	}

	return hr;
}


STDMETHODIMP CDiskVolumeInfoObject::WriteVolumeInfo(IStorage* pIStorage ,  LONG bGetVolumeBitmap)
{
	// TODO: Add your implementation code here
	ObjectLock lock(this);

	DSTART(999);

	INT nStatus = 0;
	HRESULT hr = S_OK;

	CAllVolumeInfo MyAllVolumeInfo;
	MyAllVolumeInfo.Retrieve(bGetVolumeBitmap);

	nStatus = MyAllVolumeInfo.WriteToStorage(pIStorage);
	if (nStatus)
	{
		_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStorage"), E_FAIL));
		hr = E_FAIL;
	}

	return hr;
}



STDMETHODIMP CDiskVolumeInfoObject::WriteVdsDiskVolumeInfo(IStorage* pIStorageDisk ,  IStorage* pIStorageVolume)
{
	// TODO: Add your implementation code here
	ObjectLock lock(this);

	DSTART(999);

	INT nStatus = 0;
	HRESULT hr = S_OK;

	CAllVdsDiskVolumeInfo MyAllVdsDiskVolumeInfo;
	MyAllVdsDiskVolumeInfo.Retrieve();

	nStatus = MyAllVdsDiskVolumeInfo.WriteVdsDiskToStorage(pIStorageDisk);
	if (nStatus)
	{
		_OLD_DAPIERR((0, DRNPOS, TEXT("WriteVdsDiskToStorage"), E_FAIL));
		hr = E_FAIL;
	}

	nStatus = MyAllVdsDiskVolumeInfo.WriteVdsVolumeToStorage(pIStorageVolume);
	if (nStatus)
	{
		_OLD_DAPIERR((0, DRNPOS, TEXT("WriteVdsVolumeToStorage"), E_FAIL));
		hr = E_FAIL;
	}

	return hr;
}

STDMETHODIMP CDiskVolumeInfoObject::WriteSingleDiskInfo(ULONG ulDiskNumber ,  IStorage* pIStorage , LONG bBreakOnError)
{
	// TODO: Add your implementation code here
	ObjectLock lock(this);

	DSTART(999);

	INT nStatus = 0;
	HRESULT hr = S_OK;
	
	do 
	{
		//******************************************
		CSingleDiskInfo SingleDiskInfo;
		nStatus = SingleDiskInfo.Retrieve(ulDiskNumber, bBreakOnError);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("Retrieve"), E_FAIL, TEXT("[DiskNumber=%u]"), ulDiskNumber));
			hr = E_FAIL;
			break;
		}

		//******************************************
		nStatus = SingleDiskInfo.WriteToStorage(pIStorage);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStorage"), E_FAIL, TEXT("[DiskNumber=%u]"), ulDiskNumber));
			hr = E_FAIL;
			break;
		}

	} while (FALSE);
	
	return hr;
}


STDMETHODIMP CDiskVolumeInfoObject::WriteSingleVolumeInfo(BSTR bstrVolume , LONG bGetVolumeBitmap , IStorage* pIStorage , LONG bBreakOnError)
{
	// TODO: Add your implementation code here
	ObjectLock lock(this);

	INT nStatus = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CSingleVolumeInfo SingleVolumeInfo;
		nStatus = SingleVolumeInfo.Retrieve(bstrVolume, bGetVolumeBitmap, bBreakOnError);
		if (nStatus)
		{
			hr = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Retrieve"), hr, TEXT("[Volume=%s]"), bstrVolume));
			break;
		}

		//******************************************
		nStatus = SingleVolumeInfo.WriteToStorage(pIStorage);
		if (nStatus)
		{
			hr = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStorage"), hr, TEXT("[Volume=%s]"), bstrVolume));
			break;
		}

	} while (FALSE);

	return hr;
}

