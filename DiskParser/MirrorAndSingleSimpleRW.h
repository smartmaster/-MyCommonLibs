#pragma once


#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <vector>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "DiskExt.h"

namespace SmartLib
{
	class CMirrorAndSingleSimpleRW : public IFileDevice
	{
	private:
		LONG m_refCount{ 0 }; /***INTERNAL***/
		std::vector<DiskExtent> & m_disks; /***INCOMMING***/
		LONGLONG m_pointer{ 0 }; /***INTERNAL***/
	public:
		CMirrorAndSingleSimpleRW(std::vector<DiskExtent> & disks);

		IMP_CLASS_DESTRUCTOR(CMirrorAndSingleSimpleRW);
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
		IMP_WHO_AM_I(TEXT("class CMirrorAndSingleSimpleRW : public IFileDeviceFlush"));
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
		//};
	};


	HRESULT CreateInstanceCMirrorAndSingleSimpleRW(std::vector<DiskExtent> & disks, IFileDevice ** ppObj, BOOL bCreateMember);

	HRESULT CreateInstanceCMirrorAndSingleSimpleRWCacheIOMaker(std::vector<DiskExtent> & disks, IFileDeviceFlush ** ppObj, BOOL bCreateMember);

}//namespace SmartLib