// OuterObject.cpp : Implementation of COuterObject

#include "stdafx.h"
#include "OuterObject.h"


// COuterObject
#include "..\AaaaCommon\ObjectMethod.h"


STDMETHODIMP COuterObject::OutDynamicArray(LONG* pLength , WCHAR ** ppBuffer)
{
	// TODO: Add your implementation code here
	HRESULT hr = ::OutDynamicArray(
		pLength, 
		ppBuffer);
	return hr;
}
