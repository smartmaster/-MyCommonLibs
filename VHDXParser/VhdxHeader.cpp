#include "stdafx.h"
#include "VhdxHeader.h"

#include "VhdxPrintField.h"


VOID VhdxHeader::Print()
{
	//D_INFO(0, TEXT("VhdxHeader+++++++++++++++++START"));
	PRINT_FIELD(m_Signature);
	PRINT_FIELD(m_Checksum);
	PRINT_FIELD(m_SequenceNumber);
	PRINT_FIELD(m_FileWrite);
	PRINT_FIELD(m_DataWrite);
	PRINT_FIELD(m_Log);
	PRINT_FIELD(m_LogVersion);
	PRINT_FIELD(m_Version);
	PRINT_FIELD(m_LogLength);
	PRINT_FIELD(m_LogOffset);
	PRINT_FIELD(m_Reserved[4016]);
	PRINT_FIELD(m_Reserved[_countof(m_Reserved) - 1]);
	//D_INFO(0, TEXT("VhdxHeader+++++++++++++++++END\r\n\r\n\r\n"));
}

UINT32 VhdxHeader::SignatureSpecConst()
{
	return 0x64616568;
}

INT VhdxHeader::SizeSpecConst()
{
	return 4 * 1024;
}

INT VhdxHeader::Headser1OffsetSpecConst()
{
	return 64 * 1024;
}

INT VhdxHeader::Headser2OffsetSpecConst()
{
	return 128 * 1024;
}

VOID VhdxHeader::CalcChecksum()
{
	m_Checksum = 0;
	m_Checksum = crc32c(0xffffffff, (LPCBYTE)(this), SizeSpecConst());
}

BOOL VhdxHeader::IsValid()
{
	if (SignatureSpecConst() != m_Signature)
	{
		return FALSE;
	}

	UINT Checksum = m_Checksum; //save original and set to ZERO
	m_Checksum = 0;
	BOOL bValidChecksum = (Checksum == crc32c(0xffffffff, (LPCBYTE)(this), SizeSpecConst()));
	m_Checksum = Checksum;
	return bValidChecksum;
}
