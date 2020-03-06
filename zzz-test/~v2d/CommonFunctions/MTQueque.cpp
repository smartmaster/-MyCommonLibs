#include "stdafx.h"
#include "MTQueque.h"

struct MyBufferIterm
{
	CString m_str1;
	CString m_str2;

	HRESULT CreateMember()
	{
		return S_OK;
	}

	HRESULT DestroyMember()
	{
		return S_OK;
	}
};


class CMTQueueTest
{
private:
	NS_MTQ::CSharedBuffer<MyBufferIterm> m_buffers;
	NS_MTQ::CMTQueue<MyBufferIterm> m_queue;
	CONST LONG m_LOOP_COUNT;
	volatile LONG m_nCount;

public:
	CMTQueueTest() :
	m_LOOP_COUNT(300000),
	m_nCount(0)
	{
	}

	VOID CreateMember()
	{
		CONST LONG nCount = 10;
		m_buffers.CreateMember(nCount);
		m_queue.CreateMember(nCount);
	}

	VOID Provider()
	{
		for (size_t ii=0; ii<m_LOOP_COUNT; ++ii)
		{
			MyBufferIterm * pData = NULL;
			HRESULT hr = m_buffers.Allocate(&pData);
			if (FAILED(hr) || NULL == pData)
			{
				m_buffers.Stop();
				m_queue.Stop();
				break;
			}
			else
			{
				LONG nCount = InterlockedIncrement(&m_nCount);
				pData->m_str1.Format(TEXT("%u") TEXT("\t") TEXT("hello....."), nCount);
				pData->m_str2.Format(TEXT("%u") TEXT("\t") TEXT("world....."), nCount);
			}

			hr = m_queue.Push(pData);
		}
	}

	static DWORD WINAPI ThreadProvider(LPVOID pVoid)
	{
		CMTQueueTest * pTest = (CMTQueueTest*)(pVoid);
		pTest->Provider();
		return 0;
	}

	VOID Consumer()
	{
		for (size_t ii=0; ii<m_LOOP_COUNT; ++ii)
		{
			MyBufferIterm * pData = NULL;
			HRESULT hr = m_queue.Pull(&pData);
			if (FAILED(hr) || NULL == pData)
			{
				m_buffers.Stop();
				m_queue.Stop();
				break;
			}
			else
			{
				_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), pData->m_str1.GetString());
				_ftprintf_s(stdout, TEXT("%s") TEXT("\r\n"), pData->m_str2.GetString());
				m_buffers.Free(pData);
			}
		}
	}

	static DWORD WINAPI ThreadConsumer(LPVOID pVoid)
	{
		CMTQueueTest * pTest = (CMTQueueTest*)(pVoid);
		pTest->Consumer();
		return 0;
	}
};

VOID TestMTQ()
{
	CMTQueueTest queTest;
	queTest.CreateMember();

	CONST LONG THREAD_COUNT = 10;
	HANDLE hThred[THREAD_COUNT] = {NULL};
	DWORD dwTid[THREAD_COUNT] = {0};

	for (INT ii=0; ii<THREAD_COUNT/2; ++ii)
	{
		hThred[ii] = CreateThread(
			NULL,//__in_opt   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//__in       SIZE_T dwStackSize,
			CMTQueueTest::ThreadProvider,//__in       LPTHREAD_START_ROUTINE lpStartAddress,
			(LPVOID)(&queTest),//__in_opt   LPVOID lpParameter,
			0,//__in       DWORD dwCreationFlags,
			&dwTid[ii]//__out_opt  LPDWORD lpThreadId
			);
	}

	for (INT ii=THREAD_COUNT/2; ii<THREAD_COUNT; ++ii)
	{
		hThred[ii] = CreateThread(
			NULL,//__in_opt   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//__in       SIZE_T dwStackSize,
			CMTQueueTest::ThreadConsumer,//__in       LPTHREAD_START_ROUTINE lpStartAddress,
			(LPVOID)(&queTest),//__in_opt   LPVOID lpParameter,
			0,//__in       DWORD dwCreationFlags,
			&dwTid[ii]//__out_opt  LPDWORD lpThreadId
		);
	}

	WaitForMultipleObjects(
		THREAD_COUNT,//__in  DWORD nCount,
		hThred,//__in  const HANDLE* lpHandles,
		TRUE,//__in  BOOL bWaitAll,
		INFINITE//__in  DWORD dwMilliseconds
		);
}