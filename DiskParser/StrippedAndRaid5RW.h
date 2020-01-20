#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>

#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicCacheIO.h"
#include "DiskExt.h"

namespace SmartLib
{

	class CRaid5BlockHelper
	{
	public:
		static void LogicalBlockToPhysicalBlock(BOOL isRaid5, LONGLONG logicalBlock, LONG diskCount, LONGLONG & physicalBlock, LONG & diskNumber);

		static void Test();
	};

	class CStrippedAndRaid5RW : public IFileDeviceFlush
	{
	public:
		static CONST LONG CACHE_SIZE = 1 << 20; //1MB

	private:
		LONG m_refCount{ 0 }; /***INTERNAL***/
		std::vector<DiskExtent> & m_disks; /***INCOMMING***/
		LONG m_strippedBlockSize{ 0 }; /***INCOMMING***/
		BOOL m_isRaid5{ FALSE }; /***INCOMMING***/

		
		std::vector<CComPtr<ICCacheIO> > m_diskCachIo; /***INTERNAL***/
		LONGLONG m_pointer{ 0 }; /***INTERNAL***/
	public:
		CStrippedAndRaid5RW(std::vector<DiskExtent> & disks, LONG strippedBlockSize, BOOL isRaid5);


		IMP_CLASS_DESTRUCTOR(CStrippedAndRaid5RW);
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
		IMP_WHO_AM_I(TEXT("class StrippedAndRaid5RW : public IFileDevice"));
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
		//	virtual HRESULT Flush() = 0;
		virtual HRESULT Flush();
		//};
	};

	//blockSize is size of stripped block (not cache block size)
	HRESULT CreateInstanceCStrippedAndRaid5RW(std::vector<DiskExtent> & disks, LONG blockSize, BOOL isRaid5, IFileDeviceFlush ** ppObj, BOOL bCreateMember);
	HRESULT CreateInstanceCStrippedAndRaid5RWCacheIOMaker(std::vector<DiskExtent> & disks, LONG blockSize, BOOL isRaid5, IFileDeviceFlush ** ppObj, BOOL bCreateMember);

}