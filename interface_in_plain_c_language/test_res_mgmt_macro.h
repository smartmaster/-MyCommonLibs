#pragma once

#include "sml_interface_common.h"

struct MyResObj
{
	long m_refCount;
	int m_len;
	int * m_someRes;
};

//#define SML_OBJ_FUNC_ALLOC_SPACE(obj)		SML_OBJ_FUNC_1(obj, AllocSpace)
struct MyResObj * SML_OBJ_FUNC_ALLOC_SPACE(MyResObj)();
//#define SML_OBJ_FUNC_INIT_MEMBER(obj)		SML_OBJ_FUNC_1(obj, InitMember)
void SML_OBJ_FUNC_INIT_MEMBER(MyResObj)(struct MyResObj * obj);
//#define SML_OBJ_FUNC_CREATE_MEMBER(obj)		SML_OBJ_FUNC_1(obj, CreateMember)
HRESULT SML_OBJ_FUNC_CREATE_MEMBER(MyResObj)(struct MyResObj * obj, int len);
//#define SML_OBJ_FUNC_DESTROY_MEMBER(obj)	SML_OBJ_FUNC_1(obj, DestroyMember)
void SML_OBJ_FUNC_DESTROY_MEMBER(MyResObj)(struct MyResObj * obj);
//#define SML_OBJ_FUNC_FREE_SPACE(obj)		SML_OBJ_FUNC_1(obj, FreeSpace)
void SML_OBJ_FUNC_FREE_SPACE(MyResObj)(struct MyResObj * obj);


//#define SML_OBJ_FUNC_ADD_REF(obj)			SML_OBJ_FUNC_1(obj, AddRef)
long SML_OBJ_FUNC_ADD_REF(MyResObj)(struct MyResObj * obj);
//#define SML_OBJ_FUNC_RELEASE(obj)			SML_OBJ_FUNC_1(obj, Release)
long SML_OBJ_FUNC_RELEASE(MyResObj)(struct MyResObj * obj);
//#define SML_OBJ_FUNC_QUERY_INTERFACE(obj)	SML_OBJ_FUNC_1(obj, QueryInterface)
void * SML_OBJ_FUNC_QUERY_INTERFACE(MyResObj)(struct MyResObj * obj, const WCHAR * name);

//#define SML_OBJ_FUNC_CRAETE_INSTANCE(obj)	SML_OBJ_FUNC_1(obj, CraeteInstance);
struct MyResObj * SML_OBJ_FUNC_CRAETE_INSTANCE(MyResObj)(int len, BOOL bCreateMember);


void SML_OBJ_FUNC_1(MyResObj, Test)();
void SML_OBJ_FUNC_1(MyResObj, Test2)();