#pragma once 

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>



#include "FileDeviceInterface.h"
#include "FileDeviceHandle.h"


class CFileDeviceWin32 : public IFileDevice
{
private:	//******************************************
	IN CString		m_strFileName; /***Incomming***/
	IN ULONG		m_ulDesiredAccess; /***Incomming***/
	IN ULONG		m_ulShareMode;/***Incomming***/
	IN ULONG		m_ulCreationDisposition;/***Incomming***/
	IN ULONG		m_ulFlagsAndAttributes; /***Incomming***/
	
	ULONG			m_RefCount; /***Internal***/

	CFileDeviceHandle m_FileDeviceImp; /***Internal***/

public: //******************************************
	CFileDeviceWin32(
		IN LPCTSTR		strFileName,
		IN ULONG		ulDesiredAccess,
		IN ULONG		ulShareMode,
		IN ULONG		ulCreationDisposition,
		IN ULONG		ulFlagsAndAttributes);

	IMP_CLASS_DESTRUCTOR(CFileDeviceWin32);

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
	IMP_IUNKNOWN_METHODS(m_RefCount);

	IMP_WHO_AM_I(TEXT("CFileDeviceWin32:IFileDevice"));

	virtual HRESULT CreateMember();

	virtual HRESULT DestroyMember();
	////};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);

	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);

	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);

	virtual HRESULT GetFileSize(LONGLONG * pFileSize);

	virtual HANDLE GetRawHandle();

private: //******************************************
	CFileDeviceWin32(CONST CFileDeviceWin32&);
	CFileDeviceWin32& operator=(CONST CFileDeviceWin32&);
};
