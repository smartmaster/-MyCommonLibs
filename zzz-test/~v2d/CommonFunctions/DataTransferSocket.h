#pragma once

#include "DataTransfer.h"
#include <winsock2.h>
#include <ws2tcpip.h>

namespace NS_DATATRANSFER
{
	class CSocketServer : public IServer
	{
		friend IServer * CreateSocketServerInterface(ULONG nIP4or6, ULONG PortNumber);
	private:
		ULONG m_nIP4or6;
		ULONG m_PortNumber;
		SOCKET m_SocketListen; //to cleanup
		SOCKET m_SocketConnection; //to cleanup

		CSocketServer(ULONG nIP4or6, ULONG PortNumber);
		HRESULT DestroyMember();
		virtual ~CSocketServer();
		virtual HRESULT Release();

		virtual HRESULT Create();
		virtual HRESULT WaiForConnect();
		virtual HRESULT SendOnce(LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent);
		virtual HRESULT Send(LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent);
		virtual HRESULT ReceiveOnce(LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived);
		virtual HRESULT Receive(LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived);
		virtual HRESULT Disconnect();
		virtual HRESULT Close();
	};

	class CSocketClient : public IClient
	{
		friend IClient * CreateSocketClientInterface(LPCTSTR pszServer, ULONG nPort);
	private:
		CString m_strServer;
		ULONG m_Port;
		SOCKET m_SocketConnection;

		CSocketClient(LPCTSTR pszServer, ULONG nPort);
		HRESULT DestroyMember();
		virtual ~CSocketClient();
		virtual HRESULT Release();
		virtual HRESULT Create();

		virtual HRESULT ConnetToServer();
		virtual HRESULT SendOnce(LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent);
		virtual HRESULT Send(LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent);
		virtual HRESULT ReceiveOnce(LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived);
		virtual HRESULT Receive(LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived);
		virtual HRESULT Disconnect();
		virtual HRESULT Close();
		//virtual HRESULT Release();
	};
}