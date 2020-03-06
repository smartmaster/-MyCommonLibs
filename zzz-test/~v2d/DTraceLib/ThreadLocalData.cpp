#include "stdafx.h"
#include "ThreadLocalData.h"

static DWORD s_dwIndex = TLS_OUT_OF_INDEXES;

CTlsData & CTlsData::operator=( CONST CTlsData & r )
{
	m_dwProcessID = r.m_dwProcessID;
	m_dwThreadID = r.m_dwThreadID;
	m_liLastStatus.LowPart = r.m_liLastStatus.LowPart;
	m_liLastStatus.HighPart = r. m_liLastStatus.HighPart;
	m_strLastErrorMessage = r.m_strLastErrorMessage; //<sonmi01>2012-8-29 ###???
	m_strMessage = r.m_strMessage;
	m_strHelper = r.m_strHelper;
	return (*this);
}

DWORD WINAPI CTlsData::AllocateIndex()
{
	return (s_dwIndex = TlsAlloc());
}

BOOL WINAPI CTlsData::FreeIndex()
{
	BOOL bRet = TRUE;
	if (TLS_OUT_OF_INDEXES != s_dwIndex)
	{
		bRet = TlsFree(s_dwIndex);
		s_dwIndex = TLS_OUT_OF_INDEXES;
	}
	return bRet;
}

BOOL WINAPI CTlsData::AllocateData( )
{
	CTlsData * pTheadLocalData = new CTlsData;
	if (pTheadLocalData)
	{
		pTheadLocalData->m_dwProcessID = GetCurrentProcessId();
		pTheadLocalData->m_dwThreadID = GetCurrentThreadId();
		pTheadLocalData->m_liLastStatus.LowPart = 0;
		pTheadLocalData->m_liLastStatus.HighPart = 0;
	}
	
	return TlsSetValue(s_dwIndex, LPVOID(pTheadLocalData));
}

BOOL WINAPI CTlsData::FreeData()
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

BOOL WINAPI CTlsData::LoadData( CONST CTlsData * pTheadLocalData )
{
	CTlsData * pData = (CTlsData *)TlsGetValue(s_dwIndex);
	if (NULL == pData)
	{
		CTlsData::AllocateData();
		pData = (CTlsData *)TlsGetValue(s_dwIndex);
	}
	
	if (pData && pTheadLocalData)
	{
		(*pData) = (*pTheadLocalData);
	}
	return TRUE;
}

CTlsData * WINAPI CTlsData::GetData()
{
	return (CTlsData*)TlsGetValue(s_dwIndex);
}