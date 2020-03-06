#include "stdafx.h"

#include "FileDeviceMem.h"
#include <intsafe.h>



CFileDeviceMem::CFileDeviceMem( BYTE * pBuffer, LONGLONG BufferSize ) :
	m_CurrentPointer(0),
	m_DataSize(0),
	m_DataSizeRead(0),
	m_BufferSize(BufferSize),
	m_pBuffer(pBuffer),
	m_RefCount(0)
{
	//if (nullptr == m_pBuffer && 0 != m_BufferSize)
	//{
	//	m_InternalBuffer.Allocate(m_BufferSize);
	//	m_pBuffer = m_InternalBuffer.m_pData;
	//}

	//if (NULL == m_pBuffer && 0 == m_BufferSize)
	//{
	//	m_BufferSize = LONGLONG_MAX / 2; //assume the max buffer size is half of max 
	//}
}



HRESULT CFileDeviceMem::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (nullptr == m_pBuffer && 0 != m_BufferSize)
	{
		m_InternalBuffer.Allocate(m_BufferSize);
		m_pBuffer = m_InternalBuffer.m_pData;

		if (nullptr == m_pBuffer)
		{
			hr = E_OUTOFMEMORY;
			D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (NULL == m_pBuffer && 0 == m_BufferSize)
	{
		m_BufferSize = LONGLONG_MAX / 2; //assume the max buffer size is half of max 
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT CFileDeviceMem::DestroyMember()
{
	if (m_InternalBuffer.m_pData)
	{
		m_InternalBuffer.Free();
		m_pBuffer = nullptr;
	}
	return S_OK;
}

HRESULT CFileDeviceMem::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	HRESULT hr = S_OK;

	LONGLONG TempCurrent = 0;
	if (FILE_BEGIN == ulMoveMethod)
	{
		TempCurrent = llDistanceToMove;
	}
	else if (FILE_END == ulMoveMethod)
	{
		TempCurrent = m_BufferSize + llDistanceToMove;
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

HRESULT CFileDeviceMem::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	if (m_CurrentPointer + nNumberOfBytesToRead > m_BufferSize)
	{
		nNumberOfBytesToRead = m_BufferSize - m_CurrentPointer;
		D_INFO(0, TEXT("Out of range reading, to read %u bytes. --%s--"), nNumberOfBytesToRead, GetCurrentModuleName());
	}
	*lpNumberOfBytesRead = nNumberOfBytesToRead;
	if (m_pBuffer)
	{
		CopyMemory(pBuffer, m_pBuffer + m_CurrentPointer, nNumberOfBytesToRead);
	}
	m_CurrentPointer += nNumberOfBytesToRead;
	if (m_CurrentPointer > m_DataSizeRead)
	{
		m_DataSizeRead = m_CurrentPointer;
	}
	return S_OK;
}

HRESULT CFileDeviceMem::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	if (m_CurrentPointer + nNumberOfBytesToWrite > m_BufferSize)
	{
		nNumberOfBytesToWrite = m_BufferSize - m_CurrentPointer;
		D_INFO(0, TEXT("Out of range writing, to write %u bytes. --%s--"), nNumberOfBytesToWrite, GetCurrentModuleName());
	}
	*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
	if (m_pBuffer)
	{
		CopyMemory(m_pBuffer + m_CurrentPointer, pBuffer, nNumberOfBytesToWrite);
	}
	m_CurrentPointer += nNumberOfBytesToWrite;
	if (m_CurrentPointer > m_DataSize)
	{
		m_DataSize = m_CurrentPointer;
	}
	//m_DataSize += nNumberOfBytesToWrite;
	return S_OK;
}

HRESULT CFileDeviceMem::GetFileSize( LONGLONG * pFileSize )
{
	*pFileSize = m_DataSize;
	return S_OK;
}

HRESULT CFileDeviceMem::GetBufferSize( LONGLONG * pBufferSize )
{
	*pBufferSize = m_BufferSize;
	return S_OK;
}

HANDLE CFileDeviceMem::GetRawHandle()
{
	return INVALID_HANDLE_VALUE;
}

HRESULT CreateInstanceCFileDeviceMem(BYTE * pBuffer, LONGLONG BufferSize, IFileDeviceMem ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(/*CFileDeviceMem,*/ (new CFileDeviceMem(pBuffer, BufferSize)), ppObj, bCreateMember);
}