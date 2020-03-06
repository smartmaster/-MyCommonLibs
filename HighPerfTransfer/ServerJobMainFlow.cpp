#include "stdafx.h"

#include "ServerJobMainFlow.h"



CServerJobMainFlow::CServerJobMainFlow(SOCKET ConnectionSocket, BOOL bUseSsl) :
m_RefCount(0),
m_ConnectionSocket(ConnectionSocket),
m_bUseSsl(bUseSsl)
{

}

HRESULT CServerJobMainFlow::CreateMember()
{
	return S_OK;
}

HRESULT CServerJobMainFlow::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (INVALID_SOCKET != m_ConnectionSocket)
	{
		closesocket(m_ConnectionSocket);
		m_ConnectionSocket = INVALID_SOCKET;
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

/*static*/ HRESULT GetServerSslCertKeyFiles(CString & CertFile, CString & PrivateKeyFile, CString & VerifyPeerCert, CString & KeyFilePwd)
{
	TCHAR szPath[1024] = { 0 };
	DWORD dwCount = GetModuleFileName(
		NULL,//_In_opt_  HMODULE hModule,
		szPath,//_Out_     LPTSTR lpFilename,
		_countof(szPath)//_In_      DWORD nSize
		);
	ATLASSERT(dwCount);

	CString strParent;
	CMiscHelper::GetStringParentByLevel(strParent, szPath, 1, TEXT('\\'));
	ATLASSERT(strParent.GetLength());

	strParent += TEXT("\\cert");
	CertFile = strParent + TEXT("\\sslservercert.pem");
	PrivateKeyFile = strParent + TEXT("\\sslserverkey.pem");
	VerifyPeerCert = strParent + TEXT("\\cacert.pem");
	KeyFilePwd = TEXT("1qaz");

	return S_OK;
}

HRESULT CServerJobMainFlow::Main()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	CComPtr<ISocketClient> spISocketClient;
	CComPtr<IHPTServerHandlerFactory> spIHPTServerHandlerFactory;
	CComPtr<IHPTSendReceive> spIHPTSendReceive;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_bUseSsl)
	{
		CString CertFile; // = TEXT("F:\\MyProjects\\~MyCommonLibs\\PublicLibExtern\\cert\\sslservercert.pem");
		CString PrivateKeyFile; // = TEXT("F:\\MyProjects\\~MyCommonLibs\\PublicLibExtern\\cert\\sslserverkey.pem");
		CString VerifyPeerCert; // = TEXT("F:\\MyProjects\\~MyCommonLibs\\PublicLibExtern\\cert\\cacert.pem");
		CString KeyFilePwd; // = TEXT("1qaz");
		GetServerSslCertKeyFiles(CertFile, PrivateKeyFile, VerifyPeerCert, KeyFilePwd);

		hr = CreateInstanceCSocketHandleWrapper(
			m_ConnectionSocket,//SOCKET SocketConnection,
			TRUE,//BOOL bAttachSocketHandle,
			&spISocketClient,//ISocketClient ** ppObj,
			FALSE,//BOOL bSendAll,
			FALSE,//BOOL bReceiveAll,
			TRUE,//BOOL bServer,
			CertFile.GetString(),//LPCTSTR pCertFile,
			PrivateKeyFile.GetString(),//LPCTSTR PrivateKeyFile,
			VerifyPeerCert.GetString(),//LPCTSTR pVerifyPeerCert,
			KeyFilePwd.GetString(),
			TRUE//BOOL bCreateMember
			);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("CreateInstanceCSocketHandleWrapper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		hr = CreateInstanceCSocketHandleWrapper(
			m_ConnectionSocket,//SOCKET SocketConnection, 
			TRUE,//BOOL bAttachSocketHandle, 
			&spISocketClient,//ISocketClient ** ppObj, 
			FALSE,//BOOL bSendAll, 
			FALSE,//BOOL bReceiveAll, 
			TRUE//BOOL bCreateMember
			);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("CreateInstanceCSocketHandleWrapper"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}
	
	m_ConnectionSocket = INVALID_SOCKET;


	//////////////////////////////////////////////////////////////////////////
	hr = CreateInstanceCHPTServerHandlerFactory(&spIHPTServerHandlerFactory, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCHPTServerHandlerFactory"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	hr = CreateInstanceCHPTSendReceive(
		spISocketClient.p,//IFileDevice * pSender,
		spISocketClient.p,//IFileDevice * pReceiver,
		FALSE,//BOOL bAttachSenderReceiver,
		spIHPTServerHandlerFactory.p,//IHPTServerHandlerFactory * pHPTNewHandlerFactory,
		TRUE,//BOOL bServer,
		&spIHPTSendReceive,//IHPTSendReceive  ** ppIHPTSendReceive,
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCHPTSendReceive"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	if (spIHPTSendReceive.p)
	{
		hr = SendReceiverMonitorProc(spIHPTSendReceive.p);
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("SendReceiverMonitorProc"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}

	}

	return hr;
}

HRESULT CreateInstanceCServerJobMainFlow(SOCKET ConnectionSocket, BOOL bUseSsl, IJobMainFlow ** ppIJobMainFlow, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		new CServerJobMainFlow(ConnectionSocket, bUseSsl),//TY_CLASS * NEW_COBJECT, 
		ppIJobMainFlow,//TY_INTERFACE ** ppObj, 
		bCreateMember//BOOL bCreateMember
		);
}

/*static*/ DWORD WINAPI ThreadProcServerMain(
	_In_  LPVOID lpParameter
	)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	SOCKET ConnectionSocket = (SOCKET)(lpParameter);

	CComPtr<IJobMainFlow> spIJobMainFlow;
	//BOOL bUseSsl = TRUE;
	BOOL bUseSsl = FALSE;
	hr = CreateInstanceCServerJobMainFlow(ConnectionSocket, bUseSsl, &spIJobMainFlow, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCDownloadServer"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = spIJobMainFlow->Main();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("IJobMainFlow->Main"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT CreateThreadServerMain(SOCKET ConnectionSocket)
{

	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	DWORD ThreadId = 0;
	HANDLE hThread = CreateThread(
		NULL,//_In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
		0,//_In_       SIZE_T dwStackSize,
		ThreadProcServerMain,//_In_       LPTHREAD_START_ROUTINE lpStartAddress,
		(LPVOID)(ConnectionSocket),//_In_opt_   LPVOID lpParameter,
		0,//_In_       DWORD dwCreationFlags,
		&ThreadId//_Out_opt_  LPDWORD lpThreadId
		);
	if (NULL == hThread)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_API_ERR(0, TEXT("CreateThread"), hr, TEXT("ThreadProcDownloadServer"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CloseHandle(hThread);
	hThread = NULL;
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
	
}


