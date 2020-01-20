#pragma once


#if 0

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\..\PublicHeader\PublicDTraceLib.h"
#include "..\..\PublicHeader\PublicDiskImageInterface.h"

namespace SmartLib
{
	class CVHDXSkipZeroWriter : public IFileDeviceFlush
	{
	private:
		CString m_vhdxName; /***INCOMMING***/
		LONGLONG m_diskSize{ 0 }; /***INCOMMING***/
		LONG m_vhdxBlockSize{ 0 }; /***INCOMMING***/
		LONG m_refCount{ 0 };
		CComPtr<IDiskImageVirtual> m_vhdx; /***INTERNAL***/ /***INCOMMING***/
	public:
		CVHDXSkipZeroWriter(LPCTSTR vhdxName, LONGLONG diskSize, LONG vhdxBlockSize);
		//struct IFileDeviceFlush : public IFileDevice
		//{
		//	//struct IFileDevice : public ISimpleUnknown
		//	//{
		//	//	//struct ISimpleUnknown : public IUnknown
		//	//	//{
		//	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
		//	//	//	//	IUnknown
		//	//	//	//{
		//	//	//	//public:
		//	//	//	//	BEGIN_INTERFACE
		//	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
		//	//	//	//		/* [in] */ REFIID riid,
		//	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

		//	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

		//	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

		//	//	//	//	END_INTERFACE
		//	//	//	//};
		IMP_IUNKNOWN_METHODS(m_refCount);

		//	//	//	virtual LPCTSTR WhoAmI() = 0;
		IMP_WHO_AM_I(TEXT("class CVHDXSkipZeroWriter : public IFileDeviceFlush"));
		//	//	//	virtual HRESULT CreateMember() = 0;
		virtual HRESULT CreateMember();
		//	//	//	virtual HRESULT DestroyMember() = 0;
		virtual HRESULT DestroyMember();
		//	//	//};

		//	//	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
		virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
		//	//	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
		virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
		//	//	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
		virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);

		//	//	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;
		virtual HRESULT GetFileSize(LONGLONG * pFileSize);
		//	//	virtual HANDLE GetRawHandle() = 0;
		virtual HANDLE GetRawHandle();

		//	//};
		//	virtual HRESULT Flush() = 0;
		virtual HRESULT Flush();
		//};
	};
}

#endif //#if 0