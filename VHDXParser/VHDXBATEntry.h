#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "VhdxPrintField.h"

//#include <pshpack1.h> //not must
//#include <poppack.h>


#include <pshpack1.h> //not must
typedef struct VHDX_BAT_ENTRY 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	UINT64 m_State:3;
	UINT64 m_Reserved:17;
	UINT64 m_FileOffsetMB:44;
	//end of data.. MUST NOT add any more

	//////////////////////////////////////////////////////////////////////////
	//code
	static CONST INT PAYLOAD_BLOCK_NOT_PRESENT = 0;
	static CONST INT PAYLOAD_BLOCK_UNDEFINED = 1;
	static CONST INT PAYLOAD_BLOCK_ZERO = 2;
	static CONST INT PAYLOAD_BLOCK_UNMAPPED = 3;
	static CONST INT PAYLOAD_BLOCK_FULLY_PRESENT = 6;
	static CONST INT PAYLOAD_BLOCK_PARTIALLY_PRESENT = 7;

	static CONST INT SB_BLOCK_NOT_PRESENT = 0;
	static CONST INT SB_BLOCK_PRESENT = 6;
	static CONST LONG SECTOR_BITMAP_BLOCK_SIZE = 1024 * 1024;

	static LPCTSTR StateToString(INT State);
	//<sonmi01>2014-1-9 ###???
	static BOOL CurentDiskDataZeroedOrUndefined(CONST VHDX_BAT_ENTRY & BatEntry);

	VOID Print(LONG Index);
} VHDX_BAT_ENTRY;
#include <poppack.h>