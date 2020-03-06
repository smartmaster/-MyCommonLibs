// ObjFree.cpp : Implementation of CObjFree

#include "stdafx.h"
#include "ObjFree.h"


// CObjFree



STDMETHODIMP CObjFree::Print()
{
	// TODO: Add your implementation code here
	_tprintf_s(TEXT("FREE %d") TEXT("\r\n"), GetCurrentThreadId());
	return S_OK;
}
