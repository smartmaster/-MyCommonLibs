// InnerObject.cpp : Implementation of CInnerObject

#include "stdafx.h"
#include "InnerObject.h"


// CInnerObject
#include "..\AaaaCommon\ObjectMethod.h"


STDMETHODIMP CInnerObject::OutDynamicArray(LONG* pLength, WCHAR ** ppBuffer)
{
	// TODO: Add your implementation code here
	HRESULT hr = ::OutDynamicArray(
		pLength, 
		ppBuffer);
	return hr;
}
