#include "stdafx.h"
#include "RWLockWin32.h"

//#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
//namespace DTRACELIB_N {
//#endif




CRWLockWin32::CRWLockWin32() :
m_RefCount(0)
{

}


HRESULT CRWLockWin32::SharedUnlock()
{
	m_pImpl->LeaveReader();
	return S_OK;
}

HRESULT CRWLockWin32::SharedLock()
{
	m_pImpl->EnterReader();
	return S_OK;
}

HRESULT CRWLockWin32::Unlock()
{
	m_pImpl->LeaveWriter();
	return S_OK;
}

HRESULT CRWLockWin32::Lock()
{
	m_pImpl->EnterWriter();
	return S_OK;
}

HRESULT CRWLockWin32::DestroyMember()
{
	m_pImpl.Free();
	return S_OK;
}

HRESULT CRWLockWin32::CreateMember()
{
	HRESULT hr = S_OK;
	m_pImpl.Attach(new MultiReaderSingleWriter());
	if (nullptr == m_pImpl.m_p)
	{
		D_API_ERR(0, TEXT("new"), hr, TEXT("..."));
		hr = E_OUTOFMEMORY;
	}
	return hr;
}

HRESULT CreateInstacnceCRWLockWin32(IThreadRWLock ** ppIRWLock, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(/*CRWLock,*/ (new CRWLockWin32()), ppIRWLock, bCreateMember);
}

//#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
//}
//#endif