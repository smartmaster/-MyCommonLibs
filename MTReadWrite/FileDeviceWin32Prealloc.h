#pragma once 

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"

#include "FileDeviceInterface.h"

class CFileDeviceWin32Prealloc : public IFileDevice
{
private:
	LONG m_RefCount; /***Internal***/
	HANDLE m_hFile; /***Internal to cleanup***/
	LONGLONG m_Delata; /***Incomming***/
	LONGLONG m_CurPointer; /***Internal***/
	LONGLONG m_MaxDataSize; /***Internal***/
	LONGLONG m_AllocatedSize; /***Internal***/
	CString m_strFileName; /***Incomming***/
	ULONG m_ulDesiredAccess; /***Incomming***/
	ULONG m_ulShareMode; /***Incomming***/
	ULONG m_ulCreationDisposition; /***Incomming***/
	DWORD m_ulFlagsAndAttributes; /***Incomming***/
	BOOL m_bToEnablePrivilege; /***INTERNAL***/

public:
	CFileDeviceWin32Prealloc(LONGLONG Delta, LPCTSTR strFileName, ULONG ulDesiredAccess, ULONG ulShareMode, ULONG ulCreationDisposition, ULONG ulFlagsAndAttributes);
	IMP_CLASS_DESTRUCTOR(CFileDeviceWin32Prealloc);

public:
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

	virtual LPCTSTR WhoAmI();
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
