#include "stdafx.h"
#include "SocketAddrHelper.h"

#pragma comment(lib, "Ws2_32.lib")


VOID CSocketAddrHelper::InitAddrInfo( PADDRINFOW pAiHints, INT ai_family /*= AF_UNSPEC*/, INT ai_socktype /*= SOCK_STREAM*/, INT ai_protocol /*= IPPROTO_TCP */ )
{
	memset(pAiHints, 0, sizeof(ADDRINFOW));
	pAiHints->ai_family = ai_family;
	pAiHints->ai_socktype = ai_socktype;
	pAiHints->ai_protocol = ai_protocol;
}

INT CSocketAddrHelper::MyGetAddrInfoW( __in PCWSTR pNodeName, __in PCWSTR pServiceName, __in const ADDRINFOW* pHints, __out PADDRINFOW* ppResult )
{
	INT nStatus = ::GetAddrInfoW(
		pNodeName,
		pServiceName,
		pHints,
		ppResult
		);

	return nStatus;
}

INT CSocketAddrHelper::MyGetNameInfoW( const ADDRINFOW* pAddrInfo, CString & strNode, CString & strService, INT Flags /*= NI_NUMERICSERV */ )
{
	TCHAR hostname[NI_MAXHOST] = {0};
	TCHAR servInfo[NI_MAXSERV] = {0};

	INT nStatus = ::GetNameInfoW(
		pAddrInfo->ai_addr,//__in   const SOCKADDR* pSockaddr,
		pAddrInfo->ai_addrlen,//__in   socklen_t SockaddrLength,
		hostname,//__out  PWCHAR pNodeBuffer,
		_countof(hostname),//__in   DWORD NodeBufferSize,
		servInfo,//__out  PWCHAR pServiceBuffer,
		_countof(servInfo),//__in   DWORD ServiceBufferSize,
		Flags//__in   INT Flags
		);
	if (0 == nStatus)
	{
		strNode = hostname;
		strService = servInfo;
	}

	return nStatus;
}

VOID CSocketAddrHelper::UT( LPCTSTR pszNode )
{
	ADDRINFOW AiHints;
	InitAddrInfo(
		&AiHints//PADDRINFOW pAiHints,
		//INT ai_family = AF_UNSPEC,
		//INT ai_socktype = SOCK_STREAM,
		//INT ai_protocol = IPPROTO_TCP
		);

	PADDRINFOW pResult = NULL;
	INT nStatus = MyGetAddrInfoW(
		pszNode,//__in   PCWSTR pNodeName,
		TEXT("27015"),//__in   PCWSTR pServiceName,
		&AiHints,//__in   const ADDRINFOW* pHints,
		&pResult//__out  PADDRINFOW* ppResult
		);
	if (nStatus)
	{
		INT nLatError = WSAGetLastError();
		_OLD_DAPIERR((0, DRNPOS, TEXT("MyGetAddrInfoW"), nLatError));
	}

	PADDRINFOW paddr = pResult;
	while (paddr)
	{
		CString strNode;
		CString strService;
		nStatus = MyGetNameInfoW(
			paddr,//const ADDRINFOW* pAddrInfo,
			strNode,//CString & strNode,
			strService,//CString & strService,
			NI_NUMERICSERV//INT Flags = NI_NUMERICSERV
			);
		if (nStatus)
		{
			INT nLatError = WSAGetLastError();
			_OLD_DAPIERR((0, DRNPOS, TEXT("MyGetNameInfoW"), nLatError));
		}

		CString strNodeNum;
		CString strServiceNum;
		nStatus = MyGetNameInfoW(
			paddr,//const ADDRINFOW* pAddrInfo,
			strNodeNum,//CString & strNode,
			strServiceNum,//CString & strService,
			NI_NUMERICHOST|NI_NUMERICSERV//INT Flags = NI_NUMERICSERV
			);
		if (nStatus)
		{
			INT nLatError = WSAGetLastError();
			_OLD_DAPIERR((0, DRNPOS, TEXT("MyGetNameInfoW"), nLatError));
		}

		paddr = paddr->ai_next;
	}

	FreeAddrInfoW(pResult);
	pResult =NULL;
}