#include "stdafx.h"
#include "CodeBlockDef.h"
#include "UtililityAndHelper.h"

DWORD TranslateStartType(LPCWSTR StartTypeString)
{
	DWORD StartType = -1;
	if (0 == _wcsicmp(L"a", StartTypeString) || 0 == _wcsicmp(L"auto", StartTypeString))
	{
		StartType = SERVICE_AUTO_START;
	}
	else if (0 == _wcsicmp(L"m", StartTypeString) || 0 == _wcsicmp(L"manual", StartTypeString))
	{
		StartType = SERVICE_DEMAND_START;
	}
	else if (0 == _wcsicmp(L"d", StartTypeString) || 0 == _wcsicmp(L"disabled", StartTypeString))
	{
		StartType = SERVICE_DISABLED;
	}

	return StartType;
}

HRESULT QuotedStringsToVector( CONST CString & strQuoted, vector<CString> & vecStr )
{
	INT Begin = 0;
	INT End = 0;

	CONST TCHAR QUOTE = TEXT('"');
	while (TRUE)
	{
		Begin = strQuoted.Find(QUOTE, Begin);
		if (-1 == Begin)
		{
			break;
		}

		++Begin;
		End = strQuoted.Find(QUOTE, Begin);
		if (-1 == End)
		{
			break;
		}

		vecStr.push_back(strQuoted.Mid(Begin, End - Begin));

		Begin = End + 1;
	}

	return 0;
}

HRESULT VectorStringsToBuffer(CONST vector<CString> & vecStr, TCHAR ** ppBuffer)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	INT CharCount = 0;
	for (size_t ii = 0; ii < vecStr.size(); ++ ii) //char count
	{
		CharCount += vecStr[ii].GetLength();
	}
	CharCount += (vecStr.size() + 1); //null count. NOTE: ending double null

	//////////////////////////////////////////////////////////////////////////
	CHeapPtr<TCHAR> apBuffer;
	apBuffer.Allocate(CharCount);
	if (NULL == apBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		break;
	}
	ZeroMemory(apBuffer.m_pData, CharCount * sizeof(TCHAR));

	//////////////////////////////////////////////////////////////////////////
	INT Index = 0;
	for (size_t ii = 0; ii < vecStr.size(); ++ ii)
	{
		_tcscpy(apBuffer.m_pData + Index, vecStr[ii].GetString());
		Index += (vecStr[ii].GetLength() + 1);
	}

	*ppBuffer = apBuffer.Detach();
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


HRESULT QuotedStringsToBuffer( LPCTSTR pQuoted, TCHAR ** ppBuffer )
{
	vector<CString> vecStr;
	QuotedStringsToVector(pQuoted, vecStr);
	return VectorStringsToBuffer(vecStr, ppBuffer);
}


VOID FreeQuotedStringsBuffer( TCHAR * pBuffer )
{
	CHeapPtr<TCHAR> apBuffer;
	apBuffer.Attach(pBuffer);
}
