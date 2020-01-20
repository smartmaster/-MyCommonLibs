#include "stdafx.h"


#include "sml_resource_manager.h"

void SML_OBJ_FUNC_1(SML_Resource, Cleanup)(struct SML_Resource * res)
{
	switch (res->m_rcm)
	{

	case SML_RCM_FUNC_0:
	{
		SML_pfnCleanup_0 func = (SML_pfnCleanup_0)(res->m_func);
		func();
	}
	break;

	case SML_RCM_FUNC_1:
	{
		SML_pfnCleanup_1 func = (SML_pfnCleanup_1)(res->m_func);
		func(res->m_resource);
	}
	break;

	case SML_RCM_FUNC_STDCALL_1:
	{
		SML_pfnCleanupStdcall_1 func = (SML_pfnCleanupStdcall_1)(res->m_func);
		func(res->m_resource);
	}
	break;

	case SML_RCM_FUNC_2:
	{
		SML_pfnCleanup_2 func = (SML_pfnCleanup_2)(res->m_func);
		func(res->m_resource, res->m_resource2);
	}
	break;

	case SML_RCM_FUNC_STDCALL_2:
	{
		SML_pfnCleanupStdcall_2 func = (SML_pfnCleanupStdcall_2)(res->m_func);
		func(res->m_resource, res->m_resource2);
	}
	break;

	case SML_RCM_FUNC_3:
	{
		SML_pfnCleanup_3 func = (SML_pfnCleanup_3)(res->m_func);
		func(res->m_resource, res->m_resource2, res->m_resource3);
	}
	break;

	case SML_RCM_FUNC_STDCALL_3:
	{
		SML_pfnCleanupStdcall_3 func = (SML_pfnCleanupStdcall_3)(res->m_func);
		func(res->m_resource, res->m_resource2, res->m_resource3);
	}
	break;


	case SML_RCM_FUNC_4:
	{
		SML_pfnCleanup_4 func = (SML_pfnCleanup_4)(res->m_func);
		func(res->m_resource, res->m_resource2, res->m_resource3, res->m_resource4);
	}
	break;

	case SML_RCM_FUNC_STDCALL_4:
	{
		SML_pfnCleanupStdcall_4 func = (SML_pfnCleanupStdcall_4)(res->m_func);
		func(res->m_resource, res->m_resource2, res->m_resource3, res->m_resource4);
	}
	break;


	case SML_RCM_FUNC_5:
	{
		SML_pfnCleanup_5 func = (SML_pfnCleanup_5)(res->m_func);
		func(res->m_resource, res->m_resource2, res->m_resource3, res->m_resource4, res->m_resource5);
	}
	break;

	case SML_RCM_FUNC_STDCALL_5:
	{
		SML_pfnCleanupStdcall_5 func = (SML_pfnCleanupStdcall_5)(res->m_func);
		func(res->m_resource, res->m_resource2, res->m_resource3, res->m_resource4, res->m_resource5);
	}
	break;


	case SML_RCM_FUNC_6:
	{
		SML_pfnCleanup_6 func = (SML_pfnCleanup_6)(res->m_func);
		func(res->m_resource, res->m_resource2, res->m_resource3, res->m_resource4, res->m_resource5, res->m_resource6);
	}
	break;

	case SML_RCM_FUNC_STDCALL_6:
	{
		SML_pfnCleanupStdcall_6 func = (SML_pfnCleanupStdcall_6)(res->m_func);
		func(res->m_resource, res->m_resource2, res->m_resource3, res->m_resource4, res->m_resource5, res->m_resource6);
	}
	break;

	case SML_RCM_FUNC_PARAMS:
	{
		SML_pfnCleanup_1 func = (SML_pfnCleanup_1)(res->m_func);
		func(res->m_resource);
		if (res->m_resource2)
		{
			SML_pfnCleanup_1 func_destroy_params_member = (SML_pfnCleanup_1)(res->m_resource2);
			func_destroy_params_member(res->m_resource);
		}
		SML_FREE_PARAMS(res->m_resource);
	}
	break;
	
	}

	ZeroMemory(res, sizeof(struct SML_Resource));
}




//void Init();
void SML_OBJ_FUNC_1(SML_ResourceList, Init)(struct SML_ResourceList * resList)
{
	ZeroMemory(resList, sizeof(struct SML_ResourceList));
	resList->m_list = resList->m_stack;
	resList->m_count = 0;
	resList->m_capacity = SML_INITIAL_RESOURCE_COUNT;
}
//void Add();
int SML_OBJ_FUNC_1(SML_ResourceList, Add)(
	struct SML_ResourceList * resList,
	enum  SML_RESOURCE_CLEANUP_METHOD rcm,
	void * func,
	void * resource,
	void * resource2,
	void * resource3,
	void * resource4,
	void * resource5,
	void * resource6,
	const CHAR * atFile,
	const CHAR * atFunc,
	INT atLine)
{
	if (resList->m_count >= resList->m_capacity)
	{
		resList->m_capacity += SML_INITIAL_RESOURCE_COUNT;

		if (resList->m_list == resList->m_stack)
		{
			resList->m_list = (struct SML_Resource *)HeapAlloc(
				GetProcessHeap(),//_In_ HANDLE hHeap,
				HEAP_ZERO_MEMORY,//_In_ DWORD  dwFlags,
				resList->m_capacity * sizeof(struct SML_Resource)//,//_In_ SIZE_T dwBytes
				);
			CopyMemory(resList->m_list, resList->m_stack, resList->m_count * sizeof(struct SML_Resource));
			ZeroMemory(resList->m_stack, resList->m_count * sizeof(struct SML_Resource));
		}
		else
		{
			resList->m_list = (struct SML_Resource *)HeapReAlloc(
				GetProcessHeap(),//_In_ HANDLE hHeap,
				HEAP_ZERO_MEMORY,//_In_ DWORD  dwFlags,
				resList->m_list,//_In_ LPVOID lpMem,
				resList->m_capacity * sizeof(struct SML_Resource)//,//_In_ SIZE_T dwBytes
				);
		}
	}

	resList->m_list[resList->m_count].m_rcm = rcm;
	resList->m_list[resList->m_count].m_func = func;
	resList->m_list[resList->m_count].m_resource = resource;
	resList->m_list[resList->m_count].m_resource2 = resource2;
	resList->m_list[resList->m_count].m_resource3 = resource3;
	resList->m_list[resList->m_count].m_resource4 = resource4;
	resList->m_list[resList->m_count].m_resource5 = resource5;
	resList->m_list[resList->m_count].m_resource6 = resource6;
	resList->m_list[resList->m_count].m_atFile = atFile;
	resList->m_list[resList->m_count].m_atFunc = atFunc;
	resList->m_list[resList->m_count].m_atLine = atLine;

	return resList->m_count++;
}


void SML_OBJ_FUNC_1(SML_ResourceList, NoCleanupItem)(
	struct SML_ResourceList * resList,
	int index
	)
{
	ZeroMemory(&resList->m_list[index], sizeof(struct SML_Resource));
}

//void Cleanup()
void SML_OBJ_FUNC_1(SML_ResourceList, Cleanup)(struct SML_ResourceList * resList)
{
	for (LONG ii = resList->m_count - 1; ii >= 0; --ii)
	{
		SML_OBJ_FUNC_1(SML_Resource, Cleanup)(resList->m_list + ii);
	}

	if (resList->m_list != resList->m_stack)
	{
		HeapFree(GetProcessHeap(), 0, resList->m_list);
	}

	ZeroMemory(resList, sizeof(struct SML_ResourceList));
}