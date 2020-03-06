#include "stdafx.h"
#include "EchoResponceHandler.h"

#define PrintLine(format, ...) _ftprintf_s(stdout, format L"\r\n", __VA_ARGS__)


LPCTSTR CEchoResponceHandler::WhoAmI()
{
	static CONST TCHAR WHO_AM_I[] = TEXT("CEchoResponceHandler:IDataBufferHandler");
	return WHO_AM_I;
}

HRESULT CEchoResponceHandler::CreateMember()
{
	return S_OK;
}

HRESULT CEchoResponceHandler::DestroyMember()
{
	return S_OK;
}

HRESULT CEchoResponceHandler::Process( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	PrintLine(L"Received: %u '%c...%c'", InBuffer.m_DataLength, InBuffer.m_pData[0], InBuffer.m_pData[InBuffer.m_DataLength-1]);
	OutBuffer.m_DataLength = InBuffer.m_DataLength;
	if (OutBuffer.m_DataLength > OutBuffer.m_AllocatedSize)
	{
		OutBuffer.RellocateBuffer(OutBuffer.m_DataLength, FALSE);
	}

	OutBuffer.m_pData[0] = InBuffer.m_pData[0] + 1;
	OutBuffer.m_pData[OutBuffer.m_DataLength - 1] = InBuffer.m_pData[OutBuffer.m_DataLength - 1] + 1;

	PrintLine(L"To Send: %u '%c...%c'", OutBuffer.m_DataLength, OutBuffer.m_pData[0], OutBuffer.m_pData[OutBuffer.m_DataLength-1]);
	return S_OK;
}

CEchoResponceHandler::CEchoResponceHandler() : 
	m_RefCount(0)
{

}


LPCTSTR CEchoResponceHandlerFactory::WhoAmI()
{
	static CONST TCHAR WHO_AM_I[] = TEXT("CEchoResponceHandlerFactory:IDataBufferHandlerFactory");
	return WHO_AM_I;
}

HRESULT CEchoResponceHandlerFactory::CreateMember()
{
	return S_OK;
}

HRESULT CEchoResponceHandlerFactory::DestroyMember()
{
	return S_OK;
}

HRESULT CEchoResponceHandlerFactory::CreateInstance( IDataBufferHandler ** ppResponceHandler )
{
	return CREATE_INSTCANCE_COBJECT(/*CEchoResponceHandler,*/ (new CEchoResponceHandler), ppResponceHandler, TRUE);
	//CEchoResponceHandler * pObj = new CEchoResponceHandler;
	//pObj->AddRef();
	//*ppResponceHandler = pObj;
	//return S_OK;
}

CEchoResponceHandlerFactory::CEchoResponceHandlerFactory() :
	m_RefCount(0)
{
}


HRESULT CreateInstanceCEchoResponceHandlerFactory(IDataBufferHandlerFactory ** ppFactory)
{
	return CREATE_INSTCANCE_COBJECT(/*CEchoResponceHandlerFactory,*/ (new CEchoResponceHandlerFactory()), ppFactory, TRUE);
	//CEchoResponceHandlerFactory * pObj = new CEchoResponceHandlerFactory();
	//pObj->AddRef();
	//*ppFactory = pObj;
	//return S_OK;
}