// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ThreadLocalData.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	BOOL bRet = TRUE;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (TLS_OUT_OF_INDEXES == CTlsData::AllocateIndex())
		{
			bRet = FALSE;
			break;
		}
		CTlsData::AllocateData();
		break;

	case DLL_THREAD_ATTACH:
		CTlsData::AllocateData();
		break;

	case DLL_THREAD_DETACH:
		CTlsData::FreeData();
		break;

	case DLL_PROCESS_DETACH:
		CTlsData::FreeData();
		CTlsData::FreeIndex();
		break;
	}

	return bRet;
}

