#include "stdafx.h"
#include "CrcCalc.h"


ULONG CCrcCalc::Reflect( ULONG reflect, INT bitCount )
{
	// Used only by Init_CRC32_Table()

	ULONG value(0);

	// Swap bit 0 for bit 7
	// bit 1 for bit 6, etc.
	for(INT iii = 1; iii < (bitCount + 1); iii++)
	{
		if(reflect & 1)
			value |= 1 << (bitCount - iii);
		reflect >>= 1;
	}
	return value;
}

VOID CCrcCalc::InitCrc32Table()
{
	// This is the official polynomial used by CRC-32 
	// in PKZip, WinZip and Ethernet. 
	ULONG ulPolynomial = 0x04c11db7;

	// 256 values representing ASCII character codes.
	for(INT iii = 0; iii <= 0xFF; iii++)
	{
		m_pCrc32Table[iii]=Reflect(iii, 8) << 24;
		for (INT jjj = 0; jjj < 8; jjj++)
			m_pCrc32Table[iii] = (m_pCrc32Table[iii] << 1) ^ (m_pCrc32Table[iii] & (1 << 31) ? ulPolynomial : 0);
		m_pCrc32Table[iii] = Reflect(m_pCrc32Table[iii], 32);
	}
}

ULONG CCrcCalc::GetCrc( CONST BYTE * buffer, INT len )
{
	// Be sure to use unsigned variables,
	// because negative values introduce high bits
	// where zero bits are required.

	// Start out with all bits set high.
	ULONG  ulCRC(0xffffffff);

	// Perform the algorithm on each character
	// in the string, using the lookup table values.
	while(len--)
	{
		ulCRC = (ulCRC >> 8) ^ m_pCrc32Table[(ulCRC & 0xFF) ^ *buffer++];
	}
	// Exclusive OR the result with the beginning value.
	return (ulCRC ^ 0xffffffff);
}

CCrcCalc::CCrcCalc()
{
	m_Crc32TableBuffer.Allocate(BUFFER_COUNT);
	m_pCrc32Table = (ULONG *)m_Crc32TableBuffer;
	ZeroMemory(m_pCrc32Table, BUFFER_COUNT*sizeof(ULONG));
}

CCrcCalc::~CCrcCalc()
{
	m_pCrc32Table = NULL;
	m_Crc32TableBuffer.Free();
}