#include "stdafx.h"


#include <Ws2tcpip.h>
#include <WinSock2.h>

#include "OpensslHelper.h"

#include "..\PublicHeader\PublicDTraceLib.h"

#include "SslSocketClient.h"

namespace
{
	//template<typename T1, typename T2>
	LONG CalcRatio(LONGLONG a, LONGLONG b, LONGLONG tens)
	{
		return (a * tens * 10 / b + 5) / 10;
	}
} //end  namespace

/*static*/ HRESULT GetClientSslCertKeyFiles(CString & CertFile, CString & PrivateKeyFile, CString & VerifyPeerCert, CString & KeyFilePwd)
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
	CertFile = strParent + TEXT("\\sslclientcert.pem");
	PrivateKeyFile = strParent + TEXT("\\sslclientkey.pem");
	VerifyPeerCert = strParent + TEXT("\\cacert.pem");
	KeyFilePwd = TEXT("1qaz");

	return S_OK;
}


static HRESULT SSL_CTX_use_PrivateKey_file_pwd(SSL_CTX *ctx, const char *filename, const char *pwd, CSslErrorCallback & ErrorCallback)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BIO * biokey = NULL;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	EVP_PKEY * evpkey = NULL;

	biokey = BIO_new(BIO_s_file());
	BIO_read_filename(biokey, filename);
	evpkey = PEM_read_bio_PrivateKey(biokey, NULL, NULL, (char*)pwd);
	if (evpkey == NULL)
	{
		ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("PEM_read_bio_PrivateKey"), hr, TEXT("PEM_read_bio_PrivateKey"));
		CONST vector<CStringA> & errors = ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("PEM_read_bio_PrivateKey"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	if (SSL_CTX_use_PrivateKey(ctx, evpkey) <= 0)
	{
		ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("SSL_CTX_use_PrivateKey"), hr, TEXT("SSL_CTX_use_PrivateKey"));
		CONST vector<CStringA> & errors = ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("SSL_CTX_use_PrivateKey"), hr, TEXT("%S"), errors[ii].GetString());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);


	BIO_free(biokey);

	return hr;
}




static HRESULT SslSocketSend(SSL * ssl, LPCVOID pBuffer, LONG nToSend, LONG * pnSent, BOOL bSendAll)
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	CONST CHAR * pBufferChar = (CONST CHAR *)pBuffer;
	ULONG ulDataTransered = 0;
	INT nCurrentSent = 0;

	BOOL bConnectionClosed = FALSE;

	while (nToSend)
	{
		nCurrentSent = SSL_write(ssl, pBufferChar + ulDataTransered, nToSend);
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
			D_API_ERR(0, TEXT("SSL_write"), dwLastError, TEXT("nCurrentSent=%u, hr=%u"), nCurrentSent, hr);
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

static HRESULT SslSocketReceive(SSL * ssl, LPVOID pBuffer, LONG nToReceive, LONG * pnReceived, BOOL bReceiveAll)
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	CHAR * pBufferChar = (CHAR *)pBuffer;
	ULONG ulDataTransered = 0;
	INT nCurrentReceive = 0;

	BOOL bConnectionClosed = FALSE;

	while (nToReceive)
	{
		nCurrentReceive = SSL_read(ssl, pBufferChar + ulDataTransered, nToReceive);
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
			D_API_ERR(0, TEXT("SSL_read"), dwLastError, TEXT("nCurrentSent=%u, hr=%u"), nCurrentReceive, hr);
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


HRESULT CSslSocketClient::CreateMember()
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
	m_pSslSocket = new CSslSocketHandleWrapper(
		SocketConnection,//SOCKET SocketConnection,
		TRUE,//BOOL bAttachSocketHandle,
		m_bSendAll,//BOOL bSendAll,
		m_bReceiveAll,//BOOL bReceiveAll,
		FALSE,//BOOL bServer,
		m_CertFile.GetString(),//LPCTSTR pCertFile,
		m_PrivateKeyFile.GetString(),//LPCTSTR PrivateKeyFile,
		m_VerifyPeerCert.GetString(),//LPCTSTR pVerifyPeerCert
		m_KeyFilePwd.GetString()
		);

	hr = m_pSslSocket->CreateMember();
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("SslSocket->CreateMember"), hr, TEXT(""));
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

HRESULT CSslSocketClient::Write(ULONG nToSend, ULONG * pnSent,  LPCBYTE pBuffer)
{
#if 1
	return m_pSslSocket->Write(nToSend, pnSent, pBuffer);
#else
	LONG Sent = 0;

	HRESULT hr = SslSocketSend(m_SocketConnection, pBuffer, nToSend, &Sent, m_bSendAll); //<sonmi01>2013-12-6 ###???
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

HRESULT CSslSocketClient::Read(ULONG nToReceive, ULONG * pnReceived,  BYTE* pBuffer)
{
#if 1
	return m_pSslSocket->Read(nToReceive, pnReceived, pBuffer);
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

HRESULT CSslSocketClient::Disconnect()
{
#if 1
	return m_pSslSocket->Disconnect();
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


CSslSocketClient::CSslSocketClient(
	LPCTSTR pszServer,
	LPCTSTR pPort,
	BOOL bSendAll,
	BOOL bReceiveAll,
	LPCTSTR pCertFile,
	LPCTSTR PrivateKeyFile,
	LPCTSTR pVerifyPeerCert,
	LPCTSTR pKeyFilePwd
	) :
	m_strServer(pszServer),
	m_Port(pPort),
	m_bSendAll(bSendAll), //<sonmi01>2013-12-6 ###???
	m_bReceiveAll(bReceiveAll),
	m_RefCount(0),
	//m_TotalSent(0),
	//m_TotalReceived(0),
	m_CertFile(pCertFile),
	m_PrivateKeyFile(PrivateKeyFile),
	m_VerifyPeerCert(pVerifyPeerCert),
	m_KeyFilePwd(pKeyFilePwd),
	m_pSslSocket(NULL)
{

}

HRESULT CSslSocketClient::DestroyMember()
{
	HRESULT hr = S_OK;
	if (m_pSslSocket)
	{
		m_pSslSocket->DestroyMember();
		delete m_pSslSocket;
		m_pSslSocket = NULL;
	}
	return hr;
}

LONGLONG CSslSocketClient::GetTotalSentSize()
{
	return m_pSslSocket->GetTotalSentSize();
}

LONGLONG CSslSocketClient::GetTotalReceivedSize()
{
	return m_pSslSocket->GetTotalReceivedSize();
}

HRESULT CSslSocketClient::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	return m_pSslSocket->SetPointerEx(llDistanceToMove, lpNewFilePointer, ulMoveMethod);
}

HRESULT CSslSocketClient::GetFileSize( LONGLONG * pFileSize )
{
	return m_pSslSocket->GetFileSize(pFileSize);
}

HRESULT CSslSocketClient::SendLengthAndBuffer( LONG Length, CONST BYTE * Buffer, LONG * LengthSent )
{
	return m_pSslSocket->SendLengthAndBuffer(Length, Buffer, LengthSent);
}

HANDLE CSslSocketClient::GetRawHandle()
{
	return (HANDLE)m_pSslSocket->GetRawHandle();
}

HRESULT CreateInstanceCSslSocketClient(
	LPCTSTR pszServer, 
	LPCTSTR pPort, 
	ISocketClient ** ppObj, 
	BOOL bSendAll, 
	BOOL bReceiveAll, 
	LPCTSTR pCertFile,
	LPCTSTR PrivateKeyFile,
	LPCTSTR pVerifyPeerCert,
	LPCTSTR pKeyFilePwd,
	BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT((new CSslSocketClient(pszServer, pPort, bSendAll, bReceiveAll, pCertFile, PrivateKeyFile, pVerifyPeerCert, pKeyFilePwd)), ppObj, bCreateMember); //<sonmi01>2013-12-6 ###???
}

CSslSocketHandleWrapper::CSslSocketHandleWrapper(
	SOCKET SocketConnection,
	BOOL bAttachSocketHandle,
	BOOL bSendAll,
	BOOL bReceiveAll,
	BOOL bServer,
	LPCTSTR pCertFile,
	LPCTSTR PrivateKeyFile,
	LPCTSTR pVerifyPeerCert,
	LPCTSTR pKeyFilePwd
	) :
m_SocketConnection(SocketConnection),
m_bAttachSocketHandle(bAttachSocketHandle),
m_bSendAll(bSendAll), //<sonmi01>2013-12-6 ###???
m_bReceiveAll(bReceiveAll),
m_RefCount(0),
m_TotalSent(0),
m_TotalReceived(0),
m_TotalSentSsl(0),
m_TotalReceivedSsl(0),
m_bServer(bServer),
m_CertFile(pCertFile),
m_PrivateKeyFile(PrivateKeyFile),
m_VerifyPeerCert(pVerifyPeerCert),
m_KeyFilePwd(pKeyFilePwd),
m_ctx(NULL),
m_ssl(NULL)
{

}

HRESULT CSslSocketHandleWrapper::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	SslLibInit();

	//////////////////////////////////////////////////////////////////////////
	CONST SSL_METHOD *meth = TLS_method();//SSLv3_method();
	m_ctx = SSL_CTX_new(meth);
	if (NULL == m_ctx)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("SSL_CTX_new"), hr, TEXT("SSL_CTX_new"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("SSL_CTX_new"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	if (m_CertFile.GetLength() || m_PrivateKeyFile.GetLength())
	{
		/* Load the client certificate into the SSL_CTX structure */
		if (SSL_CTX_use_certificate_file(m_ctx, m_CertFile.GetString(), SSL_FILETYPE_PEM) <= 0)
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("SSL_CTX_use_certificate_file"), hr, TEXT("%S"), m_CertFile.GetString());
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("SSL_CTX_use_certificate_file"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}


		/* Load the private-key corresponding to the client certificate */
		if (m_KeyFilePwd.GetLength())
		{
			hr = SSL_CTX_use_PrivateKey_file_pwd(m_ctx, m_PrivateKeyFile.GetString(), m_KeyFilePwd.GetString(), m_ErrorCallback);
			if (FAILED(hr))
			{
				//LastError = WSAGetLastError();
				//LastError = GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				//D_SET_FISRT_STATUS(hr, LastError);
				//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
				//D_SET_LAST_STATUS(hr, LastError);
				//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
				D_API_ERR(0, TEXT("SSL_CTX_use_PrivateKey_file_pwd"), hr, TEXT("%S"), m_PrivateKeyFile.GetString());
				LEAVE_BLOCK(0);
			}
		}
		else if (SSL_CTX_use_PrivateKey_file(m_ctx, m_PrivateKeyFile.GetString(), SSL_FILETYPE_PEM) <= 0)
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("SSL_CTX_use_PrivateKey_file"), hr, TEXT("%S"), m_PrivateKeyFile.GetString());
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("SSL_CTX_use_PrivateKey_file"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}

		/* Check if the client certificate and private-key matches */
		if (!SSL_CTX_check_private_key(m_ctx))
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("SSL_CTX_check_private_key"), hr, TEXT("SSL_CTX_check_private_key"));
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("SSL_CTX_check_private_key"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	if (m_VerifyPeerCert.GetLength())
	{
		//////////////////////////////////////////////////////////////////////////
		/* Load the RSA CA certificate into the SSL_CTX structure */
		/* This will allow this client to verify the server's     */
		/* certificate.                                           */
		if (!SSL_CTX_load_verify_locations(m_ctx, m_VerifyPeerCert.GetString(), NULL))
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("SSL_CTX_load_verify_locations"), hr, TEXT("%S"), m_VerifyPeerCert.GetString());
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("SSL_CTX_load_verify_locations"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}

		/* Set flag in context to require peer (server) certificate */
		/* verification */

		SSL_CTX_set_verify(m_ctx, SSL_VERIFY_PEER, NULL);

		SSL_CTX_set_verify_depth(m_ctx, 1);
	}

	//////////////////////////////////////////////////////////////////////////
	/* An SSL structure is created */
	m_ssl = SSL_new(m_ctx);
	if (NULL == m_ssl)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("SSL_new"), hr, TEXT("SSL_new"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("SSL_new"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	/* Assign the socket into the SSL structure (SSL and socket without BIO) */
	SSL_set_fd(m_ssl, m_SocketConnection);

	SetSslIoCallback(m_ssl, this); 

	if (m_bServer)
	{
		/* Perform SSL Handshake on the SSL client */
		INT err = SSL_accept(m_ssl);
		if (-1 == err)
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("SSL_accept"), hr, TEXT("SSL_accept"));
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("SSL_accept"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}
	}
	else
	{
		/* Perform SSL Handshake on the SSL client */
		INT err = SSL_connect(m_ssl);
		if (-1 == err)
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("SSL_connect"), hr, TEXT("SSL_connect"));
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("SSL_connect"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}
	}

	/* Informational output (optional) */
	D_INFO(0, TEXT("SSL connection using %S"), SSL_get_cipher(m_ssl));

	/* Get the server's certificate (optional) */
	X509 * peer_cert = SSL_get_peer_certificate(m_ssl);
	if (peer_cert != NULL)
	{
		vector<BYTE> vecPrint;
		OpensslWrappers::X509CalcThumbPront(peer_cert, vecPrint);
		CHAR * str = X509_NAME_oneline(X509_get_subject_name(peer_cert), 0, 0);
		if (NULL == str)
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("X509_NAME_oneline"), hr, TEXT("X509_NAME_oneline"));
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("X509_NAME_oneline"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}

		D_INFO(0, TEXT("Peer certificate subject : %S"), str);
		free(str);

		str = X509_NAME_oneline(X509_get_issuer_name(peer_cert), 0, 0);
		if (NULL == str)
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("X509_NAME_oneline"), hr, TEXT("X509_NAME_oneline"));
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("X509_NAME_oneline"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}

		D_INFO(0, TEXT("Peer certificate issuer : %S"), str);
		free(str);
		X509_free(peer_cert);
	}
	else
	{
		D_INFO(0, TEXT("The peer does not have certificate."));
	}

	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HRESULT CSslSocketHandleWrapper::DestroyMember()
{
	HRESULT hr = S_OK;

	INT nStatus = 0;

	if (m_ssl)
	{
		nStatus = SSL_shutdown(m_ssl);
		if (-1 == nStatus)
		{
			m_ErrorCallback.CollectErrors(TRUE);
			//hr = E_FAIL;
			D_API_ERR(0, TEXT("SSL_shutdown"), E_FAIL, TEXT("SSL_shutdown"));
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("SSL_shutdown"), E_FAIL, TEXT("%S"), errors[ii].GetString());
			}
		}
	}

	if (m_bAttachSocketHandle)
	{
		if (INVALID_SOCKET != m_SocketConnection)
		{
			D_INFO(0, TEXT("SslAppLayer=0x%p, TotalSent=%I64d, TotalReceived=%I64d"), m_ssl, m_TotalSent, m_TotalReceived);
			D_INFO(0, TEXT("SslSocketLayer=0x%p, TotalSentSsl=%I64d, TotalReceivedSsl=%I64d"), m_SocketConnection, m_TotalSentSsl, m_TotalReceivedSsl);

			CONST LONG TENS = 10000;
			D_INFO(0, TEXT("SendRatio=%d:%d, ReceiveRatio=%d:%d"), 
				m_TotalSent? CalcRatio(m_TotalSentSsl, m_TotalSent, TENS) : -1, 
				TENS, 
				m_TotalReceived? CalcRatio(m_TotalReceivedSsl, m_TotalReceived, TENS) : -1, 
				TENS);

			closesocket(m_SocketConnection);
			m_SocketConnection = INVALID_SOCKET;
		}
	}

	if (m_ssl)
	{
		/* Free the SSL structure */
		SSL_free(m_ssl);
		m_ssl = NULL;
	}

	if (m_ctx)
	{
		SSL_CTX_free(m_ctx);
		m_ctx = NULL;
	}

	return hr;
}

HRESULT CSslSocketHandleWrapper::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	if (lpNewFilePointer)
	{
		*lpNewFilePointer = 0;
	}
	return S_OK;
}

HRESULT CSslSocketHandleWrapper::Read(ULONG nToReceive, ULONG * pnReceived, BYTE* pBuffer)
{
	LONG Received = 0;
	HRESULT hr = SslSocketReceive(m_ssl, pBuffer, nToReceive, &Received, m_bReceiveAll); //<sonmi01>2013-12-6 ###???
	if (SUCCEEDED(hr))
	{
		m_TotalReceived += Received;
		if (pnReceived)
		{
			*pnReceived = Received;
		}
	}
	else
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("SslSocketReceive"), hr, TEXT("SslSocketReceive"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("SslSocketReceive"), hr, TEXT("%S"), errors[ii].GetString());
		}
	}

	return hr;
}

HRESULT CSslSocketHandleWrapper::Write(ULONG nToSend, ULONG * pnSent, LPCBYTE pBuffer)
{
	LONG Sent = 0;

	HRESULT hr = SslSocketSend(m_ssl, pBuffer, nToSend, &Sent, m_bSendAll); //<sonmi01>2013-12-6 ###???
	if (SUCCEEDED(hr))
	{
		m_TotalSent += Sent;
		if (pnSent)
		{
			*pnSent = Sent;
		}
	}
	else
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("SslSocketSend"), hr, TEXT("SslSocketSend"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("SslSocketSend"), hr, TEXT("%S"), errors[ii].GetString());
		}
	}

	return hr;
}

HRESULT CSslSocketHandleWrapper::GetFileSize(LONGLONG * pFileSize)
{
	if (pFileSize)
	{
		*pFileSize = 0;
	}
	return S_OK;
}

HRESULT CSslSocketHandleWrapper::Disconnect()
{
#if 1
	return S_OK; //do nothing, before ssl shutdown
#else
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
#endif
}

LONGLONG CSslSocketHandleWrapper::GetTotalSentSize()
{
	return m_TotalSent;
}

LONGLONG CSslSocketHandleWrapper::GetTotalReceivedSize()
{
	return m_TotalReceived;
}

HRESULT CSslSocketHandleWrapper::SendLengthAndBuffer(LONG Length, CONST BYTE * Buffer, LONG * LengthSent)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ULONG nSent = 0;
	LONG SendLen = htonl(Length);
	hr = Write(sizeof(SendLen), &nSent, (LPCBYTE)(&SendLen));
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	*LengthSent += SendLen;

	//////////////////////////////////////////////////////////////////////////
	nSent = 0;
	hr = Write(Length, &nSent, Buffer);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Write"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	*LengthSent += SendLen;

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}

HANDLE CSslSocketHandleWrapper::GetRawHandle()
{
	return (HANDLE)m_SocketConnection;
}

long CSslSocketHandleWrapper::ssl_io_callback(BIO *b, int oper, const char *argp, int argi, long argl, long retvalue)
{
	CSslSocketHandleWrapper * pobj = (CSslSocketHandleWrapper *)(BIO_get_callback_arg(b));
	if ((BIO_CB_READ | BIO_CB_RETURN) == oper)
	{
		if (retvalue > 0)
		{
			pobj->m_TotalReceivedSsl += retvalue;
		}
		
	}
	else if ((BIO_CB_WRITE | BIO_CB_RETURN) == oper)
	{
		if (retvalue > 0)
		{
			pobj->m_TotalSentSsl += retvalue;
		}
	}

	return retvalue;
}

VOID CSslSocketHandleWrapper::SetSslIoCallback(SSL* ssl, CSslSocketHandleWrapper * pobj)
{
	BIO * rbio = SSL_get_rbio(ssl);
	BIO_set_callback(rbio, ssl_io_callback);
	BIO_set_callback_arg(rbio, (char*)(pobj));


	BIO * wbio = SSL_get_wbio(ssl);
	if (wbio != rbio)
	{
		BIO_set_callback(wbio, ssl_io_callback);
		BIO_set_callback_arg(wbio, (char*)(pobj));
	}
}


HRESULT CreateInstanceCSocketHandleWrapper(
	SOCKET SocketConnection, 
	BOOL bAttachSocketHandle, 
	ISocketClient ** ppObj, 
	BOOL bSendAll, 
	BOOL bReceiveAll, 
	BOOL bServer,
	LPCTSTR pCertFile,
	LPCTSTR PrivateKeyFile,
	LPCTSTR pVerifyPeerCert,
	LPCTSTR pKeyFilePwd,
	BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		(new CSslSocketHandleWrapper(
		SocketConnection, 
		bAttachSocketHandle, 
		bSendAll, 
		bReceiveAll,
		bServer,
		pCertFile,
		PrivateKeyFile,
		pVerifyPeerCert,
		pKeyFilePwd
		)), 
		ppObj, 
		bCreateMember
		);
}