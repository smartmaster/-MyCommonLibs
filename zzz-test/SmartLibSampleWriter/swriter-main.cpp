/*
**++
**
** Copyright (c) Microsoft Corporation
**
**
** Module Name:
**
**	main.cpp
**
**
** Abstract:
**
**	Sample dependency writer
**
*/


///////////////////////////////////////////////////////////////////////////////
// Includes

#include "pch.h"
#include "swriter-main.h"
#include "swriter.h"
#include "log.h"
///////////////////////////////////////////////////////////////////////////////
// Declarations

HANDLE g_quitEvent = NULL;


///////////////////////////////////////////////////////////////////////////////

extern "C" int __cdecl  wmain(__in int argc, __in_ecount(argc)wchar_t ** argv)
{

	CString debugFile = argv[0];
	debugFile += TEXT(".debug");
	DWORD gfa = GetFileAttributes(debugFile.GetString());
	if (gfa != INVALID_FILE_ATTRIBUTES)
	{
		DWORD dwTime = 300;
		for (int ii = 0; ii < dwTime; ++ ii)
		{
			Sleep(1000);
		}
	}


    UNREFERENCED_PARAMETER(argc);
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED );
	if (FAILED(hr))	{
	    SML_LOG_LINE(TEXT(__FUNCTION__) L"::CoInitializeEx failed!");
	    return 1;
	}
	
   hr = ::CoInitializeSecurity(
       NULL,                                 //  IN PSECURITY_DESCRIPTOR         pSecDesc,
       -1,                                  //  IN LONG                         cAuthSvc,
       NULL,                                //  IN SOLE_AUTHENTICATION_SERVICE *asAuthSvc,
       NULL,                                //  IN void                        *pReserved1,
       RPC_C_AUTHN_LEVEL_PKT_PRIVACY,       //  IN DWORD                        dwAuthnLevel,
       RPC_C_IMP_LEVEL_IDENTIFY,            //  IN DWORD                        dwImpLevel,
       NULL,                                //  IN void                        *pAuthList,
       EOAC_NONE,
                                            //  IN DWORD                        dwCapabilities,
       NULL                                 //  IN void                        *pReserved3
       );
	if (FAILED(hr))	{
		SML_LOG_LINE(TEXT(__FUNCTION__) L"::CoInitializeSecurity failed!");
		return 1;
	}

	g_quitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (g_quitEvent == NULL)	{
		SML_LOG_LINE(TEXT(__FUNCTION__) L"::CreateEvent failed!");
		return 1;
	}

	// set a control handler that allows the writer to be shut down
	if (!::SetConsoleCtrlHandler(handler, TRUE))	{
		SML_LOG_LINE(TEXT(__FUNCTION__) L"::SetConsoleSecurityHandler failed!");
		return 1;
	}

    SmlSampleWriter::StaticInitialize();
        
	// We want the writer to go out of scope before the return statement
	{
		SmlSampleWriter writer;
		hr = writer.Initialize();
		if (FAILED(hr))	{
		    SML_LOG_LINE(TEXT(__FUNCTION__) L"::Writer init failed!");
		    return 1;
		}


		if(::WaitForSingleObject(g_quitEvent, INFINITE) != WAIT_OBJECT_0)	{
			SML_LOG_LINE(TEXT(__FUNCTION__) L"::WaitForSingleObject failed!");
			return 1;
		}
		writer.Uninitialize();
	}
	
	return 0;	
}

BOOL WINAPI handler(DWORD)
{
	// we want to quit independent of what the control event was
	::SetEvent(g_quitEvent);

	return TRUE;
}
