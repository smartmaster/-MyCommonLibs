// SharedSocketProcess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicJobMainFlowbInterface.h"

#include "DuplicateSocket.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//BOOL bDebug = ((argc > 1) && (0 == _tcsicmp(TEXT("-debug"), argv[1])));
	//while (bDebug)
	//{
	//	Sleep(1000);
	//}

	D_SETTINGS(99, FALSE, NULL, NULL);
	D_SLEEP(TEXT(""), -1, -1);

	WSADATA WSAData = { 0 };
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	HRESULT hr = TargetProcessMain(ThreadProcServerMain);
	WSACleanup();
	return hr;
}

