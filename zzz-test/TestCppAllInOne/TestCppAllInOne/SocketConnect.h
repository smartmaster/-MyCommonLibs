#pragma once


#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#include "SmlHook.h"
#include "CodeBlock.h"
#include "log.h"
#include "SmlHook.h"

//#include "HookedAPIList.h"

namespace SmartLib
{
    class CSocketConnect
    {
    public:
        static bool IsIPInCIDR(LPCTSTR ip, LPCTSTR ipMask, LPCTSTR ipCidr, LPCTSTR ipMaskCidr)
        {
            //ensure wsastarup has been called

            std::wstring ipstr;
            SOCKADDR_STORAGE ipaddr = { 0 };
            int len = sizeof(SOCKADDR_STORAGE);

            ipstr = ip;
            int ret = ::WSAStringToAddress(
                &ipstr[0],//_In_      LPTSTR AddressString,
                ipstr.find(L'.') != std::wstring::npos ? AF_INET : AF_INET6,//_In_      INT AddressFamily,
                nullptr,//_In_opt_  LPWSAPROTOCOL_INFO lpProtocolInfo,
                (LPSOCKADDR)&ipaddr,//_Out_     LPSOCKADDR lpAddress,
                &len//_Inout_   LPINT lpAddressLength
            );

            

            SOCKADDR_STORAGE ipMaskaddr = { 0 };
            len = sizeof(SOCKADDR_STORAGE);
            if (0 == ret)
            {
                ipstr = ipMask;
                ret = ::WSAStringToAddress(
                    &ipstr[0],//_In_      LPTSTR AddressString,
                    ipstr.find(L'.') != std::wstring::npos ? AF_INET : AF_INET6,//_In_      INT AddressFamily,
                    nullptr,//_In_opt_  LPWSAPROTOCOL_INFO lpProtocolInfo,
                    (LPSOCKADDR)&ipMaskaddr,//_Out_     LPSOCKADDR lpAddress,
                    &len//_Inout_   LPINT lpAddressLength
                );
            }


            SOCKADDR_STORAGE ipCidraddr = { 0 };
            len = sizeof(SOCKADDR_STORAGE);
            if (0 == ret)
            {
                ipstr = ipCidr;
                ret = ::WSAStringToAddress(
                    &ipstr[0],//_In_      LPTSTR AddressString,
                    ipstr.find(L'.') != std::wstring::npos ? AF_INET : AF_INET6,//_In_      INT AddressFamily,
                    nullptr,//_In_opt_  LPWSAPROTOCOL_INFO lpProtocolInfo,
                    (LPSOCKADDR)&ipCidraddr,//_Out_     LPSOCKADDR lpAddress,
                    &len//_Inout_   LPINT lpAddressLength
                );
            }


            SOCKADDR_STORAGE ipMaskCidraddr = { 0 };
            len = sizeof(SOCKADDR_STORAGE);
            if (0 == ret)
            {
                ipstr = ipMaskCidr;
                ret = ::WSAStringToAddress(
                    &ipstr[0],//_In_      LPTSTR AddressString,
                    ipstr.find(L'.') != std::wstring::npos ? AF_INET : AF_INET6,//_In_      INT AddressFamily,
                    nullptr,//_In_opt_  LPWSAPROTOCOL_INFO lpProtocolInfo,
                    (LPSOCKADDR)&ipMaskCidraddr,//_Out_     LPSOCKADDR lpAddress,
                    &len//_Inout_   LPINT lpAddressLength
                );
            }


            bool bb = false;
            if (0 == ret)
            {
                bb = IsIPInCIDR(&ipaddr, &ipMaskaddr, &ipCidraddr, &ipMaskCidraddr);
            }

            return bb;
        }

        static bool IsIPInCIDR(const SOCKADDR_STORAGE * ip, const SOCKADDR_STORAGE* ipMask, const SOCKADDR_STORAGE* ipCidr, const SOCKADDR_STORAGE* ipMaskCidr)
        {
            bool b1 = (AF_INET == ip->ss_family) && (AF_INET == ipMask->ss_family) && (AF_INET == ipCidr->ss_family) && (AF_INET == ipMaskCidr->ss_family);

            bool b2 = false;
            if (b1)
            {
                ULONG mask = (((const sockaddr_in*)ipMask)->sin_addr.S_un.S_addr) & (((const sockaddr_in*)ipMaskCidr)->sin_addr.S_un.S_addr);
                ULONG ip1 = ((((const sockaddr_in*)ip)->sin_addr.S_un.S_addr) & mask);
                ULONG ip2 = ((((const sockaddr_in*)ipCidr)->sin_addr.S_un.S_addr) & mask);
                b2 = (ip1 == ip2);
            }
            return b2;
        }

        static bool IsIpAddrEqual(LPCTSTR ip1, LPCTSTR ip2)
        {
            //ensure wsastarup has been called

            std::wstring ipstr;
            SOCKADDR_STORAGE ip1addr = { 0 };
            int len = sizeof(SOCKADDR_STORAGE);

            ipstr = ip1;
            int ret = ::WSAStringToAddress(
                &ipstr[0],//_In_      LPTSTR AddressString,
                ipstr.find(L'.') != std::wstring::npos ? AF_INET : AF_INET6,//_In_      INT AddressFamily,
                nullptr,//_In_opt_  LPWSAPROTOCOL_INFO lpProtocolInfo,
                (LPSOCKADDR)&ip1addr,//_Out_     LPSOCKADDR lpAddress,
                &len//_Inout_   LPINT lpAddressLength
            );



            SOCKADDR_STORAGE ip2addr = { 0 };
            len = sizeof(SOCKADDR_STORAGE);
            if (0 == ret)
            {
                ipstr = ip2;
                ret = ::WSAStringToAddress(
                    &ipstr[0],//_In_      LPTSTR AddressString,
                    ipstr.find(L'.') != std::wstring::npos ? AF_INET : AF_INET6,//_In_      INT AddressFamily,
                    nullptr,//_In_opt_  LPWSAPROTOCOL_INFO lpProtocolInfo,
                    (LPSOCKADDR)&ip2addr,//_Out_     LPSOCKADDR lpAddress,
                    &len//_Inout_   LPINT lpAddressLength
                );
            }


            bool bb = false;
            if (0 == ret)
            {
                bb = IsIpAddrEqual(&ip1addr, &ip2addr);
            }

            return bb;
        }

        static bool IsIpAddrEqual(const SOCKADDR_STORAGE* ip1, const SOCKADDR_STORAGE* ip2)
        {
            bool b1 = (ip1->ss_family == ip2->ss_family);
            bool b2 = false;
            if (b1)
            {
                if (AF_INET == ip1->ss_family)
                {
                    b2 = (((const sockaddr_in*)ip1)->sin_addr.S_un.S_addr) == (((const sockaddr_in*)ip2)->sin_addr.S_un.S_addr);
                }
                else if (AF_INET6 == ip1->ss_family)
                {
                    
                    b2 = (0 == memcmp(&(((const sockaddr_in6*)ip1)->sin6_addr), &(((const sockaddr_in6*)ip2)->sin6_addr), sizeof(IN6_ADDR)));
                }
            }
            return b2;
        }

        static void Case3()
        {
            bool bb = IsIpAddrEqual(L"[::1]:80", L"::1");
            bb = IsIpAddrEqual(L"::1", L"[::2]:1080");

            bb = IsIpAddrEqual(L"127.10.10.10", L"127.10.10.10");
            bb = IsIpAddrEqual(L"127.10.10.10", L"127.10.10.20");


            bb = IsIpAddrEqual(L"[::20]:80", L"127.10.10.20");

            bb = IsIpAddrEqual(L"127.10.10.30", L"[::30]:80");

            return;
        }

        static void Case2()
        {
            TCHAR ip[] = TEXT("10.20.30.40");
            TCHAR mask[] = TEXT("255.255.255.0");

            TCHAR ipcidr[] = TEXT("10.20.30.0");
            TCHAR cidrmask[] = TEXT("255.255.255.0");

            bool bb = IsIPInCIDR(ip, mask, ipcidr, cidrmask);
            return;
        }

        static void Case1()
        {
            TCHAR ip[] = TEXT("10.20.30.40");
            TCHAR mask[] = TEXT("255.255.255.0");

            TCHAR ipcidr[] = TEXT("100.101.0.0");
            TCHAR cidrmask[] = TEXT("255.255.0.0");

            bool bb = IsIPInCIDR(ip, mask, ipcidr, cidrmask);
            return;
        }

        static HRESULT Connect(LPCTSTR wserver, LPCTSTR wport)
        {
            HRESULT hr = S_OK;
            DWORD LastError = 0;

            PADDRINFOA pResult = nullptr; /***CLEANUP***/
            
            BEGIN_BLOCK(0);
            //////////////////////////////////////////////////////////////////////////
			auto instance = MyAPIHooks::GetInstance();
			HMODULE  mod = ::GetModuleHandle(L"ws2_32.dll");
			LPVOID proc = ::GetProcAddress(mod, "connect");
			//int boundary = sizeof(void*) == 8 ? 0xf : 0x8;
			int boundary = -1;
			SmlHook::Hook(proc, MyAPIHooks::Myconnect, boundary, (LPVOID*)& instance->pOriginalconnect);
			

            //////////////////////////////////////////////////////////////////////////
            WSAData wsaData{ 0 };
           int iwsa = WSAStartup(MAKEWORD(2, 2), &wsaData);
           if (0 != iwsa)
           {
               LastError = ::WSAGetLastError();
               hr = HRESULT_FROM_WIN32(LastError);
               LEAVE_BLOCK(0);
           }

           char server[256] = { 0 };
           char port[64] = { 0 };
           _snprintf_s(server, _countof(server), "%S", wserver);
           _snprintf_s(port, _countof(port), "%S", wport);
            ADDRINFOA hints{ 0 };
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            int error = getaddrinfo(
                server,//_In_opt_  PCSTR pNodeName,
                port,//_In_opt_  PCSTR pServiceName,
                &hints,//_In_opt_  const ADDRINFOA * pHints,
                &pResult//_Out_     PADDRINFOA * ppResult
            );


            for (PADDRINFOA iter = pResult; iter; iter = iter->ai_next )
            {
                TCHAR AddressString[128] = { 0 };
                DWORD addrStrlen = _countof(AddressString);
                WSAAddressToString(
                    iter->ai_addr,//_In_      LPSOCKADDR lpsaAddress,
                    iter->ai_addrlen,//_In_      DWORD dwAddressLength,
                    nullptr,//_In_opt_  LPWSAPROTOCOL_INFO lpProtocolInfo,
                    AddressString,//_Inout_   LPTSTR lpszAddressString,
                    &addrStrlen//,//_Inout_   LPDWORD lpdwAddressStringLength
                );
                SML_LOG_LINE(TEXT("Connect to %s"), AddressString);

                SOCKET sock = ::socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);

                //std::wstring ebableForThread;
                //SmartLib::ConvHelper::ConvToString(L"tid:", ::GetCurrentThreadId(), ebableForThread);
                //hookedAPIList->ConfigData().Set(ebableForThread.c_str(), L"1");
                error = ::connect(sock, iter->ai_addr, iter->ai_addrlen);
                //hookedAPIList->ConfigData().Set(ebableForThread.c_str(), L"0");
                //hookedAPIList->ConfigData().Set(ebableForThread.c_str(), nullptr);

                if (SOCKET_ERROR == error)
                {
                    LastError = ::WSAGetLastError();
                    hr = HRESULT_FROM_WIN32(LastError);
                }
                ::closesocket(sock);
                sock = INVALID_SOCKET;
            }

            
            //////////////////////////////////////////////////////////////////////////
            END_BLOCK(0);


            if (pResult)
            {
                ::freeaddrinfo(pResult);
                pResult = nullptr;
            }

            WSACleanup();
            
            return hr;
        }
    };
}
