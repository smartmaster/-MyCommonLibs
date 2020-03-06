// ObjSingle.cpp : Implementation of CObjSingle

#include "stdafx.h"
#include "ObjSingle.h"

// CObjSingle



STDMETHODIMP CObjSingle::Print()
{
	// TODO: Add your implementation code here
	_tprintf_s(TEXT("SINGLE %d") TEXT("\r\n"), GetCurrentThreadId());
	return S_OK;
}
