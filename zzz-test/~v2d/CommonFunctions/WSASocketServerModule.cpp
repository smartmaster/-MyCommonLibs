#include "stdafx.h"
#include "WSASocketServerModule.h"


CWSASocketServerModule::CWSASocketServerModule( LPCTSTR pPort, ULONG ThreadCount , IResponceHandlerFactory * pResponceHandlerFactory) :
m_Port(pPort),
m_WorkerThreadCount(ThreadCount),
m_hIOCP(NULL),
m_ListenSocket(INVALID_SOCKET),
m_pResponceHandlerFactory(pResponceHandlerFactory),
m_AcceptThreadHandle(NULL)
{
	//////////////////////////////////////////////////////////////////////////
	if (0 == ThreadCount)
	{
		SYSTEM_INFO systemInfo = {0};
		GetSystemInfo(&systemInfo);
		m_WorkerThreadCount = systemInfo.dwNumberOfProcessors * 2;
	}
	if (m_WorkerThreadCount > MAX_THREAD_COUNT)
	{
		m_WorkerThreadCount = MAX_THREAD_COUNT;
	}

	//////////////////////////////////////////////////////////////////////////
	ZeroMemory(m_WorkerThreadHandles, sizeof(m_WorkerThreadHandles));
}

HRESULT CWSASocketServerModule::CreateMember(BOOL bInet4)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		hr = m_CS.Init();
		if (FAILED(hr))
		{
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("CS::Init"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		m_hIOCP = CreateIoCompletionPort(
			INVALID_HANDLE_VALUE,//__in      HANDLE FileHandle,
			NULL,//__in_opt  HANDLE ExistingCompletionPort,
			0,//__in      ULONG_PTR CompletionKey,
			0//__in      DWORD NumberOfConcurrentThreads
			);
		if (NULL == m_hIOCP)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("CreateIoCompletionPort"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		hr = CreateServerSocket(bInet4);
		if (FAILED(hr))
		{
			DAPIERR((0, DRNPOS, TEXT("CreateServerSocket"), hr));
			break;
		}
		
		//////////////////////////////////////////////////////////////////////////
		for (ULONG ii = 0; ii < m_WorkerThreadCount; ++ii)
		{
			DWORD ThreadId = 0;
			m_WorkerThreadHandles[ii] = CreateThread(
				NULL,//__in_opt   LPSECURITY_ATTRIBUTES lpThreadAttributes,
				0,//__in       SIZE_T dwStackSize,
				WorkerThreadProc,//__in       LPTHREAD_START_ROUTINE lpStartAddress,
				(LPVOID)this,//__in_opt   LPVOID lpParameter,
				0,//__in       DWORD dwCreationFlags,
				&ThreadId//__out_opt  LPDWORD lpThreadId
				);
			if (NULL == m_WorkerThreadHandles[ii])
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				DSETLASTSTATUS(hr);
				DAPIERR((0, DRNPOS, TEXT("CreateThread"), hr));
				break;
			}
		}
		if (FAILED(hr))
		{
			break;
		}
		
		//////////////////////////////////////////////////////////////////////////
		DWORD AcceptThreadId = 0;
		m_AcceptThreadHandle = CreateThread(
			NULL,//__in_opt   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//__in       SIZE_T dwStackSize,
			AcceptThreadProc,//__in       LPTHREAD_START_ROUTINE lpStartAddress,
			(LPVOID)this,//__in_opt   LPVOID lpParameter,
			0,//__in       DWORD dwCreationFlags,
			&AcceptThreadId//__out_opt  LPDWORD lpThreadId
			);
		if (NULL == m_AcceptThreadHandle)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("CreateThread"), hr));
			break;
		}
		
		//////////////////////////////////////////////////////////////////////////
	} while (FALSE);

	return hr;
}

HRESULT CWSASocketServerModule::CreateServerSocket( BOOL bInet4 )
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	INT Ret = 0;
	addrinfo Hints = {0};
	addrinfo * LocalAddr = NULL;

	Hints.ai_flags  = AI_PASSIVE;
	bInet4? Hints.ai_family = AF_INET : Hints.ai_family = AF_INET6;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_IP;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		CStringA PortA(m_Port);
		Ret = getaddrinfo(NULL, PortA.GetString(), &Hints, &LocalAddr);
		if (0 != Ret)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("getaddrinfo"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		m_ListenSocket = WSASocket(LocalAddr->ai_family, LocalAddr->ai_socktype, LocalAddr->ai_protocol, 
			NULL, 0, WSA_FLAG_OVERLAPPED); 
		if (INVALID_SOCKET == m_ListenSocket)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("WSASocket"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		Ret = bind(m_ListenSocket, LocalAddr->ai_addr, (INT)LocalAddr->ai_addrlen);
		if (SOCKET_ERROR == Ret)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("bind"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		Ret = listen(m_ListenSocket, SOMAXCONN);
		if (SOCKET_ERROR == Ret)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("listen"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
	} while (FALSE);

	//////////////////////////////////////////////////////////////////////////
	if (LocalAddr)
	{
		freeaddrinfo(LocalAddr);
		LocalAddr = NULL;
	}

	return hr;
}

HRESULT CWSASocketServerModule::WorkerProc()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bSuccess = FALSE;
	DWORD dwIoSize = 0;
	TWSASocketContext * pSocketContextKey = NULL;
	LPOVERLAPPED pOverlapped = NULL;

	for (;;)
	{
		//////////////////////////////////////////////////////////////////////////
		dwIoSize = 0;
		pSocketContextKey = NULL;
		pOverlapped = NULL;

		bSuccess = GetQueuedCompletionStatus(
			m_hIOCP, &dwIoSize,
			(PDWORD_PTR)&pSocketContextKey,
			(LPOVERLAPPED *)&pOverlapped, INFINITE);
		if (!bSuccess)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("GetQueuedCompletionStatus"), hr));

			if (ERROR_NETNAME_DELETED == LastError)
			{
				RemoveClientSocketContext(pSocketContextKey);
				continue;
			}
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		if (NULL == pSocketContextKey) // NULL CompletionKey posted
		{
			DINFO((0, DRNPOS, TEXT("NULL CompletionKey was posted, quit thread [%u]"), GetCurrentThreadId()));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		TWSASocketContext * pSocketContext = CONTAINING_RECORD(pOverlapped, TWSASocketContext, m_Overlapped);
		ATLASSERT(pSocketContext == pSocketContextKey);
		BOOL bCloseSocketContext = FALSE;

		do 
		{
			//////////////////////////////////////////////////////////////////////////
			if (dwIoSize == 0)
			{
				bCloseSocketContext = TRUE;
				DAPIERR((0, DRNPOS, TEXT("Size zero, socket closed"), hr));
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			hr = pSocketContext->OnIOCompleted(dwIoSize);
			if (FAILED(hr))
			{
				bCloseSocketContext = TRUE;
				DAPIERR((0, DRNPOS, TEXT("SocketContext::ProcessIO"), hr));
				break;
			}

			//////////////////////////////////////////////////////////////////////////
		} while (FALSE);

		if (bCloseSocketContext)
		{
			RemoveClientSocketContext(pSocketContext);
		}
		
		//////////////////////////////////////////////////////////////////////////
	}

	return hr;
}

DWORD WINAPI CWSASocketServerModule::WorkerThreadProc( LPVOID lpParameter )
{
	CWSASocketServerModule * pServerModule = (CWSASocketServerModule *)(lpParameter);
	return pServerModule->WorkerProc();
}

HRESULT CWSASocketServerModule::DestroyMember()
{
	//////////////////////////////////////////////////////////////////////////
	if (NULL != m_hIOCP)
	{
		CloseHandle(m_hIOCP);
		m_hIOCP = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	if (INVALID_SOCKET != m_ListenSocket)
	{
		closesocket(m_ListenSocket);
		m_ListenSocket = INVALID_SOCKET;
	}

	//////////////////////////////////////////////////////////////////////////
	for (INT ii = 0; ii < _countof(m_WorkerThreadHandles); ++ii)
	{
		if (m_WorkerThreadHandles[ii])
		{
			CloseHandle(m_WorkerThreadHandles[ii]);
			m_WorkerThreadHandles[ii] = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (m_AcceptThreadHandle)
	{
		CloseHandle(m_AcceptThreadHandle);
		m_AcceptThreadHandle = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	for (list<TWSASocketContext*>::iterator iter = m_ListClients.begin(); iter != m_ListClients.end(); ++iter)
	{
		if (*iter)
		{
			(**iter).DestroyMember();
			delete (*iter);
			*iter = NULL;
		}
	}
	m_ListClients.clear();

	//////////////////////////////////////////////////////////////////////////
	m_CS.Term();
	
	//////////////////////////////////////////////////////////////////////////
	return S_OK;
}

CWSASocketServerModule::~CWSASocketServerModule()
{
	DestroyMember();
}

HRESULT CWSASocketServerModule::RemoveClientSocketContext( TWSASocketContext * pSocketContext )
{
	CComCritSecLock<CComCriticalSection>  Lock(m_CS, FALSE);
	HRESULT hr = Lock.Lock();
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr);
		DAPIERR((0, DRNPOS, TEXT("Lock"), hr));
	}
	else
	{
		m_ListClients.remove(pSocketContext);

		//<mycode> 2012-1-19
		//SOCKADDR_STORAGE SockAddr = {0};
		//INT AddrLen = sizeof(SockAddr);
		//int RetGPN = getpeername(
		//	pSocketContext->m_ConnectedSocket,//__in     SOCKET s,
		//	(SOCKADDR*)&SockAddr,//__out    struct sockaddr *name,
		//	&AddrLen//__inout  int *namelen
		//	);
		//if (SOCKET_ERROR == RetGPN)
		//{
		//	DWORD LastError = WSAGetLastError();
		//	hr = HRESULT_FROM_WIN32(LastError);
		//	DAPIERR((0, DRNPOS, TEXT("getpeername"), hr));
		//}

		pSocketContext->DestroyMember();
		delete pSocketContext;
	}

	return hr;
}

HRESULT CWSASocketServerModule::AddClientSocketContext( TWSASocketContext * pSocketContext )
{
	CComCritSecLock<CComCriticalSection>  Lock(m_CS, FALSE);
	HRESULT hr = Lock.Lock();
	if (FAILED(hr))
	{
		DSETLASTSTATUS(hr);
		DAPIERR((0, DRNPOS, TEXT("Lock"), hr));
	}
	else
	{
		m_ListClients.push_back(pSocketContext);
	}

	return hr;
}

HRESULT CWSASocketServerModule::AcceptProc()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	SOCKET ConnectedSocket = INVALID_SOCKET;

	for (;;)
	{
		//////////////////////////////////////////////////////////////////////////
		SOCKADDR_STORAGE SockAddrStor = {0};
		INT AddrLen = sizeof(SockAddrStor);
		ConnectedSocket =  WSAAccept(
			m_ListenSocket,//__in     SOCKET s,
			(LPSOCKADDR)&SockAddrStor,//__out    struct sockaddr *addr,
			&AddrLen,//__inout  LPINT addrlen,
			NULL,//__in     LPCONDITIONPROC lpfnCondition,
			0//__in     DWORD dwCallbackData
			); 
		if (INVALID_SOCKET == ConnectedSocket)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("WSAAccept"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		TCHAR AddressString[128] = {0};
		DWORD AddressStringLength = _countof(AddressString);
		WSAAddressToString(
			(LPSOCKADDR)&SockAddrStor,//__in      LPSOCKADDR lpsaAddress,
			AddrLen,//__in      DWORD dwAddressLength,
			NULL,//__in_opt  LPWSAPROTOCOL_INFO lpProtocolInfo,
			AddressString,//__inout   LPTSTR lpszAddressString,
			&AddressStringLength//__inout   LPDWORD lpdwAddressStringLength
			);
		DINFO((0, DRNPOS, TEXT("Client [%s] connected"), AddressString));

		//////////////////////////////////////////////////////////////////////////
		IResponceHandler * pResponceHandler = NULL;
		hr = m_pResponceHandlerFactory->CreateInstance(&pResponceHandler);
		if (FAILED(hr))
		{
			DAPIERR((0, DRNPOS, TEXT("ResponceHandlerFactory::CreateInstance"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		CONST ULONG AllocatedSize = 2 * 1024 * 1024;
		TWSASocketContext * pSocketContext = new TWSASocketContext(ConnectedSocket, AllocatedSize, pResponceHandler);
		hr = pSocketContext->CreateMemer();
		if (FAILED(hr))
		{
			DAPIERR((0, DRNPOS, TEXT("CreateMemer"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		hr = AddClientSocketContext(pSocketContext);
		if (FAILED(hr))
		{
			DAPIERR((0, DRNPOS, TEXT("AddClientSocketContext"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		HANDLE hIocp = CreateIoCompletionPort((HANDLE)ConnectedSocket, m_hIOCP, (ULONG_PTR)pSocketContext, 0);
		if (NULL == hIocp)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("CreateIoCompletionPort"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		WSABUF WsaBuffer = {0};
		WsaBuffer.len = sizeof(pSocketContext->m_RecvBuffer.m_DataLength);
		WsaBuffer.buf = (CHAR*)&(pSocketContext->m_RecvBuffer.m_DataLength);

		ULONG NumberOfBytesXfered = 0;
		ULONG SockIoFlags = 0;
		INT RetCode = WSARecv(
			ConnectedSocket,//__in     SOCKET s,
			&WsaBuffer,//__inout  LPWSABUF lpBuffers,
			1,//__in     DWORD dwBufferCount,
			&NumberOfBytesXfered,//__out    LPDWORD lpNumberOfBytesRecvd,
			&SockIoFlags,//__inout  LPDWORD lpFlags,
			&pSocketContext->m_Overlapped,//__in     LPWSAOVERLAPPED lpOverlapped,
			NULL//__in     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);
		if (SOCKET_ERROR == RetCode)
		{
			LastError = WSAGetLastError();
			if (WSA_IO_PENDING != LastError)
			{
				hr = HRESULT_FROM_WIN32(LastError);
				DSETLASTSTATUS(hr);
				DAPIERR((0, DRNPOS, TEXT("WSARecv"), hr));
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
	}

	return hr;
}

DWORD WINAPI CWSASocketServerModule::AcceptThreadProc( LPVOID lpParameter )
{
	CWSASocketServerModule * pServerModule = (CWSASocketServerModule *)lpParameter;
	return pServerModule->AcceptProc();
}

HRESULT CWSASocketServerModule::EndServer()
{
	if (m_ListenSocket != INVALID_SOCKET)
	{
		closesocket(m_ListenSocket);
		m_ListenSocket = INVALID_SOCKET;
	}

	return S_OK;
}

HRESULT CWSASocketServerModule::EndWorkerThreads()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL Ret = FALSE;

	for (ULONG ii = 0; ii < m_WorkerThreadCount; ++ii)
	{
		Ret = PostQueuedCompletionStatus(
			m_hIOCP,//__in      HANDLE CompletionPort,
			0,//__in      DWORD dwNumberOfBytesTransferred,
			0,//__in      ULONG_PTR dwCompletionKey,
			NULL//__in_opt  LPOVERLAPPED lpOverlapped
			);
		if (!Ret)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("PostQueuedCompletionStatus"), hr));
			break;
		}
	}

	return hr;
}

HRESULT CWSASocketServerModule::Run()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	DWORD dwWait = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		dwWait = WaitForSingleObject(
			m_AcceptThreadHandle,//__in  HANDLE hHandle,
			INFINITE//__in  DWORD dwMilliseconds
			);
		if (WAIT_FAILED == dwWait)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("WaitForSingleObject"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		hr = EndWorkerThreads();
		if (FAILED(hr))
		{
			DAPIERR((0, DRNPOS, TEXT("EndWorkerThreads"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		CONST DWORD dwMilliseconds = 15*60*1000; 
		dwWait = WaitForMultipleObjects(
			m_WorkerThreadCount,//__in  DWORD nCount,
			m_WorkerThreadHandles,//__in  const HANDLE *lpHandles,
			TRUE,//__in  BOOL bWaitAll,
			dwMilliseconds//INFINITE//__in  DWORD dwMilliseconds
			);
		if (WAIT_FAILED == dwWait)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			DSETLASTSTATUS(hr);
			DAPIERR((0, DRNPOS, TEXT("WaitForMultipleObjects"), hr));
			break;
		}

	} while (FALSE);

	return hr;
}
