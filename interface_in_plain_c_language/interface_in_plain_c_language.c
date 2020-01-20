// test_c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <tchar.h>
#include <windows.h>

#include "my_interface.h"
#include "sml_resource_manager.h"
#include "test_res_mgmt_macro.h"
#include "test_SML_RCM_FUNC_PARAMS.h"

int _tmain_case_000_iinc(INT argc, TCHAR ** argv);
int _tmain_case_001_resoure_mgr(INT argc, TCHAR ** argv);
int _tmain_case_001_res_mgr_again(int argc, TCHAR ** argv);
int _tmain_case_002_iinc_with_res_mgr(INT argc, TCHAR ** argv);
int _tmain_test_003_res_no_cleanup(INT argc, TCHAR ** argv);


int _tmain(INT argc, TCHAR ** argv)
{
	int testCase = 2;
	switch (testCase)
	{
	case 2:
	{
		_tmain_case_002_iinc_with_res_mgr(argc, argv);
	}
	break;
	case 1:
	{
		_tmain_test_003_res_no_cleanup(argc, argv);
		SML_OBJ_FUNC_1(MySomeParams, Test)();
		SML_OBJ_FUNC_1(MyResObj, Test)();
		SML_OBJ_FUNC_1(MyResObj, Test2)();
		_tmain_case_001_res_mgr_again(argc, argv);
		_tmain_case_001_resoure_mgr(argc, argv);
	}
	break;
	case  0:
	{
		_tmain_case_000_iinc(argc, argv);
	}
	break;
	}
}


int _tmain_case_002_iinc_with_res_mgr(INT argc, TCHAR ** argv)
{
	SML_RESOURCE_INIT;

	struct  SML_IObject * pIMyObject = NULL; //create instance, ref count  ++

	int allocMehtod = SAM_VIRTUAL_ALLOC;
	switch (allocMehtod)
	{
	case SAM_AUTO:
	{
		INT size = SML_OBJ_FUNC_1(MyNameObj, SizeOf)();
		VOID * addr = _alloca(size);
		pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstanceStack_SAM_AUTO)(addr, size, 1024);
	}
	break;
	case SAM_MALLOC:
	{
		pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_MALLOC)(1024);
	}
	break;
	case SAM_HEAP_ALLOC:
	{
		pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_HEAP_ALLOC)(1024);
	}
	break;
	case SAM_VIRTUAL_ALLOC:
	{
		pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_VIRTUAL_ALLOC)(1024);
	}
	break;
	case SAM__MALLOCA:
	{
		pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM__MALLOCA)(1024);
	}
	break;

	}

	SML_RESOURCE_ADD_1(pIMyObject->Release, pIMyObject);



	struct IMyProp * pIMyProp = SML_CALL_METHOD(QueryInterface, pIMyObject, SML_NAME_OF(IMyProp)); //QueryInterface, ref count ++
	SML_RESOURCE_ADD_1(pIMyProp->SML_INTERFACE(SML_IObject).Release, &pIMyProp->SML_INTERFACE(SML_IObject));

	{
		struct SML_IObject * pIMyObject1 = SML_CALL_METHOD(QueryInterface, pIMyObject, SML_NAME_OF(SML_IObject));//QueryInterface, ref count ++
		SML_RESOURCE_ADD_1(pIMyObject1->Release, pIMyObject1);

		//SML_CALL_METHOD(Release, pIMyObject); //ref count --
		//pIMyObject = NULL;

		//SML_CALL_METHOD(Release, pIMyObject1); //ref count --
		//pIMyObject1 = NULL;
	}


	SML_CALL_METHOD(CreateMember, &pIMyProp->SML_INTERFACE(SML_IObject)); //create resources 

	SML_CALL_METHOD(SetName, pIMyProp, TEXT("Iron Mike Tyson")); //object operation

	WCHAR name[256] = { 0 };
	_tcscpy_s(name, _countof(name), SML_CALL_METHOD(GetName, pIMyProp)); //object operation N...

	{
		struct IMyMethod * pIMyMethod = SML_CALL_METHOD(QueryInterface, &pIMyProp->SML_INTERFACE(SML_IObject), SML_NAME_OF(IMyMethod));
		SML_RESOURCE_ADD_1(pIMyMethod->SML_INTERFACE(SML_IObject).Release, &pIMyMethod->SML_INTERFACE(SML_IObject));

		SML_CALL_METHOD(SayHi, pIMyMethod);
		SML_CALL_METHOD(SayBye, pIMyMethod);

		{
			struct SML_IObject * ptemp1 = SML_CALL_METHOD(QueryInterface, &pIMyMethod->SML_INTERFACE(SML_IObject), SML_NAME_OF(SML_IObject));
			SML_RESOURCE_ADD_1(ptemp1->Release, ptemp1);
			//SML_CALL_METHOD(Release, ptemp1);

			struct IMyProp * ptemp2 = SML_CALL_METHOD(QueryInterface, &pIMyMethod->SML_INTERFACE(SML_IObject), SML_NAME_OF(IMyProp));
			SML_RESOURCE_ADD_1(ptemp2->SML_INTERFACE(SML_IObject).Release, &ptemp2->SML_INTERFACE(SML_IObject));
			//SML_CALL_METHOD(Release, &ptemp2->SML_INTERFACE(SML_IObject));

			struct IMyMethod * ptemp3 = SML_CALL_METHOD(QueryInterface, &pIMyMethod->SML_INTERFACE(SML_IObject), SML_NAME_OF(IMyMethod));
			SML_RESOURCE_ADD_1(ptemp3->SML_INTERFACE(SML_IObject).Release, &ptemp3->SML_INTERFACE(SML_IObject));
			//SML_CALL_METHOD(Release, &ptemp3->SML_INTERFACE(SML_IObject));
		}


		//SML_CALL_METHOD(Release, &pIMyMethod->SML_INTERFACE(SML_IObject));
	}


	SML_CALL_METHOD(DestroyMember, &pIMyProp->SML_INTERFACE(SML_IObject)); //destroy resources

	//SML_CALL_METHOD(Release, &pIMyProp->SML_INTERFACE(SML_IObject)); //ref count --, zero reached , destroy resources (again) by calling DestroyMember(...) and free object itself

	SML_RESOURCE_CLEANUP;

	return 0;
}

static void WINAPI some_cleanup_func(int ii)
{
	_ftprintf_s(stdout, L"cleanup %d" L"\r\n", ii);
}

int _tmain_test_003_res_no_cleanup(INT argc, TCHAR ** argv)
{
	SML_RESOURCE_INIT;

	int index = SML_RESOURCE_ADD_STDCALL_1(some_cleanup_func, 100);
	int index1 = SML_RESOURCE_ADD_STDCALL_1(some_cleanup_func, 200);

	SML_RESOURCE_NO_CLEANUP_ITEM(index);

	SML_RESOURCE_CLEANUP;

	return 0;
}

int _tmain_case_001_resoure_mgr(INT argc, TCHAR ** argv)
{
	//struct SML_ResourceList resList; 
	//SML_OBJ_FUNC_1(SML_ResourceList, Init)(&resList);
	SML_RESOURCE_INIT;

	for (int ii = 0; ii < 16; ++ ii)
	{
		//SML_OBJ_FUNC_1(SML_ResourceList, Add)(&resList, SML_RCM_WINAPI_FUNC, (void*)(ii), (void*)(some_cleanup_func));
		SML_RESOURCE_ADD_STDCALL_1(some_cleanup_func, ii);
	}


	//////////////////////////////////////////////////////////////////////////
	CoInitializeEx(NULL, COINITBASE_MULTITHREADED);
	SML_RESOURCE_ADD_0(CoUninitialize);


	//////////////////////////////////////////////////////////////////////////
	FILE * fd = NULL;
	_wfopen_s(
		&fd,
		TEXT("\\\\?\\C:"),//const wchar_t *filename,
		TEXT("r")//,//const wchar_t *mode
		);
	if (fd)
	{
		SML_RESOURCE_ADD_1(fclose, fd);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE hFile = CreateFile(
		TEXT("\\\\?\\C:"),//_In_     LPCTSTR               lpFileName,
		GENERIC_READ,//_In_     DWORD                 dwDesiredAccess,
		FILE_SHARE_READ | FILE_SHARE_WRITE,//_In_     DWORD                 dwShareMode,
		NULL,//_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		OPEN_EXISTING,//_In_     DWORD                 dwCreationDisposition,
		0,//_In_     DWORD                 dwFlagsAndAttributes,
		NULL//,//_In_opt_ HANDLE                hTemplateFile
		);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		SML_RESOURCE_ADD_STDCALL_1(CloseHandle, hFile);
	}


	//////////////////////////////////////////////////////////////////////////
	WSADATA wSAData = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wSAData);
	SML_RESOURCE_ADD_0(WSACleanup);


	//////////////////////////////////////////////////////////////////////////
	struct  SML_IObject * pIMyObject = NULL;
	pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_MALLOC)(1024);
	SML_RESOURCE_ADD_1(pIMyObject->Release, pIMyObject);

	//////////////////////////////////////////////////////////////////////////
	void * mem1 = HeapAlloc(GetProcessHeap(), 0, 100);
	SML_RESOURCE_ADD_STDCALL_3(HeapFree, GetProcessHeap(), 0, mem1);

	void * mem2 = VirtualAlloc(NULL, 200, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	SML_RESOURCE_ADD_STDCALL_3(VirtualFree, mem2, 0, MEM_RELEASE);
	
	//SML_OBJ_FUNC_1(SML_ResourceList, Cleanup)(&resList);
	SML_RESOURCE_CLEANUP;

	return 0;
}


int _tmain_case_000_iinc(INT argc, TCHAR ** argv)
{
	struct  SML_IObject * pIMyObject = NULL; //create instance, ref count  ++

	int allocMehtod = SAM_VIRTUAL_ALLOC;
	switch (allocMehtod)
	{
	case SAM_AUTO:
	{
		INT size = SML_OBJ_FUNC_1(MyNameObj, SizeOf)();
		VOID * addr = _alloca(size);
		pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstanceStack_SAM_AUTO)(addr, size, 1024);
	}
	break;
	case SAM_MALLOC:
	{
		pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_MALLOC)(1024);
	}
	break;
	case SAM_HEAP_ALLOC:
	{
		pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_HEAP_ALLOC)(1024);
	}
	break;
	case SAM_VIRTUAL_ALLOC:
	{
		pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_VIRTUAL_ALLOC)(1024);
	}
	break;
	case SAM__MALLOCA:
	{
		pIMyObject = SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM__MALLOCA)(1024);
	}
	break;
	
	}

	

	struct IMyProp * pIMyProp = SML_CALL_METHOD(QueryInterface, pIMyObject, SML_NAME_OF(IMyProp)); //QueryInterface, ref count ++

	{
		struct SML_IObject * pIMyObject1 = SML_CALL_METHOD(QueryInterface, pIMyObject, SML_NAME_OF(SML_IObject));//QueryInterface, ref count ++

		SML_CALL_METHOD(Release, pIMyObject); //ref count --
		pIMyObject = NULL;

		SML_CALL_METHOD(Release, pIMyObject1); //ref count --
		pIMyObject1 = NULL;
	}
	

	SML_CALL_METHOD(CreateMember, &pIMyProp->SML_INTERFACE(SML_IObject)); //create resources 

	SML_CALL_METHOD(SetName, pIMyProp, TEXT("Iron Mike Tyson")); //object operation

	WCHAR name[256] = { 0 };
	_tcscpy_s(name, _countof(name), SML_CALL_METHOD(GetName, pIMyProp)); //object operation N...

	{
		struct IMyMethod * pIMyMethod = SML_CALL_METHOD(QueryInterface, &pIMyProp->SML_INTERFACE(SML_IObject), SML_NAME_OF(IMyMethod));
		SML_CALL_METHOD(SayHi, pIMyMethod);
		SML_CALL_METHOD(SayBye, pIMyMethod);

		{
			struct SML_IObject * ptemp1 = SML_CALL_METHOD(QueryInterface, &pIMyMethod->SML_INTERFACE(SML_IObject), SML_NAME_OF(SML_IObject));
			SML_CALL_METHOD(Release, ptemp1);

			struct IMyProp * ptemp2 = SML_CALL_METHOD(QueryInterface, &pIMyMethod->SML_INTERFACE(SML_IObject), SML_NAME_OF(IMyProp));
			SML_CALL_METHOD(Release, &ptemp2->SML_INTERFACE(SML_IObject));

			struct IMyMethod * ptemp3 = SML_CALL_METHOD(QueryInterface, &pIMyMethod->SML_INTERFACE(SML_IObject), SML_NAME_OF(IMyMethod));
			SML_CALL_METHOD(Release, &ptemp3->SML_INTERFACE(SML_IObject));
		}
		

		SML_CALL_METHOD(Release, &pIMyMethod->SML_INTERFACE(SML_IObject));
	}
	

	SML_CALL_METHOD(DestroyMember, &pIMyProp->SML_INTERFACE(SML_IObject)); //destroy resources

	SML_CALL_METHOD(Release, &pIMyProp->SML_INTERFACE(SML_IObject)); //ref count --, zero reached , destroy resources (again) by calling DestroyMember(...) and free object itself

	return 0;
}



static void Cleanup_0()
{
	_ftprintf_s(stdout, TEXT("%s  ") TEXT("\r\n"), TEXT(__FUNCTION__));
}

static void Cleanup_1(void * p1)
{
	_ftprintf_s(stdout, TEXT("%s %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1);
}
static void Cleanup_2(void * p1, void * p2)
{
	_ftprintf_s(stdout, TEXT("%s %p %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1, p2);
}
static void Cleanup_3(void * p1, void * p2, void * p3)
{
	_ftprintf_s(stdout, TEXT("%s %p %p %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1, p2, p3);
}
static void Cleanup_4(void * p1, void * p2, void * p3, void * p4)
{
	_ftprintf_s(stdout, TEXT("%s %p %p %p %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1, p2, p3, p4);
}
static void Cleanup_5(void * p1, void * p2, void * p3, void * p4, void * p5)
{
	_ftprintf_s(stdout, TEXT("%s %p %p %p %p %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1, p2, p3, p4, p5);
}
static void Cleanup_6(void * p1, void * p2, void * p3, void * p4, void * p5, void * p6)
{
	_ftprintf_s(stdout, TEXT("%s %p %p %p %p %p %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1, p2, p3, p4, p5, p6);
}


static void WINAPI CleanupStdcall_1(void * p1)
{
	_ftprintf_s(stdout, TEXT("%s %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1);
}
static void WINAPI CleanupStdcall_2(void * p1, void * p2)
{
	_ftprintf_s(stdout, TEXT("%s %p %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1, p2);
}
static void WINAPI CleanupStdcall_3(void * p1, void * p2, void * p3)
{
	_ftprintf_s(stdout, TEXT("%s %p %p %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1, p2, p3);
}
static void WINAPI CleanupStdcall_4(void * p1, void * p2, void * p3, void * p4)
{
	_ftprintf_s(stdout, TEXT("%s %p %p %p %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1, p2, p3, p4);
}
static void WINAPI CleanupStdcall_5(void * p1, void * p2, void * p3, void * p4, void * p5)
{
	_ftprintf_s(stdout, TEXT("%s %p %p %p %p %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1, p2, p3, p4, p5);
}
static void WINAPI CleanupStdcall_6(void * p1, void * p2, void * p3, void * p4, void * p5, void * p6)
{
	_ftprintf_s(stdout, TEXT("%s %p %p %p %p %p %p ") TEXT("\r\n"), TEXT(__FUNCTION__), p1, p2, p3, p4, p5, p6);
}

int _tmain_case_001_res_mgr_again(int argc, TCHAR ** argv)
{
	SML_RESOURCE_INIT;

	SML_RESOURCE_ADD_0(Cleanup_0);
	SML_RESOURCE_ADD_1(Cleanup_1, 1);
	SML_RESOURCE_ADD_2(Cleanup_2, 1, 2);
	SML_RESOURCE_ADD_3(Cleanup_3, 1, 2, 3);
	SML_RESOURCE_ADD_4(Cleanup_4, 1, 2, 3, 4);
	SML_RESOURCE_ADD_5(Cleanup_5, 1, 2, 3, 4, 5);
	SML_RESOURCE_ADD_6(Cleanup_6, 1, 2, 3, 4, 5, 6);


	SML_RESOURCE_ADD_STDCALL_1(CleanupStdcall_1, 1);
	SML_RESOURCE_ADD_STDCALL_2(CleanupStdcall_2, 1, 2);
	SML_RESOURCE_ADD_STDCALL_3(CleanupStdcall_3, 1, 2, 3);
	SML_RESOURCE_ADD_STDCALL_4(CleanupStdcall_4, 1, 2, 3, 4);
	SML_RESOURCE_ADD_STDCALL_5(CleanupStdcall_5, 1, 2, 3, 4, 5);
	SML_RESOURCE_ADD_STDCALL_6(CleanupStdcall_6, 1, 2, 3, 4, 5, 6);

	SML_RESOURCE_CLEANUP;
	return 0;
}