#pragma once

#include "stdafx.h"

//********************************************************************
struct CSimpleBitArray 
{
	//********************************************************************
	PVOID m_pBitArray; //outside object
	LONGLONG m_BitLength;

public:	//********************************************************************
	CSimpleBitArray(PVOID pBitArray, LONGLONG llBitLength);
	~CSimpleBitArray();

	INT Get(LONGLONG llBitIndex);
	BOOL Set(LONGLONG llBitIndex, INT nValue);
	BOOL Flip();

	LONGLONG FindFirst(INT nVal, LONGLONG llStart);
	LONGLONG GetBitLength();

	HRESULT ClearRange(LONGLONG llStartOffset, LONGLONG llEndOffset);
	HRESULT SetRange(LONGLONG llStartOffset, LONGLONG llEndOffset);
	LONGLONG CountBit1();

	//********************************************************************
	static LONGLONG CountBit1InByte(BYTE byteValue);
	static LONGLONG CountBit1InArray(LPBYTE pByte, LONGLONG llLength);
};