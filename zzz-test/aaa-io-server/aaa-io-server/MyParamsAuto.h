#pragma once

#include <atlsafe.h>
#include "aaaioserver_i.h"

class ComInitAuto
{
	HRESULT m_hr;
public:
	ComInitAuto(DWORD dwCoInit)
	{
		m_hr = CoInitializeEx(nullptr, dwCoInit);
	}

	~ComInitAuto()
	{
		if (SUCCEEDED(m_hr))
		{
			CoUninitialize();
		}
	}

	HRESULT Hr()
	{
		return m_hr;
	}
};


//must have no more data member
class MyParamsAuto : public MyParams
{
public:
	VOID Zero()
	{
		Code = 0;
		CodeEx = 0;
		Flags = 0;
		FlagsEx = 0;
		StringData = nullptr;
		ArrayData = nullptr;
	}

	VOID Cleanup()
	{
		if (StringData)
		{
			CComBSTR temp;
			temp.Attach(StringData);
		}

		if (ArrayData)
		{
			CComSafeArray<BYTE> temp;
			temp.Attach(ArrayData);
		}

		Zero();
	}
public:
	MyParamsAuto()
	{
		Zero();
	}

	VOID Set(LPCTSTR psz)
	{
		CComBSTR temp(psz);
		StringData = temp.Detach();
	}


	VOID Set(SAFEARRAY * psa)
	{
		CComSafeArray<BYTE> sa;
		sa.Add(psa);
		ArrayData = sa.Detach();
	}

	VOID Set(LPCBYTE buffer, LONG length)
	{
		CComSafeArray<BYTE> sa;
		sa.Add(length, buffer);
		ArrayData = sa.Detach();
	}

	~MyParamsAuto()
	{
		Cleanup();
	}
};

