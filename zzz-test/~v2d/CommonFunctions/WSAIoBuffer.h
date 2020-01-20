#pragma once

#include "stdafx.h"


struct TWSAIoBuffer
{
	//////////////////////////////////////////////////////////////////////////
	BOOL							m_DataLengthReady;
	ULONG							m_DataLength;
	ULONG							m_Transfered;
	ULONG							m_AllocatedSize;
	BYTE*							m_pBuffer;  //INTERNAL

	//////////////////////////////////////////////////////////////////////////
	TWSAIoBuffer(ULONG AllocatedSize);
	HRESULT CreateMember();
	HRESULT RellocateBuffer(ULONG Length);
	HRESULT DestroyMember();
	~TWSAIoBuffer();
};