#include "stdafx.h"
#include "WSAIoBuffer.h"


TWSAIoBuffer::TWSAIoBuffer( ULONG AllocatedSize ) :
m_DataLengthReady(FALSE),
m_DataLength(0),
m_Transfered(0),
m_AllocatedSize(AllocatedSize),
m_pBuffer(NULL)
{

}

HRESULT TWSAIoBuffer::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	m_pBuffer = (BYTE*)VirtualAlloc(
		NULL,//__in_opt  LPVOID lpAddress,
		m_AllocatedSize,//__in      SIZE_T dwSize,
		MEM_COMMIT | MEM_RESERVE,//__in      DWORD flAllocationType,
		PAGE_READWRITE//__in      DWORD flProtect
		);
	if (NULL == m_pBuffer)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DSETLASTSTATUS(hr);
		DAPIERR((0, DRNPOS, TEXT("VirtualAlloc"), hr));
	}

	return hr;
}

HRESULT TWSAIoBuffer::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (m_pBuffer)
	{
		m_AllocatedSize = 0;
		BOOL bRet = VirtualFree(
			m_pBuffer,//__in  LPVOID lpAddress,
			0,//__in  SIZE_T dwSize,
			MEM_RELEASE//__in  DWORD dwFreeType
			);
		m_pBuffer = NULL;
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("VirtualFree"), hr));
		}
	}

	return hr;
}

TWSAIoBuffer::~TWSAIoBuffer()
{
	DestroyMember();
}

HRESULT TWSAIoBuffer::RellocateBuffer( ULONG Length )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = VirtualFree(
		m_pBuffer,//__in  LPVOID lpAddress,
		0,//__in  SIZE_T dwSize,
		MEM_RELEASE//__in  DWORD dwFreeType
		);
	m_pBuffer = NULL;
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DSETLASTSTATUS(hr);
		DAPIERR((0, DRNPOS, TEXT("VirtualFree"), hr));
	}

	m_AllocatedSize = Length;
	m_pBuffer = (BYTE*)VirtualAlloc(
		NULL,//__in_opt  LPVOID lpAddress,
		m_AllocatedSize,//__in      SIZE_T dwSize,
		MEM_COMMIT | MEM_RESERVE,//__in      DWORD flAllocationType,
		PAGE_READWRITE//__in      DWORD flProtect
		);
	if (NULL == m_pBuffer)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DSETLASTSTATUS(hr);
		DAPIERR((0, DRNPOS, TEXT("VirtualAlloc"), hr));
	}

	return hr;
}
