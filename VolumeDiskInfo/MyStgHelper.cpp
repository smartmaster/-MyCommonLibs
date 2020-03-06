#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"
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
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("CreateILockBytesOnHGlobal"), hr, TEXT("")); /*0004*/
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
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("StgCreateDocfileOnILockBytes"), hr, TEXT("")); /*0004*/
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
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, /*DRNPOS,*/ TEXT("StgCreateStorageEx"), hr, TEXT("[FileName=%s]"), szFileName);
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
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, /*DRNPOS,*/ TEXT("StgOpenStorageEx"), hr, TEXT("[FileName=%s]"), szFileName);
	}
	return hr;
}