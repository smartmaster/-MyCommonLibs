#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"

namespace SmartLib
{
	class CSparseBlockMemory : public IFileDevice
	{
	private:
		LONG m_refCount{ 0 }; /***INTERNAL***/

		LONGLONG m_CurrentPointer{ 0 }; /***INTERNAL***/
		LONG m_blockSize{ 0 }; /***INCOMMING***/
		LONGLONG m_InitialSize{ 0 }; /***INCOMMING***/
		std::vector<LPBYTE> m_bat; /***INTERNAL***/

	private:
		BOOL IsValidOffset(LONGLONG Offset);
		HRESULT ReadBlock(LONGLONG offset, ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
		HRESULT ReadPartialBlock(LONGLONG offset, ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
		HRESULT WriteBlock(LONGLONG offset, ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
		HRESULT WritePartialBlock(LONGLONG offset, ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);

	public:
		CSparseBlockMemory(LONG blockSize /*must be 2^n*/, LONGLONG InitialSize);

		IMP_CLASS_DESTRUCTOR(CSparseBlockMemory);

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
		IMP_WHO_AM_I(TEXT("class CSparseBlockMemory : public IFileDevice"));

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

		//};
	};

	//API
	HRESULT CreateInstanceCSparseBlockMemory(LONG blockSize /*must be 2^n*/, LONGLONG InitialSize, IFileDevice ** ppIFileDevice, BOOL bCreateMember = TRUE);

	//test case
	HRESULT test_CSparseBlockMemory(LPCTSTR srcFile, LPCTSTR destFile);
} //end namespace SmartLib