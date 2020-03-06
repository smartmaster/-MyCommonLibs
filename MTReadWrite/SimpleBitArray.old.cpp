#include "stdafx.h"

#if 0


#include "SimpleBitArray.h"

CSimpleBitArray::CSimpleBitArray( PVOID pBitArray, LONGLONG BitLength ) :
	m_pBitArray(pBitArray), 
	m_BitLength(BitLength)
{

}

CSimpleBitArray::~CSimpleBitArray()
{
	m_pBitArray = NULL;
	m_BitLength = 0;
}

INT CSimpleBitArray::Get( LONGLONG BitIndex ) CONST
{
	INT nRet = -1;
	if (m_pBitArray && BitIndex >= 0 && BitIndex < m_BitLength)
	{
		LPCBYTE pByte = (LPCBYTE)m_pBitArray;
		LONGLONG llByteIndex = BitIndex / BIT_COUNT_PER_BYTE;
		LONG lBitIndexInByte = BitIndex % BIT_COUNT_PER_BYTE;
		BYTE byteValue = (1 << lBitIndexInByte);

		nRet = (pByte[llByteIndex] & byteValue);
		if (nRet)
		{
			nRet = 1;
		}
	}
	return nRet;
}

BOOL CSimpleBitArray::Set( LONGLONG llBitIndex, INT nValue )
{
	BOOL bRet = FALSE;
	if (m_pBitArray && llBitIndex >= 0 && llBitIndex < m_BitLength)
	{
		LPBYTE pByte = (LPBYTE)m_pBitArray;
		LONGLONG llByteIndex = llBitIndex / BIT_COUNT_PER_BYTE ;
		LONG lBitIndexInByte = llBitIndex % BIT_COUNT_PER_BYTE;
		BYTE byteValue = (1 << lBitIndexInByte);

		if (nValue)
		{
			pByte[llByteIndex] |= byteValue;
		}
		else
		{
			pByte[llByteIndex] &= (~byteValue);
		}

		bRet = TRUE;
	}
	return bRet;
}

BOOL CSimpleBitArray::Flip()
{
	BOOL bRet = FALSE;
	if (m_pBitArray && m_BitLength)
	{
		//////////////////////////////////////////////////////////////////////////
		LPBYTE pByte = (LPBYTE)m_pBitArray;
		LONGLONG llBitLength = AtlAlignUp(m_BitLength, BIT_COUNT_PER_BYTE);
		LONGLONG llByteLength = llBitLength / BIT_COUNT_PER_BYTE;
		for (LONGLONG ll = 0; ll < llByteLength; ++ ll)
		{
			pByte[ll] = ~(pByte[ll]);
		}

		//////////////////////////////////////////////////////////////////////////
		CSimpleBitArray bitArrTmp(m_pBitArray, llBitLength);
		bitArrTmp.AssignRange(0, m_BitLength, llBitLength);

		//////////////////////////////////////////////////////////////////////////
		bRet = TRUE;
	}
	return bRet;
}

LONGLONG CSimpleBitArray::CountBitOneInByte( BYTE byteValue )
{
	LONGLONG llCount = 0;
	if (BIT_ALL_ZERO == byteValue)
	{
		llCount = 0;
	}
	else if (BIT_ALL_ONE == byteValue)
	{
		llCount = BIT_COUNT_PER_BYTE;
	}
	else
	{
		while (byteValue)
		{
			++ llCount;
			byteValue &= (byteValue - 1);
		}
	}

	return llCount;
}

LONGLONG CSimpleBitArray::CountBitOneInArray( LPCBYTE pByte, LONGLONG llLength )
{
	LONGLONG llCount = 0;

	do 
	{
		if (NULL == pByte || 0 == llLength)
		{
			break;
		}

		for (LONGLONG ll = 0; ll < llLength; ++ ll)
		{
			llCount += CountBitOneInByte(pByte[ll]);
		}

	} while (FALSE);

	return llCount;
}

#if 1
LONGLONG CSimpleBitArray::FindFirstOld( INT nVal, LONGLONG llStart )  CONST
{
	LONGLONG llIndex = llStart;
	for (llIndex = llStart; llIndex < m_BitLength; ++ llIndex)
	{
		if (Get(llIndex) == nVal)
		{
			break;
		}
	}
	if (llIndex > m_BitLength)
	{
		llIndex = m_BitLength;
	}
	return llIndex;
}
#endif

LONGLONG CSimpleBitArray::FindFirst(INT Val, LONGLONG Start) CONST
{
	//////////////////////////////////////////////////////////////////////////
	if (Start >= m_BitLength)
	{
		return m_BitLength;
	}

	Val = Val? 1 : 0;
	//////////////////////////////////////////////////////////////////////////
	//if ((m_BitLength - Start) <= (BIT_COUNT_PER_BYTE + BIT_COUNT_PER_BYTE))
	if ((m_BitLength - Start) <= BIT_COUNT_PER_BYTE)
	{
		for (LONGLONG ii = Start; ii < m_BitLength; ++ ii)
		{
			if (Val == Get(ii))
			{
				return ii;
			}
		}
		return m_BitLength;
	}

	//////////////////////////////////////////////////////////////////////////
	LONGLONG BeginByteBoudary = AtlAlignUp(Start, BIT_COUNT_PER_BYTE);
	LONGLONG EndByteBoudary = AtlAlignDown(m_BitLength, BIT_COUNT_PER_BYTE);

	//////////////////////////////////////////////////////////////////////////
	for (LONGLONG ii = Start; ii < BeginByteBoudary; ++ ii)
	{
		if (Val == Get(ii))
		{
			return ii;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CONST BYTE OppsiteByte = (Val? BIT_ALL_ZERO : BIT_ALL_ONE);
	LPCBYTE pByteData = (LPCBYTE)m_pBitArray;
	for (LONGLONG ii = BeginByteBoudary / BIT_COUNT_PER_BYTE; ii < EndByteBoudary / BIT_COUNT_PER_BYTE; ++ ii )
	{
		if (OppsiteByte != pByteData[ii])
		{
			for (LONGLONG jj = ii * BIT_COUNT_PER_BYTE; jj < ii * BIT_COUNT_PER_BYTE + BIT_COUNT_PER_BYTE; ++ jj)
			{
				if (Val == Get(jj))
				{
					return jj;
				}
			}
			return m_BitLength; //should never go here
		}
	}

	//////////////////////////////////////////////////////////////////////////
	for (LONGLONG ii = EndByteBoudary; ii < m_BitLength; ++ ii)
	{
		if (Val == Get(ii))
		{
			return ii;
		}
	}

	return m_BitLength;
}


LONGLONG CSimpleBitArray::GetBitLength() CONST
{
	return m_BitLength;
}

#if 1
HRESULT CSimpleBitArray::ClearRange( LONGLONG StartOffset, LONGLONG EndOffset ) //EndOffset - one beyond the last bit
{
	return AssignRange(0, StartOffset, EndOffset);
}

HRESULT CSimpleBitArray::SetRange( LONGLONG llStartOffset, LONGLONG llEndOffset ) //EndOffset - one beyond the last bit
{
	return AssignRange(1, llStartOffset, llEndOffset);
}
#endif

LONGLONG CSimpleBitArray::CountBitOne() CONST
{
	LONGLONG llOffset = AtlAlignDown(m_BitLength, BIT_COUNT_PER_BYTE);
	LONGLONG llCount = CountBitOneInArray((LPBYTE)m_pBitArray, llOffset / BIT_COUNT_PER_BYTE);
	for (LONGLONG ii = llOffset; ii < m_BitLength; ++ ii)
	{
		llCount += Get(ii);
	}
	return llCount;
}

HRESULT CSimpleBitArray::AssignRange( INT Val, LONGLONG llStartOffset, LONGLONG llEndOffset )
{
	HRESULT hr = S_OK;

	Val = (Val? 1 : 0);
	//******************************************
	if (llStartOffset > llEndOffset)
	{
		hr = E_INVALIDARG;
		return hr;
	}

	if (llStartOffset < 0)
	{
		llStartOffset = 0;
	}
	if (llEndOffset > m_BitLength)
	{
		llEndOffset = m_BitLength;
	}

	//////////////////////////////////////////////////////////////////////////
	//if ((llEndOffset - llStartOffset) <= (BIT_COUNT_PER_BYTE + BIT_COUNT_PER_BYTE) )
	if ((llEndOffset - llStartOffset) <= BIT_COUNT_PER_BYTE )
	{
		for (LONGLONG ii=llStartOffset; ii<llEndOffset; ++ii)
		{
			Set(ii, Val);
		}
		return hr;
	}


	//////////////////////////////////////////////////////////////////////////
	LONGLONG llOffset1 = AtlAlignUp(llStartOffset, BIT_COUNT_PER_BYTE);
	LONGLONG llOffset2 = AtlAlignDown(llEndOffset, BIT_COUNT_PER_BYTE);

	//////////////////////////////////////////////////////////////////////////
	for (LONGLONG ii=llStartOffset; ii<llOffset1; ++ii)
	{
		Set(ii, Val);
	}

	//////////////////////////////////////////////////////////////////////////
	LPBYTE pByte = (LPBYTE)m_pBitArray;
	BYTE AllSet = (Val? BIT_ALL_ONE : BIT_ALL_ZERO);
	for (LONGLONG ii = llOffset1 / BIT_COUNT_PER_BYTE; ii < llOffset2 / BIT_COUNT_PER_BYTE; ++ ii)
	{
		pByte[ii] = AllSet;
	}

	//////////////////////////////////////////////////////////////////////////
	for (LONGLONG ii=llOffset2; ii<llEndOffset; ++ii)
	{
		Set(ii, Val);
	}

	//////////////////////////////////////////////////////////////////////////


	return hr;
}

#endif