// MTReadWrite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
using namespace std;

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicInterfaceLock.h"

#include "MTPoolAndQueue.h"
#include "FileDeviceInterface.h"
#include "MTCopy.h"
#include "StopTimer.h"
#include "..\PublicHeader\PublicNameValueStream.h"
#include "CircularBuffer.h"
#include "..\PublicHeader\PublicMiscHelper.h"
#include "InstantSpeed.h"
#include "TaskMonitorInterface.h"
#include "STCopy.h"
#include "simple_fraction.h"
#include "MTQueueV2.h"
#include "ThrottleControl.h"
#include "sharedData.h"
#include "MachineMRSWLock.h"
#include "ParallelRun.h"
#include "WideNarrowChar.h"
#include "counted_ptr.test.h"

int main_test_all(int argc, TCHAR * argv[]);

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
	for (int ii = 0; ii < element_count; ++ii)
	{
		MyElement * p = NULL;
		HRESULT hr = MTPool.MTAllocate(&p);
		MTQueue.MTPushBack(p);
	}

	for (size_t ii = 0; ii < element_count; ++ii)
	{
		MyElement * p = NULL;
		MTQueue.MTPopFront(&p);
		MTPool.MTFree(p);
	}



	//////////////////////////////////////////////////////////////////////////
	for (int ii = 0; ii < element_count; ++ii)
	{
		MyElement * p = NULL;
		HRESULT hr = MTPool.MTAllocate(&p);
		MTQueue.MTPushBack(p);
	}

	for (size_t ii = 0; ii < element_count; ++ii)
	{
		MyElement * p = NULL;
		MTQueue.MTPopFront(&p);
		MTPool.MTFree(p);
	}

	//////////////////////////////////////////////////////////////////////////
	for (int ii = 0; ii < element_count; ++ii)
	{
		MyElement * p = NULL;
		HRESULT hr = MTPool.MTAllocate(&p);
		MTQueue.MTPushBack(p);
	}

	SetEvent(EventProviderFinish);
	for (size_t ii = 0; ii < element_count + 3; ++ii)
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
		for (INT ii = 0; ii < LOOP_COUNT; ++ii)
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
				D_INFO(0, TEXT("Provider: [%s, %d]"), p->strName.GetString(), p->id);
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
		for (INT ii = 0; ii < LOOP_COUNT; ++ii)
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
				D_INFO(0, TEXT("Consumer: [%s, %d]"), p->strName.GetString(), p->id);
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
		HANDLE hThread[THREAD_COUNT + THREAD_COUNT] = { 0 };

		CUTMTPoolQueue obj;
		obj.CreateMember();

		ULONG ThreadId = 0;
		for (INT ii = 0; ii < THREAD_COUNT + THREAD_COUNT; ++ii)
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

	CCmdLine() :
		m_bSourceBufferIO(TRUE),
		m_bDestBufferIO(TRUE),
		m_RWBufferSize(32 * 1024),
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
	HRESULT hr = S_OK;
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
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Create"), hr, TEXT("%s"), cmdLine.m_strSource.GetString());
		LEAVE_BLOCK(0);
	}

	LONGLONG liFileSize = 0;
	hr = spSource->GetFileSize(
		&liFileSize//__out  PLARGE_INTEGER lpFileSize
		);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("GetFileSizeEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	ULONG ulBlockSize = 1024 * 64;
	LONGLONG llbitCount = AtlAlignUp(liFileSize, ulBlockSize) / ulBlockSize;
	CComHeapPtr<BYTE> apBitArrayBuffer;
	ULONG nBufferSize = AtlAlignUp(llbitCount, 8) / 8;
	apBitArrayBuffer.Allocate(nBufferSize);
	memset(apBitArrayBuffer.m_pData, 0xff, nBufferSize);
	CSimpleBitArray SimpleBitArray(apBitArrayBuffer.m_pData, llbitCount);

	//////////////////////////////////////////////////////////////////////////
	DWORD dwFlagsAndAttributeDest = 0;
	if (!cmdLine.m_bDestBufferIO)
	{
		dwFlagsAndAttributeDest |= FILE_FLAG_NO_BUFFERING;
	}

	CComPtr<IFileDevice> spDest;
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
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Create"), hr, TEXT("%s"), cmdLine.m_strDest.GetString());
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CMTCopy MTCopy(
		spSource,//IFileDevice * pSource,
		spDest,//IFileDevice * pDest, 
		0,//LONGLONG SourceStartOffset, 
		0,//LONGLONG Diff,
		&SimpleBitArray,//CSimpleBitArray * pSimpleBitArray,
		ulBlockSize,//ULONG BlockSize
		FALSE //<sonmi01>2014-12-12 ###???
		);
	hr = MTCopy.CreateMember();
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("CreateMember"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CStopTimer perf(TRUE);
	perf.Start();

	hr = MTCopy.Copy();
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("MTCopy.Copy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	perf.End();
	LONGLONG llMilSec = perf.Timing();
	if (0 == llMilSec)
	{
		llMilSec = 1;
	}
	LONGLONG llSpeed = liFileSize * 1000 / llMilSec;
	D_INFO(0, TEXT("FileSize=%I64d, Elapsed=%I64d ms, Speed=%I64d bytes/s"),
		liFileSize,
		llMilSec,
		llSpeed);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//sourceFile destFile sizeMB
static HRESULT test_mtcopy_main_size(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LPCTSTR sourceFile = argc > 1? argv[1] : TEXT("xxxxxx");
	LPCTSTR destFile = argc > 2? argv[2] : TEXT("yyyy");
	LONGLONG copySize = argc > 3? _tcstoi64(argv[3], NULL, 10) : 30;
	copySize <<= 20;

	//////////////////////////////////////////////////////////////////////////
	DWORD dwFlagsAndAttributesSource = 0;
	//if (!cmdLine.m_bSourceBufferIO)
	//{
	//	dwFlagsAndAttributesSource |= FILE_FLAG_NO_BUFFERING;
	//}

	CComPtr<IFileDevice> spSource;
	hr = CreateInstanceFileDeviceWin32(
		sourceFile,//(LPTSTR)cmdLine.m_strSource.GetString(),//IN LPCTSTR strFileName, 
		GENERIC_READ,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		OPEN_EXISTING,//IN ULONG ulCreationDisposition, 
		dwFlagsAndAttributesSource,//IN ULONG ulFlagsAndAttributes, 
		&spSource,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Create"), hr, TEXT("%s"), sourceFile);
		LEAVE_BLOCK(0);
	}

	LONGLONG liFileSize = 0;
	hr = spSource->GetFileSize(
		&liFileSize//__out  PLARGE_INTEGER lpFileSize
		);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("GetFileSizeEx"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (liFileSize > copySize)
	{
		liFileSize = copySize;
	}

	//////////////////////////////////////////////////////////////////////////
	ULONG ulBlockSize = 1024 * 64;
	LONGLONG llbitCount = AtlAlignUp(liFileSize, ulBlockSize) / ulBlockSize;
	CComHeapPtr<BYTE> apBitArrayBuffer;
	ULONG nBufferSize = AtlAlignUp(llbitCount, 8) / 8;
	apBitArrayBuffer.Allocate(nBufferSize);
	memset(apBitArrayBuffer.m_pData, 0xff, nBufferSize);
	CSimpleBitArray SimpleBitArray(apBitArrayBuffer.m_pData, llbitCount);

	//////////////////////////////////////////////////////////////////////////
	DWORD dwFlagsAndAttributeDest = 0;
	//if (!cmdLine.m_bDestBufferIO)
	//{
	//	dwFlagsAndAttributeDest |= FILE_FLAG_NO_BUFFERING;
	//}

	CComPtr<IFileDevice> spDest;
	hr = CreateInstanceFileDeviceWin32(
		destFile,//(LPTSTR)cmdLine.m_strDest.GetString(),//IN LPCTSTR strFileName, 
		GENERIC_WRITE,//IN ULONG ulDesiredAccess, 
		0,//IN ULONG ulShareMode, 
		CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		dwFlagsAndAttributeDest,//IN ULONG ulFlagsAndAttributes, 
		&spDest,//OUT IFileDevice ** ppFileDevice, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("Create"), hr, TEXT("%s"), destFile);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CMTCopy MTCopy(
		spSource,//IFileDevice * pSource,
		spDest,//IFileDevice * pDest, 
		0,//LONGLONG SourceStartOffset, 
		0,//LONGLONG Diff,
		&SimpleBitArray,//CSimpleBitArray * pSimpleBitArray,
		ulBlockSize,//ULONG BlockSize,
		FALSE //<sonmi01>2014-12-12 ###???
		);
	hr = MTCopy.CreateMember();
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("CreateMember"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CStopTimer perf(TRUE);
	perf.Start();

	hr = MTCopy.Copy();
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("MTCopy.Copy"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	perf.End();
	LONGLONG llMilSec = perf.Timing();
	if (0 == llMilSec)
	{
		llMilSec = 1;
	}
	LONGLONG llSpeed = liFileSize * 1000 / llMilSec;
	D_INFO(0, TEXT("FileSize=%I64d, Elapsed=%I64d ms, Speed=%I64d bytes/s"),
		liFileSize,
		llMilSec,
		llSpeed);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

static int _tmain_test_file_device_mem()
{
	LONGLONG Buffer[100] = { 0 };

	CComPtr<IFileDeviceMem> spIFileDeviceMem;
	CreateInstanceCFileDeviceMem((BYTE*)Buffer, sizeof(Buffer), &spIFileDeviceMem, TRUE);

	for (int ii = 0; ii < _countof(Buffer) + 7; ++ii)
	{
		LONGLONG Data = ii * 100;
		ULONG Written = 0;
		spIFileDeviceMem->Write(sizeof(Data), &Written, (CONST BYTE*)(&Data));
	}

	LONGLONG FileSize = -1;
	spIFileDeviceMem->GetFileSize(&FileSize);

	LONGLONG BufferSize = -1;
	spIFileDeviceMem->GetBufferSize(&BufferSize);

	return 0;
}

int _tmain_test_CNameValueStream()
{
	CONST LONG BUFFER_SIZE = 16 * 1024;
	CHeapPtr<BYTE> apBuffer;
	apBuffer.Allocate(BUFFER_SIZE);

	{
		CComPtr<IFileDeviceMem> spIFileDeviceMem;
		CreateInstanceCFileDeviceMem(apBuffer.m_pData, BUFFER_SIZE, &spIFileDeviceMem, TRUE);

		CNameValueStream nvs(spIFileDeviceMem);

		nvs.SetByteOrder(CNameValueStream::LITTLE_ENDIAN);

		nvs.WriteNameValue(TEXT("short"), (short)(1));
		nvs.WriteNameValue(TEXT("int"), (int)(1));
		nvs.WriteNameValue(TEXT("longlong"), (LONGLONG)(1));

		WCHAR str[] = TEXT("Good Morning");
		nvs.WriteNameValue(TEXT("string"), str, _countof(str));

		SHORT sa[] = { 1, 2, 3, 4 };
		nvs.WriteNameValue(TEXT("sa"), sa, _countof(sa));

		int ia[] = { 1, 2, 3, 4 };
		nvs.WriteNameValue(TEXT("ia"), ia, _countof(ia));

		LONGLONG lla[] = { 1, 2, 3, 4 };
		nvs.WriteNameValue(TEXT("lla"), lla, _countof(lla));

		nvs.WriteEnding();
	}


	{
		CComPtr<IFileDeviceMem> spIFileDeviceMem;
		CreateInstanceCFileDeviceMem(apBuffer.m_pData, BUFFER_SIZE, &spIFileDeviceMem, TRUE);

		CNameValueStream nvs(spIFileDeviceMem);

		CNameValueStream::DATA_BYTE_ORDER byteOrder;
		nvs.GetByteOrder(byteOrder);

		wstring name;
		LONG ValueSize = -1;

		SHORT s = -1;
		nvs.ReadNameValueSize(name, ValueSize);
		ATLASSERT(sizeof(SHORT) == ValueSize);
		nvs.ReadValueData(s);


		int iiii = -1;
		nvs.ReadNameValueSize(name, ValueSize);
		ATLASSERT(sizeof(int) == ValueSize);
		nvs.ReadValueData(iiii);

		LONGLONG ll;
		nvs.ReadNameValueSize(name, ValueSize);
		ATLASSERT(sizeof(LONGLONG) == ValueSize);
		nvs.ReadValueData(ll);

		WCHAR str[128] = { 0 };
		nvs.ReadNameValueSize(name, ValueSize);
		nvs.ReadValueData(str, ValueSize / sizeof(WCHAR));

		SHORT sa[128] = { 0 };
		nvs.ReadNameValueSize(name, ValueSize);
		nvs.ReadValueData(sa, ValueSize / sizeof(SHORT));

		int ia[128] = { 0 };
		nvs.ReadNameValueSize(name, ValueSize);
		nvs.ReadValueData(ia, ValueSize / sizeof(int));

		LONGLONG lla[128] = { 0 };
		nvs.ReadNameValueSize(name, ValueSize);
		nvs.ReadValueData(lla, ValueSize / sizeof(LONGLONG));

		nvs.ReadNameValueSize(name, ValueSize);

	}

	return 0;
}

//UT end
//////////////////////////////////////////////////////////////////////////

void test_CSimpleBitArray(CSimpleBitArray & BitArr)
{
	//////////////////////////////////////////////////////////////////////////
	LONGLONG index1 = 0;
	LONGLONG index2 = 0;
	ULONG ErrCount = 0;
	LONGLONG BitLength = BitArr.GetBitLength();
	LONGLONG tmpIndex1 = 0;
	LONGLONG tmpIndex2 = 0;
	LONGLONG BitOneCount1 = 0;
	LONGLONG BitOneCount2 = 0;
	while (index1 < BitLength && index2 < BitLength)
	{
		tmpIndex1 = index1 = BitArr.FindFirstOld(1, index1);
		tmpIndex2 = index2 = BitArr.FindFirst(1, index2);
		if (index1 != index2)
		{
			++ErrCount;
			D_INFO(0, TEXT("defect found '0'! [%I64d, %I64d]"), index1, index2);
			break;
		}

		index1 = BitArr.FindFirstOld(0, index1);
		index2 = BitArr.FindFirst(0, index2);
		if (index1 != index2)
		{
			++ErrCount;
			D_INFO(0, TEXT("defect found '1'! [%I64d, %I64d]"), index1, index2);
			break;
		}

		BitOneCount1 += (index1 - tmpIndex1);
		BitOneCount2 += (index2 - tmpIndex2);

		BitArr.AssignRange(1, tmpIndex1, index1);
	}


	D_INFO(0, TEXT("test end. ErrorCount=%u, [%I64d, %I64d, %I64d]"), ErrCount, index1, index2, BitLength);

	LONGLONG BitOneCount3 = BitArr.CountBitOne();

	LONGLONG BitOneCount4 = 0;
	for (LONGLONG ii = 0; ii < BitLength; ++ii)
	{
		BitOneCount4 += BitArr.Get(ii);
	}
	D_INFO(0, TEXT("Bit '1' count. [%I64d, %I64d, %I64d, %I64d]"), BitOneCount1, BitOneCount2, BitOneCount3, BitOneCount4);
}

int TestBitmap(LPCTSTR FileAsBitmap)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	HANDLE hFile = CreateFile(
		FileAsBitmap,//_In_      LPCTSTR lpFileName,
		GENERIC_READ,//_In_      DWORD dwDesiredAccess,
		FILE_SHARE_READ,//_In_      DWORD dwShareMode,
		NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		NULL//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CHandle ahFile(hFile); hFile = INVALID_HANDLE_VALUE;
	LARGE_INTEGER FileSize = { 0 };
	BOOL bRet = GetFileSizeEx(
		ahFile,//_In_   HANDLE hFile,
		&FileSize//_Out_  PLARGE_INTEGER lpFileSize
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apBuffer;
	apBuffer.Allocate(FileSize.LowPart);

	DWORD NumberOfBytesRead = 0;
	bRet = ReadFile(
		ahFile,//_In_         HANDLE hFile,
		apBuffer.m_pData,//_Out_        LPVOID lpBuffer,
		FileSize.LowPart,//_In_         DWORD nNumberOfBytesToRead,
		&NumberOfBytesRead,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
		NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
		);
	if (!bRet)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CSimpleBitArray BitArr(apBuffer.m_pData, FileSize.LowPart * 8);

	LONGLONG Count1 = BitArr.CountBitOne();
	test_CSimpleBitArray(BitArr);

	BitArr.Flip();
	LONGLONG Count2 = BitArr.CountBitOne();
	test_CSimpleBitArray(BitArr);

	BitArr.Flip();
	LONGLONG Count3 = BitArr.CountBitOne();
	test_CSimpleBitArray(BitArr);

	BitArr.Flip();
	LONGLONG Count4 = BitArr.CountBitOne();
	test_CSimpleBitArray(BitArr);

	D_INFO(0, TEXT("[%I64d, %I64d, %I64d, %I64d, %I64d, %I64d]"), Count1, Count2, Count3, Count4, Count1 + Count2, BitArr.GetBitLength());

	END_BLOCK(0);

	return 0;
}

static /*CONST*/ BYTE g_providedData[] = "012345678901234567890123456";
//"1234567890abcdefghij\r\n";

DWORD WINAPI ConsumerThreadProc(LPVOID lpParameter)
{
	CCircularBuffer * pCirBuffer = (CCircularBuffer *)(lpParameter);

	BYTE buffer[sizeof(g_providedData)];
	LONG len = 0;
	//DWORD NumberOfBytesWritten = 0;
	//HANDLE hFile = CreateFile(
	//	TEXT("q:\\cir-buffer.txt"),//_In_      LPCTSTR lpFileName,
	//	GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
	//	FILE_SHARE_READ,//_In_      DWORD dwShareMode,
	//	NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	//	CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
	//	0,//_In_      DWORD dwFlagsAndAttributes,
	//	NULL//_In_opt_  HANDLE hTemplateFile
	//	);
	HRESULT hr = S_OK;
	int ii = 0;
	for (;;)
	{
		ZeroMemory(buffer, sizeof(buffer));
		static CONST LONG NUMBER = 3;
		ATLASSERT(0 == (sizeof(g_providedData)-1) % NUMBER);
		LONG TotalProcessed = 0;
		for (LONG ii = 0; ii < (sizeof(g_providedData)-1) / NUMBER; ++ii)
		{
			hr = pCirBuffer->ConsumeAll(buffer + ii * NUMBER, NUMBER, TotalProcessed);
			if (0 == TotalProcessed)
			{
				break;
			}
			//pCirBuffer->Consume(buffer + ii * NUMBER, NUMBER, len);
			//ATLASSERT(buffer[ii] == g_providedData[ii]);
		}
		if (0 == TotalProcessed)
		{
			break;
		}
		else
		{
			ATLASSERT(0 == memcmp(buffer, g_providedData, sizeof(g_providedData)-1));
			printf("%010d:\t%s" "\r\n", ii++, buffer);
		}

		//printf("%s", buffer);

		//WriteFile(
		//	hFile,//_In_         HANDLE hFile,
		//	buffer,//_In_         LPCVOID lpBuffer,
		//	len,//_In_         DWORD nNumberOfBytesToWrite,
		//	&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
		//	NULL//_Inout_opt_  LPOVERLAPPED lpOverlapped
		//	);
	}

	//CloseHandle(hFile);

	return hr;
}


int _tmain_test_circular_buffer(int argc, _TCHAR* argv[])
{
	BYTE CirularBuffer[4] = { 0 };

	//for (LONG ii = 0; ii < sizeof(g_providedData) - 1; ++ ii)
	//{
	//	g_providedData[ii] = ii;
	//}


	CCircularBuffer CirBuffer(CirularBuffer, sizeof(CirularBuffer));
	CirBuffer.CreateMember();
	DWORD ThreadId = 0;
	HANDLE hThread = CreateThread(
		NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
		0,//_In_       SIZE_T dwStackSize,
		ConsumerThreadProc,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
		(LPVOID)(&CirBuffer),//_In_opt_   LPVOID lpParameter,
		0,//_In_       DWORD dwCreationFlags,
		&ThreadId//_Out_opt_  LPDWORD lpThreadId
		);


	LONG loopCount = _tcstol(argv[1], NULL, 0);
	LONG TotalProcessed = 0;
	for (LONG ii = 0; ii < loopCount; ++ii)
	{
		CirBuffer.ProvideAll(g_providedData, sizeof(g_providedData)-1, TotalProcessed);
	}

	CirBuffer.Stop();

	WaitForSingleObject(hThread, INFINITE);
	printf("consumer thread ended" "\r\n");
	CloseHandle(hThread);

	return 0;
}


VOID CopyMemPerf(LONGLONG TotalSize, LONG BlockSize, LONG diff0, LONG diff1)
{
	CONST LONG DELTA = 4 * 1024;

	CVMemPtr<BYTE> buffer0;
	buffer0.Allocate(BlockSize + DELTA);

	CVMemPtr<BYTE> buffer1;
	buffer1.Allocate(BlockSize + DELTA);

	LPBYTE p0 = buffer0.m_pData + diff0;
	LPBYTE p1 = buffer1.m_pData + diff1;

	LONG loopCount = TotalSize / BlockSize;

	CStopTimer timer0(TRUE);
	timer0.Start();
	for (LONG ii = 0; ii < loopCount; ++ii)
	{
		CopyMemory(p0, p1, BlockSize);
	}
	timer0.End();
	LONGLONG elapsedTotal = timer0.Timing();

	CStopTimer timer1(TRUE);
	timer1.Start();
	for (LONG ii = 0; ii < loopCount; ++ii)
	{
		//CopyMemory(p0, p1, BlockSize);
	}
	timer1.End();
	LONGLONG elapsedLoop = timer1.Timing();

	LONGLONG elasped = elapsedTotal - elapsedLoop;

	_ftprintf_s(stdout, TEXT("elapsedTotal=%I64d, elapsedLoop=%I64d, elasped=%I64d, TotalSize=%I64d, BlockSize=%d, diff0=%d, diff1=%d") TEXT("\r\n"), elapsedTotal, elapsedLoop, elasped, TotalSize, BlockSize, diff0, diff1);


	CString strAverageSpeedSec;
	CString strAverageSpeedMin;
	if (elasped)
	{
		LONGLONG AverageSpeedSec = TotalSize * 1000 / elasped;
		LONGLONG AverageSpeedMin = TotalSize * 1000 * 60 / elasped;

		CMiscHelper::NumberToString(AverageSpeedSec, strAverageSpeedSec);
		CMiscHelper::NumberToString(AverageSpeedMin, strAverageSpeedMin);
	}


	_ftprintf_s(stdout, TEXT("Speed=%s Bytes/Sec (%s Bytes/Min)") TEXT("\r\n"), strAverageSpeedSec.GetString(), strAverageSpeedMin.GetString());

}


int _tmain_test_task_monitor()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CONST DWORD TIME_OUT = 10 * 1000;
	CComPtr<ITaskMonitor> spITaskMonitor;
	hr = CreateInstanceCTaskMonitor(
		TIME_OUT,//ULONG TimeOut, 
		&spITaskMonitor,//ITaskMonitor ** ppObj, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCTaskMonitor"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = spITaskMonitor->Start();

	Sleep(1000);
	spITaskMonitor->SetTaskDone();
	hr = spITaskMonitor->Stop();

	hr = spITaskMonitor->Restart();
	Sleep(2000);
	spITaskMonitor->SetTaskDone();

	Sleep(5000);
	spITaskMonitor->SetTaskDone();



	Sleep(20000);
	spITaskMonitor->SetTaskDone();
	

	spITaskMonitor->Stop();
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;

}


int _tmain_test_ref_pointer(int argc, _TCHAR* argv[])
{
	{
		BYTE BUFFER[8] = { 0 };
		CComPtr<IFileDeviceMem> sp1;
		CreateInstanceCFileDeviceMem(BUFFER, sizeof(BUFFER), &sp1, TRUE);

		CComPtr<IFileDeviceMem> sp2(sp1);

		CComPtr<IFileDeviceMem> sp3;
		sp3 = sp1;

		CComPtr<IFileDeviceMem> sp4;
		sp4 = sp1.p;

		CComPtr<IFileDeviceMem> sp5(sp1.p);

		CComPtr<IFileDeviceMem> sp6;
		sp6.Attach(sp1);
		sp6.Detach();

		CComPtr<IFileDeviceMem> sp7;
		sp7.Attach(sp1.p);
		sp7.Detach();

		sp5.Release();
		sp4.Release();
		sp3.Release();
		sp2.Release();
		sp1.Release();
	}

	return 0;
}



int _tmain_backup(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;


	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////

	HANDLE hFile = CreateFile(
		argv[2],//_In_      LPCTSTR lpFileName,
		GENERIC_READ,//_In_      DWORD dwDesiredAccess,
		0,//_In_      DWORD dwShareMode,
		nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		nullptr//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}
	//CHandle ahfile(hFile); hFile = INVALID_HANDLE_VALUE;

	CComPtr<IFileDevice> spSource;
	hr = CreateInstanceCFileDeviceBackupHandle(hFile, TRUE, E_BACKUP_RW::READ, &spSource, TRUE);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceBackupHandle"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hFile = INVALID_HANDLE_VALUE;

	CComPtr<IFileDevice> spDest;
	hr = CreateInstanceCFileDeviceWin32Prealloc(
		10LL * 1024LL * 1024LL * 1024LL,//LONGLONG Delta, 
		argv[3],//LPCTSTR strFileName, 
		GENERIC_ALL,//ULONG ulDesiredAccess, 
		0,//ULONG ulShareMode, 
		CREATE_ALWAYS,//ULONG ulCreationDisposition, 
		0,//ULONG ulFlagsAndAttributes,
		&spDest,//IFileDevice ** ppObj,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceWin32Prealloc"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

#if 0
	HANDLE hFile2 = CreateFile(
		argv[3],//_In_      LPCTSTR lpFileName,
		GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
		0,//_In_      DWORD dwShareMode,
		nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		nullptr//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hFile2)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}
	//CHandle ahfile2(hFile2); hFile2 = INVALID_HANDLE_VALUE;

	CComPtr<IFileDevice> spDest;
	hr = CreateInstanceCFileDeviceHandle(
		hFile2,//IN HANDLE Handle,
		TRUE,//IN BOOL bAttach,
		&spDest,//OUT IFileDevice ** ppFileDevice,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hFile2 = INVALID_HANDLE_VALUE;
#endif
	
	BOOL bUseMTCopy = TRUE;
	if (bUseMTCopy)
	{
		CComPtr<IFileCopy> spIFileCopy;
		hr = CreateInstanceCMTCopy(
			spSource,//IFileDevice * pSource,
			spDest,//IFileDevice * pDest,
			0,//LONGLONG SourceStartOffset,
			0,//LONGLONG Diff,
			nullptr,//CSimpleBitArray * pSimpleBitArray,
			0,//ULONG BlockSize,
			FALSE, //<sonmi01>2014-12-12 ###???
			&spIFileCopy,//IFileCopy ** ppObj,
			TRUE//BOOL bCreateMemeber
			);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("CreateInstanceCMTCopy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = spIFileCopy->Copy();
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Copy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		hr = SimpleSTCopy(spSource, spDest);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("SimpleSTCopy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}


#if 0
	LPVOID pContext = nullptr;

	CHeapPtr<BYTE> apbuffer;
	ULONG _1M = 1024 * 1024;
	apbuffer.Allocate(_1M);
	DWORD NumberOfBytesRead = 0;
	DWORD NumberOfBytesWritten = 0;
	BOOL bRC = FALSE;
	for (;;)
	{
		NumberOfBytesRead = 0;
		bRC = BackupRead(
			ahfile,//_In_   HANDLE hFile,
			apbuffer.m_pData,//_Out_  LPBYTE lpBuffer,
			_1M,//_In_   DWORD nNumberOfBytesToRead,
			&NumberOfBytesRead,//_Out_  LPDWORD lpNumberOfBytesRead,
			FALSE,//_In_   BOOL bAbort,
			TRUE,//_In_   BOOL bProcessSecurity,
			&pContext//_Out_  LPVOID *lpContext
			);
		if (!bRC)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			break;
		}


		bRC = WriteFile(
			ahfile2,//_In_         HANDLE hFile,
			apbuffer.m_pData,//_In_         LPCVOID lpBuffer,
			NumberOfBytesRead,//_In_         DWORD nNumberOfBytesToWrite,
			&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
			nullptr//_Inout_opt_  LPOVERLAPPED lpOverlapped
			);
		if (!bRC)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			break;
		}

		if (0 == NumberOfBytesRead)
		{
			break;
		}
	}

	bRC = BackupRead(
		ahfile,//_In_   HANDLE hFile,
		nullptr,//_Out_  LPBYTE lpBuffer,
		0,//_In_   DWORD nNumberOfBytesToRead,
		0,//_Out_  LPDWORD lpNumberOfBytesRead,
		TRUE,//_In_   BOOL bAbort,
		FALSE,//_In_   BOOL bProcessSecurity,
		&pContext//_Out_  LPVOID *lpContext
		);
	if (!bRC)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//break;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;


}


int _tmain_restore(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;


	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////

	HANDLE hFile = CreateFile(
		argv[2],//_In_      LPCTSTR lpFileName,
		GENERIC_READ,//_In_      DWORD dwDesiredAccess,
		0,//_In_      DWORD dwShareMode,
		nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		nullptr//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}
	//CHandle ahfile(hFile); hFile = INVALID_HANDLE_VALUE;

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDevice> spSource;
	hr = CreateInstanceCFileDeviceHandle(hFile, TRUE, &spSource, TRUE);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceBackupHandle"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hFile = INVALID_HANDLE_VALUE;

	

	HANDLE hFile2 = CreateFile(
		argv[3],//_In_      LPCTSTR lpFileName,
		GENERIC_ALL,//_In_      DWORD dwDesiredAccess,
		0,//_In_      DWORD dwShareMode,
		nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		nullptr//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hFile2)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		LEAVE_BLOCK(0);
	}
	//CHandle ahfile2(hFile2); hFile2 = INVALID_HANDLE_VALUE;


	CComPtr<IFileDevice> spDest;
	hr = CreateInstanceCFileDeviceBackupHandle(hFile2, TRUE, E_BACKUP_RW::WRITE, &spDest, TRUE);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceBackupHandle"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	hFile2 = INVALID_HANDLE_VALUE;

	BOOL bUseMTCopy = TRUE;
	if (bUseMTCopy)
	{
		CComPtr<IFileCopy> spIFileCopy;
		hr = CreateInstanceCMTCopy(
			spSource,//IFileDevice * pSource,
			spDest,//IFileDevice * pDest,
			0,//LONGLONG SourceStartOffset,
			0,//LONGLONG Diff,
			nullptr,//CSimpleBitArray * pSimpleBitArray,
			0,//ULONG BlockSize,
			FALSE, //<sonmi01>2014-12-12 ###???
			&spIFileCopy,//IFileCopy ** ppObj,
			TRUE//BOOL bCreateMemeber
			);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("CreateInstanceCMTCopy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = spIFileCopy->Copy();
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Copy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		hr = SimpleSTCopy(spSource, spDest);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("SimpleSTCopy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}


	

#if 0
	LPVOID pContext = nullptr;

	CHeapPtr<BYTE> apbuffer;
	ULONG _1M = 1024 * 1024;
	apbuffer.Allocate(_1M);
	DWORD NumberOfBytesRead = 0;
	DWORD NumberOfBytesWritten = 0;
	BOOL bRC = FALSE;
	for (;;)
	{
		NumberOfBytesRead = 0;
		bRC = ReadFile(
			ahfile,//_In_   HANDLE hFile,
			apbuffer.m_pData,//_Out_  LPBYTE lpBuffer,
			_1M,//_In_   DWORD nNumberOfBytesToRead,
			&NumberOfBytesRead,//_Out_  LPDWORD lpNumberOfBytesRead,
			nullptr
			);
		if (!bRC)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			break;
		}


		bRC = BackupWrite(
			ahfile2,//_In_         HANDLE hFile,
			apbuffer.m_pData,//_In_         LPCVOID lpBuffer,
			NumberOfBytesRead,//_In_         DWORD nNumberOfBytesToWrite,
			&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
			FALSE,
			TRUE,
			&pContext//_Inout_opt_  LPOVERLAPPED lpOverlapped
			);
		if (!bRC)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			break;
		}

		if (0 == NumberOfBytesRead)
		{
			break;
		}
	}

	bRC = BackupWrite(
		ahfile,//_In_   HANDLE hFile,
		nullptr,//_Out_  LPBYTE lpBuffer,
		0,//_In_   DWORD nNumberOfBytesToRead,
		0,//_Out_  LPDWORD lpNumberOfBytesRead,
		TRUE,//_In_   BOOL bAbort,
		FALSE,//_In_   BOOL bProcessSecurity,
		&pContext//_Out_  LPVOID *lpContext
		);
	if (!bRC)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//break;
	}
#endif

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;


}


int _tmain_backup_restore(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	HANDLE hSource = CreateFile(
		argv[2],//_In_      LPCTSTR lpFileName,
		GENERIC_READ,//_In_      DWORD dwDesiredAccess,
		0,//_In_      DWORD dwShareMode,
		nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		nullptr//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hSource)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	HANDLE hDest = CreateFile(
		argv[3],//_In_      LPCTSTR lpFileName,
		GENERIC_ALL,//_In_      DWORD dwDesiredAccess,
		0,//_In_      DWORD dwShareMode,
		nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
		0,//_In_      DWORD dwFlagsAndAttributes,
		nullptr//_In_opt_  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hDest)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	CComPtr<IFileDevice> spSource;
	hr =  CreateInstanceCFileDeviceBackupHandle(
		hSource,//IN HANDLE Handle,
		TRUE,//IN BOOL bAttach,
		E_BACKUP_RW::READ,//IN E_BACKUP_RW BACKUP_RW,
		&spSource,//OUT IFileDevice ** ppFileDevice,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceBackupHandle"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	CComPtr<IFileDevice> spDest;
	hr = CreateInstanceCFileDeviceBackupHandle(
		hDest,//IN HANDLE Handle,
		TRUE,//IN BOOL bAttach,
		E_BACKUP_RW::WRITE,//IN E_BACKUP_RW BACKUP_RW,
		&spDest,//OUT IFileDevice ** ppFileDevice,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceBackupHandle"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	BOOL bUseMTCopy = TRUE;
	if (bUseMTCopy)
	{
		CComPtr<IFileCopy> spIFileCopy;
		hr = CreateInstanceCMTCopy(
			spSource,//IFileDevice * pSource,
			spDest,//IFileDevice * pDest,
			0,//LONGLONG SourceStartOffset,
			0,//LONGLONG Diff,
			nullptr,//CSimpleBitArray * pSimpleBitArray,
			0,//ULONG BlockSize,
			FALSE, //<sonmi01>2014-12-12 ###???
			&spIFileCopy,//IFileCopy ** ppObj,
			TRUE//BOOL bCreateMemeber
			);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("CreateInstanceCMTCopy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		hr = spIFileCopy->Copy();
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("Copy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		hr = SimpleSTCopy(spSource, spDest);
		if (FAILED(hr))
		{
			//LastError = WSAGetLastError();
			//LastError = GetLastError();
			//hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
			//D_SET_LAST_STATUS(hr, LastError);
			//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
			D_API_ERR(0, TEXT("SimpleSTCopy"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

class test_mtpool_quue_v2
{
private:
	MTQueueV2_NS::CMTPoolQueue<int> m_pq;
	LONG m_acc;

public:
	test_mtpool_quue_v2() :
		m_pq(32),
		m_acc(0)
	{
		m_pq.CreateMember();
	}

private:
	void provider()
	{
		for (;;)
		{
			int * p = nullptr;
			m_pq.GetFree(&p);
			if (nullptr == p)
			{
				break;
			}
			*p = InterlockedIncrement(&m_acc);
			wprintf_s(L"Provide %d" L"\r\n", *p);
			m_pq.PutData(p, FALSE);
			//Sleep(99);
		}
		
	}

	void consumer()
	{
		for (;;)
		{
			int * p = nullptr;
			m_pq.GetData(&p, FALSE);
			if (nullptr == p)
			{
				break;
			}
			wprintf_s(L"Consume %d" L"\r\n", *p);
			m_pq.PutFree(p);
			//Sleep(137);
		}
		
	}

	void SignalProviderFinished()
	{
		m_pq.SignalProviderFinished();
	}

	static DWORD WINAPI providerThreadProc(
		_In_  LPVOID lpParameter
		)
	{
		test_mtpool_quue_v2 * p = (test_mtpool_quue_v2 *)(lpParameter);
		p->provider();
		return 0;
	}

	static DWORD WINAPI consumerThreadProc(
		_In_  LPVOID lpParameter
		)
	{
		test_mtpool_quue_v2 * p = (test_mtpool_quue_v2 *)(lpParameter);
		p->consumer();
		return 0;
	}

public:
	
	static void test()
	{
		test_mtpool_quue_v2 ttt;

		const INT PROVIDER_COUNT = 2;
		const INT CONSUMER_COUNT = 3;
		HANDLE hThread[PROVIDER_COUNT + CONSUMER_COUNT] = { 0 };

		int index = 0;
		for (int ii = 0; ii < PROVIDER_COUNT; ++ii)
		{
			hThread[index++] = CreateThread(
				nullptr,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
				0,//_In_       SIZE_T dwStackSize,
				(LPTHREAD_START_ROUTINE)providerThreadProc,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
				(LPVOID)&ttt,//_In_opt_   LPVOID lpParameter,
				0,//_In_       DWORD dwCreationFlags,
				nullptr//_Out_opt_  LPDWORD lpThreadId
				);
		}

		for (int ii = 0; ii < CONSUMER_COUNT; ++ii)
		{
			hThread[index++] = CreateThread(
				nullptr,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
				0,//_In_       SIZE_T dwStackSize,
				(LPTHREAD_START_ROUTINE)consumerThreadProc,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
				(LPVOID)&ttt,//_In_opt_   LPVOID lpParameter,
				0,//_In_       DWORD dwCreationFlags,
				nullptr//_Out_opt_  LPDWORD lpThreadId
				);
		}

		for (;;)
		{
			Sleep(3000);
		}

		ttt.SignalProviderFinished();

		WaitForMultipleObjects(_countof(hThread), hThread, TRUE, INFINITE);
		for (auto & h : hThread)
		{
			CloseHandle(h);
			h = nullptr;
		}
		
	}

};


namespace test_rw_lock
{
	DWORD WINAPI ThreadProc_Lock(
		_In_  LPVOID lpParameter
		)
	{
		CComPtr<IThreadRWLock> spIRWLock;
		spIRWLock.Attach((IThreadRWLock*)lpParameter);
		spIRWLock->Lock();
		for (int ii = 0; ii < 5; ++ii)
		{
			Sleep(1000);
		}
		spIRWLock->Unlock();

		return 0;
	}

	DWORD WINAPI ThreadProc_SharedLock(
		_In_  LPVOID lpParameter
		)
	{
		CComPtr<IThreadRWLock> spIRWLock;
		spIRWLock.Attach((IThreadRWLock*)lpParameter);
		spIRWLock->SharedLock();
		for (int ii = 0; ii < 5; ++ii)
		{
			Sleep(1000);
		}
		spIRWLock->SharedUnlock();

		return 0;
	}

	int _tmain_ut(int argc, _TCHAR* argv[])
	{
		CComPtr<IThreadRWLock> spIRWLock;
		int testCase = 2;

		switch (testCase)
		{
		case 0:
			CreateInstacnceSRWLock(&spIRWLock, TRUE);
			break;
		case  1:
			CreateInstacnceCriticalSectionLock(&spIRWLock, TRUE);
			break;
		case 2:
			CreateInstacnceCRWLockWin32(&spIRWLock, TRUE);
			break;
		case 3:
			CreateInstacnceCRWLockProcessWin32(&spIRWLock, TEXT("this_is_my_test_"), TRUE);
			break;
		}


		LPCTSTR pObjectName = spIRWLock->WhoAmI();

		{
			spIRWLock->Lock();

			CComPtr<IThreadRWLock> spThreadLock = spIRWLock;
			HANDLE hThread = CreateThread(
				NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
				0,//_In_       SIZE_T dwStackSize,
				ThreadProc_Lock,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
				spThreadLock.Detach(),//_In_opt_   LPVOID lpParameter,
				0,//_In_       DWORD dwCreationFlags,
				NULL//_Out_opt_  LPDWORD lpThreadId
				);

			for (int ii = 0; ii < 5; ++ii)
			{
				Sleep(1000);
			}

			spIRWLock->Unlock();

			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread); hThread = NULL;
		}

	{
		spIRWLock->Lock();

		CComPtr<IThreadRWLock> spThreadLock = spIRWLock;
		HANDLE hThread = CreateThread(
			NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//_In_       SIZE_T dwStackSize,
			ThreadProc_SharedLock,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
			spThreadLock.Detach(),//_In_opt_   LPVOID lpParameter,
			0,//_In_       DWORD dwCreationFlags,
			NULL//_Out_opt_  LPDWORD lpThreadId
			);

		for (int ii = 0; ii < 5; ++ii)
		{
			Sleep(1000);
		}

		spIRWLock->Unlock();

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread); hThread = NULL;
	}


	{
		spIRWLock->SharedLock();

		CComPtr<IThreadRWLock> spThreadLock = spIRWLock;
		HANDLE hThread = CreateThread(
			NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//_In_       SIZE_T dwStackSize,
			ThreadProc_Lock,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
			spThreadLock.Detach(),//_In_opt_   LPVOID lpParameter,
			0,//_In_       DWORD dwCreationFlags,
			NULL//_Out_opt_  LPDWORD lpThreadId
			);

		for (int ii = 0; ii < 5; ++ii)
		{
			Sleep(1000);
		}

		spIRWLock->SharedUnlock();

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread); hThread = NULL;

	}

	{
		spIRWLock->SharedLock();

		CComPtr<IThreadRWLock> spThreadLock = spIRWLock;
		HANDLE hThread = CreateThread(
			NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//_In_       SIZE_T dwStackSize,
			ThreadProc_SharedLock,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
			spThreadLock.Detach(),//_In_opt_   LPVOID lpParameter,
			0,//_In_       DWORD dwCreationFlags,
			NULL//_Out_opt_  LPDWORD lpThreadId
			);

		for (int ii = 0; ii < 5; ++ii)
		{
			Sleep(1000);
		}

		spIRWLock->SharedUnlock();

		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread); hThread = NULL;

	}

	//spIRWLock->Lock();
	//spIRWLock->Lock();
	//spIRWLock->Unlock();
	//spIRWLock->Unlock();

	//spIRWLock->Lock();
	//spIRWLock->SharedLock();
	//spIRWLock->SharedUnlock();
	//spIRWLock->Unlock();

	//spIRWLock->SharedLock();
	//spIRWLock->Lock();
	//spIRWLock->Unlock();
	//spIRWLock->SharedUnlock();

	spIRWLock->SharedLock();
	spIRWLock->SharedLock();
	spIRWLock->SharedUnlock();
	spIRWLock->SharedUnlock();



	return 0;
	}

}//end namespace test_rw_lock

int _tmain(int argc, _TCHAR* argv[])
{
	D_SETTINGS(99999, FALSE, TEXT("mtrw-test"), TEXT(""));
	volatile int testCase = 27;
	switch (testCase)
	{
	case 29:
	{
		SmartLib::counted_ptr_test::Case1();
	}
	break;
	case 28:
	{
		CONST WCHAR strW[] = L"aaŒ“√«bb";

		CStringA strUtf8;
		Utf16To8String(strW, strUtf8);
		CStringW strUtf16;
		Utf8To16String(strUtf8, strUtf16);

		ATLASSERT(0 == memcmp(strW, strUtf16.GetString(), sizeof(strW)));

		std::string str8;
		SmartLib::Utf16To8StdString(strW, str8);
		std::wstring str16;
		SmartLib::Utf8To16StdString(str8, str16);

		ATLASSERT(0 == memcmp(strW, str16.c_str(), sizeof(strW)));

		ATLASSERT(strUtf8.GetLength() == str8.size());
		ATLASSERT(0 == memcmp(strUtf8.GetString(), str8.c_str(), strUtf8.GetLength()));
	}
	break;

	case 27:
	{
		{
			BYTE val = -1;
			LONGLONG count1 = CSimpleBitArray::CountBitOneInByteArray((LPCBYTE)&val, sizeof(val));
			LONGLONG count2 = CSimpleBitArray::CountBitOneInArray((LPCBYTE)&val, sizeof(val));
			ATLASSERT(count1 == count2);
		}

		{
			USHORT val = -1;
			LONGLONG count1 = CSimpleBitArray::CountBitOneInByteArray((LPCBYTE)&val, sizeof(val));
			LONGLONG count2 = CSimpleBitArray::CountBitOneInArray((LPCBYTE)&val, sizeof(val));
			ATLASSERT(count1 == count2);
		}

		{
			ULONG val = -1;
			LONGLONG count1 = CSimpleBitArray::CountBitOneInByteArray((LPCBYTE)&val, sizeof(val));
			LONGLONG count2 = CSimpleBitArray::CountBitOneInArray((LPCBYTE)&val, sizeof(val));
			ATLASSERT(count1 == count2);
		}

		{
			ULONGLONG val = -1;
			LONGLONG count1 = CSimpleBitArray::CountBitOneInByteArray((LPCBYTE)&val, sizeof(val));
			LONGLONG count2 = CSimpleBitArray::CountBitOneInArray((LPCBYTE)&val, sizeof(val));
			ATLASSERT(count1 == count2);
		}

		CONST LONG BUFFER_SIZE = 16 * 1024 * 1024;
		CVMemPtr<BYTE> apBuffer;
		apBuffer.Allocate(BUFFER_SIZE);
		

		CComPtr<IFileDevice> spIFileDevice;
		CreateInstanceFileDeviceWin32(argv[1], GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, 0, &spIFileDevice, TRUE);
		
		ULONG NumberOfBytesRead = 0;
		spIFileDevice->Read(BUFFER_SIZE, &NumberOfBytesRead, apBuffer.m_pData);
		memset(apBuffer.m_pData, 0xff, sizeof(ULONGLONG));
		for (LONG offset = 0; offset <= 8; ++ offset)
		{
			LONGLONG count1 = CSimpleBitArray::CountBitOneInByteArray(apBuffer.m_pData + offset, NumberOfBytesRead - offset);
			LONGLONG count2 = CSimpleBitArray::CountBitOneInArray(apBuffer.m_pData + offset, NumberOfBytesRead - offset);
			ATLASSERT(count1 == count2);
		}

		CSimpleBitArray sb(apBuffer.m_pData, ((LONGLONG)NumberOfBytesRead << 3));
		LONGLONG start = 0;
		LONGLONG startIndex = 0;
		LONGLONG endIndex = 0;
		for (;;)
		{
			sb.FindBitOneRange(start, startIndex, endIndex);
			if (startIndex == endIndex)
			{
				break;
			}
			start = endIndex;
		}

	}
	break;
	case 26:
	{
		SmartLib::ParallelRun::Test();
	}
	break;
	case 25:
	{
		test_rw_lock::_tmain_ut(argc, argv);
	}
	break;
	case 24:
	{
		LPCTSTR Name = TEXT("Global\\MySharedDataForTest_JJJJJJJJJJJJJJ");

		CSharedData<LONGLONG> data1(Name, 4096);
		ATLASSERT(SUCCEEDED(data1.Result()));

		CSharedData<LONGLONG> data2(Name, 0);
		ATLASSERT(SUCCEEDED(data2.Result()));

		LONGLONG & val1 = data1;
		LONGLONG & val2 = data2;

		val1 = 100;
		val2 = 200;

		++val1;
		++val2;

	}
	break;
	case  23:
	{
		CONST LONG __1M = 1024 * 1024;
		ThrottleControl_NS::CThrottleControl tc(__1M, 3, 2 * __1M);

		CONST LONGLONG totolSize = 100 * __1M;
		CONST LONG DELTA = 1024;

		LONGLONG transffered = 0;
		LONG sleepTime = 0;

		LONG elapsed = GetTickCount();
		for (;;)
		{
			transffered += DELTA;
			sleepTime += tc.Throttle(DELTA);
			if (transffered >= totolSize)
			{
				break;
			}
		}
		elapsed = GetTickCount() - elapsed;

		int actualSpeed = transffered * 1000 / elapsed;
		long elapsedSec = elapsed / 1000;
		int tollerance = actualSpeed - 1024 * 1024;
		tollerance = actualSpeed - 1024 * 1024;
		tollerance = actualSpeed - 1024 * 1024;
	}
	break;
	case 22:
	{
		LONGLONG InitialSize = 4 * 1024;
		LONGLONG AllocDelta = 4 * 1024;
		LONGLONG MaxAllocSize = 16 * 1024;
		CComPtr<IFileDeviceMem> spIFileDeviceMem;
		HRESULT hr = CreateInstanceCFileDeviceMemAutoAlloc(InitialSize, AllocDelta, MaxAllocSize, &spIFileDeviceMem, TRUE);
		static const BYTE data[1025] = { 's' };

		for (int ii = 0; ii < 24; ++ ii)
		{
			ULONG NumberOfBytesWritten = 0;
			spIFileDeviceMem->Write(_countof(data), &NumberOfBytesWritten, data);
			if (0 == NumberOfBytesWritten)
			{
				break;
			}
		}


		CComPtr<IFileDevice> spIDest;
		hr = CreateInstanceFileDeviceWin32(
			TEXT("debug.txt"),//IN LPCTSTR strFileName,
			GENERIC_WRITE,//IN ULONG ulDesiredAccess,
			0,//IN ULONG ulShareMode,
			CREATE_ALWAYS,//IN ULONG ulCreationDisposition,
			0,//IN ULONG ulFlagsAndAttributes,
			&spIDest,//OUT IFileDevice ** ppFileDevice,
			TRUE//BOOL bCreateMember
			);

		LONGLONG NewFilePointer = 0;
		spIFileDeviceMem->SetPointerEx(0, &NewFilePointer, FILE_BEGIN);
		spIDest->SetPointerEx(0, &NewFilePointer, FILE_BEGIN);
		SimpleSTCopy(spIFileDeviceMem, spIDest);

	}
	break;
	case 21:
	{
		test_mtpool_quue_v2::test();
		return 0;
	}
	break;
	case  20:
	{
		using namespace MTQueueV2_NS;
		CMTPoolQueue<int> mtpq(9);
		mtpq.CreateMember();

		//////////////////////////////////////////////////////////////////////////
		//provider
		{
			int * p = nullptr;
			mtpq.GetFree(&p);
			*p = 10000;
			mtpq.PutData(p, TRUE);
		}

		//////////////////////////////////////////////////////////////////////////
		//consumer
		{
			int * p = nullptr;
			mtpq.GetData(&p, TRUE);
			*p = -1;
			mtpq.PutFree(p);

			mtpq.SignalFinished();
			mtpq.GetData(&p, TRUE);
		}

		mtpq.DestroyMember();

	}
	break;
	case 19:
	{
		using namespace MTQueueV2_NS;
		CFreeSlots<int> test(nullptr, nullptr, nullptr, nullptr, 100);
		CDataSlots<int> test2(nullptr, nullptr, nullptr, nullptr, 100);
		//int x = (int)EWaitIndex::FATAL_ERROR;
		//int y = (int)EWaitIndex::FINISHED;
		//x = x - y;
	}
	break;
	case  18:
	{
		simple_fraction_ns::CFraction<LONGLONG> f1(-240, 0), f2(997766, -6 * 0);
		simple_fraction_ns::CFraction<LONGLONG> sum = f1 + f2;
		simple_fraction_ns::CFraction<LONGLONG> diff = f1 - f2;
		simple_fraction_ns::CFraction<LONGLONG> mul = f1 * f2;
		simple_fraction_ns::CFraction<LONGLONG> div = f1 / f2;

		auto v = sum + diff * mul / div;

		PRINT_LINE(L"%s = %I64d/%I64d", L"f1", f1.GetUp(), f1.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"f2", f2.GetUp(), f2.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"sum", sum.GetUp(), sum.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"diff", diff.GetUp(), diff.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"mul", mul.GetUp(), mul.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"div", div.GetUp(), div.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"v", v.GetUp(), v.GetDown());
	}
	break;
	case  17:
	{
		simple_fraction_ns::CFraction<LONGLONG> f1(-240, 1200), f2(997766, -6 * 997766);
		simple_fraction_ns::CFraction<LONGLONG> sum = f1 + f2;
		simple_fraction_ns::CFraction<LONGLONG> diff = f1 - f2;
		simple_fraction_ns::CFraction<LONGLONG> mul = f1 * f2;
		simple_fraction_ns::CFraction<LONGLONG> div = f1 / f2;

		auto v = sum + diff * mul / div;

		PRINT_LINE(L"%s = %I64d/%I64d", L"f1", f1.GetUp(), f1.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"f2", f2.GetUp(), f2.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"sum", sum.GetUp(), sum.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"diff", diff.GetUp(), diff.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"mul", mul.GetUp(), mul.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"div", div.GetUp(), div.GetDown());
		PRINT_LINE(L"%s = %I64d/%I64d", L"v", v.GetUp(), v.GetDown());
	}
	break;
	case 16:
	{
		simple_fraction_ns::CFraction<LONG> f1(1200, -240), f2(789, -1987 * 789);
		simple_fraction_ns::CFraction<LONG> sum = f1 + f2;
		simple_fraction_ns::CFraction<LONG> diff = f1 - f2;
		simple_fraction_ns::CFraction<LONG> mul = f1 * f2;
		simple_fraction_ns::CFraction<LONG> div = f1 / f2;

		auto v = sum + diff * mul / div;

		PRINT_LINE(L"%s = %d/%d", L"f1", f1.GetUp(), f1.GetDown());
		PRINT_LINE(L"%s = %d/%d", L"f2", f2.GetUp(), f2.GetDown());
		PRINT_LINE(L"%s = %d/%d", L"sum", sum.GetUp(), sum.GetDown());
		PRINT_LINE(L"%s = %d/%d", L"diff", diff.GetUp(), diff.GetDown());
		PRINT_LINE(L"%s = %d/%d", L"mul", mul.GetUp(), mul.GetDown());
		PRINT_LINE(L"%s = %d/%d", L"div", div.GetUp(), div.GetDown());
		PRINT_LINE(L"%s = %d/%d", L"v", v.GetUp(), v.GetDown());

	}
	break;
	case 0:
		return _tmain_test_CNameValueStream();
		break;
	case 1:
		return _tmain_test_file_device_mem();
		break;
	case 2:
		return test_mtcopy_main(argc, argv);
		break;
	case 3:
		return _tmain_test_mt_pool_queue(argc, argv);
		break;
	case 4:
		return _tmain_test_mtpool_st(argc, argv);
		break;
	case 5:
		return TestBitmap(argv[1]);
		break;
	case 6:
		_tmain_test_circular_buffer(argc, argv);
		break;
	case 7:
	{
			  LONGLONG TotalSize = 1;
			  TotalSize <<= 40;
			  if (argc > 1)
			  {
				  TotalSize = _tcstoi64(argv[1], nullptr, 0);
				  TotalSize <<= 30;
			  }

			  LONG BlockSize = 4 * 1024;
			  if (argc > 2)
			  {
				  BlockSize = _tcstol(argv[2], nullptr, 0);
				  BlockSize <<= 10;
			  }

			  LONG diff0 = 1;
			  if (argc > 3)
			  {
				  diff0 = _tcstol(argv[3], nullptr, 0);
			  }

			  LONG diff1 = 2;
			  if (argc > 4)
			  {
				  diff1 = _tcstol(argv[4], nullptr, 0);
			  }


			  CopyMemPerf(TotalSize, BlockSize, diff0, diff1);
	}
		break;
	case 8:
	{
			  CIOInstantSpeed is(2LL << 20, 3000LL);
			  is.Start();
			  for (int ii = 0; ii < 100; ++ii)
			  {
				  Sleep(500);
				  LONGLONG speed = is.Speed(1 << 20);
				  if (speed > 0)
				  {
					  _ftprintf_s(stdout, TEXT("%I64d Bytes/Sec") TEXT("\r\n"), speed);
				  }
			  }

			  LONGLONG avapeed = is.AverageSpeed(-1LL);
			  _ftprintf_s(stdout, TEXT("Aveage spedd %I64d Bytes/Sec") TEXT("\r\n"), avapeed);

	}
		break;
	case 9:
	{
			  main_test_all(argc, argv);
	}
		break;
	case 10:
		test_mtcopy_main_size(argc, argv);
		break;
	case  11:
		_tmain_test_task_monitor();
		break;
	case 12:
		_tmain_test_ref_pointer(argc, argv);
		break;
	case 13:
		_tmain_backup(argc, argv);
		break;
	case 14:
		_tmain_restore(argc, argv);
		break;
	case 15:
		_tmain_backup_restore(argc, argv);
		break;
	default:
		break;
	}

	return 0;
}