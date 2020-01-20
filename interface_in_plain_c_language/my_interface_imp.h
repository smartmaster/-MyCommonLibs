#pragma once

#include <tchar.h>
#include <windows.h>

#include "my_interface.h"



struct MyNameObj
{
	volatile LONG m_refCount;
	SML_INTERFACE_DEFINE(IMyProp); //do not matter where the interface is put
	INT m_allocMethod;
	SML_INTERFACE_DEFINE(IMyMethod); //do not matter where the interface is put
	INT m_bufferSize;
	LPTSTR m_name; //some resource

};

HRESULT SML_OBJ_FUNC_1(MyNameObj, InitObj)(struct MyNameObj * pObj, enum SML_ALLOC_METHOD allocMethod, INT bufferSize);


//////////////////////////////////////////////////////////////////////////
HRESULT SML_OBJ_FUNC_1(MyNameObj, SetName)(struct MyNameObj *_this, LPCTSTR name);
LPCTSTR SML_OBJ_FUNC_1(MyNameObj, GetName)(struct MyNameObj *_this);

HRESULT SML_OBJ_FUNC_1(MyNameObj, CreateMember)(struct MyNameObj * _this);
HRESULT SML_OBJ_FUNC_1(MyNameObj, DestroyMember)(struct MyNameObj * _this);
LONG SML_OBJ_FUNC_1(MyNameObj, AddRef)(struct MyNameObj * _this);
LONG SML_OBJ_FUNC_1(MyNameObj, Release)(struct MyNameObj * _this);
void * SML_OBJ_FUNC_1(MyNameObj, QueryInterface)(struct MyNameObj * _this, LPCSTR name);
//////////////////////////////////////////////////////////////////////////

HRESULT SML_OBJ_FUNC_2(MyNameObj, IMyProp, SetName)(struct IMyProp *_this, LPCTSTR name);
LPCTSTR SML_OBJ_FUNC_2(MyNameObj, IMyProp, GetName)(struct IMyProp *_this);

HRESULT SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, CreateMember)(struct SML_IObject * _this);
HRESULT SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, DestroyMember)(struct SML_IObject * _this);
LONG SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, AddRef)(struct SML_IObject * _this);
LONG SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, Release)(struct SML_IObject * _this);
void * SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, QueryInterface)(struct SML_IObject * _this, LPCSTR name);



//////////////////////////////////////////////////////////////////////////
HRESULT SML_OBJ_FUNC_1(MyNameObj, SayHi)(struct MyNameObj * _this);
HRESULT SML_OBJ_FUNC_1(MyNameObj, SayBye)(struct MyNameObj * _this);
//////////////////////////////////////////////////////////////////////////

HRESULT SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, CreateMember)(struct SML_IObject * _this);
HRESULT SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, DestroyMember)(struct SML_IObject * _this);
LONG SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, AddRef)(struct SML_IObject * _this);
LONG SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, Release)(struct SML_IObject * _this);
void * SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, QueryInterface)(struct SML_IObject * _this, LPCSTR name);
HRESULT SML_OBJ_FUNC_2(MyNameObj, IMyMethod, SayHi)(struct IMyMethod * _this);
HRESULT SML_OBJ_FUNC_2(MyNameObj, IMyMethod, SayBye)(struct IMyMethod * _this);
//};
