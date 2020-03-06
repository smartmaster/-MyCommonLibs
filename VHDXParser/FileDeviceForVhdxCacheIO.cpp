#include "stdafx.h"
#include "FileDeviceForVhdxCacheIO.h"




CFileDeviceForVhdxCacheIO::CFileDeviceForVhdxCacheIO( ParserSeparateVHDX * pParserVHDXMetadata ) : 
	m_pSeparateVHDX(pParserVHDXMetadata),
	m_CurrentBlockIndex(0)
{

}

CFileDeviceForVhdxCacheIO::~CFileDeviceForVhdxCacheIO()
{

}

HRESULT STDMETHODCALLTYPE CFileDeviceForVhdxCacheIO::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ void **ppvObject )
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE CFileDeviceForVhdxCacheIO::AddRef( void )
{
	return S_OK;
}

ULONG STDMETHODCALLTYPE CFileDeviceForVhdxCacheIO::Release( void )
{
	return S_OK;
}

LPCTSTR CFileDeviceForVhdxCacheIO::WhoAmI()
{
	return NULL;
}

HRESULT CFileDeviceForVhdxCacheIO::CreateMember()
{
	return S_OK;
}

HRESULT CFileDeviceForVhdxCacheIO::DestroyMember()
{
	return S_OK;
}

HRESULT CFileDeviceForVhdxCacheIO::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	ATLASSERT(FILE_BEGIN == ulMoveMethod);
	ATLASSERT(0 == llDistanceToMove % m_pSeparateVHDX->m_FileParameters.m_BlockSize);
	m_CurrentBlockIndex = llDistanceToMove / m_pSeparateVHDX->m_FileParameters.m_BlockSize;
	*lpNewFilePointer = llDistanceToMove;
	return S_OK;
}

HRESULT CFileDeviceForVhdxCacheIO::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	ATLASSERT(nNumberOfBytesToRead == m_pSeparateVHDX->m_FileParameters.m_BlockSize);
	HRESULT hr = m_pSeparateVHDX->ReadBlocK(TRUE, m_CurrentBlockIndex, pBuffer, nNumberOfBytesToRead);
	if (lpNumberOfBytesRead)
	{
		if (SUCCEEDED(hr))
		{
			*lpNumberOfBytesRead = nNumberOfBytesToRead;
		}
	}
	return hr;
}

HRESULT CFileDeviceForVhdxCacheIO::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	ATLASSERT(nNumberOfBytesToWrite == m_pSeparateVHDX->m_FileParameters.m_BlockSize);
	HRESULT hr = m_pSeparateVHDX->WriteBlocK(m_CurrentBlockIndex, pBuffer, nNumberOfBytesToWrite);
	if (lpNumberOfBytesWritten)
	{
		if (SUCCEEDED(hr))
		{
			*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
		}
	}
	return hr;
}

HRESULT CFileDeviceForVhdxCacheIO::GetFileSize( LONGLONG * pFileSize )
{
	return S_OK;
}

HANDLE CFileDeviceForVhdxCacheIO::GetRawHandle()
{
	return INVALID_HANDLE_VALUE;
}
