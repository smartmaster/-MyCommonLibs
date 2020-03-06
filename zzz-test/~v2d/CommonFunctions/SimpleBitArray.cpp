#include "stdafx.h"
#include "SimpleBitArray.h"

CSimpleBitArray::CSimpleBitArray( PVOID pBitArray, LONGLONG llBitLength ) :
m_pBitArray(pBitArray), 
m_BitLength(llBitLength)
{

}

CSimpleBitArray::~CSimpleBitArray()
{
	m_pBitArray = NULL;
	m_BitLength = 0;
}

INT CSimpleBitArray::Get( LONGLONG llBitIndex )
{
	INT nRet = -1;
	if (m_pBitArray && llBitIndex>=0 && llBitIndex<m_BitLength)
	{
		LPBYTE pByte = (LPBYTE)m_pBitArray;
		LONGLONG llByteIndex = llBitIndex/8;
		LONG lBitIndexInByte = llBitIndex%8;
		BYTE byteValue = (1<<lBitIndexInByte);

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
	if (m_pBitArray && llBitIndex>=0 && llBitIndex<m_BitLength)
	{
		LPBYTE pByte = (LPBYTE)m_pBitArray;
		LONGLONG llByteIndex = llBitIndex/8;
		LONG lBitIndexInByte = llBitIndex%8;
		BYTE byteValue = (1<<lBitIndexInByte);

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
		LPBYTE pByte = (LPBYTE)m_pBitArray;
		LONGLONG llByteLength = (m_BitLength+7)/8;
		for (LONGLONG ll=0; ll<llByteLength; ++ll)
		{
			pByte[ll] = ~(pByte[ll]);
		}
		bRet = TRUE;
	}
	return bRet;
}

LONGLONG CSimpleBitArray::CountBit1InByte( BYTE byteValue )
{
	LONGLONG llCount = 0;
	if (0 == byteValue)
	{
		llCount = 0;
	}
	else if (0xff == byteValue)
	{
		llCount = 8;
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

LONGLONG CSimpleBitArray::CountBit1InArray( LPBYTE pByte, LONGLONG llLength )
{
	LONGLONG llCount = 0;

	do 
	{
		if (NULL==pByte || 0==llLength)
		{
			break;
		}

		for (LONGLONG ll=0; ll<llLength; ++ll)
		{
			llCount += CountBit1InByte(pByte[ll]);
		}

	} while (FALSE);

	return llCount;
}

LONGLONG CSimpleBitArray::FindFirst( INT nVal, LONGLONG llStart )
{
	LONGLONG llIndex = llStart;
	for (llIndex=llStart; llIndex<m_BitLength; ++llIndex)
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

LONGLONG CSimpleBitArray::GetBitLength()
{
	return m_BitLength;
}

HRESULT CSimpleBitArray::ClearRange( LONGLONG llStartOffset, LONGLONG llEndOffset )
{
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		if (llStartOffset > llEndOffset)
		{
			hr = E_INVALIDARG;
			break;
		}

		if (llStartOffset < 0)
		{
			llStartOffset = 0;
		}
		if (llEndOffset > m_BitLength)
		{
			llEndOffset = m_BitLength;
		}

		if (llEndOffset - llStartOffset <= 16)
		{
			for (LONGLONG ii=llStartOffset; ii<llEndOffset; ++ii)
			{
				Set(ii, 0);
			}
		}
		else
		{
			LONGLONG llOffset1 = AtlAlignUp(llStartOffset, 8);
			LONGLONG llOffset2 = AtlAlignDown(llEndOffset, 8);

			for (LONGLONG ii=llStartOffset; ii<llOffset1; ++ii)
			{
				Set(ii, 0);
			}

			if (llOffset2 > llOffset1)
			{
				for (LONGLONG ii=llOffset1/8; ii<llOffset2/8; ++ii)
				{
					((BYTE*)m_pBitArray)[ii] = 0;
				}
				//ZeroMemory((BYTE*)m_pBitArray + llOffset1/8, (llOffset2-llOffset1)/8);
			}

			for (LONGLONG ii=llOffset2; ii<llEndOffset; ++ii)
			{
				Set(ii, 0);
			}
		}

	} while (FALSE);

	return hr;
}

HRESULT CSimpleBitArray::SetRange( LONGLONG llStartOffset, LONGLONG llEndOffset )
{
	HRESULT hr = S_OK;

	do 
	{
		//******************************************
		if (llStartOffset > llEndOffset)
		{
			hr = E_INVALIDARG;
			break;
		}

		if (llStartOffset < 0)
		{
			llStartOffset = 0;
		}
		if (llEndOffset > m_BitLength)
		{
			llEndOffset = m_BitLength;
		}

		if (llEndOffset - llStartOffset <= 16)
		{
			for (LONGLONG ii=llStartOffset; ii<llEndOffset; ++ii)
			{
				Set(ii, 1);
			}
		}
		else
		{
			LONGLONG llOffset1 = AtlAlignUp(llStartOffset, 8);
			LONGLONG llOffset2 = AtlAlignDown(llEndOffset, 8);

			for (LONGLONG ii=llStartOffset; ii<llOffset1; ++ii)
			{
				Set(ii, 1);
			}

			if (llOffset2 > llOffset1)
			{
				for (LONGLONG ii=llOffset1/8; ii<llOffset2/8; ++ii)
				{
					((BYTE*)m_pBitArray)[ii] = 0xFF;
				}
			}

			for (LONGLONG ii=llOffset2; ii<llEndOffset; ++ii)
			{
				Set(ii, 1);
			}
		}

	} while (FALSE);

	return hr;
}

LONGLONG CSimpleBitArray::CountBit1()
{
	LONGLONG llOffset = AtlAlignDown(m_BitLength, 8);
	LONGLONG llCount = CountBit1InArray((LPBYTE)m_pBitArray, llOffset/8);
	for (LONGLONG ii=llOffset; ii<m_BitLength; ++ii)
	{
		llCount += Get(ii);
	}
	return llCount;
}