#pragma once

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>

#include "SimpleUnknown.h"


struct IFileDevice : public ISimpleUnknown
{
	//struct ISimpleUnknown : public IUnknown
	//{
	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	IUnknown
	//	//{
	//	//public:
	//	//	BEGIN_INTERFACE
	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//		/* [in] */ REFIID riid,
	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	END_INTERFACE
	//	//};

	//	virtual HRESULT CreateMember() = 0;
	//	virtual HRESULT DestroyMember() = 0;
	//};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;

};

//////////////////////////////////////////////////////////////////////////
HRESULT ReadAtOffsetHelper( IFileDevice * pIFileDevice, LONGLONG Offset, ULONG nNumberOfBytesToRead, ULONG * pNumberOfBytesRead, BYTE* pBuffer );
HRESULT WriteAtOffsetHelper( IFileDevice * pIFileDevice, LONGLONG Offset, ULONG nNumberOfBytesToWrite, ULONG * pNumberOfBytesWritten, CONST BYTE* pBuffer );



//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceFileDeviceWin32(
	IN LPCTSTR strFileName, 
	IN ULONG ulDesiredAccess, 
	IN ULONG ulShareMode, 
	IN ULONG ulCreationDisposition, 
	IN ULONG ulFlagsAndAttributes, 
	OUT IFileDevice ** ppFileDevice, 
	BOOL bCreateMember);