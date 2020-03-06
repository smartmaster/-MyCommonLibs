#include "stdafx.h"
#include "AsyncIoBuffer.h"

CAsyncIoBufferItem * CAsyncIoBufferItem::Allocate( ULONG nMetaDataLength, ULONG nMaxDataLength )
{
	DSTART(999);

	CAsyncIoBufferItem *  pAsyncIoBufferItem = NULL;

	//******************************************
	nMetaDataLength = AtlAlignUp(nMetaDataLength, 8);
	nMaxDataLength = AtlAlignUp(nMaxDataLength, 8);
	ULONG nStructLength = sizeof(CAsyncIoBufferItem) + nMetaDataLength + nMaxDataLength;

	//******************************************
	CHeapPtr<BYTE> apBufer;
	BOOL bRet = apBufer.Allocate(nStructLength);
	if (bRet)
	{
		pAsyncIoBufferItem = (CAsyncIoBufferItem *)(apBufer.Detach());
		ZeroMemory(pAsyncIoBufferItem, nStructLength);
		pAsyncIoBufferItem->m_nMetaInformationLength = nMetaDataLength;
		pAsyncIoBufferItem->m_nMaximumDataLength = nMaxDataLength;
	}
	else
	{
		_OLD_DSETLASTSTATUS(E_OUTOFMEMORY);
		_OLD_DAPIERR((0, DRNPOS, TEXT("Allocate"), E_OUTOFMEMORY, TEXT("[size]=[%u]"), nStructLength));
	}

	return pAsyncIoBufferItem;
}

VOID CAsyncIoBufferItem::Free( CAsyncIoBufferItem * pAsyncIoBufferItem )
{
	DSTART(9999);
	{
		CHeapPtr<BYTE> apBuffer((BYTE*)pAsyncIoBufferItem);
	}
}

INT CAsyncIoSharedBuffer::CreateMember( ULONG nMetaDataLength, ULONG nMaxDataLength, INT nItemCount)
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		m_nItemCount = nItemCount;
		m_nAllocatedItemCount = m_nItemCount + DELTA;

		//********************************************************************
		m_hSemaphore = CreateSemaphore(NULL, m_nItemCount, m_nItemCount, NULL);
		if (NULL == m_hSemaphore)
		{
			dwLastError = GetLastError();
			_OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateSemaphore"), dwLastError));
			nStatus = dwLastError;
			break;
		}

		//********************************************************************
		hr = m_cs.Init();
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("m_cs.Init()"), hr));
			nStatus = hr;
			break;
		}

		//********************************************************************
		for (INT i=0; i<m_nAllocatedItemCount; ++i)
		{
			CAsyncIoBufferItem * pAsyncIoBufferItem = CAsyncIoBufferItem::Allocate(nMetaDataLength, nMaxDataLength);
			if (pAsyncIoBufferItem)
			{
				m_SharedBuffers.push_back(pAsyncIoBufferItem);
			}
			else
			{
				_OLD_DAPIERR((0, DRNPOS, TEXT("CAsyncIoBufferItem::Allocate"), E_FAIL));
				nStatus = E_FAIL;
				break;
			}
		}
		if (nStatus)
		{
			break;
		}

		//******************************************
	} while (FALSE);

	return nStatus;
}

INT CAsyncIoSharedBuffer::DestroyMember()
{
	DSTART(999);

	INT nStatus = 0;

	//******************************************
	if (m_hSemaphore)
	{
		CloseHandle(m_hSemaphore);
		m_hSemaphore = NULL;
	}

	//******************************************
	m_cs.Term();

	//******************************************
	for (size_t i=0; i<m_SharedBuffers.size(); ++i)
	{
		CAsyncIoBufferItem::Free(m_SharedBuffers[i]);
		m_SharedBuffers[i] = NULL;
	}
	m_SharedBuffers.clear();

	return nStatus;
}

CAsyncIoBufferItem * CAsyncIoSharedBuffer::FindNotUsedBufferItem()
{
	DSTART(9999);

	CAsyncIoBufferItem * pAsyncIoBufferItem = NULL;
	for (INT i=0; i<m_nAllocatedItemCount; ++i)
	{
		if (!m_SharedBuffers[i]->m_bAllocated)
		{
			pAsyncIoBufferItem = m_SharedBuffers[i];
			break;
		}
	}
	return pAsyncIoBufferItem;
}

CAsyncIoBufferItem * CAsyncIoSharedBuffer::AllocateItem( DWORD dwTimeout )
{
	DSTART(999);

	CAsyncIoBufferItem * pAsyncIoBufferItem = NULL;
	CComCritSecLock<CComCriticalSection> Lock(m_cs, FALSE);
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	do 
	{
		//********************************************************************
		DWORD dwWait = WaitForSingleObject(m_hSemaphore, dwTimeout);
		if (WAIT_OBJECT_0 != dwWait)
		{
			dwLastError = GetLastError();
			_OLD_DSETLASTSTATUS(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("WaitForSingleObject"), dwLastError, TEXT("RetVal=%d"), dwWait));
			break;
		}

		//********************************************************************
		hr = Lock.Lock();
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Lock.Lock()"), hr));
			break;
		}

		//********************************************************************
		pAsyncIoBufferItem = FindNotUsedBufferItem();
		if (pAsyncIoBufferItem)
		{
			pAsyncIoBufferItem->m_bAllocated = TRUE;
		}
		else
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT("FindNotUsedBufferItem"), E_FAIL));
			ReleaseSemaphore(m_hSemaphore, 1, NULL); //should never go here
		}

		//********************************************************************
	} while (FALSE);

	return pAsyncIoBufferItem;
}

INT CAsyncIoSharedBuffer::RecycleItem( CAsyncIoBufferItem * pAsyncIoBufferItem )
{
	DSTART(999);

	INT nStatus = 0;
	DWORD dwLastError = 0;
	HRESULT hr = S_OK;

	CComCritSecLock<CComCriticalSection> Lock(m_cs, FALSE);

	do 
	{
		//********************************************************************
		hr = Lock.Lock();
		if (FAILED(hr))
		{
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("Lock.Lock()"), hr));
			nStatus = hr;
			break;
		}

		//********************************************************************
		if (NULL==pAsyncIoBufferItem || !pAsyncIoBufferItem->m_bAllocated)
		{
			_OLD_DAPIERR((0, DRNPOS, TEXT(__FUNCTION__), E_INVALIDARG));
			nStatus = E_INVALIDARG;
			break;
		}

		//********************************************************************
		pAsyncIoBufferItem->m_bAllocated = FALSE;
		ZeroMemory(&pAsyncIoBufferItem->m_OverLapped, sizeof(OVERLAPPED));
		CONST LONG RELEASE_COUNT =  1;
		ReleaseSemaphore(m_hSemaphore, RELEASE_COUNT, NULL);

		//********************************************************************
	} while (FALSE);

	return nStatus;
}

CAsyncIoSharedBuffer::CAsyncIoSharedBuffer() :
m_hSemaphore(NULL),
m_nItemCount(0),
m_nAllocatedItemCount(0)
{
	DSTART(9999);
}

CAsyncIoSharedBuffer::~CAsyncIoSharedBuffer()
{
	DSTART(9999);
	DestroyMember();
}

INT CAsyncIoSharedBuffer::RecycleAllItems()
{
	INT nStatus = 0;
	for (size_t ii=0; ii<m_SharedBuffers.size(); ++ii)
	{
		RecycleItem(m_SharedBuffers[ii]);
	}
	return nStatus;
}