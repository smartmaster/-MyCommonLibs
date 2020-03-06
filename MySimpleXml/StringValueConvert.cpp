#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlenc.h>

#include <algorithm>

#include "..\PublicHeader\PublicWideNarrowChar.h"
#include "StringValueConvert.h"

//////////////////////////////////////////////////////////////////////////
static VOID ValueToStringPrivate(CONST vector<BYTE> & Value, CString & str);
static VOID StringToValuePrivate(vector<BYTE> & Value, CONST CString & str);
//////////////////////////////////////////////////////////////////////////

template<>
VOID ValueToString<bool>(CONST bool & Value, CString & str)
{
	Value ? str = TEXT("true") : str = TEXT("false");
	return;
}

template<>
VOID StringToValue<bool>(bool & Value, CONST CString & str)
{
	(0 == str.GetLength() || 0 == str.CompareNoCase(TEXT("false"))) ? Value = false : Value = true;
	return;
}

template<>
VOID ValueToString<CString>(CONST CString & Value, CString & str)
{
	str = Value;
}

template<>
VOID ValueToString<wstring>(CONST wstring & Value, CString & str)
{
	str = Value.c_str();
}

template<>
VOID ValueToString<LONGLONG>(CONST LONGLONG & Value, CString & str)
{
	str.Format(TEXT("%I64d"), Value);
}

template<>
VOID ValueToString<ULONGLONG>(CONST ULONGLONG & Value, CString & str)
{
	str.Format(TEXT("%I64u"), Value);
}

template<>
VOID ValueToString<LONG>(CONST LONG & Value, CString & str)
{
	str.Format(TEXT("%d"), Value);
}

template<>
VOID ValueToString<ULONG>(CONST ULONG & Value, CString & str)
{
	str.Format(TEXT("%u"), Value);
}

template<>
VOID ValueToString<CHAR>(CONST CHAR & Value, CString & str)
{
	ValueToString<LONG>(Value, str);
}

template<>
VOID ValueToString<UCHAR>(CONST UCHAR & Value, CString & str)
{
	ValueToString<ULONG>(Value, str);
}

template<>
VOID ValueToString<SHORT>(CONST SHORT & Value, CString & str)
{
	ValueToString<LONG>(Value, str);
}

template<>
VOID ValueToString<USHORT>(CONST USHORT & Value, CString & str)
{
	ValueToString<ULONG>(Value, str);
}

template<>
VOID ValueToString<INT>(CONST INT & Value, CString & str)
{
	ValueToString<LONG>(Value, str);
}

template<>
VOID ValueToString<UINT>(CONST UINT & Value, CString & str)
{
	ValueToString<ULONG>(Value, str);
}

//template<>
static VOID ValueToStringPrivate(CONST vector<BYTE> & Value, CString & str)
{
	if (Value.empty())
	{
		return;
	}

	INT Len = Base64EncodeGetRequiredLength(Value.size()) + 16;

	CStringA strA;
	LPSTR pBuffer = strA.GetBuffer(Len);
	ZeroMemory(pBuffer, Len);
	Base64Encode(
		&Value[0],//const BYTE* pbSrcData,
		Value.size(),//int nSrcLen,
		pBuffer,//LPSTR szDest,
		&Len,//int* pnDestLen,
		ATL_BASE64_FLAG_NOCRLF//DWORD dwFlags = ATL_BASE64_FLAG_NONE
		);
	strA.ReleaseBuffer();

	str = strA;
}


template<>
VOID StringToValue(VectorBYTE & Value, CONST CString & str)
{
	StringToValuePrivate(Value/*.Buffer*//*public vector<byte>*/, str);
}

template<>
VOID ValueToString<GUID>(CONST GUID & Value, CString & str)
{
	CONST INT BUFFER_SIZE = 128;
	LPTSTR pBufferTemp = str.GetBuffer(BUFFER_SIZE);
	StringFromGUID2(
		Value,//_In_   REFGUID rguid,
		pBufferTemp,//_Out_  LPOLESTR lpsz,
		BUFFER_SIZE//_In_   int cchMax
		);
	str.ReleaseBuffer();
}

template<>
VOID StringToValue<LONGLONG>(LONGLONG & Value, CONST CString & str)
{
	Value = _tcstoi64(str.GetString(), NULL, 0);
}

template<>
VOID StringToValue<ULONGLONG>(ULONGLONG & Value, CONST CString & str)
{
	Value = _tcstoui64(str.GetString(), NULL, 0);
}

template<>
VOID StringToValue<LONG>(LONG & Value, CONST CString & str)
{
	Value = _tcstol(str.GetString(), NULL, 0);
}

template<>
VOID StringToValue<ULONG>(ULONG & Value, CONST CString & str)
{
	Value = _tcstoul(str.GetString(), NULL, 0);
}

template<>
VOID StringToValue<CHAR>(CHAR & Value, CONST CString & str)
{
	Value = (CHAR)_tcstol(str.GetString(), NULL, 0);
}

template<>
VOID StringToValue<UCHAR>(UCHAR & Value, CONST CString & str)
{
	Value = (UCHAR)_tcstoul(str.GetString(), NULL, 0);
}

template<>
VOID StringToValue<SHORT>(SHORT & Value, CONST CString & str)
{
	Value = (SHORT)_tcstol(str.GetString(), NULL, 0);
}

template<>
VOID StringToValue<USHORT>(USHORT & Value, CONST CString & str)
{
	Value = (USHORT)_tcstoul(str.GetString(), NULL, 0);
}

template<>
VOID StringToValue<INT>(INT & Value, CONST CString & str)
{
	Value = _tcstol(str.GetString(), NULL, 0);
}

template<>
VOID StringToValue<UINT>(UINT & Value, CONST CString & str)
{
	Value = _tcstoul(str.GetString(), NULL, 0);
}

template<>
VOID ValueToString(CONST VectorBYTE & Value, CString & str)
{
	ValueToStringPrivate(Value/*.Buffer*//*public vector<byte>*/, str);
}

template<>
VOID StringToValue<CString>(CString & Value, CONST CString & str)
{
	Value = str;
}

template<>
VOID StringToValue<wstring>(wstring & Value, CONST CString & str)
{
	Value = str.GetString();
}


//template<>
static VOID StringToValuePrivate(vector<BYTE> & Value, CONST CString & str)
{
	if (str.IsEmpty())
	{
		return;
	}

	INT Len = Base64DecodeGetRequiredLength(str.GetLength()) + 16;

	CStringA strA(str);

	Value.resize(Len);

	Base64Decode(
		strA.GetString(),//LPCSTR szSrc,
		strA.GetLength(),//int nSrcLen,
		&Value[0],//BYTE* pbDest,
		&Len//int* pnDestLen
		);

	Value.resize(Len);
}


template<>
VOID StringToValue<GUID>(GUID & Value, CONST CString & str)
{
	CLSIDFromString(
		str.GetString(),//_In_   LPCOLESTR lpsz,
		&Value//_Out_  LPCLSID pclsid
		);
}


template<>
VOID ValueToString(CONST SYSTEMTIME & Value, CString & str)
{
	str.Format(TEXT("%04d-%02d-%02d %02d:%02d:%02d-%03d %02d"),
		Value.wYear,
		Value.wMonth,
		Value.wDay,
		Value.wHour,
		Value.wMinute,
		Value.wSecond,
		Value.wMilliseconds,
		Value.wDayOfWeek
		);
}


namespace
{
	static BOOL MyIsDecNumber(TCHAR val)
	{
		return val >= TEXT('0') && val <= TEXT('9');
	}

	static LPCTSTR FindNumber(LPCTSTR p)
	{
		while (*p)
		{
			if (MyIsDecNumber(*p))
			{
				break;
			}
			++ p;
		}
		return p;
	}
}

template<>
VOID StringToValue(SYSTEMTIME & Value, CONST CString & str)
{
	int valCase = 0;

	LPCTSTR p = str.GetString();
	BOOL loop = TRUE;
	while (loop)
	{
		p = FindNumber(p);
		switch(valCase)
		{
		case 0:
			Value.wYear = _tcstol(p, (WCHAR**)(&p), 10);
			++ valCase;
			break;
		case 1:
			Value.wMonth = _tcstol(p, (WCHAR**)(&p), 10);
			++ valCase;
			break;
		case 2:
			Value.wDay = _tcstol(p, (WCHAR**)(&p), 10);
			++ valCase;
			break;
		case 3:
			Value.wHour = _tcstol(p, (WCHAR**)(&p), 10);
			++ valCase;
			break;
		case 4:
			Value.wMinute = _tcstol(p, (WCHAR**)(&p), 10);
			++ valCase;
			break;
		case 5:
			Value.wSecond = _tcstol(p, (WCHAR**)(&p), 10);
			++valCase;
			break;
		case 6:
			Value.wMilliseconds = _tcstol(p, (WCHAR**)(&p), 10);
			++valCase;
			break;
		case 7:
			Value.wDayOfWeek = _tcstol(p, (WCHAR**)(&p), 10);
			++valCase;
			break;
		default:
			loop = FALSE;
			break;
		}
	}
	
#if 0
	LPCTSTR p = str.GetString();
	LPCTSTR end = p + str.GetLength();

	Value.wYear = _tcstol(p, (WCHAR**)(&p), 10);

	++p; if (p >= end) { return; }
	Value.wMonth = _tcstol(p, (WCHAR**)(&p), 10);

	++p; if (p >= end) { return; }
	Value.wDay = _tcstol(p, (WCHAR**)(&p), 10);

	++p; if (p >= end) { return; }
	Value.wHour = _tcstol(p, (WCHAR**)(&p), 10);

	++p; if (p >= end) { return; }
	Value.wMinute = _tcstol(p, (WCHAR**)(&p), 10);

	++p; if (p >= end) { return; }
	Value.wSecond = _tcstol(p, (WCHAR**)(&p), 10);

	++p; if (p >= end) { return; }
	Value.wMilliseconds = _tcstol(p, (WCHAR**)(&p), 10);

	++p; if (p >= end) { return; }
	Value.wDayOfWeek = _tcstol(p, (WCHAR**)(&p), 10);
#endif
}

template<>
VOID ValueToString(CONST DOUBLE & Value, CString & str)
{
	ULONGLONG buffer = 0;
	CopyMemory(&buffer, &Value, std::min<LONG>(sizeof(ULONGLONG), sizeof(DOUBLE)));
	str.Format(TEXT("%I64u %lf"), buffer, Value);
}

template<>
VOID StringToValue(DOUBLE & Value, CONST CString & str)
{
	ULONGLONG buffer = _tcstoui64(str.GetString(), nullptr, 0);
	CopyMemory(&Value, &buffer, std::min<LONG>(sizeof(ULONGLONG), sizeof(DOUBLE)));
}

template<>
VOID ValueToString(CONST FLOAT & Value, CString & str)
{
	ULONGLONG buffer = 0;
	CopyMemory(&buffer, &Value, std::min<LONG>(sizeof(ULONGLONG), sizeof(FLOAT)));
	str.Format(TEXT("%I64u %f"), buffer, Value);
}

template<>
VOID StringToValue(FLOAT & Value, CONST CString & str)
{
	ULONGLONG buffer = _tcstoui64(str.GetString(), nullptr, 0);
	CopyMemory(&Value, &buffer, std::min<LONG>(sizeof(ULONGLONG), sizeof(FLOAT)));
}

//////////////////////////////////////////////////////////////////////////
VOID BufferToVectorHelper(LPCVOID pBuffer, LONG SizeInBytes, vector<BYTE> & vecBin)
{
	if (pBuffer && SizeInBytes)
	{
		vecBin.resize(SizeInBytes);
		CopyMemory(&vecBin[0], pBuffer, SizeInBytes);
	}
}

VOID VectorToBufferHelper(LPVOID pBuffer, LONG SizeInBytes, CONST vector<BYTE> & vecBin)
{
	if (pBuffer && SizeInBytes && vecBin.size())
	{
		CopyMemory(pBuffer, &vecBin[0], std::min<size_t>(SizeInBytes, vecBin.size()));
	}
}



template<>
VOID ValueToString(CONST string & Value, CString & str)
{
	CStringA strA = Value.c_str();
	Utf8To16String(strA, str);
}

template<>
VOID ValueToString(CONST CStringA & Value, CString & str)
{
	Utf8To16String(Value, str);
}

template<>
VOID StringToValue(string & Value, CONST CString & str)
{
	CStringA strA;
	Utf16To8String(str, strA);
	Value = strA.GetString();
}

template<>
VOID StringToValue(CStringA & Value, CONST CString & str)
{
	Utf16To8String(str, Value);
}


//<sonmi01>2016-4-25 ###???
template<>
VOID StringToValue(FILETIME & Value, CONST CString & str)
{
	SYSTEMTIME st = { 0 };
	StringToValue<SYSTEMTIME>(st, str);
	SystemTimeToFileTime(&st, &Value);
}

template<>
VOID ValueToString(CONST FILETIME & Value, CString & str)
{
	SYSTEMTIME st = { 0 };
	FileTimeToSystemTime(&Value, &st);
	ValueToString<SYSTEMTIME>(st, str);
}