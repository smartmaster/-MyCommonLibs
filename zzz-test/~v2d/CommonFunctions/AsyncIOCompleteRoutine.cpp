#include "stdafx.h"
#include "AsyncIOCompleteRoutine.h"

ULONG AsyncIOCompleteRoutineApp::CBufferItem:: BUFF_LEN = 0;


AsyncIOCompleteRoutineApp::CBufferItem::CBufferItem()
{
	ZeroMemory(this, sizeof(CBufferItem));
}

AsyncIOCompleteRoutineApp::CBufferItem::~CBufferItem()
{
	DestroyMember();
	ZeroMemory(this, sizeof(CBufferItem));
}

HRESULT AsyncIOCompleteRoutineApp::CBufferItem::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	m_pBuffer = VirtualAlloc(
		NULL,//__in_opt  LPVOID lpAddress,
		BUFF_LEN,//__in      SIZE_T dwSize,
		MEM_COMMIT | MEM_RESERVE,//__in      DWORD flAllocationType,
		PAGE_READWRITE//__in      DWORD flProtect
		);
	if (NULL == m_pBuffer)
	{
		dwLastError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwLastError);
	}
	return hr;
}

HRESULT AsyncIOCompleteRoutineApp::CBufferItem::DestroyMember()
{
	if (m_pBuffer)
	{
		VirtualFree(
			m_pBuffer,//__in  LPVOID lpAddress,
			0,//__in  SIZE_T dwSize,
			MEM_RELEASE//__in  DWORD dwFreeType
			);
		m_pBuffer = NULL;
	}
	return S_OK;
}

AsyncIOCompleteRoutineApp::CAsyncCopy::CAsyncCopy( IN HANDLE hSource, IN HANDLE hDest, IN LONGLONG totalSourceSize, IN LPVOID pBitArray, IN LONGLONG llBitCount, IN ULONG BlockSize, IN LONGLONG llOffsetDiff ) :
m_hSource(hSource),
	m_hDest(hDest), 
	m_totalSourceSize(totalSourceSize),
	m_bitArray(pBitArray, llBitCount),
	m_BlockSize(BlockSize),
	m_TotalRead(0),
	m_TotalWrite(0),
	m_llOffsetDiff(llOffsetDiff),
	m_ErrorCount(0),
	m_hRWFinished(NULL)
{

}

HRESULT AsyncIOCompleteRoutineApp::CAsyncCopy::CreateMember( ULONG nBufferItemCount, ULONG nBufferSize )
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		if (CBufferItem::BUFF_LEN == 0)
		{
			CBufferItem::BUFF_LEN = nBufferSize;
		}
		hr = m_sharedBuffers.CreateMember(nBufferItemCount);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("m_sharedBuffers.CreateMember"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		vector<CBufferItem*> vecItemPtrs;
		m_sharedBuffers.GetAllBufferItemPtrs(vecItemPtrs);
		for (size_t ii=0; ii<vecItemPtrs.size(); ++ii)
		{
			vecItemPtrs[ii]->m_pThis = (LPVOID)(this);
		}

		//////////////////////////////////////////////////////////////////////////
		m_hRWFinished = CreateEvent(
			NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpEventAttributes,
			TRUE,//__in      BOOL bManualReset,
			FALSE,//__in      BOOL bInitialState,
			NULL//__in_opt  LPCTSTR lpName
			);
		if (NULL == m_hRWFinished)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateEvent"), hr));
			break;
		}

	} while (FALSE);

	return hr;
}

HRESULT AsyncIOCompleteRoutineApp::CAsyncCopy::DestroyMember()
{
	HRESULT hr = S_OK;

	m_sharedBuffers.DestroyMember();

	if (m_hRWFinished)
	{
		CloseHandle(m_hRWFinished);
		m_hRWFinished = NULL;
	}

	return hr;
}

AsyncIOCompleteRoutineApp::CAsyncCopy::~CAsyncCopy()
{
	DestroyMember();
}

HRESULT AsyncIOCompleteRoutineApp::CAsyncCopy::Copy()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		LONGLONG llBegintBit = 0;
		LONGLONG llEndtBit = 0;

		CONST INT ONE = 1;
		CONST INT ZERO = 0;
		while (llBegintBit < m_bitArray.GetBitLength())
		{
			//////////////////////////////////////////////////////////////////////////
			llBegintBit = m_bitArray.FindFirst(ONE, llBegintBit);
			llEndtBit = m_bitArray.FindFirst(ZERO, llBegintBit);

			//////////////////////////////////////////////////////////////////////////
			hr = ReadData(llBegintBit*m_BlockSize, (llEndtBit - llBegintBit)*m_BlockSize);
			if (FAILED(hr))
			{
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadData"), hr, TEXT("offset=%I64d, length=%I64d"), llBegintBit*m_BlockSize, (llEndtBit - llBegintBit)*m_BlockSize));
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			llBegintBit = llEndtBit;
		}
		if (FAILED(hr))
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		CONST DWORD dwMilliseconds = 1800*1000;
		DWORD dwWait = WaitForSingleObject/*Ex*/(m_hRWFinished, dwMilliseconds/*, TRUE*/);
		if (WAIT_FAILED == dwWait)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("WaitForSingleObject"), hr, TEXT("dwWait=%u"), dwWait));
			break;
		}

	} while (FALSE);

	return hr;
}

HRESULT AsyncIOCompleteRoutineApp::CAsyncCopy::ReadData( LONGLONG llStartOffset, LONGLONG llDataLength )
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bRet = FALSE;
	DWORD dwNumberOfBytesRead = 0;

	DWORD dwWaitEx = 0;

	ULONG nToRead = 0;
	LARGE_INTEGER liOffset = {0};
	liOffset.QuadPart = llStartOffset;
	while (llDataLength > 0)
	{
		//////////////////////////////////////////////////////////////////////////
		if (m_ErrorCount)
		{
			m_sharedBuffers.Stop();
			hr = E_FAIL;
			_OLD_DAPIERR((0, DRNPOS, TEXT("WorkerThreadError"), hr, TEXT("Count=%u"), m_ErrorCount));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		nToRead = CBufferItem::BUFF_LEN;
		if (llDataLength < CBufferItem::BUFF_LEN)
		{
			nToRead = llDataLength;
		}

		//////////////////////////////////////////////////////////////////////////
		CBufferItem * pBufferItem = NULL;
		hr = m_sharedBuffers.Allocate(&pBufferItem);
		if (FAILED(hr) || NULL == pBufferItem)
		{
			m_sharedBuffers.Stop();
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("m_sharedBuffers.Allocate"), hr, TEXT("pBufferItem=0x%08x"), pBufferItem));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		ZeroMemory(&pBufferItem->m_Overlapped, sizeof(OVERLAPPED));
		pBufferItem->m_Overlapped.Offset = liOffset.LowPart;
		pBufferItem->m_Overlapped.OffsetHigh = liOffset.HighPart;
		pBufferItem->m_offset = liOffset.QuadPart;
		pBufferItem->m_dataLength = nToRead;

		bRet = ReadFileEx(
			m_hSource,//__in         HANDLE hFile,
			pBufferItem->m_pBuffer,//__out        LPVOID lpBuffer,
			nToRead,//__in         DWORD nNumberOfBytesToRead,
			&pBufferItem->m_Overlapped,//__out_opt    LPDWORD lpNumberOfBytesRead,
			OnReadComplete//__inout_opt  LPOVERLAPPED lpOverlapped
			);
		if (!bRet)
		{
			dwLastError = GetLastError();
			if (ERROR_IO_PENDING != dwLastError)
			{
				hr = HRESULT_FROM_WIN32(dwLastError);
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFile"), hr));
				break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		llDataLength -= nToRead;
		liOffset.QuadPart += nToRead;

		//////////////////////////////////////////////////////////////////////////
		//dwWaitEx = WaitForSingleObjectEx(m_hRWFinished, 15*60*1000, TRUE);
		dwWaitEx = SleepEx(15*60*1000, TRUE);
		if (dwWaitEx != WAIT_IO_COMPLETION)
		{
			m_ErrorCount++;
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("SleepEx"), hr));
			break;
		}
	}

	return hr;
}

VOID AsyncIOCompleteRoutineApp::CAsyncCopy::MyOnReadComplete( __in DWORD dwErrorCode, __in DWORD dwNumberOfBytesTransfered, __in LPOVERLAPPED lpOverlapped )
{
	HRESULT hr= S_OK;
	DWORD dwLastError = 0;

	DWORD dwWaitEx = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		CBufferItem * pItem = (CBufferItem*)(lpOverlapped);
		if (dwErrorCode || 0 == dwNumberOfBytesTransfered)
		{
			++m_ErrorCount;
			dwLastError = dwErrorCode;
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("MyOnReadComplete"), hr, TEXT("dwErrorCode=%u, dwNumberOfBytesTransfered=%u"), dwErrorCode, dwNumberOfBytesTransfered));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		LARGE_INTEGER liOffset = {0};
		liOffset.LowPart = lpOverlapped->Offset;
		liOffset.HighPart = lpOverlapped->OffsetHigh;
		liOffset.QuadPart += m_llOffsetDiff;
		ZeroMemory(lpOverlapped, sizeof(OVERLAPPED));
		lpOverlapped->Offset = liOffset.LowPart;
		lpOverlapped->OffsetHigh = liOffset.HighPart;

		if (dwNumberOfBytesTransfered % 512)
		{
			dwNumberOfBytesTransfered = AtlAlignUp(dwNumberOfBytesTransfered, 512);
		}



		BOOL bRet = WriteFileEx(
			m_hDest,//__in         HANDLE hFile,
			pItem->m_pBuffer,//__in         LPCVOID lpBuffer,
			dwNumberOfBytesTransfered,//__in         DWORD nNumberOfBytesToWrite,
			&pItem->m_Overlapped,//__out_opt    LPDWORD lpNumberOfBytesWritten,
			OnWriteComplete//__inout_opt  LPOVERLAPPED lpOverlapped
			);
		if (!bRet)
		{
			dwLastError = GetLastError();
			if (ERROR_IO_PENDING != dwLastError)
			{
				++m_ErrorCount;
				hr = HRESULT_FROM_WIN32(dwLastError);
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteFile"), hr));
				break;
			}
		}

		m_TotalRead += dwNumberOfBytesTransfered;

		//////////////////////////////////////////////////////////////////////////
		//dwWaitEx = WaitForSingleObjectEx(m_hRWFinished, 15*60*1000, TRUE);
		dwWaitEx = SleepEx(15*60*1000, TRUE);
		if (dwWaitEx != WAIT_IO_COMPLETION)
		{
			m_ErrorCount++;
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("SleepEx"), hr));
			break;
		}

	} while (FALSE);
}

VOID CALLBACK AsyncIOCompleteRoutineApp::CAsyncCopy::OnReadComplete( __in DWORD dwErrorCode, __in DWORD dwNumberOfBytesTransfered, __in LPOVERLAPPED lpOverlapped )
{
	CBufferItem* pItem = (CBufferItem*)(lpOverlapped);
	CAsyncCopy * pThis = (CAsyncCopy *)(pItem->m_pThis);
	pThis->MyOnReadComplete(dwErrorCode, dwNumberOfBytesTransfered, lpOverlapped);
}

VOID AsyncIOCompleteRoutineApp::CAsyncCopy::MyOnWriteComplete( __in DWORD dwErrorCode, __in DWORD dwNumberOfBytesTransfered, __in LPOVERLAPPED lpOverlapped )
{
	HRESULT hr= S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		CBufferItem * pItem = (CBufferItem*)(lpOverlapped);
		if (dwErrorCode || 0 == dwNumberOfBytesTransfered)
		{
			++m_ErrorCount;
			dwLastError = dwErrorCode;
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("MyOnWriteComplete"), hr, TEXT("dwErrorCode=%u, dwNumberOfBytesTransfered=%u"), dwErrorCode, dwNumberOfBytesTransfered));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		m_TotalWrite += dwNumberOfBytesTransfered;
		hr = m_sharedBuffers.Free(pItem);
		if (FAILED(hr))
		{
			++m_ErrorCount;
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("m_sharedBuffers.Free"), hr));
			break;
		}

	} while (FALSE);

	//////////////////////////////////////////////////////////////////////////
	_OLD_DINFO((9999, DRNPOS, TEXT("TotalRead=%I64d, TotalWrite=%I64d, totalSourceSize=%I64d"), m_TotalRead, m_TotalWrite, m_totalSourceSize));
	if (m_TotalRead >= m_totalSourceSize && m_TotalWrite >= m_totalSourceSize)
	{
		SetEvent(m_hRWFinished);
		_OLD_DINFO((0, DRNPOS, TEXT("IO finished, TotalRead=%I64d, TotalWrite=%I64d, totalSourceSize=%I64d"), m_TotalRead, m_TotalWrite, m_totalSourceSize));
	}
}

VOID CALLBACK AsyncIOCompleteRoutineApp::CAsyncCopy::OnWriteComplete( __in DWORD dwErrorCode, __in DWORD dwNumberOfBytesTransfered, __in LPOVERLAPPED lpOverlapped )
{
	CBufferItem* pItem = (CBufferItem*)(lpOverlapped);
	CAsyncCopy * pThis = (CAsyncCopy *)(pItem->m_pThis);
	pThis->MyOnWriteComplete(dwErrorCode, dwNumberOfBytesTransfered, lpOverlapped);
}

AsyncIOCompleteRoutineApp::CTestFileCopyCmdLine::CTestFileCopyCmdLine() :
m_bSourceBufferIO(TRUE),
	m_bDestBufferIO(FALSE),
	m_RWBufferSize(32*1024),
	m_RWBuffersCount(256)
{

}

VOID AsyncIOCompleteRoutineApp::CTestFileCopyCmdLine::Parse( INT argc, TCHAR ** argv )
{
	if (argc > 1)
	{
		m_strSource = argv[1];
	}

	if (argc > 2)
	{
		m_strDest = argv[2];
	}

	if (argc > 3)
	{
		m_bSourceBufferIO = _tcstol(argv[3], NULL, 10);
	}

	if (argc > 4)
	{
		m_bDestBufferIO = _tcstol(argv[4], NULL, 10);
	}

	if (argc > 5)
	{
		m_RWBufferSize = _tcstol(argv[5], NULL, 10);
	}

	if (argc > 6)
	{
		m_RWBuffersCount = _tcstol(argv[6], NULL, 10);
	}
}

HRESULT AsyncIOCompleteRoutineApp::Main( CTestFileCopyCmdLine & cmdLine )
{
	HRESULT hr =S_OK;
	DWORD dwLastError = 0;

	do 
	{
		DWORD dwFlagsAndAttributesSource = FILE_FLAG_OVERLAPPED;
		if (!cmdLine.m_bSourceBufferIO)
		{
			dwFlagsAndAttributesSource |= FILE_FLAG_NO_BUFFERING;
		}

		HANDLE hTmp =  CreateFile(
			cmdLine.m_strSource.GetString(),//__in      LPCTSTR lpFileName,
			GENERIC_READ,//__in      DWORD dwDesiredAccess,
			0,//__in      DWORD dwShareMode,
			NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_EXISTING,//__in      DWORD dwCreationDisposition,
			dwFlagsAndAttributesSource,//__in      DWORD dwFlagsAndAttributes,
			NULL//__in_opt  HANDLE hTemplateFile
			);
		if (INVALID_HANDLE_VALUE == hTmp)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFile"), hr, TEXT("file=%s"), cmdLine.m_strSource.GetString()));
			break;
		}
		CHandle ahSource(hTmp); hTmp = INVALID_HANDLE_VALUE;

		LARGE_INTEGER liFileSize = {0};
		BOOL bRet = GetFileSizeEx(
			ahSource,//__in   HANDLE hFile,
			&liFileSize//__out  PLARGE_INTEGER lpFileSize
			);
		if (!bRet)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("GetFileSizeEx"), hr, TEXT("file=%s"), cmdLine.m_strSource.GetString()));
			break;
		}

		ULONG ulBlockSize = 1024;
		LONGLONG llbitCount = AtlAlignUp(liFileSize.QuadPart, ulBlockSize)/ulBlockSize;

		CComHeapPtr<BYTE> apBitArrayBuffer;
		ULONG nBufferSize =  AtlAlignUp(llbitCount, 8)/8;
		apBitArrayBuffer.Allocate(nBufferSize);
		memset(apBitArrayBuffer.m_pData, 0xff, nBufferSize);

		DWORD dwFlagsAndAttributeDest = FILE_FLAG_OVERLAPPED;
		if (!cmdLine.m_bDestBufferIO)
		{
			dwFlagsAndAttributeDest |= FILE_FLAG_NO_BUFFERING;
		}
		hTmp =  CreateFile(
			cmdLine.m_strDest.GetString(),//__in      LPCTSTR lpFileName,
			GENERIC_WRITE,//__in      DWORD dwDesiredAccess,
			0,//__in      DWORD dwShareMode,
			NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			CREATE_ALWAYS,//__in      DWORD dwCreationDisposition,
			dwFlagsAndAttributeDest,//__in      DWORD dwFlagsAndAttributes,
			NULL//__in_opt  HANDLE hTemplateFile
			);
		if (INVALID_HANDLE_VALUE == hTmp)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFile"), hr, TEXT("file=%s"), cmdLine.m_strDest.GetString()));
			break;
		}
		CHandle ahDest(hTmp); hTmp = INVALID_HANDLE_VALUE;

		CAsyncCopy AsyncCopy( 
			ahSource,//IN HANDLE hSource, 
			ahDest,//IN HANDLE hDest, 
			liFileSize.QuadPart,//IN LONGLONG totalSourceSize, 
			apBitArrayBuffer.m_pData,//IN LPVOID pBitArray, 
			llbitCount,//IN LONGLONG llBitCount, 
			ulBlockSize,//IN ULONG BlockSize,
			0//IN LONGLONG llOffsetDiff
			);

		hr = AsyncCopy.CreateMember(cmdLine.m_RWBuffersCount, cmdLine.m_RWBufferSize);


		CStopTimer perf(TRUE);
		perf.Start();

		hr = AsyncCopy.Copy();
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("IocpAsyncCopy.Copy"), hr));
			break;
		}

		perf.End();
		LONGLONG llMilSec = perf.Timing();

		LARGE_INTEGER liDistanceToMove;
		liDistanceToMove.QuadPart = liFileSize.QuadPart;
		bRet = SetFilePointerEx(
			ahDest,//__in       HANDLE hFile,
			liDistanceToMove,//__in       LARGE_INTEGER liDistanceToMove,
			NULL,//__out_opt  PLARGE_INTEGER lpNewFilePointer,
			FILE_BEGIN//__in       DWORD dwMoveMethod
			);
		bRet = SetEndOfFile(ahDest);

		LONGLONG llSpeed = liFileSize.QuadPart * 1000 / llMilSec;

		_OLD_DINFO((0, DRNPOS, TEXT("FileSize=%I64d, Elapsed=%I64d ms, Speed=%I64d bytes/s"), 
			liFileSize.QuadPart,
			llMilSec,
			llSpeed));

	} while (FALSE);

	return hr;
}
