// MTReadWrite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
using namespace std;

#include "MTPoolAndQueue.h"
#include "FileDeviceInterface.h"
#include "MTCopy.h"
#include "StopTimer.h"

struct MyElement
{
	CString strName;
	INT id;

	//////////////////////////////////////////////////////////////////////////
	HRESULT CreateMember()
	{
		strName.Format(TEXT("Name%04d"), count);
		id = count;
		++count;
		return S_OK;
	}

	HRESULT DestroyMember()
	{
		strName;
		id;
		--count;
		return S_OK;
	}

	static INT count;
};
INT MyElement::count = 0;

int _tmain_test_mtpool_st(int argc, _TCHAR* argv[])
{
	HANDLE EventError = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);

	HANDLE EventAbort = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);

	HANDLE EventProviderFinish = CreateEvent(
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
		TRUE,//_In_      BOOL bManualReset,
		FALSE,//_In_      BOOL bInitialState,
		NULL//_In_opt_  LPCTSTR lpName
		);


	const int element_count = 3;
	CMTPool<MyElement> MTPool(EventError, EventAbort, element_count);
	MTPool.CreateMember();

	CMTQueue<MyElement> MTQueue(EventError, EventAbort, EventProviderFinish, element_count);
	MTQueue.CreateMember();

	//////////////////////////////////////////////////////////////////////////
	for (int ii = 0; ii < element_count; ++ ii)
	{
		MyElement * p = NULL;
		HRESULT hr = MTPool.MTAllocate(&p);
		MTQueue.MTPushBack(p);
	}

	for (size_t ii = 0; ii < element_count; ++ ii)
	{
		MyElement * p = NULL;
		MTQueue.MTPopFront(&p);
		MTPool.MTFree(p);
	}



	//////////////////////////////////////////////////////////////////////////
	for (int ii = 0; ii < element_count; ++ ii)
	{
		MyElement * p = NULL;
		HRESULT hr = MTPool.MTAllocate(&p);
		MTQueue.MTPushBack(p);
	}

	for (size_t ii = 0; ii < element_count; ++ ii)
	{
		MyElement * p = NULL;
		MTQueue.MTPopFront(&p);
		MTPool.MTFree(p);
	}

	//////////////////////////////////////////////////////////////////////////
	for (int ii = 0; ii < element_count; ++ ii)
	{
		MyElement * p = NULL;
		HRESULT hr = MTPool.MTAllocate(&p);
		MTQueue.MTPushBack(p);
	}

	SetEvent(EventProviderFinish);
	for (size_t ii = 0; ii < element_count + 3; ++ ii)
	{
		MyElement * p = NULL;
		MTQueue.MTPopFront(&p);
		if (p == NULL)
		{
			break;
		}
		MTPool.MTFree(p);
	}


	//////////////////////////////////////////////////////////////////////////
	//for (int ii = 0; ii < element_count + 1; ++ ii)
	//{
	//	MyElement * p = NULL;
	//	HRESULT hr = Pool.MTAllocate(&p); //hang
	//	vec.push_back(p);
	//}

	//////////////////////////////////////////////////////////////////////////
	MTPool.DestroyMember();
	MTQueue.DestroyMember();
	return 0;
}


//////////////////////////////////////////////////////////////////////////
class  CUTMTPoolQueue
{
private:
	CMTPool<MyElement> m_pool;
	CMTQueue<MyElement> m_queue;
	HANDLE m_EventError;
	HANDLE m_EventAbort;
	HANDLE m_EventProviderFinish;

private:
	CUTMTPoolQueue()
	{

	}

	virtual ~CUTMTPoolQueue()
	{
		DestroyMember();
	}

	HRESULT CreateMember()
	{
		m_EventError = CreateEvent(
			NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
			TRUE,//_In_      BOOL bManualReset,
			FALSE,//_In_      BOOL bInitialState,
			NULL//_In_opt_  LPCTSTR lpName
			);

		m_EventAbort = CreateEvent(
			NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
			TRUE,//_In_      BOOL bManualReset,
			FALSE,//_In_      BOOL bInitialState,
			NULL//_In_opt_  LPCTSTR lpName
			);

		m_EventProviderFinish = CreateEvent(
			NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
			TRUE,//_In_      BOOL bManualReset,
			FALSE,//_In_      BOOL bInitialState,
			NULL//_In_opt_  LPCTSTR lpName
			);

		m_pool.Init(m_EventError, m_EventAbort, ELEMENT_COUNT);
		m_pool.CreateMember();

		m_queue.Init(m_EventError, m_EventAbort, m_EventProviderFinish, ELEMENT_COUNT);
		m_queue.CreateMember();

		return S_OK;
	}

	HRESULT DestroyMember()
	{
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

		return S_OK;
	}

	HRESULT Provider()
	{
		HRESULT hr = S_OK;
		for (INT ii = 0; ii < LOOP_COUNT; ++ ii)
		{
			MyElement * p = NULL;
			hr = m_pool.MTAllocate(&p);
			if (FAILED(hr))
			{
				SetEvent(m_EventError);
				break;
			}
			else if (NULL == p)
			{
				break;
			}
			else
			{
				DINFO(0, TEXT("Provider: [%s, %d]"), p->strName.GetString(), p->id);
				m_queue.MTPushBack(p);
			}
		}

		return hr;
	}

	static DWORD WINAPI ProviderThreadProc(
		_In_  LPVOID lpParameter
		)
	{
		CUTMTPoolQueue * pObj = (CUTMTPoolQueue *)lpParameter;
		return pObj->Provider();
	}

	HRESULT Consumer()
	{
		HRESULT hr = S_OK;
		for (INT ii = 0; ii < LOOP_COUNT; ++ ii)
		{
			MyElement * p = NULL;
			hr = m_queue.MTPopFront(&p);
			if (FAILED(hr))
			{
				SetEvent(m_EventError);
				break;
			}
			else if (NULL == p)
			{
				break;
			}
			else
			{
				DINFO(0, TEXT("Consumer: [%s, %d]"), p->strName.GetString(), p->id);
				m_pool.MTFree(p);
			}
		}

		return hr;
	}

	static DWORD WINAPI ConsumerThreadProc(
		_In_  LPVOID lpParameter
		)
	{
		CUTMTPoolQueue * pObj = (CUTMTPoolQueue *)lpParameter;
		return pObj->Consumer();
	}

public:
	static HRESULT RunTest()
	{
		HANDLE hThread[THREAD_COUNT + THREAD_COUNT] = {0};

		CUTMTPoolQueue obj;
		obj.CreateMember();

		ULONG ThreadId = 0;
		for (INT ii = 0; ii < THREAD_COUNT + THREAD_COUNT; ++ ii)
		{
			if (1 & ii)
			{
				hThread[ii] = CreateThread(
					NULL,//_In_   SEC_ATTRS SecurityAttributes,
					0,//_In_   ULONG StackSize,
					ConsumerThreadProc,//_In_   SEC_THREAD_START StartFunction,
					&obj,//_In_   PVOID ThreadParameter,
					0,//_In_   ULONG CreationFlags,
					&ThreadId//_Out_  PULONG ThreadId
					);
			}
			else
			{
				hThread[ii] = CreateThread(
					NULL,//_In_   SEC_ATTRS SecurityAttributes,
					0,//_In_   ULONG StackSize,
					ProviderThreadProc,//_In_   SEC_THREAD_START StartFunction,
					&obj,//_In_   PVOID ThreadParameter,
					0,//_In_   ULONG CreationFlags,
					&ThreadId//_Out_  PULONG ThreadId
					);
			}
		}

		CONST DWORD WAIT_TIMEOUT_MILSEC = 1000 * 60 * 60 * 3;
		WaitForMultipleObjects(_countof(hThread), hThread, TRUE, WAIT_TIMEOUT_MILSEC);

		return 0;
	}

private:
	static CONST INT ELEMENT_COUNT = 63;
	static CONST INT LOOP_COUNT = 753089;
	static CONST INT THREAD_COUNT = 19;
};

int _tmain_test_mt_pool_queue(int argc, _TCHAR* argv[])
{
	return CUTMTPoolQueue::RunTest();
}


//////////////////////////////////////////////////////////////////////////
//UT start
struct CCmdLine
{
	CString m_strSource;
	CString m_strDest;
	BOOL m_bSourceBufferIO;
	BOOL m_bDestBufferIO;
	ULONG m_RWBufferSize;
	ULONG m_RWBuffersCount;

	CCmdLine():
		m_bSourceBufferIO(TRUE),
		m_bDestBufferIO(TRUE),
		m_RWBufferSize(32*1024),
		m_RWBuffersCount(256)
	{

	}

	VOID Parse(INT argc, TCHAR ** argv)
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


};

static HRESULT test_mtcopy_main(int argc, _TCHAR* argv[])
{
	HRESULT hr =S_OK;
	DWORD dwLastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CCmdLine cmdLine;
	cmdLine.Parse(argc, argv);

	//////////////////////////////////////////////////////////////////////////
	DWORD dwFlagsAndAttributesSource = 0;
	if (!cmdLine.m_bSourceBufferIO)
	{
		dwFlagsAndAttributesSource |= FILE_FLAG_NO_BUFFERING;
	}

	CComPtr<IFileDevice> spSource;
	hr = CreateInstanceFileDeviceWin32(
		(LPTSTR)cmdLine.m_strSource.GetString(),//IN LPCTSTR strFileName, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		dwFlagsAndAttributesSource,//IN ULONG ulFlagsAndAttributes, 
		&spSource,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("Create"), hr, TEXT("%s"), cmdLine.m_strSource.GetString());
		LEAVE_BLOCK(0);
	}

	LONGLONG liFileSize = 0;
	hr = spSource->GetFileSize(
		&liFileSize//__out  PLARGE_INTEGER lpFileSize
		);
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("GetFileSizeEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	ULONG ulBlockSize = 1024 * 64;
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

	CComHeapPtr<IFileDevice> spDest;
	hr = CreateInstanceFileDeviceWin32(
		(LPTSTR)cmdLine.m_strDest.GetString(),//IN LPCTSTR strFileName, 
		GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		dwFlagsAndAttributeDest,//IN ULONG ulFlagsAndAttributes, 
		&spDest,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("Create"), hr, TEXT("%s"), cmdLine.m_strDest.GetString());
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CMTCopy MTCopy(
		spSource,//IFileDevice * pSource,
		spDest,//IFileDevice * pDest, 
		0,//LONGLONG SourceStartOffset, 
		0,//LONGLONG Diff,
		&SimpleBitArray,//CSimpleBitArray * pSimpleBitArray,
		ulBlockSize//ULONG BlockSize
		);
	hr = MTCopy.CreateMember();
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("CreateMember"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CStopTimer perf(TRUE);
	perf.Start();

	hr = MTCopy.Copy();
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("MTCopy.Copy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	perf.End();
	LONGLONG llMilSec = perf.Timing();
	if (0 == llMilSec)
	{
		llMilSec = 1;
	}
	LONGLONG llSpeed = liFileSize * 1000 / llMilSec;
	DINFO(0, TEXT("FileSize=%I64d, Elapsed=%I64d ms, Speed=%I64d bytes/s"), 
		liFileSize,
		llMilSec,
		llSpeed);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//UT end
//////////////////////////////////////////////////////////////////////////



int _tmain(int argc, _TCHAR* argv[])
{
	DSETTINGS(99, FALSE, NULL);
	return test_mtcopy_main(argc, argv);
	return _tmain_test_mt_pool_queue(argc, argv);
	return _tmain_test_mtpool_st(argc, argv);
	return 0;
}