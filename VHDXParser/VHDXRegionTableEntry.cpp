#include "stdafx.h"
#include "VHDXRegionTableEntry.h"
#include "VhdxPrintField.h"



VOID VHDXRegionTableEntry::Print()
{
	//D_INFO(0, TEXT("VHDXRegionTableEntry+++++++++++++++++START"));
	PRINT_FIELD(m_Guid);
	PRINT_FIELD(m_FileOffset);
	PRINT_FIELD(m_Length);
	PRINT_FIELD((UINT32)m_Required);
	PRINT_FIELD((UINT32)m_Reserved);
	D_INFO(0, TEXT("Region=%s"), RegionGuidTostring(m_Guid));
	//D_INFO(0, TEXT("VHDXRegionTableEntry+++++++++++++++++END\r\n\r\n\r\n"));
}

CONST GUID & VHDXRegionTableEntry::BatGuidSpecConst()
{
	static CONST GUID guid = {0x2DC27766, 0xF623, 0x4200, {0x9D, 0x64, 0x11, 0x5E, 0x9B, 0xFD, 0x4A, 0x08}};
	return guid;
}

CONST GUID & VHDXRegionTableEntry::MetadataRegionGuidSpecConst()
{
	static CONST GUID guid = {0x8B7CA206, 0x4790, 0x4B9A, {0xB8, 0xFE, 0x57, 0x5F, 0x05, 0x0F, 0x88, 0x6E}};
	return guid;
}

BOOL VHDXRegionTableEntry::IsBatEntry()
{
	return IsEqualGUID(BatGuidSpecConst(), m_Guid) && m_Required;
}

BOOL VHDXRegionTableEntry::IsMetadataRegionEntry()
{
	return IsEqualGUID(MetadataRegionGuidSpecConst(), m_Guid) && m_Required;
}

BOOL VHDXRegionTableEntry::IsKnownEntry()
{
	return IsBatEntry() || IsMetadataRegionEntry();
}

LPCTSTR VHDXRegionTableEntry::RegionGuidTostring( CONST GUID & guid )
{
	if (IsEqualGUID(guid, BatGuidSpecConst()))
	{
		return L"BatGuidSpecConst";
	}
	else if (IsEqualGUID(guid, MetadataRegionGuidSpecConst()))
	{
		return L"MetadataRegionGuidSpecConst";
	}

	return L"'Not a well known Region GUID'";
}
