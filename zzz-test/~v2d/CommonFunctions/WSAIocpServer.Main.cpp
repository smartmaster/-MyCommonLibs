// WSAIocpServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "WSASocketServerModule.h"

VOID PrintUsage()
{
	_ftprintf_s(stdout, TEXT("Usage:") TEXT("\r\n"));
	_ftprintf_s(stdout, TEXT("\t") TEXT("this.exe <Port> <ThreadCount>") TEXT("\r\n"));
}

static CWSASocketServerModule * g_SocketServerModule = NULL;

BOOL WINAPI CtrlHandler (DWORD dwEvent) 
{
	switch( dwEvent ) 
	{
	case CTRL_BREAK_EVENT: 
	case CTRL_C_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_CLOSE_EVENT:
		if (g_SocketServerModule)
		{
			g_SocketServerModule->EndServer();
		}
		break;

	default:
		return(FALSE);
	}
	return(TRUE);
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3)
	{
		PrintUsage();
		return E_INVALIDARG;
	}

	//////////////////////////////////////////////////////////////////////////
	SetConsoleCtrlHandler(CtrlHandler, TRUE);

	//////////////////////////////////////////////////////////////////////////
	WSADATA WsaData = {0};
	WSAStartup(
		MAKEWORD(2, 2),//__in   WORD wVersionRequested,
		&WsaData//__out  LPWSADATA lpWSAData
		);
	
	
	LPCTSTR pPort = argv[1];
	ULONG ThreadCount = _tcstoul(argv[2], NULL, 0);

	IResponceHandlerFactory * pResponceHandlerFactory = new CEchoResponceHandlerFactory;

	CWSASocketServerModule Server(pPort, ThreadCount, pResponceHandlerFactory);
	Server.CreateMember(TRUE);
	
	g_SocketServerModule = &Server;
	Server.Run();
	g_SocketServerModule = NULL;
	
	Server.DestroyMember();

	pResponceHandlerFactory->Release();

	WSACleanup();
	return 0;
}

