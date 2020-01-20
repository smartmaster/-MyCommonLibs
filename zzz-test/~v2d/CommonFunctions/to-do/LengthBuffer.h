#pragma once

#include "stdafx.h"

#include <vector>
using std::vector;

class CLengthBuffer
{

private:
	BYTE * m_pGrand;
	BYTE * m_pData;
	LONGLONG m_GrandLength;
	LONGLONG m_DataLength;
	vector<BYTE> m_Buffer;

public:
	CLengthBuffer();

	VOID Allocate(LONGLONG DataLengh);

	LONGLONG GetGrandLength() CONST;
	LONGLONG GetDataLength() CONST;
	BYTE * GetGrandBuffer() CONST;
	BYTE * GetDataBuffer() CONST;

public:
	static CONST ULONG HEADER_LENGTH = sizeof(LONGLONG) + sizeof(LONGLONG);
};



struct TestLengthBuffer
{
	static VOID Main()
	{
		CLengthBuffer LenBuff;

		CONST ULONG BLOCK_SIZE = 1024*4;

		for (INT ii=0; ii<3; ++ii)
		{
			LenBuff.Allocate(ii*BLOCK_SIZE);
			LONGLONG l1 = LenBuff.GetGrandLength();
			LONGLONG l2 = LenBuff.GetDataLength();
			BYTE * p1 = LenBuff.GetGrandBuffer();
			BYTE * p2 = LenBuff.GetDataBuffer();
		}
	}
};