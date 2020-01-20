#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>



#include "..\PublicHeader\PublicSimpleUnknown.h"


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

	//	virtual LPCTSTR WhoAmI() = 0;
	//	virtual HRESULT CreateMember() = 0;
	//	virtual HRESULT DestroyMember() = 0;
	//};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;
	virtual HANDLE GetRawHandle() = 0; 

};

struct IFileDeviceFlush : public IFileDevice
{
	//struct IFileDevice : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};

	//	//	virtual LPCTSTR WhoAmI() = 0;
	//	//	virtual HRESULT CreateMember() = 0;
	//	//	virtual HRESULT DestroyMember() = 0;
	//	//};

	//	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
	//	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
	//	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
	//	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;
	//	virtual HANDLE GetRawHandle() = 0;

	//};
	virtual HRESULT Flush() = 0;
};

struct IFileDeviceMem : public IFileDevice
{
	//struct IFileDevice : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};

	//	//	virtual HRESULT CreateMember() = 0;
	//	//	virtual HRESULT DestroyMember() = 0;
	//	//};

	//	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
	//	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
	//	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
	//	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;

	//};

	virtual HRESULT GetBufferSize(LONGLONG * pBufferSize) = 0;
};


struct IFileDeviceSocketClient : public IFileDevice
{
	//struct IFileDevice : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};

	//	//	virtual LPCTSTR WhoAmI() = 0;
	//	//	virtual HRESULT CreateMember() = 0;
	//	//	virtual HRESULT DestroyMember() = 0;
	//	//};

	//	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
	//	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
	//	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
	//	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;

	//};

	virtual HRESULT ReadAtOffsetHelper(LONGLONG Offset, ULONG nNumberOfBytesToRead, ULONG * pNumberOfBytesRead, BYTE* pBuffer ) = 0;
	virtual HRESULT WriteAtOffsetHelper(LONGLONG Offset, ULONG nNumberOfBytesToWrite, ULONG * pNumberOfBytesWritten, CONST BYTE* pBuffer ) = 0;

	virtual BYTE * GetSendBuffer() = 0;
	virtual BYTE * GetReceiveBuffer() = 0;


	virtual HRESULT ClientCreateInstanceFileDeviceWin32(
		LPCTSTR strFileName, 
		ULONG ulDesiredAccess, 
		ULONG ulShareMode, 
		ULONG ulCreationDisposition, 
		ULONG ulFlagsAndAttributes, 
		BOOL bCreateMember) = 0;
};

//////////////////////////////////////////////////////////////////////////
HRESULT ReadAtOffsetHelper( IFileDevice * pIFileDevice, LONGLONG Offset, ULONG nNumberOfBytesToRead, ULONG * pNumberOfBytesRead, BYTE* pBuffer );
HRESULT WriteAtOffsetHelper( IFileDevice * pIFileDevice, LONGLONG Offset, ULONG nNumberOfBytesToWrite, ULONG * pNumberOfBytesWritten, CONST BYTE* pBuffer );

HRESULT WriteAllHelper(IFileDevice * pIFileDevice, LONG TotalBytesToWrite, LONG * pTotalDataWritten, CONST BYTE* pBuffer);
HRESULT ReadAllHelper(IFileDevice * pIFileDevice, LONG TotalBytesToRead, LONG * pTotalDataRead, BYTE* pBuffer);



//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceFileDeviceWin32(
	IN LPCTSTR strFileName, 
	IN ULONG ulDesiredAccess, 
	IN ULONG ulShareMode, 
	IN ULONG ulCreationDisposition, 
	IN ULONG ulFlagsAndAttributes, 
	OUT IFileDevice ** ppFileDevice, 
	BOOL bCreateMember
	);

HRESULT CreateInstanceCFileDeviceWin32Prealloc(
	LONGLONG Delta, 
	LPCTSTR strFileName, 
	ULONG ulDesiredAccess, 
	ULONG ulShareMode, 
	ULONG ulCreationDisposition, 
	ULONG ulFlagsAndAttributes,
	IFileDevice ** ppObj,
	BOOL bCreateMember
	);

HRESULT CreateInstanceCFileDeviceMem(BYTE * pBuffer, LONGLONG BufferSize, IFileDeviceMem ** ppObj, BOOL bCreateMember);
HRESULT CreateInstanceCFileDeviceMemAutoAlloc(LONGLONG InitialSize, LONGLONG AllocDelta, LONGLONG MaxAllocSize, IFileDeviceMem ** ppObj, BOOL bCreateMember);


HRESULT CreateInstanceCFileDeviceHandle(
	IN HANDLE Handle,
	IN BOOL bAttach,
	OUT IFileDevice ** ppFileDevice,
	BOOL bCreateMember
	);

//<sonmi01>2014-12-8 ###???
enum class E_BACKUP_RW {READ, WRITE};
HRESULT CreateInstanceCFileDeviceBackupHandle(
	IN HANDLE Handle,
	IN BOOL bAttach,
	IN E_BACKUP_RW BACKUP_RW,
	OUT IFileDevice ** ppFileDevice,
	BOOL bCreateMember
	);