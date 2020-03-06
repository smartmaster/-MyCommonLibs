#include "stdafx.h"
#include "FileDeviceForVhdxChainCacheIO.h"




CFileDeviceForVhdxChainCacheIO::CFileDeviceForVhdxChainCacheIO( ParserChainVHDXBase * m_pChainVHDX ) : 
	m_pChainVHDX(m_pChainVHDX),
	m_CurrentBlockIndex(0)
{

}

CFileDeviceForVhdxChainCacheIO::~CFileDeviceForVhdxChainCacheIO()
{

}

HRESULT STDMETHODCALLTYPE CFileDeviceForVhdxChainCacheIO::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void **ppvObject )
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE CFileDeviceForVhdxChainCacheIO::AddRef( void )
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE CFileDeviceForVhdxChainCacheIO::Release( void )
{
	return S_OK;
}

LPCTSTR CFileDeviceForVhdxChainCacheIO::WhoAmI()
{
	return NULL;
}

HRESULT CFileDeviceForVhdxChainCacheIO::CreateMember()
{
	return S_OK;
}

HRESULT CFileDeviceForVhdxChainCacheIO::DestroyMember()
{
	return S_OK;
}

HRESULT CFileDeviceForVhdxChainCacheIO::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	ATLASSERT(FILE_BEGIN == ulMoveMethod);
	ATLASSERT(0 == llDistanceToMove % m_pChainVHDX->m_MaxBlockSize);
	m_CurrentBlockIndex = llDistanceToMove / m_pChainVHDX->m_MaxBlockSize;
	*lpNewFilePointer = llDistanceToMove;
	return S_OK;
}

HRESULT CFileDeviceForVhdxChainCacheIO::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	ATLASSERT(nNumberOfBytesToRead == m_pChainVHDX->m_MaxBlockSize);
	HRESULT hr = m_pChainVHDX->ReadBlocK(TRUE, m_CurrentBlockIndex, pBuffer, nNumberOfBytesToRead);
	if (lpNumberOfBytesRead)
	{
		if (SUCCEEDED(hr))
		{
			*lpNumberOfBytesRead = nNumberOfBytesToRead;
		}
	}
	return hr;
}

HRESULT CFileDeviceForVhdxChainCacheIO::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	ATLASSERT(nNumberOfBytesToWrite == m_pChainVHDX->m_MaxBlockSize);
	HRESULT hr = m_pChainVHDX->WriteBlocK(m_CurrentBlockIndex, pBuffer, nNumberOfBytesToWrite);
	if (lpNumberOfBytesWritten)
	{
		if (SUCCEEDED(hr))
		{
			*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
		}
	}
	return hr;
}

HRESULT CFileDeviceForVhdxChainCacheIO::GetFileSize( LONGLONG * pFileSize )
{
	return S_OK;
}

HANDLE CFileDeviceForVhdxChainCacheIO::GetRawHandle()
{
	return INVALID_HANDLE_VALUE;
}
