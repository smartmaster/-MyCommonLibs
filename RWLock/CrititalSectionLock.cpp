#include "stdafx.h"

#include <tchar.h>
#include <windows.h>

#include "..\PublicHeader\PublicCodeBlock.h"


#include "CrititalSectionLock.h"

#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
namespace DTRACELIB_N {
#endif

CCiticalSectionLock::CCiticalSectionLock() :
	m_RefCount(0)
{

}

LPCTSTR CCiticalSectionLock::WhoAmI()
{
	static CONST TCHAR CLASS_NAME[] = TEXT("CCiticalSectionLock:IRWLock");
	return CLASS_NAME;
}

HRESULT CCiticalSectionLock::CreateMember()
{
	InitializeCriticalSection(
		&m_CS//_Out_  LPCRITICAL_SECTION lpCriticalSection
		);
	return S_OK;
}

HRESULT CCiticalSectionLock::DestroyMember()
{
	DeleteCriticalSection(
		&m_CS//_Inout_  LPCRITICAL_SECTION lpCriticalSection
		);
	return S_OK;
}

HRESULT CCiticalSectionLock::Lock()
{
	EnterCriticalSection(
		&m_CS//_Inout_  LPCRITICAL_SECTION lpCriticalSection
		);
	return S_OK;
}

HRESULT CCiticalSectionLock::Unlock()
{
	LeaveCriticalSection(
		&m_CS//_Inout_  LPCRITICAL_SECTION lpCriticalSection
		);
	return S_OK;
}

HRESULT CCiticalSectionLock::SharedLock()
{
	return Lock();
}

HRESULT CCiticalSectionLock::SharedUnlock()
{
	return Unlock();
}

HRESULT CreateInstacnceCriticalSectionLock(IThreadRWLock ** ppLock, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(/*CCiticalSectionLock,*/ (new CCiticalSectionLock()), ppLock, bCreateMember);
	//HRESULT hr = S_OK;

	//BEGIN_BLOCK(0);
	////////////////////////////////////////////////////////////////////////////
	//CCiticalSectionLock * pCCiticalSectionLock = new CCiticalSectionLock();
	//if (NULL == pCCiticalSectionLock)
	//{
	//	hr = E_OUTOFMEMORY;
	//	LEAVE_BLOCK(0);
	//}

	////////////////////////////////////////////////////////////////////////////
	//pCCiticalSectionLock->AddRef();
	//if (bCreateMember)
	//{
	//	hr = pCCiticalSectionLock->CreateMember();
	//	if (FAILED(hr))
	//	{
	//		pCCiticalSectionLock->Release();
	//		LEAVE_BLOCK(0);
	//	}
	//}

	////////////////////////////////////////////////////////////////////////////
	//*ppLock = pCCiticalSectionLock;
	//END_BLOCK(0);

	//return hr;
}


#ifdef __76E26A7E_D764_4685_BFD9_1A40D6A0AFF1_DLL_API_
};  //end namespace DTRACELIB_N
#endif