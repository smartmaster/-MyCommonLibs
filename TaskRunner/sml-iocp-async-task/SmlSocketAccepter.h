#pragma once

#include <Ws2tcpip.h>
#include "SmlSocketAcceptTask.h"
#include "SmlIFile.h"

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "SmlTaskRunnner.h"


namespace SmartLib
{
	using namespace std;


	class SocketAccepter
	{
	private:
		CString _serviceName;
		RefPtr<TaskRunner> _taskRunner;


	public:
		SocketAccepter(LPCTSTR serviceName, RefPtr<TaskRunner> tr) :
			_serviceName{serviceName},
			_taskRunner{move(tr)}
		{
		}

		HRESULT Create()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			ADDRINFOW hints{ 0 };
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			PADDRINFOW pResult = nullptr;  /***CLEANUP***/
			ScopeResource<> sr_pResult{ [&pResult]()
			{
				if (pResult)
				{
					::FreeAddrInfoW(pResult);
					pResult = nullptr;
				}
			
			} };
			int gai = ::GetAddrInfoW(
				nullptr,//_In_opt_  PCWSTR pNodeName,
				_serviceName.GetString(),//_In_opt_  PCWSTR pServiceName,
				&hints,//_In_opt_  const ADDRINFOW * pHints,
				&pResult//,//_Out_     PADDRINFOW * ppResult
			);

			while (pResult)
			{
				//////////////////////////////////////////////////////////////////////////
				SOCKET sock = ::WSASocket(
					pResult->ai_family,//_In_  int af,
					pResult->ai_socktype,//_In_  int type,
					pResult->ai_protocol,//_In_  int protocol,
					nullptr,//_In_  LPWSAPROTOCOL_INFO lpProtocolInfo,
					0,//_In_  GROUP g,
					WSA_FLAG_OVERLAPPED//,//_In_  DWORD dwFlags
				);
				if (INVALID_SOCKET == sock)
				{
					LastError = ::WSAGetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("WSASocket"), LastError, hr);
					break;
				}


				//////////////////////////////////////////////////////////////////////////
				RefPtr<IFile*> spsock = RefPtr<IFile*>::Make(new SocketFile{ sock });
				spsock.SetDispose(Disposer::DeletePtr<IFile>);

				//////////////////////////////////////////////////////////////////////////
				int nb = ::bind(
					sock,//_In_  SOCKET s,
					pResult->ai_addr,//_In_  const struct sockaddr* name,
					pResult->ai_addrlen//,//_In_  int namelen
				);
				if (SOCKET_ERROR == nb)
				{
					LastError = ::WSAGetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("bind"), LastError, hr);
					break;
				}

				//////////////////////////////////////////////////////////////////////////
				int nl = ::listen(
					sock,//_In_  SOCKET s,
					SOMAXCONN//,//_In_  int backlog
				);
				if (SOCKET_ERROR == nl)
				{
					LastError = ::WSAGetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("listen"), LastError, hr);
					break;
				}

				

				//////////////////////////////////////////////////////////////////////////
				RefPtr<IAsyncTask*> aat = RefPtr<IAsyncTask*>::Make(new SocketAcceptAsyncTask{pResult, spsock, _taskRunner});
				aat.SetDispose(Disposer::DeletePtr<IAsyncTask>);
				_taskRunner->AddRunning(aat);
				//IAsyncTask::RefSelf(aat);

				hr = (*aat)->Create();
				if (FAILED(hr))
				{
					SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("Create"), LastError, hr);
					break;;
				}
				
				pResult = pResult->ai_next;
			}


			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}


	public:
		static HRESULT Run(LPCTSTR serviceName, RefPtr<TaskRunner> tr)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			SocketAccepter accepter{serviceName, tr};

			hr = accepter.Create();
			if (FAILED(hr))
			{
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("accepter.Create"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

	};
}