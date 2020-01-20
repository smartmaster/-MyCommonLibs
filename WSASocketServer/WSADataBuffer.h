#pragma once

#include <tchar.h>
#include <atlbase.h>
#include <windows.h>

/********************************************************************************************
for SOCKET based RPC:
m_pData:		|--------------------------Binary Data-------------------------|---any data----|------Parameter Streams------|--Parameter Streams Offset--|
				|<----------------------------Parameter Streams Offset------------------------>|
m_DataLength:	|<----------------------------------------------------m_DataLength------------------------------------------------------------------------>|
********************************************************************************************/

struct WSADataBufferT
{
	//////////////////////////////////////////////////////////////////////////
	LONG							m_DataLengthReady;
	LONG							m_DataLength;
	LONG							m_Transfered;
	LONG							m_AllocatedSize;
	BYTE*							m_pData;  //INTERNAL

	//////////////////////////////////////////////////////////////////////////
	WSADataBufferT(ULONG AllocatedSize);
	~WSADataBufferT();

	HRESULT CreateMember();
	HRESULT DestroyMember();
	HRESULT RellocateBuffer(ULONG NewSize, BOOL CopyOldData);
};

