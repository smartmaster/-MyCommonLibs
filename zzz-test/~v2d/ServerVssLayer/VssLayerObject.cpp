// VssLayerObject.cpp : Implementation of CVssLayerObject

#include "stdafx.h"
#include "VssLayerObject.h"


// CVssLayerObject

extern HRESULT VShadowComMainEntry(SAFEARRAY * psaArgvs, IStorage * pISorageXmls, IStorage * pISorageVssResults, DWORD * pdwCookieIVssBackupComponents);

STDMETHODIMP CVssLayerObject::ShadowCopy(SAFEARRAY *  psaArgvs, IStorage * pISorageXmls , IStorage * pISorageVssResults)
{
	// TODO: Add your implementation code here
	ObjectLock lock(this);
	DSTART(999);
	DWORD dwCookieIVssBackupComponents = 0;
	HRESULT hr = VShadowComMainEntry(psaArgvs, pISorageXmls, pISorageVssResults, &dwCookieIVssBackupComponents);
	if (dwCookieIVssBackupComponents)
	{
		m_spGITPtrIVssBackupComponents.Attach(dwCookieIVssBackupComponents);
	}
	return hr;
}


STDMETHODIMP CVssLayerObject::VssBackupComponentsRelease(void)
{
	// TODO: Add your implementation code here
	ObjectLock lock(this);
	DSTART(999);
	CleanupIVssBackupComponents();
	return S_OK;
}

VOID CVssLayerObject::CleanupIVssBackupComponents()
{
	m_spGITPtrIVssBackupComponents.Revoke();
}