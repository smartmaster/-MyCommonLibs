#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include <map>
using namespace std;

#include "VhdxPrintField.h"

//#include <pshpack1.h> //not must
//#include <poppack.h>


#include <pshpack1.h> //not must

typedef struct VHDX_FILE_PARAMETERS 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	UINT32 m_BlockSize;
	UINT32 m_LeaveBlocksAllocated:1;
	UINT32 m_HasParent:1;
	UINT32 m_Reserved:30;
	//end of data.. MUST NOT add any more

	//////////////////////////////////////////////////////////////////////////
	VOID Print();

}VHDX_FILE_PARAMETERS;

typedef struct VHDX_VIRTUAL_DISK_SIZE 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	UINT64 m_VirtualDiskSize;
	//end of data.. MUST NOT add any more

	VOID Print();
}VHDX_VIRTUAL_DISK_SIZE;

typedef struct VHDX_PAGE83_DATA 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	GUID m_Page83Data;
	//end of data.. MUST NOT add any more

	//////////////////////////////////////////////////////////////////////////
	VOID Print();
}VHDX_PAGE83_DATA;

typedef struct VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	UINT32 m_LogicalSectorSize;
	//end of data.. MUST NOT add any more

	//////////////////////////////////////////////////////////////////////////
	VOID Print();
}VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE;

typedef struct VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	UINT32 m_PhysicalSectorSize;
	//end of data.. MUST NOT add any more


	//////////////////////////////////////////////////////////////////////////
	VOID Print();
}VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE;

typedef struct VHDX_PARENT_LOCATOR_HEADER 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	GUID	m_LocatorType;
	UINT16	m_Reserved;
	UINT16	m_KeyValueCount;
	//end of data.. MUST NOT add any more

	//////////////////////////////////////////////////////////////////////////
	static CONST GUID & LocatorTypeGuidSpecConst();

	static LPCTSTR LocatorTypeGuidTostring(CONST GUID & guid);

	//////////////////////////////////////////////////////////////////////////
	VOID Print();
}VHDX_PARENT_LOCATOR_HEADER;


namespace VHDX_PARENT_LOCATOR_ENTRY_NS {
	static CONST TCHAR parent_linkage[] = TEXT("parent_linkage");
	static CONST TCHAR parent_linkage2[] = TEXT("parent_linkage2");
	static CONST TCHAR relative_path[] = TEXT("relative_path");
	static CONST TCHAR volume_path[] = TEXT("volume_path");
	static CONST TCHAR absolute_win32_path[] = TEXT("absolute_win32_path");
}

typedef struct VHDX_PARENT_LOCATOR_ENTRY 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	UINT32 m_KeyOffset;
	UINT32 m_ValueOffset;
	UINT16 m_KeyLength;
	UINT16 m_ValueLength;
	//end of data.. MUST NOT add any more



	VOID Print();
}VHDX_PARENT_LOCATOR_ENTRY;

//forward declaration
struct VHDXMetadata;

typedef struct VHDXMetadataTableEntry 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	GUID	m_ItemId;
	UINT32	m_Offset;
	UINT32	m_Length;
	UINT32	m_IsUser:1;
	UINT32	m_IsVirtualDisk:1;
	UINT32	m_IsRequired:1;
	UINT32	m_Reserved:29;
	UINT32	m_Reserved2;
	//end of data.. MUST NOT add any more

	////////////////////////////////////////////////////////////////////////////
	static UINT32 MinOffsetSpecConst();

	static UINT32 MaxOffsetSpecConst();

	//static UINT32 MyMaxOffsetSpecConst();

	static CONST GUID & FileParametersGuidSpecConst();

	static CONST GUID & VirtualDiskSizeGuidSpecConst();

	static CONST GUID & Page83DataGuidSpecConst();

	
	static CONST GUID & LogicalSectorSizeGuidSpecConst();
	
	static CONST GUID & PhysicalSectorSizeGuidSpecConst();
	
	static CONST GUID & ParentLocatorGuidSpecConst();

	static LPCTSTR ItemGuidTostring(CONST GUID & guid);

	//////////////////////////////////////////////////////////////////////////
	VOID ParseMetadata(LPBYTE pMetadataStart, VHDXMetadata & Metadata, map<CString, CString> & ParentPath);


	VOID Print();

} VHDXMetadataTableEntry;
#include <poppack.h>



typedef struct VHDXMetadata
{
	VHDX_FILE_PARAMETERS * m_pVHDX_FILE_PARAMETERS;
	VHDX_VIRTUAL_DISK_SIZE * m_pVHDX_VIRTUAL_DISK_SIZE;
	VHDX_PAGE83_DATA * m_pVHDX_PAGE83_DATA;
	VHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE * m_pVHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE;
	VHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE * m_pVHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE;
	VHDX_PARENT_LOCATOR_HEADER * m_pVHDX_PARENT_LOCATOR_HEADER;
	VHDX_PARENT_LOCATOR_ENTRY * m_pVHDX_PARENT_LOCATOR_ENTRY;

	//////////////////////////////////////////////////////////////////////////
	VHDXMetadata()
	{
		m_pVHDX_FILE_PARAMETERS = NULL;
		m_pVHDX_VIRTUAL_DISK_SIZE = NULL;
		m_pVHDX_PAGE83_DATA = NULL;
		m_pVHDX_VIRTUAL_DISK_LOGICAL_SECTOR_SIZE = NULL;
		m_pVHDX_VIRTUAL_DISK_PHYSICAL_SECTOR_SIZE = NULL;
		m_pVHDX_PARENT_LOCATOR_HEADER = NULL;
		m_pVHDX_PARENT_LOCATOR_ENTRY = NULL;
	}
} VHDXMetadata;
