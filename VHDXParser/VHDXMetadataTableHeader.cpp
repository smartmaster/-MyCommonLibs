#include "stdafx.h"
#include "VHDXMetadataTableHeader.h"




UINT64 VHDXMetadataTableHeader::SignatuireSpecConst()
{
	return 0x617461646174656dULL;
}

UINT16 VHDXMetadataTableHeader::MaxEntryCountSpecConst()
{
	return 2047;
}

INT VHDXMetadataTableHeader::SizeSpecConst() //include header entries header and entries data
{
	return 1024 * 1024;
}

BOOL VHDXMetadataTableHeader::IsValid()
{
	if (m_Signature != SignatuireSpecConst())
	{
		return FALSE;
	}

	if (m_EntryCount > MaxEntryCountSpecConst())
	{
		return FALSE;
	}

	return TRUE;
}

VOID VHDXMetadataTableHeader::Print()
{
	//D_INFO(0, TEXT("VHDXMetadataTableHeader+++++++++++++++++START"));
	PRINT_FIELD(m_Signature);
	PRINT_FIELD(m_Reserved);
	PRINT_FIELD(m_EntryCount);
	PRINT_FIELD(m_Reserved2[0]);
	PRINT_FIELD(m_Reserved2[_countof(m_Reserved2) - 1]);
	//D_INFO(0, TEXT("VHDXMetadataTableHeader+++++++++++++++++END\r\n\r\n\r\n"));
}
