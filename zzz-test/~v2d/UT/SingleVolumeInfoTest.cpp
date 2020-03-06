#include "stdafx.h"
#include "..\CommonFunctions\SingleVolumeInfo.h"

INT SingleVolumeInfoTest()
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr= S_OK;

	vector<CString> VolumeGuid;
	CVolumeHelper::GetAllVolumeGuid(VolumeGuid);
	for (size_t ii=0; ii<VolumeGuid.size(); ++ii)
	{
		CSingleVolumeInfo svi00;
		svi00.Retrieve(VolumeGuid[ii], TRUE, FALSE);

		//******************************************
		CComPtr<ILockBytes> spILockBytes;
		hr =  ::CreateILockBytesOnHGlobal(
			NULL,//__in          HGLOBAL hGlobal,
			TRUE,//__in          BOOL fDeleteOnRelease,
			&spILockBytes//__out         ILockBytes** ppLkbyt
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateILockBytesOnHGlobal"), hr));
			break;
		}

		CComPtr<IStorage> spIStorage;
		hr = ::StgCreateDocfileOnILockBytes(
			spILockBytes,//__in          ILockBytes* plkbyt,
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//__in          DWORD grfMode,
			0,//__in          DWORD reserved,
			&spIStorage//__out         IStorage** ppstgOpen
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("___com_error_name"), hr));
			break;
		}

		//******************************************
		nStatus = svi00.WriteToStorage(spIStorage);
		if (nStatus)
		{
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStorage"), E_FAIL));
			break;
		}

		//******************************************
		CSingleVolumeInfo svi01;
		nStatus = svi01.ReadFromStorage(spIStorage);
		if (nStatus)
		{
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStorage"), E_FAIL));
			break;
		}
	}

	return nStatus;
}

VOID TestAllVolumeInfo()
{
	CAllVolumeInfo avi;
	avi.Retrieve(FALSE);

	//******************************************
	CComPtr<ILockBytes> spILockBytes;
	::CreateILockBytesOnHGlobal(
		NULL,//__in          HGLOBAL hGlobal,
		TRUE,//__in          BOOL fDeleteOnRelease,
		&spILockBytes//__out         ILockBytes** ppLkbyt
		);

	CComPtr<IStorage> spIStorage;
	::StgCreateDocfileOnILockBytes(
		spILockBytes,//__in          ILockBytes* plkbyt,
		STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,//__in          DWORD grfMode,
		0,//__in          DWORD reserved,
		&spIStorage//__out         IStorage** ppstgOpen
		);

	avi.WriteToStorage(spIStorage);
	CAllVolumeInfo avi01;
	avi01.ReadFromStorage(spIStorage);

	return;
}

VOID TestShadowCopy(LPCTSTR szShadowCopy)
{
	CSingleVolumeInfo svi00;
	svi00.Retrieve(szShadowCopy, TRUE, FALSE);
	return;
}