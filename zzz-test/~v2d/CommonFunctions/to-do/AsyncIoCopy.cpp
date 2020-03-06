#include "stdafx.h"
#include "AsyncIoCopy.h"


CAsyncIoCopy::CAsyncIoCopy( 
	IFileDeviceObject * pSource, /*external object */ 
	IFileDeviceObject * pDest, /*external object */ 
	CSimpleBitArray * pSimpleBitArray, /*external object */ 
	LONGLONG llOffsetDiff, 
	ULONG ulBlockSize, 
	LONGLONG liTotalData,
	LONGLONG llSourceStartOffset
	) :
	m_pSource(pSource), //external object
	m_pDest(pDest), //external object
	m_pSimpleBitArray(pSimpleBitArray), //external object
	m_llOffsetDiff(llOffsetDiff),
	m_ulBlockSize(ulBlockSize),
	m_nErrorCount(0),
	m_hEventFinished(NULL),
	m_liTotalRead(0),
	m_liTotalWrite(0),
	m_liTotalData(liTotalData),
	m_llSourceStartOffset(llSourceStartOffset)
{
}

INT CAsyncIoCopy::CreateMember()
{
	INT nStatus =0;
	DWORD dwLastError = 0;
	HRESULT hr= S_OK;

	do 
	{
		//******************************************
		DWORD dwBufferItemCount = IO_BUFFER_ITEM_COUNT;
		nStatus = m_AsyncIoSharedBuffer.CreateMember( IO_BUFFER_METADATA_LENGTH, IO_BUFFER_MAX_DATA_LENGTH, dwBufferItemCount);
		if (nStatus)
		{
			//nStatus = -1;
			_OLD_DAPIERR((0, DRNPOS, TEXT("AsyncIoSharedBuffer.CreateMember"), E_FAIL, TEXT("[IO_BUFFER_METADATA_LENGTH=%u, IO_BUFFER_MAX_DATA_LENGTH=%u, IO_BUFFER_ITEM_COUNT=%u]"), IO_BUFFER_METADATA_LENGTH, IO_BUFFER_MAX_DATA_LENGTH, IO_BUFFER_ITEM_COUNT));
			break;
		}

		//******************************************
		m_hEventFinished = CreateEvent(
			NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpEventAttributes,
			TRUE,//__in      BOOL bManualReset,
			FALSE,//__in      BOOL bInitialState,
			NULL//__in_opt  LPCTSTR lpName
			);
		if (NULL == m_hEventFinished)
		{
			dwLastError = GetLastError();
			_OLD_DSETLASTSTATUS(dwLastError);
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateEvent"), dwLastError));
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

CAsyncIoCopy::~CAsyncIoCopy()
{
	DestroyMember();
}

INT CAsyncIoCopy::DestroyMember()
{
	m_AsyncIoSharedBuffer.DestroyMember();

	if (m_hEventFinished)
	{
		CloseHandle(m_hEventFinished);
		m_hEventFinished = NULL;
	}

	return 0;
}

INT CAsyncIoCopy::Copy() /*main thread */
{
	DSTART(999);

	INT nStatus = 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	CIocpThreadPool<CIocpWorker>  ThreadPool;

	do 
	{
		//******************************************
		CONST INT WORKER_THREAD_COUNT = 1;
		hr = ThreadPool.Initialize((VOID*)this, WORKER_THREAD_COUNT);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("ThreadPool.Initialize"), hr));
			break;
		}

		//******************************************
		LONGLONG llStartBit = 0;
		while (llStartBit < m_pSimpleBitArray->GetBitLength())
		{
			//******************************************
			CONST INT ONE = 1;
			CONST INT ZERO = 0;
			llStartBit = m_pSimpleBitArray->FindFirst(ONE, llStartBit);
			LONGLONG llEndBit = m_pSimpleBitArray->FindFirst(ZERO, llStartBit);


			//******************************************
			LONGLONG llStartOffset = llStartBit;
			llStartOffset *= m_ulBlockSize;
			llStartOffset += m_llSourceStartOffset;

			LONGLONG llDataLength = llEndBit - llStartBit;
			llDataLength *= m_ulBlockSize;
			

			nStatus = ProcessBuffer(ThreadPool, llStartOffset, llDataLength);
			if (nStatus)
			{
				//nStatus = -1;
				_OLD_DAPIERR((0, DRNPOS, TEXT("ProcessBuffer"), E_FAIL, TEXT("[StartOffset=%I64d, DataLength=%I64d]"), llStartOffset, llDataLength));
				break;
			}

			//******************************************
			llStartBit = llEndBit;
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	//******************************************
	if (0 == nStatus)
	{
		CONST DWORD WAIT_TIME_OUT = 60*60*1000;
		DWORD dwWait = WaitForSingleObject(m_hEventFinished, WAIT_TIME_OUT);
		if (WAIT_OBJECT_0 != dwWait)
		{
			dwLastError = GetLastError();
			_OLD_DSETLASTSTATUS(dwLastError);
			nStatus = dwLastError;
			_OLD_DAPIERR((0, DRNPOS, TEXT("WaitForSingleObject"), dwLastError, TEXT("[dwWait=%u]"), dwWait));
		}
	}

	ThreadPool.Shutdown();

	return nStatus;
}

INT CAsyncIoCopy::ProcessBuffer( CIocpThreadPool<CIocpWorker> & threadPool, LONGLONG llStartOffset, LONGLONG llDataLength )
{
	INT nStatus = 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************
		LONGLONG llNewFilePointer = 0;
		hr = m_pSource->SetPointerEx( 
			llStartOffset,///* [in] */ LONGLONG llDistanceToMove,
			&llNewFilePointer,///* [out] */ LONGLONG *lpNewFilePointer,
			FILE_BEGIN///* [in] */ ULONG ulMoveMethod
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr, TEXT("[StartOffset=%I64d, NewFilePointer=%I64d]"), llStartOffset, llNewFilePointer));
			break;
		}

		//******************************************
		LONGLONG llCurrentOffset = llStartOffset;
		while (llDataLength > 0)
		{
			//******************************************
			ULONG ulCurrentLength = IO_BUFFER_MAX_DATA_LENGTH;
			if (llDataLength < IO_BUFFER_MAX_DATA_LENGTH)
			{
				ulCurrentLength  = llDataLength;
			}

			//******************************************
			CONST DWORD dwWaitTimeOut= 15*60*1000;
			CAsyncIoBufferItem * pAsyncIoBufferItem = m_AsyncIoSharedBuffer.AllocateItem(dwWaitTimeOut);
			if (NULL == pAsyncIoBufferItem)
			{
				nStatus = E_FAIL;
				_OLD_DAPIERR((0, DRNPOS, TEXT("AsyncIoSharedBuffer.AllocateItem"), E_FAIL));
				break;
			}

			//******************************************
			if (m_nErrorCount)
			{
				nStatus = E_FAIL;
				_OLD_DAPIERR((0, DRNPOS, TEXT("WorkerThreadError"), E_FAIL, TEXT("[ErrorCount=%u]"), m_nErrorCount));
				break;
			}

			//******************************************
			ULONG ulNumberOfBytesRead = 0;
			hr = m_pSource->Read( 
				ulCurrentLength,///* [in] */ ULONG nNumberOfBytesToRead,
				&ulNumberOfBytesRead,///* [out] */ ULONG *lpNumberOfBytesRead,
				pAsyncIoBufferItem->m_Buffer + pAsyncIoBufferItem->m_nMetaInformationLength///* [size_is][out] */ BYTE *pBuffer
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Read"), hr, TEXT("[CurrentLength=%u, NumberOfBytesRead=%u, CurrentOffset=%I64d]"), ulCurrentLength, ulNumberOfBytesRead, llCurrentOffset));
				break;
			}

			//******************************************
			CIoBuferMetadata * pIoBuferMetadata = (CIoBuferMetadata*)(pAsyncIoBufferItem->m_Buffer);
			pIoBuferMetadata->m_liLength.QuadPart = ulNumberOfBytesRead;
			pIoBuferMetadata->m_liOffset.QuadPart = llCurrentOffset;
			m_liTotalRead += ulNumberOfBytesRead;

			//******************************************
			BOOL bQueueRequest = threadPool.QueueRequest((CIocpWorker::RequestType)(pAsyncIoBufferItem));
			if (!bQueueRequest)
			{
				dwLastError = GetLastError();
				_OLD_DSETLASTSTATUS(dwLastError);
				nStatus = dwLastError;
				_OLD_DAPIERR((0, DRNPOS, TEXT("QueueRequest"), dwLastError));
				break;
			}

			//******************************************
			llDataLength -= ulCurrentLength;
			llCurrentOffset += ulCurrentLength;
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

BOOL CIocpWorker::Initialize( void * pvParam ) throw()
{
	m_pAsyncIoCopy = (CAsyncIoCopy*)(pvParam);
	return TRUE;
}

INT CIocpWorker::OnError()
{
	INT nStatus = 0;
	InterlockedIncrement(&m_pAsyncIoCopy->m_nErrorCount);
	m_pAsyncIoCopy->m_AsyncIoSharedBuffer.RecycleAllItems();
	SetEvent(m_pAsyncIoCopy->m_hEventFinished);
	return nStatus;
}

void CIocpWorker::Terminate( void* /*pvParam*/, BOOL bAPIError, DWORD dwLastError ) throw()
{
	if (bAPIError)
	{
		_OLD_DAPIERR((0, DRNPOS, TEXT("Terminate"), dwLastError));
		OnError();
	}
}

void CIocpWorker::Execute( _In_ RequestType request, _In_ DWORD dwBytesTransfered, _In_ void *pvWorkerParam, _In_ OVERLAPPED *pOverlapped )
{
	INT nStatus = 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************
		CAsyncIoBufferItem * pAsyncIoBufferItem = (CAsyncIoBufferItem*)(request);
		if (NULL == pAsyncIoBufferItem)
		{
			nStatus = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("AsyncIoBufferItem"), E_FAIL));
			break;
		}

		//******************************************
		CIoBuferMetadata * pIoBuferMetadata = (CIoBuferMetadata *)(pAsyncIoBufferItem->m_Buffer);
		LONGLONG llDistanceToMove = pIoBuferMetadata->m_liOffset.QuadPart + m_pAsyncIoCopy->m_llOffsetDiff;
		LONGLONG liNewFilePointer = 0;
		hr = m_pAsyncIoCopy->m_pDest->SetPointerEx( 
			llDistanceToMove,///* [in] */ LONGLONG llDistanceToMove,
			&liNewFilePointer,///* [out] */ LONGLONG *lpNewFilePointer,
			FILE_BEGIN///* [in] */ ULONG ulMoveMethod
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr, TEXT("[llDistanceToMove=%I64d, liNewFilePointer=%I64d]"), llDistanceToMove, liNewFilePointer));
			break;
		}

		//******************************************
		ULONG nNumberOfBytesToWrite = pIoBuferMetadata->m_liLength.LowPart;
		ULONG ulNumberOfBytesWritten = 0;
		hr  = m_pAsyncIoCopy->m_pDest->Write( 
			nNumberOfBytesToWrite,///* [in] */ ULONG nNumberOfBytesToWrite,
			&ulNumberOfBytesWritten,///* [out] */ ULONG *lpNumberOfBytesWritten,
			pAsyncIoBufferItem->m_Buffer + pAsyncIoBufferItem->m_nMetaInformationLength///* [size_is][in] */ BYTE *pBuffer
			);
		if (FAILED(hr) || ulNumberOfBytesWritten != nNumberOfBytesToWrite)
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("Write"), hr, TEXT("[nNumberOfBytesToWrite=%u, ulNumberOfBytesWritten=%u]"), nNumberOfBytesToWrite, ulNumberOfBytesWritten));
			break;
		}

		//******************************************
		m_pAsyncIoCopy->m_AsyncIoSharedBuffer.RecycleItem(pAsyncIoBufferItem);

		//******************************************
		m_pAsyncIoCopy->m_liTotalWrite += nNumberOfBytesToWrite;
		if (m_pAsyncIoCopy->m_liTotalWrite >= m_pAsyncIoCopy->m_liTotalData)
		{
			_OLD_DINFO((0, DRNPOS, TEXT("Write finished [TotalWrite=%I64d, TotalData=%I64d]"), m_pAsyncIoCopy->m_liTotalWrite, m_pAsyncIoCopy->m_liTotalData));
			SetEvent(m_pAsyncIoCopy->m_hEventFinished);
		}
		
		//******************************************
	} while (FALSE);
	

	if (nStatus)
	{
		OnError();
	}
}


CSyncIoCopy::CSyncIoCopy( IFileDeviceObject * pSource, IFileDeviceObject * pDest, CONST map<LONGLONG, LONGLONG> & dataRange , LONGLONG llOffsetDiff) :
m_pSource(pSource),
m_pDest(pDest),
m_dataRange(dataRange),
m_llOffsetDiff(llOffsetDiff),
m_pBuffer(NULL)
{
	m_pBuffer = new BYTE[IO_BUFFER_MAX_DATA_LENGTH];
	ZeroMemory(m_pBuffer, IO_BUFFER_MAX_DATA_LENGTH);
}

INT CSyncIoCopy::Copy()
{
	INT nStatus = 0;
	for (map<LONGLONG, LONGLONG>::const_iterator citer = m_dataRange.begin();
		citer != m_dataRange.end();
		++citer
		)
	{
		nStatus = ProcessBuffer(citer->first, citer->second - citer->first);
		if (nStatus)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("ProcessBuffer"), nStatus, TEXT("[%I64d, %I64d]"), citer->first, citer->second - citer->first));
			break;
		}
	}

	return nStatus;
}

CSyncIoCopy::~CSyncIoCopy()
{
	if (m_pBuffer)
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
}

INT CSyncIoCopy::ProcessBuffer( LONGLONG llStartOffset, LONGLONG llDataLength )
{
	INT nStatus = 0;
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//******************************************
		LONGLONG llNewFilePointer = 0;
		hr = m_pSource->SetPointerEx( 
			llStartOffset,///* [in] */ LONGLONG llDistanceToMove,
			&llNewFilePointer,///* [out] */ LONGLONG *lpNewFilePointer,
			FILE_BEGIN///* [in] */ ULONG ulMoveMethod
			);
		if (FAILED(hr))
		{	
			_OLD_DSETLASTSTATUS(hr);
			nStatus = hr;
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr, TEXT("[StartOffset=%I64d, NewFilePointer=%I64d]"), llStartOffset, llNewFilePointer));
			break;
		}

		//******************************************
		LONGLONG llCurrentOffset = llStartOffset;
		while (llDataLength > 0)
		{
			//******************************************
			ULONG ulCurrentLength = IO_BUFFER_MAX_DATA_LENGTH;
			if (llDataLength < IO_BUFFER_MAX_DATA_LENGTH)
			{
				ulCurrentLength  = llDataLength;
			}

			//******************************************
			ULONG ulNumberOfBytesRead = 0;
			hr = m_pSource->Read( 
				ulCurrentLength,///* [in] */ ULONG nNumberOfBytesToRead,
				&ulNumberOfBytesRead,///* [out] */ ULONG *lpNumberOfBytesRead,
				m_pBuffer///* [size_is][out] */ BYTE *pBuffer
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Read"), hr, TEXT("[CurrentLength=%u, NumberOfBytesRead=%u, CurrentOffset=%I64d]"), ulCurrentLength, ulNumberOfBytesRead, llCurrentOffset));
				break;
			}

			//******************************************
			LONGLONG llDestOffset = llCurrentOffset + m_llOffsetDiff;
			LONGLONG liNewFilePointer = 0;
			hr = m_pDest->SetPointerEx( 
				llDestOffset,///* [in] */ LONGLONG llDistanceToMove,
				&liNewFilePointer,///* [out] */ LONGLONG *lpNewFilePointer,
				FILE_BEGIN///* [in] */ ULONG ulMoveMethod
				);
			if (FAILED(hr))
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr, TEXT("[llDistanceToMove=%I64d, liNewFilePointer=%I64d]"), llDestOffset, liNewFilePointer));
				break;
			}

			ULONG ulNumberOfBytesWritten = 0;
			hr  = m_pDest->Write( 
				ulNumberOfBytesRead,///* [in] */ ULONG nNumberOfBytesToWrite,
				&ulNumberOfBytesWritten,///* [out] */ ULONG *lpNumberOfBytesWritten,
				m_pBuffer///* [size_is][in] */ BYTE *pBuffer
				);
			if (FAILED(hr) || ulNumberOfBytesWritten != ulNumberOfBytesRead)
			{	
				_OLD_DSETLASTSTATUS(hr);
				nStatus = hr;
				_OLD_DAPIERR((0, DRNPOS, TEXT("Write"), hr, TEXT("[nNumberOfBytesToWrite=%u, ulNumberOfBytesWritten=%u]"), ulNumberOfBytesRead, ulNumberOfBytesWritten));
				break;
			}

			//******************************************
			llDataLength -= ulCurrentLength;
			llCurrentOffset += ulCurrentLength;
		}
		if (nStatus)
		{
			break;
		}

	} while (FALSE);

	return nStatus;
}