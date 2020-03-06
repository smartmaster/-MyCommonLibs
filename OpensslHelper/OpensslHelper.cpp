#include "stdafx.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>


#include "OpensslHelper.h"

ULONG GetSSlError(ULONG err, CStringA & str)
{
	CONST LONG BUFF_LEN = 1024;
	LPSTR pBuffer = str.GetBuffer(BUFF_LEN);
	ERR_error_string_n(err, pBuffer, BUFF_LEN);
	str.ReleaseBuffer();
	return err;
}

ULONG GetSSlLastError(CStringA & str)
{
	return GetSSlError(ERR_get_error(), str);
}


INT SslLibInit()
{
	static LONG INIT_COUNT = 0;
	if (0 == INIT_COUNT)
	{
		LONG Count = InterlockedIncrement(&INIT_COUNT);
		if (1 == Count)
		{
			SSL_library_init();

			/* According to draft-ietf-tls-compression-04.txt, the
			compression number ranges should be the following:

			0 to 63:    methods defined by the IETF
			64 to 192:  external party methods assigned by IANA
			193 to 255: reserved for private use */
			//if (id < 193 || id > 255)
			//{
			//	SSLerr(SSL_F_SSL_COMP_ADD_COMPRESSION_METHOD, SSL_R_COMPRESSION_ID_NOT_WITHIN_PRIVATE_RANGE);
			//	return 0;
			//}
			int err = SSL_COMP_add_compression_method(200, COMP_zlib());
			if (1 == err)
			{
				CSslErrorCallback ErrorCallback;
				ErrorCallback.CollectErrors(TRUE);
				HRESULT hr = E_FAIL;
				D_API_ERR(0, TEXT("SSL_COMP_add_compression_method"), hr, TEXT("SSL_COMP_add_compression_method"));
				CONST vector<CStringA> & errors = ErrorCallback.GetErrors();
				for (size_t ii = 0; ii < errors.size(); ++ii)
				{
					D_API_ERR(0, TEXT("SSL_COMP_add_compression_method"), hr, TEXT("%S"), errors[ii].GetString());
				}
			}

			SSL_load_error_strings();

			ERR_load_COMP_strings();
		}
	}
	return 0;
}

INT SslLibUninit()
{
	//do nothing
	return 0;
}



VOID CSslErrorCallback::OnError(const char *str, size_t len)
{
	m_errors.push_back(str);
}

VOID CSslErrorCallback::ClearError()
{
	m_errors.clear();
}

CONST vector<CStringA> & CSslErrorCallback::GetErrors() CONST
{
	return m_errors;
}

int CSslErrorCallback::SslErrorCallback(const char *str, size_t len, void *u)
{
	CSslErrorCallback * pCSslErrorCallback = (CSslErrorCallback *)(u);
	pCSslErrorCallback->OnError(str, len);
	return len;
}

VOID CSslErrorCallback::CollectErrors(BOOL bClearOldError)
{
	if (bClearOldError)
	{
		ClearError();
	}
	ERR_print_errors_cb(CSslErrorCallback::SslErrorCallback, this);
}

CSsslHelper::CSsslHelper() :
m_ctx(NULL),
m_Sokcet(INVALID_SOCKET),
m_ssl(NULL)
{

}

VOID CSsslHelper::DestroyMember()
{
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

	if (INVALID_SOCKET != m_Sokcet)
	{
		closesocket(m_Sokcet);
		m_Sokcet = INVALID_SOCKET;
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
}

CSsslHelper::~CSsslHelper()
{
	DestroyMember();
}

HRESULT CSsslHelper::CtxCreate(LONG SslVersion /*reserved for future use*/)
{
	HRESULT hr = S_OK;

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
	}

	return hr;
}

HRESULT CSsslHelper::SetCertPrivatekey(const char * vertFile, const char * privateKeyFile)
{
	ATLASSERT(m_ctx);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	/* Load the client certificate into the SSL_CTX structure */
	if (SSL_CTX_use_certificate_file(m_ctx, vertFile, SSL_FILETYPE_PEM) <= 0)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("SSL_CTX_use_certificate_file"), hr, TEXT("%S"), vertFile);
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("SSL_CTX_use_certificate_file"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	/* Load the private-key corresponding to the client certificate */
	if (SSL_CTX_use_PrivateKey_file(m_ctx, privateKeyFile, SSL_FILETYPE_PEM) <= 0)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("SSL_CTX_use_PrivateKey_file"), hr, TEXT("%S"), privateKeyFile);
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

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CSsslHelper::SetVerifyPeerCert(const char * caCertFile, int depth)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	ATLASSERT(m_ctx);

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	/* Load the RSA CA certificate into the SSL_CTX structure */
	/* This will allow this client to verify the server's     */
	/* certificate.                                           */
	if (!SSL_CTX_load_verify_locations(m_ctx, caCertFile, NULL))
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("SSL_CTX_load_verify_locations"), hr, TEXT("%S"), caCertFile);
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("SSL_CTX_load_verify_locations"), hr, TEXT("%S"), errors[ii].GetString());
		}
	}

	/* Set flag in context to require peer (server) certificate */
	/* verification */

	SSL_CTX_set_verify(m_ctx, SSL_VERIFY_PEER, NULL);

	SSL_CTX_set_verify_depth(m_ctx, depth);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CSsslHelper::AttachSocket(SOCKET & sock, BOOL bServer)
{
	ATLASSERT(m_ctx);

	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	m_Sokcet = sock;
	sock = INVALID_SOCKET;
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
	}

	/* Assign the socket into the SSL structure (SSL and socket without BIO) */
	SSL_set_fd(m_ssl, m_Sokcet);

	if (bServer)
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
		}
	}

	/* Informational output (optional) */
	D_INFO(0, TEXT("SSL connection using %S"), SSL_get_cipher(m_ssl));

	/* Get the server's certificate (optional) */
	X509 * peer_cert = SSL_get_peer_certificate(m_ssl);
	if (peer_cert != NULL)
	{
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

		D_INFO(0, TEXT("Server certificate subject : %S"), str);
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
		}
		D_INFO(0, TEXT("Server certificate issuer : %S"), str);
		free(str);
		X509_free(peer_cert);
	}
	else
	{
		D_INFO(0, TEXT("The SSL server does not have certificate."));
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CSsslHelper::SSL_CTX_use_PrivateKey_file_pwd(SSL_CTX *ctx, char *filename, char *pwd)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BIO * biokey = NULL;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	EVP_PKEY * evpkey = NULL;

	biokey = BIO_new(BIO_s_file());
	BIO_read_filename(biokey, filename);
	evpkey = PEM_read_bio_PrivateKey(biokey, NULL, NULL, pwd);
	if (evpkey == NULL)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("PEM_read_bio_PrivateKey"), hr, TEXT("PEM_read_bio_PrivateKey"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("PEM_read_bio_PrivateKey"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	if (SSL_CTX_use_PrivateKey(ctx, evpkey) <= 0)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("SSL_CTX_use_PrivateKey"), hr, TEXT("SSL_CTX_use_PrivateKey"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
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

VOID OpensslWrappers::X509CalcThumbPront(X509 * cert, vector<BYTE> & vecPrint)
{
	unsigned char         md[EVP_MAX_MD_SIZE];
	UINT n = 0;
	const EVP_MD * digest = EVP_get_digestbyname("sha1");
	X509_digest(cert, digest, md, &n);
	printf("Fingerprint: ");
	for (int pos = 0; pos < 19; pos++)
		printf("%02x:", md[pos]);
	printf("%02x\n", md[19]);
	for (int ii = 0; ii < n; ++ii)
	{
		vecPrint.push_back(md[ii]);
	}
}

#if 0
int OpensslWrappers::test_main(int argc, const char * argv[])
{
	struct stat           sb;
	unsigned char       * buff;
	int                   fd;
	size_t               len;
	BIO                 * bio;
	X509                * x;
	unsigned              err;
	int                   pos;
	char                  errmsg[1024];
	const EVP_MD        * digest;
	unsigned char         md[EVP_MAX_MD_SIZE];
	unsigned int          n;

	// checks arguments
	if (argc != 2)
	{
		fprintf(stderr, "Usage: peminfo <pemfile>\n");
		return(1);
	};

	// checks file
	if ((_stat(argv[1], &sb)) == -1)
	{
		perror("peminfo: stat()");
		return(1);
	};
	len = (sb.st_size * 2);

	// allocates memory
	if (!(buff = malloc(len)))
	{
		fprintf(stderr, "peminfo: out of virtual memory\n");
		return(1);
	};

	// opens file for reading
	if ((fd = fopen(argv[1], O_RDONLY)) == -1)
	{
		perror("peminfo: open()");
		free(buff);
		return(1);
	};

	// reads file
	if ((len = read(fd, buff, len)) == -1)
	{
		perror("peminfo: read()");
		free(buff);
		return(1);
	};

	// closes file
	close(fd);

	// initialize OpenSSL
	SSL_load_error_strings();
	SSL_library_init();

	// creates BIO buffer
	bio = BIO_new_mem_buf(buff, len);

	// decodes buffer
	if (!(x = PEM_read_bio_X509(bio, NULL, 0L, NULL)))
	{
		while ((err = ERR_get_error()))
		{
			errmsg[1023] = '\0';
			ERR_error_string_n(err, errmsg, 1023);
			fprintf(stderr, "peminfo: %s\n", errmsg);
		};
		BIO_free(bio);
		free(buff);
		return(1);
	};

	// prints x509 info
	printf("name:      %s\n", x->name);
	printf("serial:    ");
	printf("%02X", x->cert_info->serialNumber->data[0]);
	for (pos = 1; pos < x->cert_info->serialNumber->length; pos++)
		printf(":%02X", x->cert_info->serialNumber->data[pos]);
	printf("\n");

	// calculate & print fingerprint
	digest = EVP_get_digestbyname("sha1");
	X509_digest(x, digest, md, &n);
	printf("Fingerprint: ");
	for (pos = 0; pos < 19; pos++)
		printf("%02x:", md[pos]);
	printf("%02x\n", md[19]);

	// frees memory
	BIO_free(bio);
	free(buff);

	return(0);
}
#endif