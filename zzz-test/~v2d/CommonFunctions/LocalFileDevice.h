#pragma once 

#include "stdafx.h"
#include "..\ServerFileDevice\ServerFileDevice_i.h"

struct CLocalFileDevice : public IFileDeviceObject
{
private:	//******************************************
	CString m_strFileName;
	HANDLE m_hFile;

public: //******************************************
	CLocalFileDevice();
	VOID InitMember();
	VOID DestroyMember();
	~CLocalFileDevice();

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
	CLocalFileDevice(CONST CLocalFileDevice&);
	CLocalFileDevice& operator=(CONST CLocalFileDevice&);
};

struct CLocalFileDeviceHandle : public IFileDeviceObject
{
private:	//******************************************
	HANDLE m_hFile; //external object

public: //******************************************
	CLocalFileDeviceHandle(HANDLE hFile);

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
	CLocalFileDeviceHandle(CONST CLocalFileDeviceHandle&);
	CLocalFileDeviceHandle& operator=(CONST CLocalFileDeviceHandle&);
};