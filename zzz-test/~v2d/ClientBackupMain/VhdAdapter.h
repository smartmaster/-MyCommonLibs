#pragma once

#include "stdafx.h"
#include "..\LibVirtualDisk\HAVhdUtility.h"
#include "..\ServerFileDevice\ServerFileDevice_i.h"


class CVHDAdapter :
	public IFileDeviceObject
{
	/*INTERNAL*/ HaVhdUtility::IVHDFileW * m_pVHD;

public:

	CVHDAdapter(HaVhdUtility::IVHDFileW * pVHD = NULL);

	VOID Attach(HaVhdUtility::IVHDFileW * pVHD);

	virtual HRESULT STDMETHODCALLTYPE SetPointerEx( 
		/* [in] */ LONGLONG llDistanceToMove,
		/* [out] */ LONGLONG *lpNewFilePointer,
		/* [in] */ ULONG ulMoveMethod);

	virtual HRESULT STDMETHODCALLTYPE Read( 
		/* [in] */ ULONG nNumberOfBytesToRead,
		/* [out] */ ULONG *lpNumberOfBytesRead,
		/* [length_is][size_is][out] */ BYTE *pBuffer);

	virtual HRESULT STDMETHODCALLTYPE Write( 
		/* [in] */ ULONG nNumberOfBytesToWrite,
		/* [out] */ ULONG *lpNumberOfBytesWritten,
		/* [size_is][in] */ BYTE *pBuffer);

	virtual HRESULT STDMETHODCALLTYPE Create( 
		/* [in] */ BSTR bstrFileName,
		/* [in] */ ULONG ulDesiredAccess,
		/* [in] */ ULONG ulShareMode,
		/* [in] */ ULONG ulCreationDisposition,
		/* [in] */ ULONG ulFlagsAndAttributes);

	virtual HRESULT STDMETHODCALLTYPE Close(void);

	virtual HRESULT STDMETHODCALLTYPE GetFileSizeEx( 
		/* [out] */ LONGLONG *pllFileSize);

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

	virtual ~CVHDAdapter();
};