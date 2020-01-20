#pragma once

#include "stdafx.h"
#include "SimpleBitArray.h"
#include "MTQueque.h"
#include "PerformanceTiming.h"


struct AsyncIOCompleteRoutineApp
{
	struct CBufferItem
	{
		OVERLAPPED m_Overlapped;
		LPVOID m_pBuffer;
		LONGLONG m_offset;
		LONGLONG m_dataLength;
		LPVOID m_pThis;

	public:
		CBufferItem();
		~CBufferItem();

		HRESULT CreateMember();
		HRESULT DestroyMember();

	public:
		static ULONG BUFF_LEN/*32 * 1024*/;
	};


	class CAsyncCopy
	{
	private:
		IN HANDLE m_hSource;
		IN HANDLE m_hDest; 
		IN LONGLONG m_totalSourceSize;
		IN CSimpleBitArray m_bitArray;
		IN ULONG m_BlockSize;
		IN LONGLONG m_llOffsetDiff;

		NS_MTQ::CSharedBuffer<CBufferItem> m_sharedBuffers;
		LONGLONG m_TotalRead;
		LONGLONG m_TotalWrite;
		ULONG m_ErrorCount;
		HANDLE m_hRWFinished;

	public:
		CAsyncCopy
			(
			IN HANDLE hSource,
			IN HANDLE hDest, 
			IN LONGLONG totalSourceSize,
			IN LPVOID pBitArray,
			IN LONGLONG llBitCount,
			IN ULONG BlockSize,
			IN LONGLONG llOffsetDiff
			);

		HRESULT CreateMember(ULONG nBufferItemCount, ULONG nBufferSize);
		HRESULT DestroyMember();

		virtual ~CAsyncCopy();

		HRESULT Copy();

	private:
		HRESULT ReadData(LONGLONG llStartOffset, LONGLONG llDataLength);

		VOID MyOnReadComplete(
			__in  DWORD dwErrorCode,
			__in  DWORD dwNumberOfBytesTransfered,
			__in  LPOVERLAPPED lpOverlapped);

		static VOID CALLBACK OnReadComplete(
			__in  DWORD dwErrorCode,
			__in  DWORD dwNumberOfBytesTransfered,
			__in  LPOVERLAPPED lpOverlapped);

		VOID MyOnWriteComplete(
			__in  DWORD dwErrorCode,
			__in  DWORD dwNumberOfBytesTransfered,
			__in  LPOVERLAPPED lpOverlapped);

		static VOID CALLBACK OnWriteComplete(
			__in  DWORD dwErrorCode,
			__in  DWORD dwNumberOfBytesTransfered,
			__in  LPOVERLAPPED lpOverlapped);
	};

	struct CTestFileCopyCmdLine
	{
		CString m_strSource;
		CString m_strDest;
		BOOL m_bSourceBufferIO;
		BOOL m_bDestBufferIO;
		ULONG m_RWBufferSize;
		ULONG m_RWBuffersCount;

		CTestFileCopyCmdLine();
		VOID Parse(INT argc, TCHAR ** argv);
	};

	static HRESULT Main(CTestFileCopyCmdLine & cmdLine);
};
