#pragma once

#include <tchar.h>
#include <windows.h>

#include <stdlib.h>
#include <malloc.h>

#include "sml_interface_common.h"

typedef void(*SML_pfnCleanup_0)();

typedef void(*SML_pfnCleanup_1)(void * p1);
typedef void(*SML_pfnCleanup_2)(void * p1, void * p2);
typedef void(*SML_pfnCleanup_3)(void * p1, void * p2, void * p3);
typedef void(*SML_pfnCleanup_4)(void * p1, void * p2, void * p3, void * p4);
typedef void(*SML_pfnCleanup_5)(void * p1, void * p2, void * p3, void * p4, void * p5);
typedef void(*SML_pfnCleanup_6)(void * p1, void * p2, void * p3, void * p4, void * p5, void * p6);


typedef void(WINAPI *SML_pfnCleanupStdcall_1)(void * p1);
typedef void(WINAPI *SML_pfnCleanupStdcall_2)(void * p1, void * p2);
typedef void(WINAPI *SML_pfnCleanupStdcall_3)(void * p1, void * p2, void * p3);
typedef void(WINAPI *SML_pfnCleanupStdcall_4)(void * p1, void * p2, void * p3, void * p4);
typedef void(WINAPI *SML_pfnCleanupStdcall_5)(void * p1, void * p2, void * p3, void * p4, void * p5);
typedef void(WINAPI *SML_pfnCleanupStdcall_6)(void * p1, void * p2, void * p3, void * p4, void * p5, void * p6);

#define SML_NULL_1 NULL
#define SML_NULL_2 NULL
#define SML_NULL_3 NULL
#define SML_NULL_4 NULL
#define SML_NULL_5 NULL
#define SML_NULL_6 NULL

enum SML_RESOURCE_CLEANUP_METHOD
{
	SML_RCM_NULL = 0,
	SML_RCM_FUNC_0,
	SML_RCM_FUNC_1,
	SML_RCM_FUNC_2,
	SML_RCM_FUNC_3,
	SML_RCM_FUNC_4,
	SML_RCM_FUNC_5,
	SML_RCM_FUNC_6,
	SML_RCM_FUNC_STDCALL_1,
	SML_RCM_FUNC_STDCALL_2,
	SML_RCM_FUNC_STDCALL_3,
	SML_RCM_FUNC_STDCALL_4,
	SML_RCM_FUNC_STDCALL_5,
	SML_RCM_FUNC_STDCALL_6,
	SML_RCM_FUNC_PARAMS,
};



struct SML_Resource
{
	enum  SML_RESOURCE_CLEANUP_METHOD m_rcm;
	void * m_func;
	void * m_resource;
	void * m_resource2;
	void * m_resource3;
	void * m_resource4;
	void * m_resource5;
	void * m_resource6;
	const CHAR * m_atFile;
	const CHAR * m_atFunc;
	INT m_atLine;
};

void SML_OBJ_FUNC_1(SML_Resource, Cleanup)(struct SML_Resource * res);

#define SML_INITIAL_RESOURCE_COUNT 8

struct SML_ResourceList
{
	struct SML_Resource * m_list;
	struct SML_Resource m_stack[SML_INITIAL_RESOURCE_COUNT];
	LONG m_count;
	LONG m_capacity;
};

void SML_OBJ_FUNC_1(SML_ResourceList, Init)(struct SML_ResourceList * resList);
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
	INT atLine);
void SML_OBJ_FUNC_1(SML_ResourceList, NoCleanupItem)(
	struct SML_ResourceList * resList,
	int index
	);
void SML_OBJ_FUNC_1(SML_ResourceList, Cleanup)(struct SML_ResourceList * resList);

#define  SML_RESOURCE_INIT 	\
struct SML_ResourceList sml_resourcelist; \
SML_OBJ_FUNC_1(SML_ResourceList, Init)(&sml_resourcelist);


#define  SML_RESOURCE_ADD_0(func) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_0, (void*)(func), SML_NULL_1, SML_NULL_2, SML_NULL_3, SML_NULL_4, SML_NULL_5, SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_1(func, resource) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_1, (void*)(func), (void*)(resource), SML_NULL_2, SML_NULL_3, SML_NULL_4, SML_NULL_5, SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_2(func, resource, resource2) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_2, (void*)(func), (void*)(resource), (void*)(resource2), SML_NULL_3, SML_NULL_4, SML_NULL_5, SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_3(func, resource, resource2, resource3) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_3, (void*)(func), (void*)(resource), (void*)(resource2), (void*)(resource3), SML_NULL_4, SML_NULL_5, SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_4(func, resource, resource2, resource3, resource4) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_4, (void*)(func), (void*)(resource), (void*)(resource2), (void*)(resource3), (void*)(resource4), SML_NULL_5, SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_5(func, resource, resource2, resource3, resource4, resource5) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_5, (void*)(func), (void*)(resource), (void*)(resource2), (void*)(resource3), (void*)(resource4), (void*)(resource5), SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_6(func, resource, resource2, resource3, resource4, resource5, resource6) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_6, (void*)(func), (void*)(resource), (void*)(resource2), (void*)(resource3), (void*)(resource4), (void*)(resource5), (void*)(resource6), __FILE__, __FUNCTION__, __LINE__)

//////////////////////////////////////////////////////////////////////////
#define  SML_RESOURCE_ADD_STDCALL_1(func, resource) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_STDCALL_1, (void*)(func), (void*)(resource), SML_NULL_2, SML_NULL_3, SML_NULL_4, SML_NULL_5, SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_STDCALL_2(func, resource, resource2) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_STDCALL_2, (void*)(func), (void*)(resource), (void*)(resource2), SML_NULL_3, SML_NULL_4, SML_NULL_5, SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_STDCALL_3(func, resource, resource2, resource3) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_STDCALL_3, (void*)(func), (void*)(resource), (void*)(resource2), (void*)(resource3), SML_NULL_4, SML_NULL_5, SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_STDCALL_4(func, resource, resource2, resource3, resource4) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_STDCALL_4, (void*)(func), (void*)(resource), (void*)(resource2), (void*)(resource3), (void*)(resource4), SML_NULL_5, SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_STDCALL_5(func, resource, resource2, resource3, resource4, resource5) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_STDCALL_5, (void*)(func), (void*)(resource), (void*)(resource2), (void*)(resource3), (void*)(resource4), (void*)(resource5), SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)

#define  SML_RESOURCE_ADD_STDCALL_6(func, resource, resource2, resource3, resource4, resource5, resource6) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_STDCALL_6, (void*)(func), (void*)(resource), (void*)(resource2), (void*)(resource3), (void*)(resource4), (void*)(resource5), (void*)(resource6), __FILE__, __FUNCTION__, __LINE__)


#define SML_ALLOC_PARAMS(size)	_malloca(size)
#define SML_FREE_PARAMS(ptr)	_freea(ptr)

//func(resource)
//func_destroy_params_member(resource)
//SML_FREE_PARAMS(resource)
#define  SML_RESOURCE_ADD_PARAMS(func, resource, func_destroy_params_member) \
SML_OBJ_FUNC_1(SML_ResourceList, Add)(&sml_resourcelist, SML_RCM_FUNC_PARAMS, (void*)(func), (void*)(resource), (void*)(func_destroy_params_member), SML_NULL_3, SML_NULL_4, SML_NULL_5, SML_NULL_6, __FILE__, __FUNCTION__, __LINE__)


#define  SML_RESOURCE_NO_CLEANUP_ITEM(index) \
SML_OBJ_FUNC_1(SML_ResourceList, NoCleanupItem)(&sml_resourcelist, index)


#define SML_RESOURCE_CLEANUP \
SML_OBJ_FUNC_1(SML_ResourceList, Cleanup)(&sml_resourcelist)