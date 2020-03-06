#include "stdafx.h"

HRESULT OutDynamicArray(
		LONG* pLength, 
		WCHAR ** ppBuffer)
{
	//D_START(0);
	CString str = TEXT(__FUNCTION__);

	CComHeapPtr<WCHAR> apBuffer;
	LONG BufferSize = *pLength = str.GetLength() + 1;
	apBuffer.Allocate(BufferSize);
	_tcscpy_s(apBuffer.m_pData, BufferSize, str.GetString());
	*ppBuffer = apBuffer.Detach();
	//_OLD_DINFO((0, DRNPOS, TEXT("Out String [%s]"), *ppBuffer));

	return S_OK;
}