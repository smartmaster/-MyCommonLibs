#pragma once

#include "stdafx.h"

#include <vector>
#include <map>
#include <deque>



namespace NS_MTQ 
{
	class TYPE_Contract
	{
	public:
		HRESULT CreateMember() {return S_OK;}
		HRESULT DestroyMember() {return S_OK;}
	};

	using std::vector;
	using std::map;
	using std::deque;

#define Ptr2U(TYPE, p) ((ULONG_PTR)(LPVOID)(TYPE*)(p))
#define U2Ptr(TYPE, u) ((TYPE*)(LPVOID)(ULONG_PTR)(u))

	template<class TYPE /*of TYPE_Contract*/>
	class CSharedBuffer
	{
	private:
		LONG m_nCount;
		HANDLE m_hSemaphoreItems;
		HANDLE m_hEventStop;
		CComCriticalSection m_CS;
		map<ULONG_PTR, BOOL> m_sharedItems;
		//vector<_Ty*> m_Items;
		//vector<BOOL> m_vecIsUsed;

	public:

		CSharedBuffer() :
		  m_nCount(0),
			  m_hSemaphoreItems(NULL),
			  m_hEventStop(NULL)
		  {
		  }

		  ~CSharedBuffer()
		  {
			  DestroyMember();
		  }

		  HRESULT CreateMember(LONG nCount)
		  {
			  HRESULT hr = S_OK;
			  DWORD dwLastError = 0;

			  do 
			  {
				  //******************************************
				  m_nCount = nCount;

				  //******************************************
				  m_hSemaphoreItems = CreateSemaphore(
					  NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
					  m_nCount,//__in      LONG lInitialCount,
					  m_nCount,//__in      LONG lMaximumCount,
					  NULL//__in_opt  LPCTSTR lpName
					  );
				  if (NULL == m_hSemaphoreItems)
				  {
					  dwLastError = GetLastError();
					  _OLD_DSETLASTSTATUS(dwLastError);
					  hr = HRESULT_FROM_WIN32(dwLastError);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("CreateSemaphore"), hr));
					  break;
				  }

				  //******************************************
				  m_hEventStop = CreateEvent(
					  NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpEventAttributes,
					  TRUE,//__in      BOOL bManualReset,
					  FALSE,//__in      BOOL bInitialState,
					  NULL//__in_opt  LPCTSTR lpName
					  );
				  if (NULL == m_hEventStop)
				  {
					  dwLastError = GetLastError();
					  _OLD_DSETLASTSTATUS(dwLastError);
					  hr = HRESULT_FROM_WIN32(dwLastError);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("CreateEvent"), hr));
					  break;
				  }


				  //******************************************
				  hr = m_CS.Init();
				  if (FAILED(hr))
				  {	
					  _OLD_DSETLASTSTATUS(hr);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("CS.Init"), hr));
					  break;
				  }

				  //******************************************
				  //m_vecIsUsed.resize(m_nCount, FALSE);
				  //m_Items.resize(m_nCount, NULL);
				  //for (size_t ii=0; ii<m_Items.size(); ++ii)
				  //{
					 // m_Items[ii] = new TYPE;
				  //}
				  for (size_t ii=0; ii<m_nCount; ++ii)
				  {
					  TYPE * pItem = new TYPE;
					  if (NULL == pItem)
					  {
						  hr = E_OUTOFMEMORY;
						  break;
					  }
					  ULONG_PTR uItem = Ptr2U(TYPE, pItem);
					  m_sharedItems.insert(make_pair(uItem, FALSE));

					  hr = pItem->CreateMember();
					  if (FAILED(hr))
					  {
						  break;
					  }
				  }
				  if (FAILED(hr))
				  {
					  break;
				  }

				  //******************************************
			  } while (FALSE);

			  return hr;
		  }

		  VOID GetAllBufferItemPtrs(vector<TYPE*> & vecItemPtr)
		  {
			  for (map<ULONG_PTR, BOOL>::iterator iter = m_sharedItems.begin();
				  iter != m_sharedItems.end();
				  ++iter)
			  {
				  ULONG_PTR uItem = iter->first;
				  TYPE * pItem = U2Ptr(TYPE, uItem);
				  vecItemPtr.push_back(pItem);
			  }
		  }

		  HRESULT DestroyMember()
		  {
			  HRESULT hr = S_OK;
			  DWORD dwLasterror = 0;

			  do 
			  {
				  //******************************************
				  m_nCount = 0;

				  //******************************************
				  if (m_hSemaphoreItems)
				  {
					  CloseHandle(m_hSemaphoreItems);
					  m_hSemaphoreItems = NULL;
				  }

				  //******************************************
				  if (m_hEventStop)
				  {
					  CloseHandle(m_hEventStop);
					  m_hEventStop = NULL;
				  }

				  //******************************************
				  hr = m_CS.Term();

				  //******************************************
				  //m_vecIsUsed.clear();
				  //for (size_t ii=0; ii<m_Items.size(); ++ii)
				  //{
					 // delete m_Items[ii];
				  //}
				  //m_Items.clear();
				  for (map<ULONG_PTR, BOOL>::iterator iter = m_sharedItems.begin();
					  iter != m_sharedItems.end();
					  ++iter)
				  {
					  ULONG_PTR uItem = iter->first;
					  TYPE * pItem = U2Ptr(TYPE, uItem);
					  pItem->DestroyMember();
					  delete pItem;
				  }
				  m_sharedItems.clear();

				  //******************************************
			  } while (FALSE);

			  return hr;
		  }

		  HRESULT Allocate(TYPE ** ppData)
		  {
			  HRESULT hr = S_OK;
			  DWORD dwLastError = 0;

			  do 
			  {
				  //******************************************
				  *ppData = NULL;

				  //******************************************
				  //DWORD dwMilliseconds = INFINITE;
				  //DWORD dwWait = WaitForSingleObject(
				  //	m_hSemaphoreHole,//__in  HANDLE hHandle,
				  //	dwMilliseconds//__in  DWORD dwMilliseconds
				  //	);
				  //if (WAIT_OBJECT_0 != dwWait)
				  //{
				  //	dwLastError = GetLastError();
				  //	_OLD_DSETLASTSTATUS(dwLastError);
				  //	hr = HRESULT_FROM_WIN32(dwLastError);
				  //	_OLD_DAPIERR((0, DRNPOS, TEXT("WaitForSingleObject"), hr, TEXT("Result=%u"), dwWait));
				  //	break;
				  //}
				  DWORD dwMilliseconds = INFINITE;
				  HANDLE hObjects[] = {m_hEventStop, m_hSemaphoreItems};
				  DWORD dwWait = WaitForMultipleObjects(
					  _countof(hObjects),//__in  DWORD nCount,
					  hObjects,//__in  const HANDLE* lpHandles,
					  FALSE,//__in  BOOL bWaitAll,
					  dwMilliseconds//__in  DWORD dwMilliseconds
					  );
				  if (WAIT_FAILED == dwWait)
				  {
					  dwLastError = GetLastError();
					  _OLD_DSETLASTSTATUS(dwLastError);
					  hr = HRESULT_FROM_WIN32(dwLastError);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("WaitForMultipleObjects"), hr));
					  break;
				  }
				  else if (WAIT_OBJECT_0 == dwWait)
				  {
					  break;
				  }
				  else if (WAIT_OBJECT_0 + 1 == dwWait)
				  {
					  //go on
				  }
				  else
				  {
					  hr = E_UNEXPECTED;
					  _OLD_DAPIERR((0, DRNPOS, TEXT("WaitForMultipleObjects"), hr));
					  break;
				  }

				  //******************************************
				  CComCritSecLock<CComCriticalSection> csLock(m_CS, FALSE);
				  hr = csLock.Lock();
				  if (FAILED(hr))
				  {	
					  _OLD_DSETLASTSTATUS(hr);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("Lock"), hr));
					  break;
				  }

				  //size_t ii = 0;
				  //for (ii=0; ii<m_vecIsUsed.size(); ++ii)
				  //{
					 // if (!m_vecIsUsed[ii])
					 // {
						//  break;
					 // }
				  //}

				  //if (ii < m_vecIsUsed.size())
				  //{
					 // m_vecIsUsed[ii] = TRUE;
					 // *ppData = m_Items[ii];
				  //}
				  //else
				  //{
					 // hr = E_OUTOFMEMORY;
					 // _OLD_DSETLASTSTATUS(hr);
					 // _OLD_DAPIERR((0, DRNPOS, TEXT("E_OUTOFMEMORY"), hr));
					 // break;
				  //}
				  map<ULONG_PTR, BOOL>::iterator iter;
				  for (iter = m_sharedItems.begin();
					  iter != m_sharedItems.end();
					  ++iter)
				  {
					  if (FALSE == iter->second)
					  {
						  iter->second = TRUE;
						  *ppData = U2Ptr(TYPE, iter->first);
						  break;
					  }
				  }
				  if (m_sharedItems.end() == iter)
				  {
					   hr = E_OUTOFMEMORY;
					   _OLD_DSETLASTSTATUS(hr);
					   _OLD_DAPIERR((0, DRNPOS, TEXT("E_OUTOFMEMORY"), hr));
					   break;
				  }

				  csLock.Unlock();

				  //******************************************
			  } while (FALSE);

			  return hr;
		  }

		  HRESULT Free(TYPE * pData)
		  {
			  HRESULT hr = S_OK;
			  DWORD dwLastError = 0;

			  do 
			  {
				  //******************************************
				  CComCritSecLock<CComCriticalSection> csLock(m_CS, FALSE);
				  hr = csLock.Lock();
				  if (FAILED(hr))
				  {	
					  _OLD_DSETLASTSTATUS(hr);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("Lock"), hr));
					  break;
				  }

				  //size_t ii=0;
				  //for (ii=0; ii<m_Items.size(); ++ii)
				  //{
					 // if (pData == m_Items[ii])
					 // {
						//  break;
					 // }
				  //}

				  //if (ii >= m_Items.size())
				  //{	
					 // hr = E_FAIL;
					 // _OLD_DSETLASTSTATUS(hr);
					 // _OLD_DAPIERR((0, DRNPOS, TEXT("Invalid pointer"), hr));
					 // break;
				  //}

				  //m_vecIsUsed[ii] = FALSE;
				  ULONG_PTR uItem = Ptr2U(TYPE, pData);
				  map<ULONG_PTR, BOOL>::iterator iter = m_sharedItems.find(uItem);
				  if (m_sharedItems.end() == iter)
				  {
					   hr = E_FAIL;
					   _OLD_DSETLASTSTATUS(hr);
					   _OLD_DAPIERR((0, DRNPOS, TEXT("Invalid pointer"), hr));
					   break;
				  }
				  else
				  {
					  iter->second = FALSE;
				  }

				  csLock.Unlock();

				  //******************************************
				  BOOL bRet = ReleaseSemaphore(
					  m_hSemaphoreItems,//__in       HANDLE hSemaphore,
					  1,//__in       LONG lReleaseCount,
					  NULL//__out_opt  LPLONG lpPreviousCount
					  );
				  if (!bRet)
				  {
					  dwLastError = GetLastError();
					  _OLD_DSETLASTSTATUS(dwLastError);
					  hr = HRESULT_FROM_WIN32(dwLastError);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("ReleaseSemaphore"), hr));
					  break;
				  }

				  //******************************************
			  } while (FALSE);

			  return hr;
		  }

		  HRESULT Stop()
		  {
			  HRESULT hr = S_OK;
			  SetEvent(m_hEventStop);
			  return hr;
		  }
	};

	template<class TYPE>
	class CMTQueue
	{
	private:
		LONG m_nCount;
		HANDLE m_hSemaphoreQueItems;
		HANDLE m_hEventStop;
		CComCriticalSection m_CS;
		deque<TYPE*> m_Queue;

	public:
		CMTQueue() :
		  m_nCount(0),
			  m_hSemaphoreQueItems(NULL),
			  m_hEventStop(NULL)
		  {
		  }

		  HRESULT CreateMember(LONG nCount)
		  {
			  HRESULT hr = S_OK;
			  DWORD dwLastError = 0;

			  do 
			  {
				  //******************************************
				  m_nCount = nCount;

				  //******************************************
				  m_hSemaphoreQueItems = CreateSemaphore(
					  NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
					  0,//__in      LONG lInitialCount,
					  m_nCount,//__in      LONG lMaximumCount,
					  NULL//__in_opt  LPCTSTR lpName
					  );
				  if (NULL == m_hSemaphoreQueItems)
				  {
					  dwLastError = GetLastError();
					  _OLD_DSETLASTSTATUS(dwLastError);
					  hr = HRESULT_FROM_WIN32(dwLastError);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("CreateSemaphore"), hr));
					  break;
				  }

				  //******************************************
				  m_hEventStop = CreateEvent(
					  NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpEventAttributes,
					  TRUE,//__in      BOOL bManualReset,
					  FALSE,//__in      BOOL bInitialState,
					  NULL//__in_opt  LPCTSTR lpName
					  );
				  if (NULL == m_hEventStop)
				  {
					  dwLastError = GetLastError();
					  _OLD_DSETLASTSTATUS(dwLastError);
					  hr = HRESULT_FROM_WIN32(dwLastError);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("CreateEvent"), hr));
					  break;
				  }

				  //******************************************
				  hr = m_CS.Init();
				  if (FAILED(hr))
				  {	
					  _OLD_DSETLASTSTATUS(hr);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("CS.Init"), hr));
					  break;
				  }

				  //******************************************
			  } while (FALSE);

			  return hr;
		  }

		  HRESULT DestroyMember()
		  {
			  HRESULT hr = S_OK;
			  DWORD dwLastError = 0;

			  do 
			  {
				  //******************************************
				  m_nCount = 0;

				  //******************************************
				  if (m_hSemaphoreQueItems)
				  {
					  CloseHandle(m_hSemaphoreQueItems);
					  m_hSemaphoreQueItems = NULL;
				  }

				  //******************************************
				  if (m_hEventStop)
				  {
					  CloseHandle(m_hEventStop);
					  m_hEventStop = NULL;
				  }

				  //******************************************
				  m_CS.Term();

				  //******************************************
				  m_Queue.clear();

				  //******************************************
			  } while (FALSE);

			  return hr;
		  }

		  HRESULT Push(TYPE * pData)
		  {
			  HRESULT hr = S_OK;
			  DWORD dwLastError = 0;

			  do 
			  {
				  //******************************************
				  CComCritSecLock<CComCriticalSection> csLock(m_CS, FALSE);
				  hr = csLock.Lock();
				  if (FAILED(hr))
				  {	
					  _OLD_DSETLASTSTATUS(hr);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("Lock"), hr));
					  break;
				  }

				  m_Queue.push_back(pData);

				  csLock.Unlock();

				  //******************************************
				  BOOL bRet = ReleaseSemaphore(
					  m_hSemaphoreQueItems,//__in       HANDLE hSemaphore,
					  1,//__in       LONG lReleaseCount,
					  NULL//__out_opt  LPLONG lpPreviousCount
					  );
				  if (!bRet)
				  {
					  dwLastError = GetLastError();
					  _OLD_DSETLASTSTATUS(dwLastError);
					  hr = HRESULT_FROM_WIN32(dwLastError);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("ReleaseSemaphore"), hr));
					  break;
				  }

			  } while (FALSE);

			  return hr;
		  }

		  HRESULT Pull(TYPE ** ppData)
		  {
			  HRESULT hr = S_OK;
			  DWORD dwLastError = 0;

			  do 
			  {
				  //******************************************
				  *ppData = NULL;

				  //******************************************
				  //DWORD dwMilliseconds = INFINITE;
				  //DWORD dwWait = WaitForSingleObject(
				  //	m_hSemaphoreData,//__in  HANDLE hHandle,
				  //	dwMilliseconds//__in  DWORD dwMilliseconds
				  //	);
				  //if (WAIT_OBJECT_0 != dwWait)
				  //{
				  //	dwLastError = GetLastError();
				  //	_OLD_DSETLASTSTATUS(dwLastError);
				  //	hr = HRESULT_FROM_WIN32(dwLastError);
				  //	_OLD_DAPIERR((0, DRNPOS, TEXT("WaitForSingleObject"), hr, TEXT("Result=%u"), dwWait));
				  //	break;
				  //}
				  DWORD dwMilliseconds = INFINITE;
				  HANDLE hObjects[] = {m_hEventStop, m_hSemaphoreQueItems};
				  DWORD dwWait = WaitForMultipleObjects(
					  _countof(hObjects),//__in  DWORD nCount,
					  hObjects,//__in  const HANDLE* lpHandles,
					  FALSE,//__in  BOOL bWaitAll,
					  dwMilliseconds//__in  DWORD dwMilliseconds
					  );
				  if (WAIT_FAILED == dwWait)
				  {
					  dwLastError = GetLastError();
					  _OLD_DSETLASTSTATUS(dwLastError);
					  hr = HRESULT_FROM_WIN32(dwLastError);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("WaitForMultipleObjects"), hr));
					  break;
				  }
				  else if (WAIT_OBJECT_0 == dwWait)
				  {
					  break;
				  }
				  else if (WAIT_OBJECT_0 + 1 == dwWait)
				  {
					  //go on
				  }
				  else
				  {
					  hr = E_UNEXPECTED;
					  _OLD_DAPIERR((0, DRNPOS, TEXT("WaitForMultipleObjects"), hr));
					  break;
				  }

				  //******************************************
				  CComCritSecLock<CComCriticalSection> csLock(m_CS, FALSE);
				  hr = csLock.Lock();
				  if (FAILED(hr))
				  {	
					  _OLD_DSETLASTSTATUS(hr);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("Lock"), hr));
					  break;
				  }

				  if (0 == m_Queue.size())
				  {	
					  hr = E_UNEXPECTED;
					  _OLD_DSETLASTSTATUS(hr);
					  _OLD_DAPIERR((0, DRNPOS, TEXT("Queue.size"), hr));
					  break;
				  }

				  *ppData = m_Queue.front();
				  m_Queue.pop_front();

				  csLock.Unlock();

				  //******************************************
			  } while (FALSE);

			  return hr;
		  }

		  HRESULT Stop()
		  {
			  HRESULT hr = S_OK;
			  SetEvent(m_hEventStop);
			  return hr;
		  }
	};

} //end namespace NS_MTQ