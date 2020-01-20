#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "VhdxHelper.h"

//#include <pshpack1.h> //not must
//#include <poppack.h>


#include <pshpack1.h> //not must
typedef struct VHDXRegionTableEntry 
{
	//////////////////////////////////////////////////////////////////////////
	//data
    GUID	m_Guid;                  //  128-bit unique identifier 
    UINT64	m_FileOffset;            // offset of the object in the file.
    UINT32	m_Length;                //  length, in bytes, of the object 
	UINT32	m_Required:1;
	UINT32	m_Reserved:31;
	//end of data.. MUST NOT add any more

	//////////////////////////////////////////////////////////////////////////
	//code
	static CONST GUID & BatGuidSpecConst();
	static CONST GUID & MetadataRegionGuidSpecConst();

	static LPCTSTR RegionGuidTostring(CONST GUID & guid);


	//////////////////////////////////////////////////////////////////////////
	BOOL IsBatEntry();
	BOOL IsMetadataRegionEntry();
	BOOL IsKnownEntry();

	VOID Print();
	
	//////////////////////////////////////////////////////////////////////////
} VHDXRegionTableEntry;
#include <poppack.h>