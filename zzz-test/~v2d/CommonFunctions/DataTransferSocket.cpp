#include "stdafx.h"
#include "DataTransferSocket.h"
#include "DataTransferBuffer.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


//////////////////////////////////////////////////////////////////////////
HRESULT NS_DATATRANSFER::CSocketServer::DestroyMember()
{
	HRESULT hr = S_OK;

	if (INVALID_SOCKET != m_SocketConnection)
	{
		closesocket(m_SocketConnection);
		m_SocketConnection = INVALID_SOCKET;
	}

	if (INVALID_SOCKET != m_SocketListen)
	{
		closesocket(m_SocketListen);
		m_SocketListen = INVALID_SOCKET;
	}

	return hr;
}

NS_DATATRANSFER::CSocketServer::~CSocketServer()
{
	DestroyMember();
}

HRESULT NS_DATATRANSFER::CSocketServer::Release()
{
	delete this;
	return S_OK;
}

HRESULT NS_DATATRANSFER::CSocketServer::Create()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;
	INT nRetCode = 0;

	addrinfo * pResult = NULL; //to cleanup

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		INT ai_family = AF_UNSPEC;
		if (4 == m_nIP4or6)
		{
			ai_family = AF_INET;
		}
		else if (6 == m_nIP4or6)
		{
			ai_family = AF_INET6;
		}

		addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = ai_family;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		//////////////////////////////////////////////////////////////////////////

		CStringA strPortNumber;
		strPortNumber.Format("%u", m_PortNumber);
		nRetCode = getaddrinfo(NULL, strPortNumber.GetString(), &hints, &pResult);
		if (0 != nRetCode)
		{
			dwLastError = WSAGetLastError();
			hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("getaddrinfo"), dwLastError, TEXT("rc=%u, hr=%u"), nRetCode, hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		m_SocketListen = socket(pResult->ai_family, pResult->ai_socktype, pResult->ai_protocol);
		if (m_SocketListen == INVALID_SOCKET)
		{
			dwLastError = WSAGetLastError();
			hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("socket"), dwLastError, TEXT("rc=%u, hr=%u"), m_SocketListen, hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		nRetCode = bind(m_SocketListen, pResult->ai_addr, (INT)pResult->ai_addrlen);
		if (SOCKET_ERROR == nRetCode)
		{
			dwLastError = WSAGetLastError();
			hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("bind"), dwLastError, TEXT("rc=%u, hr=%u"), nRetCode, hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		nRetCode = listen(m_SocketListen, SOMAXCONN);
		if (SOCKET_ERROR == nRetCode) 
		{
			dwLastError = WSAGetLastError();
			hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("listen"), dwLastError, TEXT("rc=%u, hr=%u"), nRetCode, hr));
			break;
		}

	} while (FALSE);

	//////////////////////////////////////////////////////////////////////////
	if (pResult)
	{
		freeaddrinfo(pResult);
		pResult = NULL;
	}

	return hr;
}

HRESULT NS_DATATRANSFER::CSocketServer::WaiForConnect()
{
	HRESULT hr= S_OK;
	DWORD dwLastError = 0;

	m_SocketConnection = accept(m_SocketListen, NULL, NULL);
	if (m_SocketConnection == INVALID_SOCKET) 
	{
		dwLastError = WSAGetLastError();
		hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
		_OLD_DSETLASTSTATUS(hr);
		_OLD_DAPIERR((0, DRNPOS, TEXT("socket"), dwLastError, TEXT("rc=%u, hr=%u"), m_SocketListen, hr));
	}

	closesocket(m_SocketListen);
	m_SocketListen = INVALID_SOCKET;

	return hr;
}

NS_DATATRANSFER::CSocketServer::CSocketServer( ULONG nIP4or6, ULONG PortNumber ) :
m_nIP4or6(nIP4or6),
m_PortNumber(PortNumber),
m_SocketListen(INVALID_SOCKET),
m_SocketConnection(INVALID_SOCKET)
{
}

namespace /*NS_NONAME*/
{
	static HRESULT SocketSendOnce(SOCKET sock, LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent )
	{
		HRESULT hr = S_OK;
		DWORD dwLastError = 0;

		CONST CHAR * pBufferChar = (CONST CHAR *)pBuffer;
		ULONG ulDataTransered = 0;
		INT nCurrentSent = 0;

		BOOL bConnectionClosed = FALSE;

		nCurrentSent = send(sock, pBufferChar + ulDataTransered, nToSend, 0);
		if (nCurrentSent > 0)
		{
			ulDataTransered += nCurrentSent;
			nToSend -= nCurrentSent;
		}
		else if (0 == nCurrentSent)
		{
			bConnectionClosed = TRUE;
			_OLD_DINFO((0, DRNPOS, TEXT("Connetion closing")));
			//break;
		}
		else
		{
			dwLastError = WSAGetLastError();
			hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("send"), dwLastError, TEXT("nCurrentSent=%u, hr=%u"), nCurrentSent, hr));
			//break;
		}

		if (S_OK == hr && !bConnectionClosed)
		{
			*pnSent = ulDataTransered;
		}

		return hr;
	}

	static HRESULT SocketSend(SOCKET sock, LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent )
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
			}
			else if (0 == nCurrentSent)
			{
				bConnectionClosed = TRUE;
				_OLD_DINFO((0, DRNPOS, TEXT("Connetion closing")));
				break;
			}
			else
			{
				dwLastError = WSAGetLastError();
				hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("send"), dwLastError, TEXT("nCurrentSent=%u, hr=%u"), nCurrentSent, hr));
				break;
			}
		}

		if (S_OK == hr && !bConnectionClosed)
		{
			*pnSent = ulDataTransered;
		}

		return hr;
	}

	static HRESULT SocketReceiveOnce(SOCKET sock, LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived )
	{
		HRESULT hr = S_OK;
		DWORD dwLastError = 0;

		CHAR * pBufferChar = (CHAR *)pBuffer;
		ULONG ulDataTransered = 0;
		INT nCurrentReceive = 0;

		BOOL bConnectionClosed = FALSE;

		nCurrentReceive = recv(sock, pBufferChar + ulDataTransered, nToReceive, 0);
		if (nCurrentReceive > 0)
		{
			ulDataTransered += nCurrentReceive;
			nToReceive -= nCurrentReceive;
		}
		else if (0 == nCurrentReceive)
		{
			bConnectionClosed = TRUE;
			_OLD_DINFO((0, DRNPOS, TEXT("Connetion closing")));
			//break;
		}
		else
		{
			dwLastError = WSAGetLastError();
			hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("send"), dwLastError, TEXT("nCurrentSent=%u, hr=%u"), nCurrentReceive, hr));
			//break;
		}

		if (S_OK == hr && !bConnectionClosed)
		{
			*pnReceived = ulDataTransered;
		}

		return hr;
	}

	static HRESULT SocketReceive(SOCKET sock, LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived )
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
			}
			else if (0 == nCurrentReceive)
			{
				bConnectionClosed = TRUE;
				_OLD_DINFO((0, DRNPOS, TEXT("Connetion closing")));
				break;
			}
			else
			{
				dwLastError = WSAGetLastError();
				hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("send"), dwLastError, TEXT("nCurrentSent=%u, hr=%u"), nCurrentReceive, hr));
				break;
			}
		}

		if (S_OK == hr && !bConnectionClosed)
		{
			*pnReceived = ulDataTransered;
		}

		return hr;
	}

}

HRESULT NS_DATATRANSFER::CSocketServer::Send( LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent )
{
	return SocketSend(m_SocketConnection, pBuffer, nToSend, pnSent);
}

HRESULT NS_DATATRANSFER::CSocketServer::SendOnce( LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent )
{
	return SocketSendOnce(m_SocketConnection, pBuffer, nToSend, pnSent);
}

HRESULT NS_DATATRANSFER::CSocketServer::Receive( LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived )
{
	return SocketReceive(m_SocketConnection, pBuffer, nToReceive, pnReceived );
}

HRESULT NS_DATATRANSFER::CSocketServer::ReceiveOnce( LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived )
{
	return SocketReceiveOnce(m_SocketConnection, pBuffer, nToReceive, pnReceived );
}

HRESULT NS_DATATRANSFER::CSocketServer::Disconnect()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	INT nRetCode = shutdown(m_SocketConnection, SD_BOTH);
	if (SOCKET_ERROR == nRetCode)
	{
		dwLastError = WSAGetLastError();
		hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
		_OLD_DSETLASTSTATUS(hr);
		_OLD_DAPIERR((0, DRNPOS, TEXT("socket"), dwLastError, TEXT("rc=%u, hr=%u"), nRetCode, hr));
	}

	return hr;
}

HRESULT NS_DATATRANSFER::CSocketServer::Close()
{
	return DestroyMember();
}

NS_DATATRANSFER::IServer * NS_DATATRANSFER::CreateSocketServerInterface( ULONG nIP4or6, ULONG PortNumber )
{
	NS_DATATRANSFER::IServer * pServer = new NS_DATATRANSFER::CSocketServer(nIP4or6, PortNumber);
	return pServer;
}


HRESULT NS_DATATRANSFER::CSocketClient::ConnetToServer()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;
	INT nRetCode = 0;

	addrinfo * pResult = NULL; //to cleanup

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		addrinfo hints;
		ZeroMemory( &hints, sizeof(hints) );
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		CStringA strServerA(m_strServer.GetString());
		CStringA strPortA;
		strPortA.Format("%u", m_Port);
		nRetCode = getaddrinfo(strServerA.GetString(), strPortA.GetString(), &hints, &pResult);
		if (0 != nRetCode)
		{
			dwLastError = WSAGetLastError();
			hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("getaddrinfo"), dwLastError, TEXT("rc=%u, hr=%u"), nRetCode, hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		for(addrinfo * ptr=pResult; ptr != NULL; ptr=ptr->ai_next) 
		{
			m_SocketConnection = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (INVALID_SOCKET == m_SocketConnection) 
			{
				dwLastError = WSAGetLastError();
				hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("socket"), dwLastError, TEXT("socket=%u, hr=%u"), m_SocketConnection, hr));
				break;
			}

			nRetCode = connect(m_SocketConnection, ptr->ai_addr, ptr->ai_addrlen);
			if (SOCKET_ERROR == nRetCode) 
			{
				closesocket(m_SocketConnection);
				m_SocketConnection = INVALID_SOCKET;
				continue;
			}
			break;
		}
		if (FAILED(hr))
		{
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		freeaddrinfo(pResult);
		pResult = NULL;

	} while (FALSE);


	//////////////////////////////////////////////////////////////////////////
	if (pResult)
	{
		freeaddrinfo(pResult);
		pResult = NULL;
	}

	return hr;
}

HRESULT NS_DATATRANSFER::CSocketClient::SendOnce( LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent )
{
	return SocketSendOnce(m_SocketConnection, pBuffer, nToSend, pnSent);
}

HRESULT NS_DATATRANSFER::CSocketClient::Send( LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent )
{
	return SocketSend(m_SocketConnection, pBuffer, nToSend, pnSent);
}

HRESULT NS_DATATRANSFER::CSocketClient::ReceiveOnce( LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived )
{
	return SocketReceiveOnce(m_SocketConnection, pBuffer, nToReceive, pnReceived);
}

HRESULT NS_DATATRANSFER::CSocketClient::Receive( LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived )
{
	return SocketReceive(m_SocketConnection, pBuffer, nToReceive, pnReceived);
}

HRESULT NS_DATATRANSFER::CSocketClient::Disconnect()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	INT nRetCode = shutdown(m_SocketConnection, SD_BOTH);
	if (SOCKET_ERROR == nRetCode)
	{
		dwLastError = WSAGetLastError();
		hr = MAKE_HRESULT(SEVERITY_ERROR, 0, dwLastError);
		_OLD_DSETLASTSTATUS(hr);
		_OLD_DAPIERR((0, DRNPOS, TEXT("socket"), dwLastError, TEXT("rc=%u, hr=%u"), nRetCode, hr));
	}

	return hr;
}

HRESULT NS_DATATRANSFER::CSocketClient::Close()
{
	return DestroyMember();
}

NS_DATATRANSFER::CSocketClient::CSocketClient( LPCTSTR pszServer, ULONG nPort ) :
m_strServer(pszServer),
	m_Port(nPort),
	m_SocketConnection(INVALID_SOCKET)
{

}

HRESULT NS_DATATRANSFER::CSocketClient::DestroyMember()
{
	HRESULT hr = S_OK;
	if (INVALID_SOCKET != m_SocketConnection)
	{
		closesocket(m_SocketConnection);
		m_SocketConnection = INVALID_SOCKET;
	}
	return hr;
}

NS_DATATRANSFER::CSocketClient::~CSocketClient()
{
	DestroyMember();
}

HRESULT NS_DATATRANSFER::CSocketClient::Release()
{
	delete this;
	return S_OK;
}

HRESULT NS_DATATRANSFER::CSocketClient::Create()
{
	return S_OK;
}

NS_DATATRANSFER::IClient * NS_DATATRANSFER::CreateSocketClientInterface( LPCTSTR pszServer, ULONG nPort )
{
	IClient * pClient = new NS_DATATRANSFER::CSocketClient(pszServer, nPort);
	return pClient;
}


INT ut_socket_main(int argc, _TCHAR* argv[])
{
	WSADATA wsaData = {0};
	WSAStartup(MAKEWORD(2,2), &wsaData);

	CONST ULONG COUNT = 1024*1024;

	if (1 == argc)
	{
		NS_DATATRANSFER::IServer * pServer = NS_DATATRANSFER::CreateSocketServerInterface(4, 27075);

		pServer->Create();
		pServer->WaiForConnect();


		//CAutoVectorPtr<INT> apBuffer(new INT[COUNT]);
		//ULONG nReceived = 0;
		//pServer->Receive(apBuffer.m_p, COUNT*sizeof(INT), &nReceived);

		CLengthBuffer LenBuffer;
		for (INT ii=0; ii<5; ++ii)
		{
			NS_DATATRANSFER::ReceiveLengthBuffer(pServer, LenBuffer);
		}

		pServer->Disconnect();
		pServer->Close();

		if (pServer)
		{
			pServer->Release();
			pServer = NULL;
		}
	}
	else
	{
		NS_DATATRANSFER::IClient * pClient = NS_DATATRANSFER::CreateSocketClientInterface(argv[1], 27075);

		pClient->Create();
		pClient->ConnetToServer();

		
		//CAutoVectorPtr<INT> apBuffer(new INT[COUNT]);
		//for (ULONG ii=0; ii<COUNT; ++ii)
		//{
		//	apBuffer.m_p[ii] = ii;
		//}

		//for (INT ii=0; ii<16; ++ii)
		//{
		//	ULONG nSent = 0;
		//	pClient->Send(apBuffer.m_p, /*COUNT*/10*sizeof(INT), &nSent);
		//}

		CONST ULONG BLOCK = 1024*1024;
		CLengthBuffer LenBuffer;
		for (INT ii=0; ii<5; ++ii)
		{
			LenBuffer.Allocate(ii*BLOCK);
			if (LenBuffer.GetDataLength())
			{
				memset(LenBuffer.GetDataBuffer(), ii, LenBuffer.GetDataLength());
			}
			NS_DATATRANSFER::SendLengthBuffer(pClient, LenBuffer);
		}

		
		pClient->Disconnect();
		pClient->Close();

		if (pClient)
		{
			pClient->Release();
			pClient = NULL;
		}
	}
	WSACleanup();
	return 0;
}