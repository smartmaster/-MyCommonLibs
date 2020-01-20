#pragma once

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>
#include <intsafe.h>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "FileCopyInterface.h"
#include "FileDeviceInterface.h"
#include "MTPoolAndQueue.h"
#include "VMemPtr.h"
#include "SimpleBitArray.h"
#include "StopTimer.h"


struct MTBufferItemT
{
	ULONG m_BufferSize;
	ULONG m_DataSize;
	LONGLONG m_Offset;
	ULONG m_Flags; //<sonmi01>2014-12-10 ###???
	ULONG m_Enum;
	BYTE * m_pData; /***Internal to cleanup***/

	//////////////////////////////////////////////////////////////////////////
	MTBufferItemT();

	//<sonmi01>2014-12-10 ###???
	VOID Init();
	HRESULT CreateMember();
	HRESULT DestroyMember();

	//////////////////////////////////////////////////////////////////////////
	enum class FLAGS
	{
		SEEK = 0x1
	};

	enum class ENUM
	{

	};

private:
	static CONST ULONG s_DataBufferSize = 1024 * 1024 * 2;
};

class CMTCopy : public IFileCopy
{
private:
	IFileDevice * m_pSource; /***Incomming***/
	IFileDevice * m_pDest; /***Incomming***/
	/*CONST*/ LONGLONG m_SourceStartOffset; /***Incomming***/
	/*CONST*/ LONGLONG m_Diff; /***Incomming***/  //m_Diff = DestOffset - SourceOffset

	/*CONST*/ CSimpleBitArray * m_pSimpleBitArray; /***Incomming***/
	/*CONST*/ LONGLONG m_BlockSize; /***Incomming***/
	BOOL m_SeekAlways; /***INCOMMING***/ //<sonmi01>2014-12-12 ###???

	LONGLONG m_TotalRead;
	LONGLONG m_TotalWritten;

	HANDLE m_EventError; /***Internal to cleanup***/
	HANDLE m_EventAbort; /***Internal to cleanup***/
	HANDLE m_EventProviderFinish; /***Internal to cleanup***/
	CMTPool<MTBufferItemT> m_pool;  /***Internal to cleanup***/
	CMTQueue<MTBufferItemT> m_queue; /***Internal to cleanup***/

	LONG m_RefCount; /***INTERNAL***/

	BOOL m_bStartPerf; /***INTERNAL***/
	CStopTimer m_StopTimer;


private:
	static CONST ULONG s_BufferItemCount = 32;

private:
	HRESULT ReadDataBuffer(LONGLONG StartOffset, LONGLONG DataLength);
	HRESULT ReadProc();
	HRESULT WriteProc();

	static DWORD WINAPI ReadThreadProc(LPVOID lpParameter);
	static DWORD WINAPI WriteThreadProc(LPVOID lpParameter);

public:
	CMTCopy(
		IFileDevice * pSource,
		IFileDevice * pDest, 
		LONGLONG SourceStartOffset, 
		LONGLONG Diff,
		CSimpleBitArray * pSimpleBitArray,
		LONGLONG BlockSize,
		BOOL SeekAlways //<sonmi01>2014-12-12 ###???
		);

	IMP_CLASS_DESTRUCTOR(CMTCopy);

public:
	//struct IFileCopy : public ISimpleUnknown
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

	//	//	virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("CMTCopy:IFileCopy"));
	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};
	virtual HRESULT Copy();
	virtual HRESULT InitParams(LPVOID pPrarams);
	//};
};

//////////////////////////////////////////////////////////////////////////
//// API
HRESULT CreateInstanceCMTCopy(
	IFileDevice * pSource,
	IFileDevice * pDest, 
	LONGLONG SourceStartOffset,			// in case 0 == SourceStartOffset,  0 == Diff, nullptr == pSimpleBitArray, it will determine whether rad/write file_seek be called
	LONGLONG Diff,						//
	CSimpleBitArray * pSimpleBitArray, //if nullptr, means reading from beginning until no more data
	LONGLONG BlockSize,
	BOOL SeekAlways, //<sonmi01>2014-12-12 ###???
	IFileCopy ** ppObj,
	BOOL bCreateMemeber
	);