#pragma once

#include <tchar.h>
#include <atlbase.h>
#include <atlstr.h>
#include <windows.h>

#include "InterfaceSocketClient.h"

class CSocketHandleWrapper : public ISocketClient
{
private:
	SOCKET m_SocketConnection; /***INCOMMING***//***INTERNAL***//***TO CLEANUP***/
	BOOL m_bAttachSocketHandle;
	LONG m_RefCount; /***INTERNAL***/

	BOOL m_bSendAll; /***INCOMMING***/ //<sonmi01>2013-12-6 ###???
	BOOL m_bReceiveAll; /***INCOMMING***/

	LONGLONG m_TotalSent; /***INTERNAL***/
	LONGLONG m_TotalReceived; /***INTERNAL***/

public:
	CSocketHandleWrapper(SOCKET SocketConnection, BOOL bAttachSocketHandle, BOOL bSendAll, BOOL bReceiveAll);

	IMP_CLASS_DESTRUCTOR(CSocketHandleWrapper);

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
	IMP_WHO_AM_I(TEXT("CSocketHandleWrapper:ISocketClient"));

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

};


class CSocketClient : public ISocketClient
{
private:
	CString m_strServer; /***Incomming***/
	CString m_Port; /***Incomming***/
	//SOCKET m_SocketConnection; /***Internal to cleanup***/
	LONG m_RefCount; /***INTERNAL***/

	BOOL m_bSendAll; /***INCOMMING***/ //<sonmi01>2013-12-6 ###???
	BOOL m_bReceiveAll; /***INCOMMING***/
	
	//LONGLONG m_TotalSent; /***INTERNAL***/
	//LONGLONG m_TotalReceived; /***INTERNAL***/

	CSocketHandleWrapper * m_pSockWapper; /***INTERNAL***/ /***TO CLEANUP***/

public:
	CSocketClient(LPCTSTR pszServer, LPCTSTR pPort, BOOL bSendAll, BOOL bReceiveAll); //<sonmi01>2013-12-6 ###???
	IMP_CLASS_DESTRUCTOR(CSocketClient);

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
	IMP_WHO_AM_I(TEXT("CSocketClient:ISocketClient"));

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



