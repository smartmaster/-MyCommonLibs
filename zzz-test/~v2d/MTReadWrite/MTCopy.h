#pragma once

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>

#include "DTraceLib.h"
#include "FileDeviceInterface.h"
#include "MTPoolAndQueue.h"
#include "VMemPtr.h"
#include "SimpleBitArray.h"


struct TMTBufferItem
{
	ULONG m_BufferSize;
	ULONG m_DataSize;
	LONGLONG m_Offset;
	BYTE * m_pData; /***Internal to cleanup***/

	TMTBufferItem();
	HRESULT CreateMember();
	HRESULT DestroyMember();

private:
	static CONST ULONG s_DataBufferSize = 1024 * 1024 * 2;
};

class CMTCopy
{
private:
	IFileDevice * m_pSource; /***Incomming***/
	IFileDevice * m_pDest; /***Incomming***/
	LONGLONG m_SourceStartOffset; /***Incomming***/
	LONGLONG m_Diff; /***Incomming***/  //m_Diff = DestOffset - SourceOffset

	CSimpleBitArray * m_pSimpleBitArray; /***Incomming***/
	ULONG m_BlockSize; /***Incomming***/

	LONGLONG m_TotalRead;
	LONGLONG m_TotalWritten;

	HANDLE m_EventError; /***Internal to cleanup***/
	HANDLE m_EventAbort; /***Internal to cleanup***/
	HANDLE m_EventProviderFinish; /***Internal to cleanup***/
	CMTPool<TMTBufferItem> m_pool;  /***Internal to cleanup***/
	CMTQueue<TMTBufferItem> m_queue; /***Internal to cleanup***/


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
		ULONG BlockSize);
	virtual ~CMTCopy();

	HRESULT CreateMember();
	HRESULT DestroyMember();

	HRESULT Copy();
};


