#pragma once

#include <tchar.h>
#include <windows.h>

//interface can only contain function pointer or interface 
struct SML_IObject
{
	HRESULT(*CreateMember)(struct SML_IObject * _this);
	HRESULT(*DestroyMember)(struct SML_IObject * _this);
	LONG(*AddRef)(struct SML_IObject * _this);
	LONG(*Release)(struct SML_IObject * _this);
	void * (*QueryInterface)(struct SML_IObject * _this, LPCSTR name);
};
