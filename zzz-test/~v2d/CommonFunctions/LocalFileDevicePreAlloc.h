#pragma once

#include "stdafx.h"
#include "..\ServerFileDevice\ServerFileDevice_i.h"

struct CLocalFileDevicePreAlloc : public IFileDeviceObject
{
private:	//******************************************
	CString m_strFileName;
	HANDLE m_hFile;
	LONGLONG m_Delata;
	LONGLONG m_CurPointer;
	LONGLONG m_MaxDataSize;
	LONGLONG m_AllocatedSize;
	DWORD m_dwFlagsAndAttributeDest; //<mycode> 2011-4-26

public:
	CLocalFileDevicePreAlloc(LONGLONG Delta);
	VOID InitMember();
	HRESULT DestroyMember();
	~CLocalFileDevicePreAlloc();

	//******************************************
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

	//******************************************
	virtual HRESULT STDMETHODCALLTYPE Create(BSTR bstrFileName, ULONG ulDesiredAccess, ULONG ulShareMode, ULONG ulCreationDisposition, ULONG ulFlagsAndAttributes);
	virtual HRESULT STDMETHODCALLTYPE SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT STDMETHODCALLTYPE Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT STDMETHODCALLTYPE Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, BYTE* pBuffer);

	virtual HRESULT STDMETHODCALLTYPE Close(void);
	virtual HRESULT STDMETHODCALLTYPE GetFileSizeEx(LONGLONG* pllFileSize);

private: //******************************************
	CLocalFileDevicePreAlloc(CONST CLocalFileDevicePreAlloc&);
	CLocalFileDevicePreAlloc& operator=(CONST CLocalFileDevicePreAlloc&);
};