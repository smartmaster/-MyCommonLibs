#include "stdafx.h"
#include "MTCopy.h"


#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicMiscHelper.h"

MTBufferItemT::MTBufferItemT() :
	m_BufferSize(0),
	m_DataSize(0),
	m_Offset(0),
	m_Flags(0), //<sonmi01>2014-12-10 ###???
	m_Enum(0),
	m_pData(NULL)
{

}

HRESULT MTBufferItemT::CreateMember()
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

HRESULT MTBufferItemT::DestroyMember()
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

VOID MTBufferItemT::Init()
{
	m_DataSize = 0;
	m_Offset = 0;
	m_Flags = 0;
	m_Enum = 0;
}

HRESULT CMTCopy::ReadDataBuffer( LONGLONG StartOffset, LONGLONG DataLength )
{
	HRESULT hr = S_OK;
	BOOL bToSeek = TRUE;
	
	while (DataLength > 0 || nullptr == m_pSimpleBitArray) //<sonmi01>2014-12-9 ###???
	{
		//////////////////////////////////////////////////////////////////////////
		MTBufferItemT * pBuffer = NULL;
		hr = m_pool.MTAllocate(&pBuffer);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, TEXT("MTAllocate"), hr, TEXT(""));
			break;
		}

		if (NULL == pBuffer)
		{
			//will never go here
			hr = HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED);
			D_SET_LAST_STATUS(hr, ERROR_OPERATION_ABORTED);
			D_API_ERR(0, TEXT("MTAllocate"), hr, TEXT(""));
			break;
		}
		pBuffer->Init(); //<sonmi01>2014-12-10 ###???

		//////////////////////////////////////////////////////////////////////////
		ULONG ReadLength = 0;
		if (DataLength > pBuffer->m_BufferSize || nullptr == m_pSimpleBitArray) //<sonmi01>2014-12-9 ###???
		{
			ReadLength = pBuffer->m_BufferSize;
		}
		else
		{
			ReadLength = (ULONG)DataLength; //no data loss here from LONGLONG to ULONG
		}


		//////////////////////////////////////////////////////////////////////////
		pBuffer->m_Offset = StartOffset;
		if (m_SeekAlways) //<sonmi01>2014-12-12 ###???
		{
			pBuffer->m_Flags |= (ULONG)(MTBufferItemT::FLAGS::SEEK);
			hr = ReadAtOffsetHelper(m_pSource, pBuffer->m_Offset, ReadLength, &pBuffer->m_DataSize, pBuffer->m_pData);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT("StartOffset=%I64d, ReadLength=%u, DataSize=%u"), StartOffset, ReadLength, pBuffer->m_DataSize);
				break;
			}
		}
		else if (nullptr == m_pSimpleBitArray) //<sonmi01>2014-12-9 ###???
		{
			if (m_SourceStartOffset && StartOffset == m_SourceStartOffset)
			{
				LONGLONG NewFilePointer = 0;
				pBuffer->m_Flags |= (ULONG)(MTBufferItemT::FLAGS::SEEK); //<sonmi01>2014-12-10 ###???
				hr = m_pSource->SetPointerEx(m_SourceStartOffset, &NewFilePointer, FILE_BEGIN);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("StartOffset=%I64d"), m_SourceStartOffset);
					break;
				}
			}

			hr = m_pSource->Read(ReadLength, &pBuffer->m_DataSize, pBuffer->m_pData);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, TEXT("Read"), hr, TEXT("StartOffset=%I64d, ReadLength=%u, DataSize=%u"), StartOffset, ReadLength, pBuffer->m_DataSize);
				break;
			}
		}
		else
		{
			if (bToSeek) //<sonmi01>2014-12-10 ###???
			{
				bToSeek = FALSE;//seek only once for one continuous buffer
				LONGLONG NewFilePointer = 0;
				pBuffer->m_Flags |= (ULONG)(MTBufferItemT::FLAGS::SEEK); //<sonmi01>2014-12-10 ###???
				hr = m_pSource->SetPointerEx(pBuffer->m_Offset, &NewFilePointer, FILE_BEGIN);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("StartOffset=%I64d"), m_SourceStartOffset);
					break;
				}
			}

			hr = m_pSource->Read(ReadLength, &pBuffer->m_DataSize, pBuffer->m_pData);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, TEXT("ReadAtOffsetHelper"), hr, TEXT("StartOffset=%I64d, ReadLength=%u, DataSize=%u"), StartOffset, ReadLength, pBuffer->m_DataSize);
				break;
			}
		}
		
		m_TotalRead += pBuffer->m_DataSize;

		//////////////////////////////////////////////////////////////////////////
		hr = m_queue.MTPushBack(pBuffer);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, TEXT("MTPushBack"), hr, TEXT(""));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		if (pBuffer->m_DataSize < ReadLength)
		{
			DataLength = 0; 
			D_INFO(0, TEXT("File end hit, stop reading"));
			break; //<sonmi01>2014-12-9 ###???
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
		D_INFO(0, TEXT("Error operation aborted, notify all threads"));
	}
	else if (FAILED(hr))
	{
		SetEvent(m_EventError);
		D_INFO(0, TEXT("Error occured, notify all threads"));
	}

	return hr;
}

HRESULT CMTCopy::WriteProc()
{
	HRESULT hr = S_OK;

	while (TRUE)
	{
		//////////////////////////////////////////////////////////////////////////
		MTBufferItemT * pBuffer = NULL;
		hr = m_queue.MTPopFront(&pBuffer);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, TEXT("MTPopFront"), hr, TEXT(""));
			break;
		}

		if (NULL == pBuffer)
		{
			D_INFO(0, TEXT("Reader thread has finished providing data into queue and Writer thread finishes processing remaining data buffers from queue"));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		ULONG  NumberOfBytesWritten = 0;
		if (m_SeekAlways) //<sonmi01>2014-12-12 ###???
		{
			hr = WriteAtOffsetHelper(m_pDest, pBuffer->m_Offset + m_Diff, pBuffer->m_DataSize, &NumberOfBytesWritten, pBuffer->m_pData);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT("WriteAtOffsetHelper, [Offset=%I64d, DataSize=%u, NumberOfBytesWritten=%u]"), pBuffer->m_Offset + m_Diff, pBuffer->m_DataSize, NumberOfBytesWritten);
				break;
			}
		}
		else if (nullptr == m_pSimpleBitArray) //<sonmi01>2014-12-9 ###???
		{
			if (pBuffer->m_Offset == m_SourceStartOffset && (m_SourceStartOffset + m_Diff != 0LL) )
			{
				LONGLONG NewFilePointer = 0;
				hr = m_pDest->SetPointerEx(m_SourceStartOffset + m_Diff, &NewFilePointer, FILE_BEGIN);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("StartOffset=%I64d"), m_SourceStartOffset + m_Diff);
					break;
				}
			}

			hr = m_pDest->Write(pBuffer->m_DataSize, &NumberOfBytesWritten, pBuffer->m_pData);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, TEXT("Write"), hr, TEXT("WriteAtOffsetHelper, [Offset=%I64d, DataSize=%u, NumberOfBytesWritten=%u]"), pBuffer->m_Offset + m_Diff, pBuffer->m_DataSize, NumberOfBytesWritten);
				break;
			}
		}
		else
		{
			if (((ULONG)(MTBufferItemT::FLAGS::SEEK) & pBuffer->m_Flags))
			{
				LONGLONG NewFilePointer = 0;
				hr = m_pDest->SetPointerEx(pBuffer->m_Offset + m_Diff, &NewFilePointer, FILE_BEGIN);
				if (FAILED(hr))
				{
					D_SET_LAST_STATUS(hr, -1);
					D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("StartOffset=%I64d"), m_SourceStartOffset + m_Diff);
					break;
				}
			}

			//hr = WriteAtOffsetHelper(m_pDest, pBuffer->m_Offset + m_Diff, pBuffer->m_DataSize, &NumberOfBytesWritten, pBuffer->m_pData);
			hr = m_pDest->Write(pBuffer->m_DataSize, &NumberOfBytesWritten, pBuffer->m_pData);
			if (FAILED(hr))
			{
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, TEXT("Write"), hr, TEXT("WriteAtOffsetHelper, [Offset=%I64d, DataSize=%u, NumberOfBytesWritten=%u]"), pBuffer->m_Offset + m_Diff, pBuffer->m_DataSize, NumberOfBytesWritten);
				break;
			}
		}
		
		if (pBuffer->m_DataSize != NumberOfBytesWritten)
		{
			hr = E_FAIL;
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, TEXT("WriteAtOffsetHelper"), hr, TEXT("Write length error, [Offset=%I64d, DataSize=%u, NumberOfBytesWritten=%u]"), pBuffer->m_Offset + m_Diff, pBuffer->m_DataSize, NumberOfBytesWritten);
			break;
		}

		m_TotalWritten += NumberOfBytesWritten;


		//////////////////////////////////////////////////////////////////////////
		hr = m_pool.MTFree(pBuffer);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, TEXT("MTFree"), hr, TEXT(""));
			break;
		}

	}

	//////////////////////////////////////////////////////////////////////////
	if (FAILED(hr))
	{
		SetEvent(m_EventError);
		D_INFO(0, TEXT("Error occured, notify all threads"));
	}

	return hr;
}

CMTCopy::CMTCopy( 
	IFileDevice * pSource, 
	IFileDevice * pDest, 
	LONGLONG SourceStartOffset, 
	LONGLONG Diff,
	CSimpleBitArray * pSimpleBitArray,
	LONGLONG BlockSize,
	BOOL SeekAlways) : //<sonmi01>2014-12-12 ###???
	m_pSource(pSource), 
	m_pDest(pDest), 
	m_SourceStartOffset(SourceStartOffset), 
	m_Diff(Diff), 
	m_pSimpleBitArray(pSimpleBitArray), 
	m_BlockSize(BlockSize),
	m_SeekAlways(SeekAlways), //<sonmi01>2014-12-12 ###???
	m_TotalRead(0),
	m_TotalWritten(0),
	m_EventError(NULL), 
	m_EventAbort(NULL), 
	m_EventProviderFinish(NULL),
	m_RefCount(0),
	m_bStartPerf(FALSE),
	m_StopTimer(TRUE)
{

}

//CMTCopy::~CMTCopy()
//{
//	DestroyMember();
//}

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
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
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
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
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
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_pool.Init(m_EventError, m_EventAbort, s_BufferItemCount);
	hr = m_pool.CreateMember();
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("pool.CreateMember"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_queue.Init(m_EventError, m_EventAbort, m_EventProviderFinish, s_BufferItemCount);
	hr = m_queue.CreateMember();
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("queue.CreateMember"), hr, TEXT(""));
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

	if (nullptr == m_pSimpleBitArray)
	{
		hr = ReadDataBuffer(m_SourceStartOffset, LONGLONG_MAX);
		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, TEXT("ReadContinuousData"), hr, TEXT("StartOffset=%I64d, DataLength=%I64d"), m_SourceStartOffset, LONGLONG_MAX);
			//break;
		}
	}
	else
	{
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
				D_SET_LAST_STATUS(hr, -1);
				D_API_ERR(0, TEXT("ReadContinuousData"), hr, TEXT("StartOffset=%I64d, DataLength=%I64d"), StartOffset, DataLength);
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			StartBit = EndBit;
		}
	}

	
	if (SUCCEEDED(hr))
	{
		SetEvent(m_EventProviderFinish);
		D_INFO(0, TEXT("Reader thread finished reading all data"));
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
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateThread"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	if (!m_bStartPerf)
	{
		m_bStartPerf = TRUE;
		m_StopTimer.Start();
	}
	

	hr = ReadProc();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadProc"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	if (hThread)
	{
		//////////////////////////////////////////////////////////////////////////
		CONST DWORD TimeoutMillSec = 1000 * 60 * 60; //60 min
		DWORD Wait = ::WaitForSingleObject(
			hThread,//__in  HANDLE hHandle,
			TimeoutMillSec//__in  DWORD dwMilliseconds
			);
		if (WAIT_OBJECT_0 != Wait)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, -1);
			D_API_ERR(0, TEXT("WaitForSingleObject"), hr, TEXT("Wait=%u"), Wait);
		}
		CloseHandle(hThread); hThread = NULL;

		//////////////////////////////////////////////////////////////////////////
		m_StopTimer.End();
		LONGLONG ElaspsedMilSec = m_StopTimer.Timing();

		CString strAverageSpeedSec;
		CString strAverageSpeedMin;
		if (ElaspsedMilSec)
		{
			LONGLONG AverageSpeedSec = m_TotalRead * 1000 / ElaspsedMilSec;
			LONGLONG AverageSpeedMin = m_TotalRead * 1000 * 60 / ElaspsedMilSec;

			CMiscHelper::NumberToString( AverageSpeedSec, strAverageSpeedSec );
			CMiscHelper::NumberToString( AverageSpeedMin, strAverageSpeedMin );
		}

		CString strTotalRead, strTotalWritten, strElaspsedMilSec;
		CMiscHelper::NumberToString( m_TotalRead, strTotalRead );
		CMiscHelper::NumberToString( m_TotalWritten, strTotalWritten );
		CMiscHelper::NumberToString( ElaspsedMilSec, strElaspsedMilSec );
		

		D_INFO(0, TEXT("TotalRead=%s Bytes, TotalWritten=%s Bytes, ElapsedTime=%s MillSec, AverageSpeed=%s Bytes/Sec (%s Bytes/Min)"), 
			strTotalRead.GetString(), strTotalWritten.GetString(), strElaspsedMilSec.GetString(), strAverageSpeedSec.GetString(), strAverageSpeedMin.GetString());
	}

	return hr;
}

HRESULT CMTCopy::InitParams( LPVOID pPrarams )
{
	CMTCopyParams * pCMTCopyParams = (CMTCopyParams *)(pPrarams);
	m_pSource = pCMTCopyParams->pSource;
	m_pDest = pCMTCopyParams->pDest; 
	m_SourceStartOffset = pCMTCopyParams->SourceStartOffset;
	m_Diff = pCMTCopyParams->Diff;
	m_pSimpleBitArray = pCMTCopyParams->pSimpleBitArray;
	m_BlockSize = pCMTCopyParams->BlockSize;
	return S_OK;
}


HRESULT CreateInstanceCMTCopy(
	IFileDevice * pSource,
	IFileDevice * pDest, 
	LONGLONG SourceStartOffset, 
	LONGLONG Diff,
	CSimpleBitArray * pSimpleBitArray,
	LONGLONG BlockSize,
	BOOL SeekAlways, //<sonmi01>2014-12-12 ###???
	IFileCopy ** ppObj,
	BOOL bCreateMemeber
	)
{
	return CREATE_INSTCANCE_COBJECT(/*CMTCopy,*/
		new CMTCopy(
		pSource,
		pDest, 
		SourceStartOffset, 
		Diff,
		pSimpleBitArray,
		BlockSize,
		SeekAlways //<sonmi01>2014-12-12 ###???
		),
		ppObj,
		bCreateMemeber
		);
}