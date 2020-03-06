#include "stdafx.h"
#include "MTCopy.h"
#include "PerformanceTiming.h"

ULONG NS_MTCopy::CBufferItem::BUFFER_SIZE = 1024*4;

NS_MTCopy::CBufferItem::CBufferItem() :
m_Offset(0),
m_DataLen(0),
m_pBuffer(NULL)
{

}

HRESULT NS_MTCopy::CBufferItem::CreateMember()
{
	HRESULT hr = S_OK;
	CVMPtr<BYTE> apBuffer;
	apBuffer.Allocate(BUFFER_SIZE);
	if (NULL == apBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
	}
	else
	{
		m_pBuffer = apBuffer.Detach();
	}
	return hr;
}

HRESULT NS_MTCopy::CBufferItem::DestroyMember()
{
	{
		CVMPtr<BYTE> apBuffer;
		apBuffer.Attach(m_pBuffer);
		m_pBuffer = NULL;
	}
	return S_OK;
}

VOID NS_MTCopy::CBufferItem::GetBlockSize()
{
	BUFFER_SIZE = 32*1024;
}

NS_MTCopy::CMTCopy::CMTCopy( IN IFileDeviceObject * pSource, IN IFileDeviceObject * pDest, IN CSimpleBitArray * pSimpleBitArray, IN ULONG ulBlockSize, IN LONGLONG llOffsetDiff, IN LONGLONG liTotalData, IN LONGLONG llSourceStartOffset ) :
m_pSource(pSource),
	m_pDest(pDest), 
	m_pSimpleBitArray(pSimpleBitArray), 
	m_ulBlockSize(ulBlockSize), 
	m_llDestSrcOffsetDiff(llOffsetDiff),
	m_liTotalData(liTotalData),
	m_llSourceStartOffset(llSourceStartOffset),

	m_TotalRead(0),
	m_TotalWrite(0)
{

}

HRESULT NS_MTCopy::CMTCopy::CreateMember( ULONG nBufferItemCount )
{
	HRESULT hr = S_OK;
	DWORD dwLaetError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		hr = m_SharedBuffers.CreateMember(nBufferItemCount);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateMember"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		hr = m_Queue.CreateMember(nBufferItemCount);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateMember"), hr));
			break;
		}


	} while (FALSE);

	return hr;
}

HRESULT NS_MTCopy::CMTCopy::DestroyMember()
{
	m_SharedBuffers.DestroyMember();
	m_Queue.DestroyMember();
	return S_OK;
}

NS_MTCopy::CMTCopy::~CMTCopy()
{
	DestroyMember();
}

HRESULT NS_MTCopy::CMTCopy::Copy()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		DWORD dwThreadId = 0;
		HANDLE hThread = ::CreateThread(
			NULL,//__in_opt   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//__in       SIZE_T dwStackSize,
			(LPTHREAD_START_ROUTINE)(ProcessDestThreadProc),//__in       LPTHREAD_START_ROUTINE lpStartAddress,
			(LPVOID)(this),//__in_opt   LPVOID lpParameter,
			0,//__in       DWORD dwCreationFlags,
			&dwThreadId//__out_opt  LPDWORD lpThreadId
			);
		if (NULL == hThread)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateThread"), hr));
			break;
		}
		CHandle ahThread(hThread); hThread = NULL;

		//////////////////////////////////////////////////////////////////////////
		hr = ProcessSource();
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("ProcessSource"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		DWORD dwMilliseconds = 1000*60*30; //30 min
		DWORD dwWait = ::WaitForSingleObject(
			ahThread,//__in  HANDLE hHandle,
			dwMilliseconds//__in  DWORD dwMilliseconds
			);
		if (WAIT_OBJECT_0 != dwWait)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("WaitForSingleObject"), hr, TEXT("Wait=%u"), dwWait));
			break;
		}

	} while (FALSE);

	//////////////////////////////////////////////////////////////////////////
	if (FAILED(hr))
	{
		m_SharedBuffers.Stop();
		m_Queue.Stop();
	}

	return hr;
}

DWORD WINAPI NS_MTCopy::CMTCopy::ProcessDestThreadProc( LPVOID lpParameter )
{
	CMTCopy * pMTCopy = (CMTCopy *)(lpParameter);
	return pMTCopy->ProcessDest();
}

HRESULT NS_MTCopy::CMTCopy::ProcessDest()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	for (;;)
	{
		//////////////////////////////////////////////////////////////////////////
		CBufferItem * pBufferItem = NULL;
		hr = m_Queue.Pull(&pBufferItem);
		if (FAILED(hr) || NULL == pBufferItem)
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Pull"), hr, TEXT("pBufferItem=0x%p"), pBufferItem));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		LONGLONG llWriteOffset = m_llDestSrcOffsetDiff + m_llSourceStartOffset + pBufferItem->m_Offset;
		LONGLONG llNewOffset = 0;
		hr = m_pDest->SetPointerEx(llWriteOffset, &llNewOffset, FILE_BEGIN);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr, TEXT("WriteOffset=%I64d, NewOffset=%I64d"), llWriteOffset, llNewOffset));
			break;
		}

		ULONG nWritten = 0;
		hr = m_pDest->Write(pBufferItem->m_DataLen, &nWritten, pBufferItem->m_pBuffer);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Write"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		hr = m_SharedBuffers.Free(pBufferItem);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Free"), hr, TEXT("pBufferItem=0x%p"), pBufferItem));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		m_TotalWrite += nWritten;
		if (m_TotalWrite >= m_liTotalData)
		{
			_OLD_DINFO((0, DRNPOS, TEXT("IO Finished, TotalWrite=%I64d, liTotalData=%I64d"), m_TotalWrite, m_liTotalData));
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (FAILED(hr))
	{
		m_SharedBuffers.Stop();
		m_Queue.Stop();
	}

	return hr;
}

HRESULT NS_MTCopy::CMTCopy::ProcessSource()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	LONGLONG llStartBit = 0;
	LONGLONG llEndBit = 0;

	while (llStartBit < m_pSimpleBitArray->GetBitLength())
	{
		//////////////////////////////////////////////////////////////////////////
		CONST INT ONE = 1;
		CONST INT ZERO = 0;
		llStartBit = m_pSimpleBitArray->FindFirst(ONE, llStartBit);
		llEndBit = m_pSimpleBitArray->FindFirst(ZERO, llStartBit);

		//////////////////////////////////////////////////////////////////////////
		LONGLONG llStartOffset = m_llSourceStartOffset + llStartBit * m_ulBlockSize;
		LONGLONG llDataLen = (llEndBit - llStartBit) * m_ulBlockSize;
		hr = ProcessSourceBuffer(llStartOffset, llDataLen);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("ProcessSourceBuffer"), hr, TEXT("StartOffset=%I64d, DataLen=%I64d"), llStartOffset, llDataLen));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		llStartBit = llEndBit;
	}

	return hr;
}

HRESULT NS_MTCopy::CMTCopy::ProcessSourceBuffer( LONGLONG llStartOffset, LONGLONG llDataLen )
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	ULONG nCurrentToRead = 0;
	while (llDataLen > 0)
	{
		//////////////////////////////////////////////////////////////////////////
		if (llDataLen > CBufferItem::BUFFER_SIZE)
		{
			nCurrentToRead = CBufferItem::BUFFER_SIZE;
		}
		else
		{
			nCurrentToRead = llDataLen;
		}

		//////////////////////////////////////////////////////////////////////////
		CBufferItem * pBufferItem = NULL;
		hr = m_SharedBuffers.Allocate(&pBufferItem);
		if (FAILED(hr) || NULL == pBufferItem)
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Allocate"), hr, TEXT("pBufferItem=0x%p"), pBufferItem));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		LONGLONG llNewFilePointer = 0;
		hr = m_pSource->SetPointerEx(llStartOffset, &llNewFilePointer, FILE_BEGIN);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetPointerEx"), hr, TEXT("StartOffset=%I64d, NewFilePointer=%I64d"), llStartOffset, llNewFilePointer));
			break;
		}

		ULONG nRead = 0;
		hr = m_pSource->Read(nCurrentToRead, &nRead, pBufferItem->m_pBuffer);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Read"), hr));
			break;
		}

		pBufferItem->m_Offset = llStartOffset;
		pBufferItem->m_DataLen = nRead;
		m_TotalRead += nRead;

		//////////////////////////////////////////////////////////////////////////
		hr = m_Queue.Push(pBufferItem);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Push"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		llDataLen -= nRead;
		llStartOffset += nRead;
		if (m_TotalRead >= m_liTotalData)
		{
			_OLD_DINFO((0, DRNPOS, TEXT("IO Finished, TotalRead=%I64d, liTotalData=%I64d"), m_TotalRead, m_liTotalData));
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (FAILED(hr))
	{
		m_SharedBuffers.Stop();
		m_Queue.Stop();
	}

	return hr;
}

NS_MTCopy::TestApp::CCmdLine::CCmdLine() :
m_bSourceBufferIO(TRUE),
	m_bDestBufferIO(FALSE),
	m_RWBufferSize(32*1024),
	m_RWBuffersCount(256),
	m_PreAllocDeltaMB(8)
{

}

VOID NS_MTCopy::TestApp::CCmdLine::Parse( INT argc, TCHAR ** argv )
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

	if (argc > 7)
	{
		m_PreAllocDeltaMB = _tcstol(argv[7], NULL, 10);
		m_PreAllocDeltaMB *= (1024*1024);
	}
}

HRESULT NS_MTCopy::TestApp::Main( CCmdLine & cmdLine )
{
	HRESULT hr =S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		DWORD dwFlagsAndAttributesSource = 0;
		if (!cmdLine.m_bSourceBufferIO)
		{
			dwFlagsAndAttributesSource |= FILE_FLAG_NO_BUFFERING;
		}

		CLocalFileDevice Source;
		hr = Source.Create(
			(LPTSTR)cmdLine.m_strSource.GetString(),//__in      LPCTSTR lpFileName,
			GENERIC_READ,//__in      DWORD dwDesiredAccess,
			0,//__in      DWORD dwShareMode,
			OPEN_EXISTING,//__in      DWORD dwCreationDisposition,
			dwFlagsAndAttributesSource//__in      DWORD dwFlagsAndAttributes,
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Create"), hr, TEXT("%s"), cmdLine.m_strSource.GetString()));
			break;
		}

		LONGLONG liFileSize = {0};
		hr = Source.GetFileSizeEx(
			&liFileSize//__out  PLARGE_INTEGER lpFileSize
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("GetFileSizeEx"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		ULONG ulBlockSize = 4096;
		LONGLONG llbitCount = AtlAlignUp(liFileSize, ulBlockSize)/ulBlockSize;
		CComHeapPtr<BYTE> apBitArrayBuffer;
		ULONG nBufferSize =  AtlAlignUp(llbitCount, 8)/8;
		apBitArrayBuffer.Allocate(nBufferSize);
		memset(apBitArrayBuffer.m_pData, 0xff, nBufferSize);
		CSimpleBitArray SimpleBitArray(apBitArrayBuffer.m_pData, llbitCount); 

		//////////////////////////////////////////////////////////////////////////
		DWORD dwFlagsAndAttributeDest = 0;
		if (!cmdLine.m_bDestBufferIO)
		{
			dwFlagsAndAttributeDest |= FILE_FLAG_NO_BUFFERING;
		}

		CLocalFileDevicePreAlloc Dest(cmdLine.m_PreAllocDeltaMB);
		hr = Dest.Create(
			(LPTSTR)cmdLine.m_strDest.GetString(),//__in      LPCTSTR lpFileName,
			GENERIC_WRITE,//__in      DWORD dwDesiredAccess,
			0,//__in      DWORD dwShareMode,
			CREATE_ALWAYS,//__in      DWORD dwCreationDisposition,
			dwFlagsAndAttributeDest//__in      DWORD dwFlagsAndAttributes,
			);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Create"), hr, TEXT("%s"), cmdLine.m_strDest.GetString()));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		CMTCopy MTCopy(
			&Source,//IN IFileDeviceObject * pSource,
			&Dest,//IN IFileDeviceObject * pDest, 
			&SimpleBitArray,//IN CSimpleBitArray * pSimpleBitArray, 
			ulBlockSize,//IN ULONG ulBlockSize, 
			0,//IN LONGLONG llOffsetDiff,
			liFileSize,//IN LONGLONG liTotalData,
			0//IN LONGLONG llSourceStartOffset
			);

		CBufferItem::BUFFER_SIZE = cmdLine.m_RWBufferSize;
		hr = MTCopy.CreateMember(cmdLine.m_RWBuffersCount);
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateMember"), hr));
			break;
		}

		CStopTimer perf(TRUE);
		perf.Start();

		hr = MTCopy.Copy();
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("MTCopy.Copy"), hr));
			break;
		}

		perf.End();
		LONGLONG llMilSec = perf.Timing();
		if (0 == llMilSec)
		{
			llMilSec = 1;
		}
		LONGLONG llSpeed = liFileSize * 1000 / llMilSec;
		_OLD_DINFO((0, DRNPOS, TEXT("FileSize=%I64d, Elapsed=%I64d ms, Speed=%I64d bytes/s"), 
			liFileSize,
			llMilSec,
			llSpeed));

	} while (FALSE);

	return hr;
}