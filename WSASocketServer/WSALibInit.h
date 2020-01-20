#pragma once

#include <WinSock2.h>

class CWSALibInit
{
public:
	CWSALibInit(WORD Version = MAKEWORD(2, 2))
	{
		WSADATA WSAData = {0};
		WSAStartup(Version, &WSAData);
	}

	~CWSALibInit()
	{
		WSACleanup();
	}
};