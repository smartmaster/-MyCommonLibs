#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "NameValueStream.h"

#include "ParameterStreamRW.h"

MethodStringDef(RunApp);
struct ParameterStruct(RunApp)
{
	SERIALIZE_BEGIN_STRUCT(ParameterRunApp, RunApp);
	CString Username; SERIALIZE_CSTRING(Username);
	CString Domain; SERIALIZE_CSTRING(Domain);
	CString Password; SERIALIZE_CSTRING(Password);
	CString CommandLine; SERIALIZE_CSTRING(CommandLine);
	CString CurDir; SERIALIZE_CSTRING(CurDir);
	ULONG TimeOut; SERIALIZE_BASIC(TimeOut);
	BOOL TerminateOnTimeOut; SERIALIZE_BASIC(TerminateOnTimeOut);
	SERIALIZE_END_STRUCT(ParameterRunApp, RunApp);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};

MethodStringDef(ResultRunApp);
struct ParameterStruct(ResultRunApp)
{
	SERIALIZE_BEGIN_STRUCT(ParameterResultRunApp, ResultRunApp);
	HRESULT hrStatus;SERIALIZE_BASIC(hrStatus);
	CString strErrorMessage;SERIALIZE_CSTRING(strErrorMessage);
	ULONG ExitCode;SERIALIZE_BASIC(ExitCode);
	SERIALIZE_END_STRUCT(ParameterResultRunApp, ResultRunApp);

public:
	STRUCT_SERIALIZER;

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR RootName;// = MethodString(xxxx);
};