#include "stdafx.h"
#include "CircularBuffer.h"


CCircularBuffer::CCircularBuffer(BYTE * Buffer, LONG BufferLen ) :
	m_Buffer(Buffer),
	m_BufferLen(BufferLen),
	//m_cs,
	m_StartOffset(0),
	m_EndBoud(0),
	m_hSpaceAvailable(NULL),
	m_hDataAvaible(NULL),
	m_hStop(NULL),
	m_tidProvider(-1), //<sonmi01>2015-3-12 ###???
	m_tidConsumer(-1)
{

}

CCircularBuffer::~CCircularBuffer()
{
	DestroyMember();
}

HRESULT CCircularBuffer::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	BOOL bRet = InitializeCriticalSectionAndSpinCount(
		&m_Lock,//_Out_  LPCRITICAL_SECTION lpCriticalSection,
		0x400//_In_   DWORD dwSpinCount
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("InitializeCriticalSectionAndSpinCount"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	m_hSpaceAvailable = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		FALSE,//_In_      BOOL bManualReset,
		TRUE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_hSpaceAvailable)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	m_hDataAvaible = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		FALSE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_hDataAvaible)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	m_hStop = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);
	if (NULL == m_hStop)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	bRet = InitializeCriticalSectionAndSpinCount(
		&m_WriterCS,//_Out_  LPCRITICAL_SECTION lpCriticalSection,
		0x400//_In_   DWORD dwSpinCount
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("InitializeCriticalSectionAndSpinCount"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	bRet = InitializeCriticalSectionAndSpinCount(
		&m_ReaderCS,//_Out_  LPCRITICAL_SECTION lpCriticalSection,
		0x400//_In_   DWORD dwSpinCount
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("InitializeCriticalSectionAndSpinCount"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CCircularBuffer::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	DeleteCriticalSection(&m_ReaderCS);

	DeleteCriticalSection(&m_WriterCS);

	DeleteCriticalSection(&m_Lock);

	if (m_hSpaceAvailable)
	{
		CloseHandle(m_hSpaceAvailable);
		m_hSpaceAvailable = NULL;
	}

	if (m_hDataAvaible)
	{
		CloseHandle(m_hDataAvaible);
		m_hDataAvaible = NULL;
	}


	if (m_hStop)
	{
		CloseHandle(m_hStop);
		m_hStop = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

VOID CCircularBuffer::GetDataBufferLocked( LONG & offset, LONG & len ) //S_OK always
{
	EnterCriticalSection(&m_Lock);
	offset = m_StartOffset;
	len = m_EndBoud - m_StartOffset;
	LeaveCriticalSection(&m_Lock);
}

VOID CCircularBuffer::GetSpaceBufferLocked( LONG & offset, LONG & len )
{
	EnterCriticalSection(&m_Lock);
	offset = m_EndBoud;
	len = m_BufferLen - (m_EndBoud - m_StartOffset);
	LeaveCriticalSection(&m_Lock);
}

VOID CCircularBuffer::UpdateStartOffsetLocked(LONG delta) //S_OK always
{
	EnterCriticalSection(&m_Lock);
	m_StartOffset += delta; //"+=" is required, must NOT use "=" as "m_StartOffset  = localStartOffset + delta"; as local localStartOffset is not synchronized
	while (m_StartOffset >= m_BufferLen)
	{
		m_StartOffset -= m_BufferLen;
		m_EndBoud -= m_BufferLen;
	}
	LeaveCriticalSection(&m_Lock);
}

VOID CCircularBuffer::UpdateEndBoundLocked(LONG delta) //S_OK always
{
	EnterCriticalSection(&m_Lock);
	m_EndBoud += delta; //"+=" is required, must NOT use "=" as "m_EndBoud  = localEndBound + delta"; as local localEndBound is not synchronized
	LeaveCriticalSection(&m_Lock);
}

HRESULT CCircularBuffer::Provide( CONST BYTE * pData, LONG Len, LONG & ProcessedLen )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_tidProvider == -1)
	{
		m_tidProvider = GetCurrentThreadId();
	}
	else
	{
		DWORD currentTid = GetCurrentThreadId();
		ATLASSERT(currentTid == m_tidProvider);
		if (currentTid != m_tidProvider)
		{
			ProcessedLen = 0;
			hr = RPC_E_CHANGED_MODE;
			D_API_ERR(0, TEXT("Provide"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LONG SpaceOffset;
	LONG SpaceLen;

	ProcessedLen = 0;

	LONG Currentlen = 0;

	HANDLE hWait[] = {m_hSpaceAvailable, m_hStop}; //do NOT change the order

	while (Len)
	{
		GetSpaceBufferLocked(SpaceOffset, SpaceLen);
		ATLASSERT(SpaceOffset >= 0 && SpaceOffset < m_BufferLen + m_BufferLen);

		if (0 == SpaceLen)
		{
			if (0 == ProcessedLen)
			{
				DWORD dwWait = WaitForMultipleObjects(_countof(hWait), hWait, FALSE, INFINITE);
				if (WAIT_OBJECT_0 == dwWait) 
				{
					//space available, do nothing here
				}
				else if (WAIT_OBJECT_0 + 1 == dwWait)
				{
					D_INFO(0, TEXT("Stop event received"));
					break; //stop event received, end processing
				}
				else
				{
					hr = E_UNEXPECTED;
					D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT(""));
					break; //error occur, should never go here
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			if (Len <= SpaceLen)
			{
				Currentlen = Len;
			}
			else
			{
				Currentlen = SpaceLen;
			}

			CircularCopyIn(SpaceOffset, pData, Currentlen);
			UpdateEndBoundLocked(Currentlen);
			

			ProcessedLen += Currentlen;
			pData += Currentlen;
			Len -= Currentlen;

			SetEvent(m_hDataAvaible);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


HRESULT CCircularBuffer::Consume( BYTE * pData, LONG Len, LONG & ProcessedLen )
{

	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_tidConsumer == -1)
	{
		m_tidConsumer = GetCurrentThreadId();
	}
	else
	{
		DWORD currentTid = GetCurrentThreadId();
		ATLASSERT(currentTid == m_tidConsumer);
		if (currentTid != m_tidConsumer)
		{
			ProcessedLen = 0;
			hr = RPC_E_CHANGED_MODE;
			D_API_ERR(0, TEXT("Consume"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LONG DataOffset;
	LONG DataLen;
	LONG Currentlen = 0;

	HANDLE hWait[] = {m_hDataAvaible, m_hStop}; //do NOT change the order

	ProcessedLen = 0;
	while (Len)
	{
		GetDataBufferLocked(DataOffset, DataLen);
		ATLASSERT(DataOffset >= 0 && DataOffset < m_BufferLen);

		if (0 == DataLen)
		{
			if (0 == ProcessedLen)
			{
				DWORD dwWait = WaitForMultipleObjects(_countof(hWait), hWait, FALSE, INFINITE);
				if (WAIT_OBJECT_0 == dwWait) 
				{
					//data available, do nothing here
				}
				else if (WAIT_OBJECT_0 + 1 == dwWait)
				{
					D_INFO(0, TEXT("Stop event received"));
					break; //stop event received, end processing
				}
				else
				{
					hr = E_UNEXPECTED;
					D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT(""));
					break; //error occur, should never go here
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			if (Len <= DataLen)
			{
				Currentlen = Len;
			}
			else
			{
				Currentlen = DataLen;
			}


			CircularCopyOut(DataOffset, pData, Currentlen);
			UpdateStartOffsetLocked(Currentlen);
			
			ProcessedLen += Currentlen;
			pData += Currentlen;
			Len -= Currentlen;

			SetEvent(m_hSpaceAvailable);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;

}



VOID CCircularBuffer::CircularCopyIn( LONG Offset, CONST BYTE * pData, LONG DataLen )
{
	ATLASSERT(DataLen <= m_BufferLen);
	while (Offset >= m_BufferLen)
	{
		Offset -= m_BufferLen;
	}

	if (Offset + DataLen <= m_BufferLen)
	{
		CopyMemory(m_Buffer + Offset, pData, DataLen);
	}
	else
	{
		LONG Len1 = m_BufferLen - Offset;
		CopyMemory(m_Buffer + Offset, pData, Len1);

		pData += Len1;
		LONG Len2 = DataLen - Len1;
		CopyMemory(m_Buffer, pData, Len2);
	}
}

VOID CCircularBuffer::CircularCopyOut( LONG Offset, BYTE * pData, LONG DataLen )
{
	ATLASSERT(DataLen <= m_BufferLen);
	while (Offset >= m_BufferLen)
	{
		Offset -= m_BufferLen;
	}

	if (Offset + DataLen <= m_BufferLen)
	{
		CopyMemory(pData, m_Buffer + Offset, DataLen);
	}
	else
	{
		LONG Len1 = m_BufferLen - Offset;
		CopyMemory(pData, m_Buffer + Offset, Len1);

		pData += Len1;
		LONG Len2 = DataLen - Len1;
		CopyMemory(pData, m_Buffer, Len2);
	}
}

HRESULT CCircularBuffer::ProvideAll( CONST BYTE * pData, LONG Len, LONG & TotalProcessedLen )
{
	HRESULT hr = S_OK;
	LPCBYTE p = pData;
	LONG ProcessedLen = 0;

	TotalProcessedLen = 0;
	while (Len)
	{
		Provide(p, Len, ProcessedLen);
		if (0 == ProcessedLen)
		{
			hr = S_FALSE;
			break;
		}
		TotalProcessedLen += ProcessedLen;
		p += ProcessedLen;
		Len -= ProcessedLen;
	}
	return hr;
}

HRESULT CCircularBuffer::ConsumeAll( BYTE * pData, LONG Len , LONG & TotalProcessedLen )
{
	HRESULT hr = S_OK;
	LPBYTE p = pData;
	LONG ProcessedLen = 0;

	TotalProcessedLen = 0;
	while (Len)
	{
		Consume(p, Len, ProcessedLen);
		if (0 == ProcessedLen)
		{
			hr = S_FALSE;
			break;
		}
		TotalProcessedLen += ProcessedLen;
		p += ProcessedLen;
		Len -= ProcessedLen;
	}
	return hr;
}

HRESULT CCircularBuffer::Stop()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	SetEvent(m_hStop);

	return hr;
}

HRESULT CCircularBuffer::WriterLock()
{
	EnterCriticalSection(&m_WriterCS);
	return S_OK;
}

HRESULT CCircularBuffer::WriterUnlock()
{
	LeaveCriticalSection(&m_WriterCS);
	return S_OK;
}

HRESULT CCircularBuffer::ReaderLock()
{
	EnterCriticalSection(&m_ReaderCS);
	return S_OK;
}

HRESULT CCircularBuffer::ReaderUnlock()
{
	LeaveCriticalSection(&m_ReaderCS);
	return S_OK;
}

HRESULT CCircularBuffer::PeekNoLocked(BYTE * pData, LONG Len, LONG & ProcessedLen) //must be called in the same thread as consumer
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_tidConsumer == -1)
	{
		m_tidConsumer = GetCurrentThreadId();
	}
	else
	{
		DWORD currentTid = GetCurrentThreadId();
		ATLASSERT(currentTid == m_tidConsumer);
		if (currentTid != m_tidConsumer)
		{
			ProcessedLen = 0;
			hr = RPC_E_CHANGED_MODE;
			D_API_ERR(0, TEXT("PeekNoLocked"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LONG curretnOffset = 0;
	LONG currentLen = 0;

	BOOL bGetDataBufferLocked = FALSE;
	if (bGetDataBufferLocked) //m_EndBoud will be incremented by provider. better to lock, but not matter without lock for performance.  TBD???
	{
		GetDataBufferLocked(curretnOffset, currentLen);
	}
	else
	{
		curretnOffset = m_StartOffset;
		currentLen = m_EndBoud - m_StartOffset;
	}


	ProcessedLen = (Len < currentLen) ? Len : currentLen;
	if (ProcessedLen > 0)
	{
		CircularCopyOut(curretnOffset, pData, ProcessedLen);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
CAutoWriterLock::CAutoWriterLock(CCircularBuffer * pCircularBuffer) :
m_pCircularBuffer(pCircularBuffer)
{
	m_pCircularBuffer->WriterLock();
}

CAutoWriterLock::~CAutoWriterLock()
{
	m_pCircularBuffer->WriterUnlock();
}

CAutoReaderLock::CAutoReaderLock(CCircularBuffer * pCircularBuffer) :
m_pCircularBuffer(pCircularBuffer)
{
	m_pCircularBuffer->ReaderLock();
}

CAutoReaderLock::~CAutoReaderLock()
{
	m_pCircularBuffer->ReaderUnlock();
}
