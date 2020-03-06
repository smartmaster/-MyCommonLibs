#include "stdafx.h"

#include "ErrorResultStatus.h"


void ErrorResultStatus_NS::ErrorElement::InitError(ErrorElement & err, HRESULT hr, LPCTSTR Message, LPCTSTR Module, LPCTSTR Function, LPCTSTR File, LONG Line, ...)
{
	//////////////////////////////////////////////////////////////////////////
	err.hr = hr;

	va_list args;
	va_start(args, Line);
	err.Message.FormatV(Message, args);
	va_end(args);
	//err.LocallizedMessage;

	//////////////////////////////////////////////////////////////////////////
	//err.HelpLink;
	//err.LocalizedHelpLink;

	//////////////////////////////////////////////////////////////////////////
	err.Pid = GetCurrentProcessId();
	err.Tid = GetCurrentThreadId();

	//////////////////////////////////////////////////////////////////////////
	err.Module = Module;
	err.Function = Function;
	err.File = File;
	err.Line = Line;

	//////////////////////////////////////////////////////////////////////////
	err.Enum = 0;
	err.Flags = 0;
	err.MoreData = 0;
	//err.MoreString;
}

void ErrorResultStatus_NS::ErrorCollection::AddCurrentError(ErrorCollection & errors)
{
	CONST LONG MAX_COUNT = 256;
	if (errors.LastErrors.size() >= MAX_COUNT)
	{
		errors.LastErrors.pop_front();
	}
	errors.LastErrors.push_back(errors.CurrentError);
}


namespace ErrorResultStatus_NS
{
#include "ErrorResultStatus.h.xml.Output.hpp"
}