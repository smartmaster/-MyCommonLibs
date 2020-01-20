#pragma once

#include "FileDeviceInterface.h"


class CFileDeviceBackupHandle : public IFileDevice
{
private:
	HANDLE m_Handle; /***Incomming***/
	LONG m_RefCount; /***Internal***/
	BOOL m_bAttacch; /***INTERNAL***/
	LPVOID m_pContext; /***INTERNAL***/ /***CLEANUP***/
	E_BACKUP_RW m_BACKUP_RW; /***INCOMMING***/


public:
	CFileDeviceBackupHandle(HANDLE Handle, BOOL bAttach, E_BACKUP_RW BACKUP_RW);

	IMP_CLASS_DESTRUCTOR(CFileDeviceBackupHandle);


	VOID InitParams(HANDLE Handle, BOOL bAttach);

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
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("CFileDeviceBackupHandle:IFileDevice"));

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
	virtual HRESULT GetFileSize(LONGLONG * pFileSize);
	virtual HANDLE GetRawHandle();
	//};
};

