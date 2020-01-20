#pragma once

#include <tchar.h>
#include <windows.h>

//////////////////////////////////////////////////////////////////////////
#define  SML_NAME_OF(x)	# x

//////////////////////////////////////////////////////////////////////////
#define  SML_OBJ_FUNC_1(obj, func)																SmartLib_ ## obj ## _ ## func
#define  SML_OBJ_FUNC_2(obj, type2, func)														SmartLib_ ## obj ## _ ## type2 ## _ ## func
#define  SML_OBJ_FUNC_3(obj, type2, type3, func)												SmartLib_ ## obj ## _ ## type2 ## _ ## type3 ## _ ## func
#define  SML_OBJ_FUNC_4(obj, type2, type3, type4, func)											SmartLib_ ## obj ## _ ## type2 ## _ ## type3 ## _ ## type4 ## _ ## func
#define  SML_OBJ_FUNC_5(obj, type2, type3, type4, type5, func)									SmartLib_ ## obj ## _ ## type2 ## _ ## type3 ## _ ## type4 ## _ ## type5 ## _ ## func
#define  SML_OBJ_FUNC_6(obj, type2, type3, type4, type5, type6, func)							SmartLib_ ## obj ## _ ## type2 ## _ ## type3 ## _ ## type4 ## _ ## type5 ## _ ## type6 ## _ ## func
#define  SML_OBJ_FUNC_7(obj, type2, type3, type4, type5, type6, type7, func)					SmartLib_ ## obj ## _ ## type2 ## _ ## type3 ## _ ## type4 ## _ ## type5 ## _ ## type6 ## _ ## type7 ## _ ## func
#define  SML_OBJ_FUNC_8(obj, type2, type3, type4, type5, type6, type7, type8, func)				SmartLib_ ## obj ## _ ## type2 ## _ ## type3 ## _ ## type4 ## _ ## type5 ## _ ## type6 ## _ ## type7 ## _ ## type8 ## _ ## func
#define  SML_OBJ_FUNC_9(obj, type2, type3, type4, type5, type6, type7, type8, type9, func)		SmartLib_ ## obj ## _ ## type2 ## _ ## type3 ## _ ## type4 ## _ ## type5 ## _ ## type6 ## _ ## type7 ## _ ## type8 ## _ ## type9 ## _ ## func
//add more if needed

//////////////////////////////////////////////////////////////////////////
//object resource management functions
//implement first
#define SML_OBJ_FUNC_ALLOC_SPACE(obj)		SML_OBJ_FUNC_1(obj, AllocSpace)
#define SML_OBJ_FUNC_INIT_MEMBER(obj)		SML_OBJ_FUNC_1(obj, InitMember)
#define SML_OBJ_FUNC_CREATE_MEMBER(obj)		SML_OBJ_FUNC_1(obj, CreateMember)
#define SML_OBJ_FUNC_DESTROY_MEMBER(obj)	SML_OBJ_FUNC_1(obj, DestroyMember)
#define SML_OBJ_FUNC_FREE_SPACE(obj)		SML_OBJ_FUNC_1(obj, FreeSpace)

#define SML_OBJ_FUNC_ADD_REF(obj)			SML_OBJ_FUNC_1(obj, AddRef)
#define SML_OBJ_FUNC_RELEASE(obj)			SML_OBJ_FUNC_1(obj, Release)
#define SML_OBJ_FUNC_QUERY_INTERFACE(obj)	SML_OBJ_FUNC_1(obj, QueryInterface)

#define SML_OBJ_FUNC_CRAETE_INSTANCE(obj)	SML_OBJ_FUNC_1(obj, CraeteInstance)


//////////////////////////////////////////////////////////////////////////
#define  SML_FUNC_TABLE(type_name, interface_name)				SmartLib_ ## type_name ## _ ## interface_name ## _ ## Table
#define  SML_FUNC_TABLE_DEFINE(type_name, interface_name)		const struct interface_name SML_FUNC_TABLE(type_name, interface_name)

//////////////////////////////////////////////////////////////////////////
#define  SML_INTERFACE(interface_type)							m_smart_lib_interface_ ## interface_type
#define  SML_INTERFACE_DEFINE(interface_type)					struct interface_type SML_INTERFACE(interface_type)


//////////////////////////////////////////////////////////////////////////
#define SML_CALL_METHOD(method, interface_ptr, ...)				(interface_ptr) -> method ((interface_ptr), __VA_ARGS__)

#define SML_GET_OBJECT_2(type0, type1, ptr)														(struct type0 *)((CHAR*)(ptr) - (FIELD_OFFSET(struct type0, SML_INTERFACE(type1))))
#define SML_GET_OBJECT_3(type0, type1, type2, ptr)												(struct type0 *)((CHAR*)(ptr) - (FIELD_OFFSET(struct type0, SML_INTERFACE(type1)) + FIELD_OFFSET(struct type1, SML_INTERFACE(type2))))
#define SML_GET_OBJECT_4(type0, type1, type2, type3, ptr)										(struct type0 *)((CHAR*)(ptr) - (FIELD_OFFSET(struct type0, SML_INTERFACE(type1)) + FIELD_OFFSET(struct type1, SML_INTERFACE(type2)) + FIELD_OFFSET(struct type2, SML_INTERFACE(type3))))
#define SML_GET_OBJECT_5(type0, type1, type2, type3, type4, ptr)								(struct type0 *)((CHAR*)(ptr) - (FIELD_OFFSET(struct type0, SML_INTERFACE(type1)) + FIELD_OFFSET(struct type1, SML_INTERFACE(type2)) + FIELD_OFFSET(struct type2, SML_INTERFACE(type3)) + FIELD_OFFSET(struct type3, SML_INTERFACE(type4))))
#define SML_GET_OBJECT_6(type0, type1, type2, type3, type4, type5, ptr)							(struct type0 *)((CHAR*)(ptr) - (FIELD_OFFSET(struct type0, SML_INTERFACE(type1)) + FIELD_OFFSET(struct type1, SML_INTERFACE(type2)) + FIELD_OFFSET(struct type2, SML_INTERFACE(type3)) + FIELD_OFFSET(struct type3, SML_INTERFACE(type4)) + FIELD_OFFSET(struct type4, SML_INTERFACE(type5))))
#define SML_GET_OBJECT_7(type0, type1, type2, type3, type4, type5, type6, ptr)					(struct type0 *)((CHAR*)(ptr) - (FIELD_OFFSET(struct type0, SML_INTERFACE(type1)) + FIELD_OFFSET(struct type1, SML_INTERFACE(type2)) + FIELD_OFFSET(struct type2, SML_INTERFACE(type3)) + FIELD_OFFSET(struct type3, SML_INTERFACE(type4)) + FIELD_OFFSET(struct type4, SML_INTERFACE(type5))+ FIELD_OFFSET(struct type5, SML_INTERFACE(type6))))
#define SML_GET_OBJECT_8(type0, type1, type2, type3, type4, type5, type6, type7, ptr)			(struct type0 *)((CHAR*)(ptr) - (FIELD_OFFSET(struct type0, SML_INTERFACE(type1)) + FIELD_OFFSET(struct type1, SML_INTERFACE(type2)) + FIELD_OFFSET(struct type2, SML_INTERFACE(type3)) + FIELD_OFFSET(struct type3, SML_INTERFACE(type4)) + FIELD_OFFSET(struct type4, SML_INTERFACE(type5))+ FIELD_OFFSET(struct type5, SML_INTERFACE(type6))+ FIELD_OFFSET(struct type6, SML_INTERFACE(type7))))
#define SML_GET_OBJECT_9(type0, type1, type2, type3, type4, type5, type6, type7, type8, ptr)	(struct type0 *)((CHAR*)(ptr) - (FIELD_OFFSET(struct type0, SML_INTERFACE(type1)) + FIELD_OFFSET(struct type1, SML_INTERFACE(type2)) + FIELD_OFFSET(struct type2, SML_INTERFACE(type3)) + FIELD_OFFSET(struct type3, SML_INTERFACE(type4)) + FIELD_OFFSET(struct type4, SML_INTERFACE(type5))+ FIELD_OFFSET(struct type5, SML_INTERFACE(type6))+ FIELD_OFFSET(struct type6, SML_INTERFACE(type7))+ FIELD_OFFSET(struct type7, SML_INTERFACE(type8))))
//add more if needed

//////////////////////////////////////////////////////////////////////////
#define  SML_ALLOC_OBJ(obj_type)	(struct obj_type *)malloc(sizeof(struct obj_type))

enum SML_ALLOC_METHOD
{
	SAM_AUTO,
	SAM_MALLOC,
	SAM_HEAP_ALLOC,
	SAM_VIRTUAL_ALLOC,
	SAM__MALLOCA
};

