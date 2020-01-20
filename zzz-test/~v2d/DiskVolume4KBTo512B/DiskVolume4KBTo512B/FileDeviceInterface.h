#pragma once

#include <tchar.h>
#include <Windows.h>

#include "SimpleUnknown.h"

#if defined(EXPORTED_FILE_DEVICE_DLLAPI_76E26A7E_D764_4685_BFD9_1A40D6A0AFF1)
#define FILE_DEVICE_DLLAPI __declspec(dllexport)
#else
#define FILE_DEVICE_DLLAPI __declspec(dllimport)
#endif


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

};


//////////////////////////////////////////////////////////////////////////
FILE_DEVICE_DLLAPI
HRESULT CreateInstanceFileDeviceWin32(
	IN LPCTSTR strFileName, 
	IN ULONG ulDesiredAccess, 
	IN ULONG ulShareMode, 
	IN ULONG ulCreationDisposition, 
	IN ULONG ulFlagsAndAttributes, 
	OUT IFileDevice ** ppFileDevice, 
	BOOL bCreateMember);