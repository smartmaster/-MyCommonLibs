#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>

#include <vector>
#include <algorithm>
using namespace std;

#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicVMemPtr.h"
#include "..\PublicHeader\PublicDTraceLib.h"



struct CacheBuffer
{
	//////////////////////////////////////////////////////////////////////////
	LONG m_DataSize;
	LONG m_BlockSize;
	LONG m_bNeedToFlush; //to support cache read / write at same time
	LONG m_Reserved;
	LONGLONG m_Offset;
	LPBYTE m_Buffer;


	//////////////////////////////////////////////////////////////////////////
	CacheBuffer(LONG BlockSize);

	HRESULT Allocate();

	HRESULT ResetCache();

	~CacheBuffer();

private:
	const CacheBuffer & operator=(const CacheBuffer &);
	CacheBuffer(const CacheBuffer &);
};


struct ICCacheIO : public IFileDeviceFlush
{
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

	//	//	//	virtual LPCTSTR WhoAmI() = 0;
	//	//	//	virtual HRESULT CreateMember() = 0;
	//	//	//	virtual HRESULT DestroyMember() = 0;
	//	//	//};

	//	//	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
	//	//	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
	//	//	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
	//	//	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;
	//	//	virtual HANDLE GetRawHandle() = 0;

	//	//};
	//	virtual HRESULT Flush() = 0; //if not called explicitly, it will be called when destructor is called
	//};

	virtual HRESULT Seek(LONGLONG FilePointer) = 0;
	virtual HRESULT Read(LONG len, BYTE * Buffer) = 0;
	virtual HRESULT Write(LONG len, CONST BYTE * Buffer) = 0;

	virtual HRESULT ReadAtOffset(LONGLONG StartOffset, LONG len, BYTE * Buffer) = 0;
	virtual HRESULT WriteAtOffset(LONGLONG StartOffset, LONG len, CONST BYTE * Buffer) = 0;
};



/*
powerful class to convert blocks read / write to random read / write at any offset and in any length
*/
class  CCacheIO : public ICCacheIO
{
private:
	LONG m_BlockSize; /***INCOMMING***/
	CComPtr<IFileDevice> m_pFileDevice; /***INCOMMING***/
	BOOL m_bAsOwner; /***INCOMMING***/

	vector<CacheBuffer*> m_Caches; /***INTERNAL TO CLEANUP***/

	LONGLONG m_HitCount; /***INTERNAL***/
	LONGLONG m_MissedCount; /***INTERNAL***/
	BOOL m_bFlushNeeded; /***INTERNAL***/
	LONGLONG	m_CurrentPointer; /***INTERNAL***/
	LONGLONG	m_MaxWritePos; /***INTERNAL***/
	LONGLONG	m_MaxReadPos; /***INTERNAL***/
	BOOL		m_bBlockSize2N; /***INTERNAL***/
	BOOL		m_bWriteNewAndSequential; // if it is true, it means no need to read/modify/write for cache write so that performance can improve
	LONG		m_refCount; /***INTERNAL***/

private:
	//temp local variables for reducing memory allocation, resize(0) before use
	vector<LONGLONG> m_hitOffsetsTemp;
	vector<LONGLONG> m_NotAlignedOffsetTemp;
	vector<size_t> m_missedCacheBufferIndexTemp;


private:
	static CONST LONG MIN_CACHE_BUFFER_COUNT = 2;


public:
	CCacheIO(IFileDevice * pFileDevice, BOOL bAsOwner, LONG BlockSize, BOOL bWriteNewAndSequential = FALSE); //<sonmi01>2015-5-19 ###???
	//~CCacheIO();
	IMP_CLASS_DESTRUCTOR(CCacheIO);

	VOID InitParams(IFileDevice * pFileDevice, LONG BlockSize);

	//HRESULT Seek(LONGLONG FilePointer);
	//HRESULT Read(LONG len, BYTE * Buffer);
	//HRESULT Write(LONG len, CONST BYTE * Buffer);

	//HRESULT ReadAtOffset(LONGLONG StartOffset, LONG len, BYTE * Buffer);
	//HRESULT WriteAtOffset(LONGLONG StartOffset, LONG len, CONST BYTE * Buffer);

	//HRESULT Flush(); //if not called explicitly, it will be called when destructor is called

	LONGLONG GetFileSizeWrite();
	LONGLONG GetFileSizeRead();


	//struct ICCacheIO : public ISimpleUnknown
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
	IMP_WHO_AM_I(TEXT("class  CCacheIO : public ICCacheIO"));

	//	//	virtual HRESULT CreateMember() = 0;
	virtual HRESULT CreateMember();
	//	//	virtual HRESULT DestroyMember() = 0;
	virtual HRESULT DestroyMember();
	//	//};

	//	virtual HRESULT Seek(LONGLONG FilePointer) = 0;
	virtual HRESULT Seek(LONGLONG FilePointer);
	//	virtual HRESULT Read(LONG len, BYTE * Buffer) = 0;
	virtual HRESULT Read(LONG len, BYTE * Buffer);
	//	virtual HRESULT Write(LONG len, CONST BYTE * Buffer) = 0;
	virtual HRESULT Write(LONG len, CONST BYTE * Buffer);

	//	virtual HRESULT ReadAtOffset(LONGLONG StartOffset, LONG len, BYTE * Buffer) = 0;
	virtual HRESULT ReadAtOffset(LONGLONG StartOffset, LONG len, BYTE * Buffer);
	//	virtual HRESULT WriteAtOffset(LONGLONG StartOffset, LONG len, CONST BYTE * Buffer) = 0;
	virtual HRESULT WriteAtOffset(LONGLONG StartOffset, LONG len, CONST BYTE * Buffer);

	//	virtual HRESULT Flush() = 0; //if not called explicitly, it will be called when destructor is called
	virtual HRESULT Flush();
	//};


	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
	virtual HRESULT GetFileSize(LONGLONG * pFileSize);
	virtual HANDLE GetRawHandle();

private:
	VOID SortCacheBuffers();
	HRESULT IncreaseCacheBuffers(LONG NewCount);
	HRESULT CacheReadBlocks(LONGLONG StartOffset, LONG BlockCount, BOOL bForCacheWrite, CONST vector<LONGLONG> & NotAlignedOffsetTemp); //<sonmi01>2013-8-26 ###???
	size_t FindOffset(LONGLONG Offset);

private:
	const CCacheIO & operator=(const CCacheIO &);
	CCacheIO(const CCacheIO &);
};





//////////////////////////////////////////////////////////////////////////
class CBulkWrite : public ICCacheIO
{
private:
	CComPtr<IFileDevice> m_pFileDevice; /***INCOMMING***/
	BOOL m_bAsOwner; /***INCOMMING***/

	LONGLONG	m_CurrentPointer; /***INTERNAL***/
	CacheBuffer m_cache; /***Internal***/
	LONG		m_refCount; /***INTERNAL***/

public:
	//bufferSize must be 2^N
	CBulkWrite(IFileDevice * pFileDevice, BOOL bAsOwner, LONG bufferSize);

	//HRESULT Seek(LONGLONG FilePointer);
	//HRESULT WriteAtOffset(LONGLONG StartOffset, LONG len, CONST BYTE * Buffer);
	//HRESULT Write(LONG len, CONST BYTE * Buffer);
	//HRESULT Flush();

	//virtual ~CBulkWrite();
	IMP_CLASS_DESTRUCTOR(CBulkWrite);


	//////////////////////////////////////////////////////////////////////////
	//struct ICCacheIO : public ISimpleUnknown
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
	IMP_WHO_AM_I(TEXT("class CBulkWrite : public ICCacheIO"));
	//	//	virtual HRESULT CreateMember() = 0;
	virtual HRESULT CreateMember();
	//	//	virtual HRESULT DestroyMember() = 0;
	virtual HRESULT DestroyMember();
	//	//};

	//	virtual HRESULT Seek(LONGLONG FilePointer) = 0;
	virtual HRESULT Seek(LONGLONG FilePointer);
	//	virtual HRESULT Read(LONG len, BYTE * Buffer) = 0;
	virtual HRESULT Read(LONG len, BYTE * Buffer);
	//	virtual HRESULT Write(LONG len, CONST BYTE * Buffer) = 0;
	virtual HRESULT Write(LONG len, CONST BYTE * Buffer);

	//	virtual HRESULT ReadAtOffset(LONGLONG StartOffset, LONG len, BYTE * Buffer) = 0;
	virtual HRESULT ReadAtOffset(LONGLONG StartOffset, LONG len, BYTE * Buffer);
	//	virtual HRESULT WriteAtOffset(LONGLONG StartOffset, LONG len, CONST BYTE * Buffer) = 0;
	virtual HRESULT WriteAtOffset(LONGLONG StartOffset, LONG len, CONST BYTE * Buffer);

	//	virtual HRESULT Flush() = 0; //if not called explicitly, it will be called when destructor is called
	virtual HRESULT Flush();
	//};


	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
	virtual HRESULT GetFileSize(LONGLONG * pFileSize);

	virtual HANDLE GetRawHandle();

private:
	HRESULT WriteAndUpdateCache(CONST LONGLONG StartOffset, CONST LONG len, CONST BYTE * Buffer);
};


//API
HRESULT CreateInstanceCCacheIO(
	IFileDevice * pFileDevice, BOOL bAsOwner, LONG BlockSize, BOOL bWriteNewAndSequential,
	OUT ICCacheIO ** ppobj,
	BOOL bCreateMember
	);

HRESULT CreateInstanceCBulkWrite(
	IFileDevice * pFileDevice, BOOL bAsOwner, LONG bufferSize,
	OUT ICCacheIO ** ppobj,
	BOOL bCreateMember
	);


