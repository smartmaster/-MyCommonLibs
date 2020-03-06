// ObjBoth.cpp : Implementation of CObjBoth

#include "stdafx.h"
#include "ObjBoth.h"


// CObjBoth



STDMETHODIMP CObjBoth::Print()
{
	// TODO: Add your implementation code here
	_tprintf_s(TEXT("BOTH %d") TEXT("\r\n"), GetCurrentThreadId());
	return S_OK;
}
