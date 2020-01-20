#pragma once


#include <tchar.h>
#include <windows.h>

#include "sml_interface_common.h"
#include "sml_iobject.h"



struct IMyProp
{
	HRESULT(*SetName)(struct IMyProp *_this, LPCTSTR name);
	SML_INTERFACE_DEFINE(SML_IObject); //do not matter where the interface is put
	LPCTSTR(*GetName)(struct IMyProp *_this);
};

struct IMyMethod
{
	HRESULT (*SayHi)(struct IMyMethod * _this);
	SML_INTERFACE_DEFINE(SML_IObject); //do not matter where the interface is put
	HRESULT(*SayBye)(struct IMyMethod * _this);
};


INT SML_OBJ_FUNC_1(MyNameObj, SizeOf)();
struct SML_IObject * SML_OBJ_FUNC_1(MyNameObj, CreateInstanceStack_SAM_AUTO)(VOID * addr, INT size, INT bufferSize);

struct SML_IObject * SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_MALLOC)(INT bufferSize);
struct SML_IObject * SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_HEAP_ALLOC)(INT bufferSize);
struct SML_IObject * SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_VIRTUAL_ALLOC)(INT bufferSize);
struct SML_IObject * SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM__MALLOCA)(INT bufferSize);

