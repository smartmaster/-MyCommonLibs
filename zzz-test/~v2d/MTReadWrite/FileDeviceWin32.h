#pragma once 

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>

#include "FileDeviceInterface.h"


class CFileDeviceWin32 : public IFileDevice
{
private:	//******************************************
	IN CString		m_strFileName;
	IN ULONG		m_ulDesiredAccess;
	IN ULONG		m_ulShareMode;
	IN ULONG		m_ulCreationDisposition;
	IN ULONG		m_ulFlagsAndAttributes;

	HANDLE			m_hFile;	//INTERNAL
	ULONG			m_RefCount;

public: //******************************************
	CFileDeviceWin32(
		IN LPCTSTR		strFileName,
		IN ULONG		ulDesiredAccess,
		IN ULONG		ulShareMode,
		IN ULONG		ulCreationDisposition,
		IN ULONG		ulFlagsAndAttributes);

	virtual ~CFileDeviceWin32();

	////struct ISimpleUnknown : public IUnknown
	////{
	////	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	////	//	IUnknown
	////	//{
	////	//public:
	////	//	BEGIN_INTERFACE
	////	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	////	//		/* [in] */ REFIID riid,
	////	//		/* [iid_is][out] */ void **ppvObject) = 0;

	////	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	////	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	////	//	END_INTERFACE
	////	//};
	IMP_IUNKNOWN_METHOD(m_RefCount);

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();
	////};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);

	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);

	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);

	virtual HRESULT GetFileSize(LONGLONG * pFileSize);

private: //******************************************
	CFileDeviceWin32(CONST CFileDeviceWin32&);
	CFileDeviceWin32& operator=(CONST CFileDeviceWin32&);
};
