#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicCacheIO.h"

#include "CacheIOMaker.h"

namespace SmartLib
{
	class CCacheIoMaker : public IFileDeviceFlush
	{
	private:
		CComPtr<IFileDevice> m_pFileDevice; /***INCOMMING***/
		BOOL m_bAsOwner{ FALSE }; /***INCOMMING***/
		LONG m_cacheBlockSize{ 0 }; /***INCOMMING***/
		BOOL m_bWriteNewAndSequential{ FALSE }; /***INCOMMING***/
		BOOL m_isFlushable{ FALSE }; /***INCOMMING***/

		CComPtr<ICCacheIO> m_cacheIo; /***INTERNAL***/
		IFileDevice * m_cacheIoShadow{ nullptr }; /***INTERNAL***/

		LONG m_recCount{ 0 }; /***INTERNAL***/
		
	public:
		CCacheIoMaker(
		IFileDevice * pFileDevice,
		BOOL bAsOwner,
		LONG cacheBlockSize,
		BOOL bWriteNewAndSequential,
		BOOL isFlushable
			);

		IMP_CLASS_DESTRUCTOR(CCacheIoMaker);

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
		IMP_IUNKNOWN_METHODS(m_recCount);

		//	//	//	virtual LPCTSTR WhoAmI() = 0;
		IMP_WHO_AM_I(TEXT("class CCacheIoMaker : public IFileDeviceFlush"));
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


	HRESULT CreateInstanceCCacheIoMaker(
		IFileDevice * pFileDevice,
		BOOL bAsOwner,
		LONG blockSize,
		BOOL bWriteNewAndSequential,
		BOOL isFlushable,
		IFileDeviceFlush ** ppObj,
		BOOL bCreateMmeber
		);
}