#pragma once

#include "stdafx.h"

struct CTlsData
{
	DWORD m_dwProcessID;
	DWORD m_dwThreadID;
	LARGE_INTEGER m_liLastStatus;
	CString	m_strLastErrorMessage; //<sonmi01>2012-8-29 ###???
	CString m_strMessage;
	CString m_strHelper;

	CTlsData & operator=(CONST CTlsData & r);

	static DWORD WINAPI AllocateIndex();
	static BOOL WINAPI FreeIndex();

	static BOOL WINAPI AllocateData();
	static BOOL WINAPI FreeData();

	static BOOL WINAPI LoadData(CONST CTlsData * pTheadLocalData);
	static CTlsData * WINAPI GetData();
};