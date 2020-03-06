#include "stdafx.h"

#include "FileDeviceMemAutoAlloc.h"
#include <intsafe.h>



CFileDeviceMemAutoAlloc::CFileDeviceMemAutoAlloc(LONGLONG InitialSize/* = 64LL * 1024*/, LONGLONG AllocDelta/* = 16LL * 1024*/, LONGLONG MaxAllocSize/* = 2LL * 1024 * 1024*/) :
	m_CurrentPointer(0),
	m_DataSize(0),
	m_DataSizeRead(0),
	m_AllocSize(InitialSize),
	m_AllocDelta(AllocDelta),
	m_MaxAllocSize(MaxAllocSize),
	m_RefCount(0)
{
}



HRESULT CFileDeviceMemAutoAlloc::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////

#define CFileDeviceMemAutoAlloc_CreateMember_IS2N(x) (0 == ((x) & (x -1)))
	if (!CFileDeviceMemAutoAlloc_CreateMember_IS2N(m_AllocSize) ||
		!CFileDeviceMemAutoAlloc_CreateMember_IS2N(m_AllocDelta) ||
		!CFileDeviceMemAutoAlloc_CreateMember_IS2N(m_MaxAllocSize)
		)
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("CreateMember"), hr, TEXT("AllocSize=%I64d, AllocDelta=%I64d, MaxAllocSize=%I64d"), m_AllocSize, m_AllocDelta, m_MaxAllocSize);
		LEAVE_BLOCK(0);
	}

	m_Buffer.Allocate(m_AllocSize);
	if (nullptr == m_Buffer.m_pData)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = E_OUTOFMEMORY;
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("%I64d"), m_AllocSize);
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT CFileDeviceMemAutoAlloc::DestroyMember()
{
	if (m_Buffer.m_pData)
	{
		m_Buffer.Free();
	}
	return S_OK;
}

HRESULT CFileDeviceMemAutoAlloc::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	HRESULT hr = S_OK;

	LONGLONG TempCurrent = 0;
	if (FILE_BEGIN == ulMoveMethod)
	{
		TempCurrent = llDistanceToMove;
	}
	else if (FILE_END == ulMoveMethod)
	{
		TempCurrent = m_AllocSize + llDistanceToMove;
	}
	else if (FILE_CURRENT == ulMoveMethod)
	{
		TempCurrent = m_CurrentPointer + llDistanceToMove;
	}
	else
	{
		hr = E_INVALIDARG;
	}

	if (SUCCEEDED(hr))
	{
		if (IsValidOffset(TempCurrent))
		{
			m_CurrentPointer = TempCurrent;
			if (m_CurrentPointer > m_DataSize)
			{
				m_DataSize = m_CurrentPointer;
			}
			*lpNewFilePointer = m_CurrentPointer;
		}
		else
		{
			hr = E_OUTOFMEMORY;
			D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("DistanceToMove=%I64d, MoveMethod=%u, --%s--"), llDistanceToMove, ulMoveMethod, GetCurrentModuleName());
		}
	}

	return hr;
}

HRESULT CFileDeviceMemAutoAlloc::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	if (m_CurrentPointer + nNumberOfBytesToRead > m_AllocSize)
	{
		nNumberOfBytesToRead = m_AllocSize - m_CurrentPointer;
		D_INFO(0, TEXT("Out of range reading, to read %u bytes. --%s--"), nNumberOfBytesToRead, GetCurrentModuleName());
	}
	*lpNumberOfBytesRead = nNumberOfBytesToRead;
	if (m_Buffer.m_pData)
	{
		CopyMemory(pBuffer, m_Buffer.m_pData + m_CurrentPointer, nNumberOfBytesToRead);
	}
	m_CurrentPointer += nNumberOfBytesToRead;
	if (m_CurrentPointer > m_DataSizeRead)
	{
		m_DataSizeRead = m_CurrentPointer;
	}
	return S_OK;
}

HRESULT CFileDeviceMemAutoAlloc::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_CurrentPointer + nNumberOfBytesToWrite > m_AllocSize)
	{
		hr = IncreaseAllocSize(m_CurrentPointer + nNumberOfBytesToWrite);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("IncreaseAllocSize"), hr, TEXT("%I64d"), m_CurrentPointer + nNumberOfBytesToWrite);
			LEAVE_BLOCK(0);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	if (m_CurrentPointer + nNumberOfBytesToWrite > m_AllocSize)
	{
		nNumberOfBytesToWrite = m_AllocSize - m_CurrentPointer;
		D_INFO(0, TEXT("Out of range writing, to write %u bytes. --%s--"), nNumberOfBytesToWrite, GetCurrentModuleName());
	}
	*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
	if (m_Buffer.m_pData)
	{
		CopyMemory(m_Buffer.m_pData + m_CurrentPointer, pBuffer, nNumberOfBytesToWrite);
	}
	m_CurrentPointer += nNumberOfBytesToWrite;
	if (m_CurrentPointer > m_DataSize)
	{
		m_DataSize = m_CurrentPointer;
	}
	//m_DataSize += nNumberOfBytesToWrite;
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT CFileDeviceMemAutoAlloc::GetFileSize( LONGLONG * pFileSize )
{
	*pFileSize = m_DataSize;
	return S_OK;
}

HRESULT CFileDeviceMemAutoAlloc::GetBufferSize( LONGLONG * pBufferSize )
{
	*pBufferSize = m_AllocSize;
	return S_OK;
}

HANDLE CFileDeviceMemAutoAlloc::GetRawHandle()
{
	return INVALID_HANDLE_VALUE;
}

HRESULT CFileDeviceMemAutoAlloc::IncreaseAllocSize(LONGLONG newSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//ATLASSERT(newSize > m_AllocSize);
	if (newSize <= m_AllocSize)
	{
		D_INFO(0, TEXT("No need to allocate memeory for smaller size"));
		LEAVE_BLOCK(0);
	}


	if (m_AllocSize >= m_MaxAllocSize)
	{
		D_INFO(0, TEXT("Max allocation size has been reached"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	newSize = AtlAlignUp(newSize, m_AllocDelta) + m_AllocDelta;
	if (newSize > m_MaxAllocSize)
	{
		newSize = m_MaxAllocSize;
	}

	CVMemPtr<BYTE> newBuffer;
	newBuffer.Allocate(newSize);
	if (nullptr == newBuffer.m_pData)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = E_OUTOFMEMORY;
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("%I64d"), newSize);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CopyMemory(newBuffer.m_pData, m_Buffer.m_pData, m_AllocSize);
	m_Buffer.Free();
	m_Buffer.Attach(newBuffer.Detach());
	m_AllocSize = newSize;

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CreateInstanceCFileDeviceMemAutoAlloc(LONGLONG InitialSize, LONGLONG AllocDelta, LONGLONG MaxAllocSize, IFileDeviceMem ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(/*CFileDeviceMem,*/ (new CFileDeviceMemAutoAlloc(InitialSize, AllocDelta, MaxAllocSize)), ppObj, bCreateMember);
}