#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>

#include "DiskExt.h"

namespace SmartLib
{
	

	class CSpannedAndSpannedSimpleRW : public IFileDevice
	{
	private:
		LONG m_refCount{ 0 };
		std::vector<DiskExtent> & m_disks; /***INCOMMING***/
		LONGLONG m_pointer{ 0 }; /***INTERNAL***/
	public:
		CSpannedAndSpannedSimpleRW(std::vector<DiskExtent> & disks);

		IMP_CLASS_DESTRUCTOR(CSpannedAndSpannedSimpleRW);
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
		IMP_WHO_AM_I(TEXT("class CSpannedAndSpannedSimpleRW : public IFileDevice"));
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

	private:
		static VOID Split(
			IN LONGLONG start,
			IN LONGLONG end,
			IN CONST std::vector<DiskExtent> & disks,
			OUT LONG & startDisk,
			OUT std::vector<LONGLONG> & segment
			);
	};

	HRESULT CreateInstanceCSpannedAndSpannedSimpleRW(std::vector<DiskExtent> & disks, IFileDevice ** ppObj, BOOL bCreateMember);
	HRESULT CreateInstanceCSpannedAndSpannedSimpleRWCacheIOMaker(std::vector<DiskExtent> & disks, IFileDeviceFlush ** ppObj, BOOL bCreateMember);

}  //namespace SmartLib