#include "stdafx.h"

#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "my_interface_imp.h"


static SML_FUNC_TABLE_DEFINE(MyNameObj, IMyProp) =
{
	//struct IMyProp
	//{
	SML_OBJ_FUNC_2(MyNameObj, IMyProp, SetName),//	HRESULT(*SetName)(struct IMyProp *_this, LPCTSTR name);

	{
		SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, CreateMember),//	//	HRESULT(*CreateMember)(struct IMyObject * _this);
		SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, DestroyMember),//	//	HRESULT(*DestroyMember)(struct IMyObject * _this);
		SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, AddRef),//	//	LONG(*AddRef)(struct IMyObject * _this);
		SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, Release),//	//	LONG(*Release)(struct IMyObject * _this);
		SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, QueryInterface)
	},

	SML_OBJ_FUNC_2(MyNameObj, IMyProp, GetName),//	LPCTSTR(*GetName)(struct IMyProp *_this);
};

static SML_FUNC_TABLE_DEFINE(MyNameObj, IMyMethod) = 
{
	SML_OBJ_FUNC_2(MyNameObj, IMyMethod, SayHi),

	{
		SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, CreateMember),
		SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, DestroyMember),
		SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, AddRef),
		SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, Release),
		SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, QueryInterface)
	},

	SML_OBJ_FUNC_2(MyNameObj, IMyMethod, SayBye)
};

INT SML_OBJ_FUNC_1(MyNameObj, SizeOf)()
{
	return sizeof(struct MyNameObj);
}

struct SML_IObject * SML_OBJ_FUNC_1(MyNameObj, CreateInstanceStack_SAM_AUTO)(VOID * addr, INT size, INT bufferSize)
{
	if (size < SML_OBJ_FUNC_1(MyNameObj, SizeOf)())
	{
		return NULL;
	}

	struct MyNameObj * pObj = (struct MyNameObj *)(addr); //allocate memory
	SML_OBJ_FUNC_1(MyNameObj, InitObj)(pObj, SAM_AUTO, bufferSize); //init fields and interface table. from now on (after obj created and initialized) call interface method only 
	struct SML_IObject * pIMyObject = &(pObj->SML_INTERFACE(IMyProp).SML_INTERFACE(SML_IObject));
	SML_CALL_METHOD(AddRef, pIMyObject); //do not forget to increment reference count
	return pIMyObject;
}


struct SML_IObject * SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_MALLOC)(int bufferSize)
{
	struct MyNameObj * pObj = SML_ALLOC_OBJ(MyNameObj); //allocate memory
	SML_OBJ_FUNC_1(MyNameObj, InitObj)(pObj, SAM_MALLOC, bufferSize); //init fields and interface table. from now on (after obj created and initialized) call interface method only 
	struct SML_IObject * pIMyObject = &(pObj->SML_INTERFACE(IMyProp).SML_INTERFACE(SML_IObject));
	SML_CALL_METHOD(AddRef, pIMyObject); //do not forget to increment reference count
	return pIMyObject;
}


struct SML_IObject * SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_HEAP_ALLOC)(INT bufferSize)
{
	struct MyNameObj * pObj = (struct MyNameObj *)HeapAlloc(GetProcessHeap(), 0, sizeof(struct MyNameObj)); //allocate memory
	SML_OBJ_FUNC_1(MyNameObj, InitObj)(pObj, SAM_HEAP_ALLOC, bufferSize); //init fields and interface table. from now on (after obj created and initialized) call interface method only 
	struct SML_IObject * pIMyObject = &(pObj->SML_INTERFACE(IMyProp).SML_INTERFACE(SML_IObject));
	SML_CALL_METHOD(AddRef, pIMyObject); //do not forget to increment reference count
	return pIMyObject;
}
struct SML_IObject * SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM_VIRTUAL_ALLOC)(INT bufferSize)
{
	struct MyNameObj * pObj = (struct MyNameObj *)VirtualAlloc(NULL, sizeof(struct MyNameObj), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); //allocate memory
	SML_OBJ_FUNC_1(MyNameObj, InitObj)(pObj, SAM_VIRTUAL_ALLOC, bufferSize); //init fields and interface table. from now on (after obj created and initialized) call interface method only 
	struct SML_IObject * pIMyObject = &(pObj->SML_INTERFACE(IMyProp).SML_INTERFACE(SML_IObject));
	SML_CALL_METHOD(AddRef, pIMyObject); //do not forget to increment reference count
	return pIMyObject;
}
struct SML_IObject * SML_OBJ_FUNC_1(MyNameObj, CreateInstance_SAM__MALLOCA)(INT bufferSize)
{
	struct MyNameObj * pObj = (struct MyNameObj *)_malloca(sizeof(struct MyNameObj)); //allocate memory
	SML_OBJ_FUNC_1(MyNameObj, InitObj)(pObj, SAM__MALLOCA, bufferSize); //init fields and interface table. from now on (after obj created and initialized) call interface method only 
	struct SML_IObject * pIMyObject = &(pObj->SML_INTERFACE(IMyProp).SML_INTERFACE(SML_IObject));
	SML_CALL_METHOD(AddRef, pIMyObject); //do not forget to increment reference count
	return pIMyObject;
}

//init interface table
HRESULT SML_OBJ_FUNC_1(MyNameObj, InitObj)(struct MyNameObj * pObj, enum SML_ALLOC_METHOD allocMethod, INT bufferSize)
{
	ZeroMemory(pObj, sizeof(struct MyNameObj));
	//pObj->m_refCount = 0;
	pObj->m_allocMethod = allocMethod;
	const INT MIN_BUFFER_SIZE = 4 * 1024 / sizeof(TCHAR);
	pObj->m_bufferSize = bufferSize >= MIN_BUFFER_SIZE ? bufferSize : MIN_BUFFER_SIZE; //some resource
																					   //pObj->m_name = NULL; //some resource
	pObj->SML_INTERFACE(IMyProp) = SML_FUNC_TABLE(MyNameObj, IMyProp);
	pObj->SML_INTERFACE(IMyMethod) = SML_FUNC_TABLE(MyNameObj, IMyMethod);
	return S_OK;
}

//struct IMyProp
//{
//	HRESULT(*SetName)(struct IMyProp *_this, LPCTSTR name);
//operation

HRESULT SML_OBJ_FUNC_1(MyNameObj, SetName)(struct MyNameObj *_this, LPCTSTR name)
{
	_tcscpy_s(_this->m_name, _this->m_bufferSize, name);
	return S_OK;
}


HRESULT SML_OBJ_FUNC_2(MyNameObj, IMyProp, SetName)(struct IMyProp * _this, LPCTSTR name)
{
	struct MyNameObj * pObj = SML_GET_OBJECT_2(MyNameObj, IMyProp, _this);
	return SML_OBJ_FUNC_1(MyNameObj, SetName)(pObj, name);
}


LPCTSTR SML_OBJ_FUNC_1(MyNameObj, GetName)(struct MyNameObj *_this)
{
	return _this->m_name;
}

//	LPCTSTR(*GetName)(struct IMyProp *_this);
//operation
LPCTSTR SML_OBJ_FUNC_2(MyNameObj, IMyProp, GetName)(struct IMyProp *_this)
{
	struct MyNameObj * pObj = SML_GET_OBJECT_2(MyNameObj, IMyProp, _this);
	return SML_OBJ_FUNC_1(MyNameObj, GetName)(pObj);
}
//	struct IMyObject m_IObject; //deliberately put at end

//	//	HRESULT(*CreateMember)(struct IMyObject * _this);
//create resources

HRESULT SML_OBJ_FUNC_1(MyNameObj, CreateMember)(struct MyNameObj * _this)
{
	_this->m_name = (LPTSTR)VirtualAlloc(
		NULL,//_In_opt_ LPVOID lpAddress,
		_this->m_bufferSize * sizeof(TCHAR),//_In_     SIZE_T dwSize,
		MEM_RESERVE | MEM_COMMIT,//_In_     DWORD  flAllocationType,
		PAGE_READWRITE//,//_In_     DWORD  flProtect
		);; //some resource
	_this->m_name[0] = 0;
	return S_OK;
}

HRESULT SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, CreateMember)(struct SML_IObject * _this)
{
	struct MyNameObj * pObj = SML_GET_OBJECT_3(MyNameObj, IMyProp, SML_IObject, _this);
	return SML_OBJ_FUNC_1(MyNameObj, CreateMember)(pObj);
}
//	//	HRESULT(*DestroyMember)(struct IMyObject * _this);
//destroy resources

HRESULT SML_OBJ_FUNC_1(MyNameObj, DestroyMember)(struct MyNameObj * _this)
{
	VirtualFree(_this->m_name, 0, MEM_RELEASE);
	_this->m_name = NULL;
	return S_OK;
}

HRESULT SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, DestroyMember)(struct SML_IObject * _this)
{
	struct MyNameObj * pObj = SML_GET_OBJECT_3(MyNameObj, IMyProp, SML_IObject, _this);
	return SML_OBJ_FUNC_1(MyNameObj, DestroyMember)(pObj);
}

//	//	LONG(*AddRef)(struct IMyObject * _this)
//add reference count

LONG SML_OBJ_FUNC_1(MyNameObj, AddRef)(struct MyNameObj * _this)
{
	return InterlockedIncrement(&_this->m_refCount);
}

LONG SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, AddRef)(struct SML_IObject * _this)
{
	struct MyNameObj * pObj = SML_GET_OBJECT_3(MyNameObj, IMyProp, SML_IObject, _this);
	return SML_OBJ_FUNC_1(MyNameObj, AddRef)(pObj);
}
//	//	LONG(*Release)(struct IMyObject * _this);
//decrement reference count, destroy resources (again) by calling DestroyMember(...), and free object itself

LONG SML_OBJ_FUNC_1(MyNameObj, Release)(struct MyNameObj * _this)
{
	LONG refCount = InterlockedDecrement(&_this->m_refCount);
	if (0 == refCount)
	{
		SML_OBJ_FUNC_1(MyNameObj, DestroyMember)(_this);
		switch (_this->m_allocMethod)
		{
		case SAM_AUTO:
		{
			//do nothing because i am not the owner of memory
		}
		break;
		case SAM_MALLOC:
			free(_this);
			break;
		case SAM_HEAP_ALLOC:
			HeapFree(GetProcessHeap(), 0, _this);
			break;
		case SAM_VIRTUAL_ALLOC:
			VirtualFree(_this, 0, MEM_RELEASE);
			break;
		case SAM__MALLOCA:
			_freea(_this);
			break;
		}
	}
	return S_OK;
}

LONG SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, Release)(struct SML_IObject * _this)
{
	struct MyNameObj * pObj = SML_GET_OBJECT_3(MyNameObj, IMyProp, SML_IObject, _this);
	return SML_OBJ_FUNC_1(MyNameObj, Release)(pObj);
}

//QueryInterface
//reference count is incremented on successful query

void * SML_OBJ_FUNC_1(MyNameObj, QueryInterface)(struct MyNameObj * _this, LPCSTR name)
{
	void * pinterface = NULL;
	if (0 == strcmp(name, SML_NAME_OF(SML_IObject)))
	{
		pinterface = &(_this->SML_INTERFACE(IMyProp).SML_INTERFACE(SML_IObject));
		//assert(pinterface == _this);
	}
	else if (0 == strcmp(name, SML_NAME_OF(IMyProp)))
	{
		pinterface = &(_this->SML_INTERFACE(IMyProp));
	}
	else if (0 == strcmp(name, SML_NAME_OF(IMyMethod)))
	{
		pinterface = &(_this->SML_INTERFACE(IMyMethod));
	}

	if (NULL != pinterface)
	{
		SML_OBJ_FUNC_1(MyNameObj, AddRef)(_this);
	}

	return pinterface;
}
void * SML_OBJ_FUNC_3(MyNameObj, IMyProp, SML_IObject, QueryInterface)(struct SML_IObject * _this, LPCSTR name)
{
	struct MyNameObj * pObj = SML_GET_OBJECT_3(MyNameObj, IMyProp, SML_IObject, _this);
	return SML_OBJ_FUNC_1(MyNameObj, QueryInterface)(pObj, name);
}


HRESULT SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, CreateMember)(struct SML_IObject * _this)
{
	struct MyNameObj * pOj = SML_GET_OBJECT_3(MyNameObj, IMyMethod, SML_IObject, _this);
	return SML_OBJ_FUNC_1(MyNameObj, CreateMember)(pOj);
}
HRESULT SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, DestroyMember)(struct SML_IObject * _this)
{
	struct MyNameObj * pOj = SML_GET_OBJECT_3(MyNameObj, IMyMethod, SML_IObject, _this);
	return SML_OBJ_FUNC_1(MyNameObj, DestroyMember)(pOj);
}
LONG SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, AddRef)(struct SML_IObject * _this)
{
	struct MyNameObj * pOj = SML_GET_OBJECT_3(MyNameObj, IMyMethod, SML_IObject, _this);
	return SML_OBJ_FUNC_1(MyNameObj, AddRef)(pOj);
}
LONG SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, Release)(struct SML_IObject * _this)
{
	struct MyNameObj * pOj = SML_GET_OBJECT_3(MyNameObj, IMyMethod, SML_IObject, _this);
	return SML_OBJ_FUNC_1(MyNameObj, Release)(pOj);
}

void * SML_OBJ_FUNC_3(MyNameObj, IMyMethod, SML_IObject, QueryInterface)(struct SML_IObject * _this, LPCSTR name)
{
	struct MyNameObj * pOj = SML_GET_OBJECT_3(MyNameObj, IMyMethod, SML_IObject, _this);
	return SML_OBJ_FUNC_1(MyNameObj, QueryInterface)(pOj, name);
}

HRESULT SML_OBJ_FUNC_1(MyNameObj, SayHi)(struct MyNameObj * _this)
{
	_ftprintf_s(stdout, L"Hello, %s" L"\r\n", _this->m_name);
	return S_OK;
}

HRESULT SML_OBJ_FUNC_2(MyNameObj, IMyMethod, SayHi)(struct IMyMethod * _this)
{
	struct MyNameObj * pOj = SML_GET_OBJECT_2(MyNameObj, IMyMethod, _this);
	return SML_OBJ_FUNC_1(MyNameObj, SayHi)(pOj);
}

HRESULT SML_OBJ_FUNC_1(MyNameObj, SayBye)(struct MyNameObj * _this)
{
	_ftprintf_s(stdout, L"Byte bye, %s" L"\r\n", _this->m_name);
	return S_OK;
}

HRESULT SML_OBJ_FUNC_2(MyNameObj, IMyMethod, SayBye)(struct IMyMethod * _this)
{
	struct MyNameObj * pOj = SML_GET_OBJECT_2(MyNameObj, IMyMethod, _this);
	return SML_OBJ_FUNC_1(MyNameObj, SayBye)(pOj);
}
//	//};
//};