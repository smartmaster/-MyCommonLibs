#include "stdafx.h"

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "SocketAddrNameHelper.h"

static BOOL FindHost(LPCTSTR pHost, CONST vector<SockAddrName_NS::IPName> & vec2)
{
	BOOL bRet = FALSE;
	for (size_t ii = 0; ii < vec2.size(); ++ ii)
	{
		if (0 == _tcsicmp(pHost, vec2[ii].m_Name.c_str()))
		{
			bRet = TRUE;
			break;
		}

	}
	return bRet;
}

static BOOL CompareHosts(CONST vector<SockAddrName_NS::IPName> & vec1, CONST vector<SockAddrName_NS::IPName> & vec2)
{
	BOOL bRet = FALSE;
	for (size_t ii = 0; ii < vec1.size(); ++ ii)
	{
		if (FindHost(vec1[ii].m_Name.c_str(), vec2))
		{
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}

HRESULT SockAddrName_NS::GetIPName(LPCTSTR pNodeName, vector<SockAddrName_NS::IPName> & vecIPAndName, int ai_flags, int ai_family)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	PADDRINFOW pResult = NULL; /***TO CLEANUP***/

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ADDRINFOW Hints = { 0 };
	Hints.ai_flags = ai_flags;
	Hints.ai_family = ai_family;// AF_UNSPEC;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_TCP;

	int nRet = GetAddrInfoW(
		pNodeName,//_In_opt_  PCWSTR pNodeName,
		TEXT("0"),//_In_opt_  PCWSTR pServiceName,
		&Hints,//_In_opt_  const ADDRINFOW *pHints,
		&pResult//_Out_     PADDRINFOW *ppResult
		);
	if (nRet)
	{
		LastError = WSAGetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("GetAddrInfoW"));
		D_API_ERR(0, TEXT("GetAddrInfoW"), hr, TEXT("%s"), pNodeName);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	TCHAR TempBuffer[NI_MAXHOST] = { 0 };
	DWORD dwAddressStringLength = _countof(TempBuffer);
	for (PADDRINFOW pTemp = pResult; pTemp; pTemp = pTemp->ai_next)
	{
		SockAddrName_NS::IPName IPAndName;

		//////////////////////////////////////////////////////////////////////////
		TempBuffer[0] = 0;
		dwAddressStringLength = _countof(TempBuffer);
		nRet = WSAAddressToString(
			pTemp->ai_addr,//_In_      LPSOCKADDR lpsaAddress,
			pTemp->ai_addrlen,//_In_      DWORD dwAddressLength,
			NULL,//_In_opt_  LPWSAPROTOCOL_INFO lpProtocolInfo,
			TempBuffer,//_Inout_   LPTSTR lpszAddressString,
			&dwAddressStringLength//_Inout_   LPDWORD lpdwAddressStringLength
			);
		if (nRet)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT(""), ...);
			D_API_ERR(0, TEXT("WSAAddressToString"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
		else
		{
			IPAndName.m_IP = TempBuffer;
		}

		//////////////////////////////////////////////////////////////////////////
		TempBuffer[0] = 0;
		nRet = GetNameInfoW(
			pTemp->ai_addr,//const SOCKADDR *    pSockaddr,
			pTemp->ai_addrlen,//socklen_t           SockaddrLength,
			TempBuffer,//PWCHAR              pNodeBuffer,
			_countof(TempBuffer),//DWORD               NodeBufferSize,
			NULL,//PWCHAR              pServiceBuffer,
			0,//DWORD               ServiceBufferSize,
			0//INT                 Flags
			);
		if (nRet)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			//D_SET_FISRT_STATUS(hr, LastError);
			//D_SET_FISRT_ERROR_MESSAGE(TEXT(""), ...);
			D_API_ERR(0, TEXT("GetNameInfoW"), hr, TEXT(""));
			//LEAVE_BLOCK(0);
		}
		else
		{
			IPAndName.m_Name = TempBuffer;
		}

		//////////////////////////////////////////////////////////////////////////
		if (IPAndName.m_IP.size() && IPAndName.m_Name.size())
		{
			vecIPAndName.push_back(IPAndName);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	if (pResult)
	{
		FreeAddrInfoW(pResult);
		pResult = NULL;
	}

	return hr;
}

BOOL SockAddrName_NS::IsSameMachine(LPCTSTR pNodeName1, LPCTSTR pNodeName2)
{
	vector<SockAddrName_NS::IPName> vecIPAndName1;
	GetIPName(pNodeName1, vecIPAndName1);

	vector<SockAddrName_NS::IPName> vecIPAndName2;
	GetIPName(pNodeName2, vecIPAndName2);

	return CompareHosts(vecIPAndName1, vecIPAndName2);
}

BOOL SockAddrName_NS::IsLocalMachine(LPCTSTR pNodeName)
#if 1
{
	return IsSameMachine(NULL, pNodeName);
}
#else
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bRet = FALSE;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CHAR hostName[NI_MAXHOST] = { 0 };
	int nRet = gethostname(
		hostName,//_Out_  char *name,
		_countof(hostName)//_In_   int namelen
		);
	if (nRet)
	{
		LastError = WSAGetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT(""), ...);
		D_API_ERR(0, TEXT("gethostname"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	CString strHost(hostName);
	bRet = IsSameMachine(strHost.GetString(), pNodeName);
		
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return bRet;
}
#endif
