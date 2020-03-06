#include "stdafx.h"
#include "OpensslCompressEncrypt.h"

static VOID GenerateSalt(vector<BYTE> & Salt, INT Length)
{
	Salt.resize(Length);
	Salt[0] = 13;
	Salt[1] = 17;
	for (int ii = 2; ii < Length; ++ii)
	{
		Salt[ii] = Salt[ii - 1] + Salt[ii - 2];
	}
}



COpensslEnc::COpensslEnc(E_CIPPHER _E_CIPPHER, E_MD _E_MD, LPCBYTE pSalt, LONG saltLen, LPCBYTE password, LONG pwdLen) :
m_RefCount(0),
m_E_CIPPHER(_E_CIPPHER),
m_E_MD(_E_MD)
{
	if (pSalt &&  saltLen)
	{
		m_Salt.assign(pSalt, pSalt + saltLen);
	}
	else
	{
		GenerateSalt(m_Salt, 256);
	}

	if (password && pwdLen)
	{
		m_password.assign(password, password + pwdLen);
	}
	else
	{
		m_password.resize(8);
	}
	
}

HRESULT COpensslEnc::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	switch (m_E_CIPPHER)
	{
	case E_CIPPHER_aes_128_cbc:
		m_cipher = EVP_aes_128_cbc();
		break;
	case E_CIPPHER_aes_192_cbc:
		m_cipher = EVP_aes_192_cbc();
		break;
	case E_CIPPHER_aes_256_cbc:
		m_cipher = EVP_aes_256_cbc();
		break;
	default:
		m_cipher = EVP_aes_256_cbc();
		break;
	}
	if (NULL == m_cipher)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("CIPPHER"), hr, TEXT("CIPPHER"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("CIPPHER"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	switch (m_E_MD)
	{
	case E_MD_md5:
		m_md = EVP_md5();
		break;
	case E_MD_sha:
		//m_md = EVP_sha();
		//break;
	case E_MD_sha1:
		m_md = EVP_sha1();
		break;
	case E_MD_sha256:
		m_md = EVP_sha256();
		break;
	case E_MD_sha512:
		m_md = EVP_sha512();
		break;
	default:
		m_md = EVP_sha512();
		break;
	}
	if (NULL == m_md)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("MD"), hr, TEXT("MD"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("MD"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	int key_length = EVP_CIPHER_key_length(m_cipher);
	int iv_length = EVP_CIPHER_iv_length(m_cipher);
	m_key.resize(key_length);
	m_iv.resize(iv_length);

	//////////////////////////////////////////////////////////////////////////
	if (0 == m_Salt.size())
	{
		GenerateSalt(m_Salt, 128);
	}

	//////////////////////////////////////////////////////////////////////////
	const int iteration_count = 7;
	int	ret = EVP_BytesToKey(
		m_cipher,//const EVP_CIPHER *type,
		m_md,//const EVP_MD *md,
		&m_Salt[0],//const unsigned char *salt, 
		&m_password[0],//const unsigned char *data,
		m_password.size(),//int datal, 
		iteration_count,//int count, 
		&m_key[0],//unsigned char *key,
		&m_iv[0]//unsigned char *iv
		);
	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_BytesToKey"), hr, TEXT("EVP_BytesToKey"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_BytesToKey"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT COpensslEnc::DestroyMember()
{
	return S_OK;
}

HRESULT COpensslEnc::Transform(LPCBYTE origBuffer, LONG origSize, LPBYTE transformedBuffer, LONG transformedSize, LONG & outTransformedSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	EVP_CIPHER_CTX & ctx = *EVP_CIPHER_CTX_new(); /**CLEANUP**/
	EVP_CIPHER_CTX_init(&ctx);

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	int ret = EVP_EncryptInit_ex(
		&ctx,//EVP_CIPHER_CTX *ctx,
		m_cipher,//const EVP_CIPHER *cipher, 
		NULL,//ENGINE *impl,
		&m_key[0],//const unsigned char *key, 
		&m_iv[0]//const unsigned char *iv
		);
	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_EncryptInit_ex"), hr, TEXT("EVP_EncryptInit_ex"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_EncryptInit_ex"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	int BlockSize = EVP_CIPHER_CTX_block_size(&ctx);
	if (transformedSize < 1024 || transformedSize < origSize || transformedSize < AtlAlignUp(origSize, BlockSize))
	{
		hr = TPM_E_PCP_BUFFER_TOO_SMALL;
		D_API_ERR(0, TEXT("buffer_size"), hr, TEXT("transformedSize=%d, origSize=%d, origSize=%d"), transformedSize, origSize, BlockSize);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	int encLen = -1;
	ret = EVP_EncryptUpdate(
		&ctx,//EVP_CIPHER_CTX *ctx, 
		transformedBuffer,//unsigned char *out,
		&encLen,//int *outl, 
		origBuffer,//const unsigned char *in, 
		origSize//int inl
		);
	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_EncryptUpdate"), hr, TEXT("EVP_EncryptUpdate"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_EncryptUpdate"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	int encLenFinal = -1;
	ret = EVP_EncryptFinal_ex(
		&ctx,//EVP_CIPHER_CTX *ctx, 
		transformedBuffer + encLen,//unsigned char *out, 
		&encLenFinal//int *outl
		);

	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_EncryptFinal_ex"), hr, TEXT("EVP_EncryptFinal_ex"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_EncryptFinal_ex"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	encLen += encLenFinal;

	outTransformedSize = encLen;

	ATLASSERT(outTransformedSize <= transformedSize);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	EVP_CIPHER_CTX_cleanup(&ctx);

	return hr;
}

HRESULT COpensslEnc::Recover(LPCBYTE transformedBuffer, LONG transformedSize, LPBYTE origBuffer, LONG origSize, LONG & outOrigSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	EVP_CIPHER_CTX& ctx = *EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(&ctx);

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	int ret = EVP_DecryptInit_ex(
		&ctx,//EVP_CIPHER_CTX *ctx,
		m_cipher,//const EVP_CIPHER *cipher, 
		NULL,//ENGINE *impl,
		&m_key[0],//const unsigned char *key, 
		&m_iv[0]//const unsigned char *iv
		);
	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_DecryptInit_ex"), hr, TEXT("EVP_DecryptInit_ex"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_DecryptInit_ex"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}


	int BlockSize = EVP_CIPHER_CTX_block_size(&ctx);
	if (origSize < 1024 || origSize < transformedSize || origSize < AtlAlignUp(transformedSize, BlockSize))
	{
		hr = TPM_E_PCP_BUFFER_TOO_SMALL;
		D_API_ERR(0, TEXT("buffer_size"), hr, TEXT("transformedSize=%d, origSize=%d, BlockSize=%d"), transformedSize, origSize, BlockSize);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	int decLen = -1;
	ret = EVP_DecryptUpdate(
		&ctx,//EVP_CIPHER_CTX *ctx, 
		origBuffer,//unsigned char *out,
		&decLen,//int *outl, 
		transformedBuffer,//const unsigned char *in, 
		transformedSize//int inl
		);
	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_DecryptUpdate"), hr, TEXT("EVP_DecryptUpdate"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_DecryptUpdate"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	int decLenFinal = -1;
	ret = EVP_DecryptFinal_ex(
		&ctx,//EVP_CIPHER_CTX *ctx, 
		origBuffer + decLen,//unsigned char *outm, 
		&decLenFinal//int *outl
		);
	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_DecryptFinal_ex"), hr, TEXT("EVP_DecryptFinal_ex"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_DecryptFinal_ex"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}
	decLen += decLenFinal;

	outOrigSize = decLen;

	ATLASSERT(outOrigSize <= origSize);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	EVP_CIPHER_CTX_cleanup(&ctx);

	return hr;
}



COpensslComp::COpensslComp(E_COMP _E_COMP, BOOL _bUseCtx) :
m_refCount(0),
m_E_COMP(_E_COMP),
m_method(NULL),
m_ctx(NULL),
m_bReuseCtx(_bUseCtx),
m_zlib_cleanup(TRUE)
{

}

HRESULT COpensslComp::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	switch (m_E_COMP)
	{
	case E_COMP_zlib:
		m_method = COMP_zlib();
		break;
	default:
		m_method = COMP_zlib();
		break;
	}
	if (NULL == m_method)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("m_E_COMP"), hr, TEXT("m_E_COMP"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("m_E_COMP"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	if (m_bReuseCtx)
	{
		m_ctx = COMP_CTX_new(m_method);
		if (NULL == m_ctx)
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("COMP_CTX_new"), hr, TEXT("COMP_CTX_new"));
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("COMP_CTX_new"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}
	}
	

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT COpensslComp::DestroyMember()
{
	if (m_ctx)
	{
		COMP_CTX_free(m_ctx);
		m_ctx = NULL;
		//COMP_zlib_cleanup();
	}

	if (m_zlib_cleanup)
	{
		COMP_zlib_cleanup();
		m_zlib_cleanup = FALSE;
	}

	return S_OK;
}

HRESULT COpensslComp::Transform(LPCBYTE origBuffer, LONG origSize, LPBYTE transformedBuffer, LONG transformedSize, LONG & outTransformedSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	COMP_CTX * ctx = NULL; /**CLEANUP**/

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////

	if (m_bReuseCtx)
	{
		ctx = m_ctx;
	}
	else
	{
		ctx = COMP_CTX_new(m_method);
		if (NULL == ctx)
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("COMP_CTX_new"), hr, TEXT("COMP_CTX_new"));
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("COMP_CTX_new"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}
	}


	if (transformedSize < 1024 || transformedSize < origSize)
	{
		hr = TPM_E_PCP_BUFFER_TOO_SMALL;
		D_API_ERR(0, TEXT("buffer_size"), hr, TEXT("transformedSize=%d, origSize=%d"), transformedSize, origSize);
		LEAVE_BLOCK(0);
	}
	

	int len = COMP_compress_block(
		ctx,//COMP_CTX *ctx, 
		transformedBuffer,//unsigned char *out, 
		transformedSize,//int olen,
		(unsigned char *)origBuffer,//unsigned char *in, 
		origSize//int ilen
		);
	if (len <= 0)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("COMP_compress_block"), hr, TEXT("COMP_compress_block"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("COMP_compress_block"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	outTransformedSize = len;

	ATLASSERT(outTransformedSize <= transformedSize);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	if (!m_bReuseCtx)
	{
		if (ctx)
		{
			COMP_CTX_free(ctx);
			ctx = NULL;
		}
	}

	return hr;
}

HRESULT COpensslComp::Recover(LPCBYTE transformedBuffer, LONG transformedSize, LPBYTE origBuffer, LONG origSize, LONG & outOrigSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	COMP_CTX * ctx = NULL; /**CLEANUP**/

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_bReuseCtx)
	{
		ctx = m_ctx;
	}
	else
	{
		ctx = COMP_CTX_new(m_method);
		if (NULL == ctx)
		{
			m_ErrorCallback.CollectErrors(TRUE);
			hr = E_FAIL;
			D_API_ERR(0, TEXT("COMP_CTX_new"), hr, TEXT("COMP_CTX_new"));
			CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
			for (size_t ii = 0; ii < errors.size(); ++ii)
			{
				D_API_ERR(0, TEXT("COMP_CTX_new"), hr, TEXT("%S"), errors[ii].GetString());
			}
			LEAVE_BLOCK(0);
		}

	}

	if (origSize < 1024 || origSize < transformedSize)
	{
		hr = TPM_E_PCP_BUFFER_TOO_SMALL;
		D_API_ERR(0, TEXT("buffer_size"), hr, TEXT("transformedSize=%d, origSize=%d"), transformedSize, origSize);
		LEAVE_BLOCK(0);
	}

	int len = COMP_expand_block(
		ctx,//COMP_CTX *ctx, 
		origBuffer,//unsigned char *out, 
		origSize,//int olen,
		(unsigned char *)transformedBuffer,//unsigned char *in, 
		transformedSize//int ilen
		);
	if (len <= 0)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("COMP_expand_block"), hr, TEXT("COMP_expand_block"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("COMP_expand_block"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	outOrigSize = len;


	ATLASSERT(outOrigSize <= origSize);

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	if (!m_bReuseCtx)
	{
		if (ctx)
		{
			COMP_CTX_free(ctx);
			ctx = NULL;
		}
	}
	

	return hr;
}


HRESULT CreateInstanceCOpensslEnc(E_CIPPHER _E_CIPPHER, E_MD _E_MD, LPCBYTE pSalt, LONG saltLen, LPCBYTE password, LONG pwdLen, ITransform ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new COpensslEnc(_E_CIPPHER, _E_MD, pSalt, saltLen, password, pwdLen), ppObj, bCreateMember);
}

HRESULT CreateInstanceCOpensslComp(E_COMP _E_COMP, BOOL _bReuseCtx, ITransform ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new COpensslComp(_E_COMP, _bReuseCtx), ppObj, bCreateMember);
}

HRESULT CreateInstanceCOpensslMD(E_MD e_md, LPCBYTE pSalt, LONG saltLen, IMessageDigest ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new COpensslMD(e_md, pSalt, saltLen), ppObj, bCreateMember);
}

HRESULT CreateInstanceCOpensslBase64(ITransform ** ppObj, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(new COpensslBase64(), ppObj, bCreateMember);
}

COpensslMD::COpensslMD(E_MD e_md, LPCBYTE pSalt, LONG saltLen) :
m_RefCount(0),
m_E_MD(e_md),
m_md(NULL)
{
	if (pSalt &&  saltLen)
	{
		m_salt.assign(pSalt, pSalt + saltLen);
	}
	else
	{
		GenerateSalt(m_salt, 256);
	}
}

HRESULT COpensslMD::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	switch (m_E_MD)
	{
	case E_MD_md5:
		m_md = EVP_md5();
		break;
	case E_MD_sha:
		//m_md = EVP_sha();
		//break;
	case E_MD_sha1:
		m_md = EVP_sha1();
		break;
	case E_MD_sha256:
		m_md = EVP_sha256();
		break;
	case E_MD_sha512:
		m_md = EVP_sha512();
		break;
	default:
		m_md = EVP_sha512();
		break;
	}
	if (NULL == m_md)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("MD"), hr, TEXT("MD"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("MD"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT COpensslMD::DestroyMember()
{
	return S_OK;
}

HRESULT COpensslMD::Hash(LPCBYTE pData, LONG dataLen, vector<BYTE> & mdValue)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	EVP_MD_CTX& ctx = *EVP_MD_CTX_new(); /**CLEANUP**/
	EVP_MD_CTX_init(&ctx);

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	
	int ret = EVP_DigestInit_ex(&ctx, m_md, NULL);
	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_DigestInit_ex"), hr, TEXT("EVP_DigestInit_ex"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_DigestInit_ex"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}
	

	ret = EVP_DigestUpdate(&ctx, pData, dataLen);
	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_DigestUpdate"), hr, TEXT("EVP_DigestUpdate"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_DigestUpdate"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}

	ret = EVP_DigestUpdate(&ctx, &m_salt[0], m_salt.size());
	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_DigestUpdate"), hr, TEXT("EVP_DigestUpdate"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_DigestUpdate"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}


	int MD_size = EVP_MD_size(m_md);
	mdValue.resize(MD_size);
	UINT size = 0;
	ret = EVP_DigestFinal_ex(&ctx, &mdValue[0], &size);
	if (0 == ret)
	{
		m_ErrorCallback.CollectErrors(TRUE);
		hr = E_FAIL;
		D_API_ERR(0, TEXT("EVP_DigestFinal_ex"), hr, TEXT("EVP_DigestFinal_ex"));
		CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
		for (size_t ii = 0; ii < errors.size(); ++ii)
		{
			D_API_ERR(0, TEXT("EVP_DigestFinal_ex"), hr, TEXT("%S"), errors[ii].GetString());
		}
		LEAVE_BLOCK(0);
	}
	
	ATLASSERT(size == MD_size);
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//EVP_MD_CTX_cleanup(&ctx);
	EVP_MD_CTX_destroy(&ctx);

	return hr;
}

COpensslBase64::COpensslBase64() :
m_RefCount(0)
{

}

HRESULT COpensslBase64::CreateMember()
{
	return S_OK;
}

HRESULT COpensslBase64::DestroyMember()
{
	return S_OK;
}

HRESULT COpensslBase64::Transform(LPCBYTE origBuffer, LONG origSize, LPBYTE transformedBuffer, LONG transformedSize, LONG & outTransformedSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (transformedSize < 64 || transformedSize < origSize + origSize)
	{
		hr = TPMAPI_E_BUFFER_TOO_SMALL;
		D_API_ERR(0, TEXT("Transform"), hr, TEXT("transformedSize=%d, origSize=%d"), transformedSize, origSize);
		break;
	}

	outTransformedSize = EVP_EncodeBlock(transformedBuffer, origBuffer, origSize);


	ATLASSERT(outTransformedSize <= transformedSize);
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT COpensslBase64::Recover(LPCBYTE transformedBuffer, LONG transformedSize, LPBYTE origBuffer, LONG origSize, LONG & outOrigSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (origSize < 64 || origSize < transformedSize)
	{
		hr = TPMAPI_E_BUFFER_TOO_SMALL;
		D_API_ERR(0, TEXT("Transform"), hr, TEXT("transformedSize=%d, origSize=%d"), transformedSize, origSize);
		break;
	}


	outOrigSize = EVP_DecodeBlock(origBuffer, transformedBuffer, transformedSize);

	ATLASSERT(outOrigSize <= origSize);

	int padLen = 0;
	for (int ii = 0; ii < 4; ++ii)
	{
		if (transformedBuffer[transformedSize - 1 - ii] == '=')
		{
			++padLen;
		}
		else
		{
			break;
		}
	}
	outOrigSize -= padLen;

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}
