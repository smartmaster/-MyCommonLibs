#pragma once

#include "stdafx.h"
#include "SimpleBitArray.h"
#include "MTQueque.h"

struct CBufferItem
{
	OVERLAPPED m_Overlapped;
	LONGLONG m_offset;
	LONGLONG m_dataLength;
	LPVOID m_pBuffer;

public:
	CBufferItem();
	~CBufferItem();
	HRESULT CreateMember();

	HRESULT DestroyMember();

public:
	static ULONG BUFF_LEN/*32 * 1024*/;
};

class CIocpAsyncCopy
{
private:
	IN HANDLE m_hSource;
	IN HANDLE m_hDest; 
	

	IN LONGLONG m_totalSourceSize;
	IN CSimpleBitArray m_bitArray;
	IN ULONG m_BlockSize;
	IN LONGLONG m_llOffsetDiff;
	
	HANDLE m_hIocp;
	NS_MTQ::CSharedBuffer<CBufferItem> m_sharedBuffers;
	LONGLONG m_TotalRead;
	LONGLONG m_TotalWrite;
	ULONG m_ErrorCount;
	
public:
	CIocpAsyncCopy
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

	HRESULT Copy();

	HRESULT DestroyMember();
	virtual ~CIocpAsyncCopy();

	static DWORD WINAPI WorkerThreadProc(__in  LPVOID lpParameter);

private:
	HRESULT ReadData(LONGLONG llStartOffset, LONGLONG llDataLength);
	HRESULT OnIocp();
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

HRESULT TestFileCopy(CTestFileCopyCmdLine & cmdLine/*LPCTSTR pszSource, LPCTSTR pszDest*/);