#pragma once

#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>

struct CSocketAddrHelper
{
	static VOID InitAddrInfo(
		PADDRINFOW pAiHints,
		INT ai_family = AF_UNSPEC,
		INT ai_socktype = SOCK_STREAM,
		INT ai_protocol = IPPROTO_TCP
		);

	static INT MyGetAddrInfoW(
		__in   PCWSTR pNodeName,
		__in   PCWSTR pServiceName,
		__in   const ADDRINFOW* pHints,
		__out  PADDRINFOW* ppResult
		);

	static INT MyGetNameInfoW(
		const ADDRINFOW* pAddrInfo,
		CString & strNode,
		CString & strService,
		INT Flags = NI_NUMERICSERV
		);

	static VOID UT(LPCTSTR pszNode);
};