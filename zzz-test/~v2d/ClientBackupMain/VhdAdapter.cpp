#include "stdafx.h"
#include "VhdAdapter.h"


HRESULT STDMETHODCALLTYPE CVHDAdapter::SetPointerEx( /* [in] */ LONGLONG llDistanceToMove, /* [out] */ LONGLONG *lpNewFilePointer, /* [in] */ ULONG ulMoveMethod )
{
	return m_pVHD->Seek(llDistanceToMove, lpNewFilePointer, ulMoveMethod);
}

HRESULT STDMETHODCALLTYPE CVHDAdapter::Read( /* [in] */ ULONG nNumberOfBytesToRead, /* [out] */ ULONG *lpNumberOfBytesRead, /* [length_is][size_is][out] */ BYTE *pBuffer )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CVHDAdapter::Write( /* [in] */ ULONG nNumberOfBytesToWrite, /* [out] */ ULONG *lpNumberOfBytesWritten, /* [size_is][in] */ BYTE *pBuffer )
{
	return m_pVHD->Write((CONST CHAR*)(pBuffer), nNumberOfBytesToWrite, lpNumberOfBytesWritten, (LONGLONG)(-1));
}

HRESULT STDMETHODCALLTYPE CVHDAdapter::Create( /* [in] */ BSTR bstrFileName, /* [in] */ ULONG ulDesiredAccess, /* [in] */ ULONG ulShareMode, /* [in] */ ULONG ulCreationDisposition, /* [in] */ ULONG ulFlagsAndAttributes )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CVHDAdapter::Close( void )
{
	if (m_pVHD)
	{
		m_pVHD->Release();
		m_pVHD = NULL;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CVHDAdapter::GetFileSizeEx( /* [out] */ LONGLONG *pllFileSize )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CVHDAdapter::QueryInterface( REFIID riid, void **ppvObject )
{
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CVHDAdapter::AddRef( void )
{
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CVHDAdapter::Release( void )
{
	delete this;
	return S_OK;
}

CVHDAdapter::~CVHDAdapter()
{
	Close();
}

CVHDAdapter::CVHDAdapter( HaVhdUtility::IVHDFileW * pVHD ) : 
m_pVHD(pVHD)
{
}

VOID CVHDAdapter::Attach( HaVhdUtility::IVHDFileW * pVHD )
{
	m_pVHD = pVHD;
}
