// ObjApartment.cpp : Implementation of CObjApartment

#include "stdafx.h"
#include "ObjApartment.h"


// CObjApartment



STDMETHODIMP CObjApartment::Print()
{
	// TODO: Add your implementation code here
	_tprintf_s(TEXT("APARTMENT %d") TEXT("\r\n"), GetCurrentThreadId());
	return S_OK;
}
