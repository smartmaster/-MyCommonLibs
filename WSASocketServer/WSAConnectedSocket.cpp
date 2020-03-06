#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"

#include "WSAConnectedSocket.h"


WSAConnectedSocketT::WSAConnectedSocketT( SOCKET ConnectedSocket, ULONG AllocatedSize, IDataBufferHandler	*	pResponceHandler) :
	m_ConnectedSocket(ConnectedSocket),
	m_IoOperation(IO_RECEIVE),
	m_AllocatedSize(AllocatedSize),
	m_RecvBuffer(AllocatedSize),
	m_SendBuffer(AllocatedSize),
	m_pIDataBufferHandler(pResponceHandler),
	m_TotalSent(0),
	m_TotalReceived(0),
	m_StopTimer(TRUE)
{
	ZeroMemory(&m_Overlapped, sizeof(WSAOVERLAPPED));
}

HRESULT WSAConnectedSocketT::CreateMemer()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		hr = m_RecvBuffer.CreateMember();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("CreateMember"), hr, TEXT(""));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		hr = m_SendBuffer.CreateMember();
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("CreateMember"), hr, TEXT(""));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		LONGLONG llStart = m_StopTimer.Start();
		D_INFO(0, TEXT("Socket 0x%p start at %I64d"), m_ConnectedSocket, llStart);

		//////////////////////////////////////////////////////////////////////////
	} while (FALSE);

	return hr;
}

HRESULT WSAConnectedSocketT::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (m_ConnectedSocket != INVALID_SOCKET)
	{
		LONGLONG llEnd = m_StopTimer.End();
		LONGLONG llElapsed = m_StopTimer.Timing();
		D_INFO(0, TEXT("Socket=0x%p end at %I64d, TotalReceived=%I64d, TotalSent=%I64d, Elasped=%I64d MillSec"), m_ConnectedSocket, llEnd, m_TotalReceived, m_TotalSent, llElapsed);
		closesocket(m_ConnectedSocket);
		m_ConnectedSocket = INVALID_SOCKET;
	}

	if (NULL != m_pIDataBufferHandler)
	{
		m_pIDataBufferHandler->Release();
		m_pIDataBufferHandler = NULL;
	}

	m_RecvBuffer.DestroyMember();
	m_SendBuffer.DestroyMember();

	return hr;
}

WSAConnectedSocketT::~WSAConnectedSocketT()
{
	DestroyMember();
}

VOID WSAConnectedSocketT::PrepareForReceive()
{
	m_IoOperation = IO_RECEIVE;
	m_RecvBuffer.m_DataLengthReady = FALSE;
	m_RecvBuffer.m_DataLength = 0;
	m_RecvBuffer.m_Transfered = 0;
}

VOID WSAConnectedSocketT::PrepareForSend()
{
	m_IoOperation = IO_SEND;
	m_SendBuffer.m_DataLengthReady = FALSE;
	m_SendBuffer.m_DataLength = 0;
	m_SendBuffer.m_Transfered = 0;
}

HRESULT WSAConnectedSocketT::OnReceiveCompleted( ULONG NumberOfBytes )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	WSABUF WsaBuffer = {0};
	//ULONG NumberOfBytesXfered = 0;
	ULONG SockIoFlags = 0;
	INT RetCode = 0;

	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	m_TotalReceived += NumberOfBytes;

	//////////////////////////////////////////////////////////////////////////
	if (!m_RecvBuffer.m_DataLengthReady)
	{
		//data length is received

		//ATLASSERT(NumberOfBytes == sizeof(LONG));
		if (NumberOfBytes != sizeof(LONG))
		{
			hr = E_INVALID_PROTOCOL_FORMAT;
			D_API_ERR(0, TEXT("LengthError"), hr, TEXT("Invalid client-server protocol buffer length size. (NumberOfBytes)=(%d)"), NumberOfBytes);
			LEAVE_BLOCK(0);
		}

		m_RecvBuffer.m_DataLengthReady = TRUE; 
		m_RecvBuffer.m_Transfered = 0;
		m_RecvBuffer.m_DataLength = ntohl(m_RecvBuffer.m_DataLength);

		//////////////////////////////////////////////////////////////////////////
		if (EXIT_LENGTH == m_RecvBuffer.m_DataLength)
		{
			D_INFO(0, TEXT("%d length received, prepare for ending transfer"), m_RecvBuffer.m_DataLength);
		}
		else if (m_RecvBuffer.m_DataLength <= 0 || m_RecvBuffer.m_DataLength > MAX_BUFFER_SIZE)
		{
			hr = E_INVALID_PROTOCOL_FORMAT;
			D_API_ERR(0, TEXT("LengthError"), hr, TEXT("Invalid client-server protocol buffer length. (Received DataLength)=(%d)"), m_RecvBuffer.m_DataLength);
			LEAVE_BLOCK(0);
		}
		else if (m_RecvBuffer.m_DataLength > m_RecvBuffer.m_AllocatedSize)
		{
			hr = m_RecvBuffer.RellocateBuffer(m_RecvBuffer.m_DataLength, FALSE);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("RellocateBuffer"), hr, TEXT(""));
				LEAVE_BLOCK(0);
			}
		}
	}
	else
	{
		//data is received
		m_RecvBuffer.m_Transfered += NumberOfBytes;
	}

	//////////////////////////////////////////////////////////////////////////
	if (EXIT_LENGTH == m_RecvBuffer.m_DataLength) //client send CLOSE request
	{
		D_INFO(0, TEXT("Received END CONNECTION request"));
		PrepareForSend();

		m_SendBuffer.m_DataLength = htonl(EXIT_LENGTH);
		WsaBuffer.len = sizeof(m_SendBuffer.m_DataLength);
		WsaBuffer.buf = (CHAR*)&m_SendBuffer.m_DataLength;

		//NumberOfBytesXfered = 0;
		SockIoFlags = 0;
		RetCode = WSASend(
			m_ConnectedSocket,//__in   SOCKET s,
			&WsaBuffer,//__in   LPWSABUF lpBuffers,
			1,//__in   DWORD dwBufferCount,
			NULL,//&NumberOfBytesXfered,//__out  LPDWORD lpNumberOfBytesSent,
			SockIoFlags,//__in   DWORD dwFlags,
			&m_Overlapped,//__in   LPWSAOVERLAPPED lpOverlapped,
			NULL//__in   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);
		if (SOCKET_ERROR == RetCode)
		{
			LastError = WSAGetLastError();
			if (WSA_IO_PENDING != LastError)
			{
				hr = HRESULT_FROM_WIN32(LastError);
				D_SET_LAST_STATUS(hr, LastError);
				D_API_ERR(0, TEXT("WSASend"), hr, TEXT(""));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	else if (m_RecvBuffer.m_DataLength > m_RecvBuffer.m_Transfered) //more data to receive
	{
		WsaBuffer.len = m_RecvBuffer.m_DataLength - m_RecvBuffer.m_Transfered;
		WsaBuffer.buf = (CHAR*)(m_RecvBuffer.m_pData + m_RecvBuffer.m_Transfered);

		//NumberOfBytesXfered = 0;
		SockIoFlags = 0;
		RetCode = WSARecv(
			m_ConnectedSocket,//__in     SOCKET s,
			&WsaBuffer,//__inout  LPWSABUF lpBuffers,
			1,//__in     DWORD dwBufferCount,
			NULL,//&NumberOfBytesXfered,//__out    LPDWORD lpNumberOfBytesRecvd,
			&SockIoFlags,//__inout  LPDWORD lpFlags,
			&m_Overlapped,//__in     LPWSAOVERLAPPED lpOverlapped,
			NULL//__in     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);
		if (SOCKET_ERROR == RetCode)
		{
			LastError = WSAGetLastError();
			if (WSA_IO_PENDING != LastError)
			{
				hr = HRESULT_FROM_WIN32(LastError);
				D_SET_LAST_STATUS(hr, LastError);
				D_API_ERR(0, TEXT("WSARecv"), hr, TEXT(""));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	else if (m_RecvBuffer.m_DataLength == m_RecvBuffer.m_Transfered) //receive complete
	{
		//ATLASSERT(m_RecvBuffer.m_DataLength == m_RecvBuffer.m_Transfered);
		PrepareForSend();

		if (m_pIDataBufferHandler)	
		{
			hr = m_pIDataBufferHandler->Process(m_RecvBuffer, m_SendBuffer);
		}

		if (FAILED(hr))
		{
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("ResponceHandler->Process"), hr, TEXT(""));
		}
		else if (0 == m_SendBuffer.m_DataLength) //no need to send response
		{
			PrepareForReceive();

			WsaBuffer.len = sizeof(m_RecvBuffer.m_DataLength);
			WsaBuffer.buf = (CHAR*)(&m_RecvBuffer.m_DataLength);

			//NumberOfBytesXfered = 0;
			SockIoFlags = 0;
			RetCode = WSARecv(
				m_ConnectedSocket,//__in     SOCKET s,
				&WsaBuffer,//__inout  LPWSABUF lpBuffers,
				1,//__in     DWORD dwBufferCount,
				NULL,//&NumberOfBytesXfered,//__out    LPDWORD lpNumberOfBytesRecvd,
				&SockIoFlags,//__inout  LPDWORD lpFlags,
				&m_Overlapped,//__in     LPWSAOVERLAPPED lpOverlapped,
				NULL//__in     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
				);
			if (SOCKET_ERROR == RetCode)
			{
				LastError = WSAGetLastError();
				if (WSA_IO_PENDING != LastError)
				{
					hr = HRESULT_FROM_WIN32(LastError);
					D_SET_LAST_STATUS(hr, LastError);
					D_API_ERR(0, TEXT("WSARecv"), hr, TEXT(""));
				}
			}
		}
		else //initiate sending response
		{
			WSABUF WsaBufferSend[2] = {0};

			WsaBufferSend[1].len = m_SendBuffer.m_DataLength;
			WsaBufferSend[1].buf = (CHAR*)m_SendBuffer.m_pData;
			
			m_SendBuffer.m_DataLength = htonl(m_SendBuffer.m_DataLength);
			WsaBufferSend[0].len = sizeof(m_SendBuffer.m_DataLength);
			WsaBufferSend[0].buf = (CHAR*)&m_SendBuffer.m_DataLength;

			//NumberOfBytesXfered = 0;
			SockIoFlags = 0;
			RetCode = WSASend(
				m_ConnectedSocket,//__in   SOCKET s,
				WsaBufferSend,//__in   LPWSABUF lpBuffers,
				_countof(WsaBufferSend),//__in   DWORD dwBufferCount,
				NULL,//&NumberOfBytesXfered,//__out  LPDWORD lpNumberOfBytesSent,
				SockIoFlags,//__in   DWORD dwFlags,
				&m_Overlapped,//__in   LPWSAOVERLAPPED lpOverlapped,
				NULL//__in   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
				);
			if (SOCKET_ERROR == RetCode)
			{
				LastError = WSAGetLastError();
				if (WSA_IO_PENDING != LastError)
				{
					hr = HRESULT_FROM_WIN32(LastError);
					D_SET_LAST_STATUS(hr, LastError);
					D_API_ERR(0, TEXT("WSASend"), hr, TEXT(""));
				}
			}
		}
	}
	else
	{
		hr = E_INVALID_PROTOCOL_FORMAT;
		D_API_ERR(0, TEXT("WSARecv"), hr, TEXT("Invalid client-server protocol buffer format. (Received DataLength, Transfered)=(%d, %d)"), m_RecvBuffer.m_DataLength, m_RecvBuffer.m_Transfered);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


HRESULT WSAConnectedSocketT::OnSendCompleted( ULONG NumberOfBytes )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	WSABUF WsaBuffer = {0};
	//ULONG NumberOfBytesXfered = 0;
	ULONG SockIoFlags = 0;
	INT RetCode = 0;

	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	m_TotalSent += NumberOfBytes;

	//////////////////////////////////////////////////////////////////////////
	if (!m_SendBuffer.m_DataLengthReady)
	{
		//data length is sent

		//ATLASSERT(NumberOfBytes >= sizeof(LONG));
		if (NumberOfBytes < sizeof(LONG))
		{
			hr = E_INVALID_PROTOCOL_FORMAT;
			D_API_ERR(0, TEXT("LengthError"), hr, TEXT("Invalid client-server protocol buffer length size. (NumberOfBytes)=(%d)"), NumberOfBytes);
			LEAVE_BLOCK(0);
		}

		m_SendBuffer.m_DataLengthReady = TRUE;
		m_SendBuffer.m_Transfered = 0;
		m_SendBuffer.m_DataLength = ntohl(m_SendBuffer.m_DataLength);

		//////////////////////////////////////////////////////////////////////////
		if (EXIT_LENGTH == m_SendBuffer.m_DataLength)
		{
			D_INFO(0, TEXT("%d length sent, prepare for ending transfer"), m_SendBuffer.m_DataLength);
		}
		else if (m_SendBuffer.m_DataLength <= 0 || m_SendBuffer.m_DataLength > MAX_BUFFER_SIZE)
		{
			hr = E_INVALID_PROTOCOL_FORMAT;
			D_API_ERR(0, TEXT("LengthError"), hr, TEXT("Invalid client-server protocol buffer length. (Sent DataLength)=(%d)"), m_SendBuffer.m_DataLength);
			LEAVE_BLOCK(0);
		}
		m_SendBuffer.m_Transfered += (NumberOfBytes - sizeof(LONG));
	}
	else
	{
		//data is sent
		m_SendBuffer.m_Transfered += NumberOfBytes;
	}

	//////////////////////////////////////////////////////////////////////////
	if (EXIT_LENGTH == m_SendBuffer.m_DataLength) //response CLOSE request from client
	{
		D_INFO(0, TEXT("Send END CONNECTION response"));
		hr = EXIT_LENGTH;
	}
	//////////////////////////////////////////////////////////////////////////
	else if (m_SendBuffer.m_DataLength > m_SendBuffer.m_Transfered) //more data to send
	{
		WsaBuffer.len = m_SendBuffer.m_DataLength - m_SendBuffer.m_Transfered;
		WsaBuffer.buf = (CHAR*)(m_SendBuffer.m_pData + m_SendBuffer.m_Transfered);

		//NumberOfBytesXfered = 0;
		SockIoFlags = 0;
		RetCode = WSASend(
			m_ConnectedSocket,//__in   SOCKET s,
			&WsaBuffer,//__in   LPWSABUF lpBuffers,
			1,//__in   DWORD dwBufferCount,
			NULL,//&NumberOfBytesXfered,//__out  LPDWORD lpNumberOfBytesSent,
			SockIoFlags,//__in   DWORD dwFlags,
			&m_Overlapped,//__in   LPWSAOVERLAPPED lpOverlapped,
			NULL//__in   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);
		if (SOCKET_ERROR == RetCode)
		{
			LastError = WSAGetLastError();
			if (WSA_IO_PENDING != LastError)
			{
				hr = HRESULT_FROM_WIN32(LastError);
				D_SET_LAST_STATUS(hr, LastError);
				D_API_ERR(0, TEXT("WSASend"), hr, TEXT(""));
			}
		}
	}
	else if (m_SendBuffer.m_DataLength == m_SendBuffer.m_Transfered) //data sending complete, and initiate next receive
	{
		//ATLASSERT(m_SendBuffer.m_DataLength == m_SendBuffer.m_Transfered);
		PrepareForReceive();

		WsaBuffer.len = sizeof(m_RecvBuffer.m_DataLength);
		WsaBuffer.buf = (CHAR*)(&m_RecvBuffer.m_DataLength);

		//NumberOfBytesXfered = 0;
		SockIoFlags = 0;
		RetCode = WSARecv(
			m_ConnectedSocket,//__in     SOCKET s,
			&WsaBuffer,//__inout  LPWSABUF lpBuffers,
			1,//__in     DWORD dwBufferCount,
			NULL,//&NumberOfBytesXfered,//__out    LPDWORD lpNumberOfBytesRecvd,
			&SockIoFlags,//__inout  LPDWORD lpFlags,
			&m_Overlapped,//__in     LPWSAOVERLAPPED lpOverlapped,
			NULL//__in     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);
		if (SOCKET_ERROR == RetCode)
		{
			LastError = WSAGetLastError();
			if (WSA_IO_PENDING != LastError)
			{
				hr = HRESULT_FROM_WIN32(LastError);
				D_SET_LAST_STATUS(hr, LastError);
				D_API_ERR(0, TEXT("WSARecv"), hr, TEXT(""));
			}
		}
	}
	else
	{
		hr = E_INVALID_PROTOCOL_FORMAT;
		D_API_ERR(0, TEXT("WSARecv"), hr, TEXT("Invalid client-server protocol buffer format. (Sent DataLength, Transfered)=(%d, %d)"), m_RecvBuffer.m_DataLength, m_RecvBuffer.m_Transfered);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT WSAConnectedSocketT::OnIOCompleted( ULONG NumberOfBytes )
{
	HRESULT hr = S_OK;
	switch (m_IoOperation)
	{
	case IO_RECEIVE:
		hr = OnReceiveCompleted(NumberOfBytes);
		break;

	case IO_SEND:
		hr = OnSendCompleted(NumberOfBytes);
		break;

	default:
		hr = E_FAIL;
		D_SET_LAST_STATUS(hr, -1);
		D_API_ERR(0, TEXT("OnIOCompleted"), hr, TEXT("Invalid operation received [%u]"), m_IoOperation);
		break;
	}

	return hr;
}

LONGLONG WSAConnectedSocketT::GetTotalSentSize()
{
	return m_TotalSent;
}

LONGLONG WSAConnectedSocketT::GetTotalReceivedSize()
{
	return m_TotalReceived;
}


