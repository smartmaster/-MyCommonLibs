#pragma once

class CCrcCalc
{
private:
	ULONG * m_pCrc32Table; // Lookup table array
	CHeapPtr<ULONG> m_Crc32TableBuffer;

	static CONST INT BUFFER_COUNT = 256;

	// Reflects CRC bits in the lookup table
	ULONG Reflect(ULONG reflect, INT bitCount);

public:
	CCrcCalc();
	~CCrcCalc();

	// Builds Lookup table array
	VOID InitCrc32Table();

	// Creates a CRC from a string buffer
	ULONG GetCrc(CONST BYTE * buffer, INT len);
};