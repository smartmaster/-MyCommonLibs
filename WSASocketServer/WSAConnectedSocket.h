#pragma once

#include <winsock2.h>
#include <Ws2tcpip.h>

#include "..\PublicHeader\PublicSimpleUnknown.h"
#include "..\PublicHeader\PublicStopTimer.h"

#include "WSADataBuffer.h"
#include "InterfaceWSADataBufferHandler.h"

enum EnumIoOperation
{
	IO_RECEIVE,
	IO_SEND
};

struct WSAConnectedSocketT
{
	//////////////////////////////////////////////////////////////////////////
	SOCKET				m_ConnectedSocket; //INTERNAL - TO cleanup
	EnumIoOperation		m_IoOperation;
	WSAOVERLAPPED		m_Overlapped;

	CONST ULONG			m_AllocatedSize;
	WSADataBufferT		m_RecvBuffer;  //INTERNAL - TO cleanup
	WSADataBufferT		m_SendBuffer; //INTERNAL - TO cleanup

	IDataBufferHandler	* m_pIDataBufferHandler; //INTERNAL - TO cleanup

	LONGLONG m_TotalSent; /***INTERNAL***/
	LONGLONG m_TotalReceived; /***INTERNAL***/

	CStopTimer m_StopTimer; /***INTERNAL***/

public:
	//////////////////////////////////////////////////////////////////////////
	static CONST LONG EXIT_LENGTH = -1;
	static CONST LONG MAX_BUFFER_SIZE = 64 * 1024 * 1024;

public:
	//////////////////////////////////////////////////////////////////////////
	WSAConnectedSocketT(SOCKET ConnectedSocket, ULONG AllocatedSize, IDataBufferHandler *	pResponceHandler);
	~WSAConnectedSocketT();

	HRESULT CreateMemer();
	HRESULT DestroyMember();
	

	//////////////////////////////////////////////////////////////////////////
	VOID PrepareForReceive();
	VOID PrepareForSend();

	//////////////////////////////////////////////////////////////////////////
	HRESULT OnIOCompleted(ULONG NumberOfBytes);
	HRESULT OnReceiveCompleted(ULONG NumberOfBytes);
	HRESULT OnSendCompleted( ULONG NumberOfBytes );

	LONGLONG GetTotalSentSize();
	LONGLONG GetTotalReceivedSize();
};