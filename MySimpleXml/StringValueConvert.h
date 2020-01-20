#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>


#include <string>
#include <vector>
#include <algorithm>
using namespace std;

////////////////////////////////////////////////////////////////////////
template<typename T>
VOID ValueToString(CONST T & Value, CString & str)
{
	//str.Format(TEXT("%d"), (INT)Value);
	return;
}

template<typename T>
VOID StringToValue(T & Value, CONST CString & str)
{
	//Value = _tcstol(str.GetString(), NULL, 0);
	return;
}

//////////////////////////////////////////////////////////////////////////
#define VALUE_STRING_CONVERTER_DECLARE(typeParam) \
template<> \
VOID ValueToString<typeParam>(CONST typeParam & Value, CString & str); \
template<> \
VOID StringToValue<typeParam>(typeParam & Value, CONST CString & str);

struct VectorBYTE : public vector < BYTE >
{
	//vector < BYTE > Buffer;
private: 
	DWORD m_notUsed{'FFUB'};
};

VALUE_STRING_CONVERTER_DECLARE(bool)
VALUE_STRING_CONVERTER_DECLARE(CString)
VALUE_STRING_CONVERTER_DECLARE(wstring)
VALUE_STRING_CONVERTER_DECLARE(LONGLONG)
VALUE_STRING_CONVERTER_DECLARE(ULONGLONG)
VALUE_STRING_CONVERTER_DECLARE(LONG)
VALUE_STRING_CONVERTER_DECLARE(ULONG)
VALUE_STRING_CONVERTER_DECLARE(CHAR)
VALUE_STRING_CONVERTER_DECLARE(UCHAR)
VALUE_STRING_CONVERTER_DECLARE(SHORT)
VALUE_STRING_CONVERTER_DECLARE(USHORT)
VALUE_STRING_CONVERTER_DECLARE(INT)
VALUE_STRING_CONVERTER_DECLARE(UINT)
VALUE_STRING_CONVERTER_DECLARE(VectorBYTE)
VALUE_STRING_CONVERTER_DECLARE(GUID)
VALUE_STRING_CONVERTER_DECLARE(string)
VALUE_STRING_CONVERTER_DECLARE(CStringA)
VALUE_STRING_CONVERTER_DECLARE(SYSTEMTIME)
VALUE_STRING_CONVERTER_DECLARE(DOUBLE)
VALUE_STRING_CONVERTER_DECLARE(FLOAT)
VALUE_STRING_CONVERTER_DECLARE(FILETIME)

//////////////////////////////////////////////////////////////////////////
VOID BufferToVectorHelper(LPCVOID pBuffer, LONG SizeInBytes, vector<BYTE> & vecBin);
VOID VectorToBufferHelper(LPVOID pBuffer, LONG SizeInBytes, CONST vector<BYTE> & vecBin);


#if 1
template<typename T>
VOID ObjectToVector(CONST T & obj, vector<BYTE> & vecBin, LONG SizeInBytes = 0)
{
	if (0 == SizeInBytes)
	{
		SizeInBytes = sizeof(T);
	}
	BufferToVectorHelper(&obj, SizeInBytes, vecBin);
}

template<typename T>
VOID VectorToObject(T & obj, CONST vector<BYTE> & vecBin, LONG SizeInBytes = 0)
{
	if (0 == SizeInBytes)
	{
		SizeInBytes = sizeof(T);
	}
	VectorToBufferHelper(&obj, SizeInBytes, vecBin);
}


template<typename T>
T & ConvertVector(vector<BYTE> & vecBin)
{
	return *(reinterpret_cast<T*>(&vecBin[0]));
}

#endif

#if 0
//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<CString>(CONST CString & Value, CString & str);
template<>
VOID StringToValue<CString>(CString & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<wstring>(CONST wstring & Value, CString & str);
template<>
VOID StringToValue<wstring>(wstring & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<LONGLONG>(CONST LONGLONG & Value, CString & str);
template<>
VOID StringToValue<LONGLONG>(LONGLONG & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<ULONGLONG>(CONST ULONGLONG & Value, CString & str);
template<>
VOID StringToValue<ULONGLONG>(ULONGLONG & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<LONG>(CONST LONG & Value, CString & str);
template<>
VOID StringToValue<LONG>(LONG & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<ULONG>(CONST ULONG & Value, CString & str);
template<>
VOID StringToValue<ULONG>(ULONG & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<CHAR>(CONST CHAR & Value, CString & str);
template<>
VOID StringToValue<CHAR>(CHAR &Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<UCHAR>(CONST UCHAR & Value, CString & str);
template<>
VOID StringToValue<UCHAR>(UCHAR & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<SHORT>(CONST SHORT & Value, CString & str);
template<>
VOID StringToValue<SHORT>(SHORT & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<USHORT>(CONST USHORT & Value, CString & str);
template<>
VOID StringToValue<USHORT>(USHORT & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<INT>(CONST INT & Value, CString & str);
template<>
VOID StringToValue<INT>(INT & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<UINT>(CONST UINT & Value, CString & str);
template<>
VOID StringToValue<UINT>(UINT & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
struct VectorBYTE
{
	vector < BYTE > Buffer;
};
template<>
VOID ValueToString<VectorBYTE>(CONST VectorBYTE & Value, CString & str);
template<>
VOID StringToValue<VectorBYTE>(VectorBYTE & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<GUID>(CONST GUID & Value, CString & str);
template<>
VOID StringToValue<GUID>(GUID & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<string>(CONST string & Value, CString & str);
template<>
VOID StringToValue<string>(string & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<CStringA>(CONST CStringA & Value, CString & str);
template<>
VOID StringToValue<CStringA>(CStringA & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<SYSTEMTIME>(CONST SYSTEMTIME & Value, CString & str);
template<>
VOID StringToValue<SYSTEMTIME>(SYSTEMTIME & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<DOUBLE>(CONST DOUBLE & Value, CString & str);

template<>
VOID StringToValue<DOUBLE>(DOUBLE & Value, CONST CString & str);

//////////////////////////////////////////////////////////////////////////
template<>
VOID ValueToString<FLOAT>(CONST FLOAT & Value, CString & str);

template<>
VOID StringToValue<FLOAT>(FLOAT & Value, CONST CString & str);
#endif