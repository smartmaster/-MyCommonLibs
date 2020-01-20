#pragma once

#include "stdafx.h"
#include "WSAIoBuffer.h"

struct IResponceHandler
{
	virtual HRESULT Process( IN CONST TWSAIoBuffer & InBuffer, 	OUT TWSAIoBuffer & OutBuffer 	) = NULL;
	virtual HRESULT Release() = NULL;
};

struct IResponceHandlerFactory
{
	virtual HRESULT CreateInstance(IResponceHandler ** ppResponceHandler) = NULL;
	virtual HRESULT Release() = NULL;
};


//////////////////////////////////////////////////////////////////////////
//a simplest sample
struct CEchoResponceHandler : public IResponceHandler
{
	virtual HRESULT Process(	IN CONST TWSAIoBuffer & InBuffer, OUT TWSAIoBuffer & OutBuffer);
	virtual HRESULT Release();
};

class CEchoResponceHandlerFactory : public IResponceHandlerFactory
{
	virtual HRESULT CreateInstance(IResponceHandler ** ppResponceHandler);
	HRESULT Release();
};
//////////////////////////////////////////////////////////////////////////