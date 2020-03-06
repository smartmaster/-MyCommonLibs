#include "stdafx.h"
#include "VHDXRegionTableHeader.h"

#include "VhdxPrintField.h"


VOID VHDXRegionTableHeader::Print()
{
	//D_INFO(0, TEXT("VHDXRegionTableHeader+++++++++++++++++START"));
	PRINT_FIELD(m_Signature);
	PRINT_FIELD(m_Checksum);
	PRINT_FIELD(m_EntryCount);
	PRINT_FIELD(m_Reserved);
	//D_INFO(0, TEXT("VHDXRegionTableHeader+++++++++++++++++END\r\n\r\n\r\n"));
}

UINT32 VHDXRegionTableHeader::SignatureSpecConst()
{
	return 0x69676572;
}

UINT32 VHDXRegionTableHeader::MaxEntryCountSpecConst()
{
	return 2047;
}

INT VHDXRegionTableHeader::OffsetSpecConst()
{
	return 192 * 1024;
}

INT VHDXRegionTableHeader::SizeSpecConst()
{
	return 64 * 1024;
}

VOID VHDXRegionTableHeader::CalcChecksum()
{
	m_Checksum = 0;
	m_Checksum = crc32c(0xffffffff, (LPCBYTE)(this), SizeSpecConst());
}

BOOL VHDXRegionTableHeader::IsValid()
{
	if (SignatureSpecConst() != m_Signature)
	{
		return FALSE;
	}

	if (m_EntryCount > MaxEntryCountSpecConst())
	{
		return FALSE;
	}

	UINT Checksum = m_Checksum; //save original and set to ZERO
	m_Checksum = 0;
	BOOL bValidChecksum = (Checksum == crc32c(0xffffffff, (LPCBYTE)(this), SizeSpecConst()));
	m_Checksum = Checksum;
	return bValidChecksum;
}
