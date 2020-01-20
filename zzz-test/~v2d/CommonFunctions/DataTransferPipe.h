#pragma once

#include "stdafx.h"
#include "DataTransfer.h"

namespace NS_DATATRANSFER
{
	class CPipeServer : public IServer
	{
		friend IServer * CreatePipeServerInterface(LPCTSTR pszPipeName, ULONG nOutBufferSize, ULONG nInBufferSize, BOOL bMessgaeMode);

	private:
		CString m_strPipeName;
		ULONG m_nOutBufferSize;
		ULONG m_nInBufferSize;
		HANDLE m_hPipe; //to cleanup
		BOOL m_bMessageMode;//<mycode> 2011-4-14
		
		CPipeServer(LPCTSTR pszPipeName, ULONG nOutBufferSize, ULONG nInBufferSize, BOOL bMessageMode); //<mycode> 2011-4-14
		HRESULT DestroyMember();
		virtual ~CPipeServer();
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

	class CPipeClient : public IClient
	{
		friend IClient * CreatePipeClientInterface(LPCTSTR pszServer, LPCTSTR pszPipeName, BOOL bMessgaeMode);

	private:
		CString m_strServer;
		CString m_strPipeName;
		HANDLE m_hPipe; //to cleanup
		BOOL m_bMessageMode; //<mycode> 2011-4-14

		CPipeClient(LPCTSTR pszServer, LPCTSTR pszPipeName, BOOL bMessageMode);
		HRESULT DestroyMember();
		virtual ~CPipeClient();
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