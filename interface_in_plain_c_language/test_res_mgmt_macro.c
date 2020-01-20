#include "stdafx.h"
#include "test_res_mgmt_macro.h"

//#define SML_OBJ_FUNC_ALLOC_SPACE(obj)		SML_OBJ_FUNC_1(obj, AllocSpace)
struct MyResObj * SML_OBJ_FUNC_ALLOC_SPACE(MyResObj)()
{
	return (struct MyResObj*)malloc(sizeof(struct MyResObj));
}
//#define SML_OBJ_FUNC_INIT_MEMBER(obj)		SML_OBJ_FUNC_1(obj, InitMember)
void SML_OBJ_FUNC_INIT_MEMBER(MyResObj)(struct MyResObj * obj)
{
	ZeroMemory(obj, sizeof(struct MyResObj));
	obj->m_refCount = 0;
	obj->m_len = 0;
	obj->m_someRes = NULL;
}
//#define SML_OBJ_FUNC_CREATE_MEMBER(obj)		SML_OBJ_FUNC_1(obj, CreateMember)
HRESULT SML_OBJ_FUNC_CREATE_MEMBER(MyResObj)(struct MyResObj * obj, int len)
{
	obj->m_len = len;
	obj->m_someRes = (int *)malloc(sizeof(int) * len);
	return S_OK;
}

//#define SML_OBJ_FUNC_DESTROY_MEMBER(obj)	SML_OBJ_FUNC_1(obj, DestroyMember)
void SML_OBJ_FUNC_DESTROY_MEMBER(MyResObj)(struct MyResObj * obj)
{
	if (obj->m_someRes)
	{
		free(obj->m_someRes);
		obj->m_someRes = NULL;
	}
}
//#define SML_OBJ_FUNC_FREE_SPACE(obj)		SML_OBJ_FUNC_1(obj, FreeSpace)
void SML_OBJ_FUNC_FREE_SPACE(MyResObj)(struct MyResObj * obj)
{
	free(obj);
}



//#define SML_OBJ_FUNC_ADD_REF(obj)			SML_OBJ_FUNC_1(obj, AddRef)
long SML_OBJ_FUNC_ADD_REF(MyResObj)(struct MyResObj * obj)
{
	return InterlockedIncrement(&obj->m_refCount);
}
//#define SML_OBJ_FUNC_QUERY_INTERFACE(obj)	SML_OBJ_FUNC_1(obj, QueryInterface)
void * SML_OBJ_FUNC_QUERY_INTERFACE(MyResObj)(struct MyResObj * obj, const WCHAR * name)
{
	SML_OBJ_FUNC_ADD_REF(MyResObj)(obj);
	return obj;
}
//#define SML_OBJ_FUNC_RELEASE(obj)			SML_OBJ_FUNC_1(obj, Release)
long SML_OBJ_FUNC_RELEASE(MyResObj)(struct MyResObj * obj)
{
	long refCount = InterlockedDecrement(&obj->m_refCount);
	if (0 == refCount)
	{
		SML_OBJ_FUNC_DESTROY_MEMBER(MyResObj)(obj);
		SML_OBJ_FUNC_FREE_SPACE(MyResObj)(obj);
	}
	return refCount;
}


struct MyResObj * SML_OBJ_FUNC_CRAETE_INSTANCE(MyResObj)(int len, BOOL bCreateMember)
{
	struct MyResObj * obj = SML_OBJ_FUNC_ALLOC_SPACE(MyResObj)();
	SML_OBJ_FUNC_INIT_MEMBER(MyResObj)(obj);
	SML_OBJ_FUNC_ADD_REF(MyResObj)(obj);
	if (bCreateMember)
	{
		SML_OBJ_FUNC_CREATE_MEMBER(MyResObj)(obj, len);
	}
	return obj;
}



void SML_OBJ_FUNC_1(MyResObj, Test)()
{
	struct MyResObj * obj = SML_OBJ_FUNC_ALLOC_SPACE(MyResObj)();
	SML_OBJ_FUNC_INIT_MEMBER(MyResObj)(obj);
	SML_OBJ_FUNC_CREATE_MEMBER(MyResObj)(obj, 100);
	SML_OBJ_FUNC_DESTROY_MEMBER(MyResObj)(obj);
	SML_OBJ_FUNC_FREE_SPACE(MyResObj)(obj);
}


void SML_OBJ_FUNC_1(MyResObj, Test2)()
{
	struct MyResObj * obj = SML_OBJ_FUNC_CRAETE_INSTANCE(MyResObj)(100, TRUE);
	SML_OBJ_FUNC_RELEASE(MyResObj)(obj);
}