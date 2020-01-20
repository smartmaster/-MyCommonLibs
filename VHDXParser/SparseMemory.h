#pragma once

#if 0

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "CacheIO.h"
#include "SparseBlockMemory.h"

namespace SmartLib
{
	class CSparseMemory : public IFileDeviceFlush
	{
	private:
		LONG m_refCount{ 0 }; /***INTERNAL***/

		LONGLONG m_CurrentPointer{ 0 }; /***INTERNAL***/
		LONG m_blockSize{ 0 }; /***INCOMMING***/
		LONGLONG m_InitialSize{ 0 }; /***INCOMMING***/

		CAutoPtr<CSparseBlockMemory> m_sbm; /***INCOMMING***/
		CAutoPtr<CCacheIO> m_cacheIO; /***INCOMMING***/

	private:

	public:
		CSparseMemory(LONG blockSize /*must be 2^n*/, LONGLONG InitialSize);

		IMP_CLASS_DESTRUCTOR(CSparseMemory);

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
		IMP_IUNKNOWN_METHODS(m_refCount);

		//	//	virtual LPCTSTR WhoAmI() = 0;
		IMP_WHO_AM_I(TEXT("class CSparseMemory : IFileDevice"));

		//	//	virtual HRESULT CreateMember() = 0;
		virtual HRESULT CreateMember();
		//	//	virtual HRESULT DestroyMember() = 0;
		virtual HRESULT DestroyMember();
		//	//};

		//	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
		virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
		//	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
		virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
		//	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
		virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
		//	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;
		virtual HRESULT GetFileSize(LONGLONG * pFileSize);
		//	virtual HANDLE GetRawHandle() = 0;
		virtual HANDLE GetRawHandle();

		virtual HRESULT Flush();

		//};
	};
	

	//API
	HRESULT CreateInstanceCSparseMemory(LONG blockSize /*must be 2^n*/, LONGLONG InitialSize, IFileDeviceFlush ** ppIFileDevice, BOOL bCreateMember = TRUE);


	//test case
	HRESULT test_CSparseMemory(LPCTSTR srcFile, LPCTSTR destFile);
}
#endif