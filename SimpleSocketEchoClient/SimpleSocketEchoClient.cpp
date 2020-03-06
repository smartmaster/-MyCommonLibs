#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include <tchar.h>

#include <thread>
#include <cassert>
#include <string>
#include <iostream>


// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

void SendProc(SOCKET ConnectSocket, long loop_count)
{
	using namespace ::std;

	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	
	for (long ii = 0; loop_count < 0 || ii < loop_count; ++ii)
	{
		string str{ to_string(ii) + " something appenped" };

		long cur = 0;
		while (cur < str.size())
		{
			cur += send(ConnectSocket, str.c_str() + cur, str.size() - cur, 0);
			if (SOCKET_ERROR == cur)
			{
				LastError = ::WSAGetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				break;
			}
		}
		if (SOCKET_ERROR == cur)
		{
			break;
		}
	}
}


void ReceiveProc(SOCKET ConnectSocket, long loop_count)
{
	using namespace ::std;



	HRESULT hr = S_OK;
	DWORD LastError = 0;



	char buffer[256];
	for (long ii = 0; loop_count < 0 || ii < loop_count; ++ii)
	{
		string str{ to_string(ii) + " something appenped" };

		buffer[0] = 0;
		long cur = 0;
		while (cur < str.size())
		{
			cur += recv(ConnectSocket, buffer + cur, str.size() - cur, 0);
			if (SOCKET_ERROR == cur)
			{
				LastError = ::WSAGetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				break;
			}
		}
		if (SOCKET_ERROR == cur)
		{
			break;
		}

		buffer[cur] = 0;
		assert(0 == memcmp(buffer, str.c_str(), cur));

		cout << "received: " << buffer << endl;

	}
}

int wmain(int argc, TCHAR** argv)
{
	using namespace ::std;
	//----------------------
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup function failed with error: %d\n", iResult);
		return 1;
	}
	//----------------------
	// Create a SOCKET for connecting to server
	SOCKET ConnectSocket;
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		wprintf(L"socket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(54321);

	//----------------------
	// Connect to server.
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"connect function failed with error: %ld\n", WSAGetLastError());
		iResult = closesocket(ConnectSocket);
		if (iResult == SOCKET_ERROR)
			wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	
	//return hr;

	long loop_count = 1000;
	if (argc > 1)
	{
		loop_count = _tcstol(argv[1], nullptr, 10);
	}

	

	thread threadSend{ [ConnectSocket, loop_count]()
	{
		SendProc(ConnectSocket, loop_count);
	}};
	thread threadReceive{ [ConnectSocket, loop_count]()
	{
		ReceiveProc(ConnectSocket, loop_count);
	} };

	threadSend.join();
	threadReceive.join();



	iResult = closesocket(ConnectSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	WSACleanup();
	return 0;
}



