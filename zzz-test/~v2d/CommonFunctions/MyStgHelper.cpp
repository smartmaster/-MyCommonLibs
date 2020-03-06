#include "stdafx.h"
#include "MyStgHelper.h"


HRESULT CMyStgHelper::CreateMemoryStorage( ILockBytes ** ppILockBytes, IStorage ** ppIStorage, DWORD grfMode )
{
	HRESULT hr = S_OK;

	do 
	{
		hr = CreateILockBytesOnHGlobal(
			NULL,//__in          HGLOBAL hGlobal,
			TRUE,//__in          BOOL fDeleteOnRelease,
			ppILockBytes//__out         ILockBytes** ppLkbyt
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateILockBytesOnHGlobal"), hr));
			break;
		}

		hr = StgCreateDocfileOnILockBytes(
			*ppILockBytes,//__in          ILockBytes* plkbyt,
			grfMode,//__in          DWORD grfMode,
			0,//__in          DWORD reserved,
			ppIStorage//__out         IStorage** ppstgOpen
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("StgCreateDocfileOnILockBytes"), hr));
			break;
		}

	} while (FALSE);

	return hr;
}

HRESULT CMyStgHelper::CreateFileStorage( LPCTSTR szFileName, IStorage ** ppIStorage, DWORD grfMode )
{
	HRESULT hr = StgCreateStorageEx(
		szFileName,//__in          const WCHAR* pwcsName,
		grfMode,//__in          DWORD grfMode,
		STGFMT_STORAGE,//__in          STGFMT stgfmt,
		0,//__in          DWORD grfAttrs,
		NULL,//__in          STGOPTIONS* pStgOptions,
		NULL,//__in          PSECURITY_DESCRIPTOR* pSecurityDescriptor,
		__uuidof(IStorage),//__in          REFIID riid,
		(VOID**)ppIStorage//__out         void** ppObjectOpen
		);
	if (FAILED(hr))
	{	
		_OLD_DSETLASTSTATUS(hr);
		_OLD_DAPIERR((0, DRNPOS, TEXT("StgCreateStorageEx"), hr, TEXT("[FileName=%s]"), szFileName));
	}
	return hr;
}

HRESULT CMyStgHelper::OpenFileStorage( LPCTSTR szFileName, IStorage ** ppIStorage, DWORD grfMode )
{
	HRESULT hr = StgOpenStorageEx(
		szFileName,//__in          const WCHAR* pwcsName,
		grfMode,//__in          DWORD grfMode,
		STGFMT_STORAGE,//__in          STGFMT stgfmt,
		0,//__in          DWORD grfAttrs,
		NULL,//__in          STGOPTIONS* pStgOptions,
		NULL,//void* reserved2,
		__uuidof(IStorage),//__in          REFIID riid,
		(VOID**)ppIStorage//__out         void** ppObjectOpen
		);
	if (FAILED(hr))
	{	
		_OLD_DSETLASTSTATUS(hr);
		_OLD_DAPIERR((0, DRNPOS, TEXT("StgOpenStorageEx"), hr, TEXT("[FileName=%s]"), szFileName));
	}
	return hr;
}