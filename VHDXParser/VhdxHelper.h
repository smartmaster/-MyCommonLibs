#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "..\PublicHeader\PublicDTraceLib.h"

//////////////////////////////////////////////////////////////////////////
LPCTSTR GuidToStringHelper(CONST GUID & guid, LPTSTR pBuffer, INT Len);

//////////////////////////////////////////////////////////////////////////
VOID GetStringInBuffer(LPCBYTE pBufferStart, UINT Offset, UINT LengthInByte, CString & str);

//////////////////////////////////////////////////////////////////////////
//TY should be struct data which can be safely zeroed.
//bad examples - STL container, class with virtual functions ...
template<typename TY>
VOID ZeroValue(TY & val) 
{
	ZeroMemory(&val, sizeof(TY));
}


template<typename TY>
VOID CopyValue(TY & val, CONST TY & const_val)
{
	CopyMemory(&val, &const_val, sizeof(TY));
}


//////////////////////////////////////////////////////////////////////////
template <typename T1, typename T2>
T1 ArithmeticAlignUp(T1 len, T2 boundary)
{
	return (len + boundary - 1) / boundary * boundary;
}

template <typename T1, typename T2>
T1 ArithmeticAlignDown(T1 len, T2 boundary)
{
	return len / boundary * boundary;
}

//<sonmi01>2014-3-2 ###???
//s - start index, e - end index
template<typename T>
BOOL LineIntersect(
	T s1, T e1, 
	T s2, T e2, 
	T & sInner, T & eInner
	)
{
	sInner = max(s1, s2);
	eInner = min(e1, e2);
	return sInner < eInner;
}

template<typename T>
BOOL LineConnect(
	T s1, T e1,
	T s2, T e2,
	T & sInner, T & eInner,
	T & sOuter, T & eOuter
	)
{
	sOuter = min(s1, s2);
	eOuter = max(e1, e2);

	sInner = max(s1, s2);
	eInner = min(e1, e2);
	return sInner <= eInner;
}


//<sonmi01>2014-4-21 ###???
HRESULT SetFileSizeFast(HANDLE hFile, LONGLONG FileSize);