#pragma once

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>

//////////////////////////////////////////////////////////////////////////
struct CSimpleBitArray 
{
private:	//////////////////////////////////////////////////////////////////////////
	PVOID m_pBitArray; /***Incomming***/
	LONGLONG m_BitLength;

private:
	static CONST INT BIT_COUNT_PER_BYTE = 8;
	static CONST BYTE BYTE_MAX_VALUE = 0xff;

public:	//////////////////////////////////////////////////////////////////////////
	CSimpleBitArray(PVOID pBitArray, LONGLONG BitLength);
	~CSimpleBitArray();

	INT Get(LONGLONG BitIndex);
	BOOL Set(LONGLONG BitIndex, INT Value);
	BOOL Flip();

	LONGLONG FindFirst(INT Val, LONGLONG Start);
	LONGLONG GetBitLength();

	HRESULT ClearRange(LONGLONG StartOffset, LONGLONG EndOffset);
	HRESULT SetRange(LONGLONG StartOffset, LONGLONG EndOffset);
	LONGLONG CountBit1();

	//////////////////////////////////////////////////////////////////////////
	static LONGLONG CountBit1InByte(BYTE byteValue);
	static LONGLONG CountBit1InArray(LPBYTE pByte, LONGLONG Length);
};