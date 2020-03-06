#include "stdafx.h"
#include "LoadLibWin32.h"

#include "..\PublicHeader\PublicDTraceLib.h"

CLoadLib::CLoadLib( LPCTSTR pModuleName ) :
	m_hMoudle(NULL),
	m_ModuleName(pModuleName),
	m_RefCount(0)
{

}


LPCTSTR CLoadLib::WhoAmI()
{
	static CONST TCHAR WHO_AM_I[] = TEXT("CLoadLib:ILoadLib");
	return WHO_AM_I;
}


HRESULT CLoadLib::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (NULL == m_hMoudle)
	{
		m_hMoudle = LoadLibrary(m_ModuleName);
		if (NULL == m_hMoudle)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("LoadLibrary"), hr, TEXT("%s"), m_ModuleName.GetString());
		}
	}
	return hr;
}

HRESULT CLoadLib::DestroyMember()
{
	if (m_hMoudle)
	{
		FreeLibrary(m_hMoudle);
		m_hMoudle = NULL;
	}
	return S_OK;
}

FARPROC CLoadLib::GetProc( LPCTSTR pProcName )
{
	HRESULT hr = S_OK;
	DWORD	LastError = 0;
	FARPROC ProcAddr = NULL;

	BEGIN_BLOCK(0);
	hr = CreateMember();
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	CStringA ProcNameA(pProcName);
	ProcAddr = GetProcAddress(m_hMoudle, ProcNameA.GetString());
	if (NULL == ProcAddr)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("GetProcAddress"), hr, TEXT("%s"), pProcName);
	}
	END_BLOCK(0);

	return ProcAddr;
}


HRESULT CreateInstanceCLoadLib( LPCTSTR pModuleName, ILoadLib ** ppILoadLib , BOOL bCreateMember)
{

	return CREATE_INSTCANCE_COBJECT(/*CLoadLib,*/ new CLoadLib(pModuleName), ppILoadLib, bCreateMember);
	//HRESULT		hr = S_OK;
	//DWORD		LastErro = 0;

	//BEGIN_BLOCK(0);
	////////////////////////////////////////////////////////////////////////////
	//ILoadLib * pLoadLib = new CLoadLib(pModuleName);
	//if (NULL == pLoadLib)
	//{
	//	hr = E_OUTOFMEMORY;
	//	break;
	//}

	////////////////////////////////////////////////////////////////////////////
	//pLoadLib->AddRef();
	//if (bCreateMember)
	//{
	//	hr = pLoadLib->CreateMember();
	//	if (FAILED(hr))
	//	{
	//		pLoadLib->Release();
	//		break;
	//	}
	//}

	////////////////////////////////////////////////////////////////////////////
	//*ppILoadLib = pLoadLib;
	//END_BLOCK(0);

	//return hr;
}