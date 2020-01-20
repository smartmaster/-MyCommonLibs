#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include "VhdxPrintField.h"


//#include <pshpack1.h> //not must
//#include <poppack.h>


#include <pshpack1.h> //not must
typedef struct VHDXMetadataTableHeader 
{
	//////////////////////////////////////////////////////////////////////////
	//data
	UINT64    m_Signature;              /* "metadata" in ASCII */
	UINT16    m_Reserved;
	UINT16    m_EntryCount;            /* number table entries. <= 2047 */
	UINT32    m_Reserved2[5];
	//end of data.. MUST NOT add any more

	//////////////////////////////////////////////////////////////////////////
	//code
	static UINT64 SignatuireSpecConst();
	static UINT16 MaxEntryCountSpecConst();
	static INT SizeSpecConst();

	//////////////////////////////////////////////////////////////////////////
	//code
	BOOL IsValid();
	VOID Print();

	//////////////////////////////////////////////////////////////////////////
} VHDXMetadataTableHeader;
#include <poppack.h>