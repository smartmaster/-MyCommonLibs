#pragma once

#include "stdafx.h"
//#include "LengthBuffer.h"

namespace NS_DATATRANSFER
{
	struct ISendReceive 
	{
		virtual HRESULT SendOnce(LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent) = NULL;
		virtual HRESULT Send(LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent) = NULL;
		virtual HRESULT ReceiveOnce(LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived) = NULL;
		virtual HRESULT Receive(LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived) = NULL;
	};

	struct IConnectionManager : public ISendReceive
	{
		virtual HRESULT Create() = NULL;
		virtual HRESULT Disconnect() = NULL;
		virtual HRESULT Close() = NULL;
		virtual HRESULT Release() = NULL;
	};

	struct IServer : public IConnectionManager
	{
		//virtual HRESULT Create() = NULL;
		virtual HRESULT WaiForConnect() = NULL;
		//virtual HRESULT SendOnce(LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent) = NULL;
		//virtual HRESULT Send(LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent) = NULL;
		//virtual HRESULT ReceiveOnce(LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived) = NULL;
		//virtual HRESULT Receive(LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived) = NULL;
		//virtual HRESULT Disconnect() = NULL;
		//virtual HRESULT Close() = NULL;
		//virtual HRESULT Release() = NULL;
	};

	struct IClient : public IConnectionManager
	{
		//virtual HRESULT Create() = NULL;
		virtual HRESULT ConnetToServer() = NULL;
		//virtual HRESULT SendOnce(LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent) = NULL;
		//virtual HRESULT Send(LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent) = NULL;
		//virtual HRESULT ReceiveOnce(LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived) = NULL;
		//virtual HRESULT Receive(LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived) = NULL;
		//virtual HRESULT Disconnect() = NULL;
		//virtual HRESULT Close() = NULL;
		//virtual HRESULT Release() = NULL;
	};

	//HRESULT SendLengthBuffer(ISendReceive * pSR, CONST CLengthBuffer & LenBuffer);
	//HRESULT ReceiveLengthBuffer(ISendReceive * pSR, CLengthBuffer & LenBuffer);

	IServer * CreateSocketServerInterface(ULONG nIP4or6, ULONG PortNumber);
	IClient * CreateSocketClientInterface(LPCTSTR pszServer, ULONG nPort);

	IServer * CreatePipeServerInterface(LPCTSTR pszPipeName, ULONG nOutBufferSize, ULONG nInBufferSize, BOOL bMessgaeMode);
	IClient * CreatePipeClientInterface(LPCTSTR pszServer, LPCTSTR pszPipeName, BOOL bMessgaeMode);
}