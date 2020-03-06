#include "stdafx.h"
#include "MergeBuffer.h"

#include "..\PublicHeader\PublicSimpleBitArray.h"



VOID MergeBuffer( OUT BYTE * ParentBuffer, IN CONST BYTE * ChildBuffer, IN BYTE * BitmapBuffer, IN LONGLONG BitCount, IN LONG BlockSizePerBit )
{
	CSimpleBitArray bitArray(BitmapBuffer, BitCount);

	LONGLONG Start = 0;
	LONGLONG End = 0;

	for (;;)
	{
		Start = bitArray.FindFirst(1, Start);
		if (Start >= bitArray.GetBitLength())
		{
			break;
		}
		End = bitArray.FindFirst(0, Start);

		LONGLONG offset = Start * BlockSizePerBit;
		LONGLONG len = (End - Start) * BlockSizePerBit;

		CopyMemory(ParentBuffer + offset, ChildBuffer + offset, len);

		Start = End;
	}
}

VOID OrBitmapBuffer(OUT BYTE * ParentBuffer, IN CONST BYTE * ChildBuffer, LONG BufferLen, BOOL & bContainOne, BOOL & bAllOne) //<sonmi01>2013-10-29 ###???
{
	bContainOne = FALSE;
	bAllOne = TRUE;

	for (LONG ii = 0; ii < BufferLen; ++ ii)
	{
		ParentBuffer[ii] |= ChildBuffer[ii];
		
		if (ParentBuffer[ii])
		{
			bContainOne = TRUE;
		}

		if (0xff != ParentBuffer[ii])
		{
			bAllOne = FALSE;
		}
	}
	//return bContainOne;
}

VOID DiffBitmapBuffer(OUT BYTE * ParentBuffer, IN CONST BYTE * ChildBuffer, LONG BufferLen, BOOL & bContainOne, BOOL & bAllOne) //<sonmi01>2013-10-29 ###???
{
	bContainOne = FALSE;
	bAllOne = TRUE;

	for (LONG ii = 0; ii < BufferLen; ++ii)
	{
		ParentBuffer[ii] |= ChildBuffer[ii];
	}

	for (LONG ii = 0; ii < BufferLen; ++ii)
	{
		ParentBuffer[ii] ^= ChildBuffer[ii];

		if (ParentBuffer[ii])
		{
			bContainOne = TRUE;
		}

		if (0xff != ParentBuffer[ii])
		{
			bAllOne = FALSE;
		}
	}
	//return bContainOne;
}


namespace {
	static LONGLONG FF_BUFFER[512 / sizeof(LONGLONG)];
	class c_fill_one
	{
	public:
		c_fill_one()
		{
			memset(FF_BUFFER, 0xff, sizeof(FF_BUFFER));
		}
	};
	static c_fill_one fill_one;
}

BOOL IsAllOne(CONST BYTE * Buffer, LONG BufferLen) //<sonmi01>2013-10-29 ###???
{
	LONG CurrentCount;
	BOOL bRet = TRUE;

	while (BufferLen)
	{
		if (BufferLen > sizeof(FF_BUFFER))
		{
			CurrentCount = sizeof(FF_BUFFER);
		}
		else
		{
			CurrentCount = BufferLen;
		}

		if (memcmp(Buffer, FF_BUFFER, CurrentCount))
		{
			bRet = FALSE;
			break;
		}
		
		Buffer += CurrentCount;
		BufferLen -= CurrentCount;
	}

	return bRet;
}


BOOL IsAllZero( CONST BYTE * Buffer, LONG BufferLen )
{
	static CONST LONGLONG ZERO_BUFFER[512 / sizeof(LONGLONG)] = {0};

	LONG CurrentCount;
	BOOL bRet = TRUE;

	while (BufferLen)
	{
		if (BufferLen > sizeof(ZERO_BUFFER))
		{
			CurrentCount = sizeof(ZERO_BUFFER);
		}
		else
		{
			CurrentCount = BufferLen;
		}

		if (memcmp(Buffer, ZERO_BUFFER, CurrentCount))
		{
			bRet = FALSE;
			break;
		}


		Buffer += CurrentCount;
		BufferLen -= CurrentCount;
	}

	return bRet;
}

BOOL ContainsOne( CONST BYTE * Buffer, LONG BufferLen )
{
	return !IsAllZero(Buffer, BufferLen);
}
