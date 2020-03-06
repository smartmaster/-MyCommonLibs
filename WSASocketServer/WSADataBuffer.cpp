#include "stdafx.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "WSADataBuffer.h"


WSADataBufferT::WSADataBufferT( ULONG AllocatedSize ) :
	m_DataLengthReady(FALSE),
	m_DataLength(0),
	m_Transfered(0),
	m_AllocatedSize(AllocatedSize),
	m_pData(NULL)
{

}

WSADataBufferT::~WSADataBufferT()
{
	DestroyMember();
}

HRESULT WSADataBufferT::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	m_pData = (BYTE*)VirtualAlloc(
		NULL,//__in_opt  LPVOID lpAddress,
		m_AllocatedSize,//__in      SIZE_T dwSize,
		MEM_COMMIT | MEM_RESERVE,//__in      DWORD flAllocationType,
		PAGE_READWRITE//__in      DWORD flProtect
		);
	if (NULL == m_pData)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("VirtualAlloc"), hr, TEXT(""));
	}

	return hr;
}

HRESULT WSADataBufferT::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (m_pData)
	{
		m_AllocatedSize = 0;
		BOOL bRet = VirtualFree(
			m_pData,//__in  LPVOID lpAddress,
			0,//__in  SIZE_T dwSize,
			MEM_RELEASE//__in  DWORD dwFreeType
			);
		m_pData = NULL;
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("VirtualFree"), hr, TEXT(""));
		}
	}

	return hr;
}

HRESULT WSADataBufferT::RellocateBuffer( ULONG NewSize, BOOL CopyOldData )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//BOOL bRet = VirtualFree(
	//	m_pData,//__in  LPVOID lpAddress,
	//	0,//__in  SIZE_T dwSize,
	//	MEM_RELEASE//__in  DWORD dwFreeType
	//	);
	//m_pData = NULL;
	//if (!bRet)
	//{
	//	LastError = GetLastError();
	//	hr = HRESULT_FROM_WIN32(LastError);
	//	D_SET_LAST_STATUS(hr, LastError);
	//	D_API_ERR(0, TEXT("VirtualFree"), hr, TEXT(""));
	//}
	if (m_AllocatedSize >= NewSize)
	{
		m_AllocatedSize = NewSize;
	}
	else
	{
		WSADataBufferT Temp(0);
		Temp.m_pData = m_pData;
		Temp.m_AllocatedSize = m_AllocatedSize;

		m_AllocatedSize = NewSize;
		m_pData = (BYTE*)VirtualAlloc(
			NULL,//__in_opt  LPVOID lpAddress,
			m_AllocatedSize,//__in      SIZE_T dwSize,
			MEM_COMMIT | MEM_RESERVE,//__in      DWORD flAllocationType,
			PAGE_READWRITE//__in      DWORD flProtect
			);
		if (NULL == m_pData)
		{
			m_AllocatedSize = 0;
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("VirtualAlloc"), hr, TEXT(""));
		}
		else
		{
			if (CopyOldData)
			{
				CopyMemory(m_pData, Temp.m_pData, Temp.m_AllocatedSize );
			}
		}

		Temp.DestroyMember();
	}

	return hr;
}
