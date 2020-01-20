#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"
//#include "..\PublicHeader\PublicNameValueStream.h"
#include "NameValueStream.h"

#include "ParameterStreamDef.h"

HRESULT SkipSubStruct(IFileDevice * pFileDevice);

MethodStringDef(CreateInstanceFileDeviceWin32T);
struct ParameterStruct(CreateInstanceFileDeviceWin32T)
{
	CString strFileName; 
	ULONG ulDesiredAccess; 
	ULONG ulShareMode; 
	ULONG ulCreationDisposition; 
	ULONG ulFlagsAndAttributes; 
	//OUT IFileDevice ** ppFileDevice; 
	BOOL bCreateMember;

public:
	HRESULT WriteTo(IFileDevice * pFileDevice);
	HRESULT ReadFrom(IFileDevice * pFileDevice);
	HRESULT WriteToBuffer(LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength);
	HRESULT ReadFromBuffer(LPBYTE pBuffer, LONGLONG Length);

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(SetPointerEx);
struct  ParameterStruct(SetPointerEx)
{
	LONGLONG llDistanceToMove; 
	//LONGLONG* lpNewFilePointer; 
	ULONG ulMoveMethod;

public:
	HRESULT WriteTo(IFileDevice * pFileDevice);
	HRESULT ReadFrom(IFileDevice * pFileDevice);
	HRESULT WriteToBuffer(LPBYTE pBuffer, LONGLONG Length, LONGLONG & WrittenLength);
	HRESULT ReadFromBuffer(LPBYTE pBuffer, LONGLONG Length);

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};