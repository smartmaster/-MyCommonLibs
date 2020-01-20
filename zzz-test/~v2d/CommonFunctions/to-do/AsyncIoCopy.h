#pragma once

#include "stdafx.h"
#include "..\ServerFileDevice\ServerFileDevice_i.h"
#include "SimpleBitArray.h"
#include "IocpThreadPool.h"
#include "AsyncIoBuffer.h"
#include <map>
using namespace std;

struct CIoBuferMetadata
{
	LARGE_INTEGER m_liOffset;
	LARGE_INTEGER m_liLength;
};

struct CAsyncIoCopy;

class CIocpWorker
{
private: //******************************************
	CAsyncIoCopy *m_pAsyncIoCopy; //external object

private:
	INT OnError();

public: //******************************************
	typedef ULONG_PTR RequestType;

	//******************************************
	BOOL Initialize(void * pvParam) throw();
	void Execute(_In_ RequestType request, _In_ DWORD dwBytesTransfered, _In_ void *pvWorkerParam, _In_ OVERLAPPED *pOverlapped);
	void Terminate(void* /*pvParam*/, BOOL bAPIError, DWORD dwLastError ) throw();
}; // class CIocpWorker

struct CAsyncIoCopy
{
	IFileDeviceObject * m_pSource; //external object
	IFileDeviceObject * m_pDest; //external object
	CSimpleBitArray * m_pSimpleBitArray; //external object
	LONGLONG m_llOffsetDiff;
	LONGLONG m_liTotalData;
	LONGLONG m_llSourceStartOffset;
	ULONG m_ulBlockSize; 

	//******************************************
	CAsyncIoSharedBuffer m_AsyncIoSharedBuffer; //internal object
	LONG m_nErrorCount;
	HANDLE m_hEventFinished; //internal object
	LONGLONG m_liTotalRead;
	LONGLONG m_liTotalWrite;

	//******************************************
	static CONST ULONG IO_BUFFER_METADATA_LENGTH = sizeof(CIoBuferMetadata);
	static CONST ULONG IO_BUFFER_MAX_DATA_LENGTH = 2*1024*1024;
	static CONST ULONG IO_BUFFER_ITEM_COUNT = 16;

	//******************************************
	CAsyncIoCopy(	
		IFileDeviceObject * pSource, //external object
		IFileDeviceObject * pDest, //external object
		CSimpleBitArray * pSimpleBitArray, //external object
		LONGLONG llOffsetDiff,
		ULONG ulBlockSize,
		LONGLONG liTotalData,
		LONGLONG llSourceStartOffset
		);
	INT CreateMember();
	INT DestroyMember();
	~CAsyncIoCopy();

	//******************************************
	INT Copy(); //main thread;

private:
	INT ProcessBuffer(CIocpThreadPool<CIocpWorker> & threadPool, LONGLONG llStartOffset, LONGLONG llDataLength);

private: //******************************************
	CAsyncIoCopy(CONST CAsyncIoCopy &);
	CAsyncIoCopy & operator=(CONST CAsyncIoCopy &);
};


struct CSyncIoCopy
{
private:
	IFileDeviceObject * m_pSource; //external object
	IFileDeviceObject * m_pDest; //external object
	CONST map<LONGLONG, LONGLONG> & m_dataRange;
	LONGLONG m_llOffsetDiff;

	BYTE* m_pBuffer;
	static CONST ULONG IO_BUFFER_MAX_DATA_LENGTH = 2*1024*1024;

public:
	CSyncIoCopy(IFileDeviceObject * pSource, IFileDeviceObject * pDest, CONST map<LONGLONG, LONGLONG> & dataRange, LONGLONG llOffsetDiff);
	virtual ~CSyncIoCopy();
	INT Copy();

private:
	INT ProcessBuffer(LONGLONG llStartOffset, LONGLONG llDataLength);

};