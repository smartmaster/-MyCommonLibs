#pragma once

#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>

#include "..\PublicHeader\PublicInterfaceSocketClient.h"

#include "OpensslHelper.h"

class CSslSocketHandleWrapper : public ISocketClient
{
private:
	SOCKET m_SocketConnection; /***INCOMMING***//***INTERNAL***//***TO CLEANUP***/
	BOOL m_bAttachSocketHandle;
	LONG m_RefCount; /***INTERNAL***/

	BOOL m_bSendAll; /***INCOMMING***/ //<sonmi01>2013-12-6 ###???
	BOOL m_bReceiveAll; /***INCOMMING***/

	LONGLONG m_TotalSent; /***INTERNAL***/
	LONGLONG m_TotalReceived; /***INTERNAL***/

	LONGLONG m_TotalSentSsl; /***INTERNAL***/
	LONGLONG m_TotalReceivedSsl; /***INTERNAL***/

	BOOL m_bServer; /***INCOMMING***/
	CStringA m_CertFile; /***INCOMMING***/
	CStringA m_PrivateKeyFile; /***INCOMMING***/
	CStringA m_VerifyPeerCert; /***INCOMMING***/
	CStringA m_KeyFilePwd; /***INCOMMING***/
	CSslErrorCallback m_ErrorCallback; /***INTERNAL***/
	SSL_CTX * m_ctx; /***INTERNAL***//***TO CLEANUP***/
	SSL * m_ssl; /***INTERNAL***/ /***TO CLEANUP***/


public:
	CSslSocketHandleWrapper(
		SOCKET SocketConnection,
		BOOL bAttachSocketHandle,
		BOOL bSendAll,
		BOOL bReceiveAll,
		BOOL bServer,
		LPCTSTR pCertFile,
		LPCTSTR PrivateKeyFile,
		LPCTSTR pVerifyPeerCert,
		LPCTSTR pKeyFilePwd
		);

	IMP_CLASS_DESTRUCTOR(CSslSocketHandleWrapper);

	//struct ISocketClient : public IFileDevice
	//{
	//	//struct IFileDevice : public ISimpleUnknown
	//	//{
	//	//	//struct ISimpleUnknown : public IUnknown
	//	//	//{
	//	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	//	IUnknown
	//	//	//	//{
	//	//	//	//public:
	//	//	//	//	BEGIN_INTERFACE
	//	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//	//		/* [in] */ REFIID riid,
	//	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	//	END_INTERFACE
	//	//	//	//};
	IMP_IUNKNOWN_METHODS(m_RefCount);
	IMP_WHO_AM_I(TEXT("CSslSocketHandleWrapper:ISocketClient"));

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//	//};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT Read(ULONG nToReceive, ULONG * pnReceived, BYTE* pBuffer);
	virtual HRESULT Write(ULONG nToSend, ULONG * pnSent, LPCBYTE pBuffer);
	virtual HRESULT GetFileSize(LONGLONG * pFileSize);
	virtual HANDLE GetRawHandle();
	//	//};
	virtual HRESULT SendLengthAndBuffer(LONG Length, CONST BYTE * Buffer, LONG * LengthSent);
	virtual HRESULT Disconnect();

	virtual LONGLONG GetTotalSentSize();
	virtual LONGLONG GetTotalReceivedSize();
	//};

private:
	static long ssl_io_callback(
		BIO *b, 
		int oper, 
		const char *argp,
		int argi, 
		long argl, 
		long retvalue);

	static VOID SetSslIoCallback(SSL* ssl, CSslSocketHandleWrapper * pobj);
};


class CSslSocketClient : public ISocketClient
{
private:
	CString m_strServer; /***Incomming***/
	CString m_Port; /***Incomming***/
	LONG m_RefCount; /***INTERNAL***/

	BOOL m_bSendAll; /***INCOMMING***/ //<sonmi01>2013-12-6 ###???
	BOOL m_bReceiveAll; /***INCOMMING***/
	
	//LONGLONG m_TotalSent; /***INTERNAL***/
	//LONGLONG m_TotalReceived; /***INTERNAL***/

	CString m_CertFile; /***INCOMMING***/
	CString m_PrivateKeyFile; /***INCOMMING***/
	CString m_VerifyPeerCert; /***INCOMMING***/
	CString m_KeyFilePwd; /***INCOMMING***/
	CSslSocketHandleWrapper * m_pSslSocket; /***INTERNAL***/ /***TO CLEANUP***/

public:
	CSslSocketClient(
		LPCTSTR pszServer, 
		LPCTSTR pPort, 
		BOOL bSendAll, 
		BOOL bReceiveAll,
		LPCTSTR pCertFile,
		LPCTSTR PrivateKeyFile,
		LPCTSTR pVerifyPeerCert,
		LPCTSTR pKeyFilePwd
		); //<sonmi01>2013-12-6 ###???
	IMP_CLASS_DESTRUCTOR(CSslSocketClient);

	//struct ISocketClient : public IFileDevice
	//{
	//	//struct IFileDevice : public ISimpleUnknown
	//	//{
	//	//	//struct ISimpleUnknown : public IUnknown
	//	//	//{
	//	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	//	IUnknown
	//	//	//	//{
	//	//	//	//public:
	//	//	//	//	BEGIN_INTERFACE
	//	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//	//		/* [in] */ REFIID riid,
	//	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	//	END_INTERFACE
	//	//	//	//};
	IMP_IUNKNOWN_METHODS(m_RefCount);
	IMP_WHO_AM_I(TEXT("CSslSocketClient:ISocketClient"));

	virtual HRESULT CreateMember() ;
	virtual HRESULT DestroyMember() ;
	//	//	//};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
	virtual HRESULT GetFileSize(LONGLONG * pFileSize);
	virtual HANDLE GetRawHandle();

	//	//};
	virtual HRESULT SendLengthAndBuffer(LONG Length, CONST BYTE * Buffer, LONG * LengthSent);
	virtual HRESULT Disconnect() ;

	virtual LONGLONG GetTotalSentSize();
	virtual LONGLONG GetTotalReceivedSize();
	//};

};


//////////////////////////////////////////////////////////////////////////
HRESULT GetClientSslCertKeyFiles(CString & CertFile, CString & PrivateKeyFile, CString & VerifyPeerCert, CString & KeyFilePwd);

//////////////////////////////////////////////////////////////////////////
////API
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
	BOOL bCreateMember
	);

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
	BOOL bCreateMember
	);

