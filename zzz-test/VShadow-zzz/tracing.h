/////////////////////////////////////////////////////////////////////////
// Copyright ?Microsoft Corporation. All rights reserved.
// 
//  This file may contain preliminary information or inaccuracies, 
//  and may not correctly represent any associated Microsoft 
//  Product as commercially released. All Materials are provided entirely 
//  “AS IS.?To the extent permitted by law, MICROSOFT MAKES NO 
//  WARRANTY OF ANY KIND, DISCLAIMS ALL EXPRESS, IMPLIED AND STATUTORY 
//  WARRANTIES, AND ASSUMES NO LIABILITY TO YOU FOR ANY DAMAGES OF 
//  ANY TYPE IN CONNECTION WITH THESE MATERIALS OR ANY INTELLECTUAL PROPERTY IN THEM. 
// 


#pragma once


/////////////////////////////////////////////////////////////////////////
//  Generic tracing/logger class
//


// Very simple tracing/logging class 
class FunctionTracer
{
public:
	FunctionTracer(CONST wstring & fileName, INT lineNumber, CONST wstring & functionName);
	~FunctionTracer();

	// tracing routine
	void Trace(CONST wstring & file, int line, CONST wstring & functionName, wstring format, ...);

	// console logging routine
	void WriteLine(wstring format, ...);

	// Converts a HRESULT into a printable message
	static wstring  HResult2String(HRESULT hrError);

	// Enables tracing
	static void EnableTracingMode();

private:

	//
	//  Data members
	//

	static bool m_traceEnabled;
	const wstring  &   m_fileName;
	int         m_lineNumber;
	const wstring   &  m_functionName;
};

