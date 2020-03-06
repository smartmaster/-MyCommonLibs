#include "stdafx.h"
#include "MTCopy.h"


TMTBufferItem::TMTBufferItem() :
	m_BufferSize(0),
	m_DataSize(0),
	m_Offset(0),
	m_pData(NULL)
{

}

HRESULT TMTBufferItem::CreateMember()
{
	HRESULT hr = S_OK;
	CVMemPtr<BYTE> apBuffer;
	m_BufferSize = s_DataBufferSize;
	apBuffer.Allocate(m_BufferSize);
	if (NULL == apBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
	}
	else
	{
		m_pData = apBuffer.Detach();
	}

	return hr;
}

HRESULT TMTBufferItem::DestroyMember()
{
	HRESULT hr = S_OK;
	if (m_pData)
	{
		CVMemPtr<BYTE> apBuffer;
		apBuffer.Attach(m_pData);
		m_pData = NULL;
	}
	return hr;
}

HRESULT CMTCopy::ReadDataBuffer( LONGLONG StartOffset, LONGLONG DataLength )
{
	HRESULT hr = S_OK;

	
	while (DataLength > 0)
	{
		//////////////////////////////////////////////////////////////////////////
		TMTBufferItem * pBuffer = NULL;
		hr = m_pool.MTAllocate(&pBuffer);
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("MTAllocate"), hr, TEXT(""));
			break;
		}

		if (NULL == pBuffer)
		{
			//will never go here
			hr = HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED);
			DSETLASTSTATUS(hr, ERROR_OPERATION_ABORTED);
			DAPIERR(0, TEXT("MTAllocate"), hr, TEXT(""));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		ULONG ReadLength = 0;
		if (DataLength > pBuffer->m_BufferSize)
		{
			ReadLength = pBuffer->m_BufferSize;
		}
		else
		{
			ReadLength = (ULONG)DataLength; //no data loss here from LONGLONG to ULONG
		}


		//////////////////////////////////////////////////////////////////////////
		pBuffer->m_Offset = StartOffset;
		hr = ReadAtOffsetHelper(m_pSource, pBuffer->m_Offset, ReadLength, &pBuffer->m_DataSize, pBuffer->m_pData);
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT("StartOffset=%I64d, ReadLength=%u, DataSize=%u"), StartOffset, ReadLength, pBuffer->m_DataSize);
			break;
		}

		m_TotalRead += pBuffer->m_DataSize;

		//////////////////////////////////////////////////////////////////////////
		hr = m_queue.MTPushBack(pBuffer);
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("MTPushBack"), hr, TEXT(""));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		if (pBuffer->m_DataSize < ReadLength)
		{
			DataLength = 0;
		}
		else
		{
			StartOffset += pBuffer->m_DataSize;
			DataLength -= pBuffer->m_DataSize;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	if (HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED) == hr)
	{
		//will never go here
		SetEvent(m_EventAbort);
		DINFO(0, TEXT("Error operation aborted, notify all threads"));
	}
	else if (FAILED(hr))
	{
		SetEvent(m_EventError);
		DINFO(0, TEXT("Error occured, notify all threads"));
	}

	return hr;
}

HRESULT CMTCopy::WriteProc()
{
	HRESULT hr = S_OK;

	while (TRUE)
	{
		//////////////////////////////////////////////////////////////////////////
		TMTBufferItem * pBuffer = NULL;
		hr = m_queue.MTPopFront(&pBuffer);
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("MTPopFront"), hr, TEXT(""));
			break;
		}

		if (NULL == pBuffer)
		{
			DINFO(0, TEXT("Reader thread has finished providing data into queue and Writer thread finishes processing remaining data buffers from queue"));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		ULONG  NumberOfBytesWritten = 0;
		hr = WriteAtOffsetHelper(m_pDest, pBuffer->m_Offset + m_Diff, pBuffer->m_DataSize, &NumberOfBytesWritten, pBuffer->m_pData);
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT("WriteAtOffsetHelper, [Offset=%I64d, DataSize=%u, NumberOfBytesWritten=%u]"), pBuffer->m_Offset + m_Diff, pBuffer->m_DataSize, NumberOfBytesWritten);
			break;
		}

		if (pBuffer->m_DataSize != NumberOfBytesWritten)
		{
			hr = E_FAIL;
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT("Write length error, [Offset=%I64d, DataSize=%u, NumberOfBytesWritten=%u]"), pBuffer->m_Offset + m_Diff, pBuffer->m_DataSize, NumberOfBytesWritten);
			break;
		}

		m_TotalWritten += NumberOfBytesWritten;


		//////////////////////////////////////////////////////////////////////////
		hr = m_pool.MTFree(pBuffer);
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("MTFree"), hr, TEXT(""));
			break;
		}

	}

	//////////////////////////////////////////////////////////////////////////
	if (FAILED(hr))
	{
		SetEvent(m_EventError);
		DINFO(0, TEXT("Error occured, notify all threads"));
	}

	return hr;
}

CMTCopy::CMTCopy( 
	IFileDevice * pSource, 
	IFileDevice * pDest, 
	LONGLONG SourceStartOffset, 
	LONGLONG Diff,
	CSimpleBitArray * pSimpleBitArray,
	ULONG BlockSize) :
	m_pSource(pSource), 
	m_pDest(pDest), 
	m_SourceStartOffset(SourceStartOffset), 
	m_Diff(Diff), 
	m_pSimpleBitArray(pSimpleBitArray), 
	m_BlockSize(BlockSize),
	m_TotalRead(0),
	m_TotalWritten(0),
	m_EventError(NULL), 
	m_EventAbort(NULL), 
	m_EventProviderFinish(NULL)
{

}

CMTCopy::~CMTCopy()
{
	DestroyMember();
}

HRESULT CMTCopy::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_EventError = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_EventError)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DSETLASTSTATUS(hr, LastError);
		DAPIERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	m_EventAbort = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_EventAbort)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DSETLASTSTATUS(hr, LastError);
		DAPIERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	m_EventProviderFinish = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_EventProviderFinish)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DSETLASTSTATUS(hr, LastError);
		DAPIERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_pool.Init(m_EventError, m_EventAbort, s_BufferItemCount);
	hr = m_pool.CreateMember();
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("pool.CreateMember"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_queue.Init(m_EventError, m_EventAbort, m_EventProviderFinish, s_BufferItemCount);
	hr = m_queue.CreateMember();
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("queue.CreateMember"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CMTCopy::DestroyMember()
{
	HRESULT hr = S_OK;
	if (m_EventError)
	{
		CloseHandle(m_EventError); m_EventError = NULL;
	}

	if (m_EventAbort)
	{
		CloseHandle(m_EventAbort); m_EventAbort = NULL;
	}

	if (m_EventProviderFinish)
	{
		CloseHandle(m_EventProviderFinish); m_EventProviderFinish = NULL;
	}

	m_pool.DestroyMember();
	m_queue.DestroyMember();

	return hr;
}

HRESULT CMTCopy::ReadProc()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	LONGLONG StartBit = 0;
	LONGLONG EndBit = 0;

	while (StartBit < m_pSimpleBitArray->GetBitLength())
	{
		//////////////////////////////////////////////////////////////////////////
		CONST INT ONE = 1;
		CONST INT ZERO = 0;
		StartBit = m_pSimpleBitArray->FindFirst(ONE, StartBit);
		EndBit = m_pSimpleBitArray->FindFirst(ZERO, StartBit);

		//////////////////////////////////////////////////////////////////////////
		LONGLONG StartOffset = m_SourceStartOffset + StartBit * m_BlockSize;
		LONGLONG DataLength = (EndBit - StartBit) * m_BlockSize;
		hr = ReadDataBuffer(StartOffset, DataLength);
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("ReadContinuousData"), hr, TEXT("StartOffset=%I64d, DataLength=%I64d"), StartOffset, DataLength);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		StartBit = EndBit;
	}

	if (SUCCEEDED(hr))
	{
		SetEvent(m_EventProviderFinish);
		DINFO(0, TEXT("Reader thread finished reading all data"));
	}

	return hr;
}

DWORD WINAPI CMTCopy::ReadThreadProc( LPVOID lpParameter )
{
	CMTCopy * pMTCopy = (CMTCopy *)lpParameter;
	return pMTCopy->ReadProc();
}

DWORD WINAPI CMTCopy::WriteThreadProc( LPVOID lpParameter )
{
	CMTCopy * pMTCopy = (CMTCopy *)lpParameter;
	return pMTCopy->WriteProc();
}

HRESULT CMTCopy::Copy()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	DWORD dwThreadId = 0;
	HANDLE hThread = NULL;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hThread = CreateThread(
		NULL,//__in_opt   LPSECURITY_ATTRIBUTES lpThreadAttributes,
		0,//__in       SIZE_T dwStackSize,
		(LPTHREAD_START_ROUTINE)(WriteThreadProc),//__in       LPTHREAD_START_ROUTINE lpStartAddress,
		(LPVOID)(this),//__in_opt   LPVOID lpParameter,
		0,//__in       DWORD dwCreationFlags,
		&dwThreadId//__out_opt  LPDWORD lpThreadId
		);
	if (NULL == hThread)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		DSETLASTSTATUS(hr, LastError);
		DAPIERR(0, TEXT("CreateThread"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = ReadProc();
	if (FAILED(hr))
	{
		DAPIERR(0, TEXT("ReadProc"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	if (hThread)
	{
		CONST DWORD TimeoutMillSec = 1000 * 60 * 60; //60 min
		DWORD Wait = ::WaitForSingleObject(
			hThread,//__in  HANDLE hHandle,
			TimeoutMillSec//__in  DWORD dwMilliseconds
			);
		if (WAIT_OBJECT_0 != Wait)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("WaitForSingleObject"), hr, TEXT("Wait=%u"), Wait);
		}
		CloseHandle(hThread); hThread = NULL;

		DINFO(0, TEXT("TotalRead = %I64d, TotalWritten = %I64d"), m_TotalRead, m_TotalWritten);
	}

	return hr;
}
