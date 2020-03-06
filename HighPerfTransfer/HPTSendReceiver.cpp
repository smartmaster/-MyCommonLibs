#include "stdafx.h"

#include "..\PublicHeader\PublicBitOP.h"

#include "HPTHelper.h"
#include "HPTSendReceiver.h"
#include "CommandList.h"



CHPTSendReceive::CHPTSendReceive(IFileDevice * pSender, IFileDevice * pReceiver, BOOL bAttachSenderReceiver, IHPTServerHandlerFactory * pHPTNewHandlerFactory, BOOL bServer) :
m_pSender(pSender),
m_pReceiver(pReceiver),
m_pHPTServerHandlerFactory(pHPTNewHandlerFactory),
m_pHPTServerHandler(NULL),
m_pHPTClientHandler(NULL),
m_bServer(bServer),
m_bAttachSenderReceiver(bAttachSenderReceiver),
m_RefCount(0)
{
	ZeroMemory(m_hThread, sizeof(m_hThread));
}

HRESULT CHPTSendReceive::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	m_apBufferSend.Allocate(S_BUFFER_SIZE);
	if (NULL == m_apBufferSend.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	m_apCircBufferSend.Attach(new CCircularBuffer(m_apBufferSend.m_pData, S_BUFFER_SIZE));
	if (NULL == m_apCircBufferSend.m_p)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("new"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_apCircBufferSend->CreateMember();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CircBufferSend->CreateMember"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_apBufferReceive.Allocate(S_BUFFER_SIZE);
	if (NULL == m_apBufferReceive.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	m_apCircBufferReceive.Attach(new CCircularBuffer(m_apBufferReceive.m_pData, S_BUFFER_SIZE));
	if (NULL == m_apCircBufferReceive.m_p)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("new"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_apCircBufferReceive->CreateMember();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CircBufferSend->CreateMember"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD ThreadId0 = 0;
	m_hThread[0] = CreateThread(
		NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
		0,//_In_       SIZE_T dwStackSize,
		ThreadSenderProc,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
		(LPVOID)this,//_In_opt_   LPVOID lpParameter,
		0,//_In_       DWORD dwCreationFlags,
		&ThreadId0//_Out_opt_  LPDWORD lpThreadId
		);
	if (NULL == m_hThread[0])
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateThread"), hr, TEXT("ThreadSenderProc"));
		LEAVE_BLOCK(0);
	}

	DWORD ThreadId1 = 0;
	m_hThread[1] = CreateThread(
		NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
		0,//_In_       SIZE_T dwStackSize,
		ThreadHandlerProc,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
		(LPVOID)this,//_In_opt_   LPVOID lpParameter,
		0,//_In_       DWORD dwCreationFlags,
		&ThreadId1//_Out_opt_  LPDWORD lpThreadId
		);
	if (NULL == m_hThread[1])
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateThread"), hr, TEXT("ThreadHandlerProc"));
		LEAVE_BLOCK(0);
	}


	DWORD ThreadId2 = 0;
	m_hThread[2] = CreateThread(
		NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
		0,//_In_       SIZE_T dwStackSize,
		ThreadReceiverProc,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
		(LPVOID)this,//_In_opt_   LPVOID lpParameter,
		0,//_In_       DWORD dwCreationFlags,
		&ThreadId2//_Out_opt_  LPDWORD lpThreadId
		);
	if (NULL == m_hThread[2])
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateThread"), hr, TEXT("ThreadReceiverProc"));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	CONST BOOL bManualReset[] = { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE}; //<sonmi01>2014-3-28 ###???
	hr = CreateInstanceCMultipleEventHelper(bManualReset, _countof(bManualReset), &m_spIMultipleEventHelper, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCMultipleEventHelper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CHPTSendReceive::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_apCircBufferSend.m_p)
	{
		m_apCircBufferSend->DestroyMember();
		m_apCircBufferSend.Free();
		m_apBufferSend.Free();
	}


	//////////////////////////////////////////////////////////////////////////
	if (m_apCircBufferReceive.m_p)
	{
		m_apCircBufferReceive->DestroyMember();
		m_apCircBufferReceive.Free();
		m_apBufferReceive.Free();
	}

	//////////////////////////////////////////////////////////////////////////

	if (m_bAttachSenderReceiver)
	{
		if (m_pReceiver)
		{
			m_pReceiver->Release();
			m_pReceiver = NULL;
		}

		if (m_pSender)
		{
			m_pSender->Release();
			m_pSender = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	for (LONG ii = 0; ii < _countof(m_hThread); ++ii)
	{
		if (m_hThread[ii])
		{
			CloseHandle(m_hThread[ii]);
			m_hThread[ii] = NULL;
		}
	}

	if (m_pHPTServerHandler)
	{
		m_pHPTServerHandler->Release();
		m_pHPTServerHandler = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	m_spIMultipleEventHelper.Release();

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

DWORD WINAPI CHPTSendReceive::ThreadSenderProc(LPVOID lpParameter)
{
	CHPTSendReceive * pHPTSendReceive = (CHPTSendReceive *)(lpParameter);
	return pHPTSendReceive->SenderProc();
}

HRESULT CHPTSendReceive::SenderProc()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apSendBuffer;
	apSendBuffer.Allocate(S_BUFFER_SIZE);
	if (NULL == apSendBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("size=%d"), S_BUFFER_SIZE);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG ProcessedLen = 0;
	LONG TotalDataWritten = 0;
	for (;;)
	{
		//////////////////////////////////////////////////////////////////////////
		ProcessedLen = 0;
		hr = m_apCircBufferSend->Consume(apSendBuffer.m_pData, S_BUFFER_SIZE, ProcessedLen);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Consume"), hr, TEXT(""));
			break;
		}

		if (0 == ProcessedLen)
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT("Consume"), hr, TEXT("size=%d"), ProcessedLen);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		TotalDataWritten = 0;
		hr = WriteAllHelper(m_pSender, ProcessedLen, &TotalDataWritten, apSendBuffer.m_pData);
		if (FAILED(hr) || (ProcessedLen != TotalDataWritten))
		{
			if (SUCCEEDED(hr))
			{
				hr = HRESULT_FROM_WIN32(ERROR_INCORRECT_SIZE);
			}
			D_API_ERR(0, TEXT("WriteAllHelper"), hr, TEXT("ProcessedLen=%d, TotalDataWritten=%d"), ProcessedLen, TotalDataWritten);
			break;
		}
		//////////////////////////////////////////////////////////////////////////
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	if (FAILED(hr))
	{
		m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::ERROR_INDEX);
		D_API_ERR(0, TEXT("Set"), E_FAIL, TEXT("ERROR_INDEX"));
	}
	m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::SENDER_INDEX); //<sonmi01>2014-3-28 ###???

	if (!m_bServer && m_pHPTClientHandler)
	{
		if (FAILED(hr))
		{
			hr = m_pHPTClientHandler->Set(NS_MultipleEventHelper::ERROR_INDEX);
		}
		else
		{
			hr = m_pHPTClientHandler->Set(NS_MultipleEventHelper::DONE_INDEX);
		}
		hr = m_pHPTClientHandler->Set(NS_MultipleEventHelper::SENDER_INDEX); //<sonmi01>2014-3-28 ###???
	}

	return hr;
}

DWORD WINAPI CHPTSendReceive::ThreadHandlerProc(LPVOID lpParameter)
{
	CHPTSendReceive * pHPTSendReceive = (CHPTSendReceive *)(lpParameter);
	return pHPTSendReceive->HandlerProc();
}

HRESULT CHPTSendReceive::HandlerProc()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	//BOOL bSystemError = FALSE;
	CONST LONG SYS_ERROR_INDEX = 0;
	CONST LONG HANDLER_ERROR_INDEX = 1;
	ULONG ErrorFlags = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	HPTHeader Header;
	HPTBody Body;

	//Header.Init();

	CONST LONG INITIAL_BODY_SIZE = 2 * 1024 * 1024;
	hr = Body.Allocate(INITIAL_BODY_SIZE);
	if (FAILED(hr))
	{
		SET_BIT(ErrorFlags, SYS_ERROR_INDEX);
		D_API_ERR(0, TEXT("Body.Allocate"), hr, TEXT("size=%d"), INITIAL_BODY_SIZE);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	IHPTHandler * pHandler = NULL;

	for (;;)
	{
		ErrorFlags = 0;

		//Header.Init();
		ZeroMemory(&Header, sizeof(Header));
		Body.Allocate(0);
		hr = GetCommmandHelper(Header, Body, m_apCircBufferReceive.m_p);
		if (FAILED(hr))
		{
			SET_BIT(ErrorFlags, SYS_ERROR_INDEX);
			D_API_ERR(0, TEXT("GetCommmandHelper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		if (m_bServer)
		{
			pHandler = UllToPtr<IHPTHandler>(Header.m_ServerHandler);//((IHPTHandler *)(ULONG_PTR)(Header.m_ServerHandler));
		}
		else
		{
			pHandler = UllToPtr<IHPTHandler>(Header.m_ClientHandler);//((IHPTHandler *)(ULONG_PTR)(Header.m_ClientHandler));
		}

		//////////////////////////////////////////////////////////////////////////
		if (pHandler)
		{
			//////////////////////////////////////////////////////////////////////////
			if (m_bServer)
			{
				if (pHandler == m_pHPTServerHandler)
				{
					hr = pHandler->ProcessCommand(Header, Body, m_apCircBufferSend.m_p, m_apCircBufferReceive.m_p);
				}
				else
				{
					hr = S_OK;
					D_API_ERR(0, TEXT("Handler"), E_POINTER, TEXT("Server handler not match, ignore this command. [HeaderHandler=0x%p, CurrentHandler=0x%p]"), pHandler, m_pHPTClientHandler);
				}
			}
			else
			{
				if (pHandler == m_pHPTClientHandler)
				{
					hr = pHandler->ProcessCommand(Header, Body, NULL, NULL);
				}
				else
				{
					hr = S_OK;
					D_API_ERR(0, TEXT("Handler"), E_POINTER, TEXT("Client handler not match, ignore this command. [HeaderHandler=0x%p, CurrentHandler=0x%p]"), pHandler, m_pHPTClientHandler);
				}
			}
			//////////////////////////////////////////////////////////////////////////
			if (FAILED(hr))
			{
				SET_BIT(ErrorFlags, HANDLER_ERROR_INDEX);
				D_API_ERR(0, TEXT("pHandler->Process"), hr, TEXT(""));
				break;
			}
		}
		else if (m_bServer && m_pHPTServerHandlerFactory)
		{

			//////////////////////////////////////////////////////////////////////////
			if (m_pHPTServerHandler)
			{
				m_pHPTServerHandler->Release();
				m_pHPTServerHandler = NULL;
			}

			if (CMD_END_APP_REQ == Header.m_CommandCode)
			{
				D_INFO(0, TEXT("Received CMD_END_APP_REQ"));
				hr = m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::DONE_INDEX);
				if (FAILED(hr))
				{
					SET_BIT(ErrorFlags, SYS_ERROR_INDEX);
					D_API_ERR(0, TEXT("IMultipleEventHelper->Set"), hr, TEXT(""));
					break;
				}
			}
			else
			{
				hr = m_pHPTServerHandlerFactory->CreateServerHandler(Header, Body, &m_pHPTServerHandler, m_apCircBufferSend.m_p, m_apCircBufferReceive.m_p);
				if (FAILED(hr))
				{
					SET_BIT(ErrorFlags, HANDLER_ERROR_INDEX);
					D_API_ERR(0, TEXT("HPTNewHandlerFactory->CreateNewHandler"), hr, TEXT(""));
					break;
				}
			}
			
		}
		else
		{
			SET_BIT(ErrorFlags, SYS_ERROR_INDEX);
			hr = E_UNEXPECTED;
			D_API_ERR(0, TEXT("Process"), hr, TEXT("invalid handler"));
			break;
		}
		//////////////////////////////////////////////////////////////////////////
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	LONG WaitIndexTemp = (FAILED(hr) ? NS_MultipleEventHelper::ERROR_INDEX : NS_MultipleEventHelper::DONE_INDEX);
	hr = m_spIMultipleEventHelper->Set(WaitIndexTemp);
	D_API_ERR(0, TEXT("Set"), hr, TEXT("WaitIndex=%d"), WaitIndexTemp);
	hr = m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::HANDLER_INDEX); //<sonmi01>2014-3-28 ###???

	if (!m_bServer && m_pHPTClientHandler)
	{
		if (FAILED(hr))
		{
			hr = m_pHPTClientHandler->Set(NS_MultipleEventHelper::ERROR_INDEX);
		}
		else
		{
			hr = m_pHPTClientHandler->Set(NS_MultipleEventHelper::DONE_INDEX);
		}
		hr = m_pHPTClientHandler->Set(NS_MultipleEventHelper::HANDLER_INDEX); //<sonmi01>2014-3-28 ###???
	}

	return hr;
}

HRESULT CHPTSendReceive::StopProcess()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	//////////////////////////////////////////////////////////////////////////
	m_apCircBufferSend->Stop();
	m_apCircBufferReceive->Stop();


	//////////////////////////////////////////////////////////////////////////
	//m_pSender->DestroyMember();
	//m_pReceiver->DestroyMember();

	BOOL bCancelIoEx = CancelIoEx(
		m_pReceiver->GetRawHandle(),//_In_      HANDLE hFile,
		NULL//_In_opt_  LPOVERLAPPED lpOverlapped
		);
	if (!bCancelIoEx)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CancelIoEx"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	if (m_pSender != m_pReceiver)
	{
		bCancelIoEx = CancelIoEx(
			m_pSender->GetRawHandle(),//_In_      HANDLE hFile,
			NULL//_In_opt_  LPOVERLAPPED lpOverlapped
			);
		if (!bCancelIoEx)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("CancelIoEx"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
	}

	BOOL bCancelSynchronousIo = FALSE;
	for (LONG ii = 0; ii < _countof(m_hThread); ++ ii)
	{
		bCancelSynchronousIo = CancelSynchronousIo(m_hThread[ii]);
		if (!bCancelSynchronousIo)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("CancelSynchronousIo"), hr, TEXT("ThreadHandle=0x%p"), m_hThread[ii]);
			//break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	CONST LONG WAIT_COUNT = 30;
	for (LONG ii = 0; ii < WAIT_COUNT; ++ ii)
	{
		const DWORD dwMilliseconds = 1000 * 30; //15 MIN
		DWORD dwWait = WaitForMultipleObjects(
			_countof(m_hThread),//_In_  DWORD nCount,
			m_hThread,//_In_  const HANDLE *lpHandles,
			TRUE,//_In_  BOOL bWaitAll,
			dwMilliseconds//_In_  DWORD dwMilliseconds
			);
		if (dwWait >= WAIT_OBJECT_0 && dwWait < (WAIT_OBJECT_0 + _countof(m_hThread)))
		{
			D_INFO(0, TEXT("Sender and Receiver threads finished"));
			break;
		}
		else if (WAIT_TIMEOUT == dwWait)
		{
			//////////////////////////////////////////////////////////////////////////
			D_INFO(0, TEXT("Wait timeout, cancel IO and wait again"));
			bCancelIoEx = CancelIoEx(
				m_pReceiver->GetRawHandle(),//_In_      HANDLE hFile,
				NULL//_In_opt_  LPOVERLAPPED lpOverlapped
				);
			if (!bCancelIoEx)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("CancelIoEx"), hr, TEXT(""));
				//LEAVE_BLOCK(0);
			}

			if (m_pSender != m_pReceiver)
			{
				bCancelIoEx = CancelIoEx(
					m_pSender->GetRawHandle(),//_In_      HANDLE hFile,
					NULL//_In_opt_  LPOVERLAPPED lpOverlapped
					);
				if (!bCancelIoEx)
				{
					LastError = GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					D_API_ERR(0, TEXT("CancelIoEx"), hr, TEXT(""));
					//LEAVE_BLOCK(0);
				}
			}
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT("WaitResult=%d"), dwWait);
			break;
		}
	}
	

	for (LONG ii = 0; ii < _countof(m_hThread); ++ii)
	{
		if (WAIT_TIMEOUT == WaitForSingleObject(m_hThread[ii], 0))
		{
			TerminateThread(m_hThread[ii], E_FAIL);
			D_INFO(0, TEXT("SeTerminate thread %d"), ii);
		}
	}

	return hr;
}

DWORD WINAPI CHPTSendReceive::ThreadReceiverProc(LPVOID lpParameter)
{
	CHPTSendReceive * pHPTSendReceive = (CHPTSendReceive *)(lpParameter);
	return pHPTSendReceive->ReceiverProc();
}

HRESULT CHPTSendReceive::ReceiverProc()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apReceiveBuffer;
	apReceiveBuffer.Allocate(S_BUFFER_SIZE);
	if (NULL == apReceiveBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("size=%d"), S_BUFFER_SIZE);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	ULONG NumberOfBytesRead = 0;
	LONG TotalProcessedLen = 0;
	for (;;)
	{
		//////////////////////////////////////////////////////////////////////////
		NumberOfBytesRead = 0;
		hr = m_pReceiver->Read(S_BUFFER_SIZE, &NumberOfBytesRead, apReceiveBuffer.m_pData);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		
		if (0 == NumberOfBytesRead)
		{
			hr = HRESULT_FROM_WIN32(ERROR_INCORRECT_SIZE);
			D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
		
		//////////////////////////////////////////////////////////////////////////
		TotalProcessedLen = 0;
		hr = m_apCircBufferReceive->ProvideAll(apReceiveBuffer.m_pData, NumberOfBytesRead, TotalProcessedLen);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ProvideAll"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		if (NumberOfBytesRead != TotalProcessedLen)
		{
			hr = E_FAIL;
			D_API_ERR(0, TEXT("ProvideAll"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

	}
	if (FAILED(hr))
	{
		//D_API_ERR(0, TEXT(""), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	if (FAILED(hr))
	{
		m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::ERROR_INDEX);
		D_API_ERR(0, TEXT("Set"), E_FAIL, TEXT("ERROR_INDEX"));
	}
	m_spIMultipleEventHelper->Set(NS_MultipleEventHelper::RECEIVER_INDEX); //<sonmi01>2014-3-28 ###???

	if (!m_bServer && m_pHPTClientHandler)
	{
		if (FAILED(hr))
		{
			hr = m_pHPTClientHandler->Set(NS_MultipleEventHelper::ERROR_INDEX);
		}
		else
		{
			hr = m_pHPTClientHandler->Set(NS_MultipleEventHelper::DONE_INDEX);
		}
		hr = m_pHPTClientHandler->Set(NS_MultipleEventHelper::RECEIVER_INDEX); //<sonmi01>2014-3-28 ###???
	}
	
	return hr;
}

HRESULT CHPTSendReceive::PutCommandToSend(CONST HPTHeader & Header, CONST HPTBody & Body)
{
	return PutCommmandHelper(Header, Body, m_apCircBufferSend.m_p);
}

DWORD CHPTSendReceive::WaitToEnd(DWORD MillSec)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	DWORD dwWait = m_spIMultipleEventHelper->WaitMultiple(FALSE, MillSec);
	if (dwWait >= WAIT_OBJECT_0 && dwWait < WAIT_OBJECT_0 + m_spIMultipleEventHelper->GetEventCount())
	{
		D_INFO(0, TEXT("Server end, EventIndex=%d"), dwWait);
	}
	else
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT(""), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	return dwWait;
}

HRESULT CHPTSendReceive::SetClientHandler(IHPTHandler * pClientHandler)
{
	m_pHPTClientHandler = pClientHandler;
	return S_OK;
}

HRESULT CHPTSendReceive::Set(LONG WaitIndexe)
{
	return m_spIMultipleEventHelper->Set(WaitIndexe);
}


HRESULT PutCommmandHelper(CONST HPTHeader & Header, CONST HPTBody & Body, CCircularBuffer * pCircBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ATLASSERT(Header.m_PayloadLength == Body.m_DataSize);

	//CAutoWriterLock AutoWriterLock(pCircBuffer); //no need if only one writer

	LONG TotalProcessedLen = 0;
	//////////////////////////////////////////////////////////////////////////
	hr = pCircBuffer->ProvideAll((CONST BYTE*)&Header, Header.m_HeaderSize, TotalProcessedLen);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ProvideAll"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (Header.m_HeaderSize != TotalProcessedLen)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("ProvideAll"), hr, TEXT("HeaderSize=%d, TotalProcessedLen=%d"), Header.m_HeaderSize, TotalProcessedLen);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	if (0 == Body.m_DataSize)
	{
		LEAVE_BLOCK(0);
	}

	TotalProcessedLen = 0;
	hr = pCircBuffer->ProvideAll(Body.m_pData, Body.m_DataSize, TotalProcessedLen);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ProvideAll"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (Body.m_DataSize != TotalProcessedLen)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("ProvideAll"), hr, TEXT("HeaderSize=%d, TotalProcessedLen=%d"), Body.m_DataSize, TotalProcessedLen);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT GetCommmandHelper(HPTHeader & Header, HPTBody & Body, CCircularBuffer * pCircBuffer)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//CAutoReaderLock AutoReaderLock(pCircBuffer); //no need if only one reader

	LONG TotalProcessedLen = 0;
	//////////////////////////////////////////////////////////////////////////
	hr = pCircBuffer->ConsumeAll((BYTE*)&Header, sizeof(Header), TotalProcessedLen);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ConsumeAll"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (0 == TotalProcessedLen)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("ConsumeAll"), hr, TEXT("TotalProcessedLen=%d"), TotalProcessedLen);
		LEAVE_BLOCK(0);
	}

	if (Header.m_HeaderSize != TotalProcessedLen)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("ConsumeAll"), hr, TEXT("HeaderSize=%d, TotalProcessedLen=%d"), Header.m_HeaderSize, TotalProcessedLen);
		LEAVE_BLOCK(0);
	}

	//ATLASSERT(Header.m_HeaderSize == sizeof(Header)); //currently only one version of header supported
	//ATLASSERT(Header.m_HeaderVersion == MAKEWORD(0, 0));
	if (Header.m_HeaderSize != sizeof(Header) || Header.m_HeaderVersion != MAKEWORD(0, 0))
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("Invalid heeader"), hr, TEXT("HeaderSize=%d, HeaderVersion=%d"), Header.m_HeaderSize, Header.m_HeaderVersion);
		LEAVE_BLOCK(0);
	}



	//////////////////////////////////////////////////////////////////////////
	hr = Body.Allocate(Header.m_PayloadLength);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("PayloadLength=%d"), Header.m_PayloadLength);
		LEAVE_BLOCK(0);
	}

	if (0 == Header.m_PayloadLength)
	{
		LEAVE_BLOCK(0);
	}

	TotalProcessedLen = 0;
	hr = pCircBuffer->ConsumeAll(Body.m_pData, Body.m_DataSize, TotalProcessedLen);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ConsumeAll"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (Body.m_DataSize != TotalProcessedLen)
	{
		hr = E_FAIL;
		D_API_ERR(0, TEXT("ConsumeAll"), hr, TEXT("BodyDataSize=%d, TotalProcessedLen=%d"), Body.m_DataSize, TotalProcessedLen);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCHPTSendReceive(
	IFileDevice * pSender,
	IFileDevice * pReceiver,
	BOOL bAttachSenderReceiver,
	IHPTServerHandlerFactory * pHPTNewHandlerFactory,
	BOOL bServer,
	IHPTSendReceive  ** ppIHPTSendReceive,
	BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		new CHPTSendReceive(
		pSender,
		pReceiver,
		bAttachSenderReceiver,
		pHPTNewHandlerFactory,
		bServer),
		ppIHPTSendReceive,
		bCreateMember
		);
}