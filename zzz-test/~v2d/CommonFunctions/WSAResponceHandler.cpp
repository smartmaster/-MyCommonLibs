#include "stdafx.h"
#include "WSAResponceHandler.h"


HRESULT CEchoResponceHandler::Process( IN CONST TWSAIoBuffer & InBuffer, OUT TWSAIoBuffer & OutBuffer )
{
	OutBuffer.m_DataLength = InBuffer.m_DataLength;
	if (OutBuffer.m_DataLength > OutBuffer.m_AllocatedSize)
	{
		OutBuffer.RellocateBuffer(OutBuffer.m_DataLength);
	}
	for (ULONG ii = 0; ii < OutBuffer.m_DataLength; ++ii)
	{
		OutBuffer.m_pBuffer[ii] = InBuffer.m_pBuffer[ii] + 1;
	}

	return S_OK;
}

HRESULT CEchoResponceHandler::Release()
{
	delete this;
	return S_OK;
}

HRESULT CEchoResponceHandlerFactory::CreateInstance( IResponceHandler ** ppResponceHandler )
{
	*ppResponceHandler = new CEchoResponceHandler;
	return S_OK;
}

HRESULT CEchoResponceHandlerFactory::Release()
{
	delete this;
	return S_OK;
}
