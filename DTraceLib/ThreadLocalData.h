#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <deque>


struct CErrorCodeString
{
	LONG m_Code;
	CString m_String;

	//////////////////////////////////////////////////////////////////////////
	CErrorCodeString();
};


struct CErrorCodeStringQueue
{
	DWORD m_dwProcessID;
	DWORD m_dwThreadID;
	LONG m_TotalErrorCount;
	std::deque<CErrorCodeString> m_Deque;

	//////////////////////////////////////////////////////////////////////////
	CErrorCodeStringQueue();
	LONG GetCount() CONST;
	HRESULT GetCode(LONG index)  CONST;
	LPCTSTR GetString(LONG index)  CONST;
	VOID Put(LONG code, LPCTSTR str);
};



struct CTlsData
{
	//////////////////////////////////////////////////////////////////////////
	DWORD m_dwProcessID;
	DWORD m_dwThreadID;

	//////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER m_liLastStatus;
	LARGE_INTEGER m_liFirstStatus;

	CString	m_strLastErrorMessage; //<sonmi01>2012-8-29 ###???
	CString	m_strFirstErrorMessage; //<sonmi01>2012-8-29 ###???
	
	//////////////////////////////////////////////////////////////////////////
	CString m_strMessage;
	CString m_strHelper;

	//////////////////////////////////////////////////////////////////////////
	CErrorCodeStringQueue m_ErrorCodeStringQueue;

	CTlsData();

private:
	CTlsData & operator=(CONST CTlsData & r);
	CTlsData(CONST CTlsData & r);
};

struct CTlsDataManager
{
	static DWORD WINAPI AllocateIndex();
	static BOOL WINAPI FreeIndex();

	static BOOL WINAPI AllocateData();
	static BOOL WINAPI FreeData();

	static CTlsData * WINAPI GetData();
	static BOOL WINAPI TestAndAllocateData(CONST CTlsData * pTheadLocalData);

private:
	static DWORD s_dwIndex;
};