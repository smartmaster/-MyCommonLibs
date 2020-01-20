#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "crc32.h"
#include "VhdxHelper.h"


//#include <pshpack1.h> //not must
//#include <poppack.h>


#include <pshpack1.h> //not must
typedef struct VHDXRegionTableHeader 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	UINT32    m_Signature;              /* "regi" in ASCII */
	UINT32    m_Checksum;               /* CRC-32C hash of the 64KB table */
	UINT32    m_EntryCount;            /* number of valid entries */
	UINT32    m_Reserved;
	//end of data.. MUST NOT add any more


	//////////////////////////////////////////////////////////////////////////
	//code
	static UINT32 SignatureSpecConst();
	static UINT32 MaxEntryCountSpecConst();
	static INT OffsetSpecConst();
	static INT SizeSpecConst();

	//////////////////////////////////////////////////////////////////////////
	VOID CalcChecksum();
	BOOL IsValid();

	VOID Print();

	//////////////////////////////////////////////////////////////////////////
} VHDXRegionTableHeader;
#include <poppack.h>
