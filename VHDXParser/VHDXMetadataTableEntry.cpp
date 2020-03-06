#include "stdafx.h"
#include "VhdxPrintField.h"
#include "VhdxHelper.h"
#include "VHDXMetadataTableEntry.h"




VOID VHDX_FILE_PARAMETERS::Print()
{
	//D_INFO(0, TEXT("VHDX_FILE_PARAMETERS+++++++++++++++++START"));
	PRINT_FIELD(m_BlockSize);
	PRINT_FIELD(m_LeaveBlocksAllocated);
	PRINT_FIELD(m_HasParent);
	PRINT_FIELD(m_Reserved);
	//D_INFO(0, TEXT("VHDX_FILE_PARAMETERS+++++++++++++++++END\r\n\r\n\r\n"));
}

VOID VHDX_VIRTUAL_DISK_SIZE::Print()
{
	//D_INFO(0, TEXT("VHDX_VIRTUAL_DISK_SIZE+++++++++++++++++START"));
	PRINT_FIELD(m_VirtualDiskSize);
	//D_INFO(0, TEXT("VHDX_VIRTUAL_DISK_SIZE+++++++++++++++++END\r\n\r\n\r\n"));
}

VOID VHDX_PAGE83_DATA::Print()
{
	//D_INFO(0, TEXT("VHDX_PAGE83_DATA+++++++++++++++++START"));
	PRINT_FIELD(m_Page83Data);
	//D_INFO(0, TEXT("VHDX_PAGE83_DATA+++++++++++++++++END\r\n\r\n\r\n"));
}

VOID VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE::Print()
{
	//D_INFO(0, TEXT("VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE+++++++++++++++++START"));
	PRINT_FIELD(m_LogicalSectorSize);
	//D_INFO(0, TEXT("VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE+++++++++++++++++END\r\n\r\n\r\n"));
}

VOID VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE::Print()
{
	//D_INFO(0, TEXT("VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE+++++++++++++++++START"));
	PRINT_FIELD(m_PhysicalSectorSize);
	//D_INFO(0, TEXT("VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE+++++++++++++++++END\r\n\r\n\r\n"));
}

CONST GUID & VHDX_PARENT_LOCATOR_HEADER::LocatorTypeGuidSpecConst()
{
	static CONST GUID guid = {0xB04AEFB7, 0xD19E, 0x4A81, {0xB7, 0x89, 0x25, 0xB8, 0xE9, 0x44, 0x59, 0x13}};
	return guid;
}

LPCTSTR VHDX_PARENT_LOCATOR_HEADER::LocatorTypeGuidTostring( CONST GUID & guid )
{
	if (IsEqualGUID(guid, LocatorTypeGuidSpecConst()))
	{
		return L"VHDX LocatorTypeGuidSpecConst";
	}

	return L"'Not a well known LocatorTypeGuid GUID'";
}

VOID VHDX_PARENT_LOCATOR_HEADER::Print()
{
	//D_INFO(0, TEXT("VHDX_PARENT_LOCATOR_HEADER+++++++++++++++++START"));
	PRINT_FIELD(m_LocatorType);
	PRINT_FIELD(m_Reserved);
	PRINT_FIELD(m_KeyValueCount);
	D_INFO(0, TEXT("LocatorType=%s"), LocatorTypeGuidTostring(m_LocatorType) );
	//D_INFO(0, TEXT("VHDX_PARENT_LOCATOR_HEADER+++++++++++++++++END\r\n\r\n\r\n"));
}

UINT32 VHDXMetadataTableEntry::MinOffsetSpecConst()
{
	return 64 * 1024;
}

UINT32 VHDXMetadataTableEntry::MaxOffsetSpecConst()
{
	return 1024 * 1024;
}

//UINT32 VHDXMetadataTableEntry::MyMaxOffsetSpecConst()
//{
//	return 128 * 1024;
//}

CONST GUID & VHDXMetadataTableEntry::FileParametersGuidSpecConst()
{
	static CONST GUID guid = {0xcaa16737, 0xfa36, 0x4d43, {0xb3, 0xb6, 0x33, 0xf0, 0xaa, 0x44, 0xe7, 0x6b}};
	return guid;
}

CONST GUID & VHDXMetadataTableEntry::VirtualDiskSizeGuidSpecConst()
{
	static CONST GUID guid = {0x2fa54224, 0xcd1b, 0x4876, {0xb2, 0x11, 0x5d, 0xbe, 0xd8, 0x3b, 0xf4, 0xb8}};
	return guid;
}

CONST GUID & VHDXMetadataTableEntry::Page83DataGuidSpecConst()
{
	static CONST GUID guid = {0xbeca12ab, 0xb2e6, 0x4523, {0x93, 0xef, 0xc3, 0x09, 0xe0, 0x00, 0xc7, 0x46}};
	return guid;
}

CONST GUID & VHDXMetadataTableEntry::LogicalSectorSizeGuidSpecConst()
{
	static CONST GUID guid = {0x8141bf1d, 0xa96f, 0x4709, {0xba, 0x47, 0xf2, 0x33, 0xa8, 0xfa, 0xab, 0x5f}};
	return guid;
}

CONST GUID & VHDXMetadataTableEntry::PhysicalSectorSizeGuidSpecConst()
{
	static CONST GUID guid = {0xcda348c7, 0x445d, 0x4471, {0x9c, 0xc9, 0xe9, 0x88, 0x52, 0x51, 0xc5, 0x56}};
	return guid;
}

CONST GUID & VHDXMetadataTableEntry::ParentLocatorGuidSpecConst()
{
	static CONST GUID guid = {0xa8d35f2d, 0xb30b, 0x454d, {0xab, 0xf7, 0xd3, 0xd8, 0x48, 0x34, 0xab, 0x0c}};
	return guid;
}

LPCTSTR VHDXMetadataTableEntry::ItemGuidTostring( CONST GUID & guid )
{
	if (IsEqualGUID(guid, FileParametersGuidSpecConst()))
	{
		return L"FileParametersGuidSpecConst";
	}
	else if (IsEqualGUID(guid, VirtualDiskSizeGuidSpecConst()))
	{
		return L"VirtualDiskSizeGuidSpecConst";
	}
	else if (IsEqualGUID(guid, Page83DataGuidSpecConst()))
	{
		return L"Page83DataGuidSpecConst";
	}
	else if (IsEqualGUID(guid, LogicalSectorSizeGuidSpecConst()))
	{
		return L"LogicalSectorSizeGuidSpecConst";
	}
	else if (IsEqualGUID(guid, PhysicalSectorSizeGuidSpecConst()))
	{
		return L"PhysicalSectorSizeGuidSpecConst";
	}
	else if (IsEqualGUID(guid, ParentLocatorGuidSpecConst()))
	{
		return L"ParentLocatorGuidSpecConst";
	}

	return L"'Not a well known metadata item GUID'";
}

VOID VHDXMetadataTableEntry::ParseMetadata( LPBYTE pMetadataStart, VHDXMetadata & Metadata, map<CString, CString> & ParentPath )
{
	if (m_Offset < MinOffsetSpecConst() || (m_Offset + m_Length) > MaxOffsetSpecConst())
	{
		D_INFO(0, TEXT("Invalid offset or length. ItemGUID=%s. %u(0x%08x), %u(0x%08x)"), ItemGuidTostring(m_ItemId), m_Offset, m_Offset, m_Length, m_Length);
		return;
	}

	if (IsEqualGUID( m_ItemId, FileParametersGuidSpecConst()))
	{
		Metadata.m_pVHDX_FILE_PARAMETERS = (VHDX_FILE_PARAMETERS *)(pMetadataStart + m_Offset);
		Metadata.m_pVHDX_FILE_PARAMETERS->Print();
	}
	else if (IsEqualGUID( m_ItemId, VirtualDiskSizeGuidSpecConst()))
	{
		Metadata.m_pVHDX_VIRTUAL_DISK_SIZE = (VHDX_VIRTUAL_DISK_SIZE  *)(pMetadataStart + m_Offset);
		Metadata.m_pVHDX_VIRTUAL_DISK_SIZE->Print();
	}
	else if (IsEqualGUID( m_ItemId, Page83DataGuidSpecConst()))
	{ 
		Metadata.m_pVHDX_PAGE83_DATA = (VHDX_PAGE83_DATA *)(pMetadataStart + m_Offset);
		Metadata.m_pVHDX_PAGE83_DATA->Print();
	}
	else if (IsEqualGUID( m_ItemId, LogicalSectorSizeGuidSpecConst()))
	{
		Metadata.m_pVHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE = (VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE   *)(pMetadataStart + m_Offset);
		Metadata.m_pVHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE->Print();
	}
	else if (IsEqualGUID( m_ItemId, PhysicalSectorSizeGuidSpecConst()))
	{
		Metadata.m_pVHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE = (VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE  *)(pMetadataStart + m_Offset);
		Metadata.m_pVHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE->Print();
	}
	else if (IsEqualGUID( m_ItemId, ParentLocatorGuidSpecConst()))
	{
		Metadata.m_pVHDX_PARENT_LOCATOR_HEADER = (VHDX_PARENT_LOCATOR_HEADER  *)(pMetadataStart + m_Offset);
		Metadata.m_pVHDX_PARENT_LOCATOR_HEADER->Print();
		Metadata.m_pVHDX_PARENT_LOCATOR_ENTRY = (VHDX_PARENT_LOCATOR_ENTRY *)((BYTE*)(Metadata.m_pVHDX_PARENT_LOCATOR_HEADER) + sizeof(VHDX_PARENT_LOCATOR_HEADER));
		for (UINT16 jj = 0; jj < Metadata.m_pVHDX_PARENT_LOCATOR_HEADER->m_KeyValueCount; ++ jj)
		{
			Metadata.m_pVHDX_PARENT_LOCATOR_ENTRY[jj].Print();

			CString strKey, strValue;
			GetStringInBuffer((LPCBYTE)Metadata.m_pVHDX_PARENT_LOCATOR_HEADER, Metadata.m_pVHDX_PARENT_LOCATOR_ENTRY[jj].m_KeyOffset, Metadata.m_pVHDX_PARENT_LOCATOR_ENTRY[jj].m_KeyLength, strKey);
			GetStringInBuffer((LPCBYTE)Metadata.m_pVHDX_PARENT_LOCATOR_HEADER, Metadata.m_pVHDX_PARENT_LOCATOR_ENTRY[jj].m_ValueOffset, Metadata.m_pVHDX_PARENT_LOCATOR_ENTRY[jj].m_ValueLength, strValue);

			D_INFO(0, TEXT("Parent locator data (Key, Value) = (%s, %s)"), strKey.GetString(), strValue.GetString());
			ParentPath.insert(make_pair(strKey, strValue));
		}
	}
	else
	{
		TCHAR GuidBuffer[64] = {0};
		D_INFO(0, TEXT("Cannot recognize item with GUID=%s"), GuidToStringHelper(m_ItemId, GuidBuffer, _countof(GuidBuffer)));
	}
}

VOID VHDXMetadataTableEntry::Print()
{
	//D_INFO(0, TEXT("VHDXMetadataTableEntry+++++++++++++++++START"));
	PRINT_FIELD(m_ItemId);
	PRINT_FIELD(m_Offset);
	PRINT_FIELD(m_Length);
	PRINT_FIELD(m_IsUser);
	PRINT_FIELD(m_IsVirtualDisk);
	PRINT_FIELD(m_IsRequired);
	PRINT_FIELD(m_Reserved);
	PRINT_FIELD(m_Reserved2);
	D_INFO(0, TEXT("Item=%s"), ItemGuidTostring(m_ItemId));
	//D_INFO(0, TEXT("VHDXMetadataTableEntry+++++++++++++++++END\r\n\r\n\r\n"));
}

VOID VHDX_PARENT_LOCATOR_ENTRY::Print()
{
	//D_INFO(0, TEXT("VHDX_PARENT_LOCATOR_ENTRY+++++++++++++++++START"));
	PRINT_FIELD(m_KeyOffset);
	PRINT_FIELD(m_ValueOffset);
	PRINT_FIELD(m_KeyLength);
	PRINT_FIELD(m_ValueLength);
	//D_INFO(0, TEXT("VHDX_PARENT_LOCATOR_ENTRY+++++++++++++++++END\r\n\r\n\r\n"));
}
