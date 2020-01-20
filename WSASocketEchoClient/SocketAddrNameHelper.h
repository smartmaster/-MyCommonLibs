#pragma once

#include <vector>
#include <string>
using namespace std;


#include <winsock2.h>
#include <ws2tcpip.h>

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>



namespace SockAddrName_NS
{
	struct IPName
	{
		wstring m_IP;
		wstring m_Name;
	};

	HRESULT GetIPName(LPCTSTR pNodeName, vector<IPName> & vecIPAndName, int ai_flags = 0, int ai_family = AF_UNSPEC);
	BOOL IsSameMachine(LPCTSTR pNodeName1, LPCTSTR pNodeName2);
	BOOL IsLocalMachine(LPCTSTR pNodeName);
}; //end namespace SockAddrName_NS