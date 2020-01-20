#pragma once

#include "..\PublicHeader\PublicSimpleUnknown.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"

struct ISocketClient : public IFileDevice
{
	//struct IFileDevice : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};

	//	//	virtual LPCTSTR WhoAmI() = 0;
	//	//	virtual HRESULT CreateMember() = 0;
	//	//	virtual HRESULT DestroyMember() = 0;
	//	//};

	//	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod) = 0;
	//	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer) = 0;
	//	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer) = 0;
	//	virtual HRESULT GetFileSize(LONGLONG * pFileSize) = 0;

	//};
	virtual HRESULT SendLengthAndBuffer(LONG Length, CONST BYTE * Buffer, LONG * LengthSent) = 0;
	virtual HRESULT Disconnect() = 0;

	virtual LONGLONG GetTotalSentSize() = 0;
	virtual LONGLONG GetTotalReceivedSize() = 0;
};

//////////////////////////////////////////////////////////////////////////
HRESULT SendLengthAndBuffer(ISocketClient * pISocketClient, LONG Length, CONST BYTE * Buffer, LONG * LengthSent);
HRESULT ReceiveLengthAndBuffer(ISocketClient * pISocketClient, LONG & Length, BYTE * Buffer, LONG * LengthRcv);

//////////////////////////////////////////////////////////////////////////
HRESULT DrainSocket(IFileDevice * pSocket, ULONG TimeOut);

//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceCSocketClient(LPCTSTR pszServer, LPCTSTR pPort, ISocketClient ** ppObj, BOOL bSendAll, BOOL bReceiveAll, BOOL bCreateMember ); //<sonmi01>2013-12-6 ###???

HRESULT CreateInstanceCSocketHandleWrapper(SOCKET SocketConnection, BOOL bAttachSocketHandle, ISocketClient ** ppObj, BOOL bSendAll, BOOL bReceiveAll, BOOL bCreateMember);


HRESULT GetServerSslCertKeyFiles(CString & CertFile, CString & PrivateKeyFile, CString & VerifyPeerCert, CString & KeyFilePwd);
HRESULT GetClientSslCertKeyFiles(CString & CertFile, CString & PrivateKeyFile, CString & VerifyPeerCert, CString & KeyFilePwd);


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
