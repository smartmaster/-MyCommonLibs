#pragma once

//////////////////////////////////////////////////////////////////////////
#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

//////////////////////////////////////////////////////////////////////////
#include <vector>
using namespace std;

//////////////////////////////////////////////////////////////////////////
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

//#include "..\PublicHeadeExtern\openssl\crypto.h"
//#include "..\PublicHeadeExtern\openssl\ssl.h"
//#include "..\PublicHeadeExtern\openssl\err.h"

//////////////////////////////////////////////////////////////////////////
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "..\PublicHeader\PublicMiscHelper.h"

//////////////////////////////////////////////////////////////////////////
ULONG GetSSlError(ULONG err, CStringA & str);
ULONG GetSSlLastError(CStringA & str);

class CSslErrorCallback
{
private:
	vector<CStringA> m_errors;

public:
	CONST vector<CStringA> &  GetErrors() CONST;
	VOID CollectErrors(BOOL bClearOldError);
	
private:
	VOID ClearError();
	VOID OnError(const char *str, size_t len);

private:
	static int SslErrorCallback(const char *str, size_t len, void *u);
};


//////////////////////////////////////////////////////////////////////////
INT SslLibInit();
INT SslLibUninit();



//////////////////////////////////////////////////////////////////////////
class CSsslHelper //not used for now
{
private:
	CSslErrorCallback m_ErrorCallback; /***INTERNAL***/
	SSL_CTX * m_ctx; /***INTERNAL***//***TO CLEANUP***/
	SSL * m_ssl; /***INTERNAL***/ /***TO CLEANUP***/
	SOCKET m_Sokcet; /***INCOMMING***//***INTERNAL***//***TO CLEANUP***/

public:
	CSsslHelper();
	VOID DestroyMember();
	~CSsslHelper();

private:
	//////////////////////////////////////////////////////////////////////////
	HRESULT CtxCreate(LONG SslVersion /*reserved for future use*/);
	HRESULT SetCertPrivatekey(const char * vertFile, const char * privateKeyFile);
	HRESULT SetVerifyPeerCert(const char * caCertFile, int depth);
	HRESULT AttachSocket(SOCKET & sock, BOOL bServer);
	HRESULT SSL_CTX_use_PrivateKey_file_pwd(SSL_CTX *ctx, char *filename, char *pwd);
};


struct OpensslWrappers
{
	//////////////////////////////////////////////////////////////////////////
	////http://stackoverflow.com/questions/9749560/how-to-calculate-x-509-certificates-sha-1-fingerprint-in-c-c-objective-c
	//////////////////////////////////////////////////////////////////////////
	static VOID X509CalcThumbPront(X509 * cert, vector<BYTE> & vecPrint);

#if 0
	static int test_main(int argc, char * argv[]);
#endif

};

#if 0
m_ErrorCallback.CollectErrors(TRUE);
hr = E_FAIL;
D_API_ERR(0, TEXT("xxxxxxxxxx"), hr, TEXT("xxxxxxxx"));
CONST vector<CStringA> & errors = m_ErrorCallback.GetErrors();
for (size_t ii = 0; ii < errors.size(); ++ii)
{
	D_API_ERR(0, TEXT("xxxxxxxx"), hr, TEXT("%S"), errors[ii].GetString());
}
LEAVE_BLOCK(0);
#endif