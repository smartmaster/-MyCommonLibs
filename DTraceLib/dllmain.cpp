// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ThreadLocalData.h"
#include "ProcessGlobalData.h"


BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	BOOL bRet = TRUE;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CSIProcessGlobalData::CreateMember();
		if (TLS_OUT_OF_INDEXES == CTlsDataManager::AllocateIndex())
		{
			bRet = FALSE;
			break;
		}
		CTlsDataManager::AllocateData();
		break;

	case DLL_THREAD_ATTACH:
		CTlsDataManager::AllocateData();
		break;

	case DLL_THREAD_DETACH:
		CTlsDataManager::FreeData();
		break;

	case DLL_PROCESS_DETACH:
		CTlsDataManager::FreeData();
		CTlsDataManager::FreeIndex();
		CSIProcessGlobalData::DestroyMember();
		break;
	}

	return bRet;
}

