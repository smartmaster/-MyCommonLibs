#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "crc32.h"
#include "VhdxHelper.h"

#include <pshpack1.h> //not must
typedef struct VhdxHeader
{
	//////////////////////////////////////////////////////////////////////////
	//data	
	UINT32	m_Signature;
	UINT32	m_Checksum;
	UINT64	m_SequenceNumber;
	GUID	m_FileWrite;
	GUID	m_DataWrite;
	GUID	m_Log;
	UINT16	m_LogVersion;
	UINT16	m_Version;
	UINT32	m_LogLength;
	UINT64	m_LogOffset;
	UINT8	m_Reserved[4016];
	//end of data.. MUST NOT add any more


	//////////////////////////////////////////////////////////////////////////
	static UINT32 SignatureSpecConst();
	static INT SizeSpecConst();
	static INT Headser1OffsetSpecConst();
	static INT Headser2OffsetSpecConst();

	//////////////////////////////////////////////////////////////////////////
	VOID CalcChecksum();
	BOOL IsValid();

	VOID Print();

	//////////////////////////////////////////////////////////////////////////
} VhdxHeader;
#include <poppack.h>