#include "stdafx.h"

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "RWLock.h"

#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
namespace DTRACELIB_N {
#endif

CRWLock::CRWLock() :
	m_RefCount(0)
{

}

CRWLock::~CRWLock()
{
	DestroyMember();
}

LPCTSTR CRWLock::WhoAmI()
{
	static CONST TCHAR CLASS_NAME[] = TEXT("CRWLock:IRWLock");
	return CLASS_NAME;
}

HRESULT CRWLock::CreateMember()
{
	HRESULT hr = S_OK;

	InitializeSRWLock(
		&SRWLock//_Out_  PSRWLOCK SRWLock
		);

	return hr;
}

HRESULT CRWLock::DestroyMember()
{
	return S_OK;
}

HRESULT CRWLock::Lock()
{
	AcquireSRWLockExclusive(
		&SRWLock//_Inout_  PSRWLOCK SRWLock
		);
	return S_OK;
}

HRESULT CRWLock::Unlock()
{
	ReleaseSRWLockExclusive(
		&SRWLock//_Inout_  PSRWLOCK SRWLock
		);
	return S_OK;
}

HRESULT CRWLock::SharedLock()
{
	AcquireSRWLockShared(
		&SRWLock//_Inout_  PSRWLOCK SRWLock
		);
	return S_OK;
}

HRESULT CRWLock::SharedUnlock()
{
	ReleaseSRWLockShared(
		&SRWLock//_Inout_  PSRWLOCK SRWLock
		);
	return S_OK;
}

HRESULT CreateInstacnceSRWLock( IThreadRWLock ** ppIRWLock, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CRWLock,*/ (new CRWLock()), ppIRWLock, bCreateMember);
	//HRESULT hr = S_OK;

	//BEGIN_BLOCK(0);
	////////////////////////////////////////////////////////////////////////////
	//CRWLock * pCRWLock = new CRWLock();
	//if (NULL == pCRWLock)
	//{
	//	hr = E_OUTOFMEMORY;
	//	LEAVE_BLOCK(0);
	//}
	//
	////////////////////////////////////////////////////////////////////////////
	//pCRWLock->AddRef();
	//if (bCreateMember)
	//{
	//	hr = pCRWLock->CreateMember();
	//	if (FAILED(hr))
	//	{
	//		pCRWLock->Release();
	//		LEAVE_BLOCK(0);
	//	}
	//}

	////////////////////////////////////////////////////////////////////////////
	//*ppIRWLock = pCRWLock;
	//END_BLOCK(0);
	//
	//return hr;
}

#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
};  //end namespace DTRACELIB_N
#endif