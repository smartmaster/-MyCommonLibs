#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <stack>
#include <deque>
using namespace std;

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"


namespace MTQueueV2_NS
{
	/***INTERNAL***/
	template<typename TItem>
	class CFreeSlots : public ISimpleUnknown
	{

	private:
		enum class EWaitIndexFree : INT
		{
			FATAL_ERROR = 0,
			CANCEL = 1,
			PROVIDER_FINISHED = 2,
			FINISHED = 3,
			SEMORPHORE = 4,
			MAX_COUNT = 5
		};

	private:
		LONG m_refCount;
		HANDLE m_handles[(INT)EWaitIndexFree::MAX_COUNT]; /***INCOMMING***/ /***INTERNAL***/ /***CLEANUP***/
		CRITICAL_SECTION m_cs; /***INTERNAL***/ /***CLEANUP***/
		INT m_itemCount; /***INCOMMING***/
		stack<TItem*> m_stack; /***INTERNAL***/
		vector<TItem*> m_vecShadow; /***INTERNAL***/ /***CLEANUP***/
	public:
		CFreeSlots(HANDLE hFATAL_ERROR, HANDLE hCANCEL, HANDLE hRPOVIDER_FINISHED, HANDLE hFINISHED, INT itemCount) :
			m_refCount(0),
			m_itemCount(itemCount)
		{
			m_handles[(INT)EWaitIndexFree::FATAL_ERROR] = hFATAL_ERROR;
			m_handles[(INT)EWaitIndexFree::CANCEL] = hCANCEL;
			m_handles[(INT)EWaitIndexFree::PROVIDER_FINISHED] = hRPOVIDER_FINISHED;
			m_handles[(INT)EWaitIndexFree::FINISHED] = hFINISHED;
			m_handles[(INT)EWaitIndexFree::SEMORPHORE] = nullptr;
		}

		IMP_CLASS_DESTRUCTOR(CFreeSlots);
	public:
		//struct ISimpleUnknown : public IUnknown
		//{
		//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
		//	//	IUnknown
		//	//{
		//	//public:
		//	//	BEGIN_INTERFACE
		//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		//	//		/* [in] */ REFIID riid,
		//	//		/* [iid_is][out] */ void **ppvObject) = 0;

		//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

		//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

		//	//	END_INTERFACE
		//	//};
		IMP_IUNKNOWN_METHODS(m_refCount);

		//	virtual LPCTSTR WhoAmI() = 0;
		IMP_WHO_AM_I(TEXT("class CFreeSlots : public ISimpleUnknown"));
		//	virtual HRESULT CreateMember() = 0;
		virtual HRESULT CreateMember()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			m_handles[(INT)EWaitIndexFree::SEMORPHORE] = CreateSemaphore(
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
				m_itemCount,//_In_      LONG lInitialCount,
				m_itemCount,//_In_      LONG lMaximumCount,
				nullptr//_In_opt_  LPCTSTR lpName
				);
			if (nullptr == m_handles[(INT)EWaitIndexFree::SEMORPHORE])
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("CreateSemaphore"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			BOOL bRet = InitializeCriticalSectionAndSpinCount(
				&m_cs,//_Out_  LPCRITICAL_SECTION lpCriticalSection,
				0x1000//_In_   DWORD dwSpinCount
				);
			if (!bRet)
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("InitializeCriticalSectionAndSpinCount"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			for (INT ii = 0; ii < m_itemCount; ++ii)
			{
				TItem * item = new TItem();
				m_stack.push(item);
				m_vecShadow.push_back(item);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
		//	virtual HRESULT DestroyMember() = 0;
		virtual HRESULT DestroyMember()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			if (nullptr != m_handles[(INT)EWaitIndexFree::SEMORPHORE])
			{
				CloseHandle(m_handles[(INT)EWaitIndexFree::SEMORPHORE]);
				m_handles[(INT)EWaitIndexFree::SEMORPHORE] = nullptr;
			}

			//////////////////////////////////////////////////////////////////////////
			DeleteCriticalSection(&m_cs);

			//////////////////////////////////////////////////////////////////////////
			for (size_t ii = 0; ii < m_vecShadow.size(); ++ii)
			{
				delete m_vecShadow[ii];
				m_vecShadow[ii] = nullptr;
			}
			m_vecShadow.clear();

			while (m_stack.size())
			{
				m_stack.pop();
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
		//};

		HRESULT Get(TItem ** pp)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			*pp = nullptr;
			DWORD TIME_OUT = 1000 * 60 * 60 * 3;
			DWORD dwWait = WaitForMultipleObjects(_countof(m_handles), m_handles, FALSE, TIME_OUT);
			if ((DWORD)EWaitIndexFree::SEMORPHORE == dwWait)
			{
				EnterCriticalSection(&m_cs);
				*pp = m_stack.top();
				m_stack.pop();
				LeaveCriticalSection(&m_cs);
			}
			else if ((DWORD)EWaitIndexFree::FATAL_ERROR == dwWait)
			{
				hr = E_FAIL;
				D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("Error has occured"));
				LEAVE_BLOCK(0);
			}
			else if ((DWORD)EWaitIndexFree::CANCEL == dwWait)
			{
				hr = HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED);
				D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("Error operation aborted"));
				LEAVE_BLOCK(0);
			}
			else if ((DWORD)EWaitIndexFree::PROVIDER_FINISHED == dwWait)
			{
				D_INFO(0, TEXT("WaitForMultipleObjects") TEXT(" provider finished"));
				LEAVE_BLOCK(0);
			}
			else if ((DWORD)EWaitIndexFree::FINISHED == dwWait)
			{
				D_INFO(0, TEXT("WaitForMultipleObjects") TEXT(" finished"));
				LEAVE_BLOCK(0);
			}
			else if (WAIT_TIMEOUT == dwWait)
			{
				hr = HRESULT_FROM_WIN32(WAIT_TIMEOUT);
				D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("wait timeout"));
				LEAVE_BLOCK(0);
			}
			else
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("wait failed"));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}

		HRESULT Put(TItem * p)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			EnterCriticalSection(&m_cs);
			m_stack.push(p);
			LeaveCriticalSection(&m_cs);

			BOOL bRet = ReleaseSemaphore(m_handles[(INT)EWaitIndexFree::SEMORPHORE], 1, nullptr);
			if (!bRet)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("ReleaseSemaphore"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
	};

	/***INTERNAL***/
	template<typename TItem>
	class CDataSlots : public ISimpleUnknown
	{
	private:
		enum class EWaitIndexData : int
		{
			FATAL_ERROR = 0,
			CANCEL = 1,
			SEMORPHORE = 2,
			PROVIDER_FINISHED = 3,
			FINISHED = 4,
			MAX_COUNT = 5
		};

	private:
		LONG m_refCount;
		HANDLE m_handles[(INT)EWaitIndexData::MAX_COUNT]; /***INCOMMING***/ /***INTERNAL***/ /***CLEANUP***/
		CRITICAL_SECTION m_cs; /***INTERNAL***/ /***CLEANUP***/
		INT m_itemCount; /***INCOMMING***/
		deque<TItem*> m_queue; /***INTERNAL***/
	public:
		CDataSlots(HANDLE hFATAL_ERROR, HANDLE hCANCEL, HANDLE hRPOVIDER_FINISHED, HANDLE hFINISHED, INT itemCount) :
			m_refCount(0),
			m_itemCount(itemCount)
		{
			m_handles[(INT)EWaitIndexData::FATAL_ERROR] = hFATAL_ERROR;
			m_handles[(INT)EWaitIndexData::CANCEL] = hCANCEL;
			m_handles[(INT)EWaitIndexData::SEMORPHORE] = nullptr;
			m_handles[(INT)EWaitIndexData::PROVIDER_FINISHED] = hRPOVIDER_FINISHED;
			m_handles[(INT)EWaitIndexData::FINISHED] = hFINISHED;
		}

		IMP_CLASS_DESTRUCTOR(CDataSlots);
	public:
		//struct ISimpleUnknown : public IUnknown
		//{
		//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
		//	//	IUnknown
		//	//{
		//	//public:
		//	//	BEGIN_INTERFACE
		//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		//	//		/* [in] */ REFIID riid,
		//	//		/* [iid_is][out] */ void **ppvObject) = 0;

		//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

		//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

		//	//	END_INTERFACE
		//	//};
		IMP_IUNKNOWN_METHODS(m_refCount);

		//	virtual LPCTSTR WhoAmI() = 0;
		IMP_WHO_AM_I(TEXT("class CDataSlots : public ISimpleUnknown"));
		//	virtual HRESULT CreateMember() = 0;
		virtual HRESULT CreateMember()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			m_handles[(INT)EWaitIndexData::SEMORPHORE] = CreateSemaphore(
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
				0,//_In_      LONG lInitialCount,
				m_itemCount,//_In_      LONG lMaximumCount,
				nullptr//_In_opt_  LPCTSTR lpName
				);
			if (nullptr == m_handles[(INT)EWaitIndexData::SEMORPHORE])
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("CreateSemaphore"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			BOOL bRet = InitializeCriticalSectionAndSpinCount(
				&m_cs,//_Out_  LPCRITICAL_SECTION lpCriticalSection,
				0x1000//_In_   DWORD dwSpinCount
				);
			if (!bRet)
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("InitializeCriticalSectionAndSpinCount"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
		//	virtual HRESULT DestroyMember() = 0;
		virtual HRESULT DestroyMember()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			if (nullptr != m_handles[(INT)EWaitIndexData::SEMORPHORE])
			{
				CloseHandle(m_handles[(INT)EWaitIndexData::SEMORPHORE]);
				m_handles[(INT)EWaitIndexData::SEMORPHORE] = nullptr;
			}

			//////////////////////////////////////////////////////////////////////////
			DeleteCriticalSection(&m_cs);

			//////////////////////////////////////////////////////////////////////////
			m_queue.clear();

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
		//};

		HRESULT Get(TItem ** pp, BOOL bGetFront)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			*pp = nullptr;
			DWORD TIME_OUT = 1000 * 60 * 60 * 3;
			DWORD dwWait = WaitForMultipleObjects(_countof(m_handles), m_handles, FALSE, TIME_OUT);
			if ((DWORD)EWaitIndexData::SEMORPHORE == dwWait)
			{
				EnterCriticalSection(&m_cs);
				if (bGetFront)
				{
					*pp = m_queue.front();
					m_queue.pop_front();
				}
				else
				{
					*pp = m_queue.back();
					m_queue.pop_back();
				}

				LeaveCriticalSection(&m_cs);
			}
			else if ((DWORD)EWaitIndexData::FATAL_ERROR == dwWait)
			{
				hr = E_FAIL;
				D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("Error has occured"));
				LEAVE_BLOCK(0);
			}
			else if ((DWORD)EWaitIndexData::CANCEL == dwWait)
			{
				hr = HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED);
				D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("Error operation aborted"));
				LEAVE_BLOCK(0);
			}
			else if ((DWORD)EWaitIndexData::PROVIDER_FINISHED == dwWait)
			{
				D_INFO(0, TEXT("WaitForMultipleObjects") TEXT(" provider finished"));
				LEAVE_BLOCK(0);
			}
			else if ((DWORD)EWaitIndexData::FINISHED == dwWait)
			{
				D_INFO(0, TEXT("WaitForMultipleObjects") TEXT(" finished"));
				LEAVE_BLOCK(0);
			}
			else if (WAIT_TIMEOUT == dwWait)
			{
				hr = HRESULT_FROM_WIN32(WAIT_TIMEOUT);
				D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("wait timeout"));
				LEAVE_BLOCK(0);
			}
			else
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("wait failed"));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}

		HRESULT Put(TItem * p, BOOL bPutBack)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			EnterCriticalSection(&m_cs);
			if (bPutBack)
			{
				m_queue.push_back(p);
			}
			else
			{
				m_queue.push_front(p);
			}

			LeaveCriticalSection(&m_cs);

			BOOL bRet = ReleaseSemaphore(m_handles[(INT)EWaitIndexData::SEMORPHORE], 1, nullptr);
			if (!bRet)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("ReleaseSemaphore"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
	};


	//////////////////////////////////////////////////////////////////////////
	template < typename TItem >
	class CMTPoolQueue : public ISimpleUnknown
	{
	private:
		enum class EWaitIndex : INT
		{
			FATAL_ERROR = 0,
			CANCEL = 1,
			PROVIDER_FINISHED = 2,
			FINISHED = 3,
			MAX_COUNT = 4
		};

	private:
		LONG m_refCount;
		INT m_itemCount; /***INCOMMING***/
		HANDLE m_handles[(INT)EWaitIndex::MAX_COUNT]; /***INTERNAL***/ /***CLEANUP***/
		CFreeSlots<TItem> * m_freeSlots; /***INTERNAL***/ /***CLEANUP***/
		CDataSlots<TItem> * m_dataSlots; /***INTERNAL***/ /***CLEANUP***/
	public:
		CMTPoolQueue(INT itemCount) :
			m_refCount(0),
			m_itemCount(itemCount),
			m_freeSlots(nullptr),
			m_dataSlots(nullptr)
		{
			ZeroMemory(m_handles, sizeof(m_handles));
		}

		IMP_CLASS_DESTRUCTOR(CMTPoolQueue);
	public:

		//struct ISimpleUnknown : public IUnknown
		//{
		//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
		//	//	IUnknown
		//	//{
		//	//public:
		//	//	BEGIN_INTERFACE
		//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		//	//		/* [in] */ REFIID riid,
		//	//		/* [iid_is][out] */ void **ppvObject) = 0;

		//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

		//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

		//	//	END_INTERFACE
		//	//};
		IMP_IUNKNOWN_METHODS(m_refCount);

		//	virtual LPCTSTR WhoAmI() = 0;
		IMP_WHO_AM_I(TEXT("class CMTPoolQueue : public ISimpleUnknown"));
		//	virtual HRESULT CreateMember() = 0;
		virtual HRESULT CreateMember()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			for (INT ii = 0; ii < _countof(m_handles); ++ii)
			{
				m_handles[ii] = CreateEvent(
					nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
					TRUE,//_In_      BOOL bManualReset,
					FALSE,//_In_      BOOL bInitialState,
					nullptr//_In_opt_  LPCTSTR lpName
					);
				if (nullptr == m_handles[ii])
				{
					//LastError = WSAGetLastError();
					LastError = GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
					break;
				}
				if (FAILED(hr))
				{
					LEAVE_BLOCK(0);
				}
			}

			//////////////////////////////////////////////////////////////////////////
			m_freeSlots = new CFreeSlots<TItem>(m_handles[(INT)EWaitIndex::FATAL_ERROR], m_handles[(INT)EWaitIndex::CANCEL], m_handles[(INT)EWaitIndex::PROVIDER_FINISHED], m_handles[(INT)EWaitIndex::FINISHED], m_itemCount);
			if (nullptr == m_freeSlots)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("new CFreeSlots"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			hr = m_freeSlots->CreateMember();
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("CreateMember"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			m_dataSlots = new CDataSlots<TItem>(m_handles[(INT)EWaitIndex::FATAL_ERROR], m_handles[(INT)EWaitIndex::CANCEL], m_handles[(INT)EWaitIndex::PROVIDER_FINISHED], m_handles[(INT)EWaitIndex::FINISHED], m_itemCount);
			if (nullptr == m_dataSlots)
			{
				hr = E_OUTOFMEMORY;
				D_API_ERR(0, TEXT("new CDataSlots"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			hr = m_dataSlots->CreateMember();
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("CreateMember"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
		//	virtual HRESULT DestroyMember() = 0;
		virtual HRESULT DestroyMember()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			for (INT ii = 0; ii < _countof(m_handles); ++ii)
			{
				if (nullptr != m_handles[ii])
				{
					CloseHandle(m_handles[ii]);
					m_handles[ii] = nullptr;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			if (nullptr != m_freeSlots)
			{
				m_freeSlots->DestroyMember();
				delete m_freeSlots;
				m_freeSlots = nullptr;
			}

			//////////////////////////////////////////////////////////////////////////
			if (nullptr != m_dataSlots)
			{
				m_dataSlots->DestroyMember();
				delete m_dataSlots;
				m_dataSlots = nullptr;
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
		//};

		//////////////////////////////////////////////////////////////////////////
		//used by provider
		HRESULT GetFree(TItem ** pp)
		{
			return m_freeSlots->Get(pp);
		}

		HRESULT PutData(TItem * p, BOOL bPutBack)
		{
			return m_dataSlots->Put(p, bPutBack);
		}


		//////////////////////////////////////////////////////////////////////////
		//used by consumer
		HRESULT GetData(TItem ** pp, BOOL bGetFront)
		{
			return m_dataSlots->Get(pp, bGetFront);
		}

		HRESULT PutFree(TItem * p)
		{
			return m_freeSlots->Put(p);
		}

		//////////////////////////////////////////////////////////////////////////
		HRESULT SignalError()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			BOOL bRet = SetEvent(m_handles[(INT)EWaitIndex::FATAL_ERROR]);
			if (!bRet)
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("SetEvent"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}

		//////////////////////////////////////////////////////////////////////////
		HRESULT SignalCancel()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			BOOL bRet = SetEvent(m_handles[(INT)EWaitIndex::CANCEL]);
			if (!bRet)
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("SetEvent"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}

		//////////////////////////////////////////////////////////////////////////
		HRESULT SignalProviderFinished()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			BOOL bRet = SetEvent(m_handles[(INT)EWaitIndex::PROVIDER_FINISHED]);
			if (!bRet)
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("SetEvent"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}

		//////////////////////////////////////////////////////////////////////////
		HRESULT SignalFinished()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			BOOL bRet = SetEvent(m_handles[(INT)EWaitIndex::FINISHED]);
			if (!bRet)
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("SetEvent"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}
	};


}//end namespace MTQueueV2_NS