#pragma once

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicInterfaceSocketClient.h"

#include "WSADataBuffer.h"
#include "SocketFileRPCDefines.h"

class CSocketFileDeviceClient : public IFileDeviceSocketClient
{
private:
	LONG m_RefCount; /***Internal***/
	ISocketClient * m_pISocketClient; /***Incomming***/
	BOOL m_RemoteObjectCreated;

	WSADataBufferT m_SendBuffer; /***Internal to cleanup***/
	WSADataBufferT m_RcvBuffer; /***Internal to cleanup***/

	//IN CString m_strFileName; 
	//IN ULONG m_ulDesiredAccess; 
	//IN ULONG m_ulShareMode; 
	//IN ULONG m_ulCreationDisposition; 
	//IN ULONG m_ulFlagsAndAttributes; 

public:
	CSocketFileDeviceClient(ISocketClient * pISocketClient, LONG MaxBinaryDataLen //,
		//IN LPCTSTR strFileName, 
		//IN ULONG ulDesiredAccess, 
		//IN ULONG ulShareMode, 
		//IN ULONG ulCreationDisposition, 
		//IN ULONG ulFlagsAndAttributes
		);

	IMP_CLASS_DESTRUCTOR(CSocketFileDeviceClient);

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
	IMP_IUNKNOWN_METHODS(m_RefCount);

	//virtual LPCTSTR WhoAmI() = 0;
	IMP_WHO_AM_I(TEXT("CSocketFileDeviceClient:IFileDevice"));

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod);
	virtual HRESULT Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer);
	virtual HRESULT Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer);
	virtual HRESULT GetFileSize(LONGLONG * pFileSize);
	virtual HANDLE GetRawHandle();
	//};

	virtual HRESULT ReadAtOffsetHelper(LONGLONG Offset, ULONG nNumberOfBytesToRead, ULONG * pNumberOfBytesRead, BYTE* pBuffer );

	virtual HRESULT WriteAtOffsetHelper(LONGLONG Offset, ULONG nNumberOfBytesToWrite, ULONG * pNumberOfBytesWritten, CONST BYTE* pBuffer );

	//////////////////////////////////////////////////////////////////////////
	virtual HRESULT ClientCreateInstanceFileDeviceWin32(
		IN LPCTSTR strFileName, 
		IN ULONG ulDesiredAccess, 
		IN ULONG ulShareMode, 
		IN ULONG ulCreationDisposition, 
		IN ULONG ulFlagsAndAttributes, 
		BOOL bCreateMember);
	virtual ULONG ClientRelease();

public:
	BYTE * GetSendBuffer();

	BYTE * GetReceiveBuffer();
};

HRESULT CreateInstanceCSocketFileDeviceClient( ISocketClient * pISocketClient, 
											  LONG MaxBinaryDataLen, 
											  IFileDeviceSocketClient ** ppObj, 
											  BOOL bCreateMember );