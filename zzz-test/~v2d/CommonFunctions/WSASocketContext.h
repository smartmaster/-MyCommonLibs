#pragma once

#include "stdafx.h"

#include <winsock2.h>
#include <Ws2tcpip.h>

#include "WSAIoBuffer.h"
#include "WSAResponceHandler.h"

enum EIoOperation
{
	IO_READ,
	IO_WRITE
};

struct TWSASocketContext
{
	//////////////////////////////////////////////////////////////////////////
	SOCKET					m_ConnectedSocket; //INTERNAL - TO cleanup
	EIoOperation				m_IoOperation;
	WSAOVERLAPPED		m_Overlapped;

	CONST ULONG			m_AllocatedSize;
	TWSAIoBuffer			m_RecvBuffer;  //INTERNAL - TO cleanup
	TWSAIoBuffer			m_SendBuffer; //INTERNAL - TO cleanup

	IResponceHandler	*	m_pResponceHandler; //INTERNAL - TO cleanup

	//////////////////////////////////////////////////////////////////////////
	static CONST ULONG EXIT_LENGTH = -1;


	//////////////////////////////////////////////////////////////////////////
	TWSASocketContext(SOCKET ConnectedSocket, ULONG AllocatedSize, IResponceHandler	*	pResponceHandler);
	HRESULT CreateMemer();
	HRESULT DestroyMember();
	~TWSASocketContext();

	//////////////////////////////////////////////////////////////////////////
	VOID PrepareForRead();
	VOID PrepareForWrite();

	//////////////////////////////////////////////////////////////////////////
	HRESULT OnIOCompleted(ULONG NumberOfBytes);
	HRESULT OnReadCompleted(ULONG NumberOfBytes);
	HRESULT OnWriteCompleted( ULONG NumberOfBytes );
};