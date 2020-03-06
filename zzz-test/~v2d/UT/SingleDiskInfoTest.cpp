#include "stdafx.h"
#include "..\CommonFunctions\SingleDiskInfo.h"

INT SingleDiskInfoTest()
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr= S_OK;

	vector<DWORD> Disknumbers;
	for (DWORD dwDiskNumber=0; dwDiskNumber<3; ++dwDiskNumber)
	{
		Disknumbers.push_back(dwDiskNumber);
	}
	
	for (size_t ii=0; ii<Disknumbers.size(); ++ii)
	{
		CSingleDiskInfo sdi00;
		sdi00.Retrieve(Disknumbers[ii], FALSE);

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
		nStatus = sdi00.WriteToStorage(spIStorage);
		if (nStatus)
		{
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("WriteToStorage"), E_FAIL));
			break;
		}

		//******************************************
		CSingleDiskInfo sdi01;
		nStatus = sdi01.ReadFromStorage(spIStorage);
		if (nStatus)
		{
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFromStorage"), E_FAIL));
			break;
		}
	}

	return nStatus;
}

VOID TestAllDiskInfo()
{
	CAllDiskInfo adi;
	adi.Retrieve(8);

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

	adi.WriteToStorage(spIStorage);
	CAllDiskInfo adi01;
	adi01.ReadFromStorage(spIStorage);

	return;
}