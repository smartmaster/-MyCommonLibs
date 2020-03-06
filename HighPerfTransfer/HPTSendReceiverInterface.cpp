#include "stdafx.h"

#include "HPTSendReceiverInterface.h"

DWORD WINAPI SendReceiverMonitorProc(IHPTSendReceive * pIHPTSendReceive)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = pIHPTSendReceive->WaitToEnd(INFINITE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("RunServer"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	hr = pIHPTSendReceive->StopProcess();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("StopProcess"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


HANDLE CreateSendReceiverMonitorThread(IHPTSendReceive * pIHPTSendReceive)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	DWORD ThreadId = 0;
	HANDLE hThread = CreateThread(
		NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
		0,//_In_       SIZE_T dwStackSize,
		(LPTHREAD_START_ROUTINE)(SendReceiverMonitorProc),//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
		(LPVOID)(pIHPTSendReceive),//_In_opt_   LPVOID lpParameter,
		0,//_In_       DWORD dwCreationFlags,
		&ThreadId //_Out_opt_  LPDWORD lpThreadId
		);
	if (FAILED(hr))
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateThread"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	return hThread;
}