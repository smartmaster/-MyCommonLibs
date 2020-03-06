#include "stdafx.h"


#include <WinSock2.h>
#include <Ws2tcpip.h>


#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicVMemPtr.h"

#include "SocketClient.h"


namespace
{
	class CDrainSocket
	{
	private:
		IFileDevice * m_pSOcket; /***INCOMMING***/
		HANDLE m_event; /***INTERNAL***/ /***CLEANUP***/
		HANDLE m_hThread; /***INTERNAL***/ /***CLEANUP***/
	public:

		CDrainSocket(IFileDevice * pSocket) :
			m_pSOcket(pSocket),
			m_event(nullptr),
			m_hThread(nullptr)
		{
		}

		~CDrainSocket()
		{
			if (nullptr != m_event)
			{
				CloseHandle(m_event);
				m_event = nullptr;
			}

			if (nullptr != m_hThread)
			{
				TerminateThread(m_hThread, HRESULT_FROM_WIN32(ERROR_SYSTEM_PROCESS_TERMINATED));
				CloseHandle(m_hThread);
				m_hThread = nullptr;
			}
		}


		HRESULT WaitToDrain(ULONG TimeOut)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			/////////////////////////////////////////////////////////////////////////////
			m_event = CreateEvent(nullptr, TRUE, FALSE, nullptr);
			if (nullptr == m_event)
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("CreateEvent"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			DWORD ThreadId = 0;
			m_hThread = CreateThread(
				nullptr,//_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
				0,//_In_ SIZE_T dwStackSize,
				ThreadProcDrain,//_In_ LPTHREAD_START_ROUTINE lpStartAddress,
				this,//_In_opt_ __drv_aliasesMem LPVOID lpParameter,
				0,//_In_ DWORD dwCreationFlags,
				&ThreadId//_Out_opt_ LPDWORD lpThreadId
				);
			if (nullptr == m_hThread)
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("CreateThread"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			HANDLE handles[] = { m_hThread, m_event };
			DWORD dwWait = WAIT_FAILED;
			for (;;)
			{
				dwWait = WaitForMultipleObjects(_countof(handles), handles, FALSE, TimeOut);
				if (WAIT_OBJECT_0 == dwWait)
				{
					D_INFO(0, TEXT("Thread ended"));
					break;
				}
				else if (WAIT_OBJECT_0 + 1 == dwWait)
				{
					D_INFO(0, TEXT("Data read from socket"));
				}
				else if (WAIT_TIMEOUT == dwWait)
				{
					D_INFO(0, TEXT("WAIT_TIMEOUT, assume no more data to be sent"));
					break;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			if (WAIT_OBJECT_0 != dwWait)
			{
				//////////////////////////////////////////////////////////////////////////
				BOOL bcio = CancelIoEx(m_pSOcket->GetRawHandle(), nullptr);
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					LastError = GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("CancelIoEx"), hr, TEXT(""));
				}

				//////////////////////////////////////////////////////////////////////////
				dwWait = WaitForSingleObject(m_hThread, TimeOut);
				if (WAIT_OBJECT_0 != dwWait)
				{
					D_API_ERR(0, TEXT("WaitForSingleObject"), HRESULT_FROM_WIN32(dwWait), TEXT("TerminateThread"));
					TerminateThread(m_hThread, HRESULT_FROM_WIN32(ERROR_SYSTEM_PROCESS_TERMINATED));
				}
			}

			CloseHandle(m_hThread);
			m_hThread = nullptr;

			CloseHandle(m_event);
			m_event = nullptr;

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

	private:
		HRESULT Drain()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			CONST LONG BUFFER_SIZE = 1024 * 1024;
			CVMemPtr<BYTE> apBUffer;
			apBUffer.Allocate(BUFFER_SIZE);
			if (nullptr == apBUffer.m_pData)
			{
				//LastError = WSAGetLastError();
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			ULONG NumberOfBytesRead = 0;
			for (;;)
			{
				//////////////////////////////////////////////////////////////////////////
				NumberOfBytesRead = 0;
				::ResetEvent(m_event);
				hr = m_pSOcket->Read(BUFFER_SIZE, &NumberOfBytesRead, apBUffer.m_pData);
				if (FAILED(hr))
				{
					//LastError = WSAGetLastError();
					//LastError = GetLastError();
					//hr = HRESULT_FROM_WIN32(LastError);
					//D_SET_FISRT_STATUS(hr, LastError);
					//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
					//D_SET_LAST_STATUS(hr, LastError);
					//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
					D_API_ERR(0, TEXT("Read"), hr, TEXT(""));
					break;
				}

				if (0 == NumberOfBytesRead)
				{
					D_INFO(0, TEXT("NumberOfBytesRead == 0"));
					break;
				}

				::SetEvent(m_event);

			}
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);

			return hr;
		}

		static DWORD WINAPI ThreadProcDrain(
			_In_  LPVOID lpParameter
			)
		{
			CDrainSocket * pObj = (CDrainSocket *)(lpParameter);
			return pObj->Drain();
		}

	};
}//end namespace

HRESULT DrainSocket(IFileDevice * pSocket, ULONG TimeOut)
{
	CDrainSocket ds(pSocket);
	return ds.WaitToDrain(TimeOut);
}

//////////////////////////////////////////////////////////////////////////
LONG PopTransferedBuffers(LPWSABUF & Buffers, LONG & BufferCount, LONG Transferred)
{
	ATLASSERT(Transferred >= 0);

	LONG Index = -1;
	while (Transferred >= 0)
	{
		++ Index;
		if (Index >= BufferCount)
		{
			Index = BufferCount;
			break;
		}
		Transferred -= Buffers[Index].len;
	}

	BufferCount -= Index;

	if (BufferCount > 0)
	{
		Buffers += Index;
		Buffers->buf += (Buffers->len + Transferred);
		Buffers->len = (-Transferred);
	}

	return BufferCount;
}

static HRESULT SocketSendBuffers(SOCKET sock, LPWSABUF Buffers, LONG BufferCount, LONG * pnSent )
{

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	int ret = 0;
	DWORD NumberOfBytesSent = 0;

	while (BufferCount)
	{
		NumberOfBytesSent = 0;
		ret = WSASend(
			sock,//_In_   SOCKET s,
			Buffers,//_In_   LPWSABUF lpBuffers,
			BufferCount,//_In_   DWORD dwBufferCount,
			&NumberOfBytesSent,//_Out_  LPDWORD lpNumberOfBytesSent,
			0,//_In_   DWORD dwFlags,
			NULL,//_In_   LPWSAOVERLAPPED lpOverlapped,
			NULL//_In_   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);
		if (0 != ret)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("WSASend"), hr, TEXT(""));
			break;
		}

		if (pnSent)
		{
			*pnSent += NumberOfBytesSent;
		}
		
		PopTransferedBuffers(Buffers, BufferCount, NumberOfBytesSent);
	}

	return hr;
}

static HRESULT SocketReceiveBuffers(SOCKET sock, LPWSABUF Buffers, LONG BufferCount, LONG * pnRecv )
{

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	int ret = 0;
	DWORD NumberOfBytesRecv = 0;

	while (BufferCount)
	{
		NumberOfBytesRecv = 0;
		ret = WSARecv(
			sock,//_In_   SOCKET s,
			Buffers,//_In_   LPWSABUF lpBuffers,
			BufferCount,//_In_   DWORD dwBufferCount,
			&NumberOfBytesRecv,//_Out_  LPDWORD lpNumberOfBytesRecvd,
			0,//_In_   DWORD dwFlags,
			NULL,//_In_   LPWSAOVERLAPPED lpOverlapped,
			NULL//_In_   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);
		if (0 != ret)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("WSARecv"), hr, TEXT(""));
			break;
		}

		if (pnRecv)
		{
			*pnRecv += NumberOfBytesRecv;
		}
		PopTransferedBuffers(Buffers, BufferCount, NumberOfBytesRecv);
	}

	return hr;
}


static HRESULT SocketSend(SOCKET sock, LPCVOID pBuffer, LONG nToSend, LONG * pnSent, BOOL bSendAll)
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	CONST CHAR * pBufferChar = (CONST CHAR *)pBuffer;
	ULONG ulDataTransered = 0;
	INT nCurrentSent = 0;

	BOOL bConnectionClosed = FALSE;

	while (nToSend)
	{
		nCurrentSent = send(sock, pBufferChar + ulDataTransered, nToSend, 0);
		if (nCurrentSent > 0)
		{
			ulDataTransered += nCurrentSent;
			nToSend -= nCurrentSent;
			if (!bSendAll) //<sonmi01>2013-12-6 ###???
			{
				break; //send only once
			}
		}
		else if (0 == nCurrentSent)
		{
			bConnectionClosed = TRUE;
			D_INFO(0, TEXT("Connection closing"));
			break;
		}
		else
		{
			dwLastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			D_SET_LAST_STATUS(hr, dwLastError);
			D_API_ERR(0, TEXT("send"), dwLastError, TEXT("nCurrentSent=%u, hr=%u"), nCurrentSent, hr);
			break;
		}
	}

	if (S_OK == hr && !bConnectionClosed)
	{
		if (pnSent)
		{
			*pnSent = ulDataTransered;
		}

	}

	return hr;
}

static HRESULT SocketReceive(SOCKET sock, LPVOID pBuffer, LONG nToReceive, LONG * pnReceived, BOOL bReceiveAll)
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	CHAR * pBufferChar = (CHAR *)pBuffer;
	ULONG ulDataTransered = 0;
	INT nCurrentReceive = 0;

	BOOL bConnectionClosed = FALSE;

	while (nToReceive)
	{
		nCurrentReceive = recv(sock, pBufferChar + ulDataTransered, nToReceive, 0);
		if (nCurrentReceive > 0)
		{
			ulDataTransered += nCurrentReceive;
			nToReceive -= nCurrentReceive;
			if (!bReceiveAll) //<sonmi01>2013-12-6 ###???
			{
				break; //receive only once
			}
		}
		else if (0 == nCurrentReceive)
		{
			bConnectionClosed = TRUE;
			D_INFO(0, TEXT("Connection closing"));
			break;
		}
		else
		{
			dwLastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			D_SET_LAST_STATUS(hr, dwLastError);
			D_API_ERR(0, TEXT("send"), dwLastError, TEXT("nCurrentSent=%u, hr=%u"), nCurrentReceive, hr);
			break;
		}
	}

	if (S_OK == hr && !bConnectionClosed)
	{
		if (pnReceived)
		{
			*pnReceived = ulDataTransered;
		}
	}

	return hr;
}


HRESULT CSocketClient::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;
	INT nRetCode = 0;

	addrinfo * pResult = NULL; //to cleanup

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	addrinfo hints;
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	CStringA strServerA(m_strServer.GetString());
	CStringA strPortA(m_Port);
	nRetCode = getaddrinfo(strServerA.GetString(), strPortA.GetString(), &hints, &pResult);
	if (0 != nRetCode)
	{
		dwLastError = WSAGetLastError();
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_SET_LAST_STATUS(hr, dwLastError);
		D_API_ERR(0, TEXT("getaddrinfo"), dwLastError, TEXT("rc=%u, hr=%u"), nRetCode, hr);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	SOCKET SocketConnection = INVALID_SOCKET;
	for(addrinfo * ptr=pResult; ptr != NULL; ptr=ptr->ai_next) 
	{
		TCHAR szAddressString[128] = { 0 };
		DWORD wAddressStringLength = _countof(szAddressString);
		WSAAddressToString(
			ptr->ai_addr,//_In_      LPSOCKADDR lpsaAddress,
			ptr->ai_addrlen,//_In_      DWORD dwAddressLength,
			NULL,//_In_opt_  LPWSAPROTOCOL_INFO lpProtocolInfo,
			szAddressString,//_Inout_   LPTSTR lpszAddressString,
			&wAddressStringLength//_Inout_   LPDWORD lpdwAddressStringLength
			);


		SocketConnection = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (INVALID_SOCKET == SocketConnection)
		{
			dwLastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			D_SET_LAST_STATUS(hr, dwLastError);
			D_API_ERR(0, TEXT("socket"), dwLastError, TEXT("socket=%u, hr=%u"), SocketConnection, hr);
			continue;
		}

		nRetCode = connect(SocketConnection, ptr->ai_addr, ptr->ai_addrlen);
		if (SOCKET_ERROR == nRetCode) 
		{
			dwLastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			D_SET_LAST_STATUS(hr, dwLastError);
			D_API_ERR(0, TEXT("connect"), dwLastError, TEXT("socket=%u, hr=%u"), SocketConnection, hr);
			closesocket(SocketConnection);
			SocketConnection = INVALID_SOCKET;
			continue;
		}

		break;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	freeaddrinfo(pResult);
	pResult = NULL;


	//////////////////////////////////////////////////////////////////////////
	m_pSockWapper = new CSocketHandleWrapper(SocketConnection, TRUE, m_bSendAll, m_bReceiveAll);
	if (NULL == m_pSockWapper)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		hr = E_OUTOFMEMORY;
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("new CSocketHandleWrapper"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_pSockWapper->CreateMember();
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateMember"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);


	//////////////////////////////////////////////////////////////////////////
	if (pResult)
	{
		freeaddrinfo(pResult);
		pResult = NULL;
	}

	return hr;
}

HRESULT CSocketClient::Write(ULONG nToSend, ULONG * pnSent,  LPCBYTE pBuffer)
{
#if 1
	return m_pSockWapper->Write(nToSend, pnSent, pBuffer);
#else
	LONG Sent = 0;

	HRESULT hr = SocketSend(m_SocketConnection, pBuffer, nToSend, &Sent, m_bSendAll); //<sonmi01>2013-12-6 ###???
	if (SUCCEEDED(hr))
	{
		m_TotalSent += Sent;
		if (pnSent)
		{
			*pnSent = Sent;
		}
	}

	return hr;
#endif
	
}

HRESULT CSocketClient::Read(ULONG nToReceive, ULONG * pnReceived,  BYTE* pBuffer)
{
#if 1
	return m_pSockWapper->Read(nToReceive, pnReceived, pBuffer);
#else
	LONG Received = 0;
	HRESULT hr = SocketReceive(m_SocketConnection, pBuffer, nToReceive, &Received, m_bReceiveAll); //<sonmi01>2013-12-6 ###???
	if (SUCCEEDED(hr))
	{
		m_TotalReceived += Received;
		if (pnReceived)
		{
			*pnReceived = Received;
		}
	}

	return hr;
#endif

}

HRESULT CSocketClient::Disconnect()
{
#if 1
	return m_pSockWapper->Disconnect();
#else
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	INT nRetCode = shutdown(m_SocketConnection, SD_BOTH);
	if (SOCKET_ERROR == nRetCode)
	{
		dwLastError = WSAGetLastError();
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_SET_LAST_STATUS(hr, dwLastError);
		D_API_ERR(0, TEXT("socket"), dwLastError, TEXT("rc=%u, hr=%u"), nRetCode, hr);
	}

	return hr;
#endif


}


CSocketClient::CSocketClient(LPCTSTR pszServer, LPCTSTR pPort, BOOL bSendAll, BOOL bReceiveAll) :
	m_strServer(pszServer),
	m_Port(pPort),
	//m_SocketConnection(INVALID_SOCKET),
	m_bSendAll(bSendAll), //<sonmi01>2013-12-6 ###???
	m_bReceiveAll(bReceiveAll),
	m_RefCount(0),
	//m_TotalSent(0),
	//m_TotalReceived(0),
	m_pSockWapper(NULL)
{

}

HRESULT CSocketClient::DestroyMember()
{
#if 1
	HRESULT hr = S_OK;
	if (m_pSockWapper)
	{
		m_pSockWapper->DestroyMember();
		m_pSockWapper = NULL;
	}
	return hr;
#else
	HRESULT hr = S_OK;
	if (INVALID_SOCKET != m_SocketConnection)
	{
		D_INFO(0, TEXT("Socket=0x%p, TotalReceived=%I64d, TotalSent=%I64d"), m_SocketConnection, m_TotalReceived, m_TotalSent);
		closesocket(m_SocketConnection);
		m_SocketConnection = INVALID_SOCKET;
	}
	return hr;
#endif


}

LONGLONG CSocketClient::GetTotalSentSize()
{
#if 1
	return m_pSockWapper->GetTotalSentSize();
#else
	return m_TotalSent;
#endif

}

LONGLONG CSocketClient::GetTotalReceivedSize()
{
#if 1
	return m_pSockWapper->GetTotalReceivedSize();
#else
	return m_TotalReceived;
#endif

}

HRESULT CSocketClient::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
#if 1
	return m_pSockWapper->SetPointerEx( llDistanceToMove, lpNewFilePointer, ulMoveMethod );
#else
	if (lpNewFilePointer)
	{
		*lpNewFilePointer = 0;
	}
	return S_OK;
#endif

}

HRESULT CSocketClient::GetFileSize( LONGLONG * pFileSize )
{
#if 1
	return m_pSockWapper->GetFileSize( pFileSize );
#else
	if (pFileSize)
	{
		*pFileSize = 0;
	}
	return S_OK;
#endif

}

HRESULT CSocketClient::SendLengthAndBuffer( LONG Length, CONST BYTE * Buffer, LONG * LengthSent )
{
#if 1
	return m_pSockWapper->SendLengthAndBuffer( Length, Buffer, LengthSent );
#else
	WSABUF wsaBuff[2] = { 0 };

	LONG SendLen = htonl(Length);
	wsaBuff[0].len = sizeof(SendLen);
	wsaBuff[0].buf = (CHAR*)&SendLen;

	wsaBuff[1].len = Length;
	wsaBuff[1].buf = (CHAR*)Buffer;

	LONG Sent = 0;

	HRESULT hr = SocketSendBuffers(m_SocketConnection, wsaBuff, _countof(wsaBuff), &Sent);

	if (SUCCEEDED(hr))
	{
		m_TotalSent += Sent;
		if (LengthSent)
		{
			*LengthSent = Sent;
		}
	}

	return hr;
#endif
	
}

HANDLE CSocketClient::GetRawHandle()
{
#if 1
	return m_pSockWapper->GetRawHandle();
#else
	return (HANDLE)m_SocketConnection;
#endif
	
}

HRESULT CreateInstanceCSocketClient(LPCTSTR pszServer, LPCTSTR pPort, ISocketClient ** ppObj, BOOL bSendAll, BOOL bReceiveAll, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(/*CSocketClient,*/ (new CSocketClient(pszServer, pPort, bSendAll, bReceiveAll)), ppObj, bCreateMember); //<sonmi01>2013-12-6 ###???
}

CSocketHandleWrapper::CSocketHandleWrapper(SOCKET SocketConnection, BOOL bAttachSocketHandle, BOOL bSendAll, BOOL bReceiveAll) :
m_SocketConnection(SocketConnection),
m_bAttachSocketHandle(bAttachSocketHandle),
m_bSendAll(bSendAll), //<sonmi01>2013-12-6 ###???
m_bReceiveAll(bReceiveAll),
m_RefCount(0),
m_TotalSent(0),
m_TotalReceived(0)
{

}

HRESULT CSocketHandleWrapper::CreateMember()
{
	return S_OK;
}

HRESULT CSocketHandleWrapper::DestroyMember()
{
	HRESULT hr = S_OK;

	if (m_bAttachSocketHandle)
	{
		if (INVALID_SOCKET != m_SocketConnection)
		{
			D_INFO(0, TEXT("Socket=0x%p, TotalSent=%I64d, TotalReceived=%I64d"), m_SocketConnection, m_TotalSent, m_TotalReceived);
			closesocket(m_SocketConnection);
			m_SocketConnection = INVALID_SOCKET;
		}
	}

	return hr;
}

HRESULT CSocketHandleWrapper::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	if (lpNewFilePointer)
	{
		*lpNewFilePointer = 0;
	}
	return S_OK;
}

HRESULT CSocketHandleWrapper::Read(ULONG nToReceive, ULONG * pnReceived, BYTE* pBuffer)
{
	LONG Received = 0;
	HRESULT hr = SocketReceive(m_SocketConnection, pBuffer, nToReceive, &Received, m_bReceiveAll); //<sonmi01>2013-12-6 ###???
	if (SUCCEEDED(hr))
	{
		m_TotalReceived += Received;
		if (pnReceived)
		{
			*pnReceived = Received;
		}
	}

	return hr;
}

HRESULT CSocketHandleWrapper::Write(ULONG nToSend, ULONG * pnSent, LPCBYTE pBuffer)
{
	LONG Sent = 0;

	HRESULT hr = SocketSend(m_SocketConnection, pBuffer, nToSend, &Sent, m_bSendAll); //<sonmi01>2013-12-6 ###???
	if (SUCCEEDED(hr))
	{
		m_TotalSent += Sent;
		if (pnSent)
		{
			*pnSent = Sent;
		}
	}

	return hr;
}

HRESULT CSocketHandleWrapper::GetFileSize(LONGLONG * pFileSize)
{
	if (pFileSize)
	{
		*pFileSize = 0;
	}
	return S_OK;
}

HRESULT CSocketHandleWrapper::Disconnect()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;
	if (m_bAttachSocketHandle)
	{
		INT nRetCode = shutdown(m_SocketConnection, SD_BOTH);
		if (SOCKET_ERROR == nRetCode)
		{
			dwLastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			D_SET_LAST_STATUS(hr, dwLastError);
			D_API_ERR(0, TEXT("socket"), dwLastError, TEXT("rc=%u, hr=%u"), nRetCode, hr);
		}
	}

	return hr;
}

LONGLONG CSocketHandleWrapper::GetTotalSentSize()
{
	return m_TotalSent;
}

LONGLONG CSocketHandleWrapper::GetTotalReceivedSize()
{
	return m_TotalReceived;
}

HRESULT CSocketHandleWrapper::SendLengthAndBuffer(LONG Length, CONST BYTE * Buffer, LONG * LengthSent)
{
	WSABUF wsaBuff[2] = { 0 };

	LONG SendLen = htonl(Length);
	wsaBuff[0].len = sizeof(SendLen);
	wsaBuff[0].buf = (CHAR*)&SendLen;

	wsaBuff[1].len = Length;
	wsaBuff[1].buf = (CHAR*)Buffer;

	LONG Sent = 0;

	HRESULT hr = SocketSendBuffers(m_SocketConnection, wsaBuff, _countof(wsaBuff), &Sent);

	if (SUCCEEDED(hr))
	{
		m_TotalSent += Sent;
		if (LengthSent)
		{
			*LengthSent = Sent;
		}
	}

	return hr;
}

HANDLE CSocketHandleWrapper::GetRawHandle()
{
	return (HANDLE)m_SocketConnection;
}


HRESULT CreateInstanceCSocketHandleWrapper(SOCKET SocketConnection, BOOL bAttachSocketHandle, ISocketClient ** ppObj, BOOL bSendAll, BOOL bReceiveAll, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(/*CSocketClient,*/ (new CSocketHandleWrapper(SocketConnection, bAttachSocketHandle, bSendAll, bReceiveAll)), ppObj, bCreateMember);
}