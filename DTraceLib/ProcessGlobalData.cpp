#include "stdafx.h"

#include "ProcessGlobalData.h"
#include "DTraceLib.h"


CProcessGlobalData::CProcessGlobalData()
{

}

CProcessGlobalData::~CProcessGlobalData()
{
	DestroyMember();
}

HRESULT CProcessGlobalData::CreateMember()
{
	DTRACELIB_N::CreateInstacnceSRWLock( &m_spRWLock, TRUE );
	return S_OK;
}

HRESULT CProcessGlobalData::DestroyMember()
{
	if (m_spRWLock)
	{
		m_spRWLock.Release();
	}

	if (m_Data.size())
	{
		m_Data.clear();
	}

	return S_OK;
}

HRESULT CProcessGlobalData::SetData( CONST wstring & strName, LPVOID pData )
{
	HRESULT hr = S_OK;
	m_spRWLock->Lock();
	map<wstring, PVOID>::const_iterator iter = m_Data.find(strName);
	if (iter == m_Data.end())
	{
		m_Data.insert(make_pair(strName, pData));
	}
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS);
	}
	m_spRWLock->Unlock();
	return hr;
}

LPVOID CProcessGlobalData::GetData( CONST wstring & strName )
{
	LPVOID pData = NULL;
	m_spRWLock->SharedLock();
	map<wstring, PVOID>::const_iterator iter = m_Data.find(strName);
	if (iter != m_Data.end())
	{
		pData = iter->second;
	}
	m_spRWLock->SharedUnlock();
	return pData;
}

LPVOID CProcessGlobalData::ClearData( CONST wstring & strName )
{
	HRESULT hr = S_OK;
	LPVOID pData = NULL;
	m_spRWLock->Lock();
	map<wstring, PVOID>::iterator iter = m_Data.find(strName);
	if (iter != m_Data.end())
	{
		pData = iter->second;
		m_Data.erase(iter);
	}
	m_spRWLock->Unlock();
	return pData;
}

HRESULT CSIProcessGlobalData::CreateMember()
{
	return s_ProcessGlobalData.CreateMember();
}

HRESULT CSIProcessGlobalData::DestroyMember()
{
	return s_ProcessGlobalData.DestroyMember();
}

HRESULT CSIProcessGlobalData::SetData( CONST wstring & strName, LPVOID pData )
{
	return s_ProcessGlobalData.SetData(strName, pData);
}

LPVOID CSIProcessGlobalData::GetData( CONST wstring & strName )
{
	return s_ProcessGlobalData.GetData(strName);
}

LPVOID CSIProcessGlobalData::ClearData( CONST wstring & strName )
{
	return s_ProcessGlobalData.ClearData(strName);
}

CProcessGlobalData CSIProcessGlobalData::s_ProcessGlobalData;

//////////////////////////////////////////////////////////////////////////
// API
HRESULT SetProcessGlobalData(LPCTSTR Name, LPVOID pData)
{
	return CSIProcessGlobalData::SetData(Name, pData);
}

LPVOID GetProcessGlobalData(LPCTSTR Name)
{
	return CSIProcessGlobalData::GetData(Name);
}

LPVOID ClearProcessGlobalData(LPCTSTR Name)
{
	return CSIProcessGlobalData::ClearData(Name);
}
