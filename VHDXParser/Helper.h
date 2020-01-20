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