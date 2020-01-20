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
	static CONST BYTE BIT_ALL_ONE = 0xff;
	static CONST BYTE BIT_ALL_ZERO = 0x0;

public:	//////////////////////////////////////////////////////////////////////////
	CSimpleBitArray(PVOID pBitArray, LONGLONG BitLength);
	~CSimpleBitArray();

	LONGLONG GetBitLength() CONST;

	INT Get(LONGLONG BitIndex) CONST;
	BOOL Set(LONGLONG BitIndex, INT Value);
	BOOL Flip();

#if 1
	LONGLONG FindFirstOld(INT Val, LONGLONG Start) CONST;
#endif
	LONGLONG FindFirst(INT Val, LONGLONG Start) CONST;

#if 1
	HRESULT ClearRange(LONGLONG StartOffset, LONGLONG EndOffset);
	HRESULT SetRange(LONGLONG StartOffset, LONGLONG EndOffset);
#endif
	HRESULT AssignRange(INT Val, LONGLONG StartOffset, LONGLONG EndOffset);

	//startIndex point to 1
	//endIndex point to 0, one beyond the last bit 1
	//startIndex == endIndex, stop
	VOID FindBitOneRange(LONGLONG start, LONGLONG & startIndex, LONGLONG & endIndex);

	LONGLONG CountBitOne() CONST;

public:
	template<typename T>
	static LONGLONG CountBitOne(T Value)
	{
		LONGLONG llCount = 0;
		if (0 == Value)
		{
			llCount = 0;
		}
		else if (0 == (T)(Value + 1))
		{
			llCount = sizeof(T) * BIT_COUNT_PER_BYTE;
		}
		else
		{
			while (Value)
			{
				++llCount;
				Value &= (Value - 1);
			}
		}

		return llCount;
	}

	//static LONGLONG CountBitOneInByte(BYTE byteValue);
	static LONGLONG CountBitOneInByteArray(LPCBYTE pByte, LONGLONG Length);
	static LONGLONG CountBitOneInArray(LPCBYTE pByte, LONGLONG llLengthBytes);
};