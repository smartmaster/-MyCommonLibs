// WSASocketServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "SocketFileDeviceHandler.h"
#include "InterfaceWSASocketServerModule.h"
#include "SocketRunAppHandler.h"
#include "WSALibInit.h"

#include "SocketFileRPCDefines.h"

#include "InterfaceRunApp.h"

static void NOP() {}


int _tmain_stream(int argc, _TCHAR* argv[]);
int _tmain_WSASocketServer(int argc, _TCHAR* argv[]);
int _tmain_download_server(int argc, _TCHAR* argv[]);

static BOOL WINAPI CtrlHandler(DWORD dwEvent);

int _tmain(int argc, _TCHAR* argv[])
{
	D_SETTINGS(9, FALSE, NULL, NULL);

	SetConsoleCtrlHandler(
		CtrlHandler,//_In_opt_  PHANDLER_ROUTINE HandlerRoutine,
		TRUE//_In_      BOOL Add
		);

	CWSALibInit WsaLibInit;

	volatile int testCase = 2;
	switch (testCase)
	{
	case 0:
		return _tmain_stream(argc, argv);
		break;
	case 1:
		return _tmain_WSASocketServer(argc, argv);
		break;
	case 2:
		return _tmain_download_server(argc, argv);
		break;
	default:
		break;
	}
	
	return 0;
}


//////////////////////////////////////////////////////////////////////////
static CComPtr<IWSASocketServerModule> g_SocketServerModule;
static HANDLE g_hStop = NULL;
namespace {
	class CGlobalCreator
	{
	public:
		CGlobalCreator()
		{
			g_hStop = CreateEvent(
				NULL,//_In_opt_  LPSECURITY_ATTRIBUTES lpEventAttributes,
				TRUE,//_In_      BOOL bManualReset,
				FALSE,//_In_      BOOL bInitialState,
				NULL//_In_opt_  LPCTSTR lpName
				);
		}
	};
	CGlobalCreator g_GlobalCreator;
};

static BOOL WINAPI CtrlHandler (DWORD dwEvent) 
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
			g_SocketServerModule.Release();
			SetEvent(g_hStop);
			//Sleep(3000);
		}
		break;

	default:
		return(FALSE);
	}
	return(TRUE);
}


int _tmain_WSASocketServer(int argc, _TCHAR* argv[])
{
	D_SETTINGS(9, FALSE, TEXT("WSAServer-"), TEXT(""));

	D_SLEEP(TEXT(""), 0, 0);

	HRESULT hr = S_OK;

	

	BEGIN_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IDataBufferHandlerFactory> spIDataBufferHandlerFactory;

	volatile int testCase = 1;
	switch (testCase)
	{
	case 0:
		hr = CreateInstanceCEchoResponceHandlerFactory(&spIDataBufferHandlerFactory);
		break;
	case 1:
		hr = CreateInstanceCSocketFileDeviceHandlerFactory(&spIDataBufferHandlerFactory, TRUE);
		break;
	case 2:
		hr = CreateInstanceCRunAppHandlerFactory(&spIDataBufferHandlerFactory, TRUE);
		break;
	case 3:
		{
			LPCTSTR Username = argv[1];
			LPCTSTR Domain = argv[2];
			LPCTSTR Password = argv[3];
			LPCTSTR CommandLine = argv[4];
			LPCTSTR CurDir = argv[5];
			//administrator sonmi01-790 Master16516336 E:\MyDrivers\DriverGenius.exe E:\MyDrivers 

			ULONG ExitCode = 631236;
			RunApp(Username, Domain, Password, CommandLine, CurDir, 0, FALSE, ExitCode);
		}
		break;
	default:
		break;
	}
	
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWSASocketServerModule> spIWSASocketServerModule;
	hr = CreateInstanceCWSASocketServerModule( TEXT("30001"), 0, 2 * 1024 * 1024 + 512 * 1024, spIDataBufferHandlerFactory, FALSE, &spIWSASocketServerModule, TRUE);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	g_SocketServerModule = spIWSASocketServerModule;

	//////////////////////////////////////////////////////////////////////////
	hr = spIWSASocketServerModule->RunServer();
	//////////////////////////////////////////////////////////////////////////
	DWORD __3min = 1000 * 60 * 3;
	WaitForSingleObject(g_hStop, __3min);
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);


	return 0;
}


int _tmain_download_server(int argc, _TCHAR* argv[])
{
	D_SETTINGS(9, FALSE, TEXT("WSAServer-"), TEXT(""));

	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CComPtr<IWSASocketServerModule> spIWSASocketServerModule;
	hr = CreateInstanceCWSASocketServerModule(TEXT("30001"), 1, 0, NULL, TRUE, &spIWSASocketServerModule, TRUE);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	g_SocketServerModule = spIWSASocketServerModule;

	//////////////////////////////////////////////////////////////////////////
	hr = spIWSASocketServerModule->RunServer();
	//////////////////////////////////////////////////////////////////////////
	DWORD __3min = 1000 * 60 * 3;
	WaitForSingleObject(g_hStop, __3min);
	END_BLOCK(0);
	
	return hr;
}


