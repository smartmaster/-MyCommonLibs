#pragma once

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicGetCurrentModuleName.h"
#include "..\PublicHeader\PublicVMemPtr.h"

#include "FileDeviceInterface.h"


class CFileDeviceMemAutoAlloc : public IFileDeviceMem
{
private:
	LONGLONG m_CurrentPointer; /***INTERNAL***/
	LONGLONG m_DataSize; /***INTERNAL***/
	LONGLONG m_DataSizeRead; /***INTERNAL***/
	LONGLONG m_AllocSize; /***INCOMMING***/
	LONGLONG m_AllocDelta; /***INCOMMING***/
	LONGLONG m_MaxAllocSize; /***INCOMMING***/
	LONG m_RefCount; /***INTERNAL***/

	CVMemPtr<BYTE> m_Buffer; /***INTERNAL***/ /***CLEANUP***/
	
	static CONST LONG _ALLIGNMENT_SIZE = 4 * 1024;

public:
	CFileDeviceMemAutoAlloc(LONGLONG InitialSize = 64LL * 1024, LONGLONG AllocDelta = 16LL * 1024, LONGLONG MaxAllocSize = 2LL * 1024 * 1024); //AllocDelta must be 2^N
	IMP_CLASS_DESTRUCTOR(CFileDeviceMemAutoAlloc);


	//struct IFileDeviceMem : public IFileDevice
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
	//	//	//	//		/* [iid_is][out] */ void **ppvObject);

	//	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void);

	//	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void);

	//	//	//	//	END_INTERFACE
	//	//	//	//};
	IMP_IUNKNOWN_METHODS(m_RefCount);

	virtual LPCTSTR WhoAmI()
	{
		static CONST TCHAR WHO_AM_I[] = TEXT("CFileDeviceMem:IFileDeviceMem");
		return WHO_AM_I;
	}

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//	//};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
	virtual HRESULT GetFileSize(LONGLONG * pFileSize);

	//	//};

	virtual HRESULT GetBufferSize(LONGLONG * pBufferSize);

	virtual HANDLE GetRawHandle();
	//};

private:
	HRESULT IncreaseAllocSize(LONGLONG newSize);

	BOOL IsValidOffset(LONGLONG Offset)
	{
		return Offset >= 0 && Offset < m_AllocSize;
	}
};