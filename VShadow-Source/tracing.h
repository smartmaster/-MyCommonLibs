/////////////////////////////////////////////////////////////////////////
// Copyright ?Microsoft Corporation. All rights reserved.
// 
//  This file may contain preliminary information or inaccuracies, 
//  and may not correctly represent any associated Microsoft 
//  Product as commercially released. All Materials are provided entirely 
//  ???????To the extent permitted by law, MICROSOFT MAKES NO 
//  WARRANTY OF ANY KIND, DISCLAIMS ALL EXPRESS, IMPLIED AND STATUTORY 
//  WARRANTIES, AND ASSUMES NO LIABILITY TO YOU FOR ANY DAMAGES OF 
//  ANY TYPE IN CONNECTION WITH THESE MATERIALS OR ANY INTELLECTUAL PROPERTY IN THEM. 
// 


#pragma once


/////////////////////////////////////////////////////////////////////////
//  Generic tracing/logger class
//

#if 0
#define MY_WPRINTF(format, ...) wprintf(format, __VA_ARGS__)
#else
#define MY_WPRINTF(format, ...) 
#endif

// Very simple tracing/logging class 
class FunctionTracer
{
public:
    FunctionTracer(const wstring & fileName, INT lineNumber, const wstring & functionName); //<sonmi01>2014-10-10 ###???
    ~FunctionTracer();
    
    // tracing routine
	void Trace(const wstring & file, int line, const wstring & functionName, LPCTSTR format, ...);
    
    // console logging routine
	void WriteLine(LPCTSTR format, ...);
    
    // Converts a HRESULT into a printable message
    static wstring  HResult2String(HRESULT hrError);

    // Enables tracing
    static void EnableTracingMode();

private:

    //
    //  Data members
    //

    static bool m_traceEnabled;

    wstring     m_fileName;
    int         m_lineNumber;
    wstring     m_functionName;

};

