#include "stdafx.h"
#include "ThreadLocalData.h"


#if 0
CTlsData & CTlsData::operator=( CONST CTlsData & r )
{
	m_dwProcessID = r.m_dwProcessID;
	m_dwThreadID = r.m_dwThreadID;

	m_liLastStatus.LowPart = r.m_liLastStatus.LowPart;
	m_liLastStatus.HighPart = r. m_liLastStatus.HighPart;
	m_strLastErrorMessage = r.m_strLastErrorMessage; //<sonmi01>2012-8-29 ###???

	m_liFirstStatus.LowPart = r.m_liFirstStatus.LowPart;
	m_liFirstStatus.HighPart = r. m_liFirstStatus.HighPart;
	m_strFirstErrorMessage = r.m_strFirstErrorMessage; //<sonmi01>2012-8-29 ###???

	m_strMessage = r.m_strMessage;
	m_strHelper = r.m_strHelper;

	return (*this);
}
#endif

CTlsData::CTlsData()
{
	m_dwProcessID = GetCurrentProcessId();
	m_dwThreadID = GetCurrentThreadId();
	m_liFirstStatus.QuadPart = 0;
	m_liLastStatus.QuadPart = 0;
}

DWORD WINAPI CTlsDataManager::AllocateIndex()
{
	return (s_dwIndex = TlsAlloc());
}

BOOL WINAPI CTlsDataManager::FreeIndex()
{
	BOOL bRet = TRUE;
	if (TLS_OUT_OF_INDEXES != s_dwIndex)
	{
		bRet = TlsFree(s_dwIndex);
		s_dwIndex = TLS_OUT_OF_INDEXES;
	}
	return bRet;
}

BOOL WINAPI CTlsDataManager::AllocateData( )
{
	CTlsData * pTheadLocalData = new CTlsData;
	//if (pTheadLocalData)
	//{
	//	pTheadLocalData->m_dwProcessID = GetCurrentProcessId();
	//	pTheadLocalData->m_dwThreadID = GetCurrentThreadId();
	//	pTheadLocalData->m_liFirstStatus.QuadPart = 0;
	//	pTheadLocalData->m_liLastStatus.QuadPart = 0;
	//}
	
	return TlsSetValue(s_dwIndex, LPVOID(pTheadLocalData));
}

BOOL WINAPI CTlsDataManager::FreeData()
{
	CTlsData * pTheadLocalData = (CTlsData *)TlsGetValue(s_dwIndex);
	if (pTheadLocalData)
	{
		delete pTheadLocalData;
		pTheadLocalData = NULL;
		TlsSetValue(s_dwIndex, NULL);
	}
	return TRUE;
}

BOOL WINAPI CTlsDataManager::TestAndAllocateData( CONST CTlsData * pTheadLocalData )
{
	CTlsData * pData = (CTlsData *)TlsGetValue(s_dwIndex);
	if (NULL == pData)
	{
		CTlsDataManager::AllocateData();
		pData = (CTlsData *)TlsGetValue(s_dwIndex);
	}

#if 0
	if (pData && pTheadLocalData)
	{
		(*pData) = (*pTheadLocalData);
	}
#endif

	return TRUE;
}

CTlsData * WINAPI CTlsDataManager::GetData()
{
	return (CTlsData*)TlsGetValue(s_dwIndex);
}

DWORD CTlsDataManager::s_dwIndex = TLS_OUT_OF_INDEXES;

//<sonmi01>2014-8-22 #
CErrorCodeStringQueue::CErrorCodeStringQueue()
{
	m_dwProcessID = GetCurrentProcessId();
	m_dwThreadID = GetCurrentThreadId();
	m_TotalErrorCount = 0;
}

LONG CErrorCodeStringQueue::GetCount() CONST
{
//#pragma message("larger integer truncation ... @ " __FUNCTION__)
	return /*(LONG)*/m_Deque.size();
}

HRESULT CErrorCodeStringQueue::GetCode(LONG index) CONST
{
	if (index >= m_Deque.size())
	{
		return S_OK;
	}
	return m_Deque[index].m_Code;
}

LPCTSTR CErrorCodeStringQueue::GetString(LONG index) CONST
{
	if (index >= m_Deque.size())
	{
		return nullptr;
	}
	return m_Deque[index].m_String.GetString();
}

VOID CErrorCodeStringQueue::Put(LONG code, LPCTSTR str)
{
	++m_TotalErrorCount;

	CONST LONG MAX_COUNT = 256;
	if (m_Deque.size() >= MAX_COUNT)
	{
		m_Deque.pop_front();
	}

	CErrorCodeString temp;
	temp.m_Code = code;
	temp.m_String = str;
	m_Deque.push_back(temp);
}

CErrorCodeString::CErrorCodeString() :
m_Code(0)
{
}
