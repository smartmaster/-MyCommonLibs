#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include <string>
#include <map>
using namespace std;

#include "..\PublicHeader\PublicInterfaceLock.h"

class CProcessGlobalData
{
private:
	CComPtr<DTRACELIB_N::IThreadRWLock> m_spRWLock;
	map<wstring, LPVOID> m_Data;

public:
	CProcessGlobalData();
	~CProcessGlobalData();

	HRESULT CreateMember();
	HRESULT DestroyMember();

	HRESULT SetData(CONST wstring & strName, LPVOID pData);
	LPVOID GetData(CONST wstring & strName);
	LPVOID ClearData(CONST wstring & strName);
};


class  CSIProcessGlobalData
{
public:
	static HRESULT CreateMember();
	static HRESULT DestroyMember();

	static HRESULT SetData(CONST wstring & strName, LPVOID pData);
	static LPVOID GetData(CONST wstring & strName);
	static LPVOID ClearData(CONST wstring & strName);

private:
	static CProcessGlobalData s_ProcessGlobalData;
};