#include "stdafx.h"
#include "WSASocketContext.h"


TWSASocketContext::TWSASocketContext( SOCKET ConnectedSocket, ULONG AllocatedSize, IResponceHandler	*	pResponceHandler) :
m_ConnectedSocket(ConnectedSocket),
m_IoOperation(IO_READ),
m_AllocatedSize(AllocatedSize),
m_RecvBuffer(AllocatedSize),
m_SendBuffer(AllocatedSize),
m_pResponceHandler(pResponceHandler)
{
	ZeroMemory(&m_Overlapped, sizeof(WSAOVERLAPPED));
}

HRESULT TWSASocketContext::CreateMemer()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		hr = m_RecvBuffer.CreateMember();
		if (FAILED(hr))
		{
			DAPIERR((0, DRNPOS, TEXT("CreateMember"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		hr = m_SendBuffer.CreateMember();
		if (FAILED(hr))
		{
			DAPIERR((0, DRNPOS, TEXT("CreateMember"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
	} while (FALSE);

	return hr;
}

HRESULT TWSASocketContext::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (m_ConnectedSocket != INVALID_SOCKET)
	{
		closesocket(m_ConnectedSocket);
		m_ConnectedSocket = INVALID_SOCKET;
	}

	if (NULL != m_pResponceHandler)
	{
		m_pResponceHandler->Release();
		m_pResponceHandler = NULL;
	}

	m_RecvBuffer.DestroyMember();
	m_SendBuffer.DestroyMember();

	return hr;
}

TWSASocketContext::~TWSASocketContext()
{
	DestroyMember();
}

VOID TWSASocketContext::PrepareForRead()
{
	m_IoOperation = IO_READ;
	m_RecvBuffer.m_DataLengthReady = FALSE;
	m_RecvBuffer.m_DataLength = 0;
	m_RecvBuffer.m_Transfered = 0;
}

VOID TWSASocketContext::PrepareForWrite()
{
	m_IoOperation = IO_WRITE;
	m_SendBuffer.m_DataLengthReady = FALSE;
	m_SendBuffer.m_DataLength = 0;
	m_SendBuffer.m_Transfered = 0;
}

HRESULT TWSASocketContext::OnReadCompleted( ULONG NumberOfBytes )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	WSABUF WsaBuffer = {0};
	ULONG NumberOfBytesXfered = 0;
	ULONG SockIoFlags = 0;
	INT RetCode = 0;

	//////////////////////////////////////////////////////////////////////////
	if (m_RecvBuffer.m_DataLengthReady)
	{
		m_RecvBuffer.m_Transfered += NumberOfBytes;
	}
	else
	{
		m_RecvBuffer.m_DataLengthReady = TRUE;
		m_RecvBuffer.m_Transfered = 0;

		if (EXIT_LENGTH != m_RecvBuffer.m_DataLength && m_RecvBuffer.m_DataLength > m_RecvBuffer.m_AllocatedSize)
		{
			hr = m_RecvBuffer.RellocateBuffer(m_RecvBuffer.m_DataLength);
			if (FAILED(hr))
			{
				DAPIERR((0, DRNPOS, TEXT("RellocateBuffer"), hr));
				//break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (EXIT_LENGTH == m_RecvBuffer.m_DataLength)
	{
		DINFO((0, DRNPOS, TEXT("Received END CONNECTION request")));
		PrepareForWrite();
		m_SendBuffer.m_DataLength = EXIT_LENGTH;
		WsaBuffer.len = sizeof(m_SendBuffer.m_DataLength);
		WsaBuffer.buf = (CHAR*)&m_SendBuffer.m_DataLength;
		NumberOfBytesXfered = 0;
		SockIoFlags = 0;
		RetCode = WSASend(
			m_ConnectedSocket,//__in   SOCKET s,
			&WsaBuffer,//__in   LPWSABUF lpBuffers,
			1,//__in   DWORD dwBufferCount,
			&NumberOfBytesXfered,//__out  LPDWORD lpNumberOfBytesSent,
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
				DSETLASTSTATUS(hr);
				DAPIERR((0, DRNPOS, TEXT("WSASend"), hr));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	else if (m_RecvBuffer.m_DataLength > m_RecvBuffer.m_Transfered)
	{
		WsaBuffer.len = m_RecvBuffer.m_DataLength - m_RecvBuffer.m_Transfered;
		WsaBuffer.buf = (CHAR*)(m_RecvBuffer.m_pBuffer + m_RecvBuffer.m_Transfered);

		NumberOfBytesXfered = 0;
		SockIoFlags = 0;
		RetCode = WSARecv(
			m_ConnectedSocket,//__in     SOCKET s,
			&WsaBuffer,//__inout  LPWSABUF lpBuffers,
			1,//__in     DWORD dwBufferCount,
			&NumberOfBytesXfered,//__out    LPDWORD lpNumberOfBytesRecvd,
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
				DSETLASTSTATUS(hr);
				DAPIERR((0, DRNPOS, TEXT("WSARecv"), hr));
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	else
	{
		ATLASSERT(m_RecvBuffer.m_DataLength == m_RecvBuffer.m_Transfered);
		PrepareForWrite();
		if (m_pResponceHandler)	
		{
			hr = m_pResponceHandler->Process(m_RecvBuffer, m_SendBuffer);
		}
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("ResponceHandler->Process"), hr));
		}
		else
		{
			WsaBuffer.len = sizeof(m_SendBuffer.m_DataLength);
			WsaBuffer.buf = (CHAR*)&m_SendBuffer.m_DataLength;
			NumberOfBytesXfered = 0;
			SockIoFlags = 0;
			RetCode = WSASend(
				m_ConnectedSocket,//__in   SOCKET s,
				&WsaBuffer,//__in   LPWSABUF lpBuffers,
				1,//__in   DWORD dwBufferCount,
				&NumberOfBytesXfered,//__out  LPDWORD lpNumberOfBytesSent,
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
					DSETLASTSTATUS(hr);
					DAPIERR((0, DRNPOS, TEXT("WSASend"), hr));
				}
			}
		}
	}

	return hr;
}


HRESULT TWSASocketContext::OnWriteCompleted( ULONG NumberOfBytes )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	WSABUF WsaBuffer = {0};
	ULONG NumberOfBytesXfered = 0;
	ULONG SockIoFlags = 0;
	INT RetCode = 0;

	//////////////////////////////////////////////////////////////////////////
	if (m_SendBuffer.m_DataLengthReady)
	{
		m_SendBuffer.m_Transfered += NumberOfBytes;
	}
	else
	{
		m_SendBuffer.m_DataLengthReady = TRUE;
		m_SendBuffer.m_Transfered = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	if (EXIT_LENGTH == m_SendBuffer.m_DataLength)
	{
		DINFO((0, DRNPOS, TEXT("Send END CONNECTION response")));
		hr = EXIT_LENGTH;
	}
	//////////////////////////////////////////////////////////////////////////
	else if (m_SendBuffer.m_DataLength > m_SendBuffer.m_Transfered)
	{
		WsaBuffer.len = m_SendBuffer.m_DataLength - m_SendBuffer.m_Transfered;
		WsaBuffer.buf = (CHAR*)(m_SendBuffer.m_pBuffer + m_SendBuffer.m_Transfered);

		NumberOfBytesXfered = 0;
		SockIoFlags = 0;
		RetCode = WSASend(
			m_ConnectedSocket,//__in   SOCKET s,
			&WsaBuffer,//__in   LPWSABUF lpBuffers,
			1,//__in   DWORD dwBufferCount,
			&NumberOfBytesXfered,//__out  LPDWORD lpNumberOfBytesSent,
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
				DSETLASTSTATUS(hr);
				DAPIERR((0, DRNPOS, TEXT("WSASend"), hr));
			}
		}
	}
	else
	{
		ATLASSERT(m_SendBuffer.m_DataLength == m_SendBuffer.m_Transfered);
		PrepareForRead();
		WsaBuffer.len = sizeof(m_RecvBuffer.m_DataLength);
		WsaBuffer.buf = (CHAR*)(&m_RecvBuffer.m_DataLength);

		NumberOfBytesXfered = 0;
		SockIoFlags = 0;
		RetCode = WSARecv(
			m_ConnectedSocket,//__in     SOCKET s,
			&WsaBuffer,//__inout  LPWSABUF lpBuffers,
			1,//__in     DWORD dwBufferCount,
			&NumberOfBytesXfered,//__out    LPDWORD lpNumberOfBytesRecvd,
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
				DSETLASTSTATUS(hr);
				DAPIERR((0, DRNPOS, TEXT("WSARecv"), hr));
			}
		}
	}

	return hr;
}

HRESULT TWSASocketContext::OnIOCompleted( ULONG NumberOfBytes )
{
	HRESULT hr = S_OK;
	switch (m_IoOperation)
	{
	case IO_READ:
		hr = OnReadCompleted(NumberOfBytes);
		break;

	case IO_WRITE:
		hr = OnWriteCompleted(NumberOfBytes);
		break;

	default:
		hr = E_FAIL;
		DSETLASTSTATUS(hr);
		DAPIERR((0, DRNPOS, TEXT(""), hr, TEXT("Invalid operation received [%u]"), m_IoOperation));
		break;
	}

	return hr;
}


