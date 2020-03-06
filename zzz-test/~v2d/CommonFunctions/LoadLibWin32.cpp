#include "stdafx.h"
#include "LoadLibWin32.h"

HRESULT CreateInstanceILoadLib( LPCTSTR pModuleName, BOOL bCreateMember, ILoadLib ** ppILoadLib )
{
	HRESULT		hr = S_OK;
	DWORD		LastErro = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ILoadLib * pLoadLib = new CLoadLib(pModuleName);
	if (NULL == pLoadLib)
	{
		hr = E_OUTOFMEMORY;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	pLoadLib->AddRef();
	if (bCreateMember)
	{
		hr = pLoadLib->CreateMember();
		if (FAILED(hr))
		{
			pLoadLib->Release();
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	*ppILoadLib = pLoadLib;
	END_BLOCK(0);

	return hr;
}
