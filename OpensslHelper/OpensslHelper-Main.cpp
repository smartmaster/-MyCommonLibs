// OpensslHelper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string.h>

#include "OpensslHelper.h"
#include <openssl/comp.h>
#include "CompressEncryptInterface.h"
#include "SslSocketClient.h"


int    main_test_comp()
{
	COMP_CTX          *ctx;
	int                  len, olen = 100, ilen = 50, total = 0;
	unsigned char in[50] = "here i am ..............................", out[100];
	unsigned char expend[200];

	COMP_METHOD * method = COMP_zlib();

	ctx = COMP_CTX_new(method);
	
	total = COMP_compress_block(ctx, out, olen, in, 50);

	len = COMP_expand_block(ctx, expend, 200, out, total);

	COMP_CTX_free(ctx);

	return 0;

}

HRESULT test_transform(ITransform * spITransform)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	const LONG BUFF_LEN = 2048;

	BYTE buffer1[] = "hello";
	//BYTE buffer1[] = "EVP_CIPHER_asn1_to_param() sets the cipher parameters based on an ASN1 AlgorithmIdentifier ``parameter''."
	//	"The precise effect depends on the cipher In the case of RC2, for example, it will set the IV and effective key length."
	//	" This function should be called after the base cipher type is set but before the key is set. "
	//	"For example EVP_CipherInit() will be called with the IV and key set to NULL, EVP_CIPHER_asn1_to_param() will be called and finally EVP_CipherInit()"
	//	" again with all parameters except the key set to NULL. It is possible for this function to fail if the cipher "
	//	"does not have any ASN1 support or the parameters cannot be set (for example the RC2 effective key length is not supported. ";
	LONG size1 = sizeof(buffer1);

	BYTE buffer2[BUFF_LEN] = { 0 };
	LONG size2 = sizeof(buffer2);

	BYTE buffer3[BUFF_LEN] = { 0 };
	LONG size3 = sizeof(buffer3);

	LONG outSize2 = 0;
	hr = spITransform->Transform(buffer1, size1, buffer2, size2, outSize2);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Transform"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	LONG outSize3 = 0;
	hr = spITransform->Recover(buffer2, outSize2, buffer3, size3, outSize3);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("Recover"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	ATLASSERT(size1 == outSize3 && 0 == memcmp(buffer1, buffer3, size1));
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}


int _tmain_enc(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	BYTE pwd[] = "fjalkeufi7489234fjsdklfuyei7897j";
	CComPtr<ITransform> spITransform;
	int testcase = 0;
	if (0 == testcase)
	{
		BOOL _bReuseCtx = FALSE;
		hr = CreateInstanceCOpensslComp(E_COMP_zlib, _bReuseCtx, &spITransform, TRUE);
	}
	else if (1 == testcase)
	{
		hr = CreateInstanceCOpensslBase64(&spITransform, TRUE);
	}
	else
	{
		hr = CreateInstanceCOpensslEnc(E_CIPPHER_aes_256_cbc, E_MD_sha512, NULL, 0, pwd, _countof(pwd) - 1, &spITransform, TRUE);
	}
	
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateInstanceCOpensslEncDec"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	for (int ii = 0; ii < 5; ++ ii)
	{
		test_transform(spITransform);
	}
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;

}


int  test_hash()
{

	EVP_MD_CTX *mdctx;
	const EVP_MD *md;
	char mess1[] = "Test Message\n";
	char mess2[] = "Hello World\n";
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len, i;

	OpenSSL_add_all_digests();

	//if (!argv[1]) {
	//	printf("Usage: mdtest digestname\n");
	//	exit(1);
	//}

	md = EVP_sha512();//; EVP_get_digestbyname(argv[1]);

	//if (!md) {
	//	printf("Unknown message digest %s\n", argv[1]);
	//	exit(1);
	//}

	mdctx = EVP_MD_CTX_create();
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
	EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	EVP_MD_CTX_destroy(mdctx);

	printf("Digest is: ");
	for (i = 0; i < md_len; i++) printf("%02x", md_value[i]);
	printf("\n");

	CComPtr<IMessageDigest> spIMessageDigest;
	HRESULT hr = CreateInstanceCOpensslMD(E_MD_sha512, (LPCBYTE)mess2, strlen(mess2), &spIMessageDigest, TRUE);

	for (LONG ii = 0; ii < 5; ++ ii)
	{
		vector<BYTE> mdValue;
		spIMessageDigest->Hash((LPCBYTE)mess1, strlen(mess1), mdValue);
		ATLASSERT(md_len == mdValue.size() && 0 == memcmp(md_value, &mdValue[0], md_len));
	}

	return 0;

}


int _tmain(int argc, _TCHAR* argv[])
{

	//HANDLE hFile = CreateFile2(
	//	TEXT("sdsdasd.tduastduayd"),//_In_      LPCWSTR lpFileName,
	//	GENERIC_READ,//_In_      DWORD dwDesiredAccess,
	//	0,//_In_      DWORD dwShareMode,
	//	OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
	//	NULL//_In_opt_  LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams
	//	);
	WSADATA WSAData = { 0 };
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SslLibInit();


	int testCase = 0;

	switch (testCase)
	{
	case 4:
	{
		LPCTSTR pszServer = argv[1];
		LPCTSTR pPort = argv[2];
		CComPtr<ISocketClient> spObj;
		HRESULT hr = CreateInstanceCSslSocketClient(
			pszServer,//LPCTSTR pszServer,
			pPort,//LPCTSTR pPort,
			&spObj,//ISocketClient ** ppObj,
			FALSE,//BOOL bSendAll,
			FALSE,//BOOL bReceiveAll,
			nullptr,//LPCTSTR pCertFile,
			nullptr,//LPCTSTR PrivateKeyFile,
			nullptr,//LPCTSTR pVerifyPeerCert,
			nullptr,//LPCTSTR pKeyFilePwd,
			TRUE//BOOL bCreateMember
			);
	}
	break;

	case 3:
	{
		//HMODULE hm = LoadLibraryW(L"zlib1.dll");
		//FreeLibrary(hm); hm = NULL;
	}
	break;

	case 2:
	{
		return _tmain_enc(argc, argv);
	}
	break;

	case 1:
	{
		return main_test_comp();
	}
	break;

	case 0:
	{
		return test_hash();
	}
	break;
	}
	
	
	return 0;
}

