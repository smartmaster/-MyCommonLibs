#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "VhdxHelper.h"

#include <pshpack1.h> //not must
typedef struct VhdxFileIdentifier
{

	//////////////////////////////////////////////////////////////////////////
	//data
	UINT64    m_Signature; 
	UINT16    m_Creator[256];
	//end of data.. MUST NOT add any more

	
	//////////////////////////////////////////////////////////////////////////
	//code
	static UINT64 SignatureSpecConst();
	static INT OffsetSpecConst();
	static LONG SizeSpecConst();
	static LONG HeadersSizeSpecConst();
	static LONG HeadersTotalSizeSpecConst();

	//////////////////////////////////////////////////////////////////////////
	BOOL IsValid();
	VOID ReadCreator(CString & strCreator);
	VOID WriteSignature();
	VOID WriteCreator(LPCTSTR szCreator);
	//////////////////////////////////////////////////////////////////////////
	VOID Print();

} VhdxFileIdentifier;
#include <poppack.h>


