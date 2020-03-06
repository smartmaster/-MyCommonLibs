#include "stdafx.h"
#include "..\ServerVssLayer\ServerVssLayer_i.h"
#include "..\ServerVssLayer\MyVssSnapshotProp.h"
#include "..\CommonFunctions\MyStgHelper.h"

INT VShadowClientTest(INT argc, TCHAR ** argv)
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		CComPtr<IVssLayerObject> spIVssShadowClient;
		hr = spIVssShadowClient.CoCreateInstance(__uuidof(VssLayerObject));
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CoCreateInstance"), hr));
			break;
		}

		//******************************************
		CComSafeArray<BSTR> spsa;
		for (INT ii=1; ii<argc; ++ii)
		{
			CString strVolume = argv[ii];
			spsa.Add(strVolume.AllocSysString(), FALSE);
		}

		//******************************************
		CComPtr<ILockBytes> spILockBytesXmls;
		CComPtr<IStorage> spIStorageXmls;
		CMyStgHelper::CreateFileStorage( 
			TEXT("my-wm-delete-me.stg"),//LPCTSTR szFileName, 
			&spIStorageXmls,//IStorage ** ppIStorage, 
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE//DWORD grfMode
			);


		//******************************************
		CComPtr<ILockBytes> spILockBytesRes;
		CComPtr<IStorage> spIStorageRes;
		CMyStgHelper::CreateMemoryStorage(
			&spILockBytesRes,//ILockBytes ** ppILockBytes, 
			&spIStorageRes,//IStorage ** ppIStorage,  
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE//DWORD grfMode
			);

		//******************************************
		hr = spIVssShadowClient->ShadowCopy(
			spsa,//SAFEARRAY * psaArgvs, 
			spIStorageXmls,//IStorage * pISorageXmls, 
			spIStorageRes//IStorage * pISorageVssResults
			);

		vector<MY_VSS_SNAPSHOT_PROP> MyVssSnapshotProps;
		MY_VSS_SNAPSHOT_PROP::ReadFromVectorRootStorage(spIStorageRes, MyVssSnapshotProps);


		//******************************************
	} while (FALSE);

	return nStatus;
}