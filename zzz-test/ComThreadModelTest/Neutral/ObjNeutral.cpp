// ObjNeutral.cpp : Implementation of CObjNeutral

#include "stdafx.h"
#include "ObjNeutral.h"


// CObjNeutral



STDMETHODIMP CObjNeutral::Print()
{
	// TODO: Add your implementation code here
	_tprintf_s(TEXT("NEUTRAL %d") TEXT("\r\n"), GetCurrentThreadId());
	return S_OK;
}
