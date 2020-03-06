#include "stdafx.h"
#include "WSASocketServerModule.h"

#include "..\PublicHeader\PublicDuplicateSocket.h"
#include "..\PublicHeader\PublicJobMainFlowbInterface.h"


CWSASocketServerModule::CWSASocketServerModule( 
	LPCTSTR pPort, 
	LONG ThreadCount, 
	LONG BufferInitialSize, 
	IDataBufferHandlerFactory * pIDataBufferHandlerFactory,
	BOOL bUseThreadSocket
	) :
	m_RefCount(0),
	m_Port(pPort),
	m_WorkerThreadCount(ThreadCount),
	m_BufferInitialSize(BufferInitialSize),
	m_hIOCP(NULL),
	m_pIDataBufferHandlerFactory(pIDataBufferHandlerFactory),
	m_bUseThreadSocket(bUseThreadSocket)
{
	//////////////////////////////////////////////////////////////////////////
	if (m_WorkerThreadCount < 0)
	{
		m_WorkerThreadCount = 1;
	}

	if (m_WorkerThreadCount == 0)
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


HRESULT CWSASocketServerModule::AddClientSocketContext( WSAConnectedSocketT * pSocketContext )
{
	CComCritSecLock<CComCriticalSection>  Lock(m_CSClients, FALSE);
	HRESULT hr = Lock.Lock();
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, hr);
		D_API_ERR(0, TEXT("Lock"), hr, TEXT(""));
	}
	else
	{
		m_ListClients.push_back(pSocketContext);
		Lock.Unlock();
	}

	return hr;
}

HRESULT CWSASocketServerModule::RemoveAndDestroyClientSocket( WSAConnectedSocketT * pSocketContext )
{
	CComCritSecLock<CComCriticalSection>  Lock(m_CSClients, FALSE);
	HRESULT hr = Lock.Lock();
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, hr);
		D_API_ERR(0, TEXT("Lock"), hr, TEXT(""));
	}
	else
	{
		m_ListClients.remove(pSocketContext);
		Lock.Unlock();

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
		//	D_API_ERR((0, DRNPOS, TEXT("getpeername"), hr));
		//}

		pSocketContext->DestroyMember();
		delete pSocketContext;
	}

	return hr;
}

HRESULT CWSASocketServerModule::CreateListenSockets()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	INT Ret = 0;
	addrinfo Hints = {0};
	addrinfo * ResultAddr = NULL;

	Hints.ai_flags  = AI_PASSIVE;
	Hints.ai_family = AF_UNSPEC;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_IP;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CStringA PortA(m_Port);
	Ret = getaddrinfo(NULL, PortA.GetString(), &Hints, &ResultAddr);
	if (0 != Ret)
	{
		LastError = WSAGetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("getaddrinfo"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	for (addrinfo * LocalAddr = ResultAddr; LocalAddr != NULL; LocalAddr = LocalAddr->ai_next)
	{
		//////////////////////////////////////////////////////////////////////////
		SOCKET ListenSocket = WSASocket(
			LocalAddr->ai_family, 
			LocalAddr->ai_socktype, 
			LocalAddr->ai_protocol, 
			NULL, 
			0, 
			WSA_FLAG_OVERLAPPED); 
		if (INVALID_SOCKET == ListenSocket)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("WSASocket"), hr, TEXT(""));
			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		Ret = bind(ListenSocket, LocalAddr->ai_addr, (INT)LocalAddr->ai_addrlen);
		if (SOCKET_ERROR == Ret)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("bind"), hr, TEXT(""));
			closesocket(ListenSocket);
			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		Ret = listen(ListenSocket, SOMAXCONN);
		if (SOCKET_ERROR == Ret)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("listen"), hr, TEXT(""));
			closesocket(ListenSocket);
			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		m_ListenSockets.push_back(ListenSocket);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	//////////////////////////////////////////////////////////////////////////
	if (ResultAddr)
	{
		freeaddrinfo(ResultAddr);
		ResultAddr = NULL;
	}

	return m_ListenSockets.size()? S_OK : hr;
}

HRESULT CWSASocketServerModule::AcceptProc( LONG Index )
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
			m_ListenSockets[Index],//__in     SOCKET s,
			(LPSOCKADDR)&SockAddrStor,//__out    struct sockaddr *addr,
			&AddrLen,//__inout  LPINT addrlen,
			NULL,//__in     LPCONDITIONPROC lpfnCondition,
			0//__in     DWORD dwCallbackData
			); 
		if (INVALID_SOCKET == ConnectedSocket)
		{
			LastError = WSAGetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("WSAAccept"), hr, TEXT(""));
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

		TCHAR NameString[256] = { 0 };
		TCHAR serviceString[64] = { 0 };
		GetNameInfoW(
			(LPSOCKADDR)&SockAddrStor,//const SOCKADDR *    pSockaddr,
			AddrLen,//socklen_t           SockaddrLength,
			NameString,//PWCHAR              pNodeBuffer,
			_countof(NameString),//DWORD               NodeBufferSize,
			serviceString,//PWCHAR              pServiceBuffer,
			_countof(serviceString),//DWORD               ServiceBufferSize,
			0//INT                 Flags
			);

		D_INFO(0, TEXT("Client IP:[%s] Name:[%s:%s] connected, ConnectedSocket=0x%p"), AddressString, NameString, serviceString, ConnectedSocket);

		//////////////////////////////////////////////////////////////////////////
		if (m_bUseThreadSocket)
		{
			CONST TCHAR SHAREDSOCKETPROCESS[] = TEXT("SharedSocketProcess.exe");
			//CONST TCHAR SHAREDSOCKETPROCESS_DEBUG[] = TEXT("SharedSocketProcess.exe -debug");
			//BOOL bDebug = TRUE;
			hr = DuplicateSocketToTargetProcess(/*bDebug ? SHAREDSOCKETPROCESS_DEBUG : */SHAREDSOCKETPROCESS, ConnectedSocket); //use a separate process to handle socket
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("DuplicateSocketToTargetProcess"), hr, TEXT("%s"), SHAREDSOCKETPROCESS);
				if (TRUE)
				{
					::closesocket(ConnectedSocket);
					ConnectedSocket = INVALID_SOCKET;
					hr = S_OK;
				}
				else
				{
					hr = CreateThreadServerMain(ConnectedSocket); //if failure, use a separate thread to handle socket instead
				}
				if (FAILED(hr))
				{
					D_API_ERR(0, TEXT("CreateThreadDownloadServer"), hr, TEXT(""));
					break;
				}
			}
		}
		else
		{
			hr = OnConnectIocp(ConnectedSocket);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("OnConnectIocp"), hr, TEXT(""));
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}

	return hr;
}

DWORD WINAPI CWSASocketServerModule::AcceptThreadProc( LPVOID lpParameter )
{
	pair<LONG, CWSASocketServerModule*> * pParam = (pair<LONG, CWSASocketServerModule*> *)(lpParameter);
	LONG Index = pParam->first;
	CWSASocketServerModule* pWSASocketServerModule = pParam->second;
	delete pParam; pParam = NULL;
	DWORD Ret = pWSASocketServerModule->AcceptProc(Index);
	return Ret;
}

HRESULT CWSASocketServerModule::WorkerProc()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL bSuccess = FALSE;
	DWORD dwIoSize = 0;
	WSAConnectedSocketT * pKey = NULL;
	LPWSAOVERLAPPED pOverlapped = NULL;

	for (;;)
	{
		//////////////////////////////////////////////////////////////////////////
		dwIoSize = 0;
		pKey = NULL;
		pOverlapped = NULL;

		bSuccess = GetQueuedCompletionStatus(
			m_hIOCP, 
			&dwIoSize,
			(PDWORD_PTR)&pKey,
			(LPOVERLAPPED *)&pOverlapped, 
			INFINITE);
		if (!bSuccess)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("GetQueuedCompletionStatus"), hr, TEXT(""));

			if (ERROR_NETNAME_DELETED == LastError)
			{
				RemoveAndDestroyClientSocket(pKey);
				continue;
			}
			else
			{
				break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		if (NULL == pKey) // NULL CompletionKey posted
		{
			D_INFO(0, TEXT("NULL CompletionKey was posted, quit thread [%u]"), GetCurrentThreadId());
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		WSAConnectedSocketT * pWSAConnectedSocketT = CONTAINING_RECORD(pOverlapped, WSAConnectedSocketT, m_Overlapped);
		ATLASSERT(pWSAConnectedSocketT == pKey);

		BOOL bCloseSocketContext = FALSE;

		BEGIN_BLOCK(0);
		//////////////////////////////////////////////////////////////////////////
		if (dwIoSize == 0)
		{
			bCloseSocketContext = TRUE;
			D_API_ERR(0, TEXT("Size zero, socket closed"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		hr = pWSAConnectedSocketT->OnIOCompleted(dwIoSize);
		if (FAILED(hr))
		{
			bCloseSocketContext = TRUE;
			D_API_ERR(0, TEXT("SocketContext::ProcessIO"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		END_BLOCK(0);

		if (bCloseSocketContext)
		{
			RemoveAndDestroyClientSocket(pWSAConnectedSocketT);
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

HRESULT CWSASocketServerModule::EndWorkerThreads()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BOOL Ret = FALSE;

	for (LONG ii = 0; ii < m_WorkerThreadCount; ++ii)
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
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("PostQueuedCompletionStatus"), hr, TEXT(""));
			//break;
		}
	}

	return hr;
}

LPCTSTR CWSASocketServerModule::WhoAmI()
{
	static CONST TCHAR WHO_AM_I[] = TEXT("CWSASocketServerModule:IWSASocketServerModule");
	return WHO_AM_I;
}

HRESULT CWSASocketServerModule::CreateMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = m_CSClients.Init();
	if (FAILED(hr))
	{
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CS::Init"), hr, TEXT(""));
		LEAVE_BLOCK(0);
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
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateIoCompletionPort"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = CreateListenSockets();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateServerSocket"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD AcceptThreadId = 0;
	for (ULONG ii = 0; ii < m_ListenSockets.size(); ++ ii)
	{
		HANDLE hThreadAccept = CreateThread(
			NULL,//__in_opt   LPSECURITY_ATTRIBUTES lpThreadAttributes,
			0,//__in       SIZE_T dwStackSize,
			AcceptThreadProc,//__in       LPTHREAD_START_ROUTINE lpStartAddress,
			(LPVOID)(new pair<LONG, CWSASocketServerModule*>(ii, this)),//__in_opt   LPVOID lpParameter,
			0,//__in       DWORD dwCreationFlags,
			&AcceptThreadId//__out_opt  LPDWORD lpThreadId
			);
		if (NULL == hThreadAccept)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("CreateThread"), hr, TEXT(""));
			break;
		}
		m_AcceptThreadHandles.push_back(hThreadAccept);
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	

	//////////////////////////////////////////////////////////////////////////
	for (LONG ii = 0; ii < m_WorkerThreadCount; ++ii)
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
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("CreateThread"), hr, TEXT(""));
			break;
		}
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
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
	for (ULONG ii = 0; ii < m_ListenSockets.size(); ++ ii)
	{
		if (INVALID_SOCKET != m_ListenSockets[ii])
		{
			closesocket(m_ListenSockets[ii]);
			m_ListenSockets[ii] = INVALID_SOCKET;
		}
	}
	m_ListenSockets.clear();

	//////////////////////////////////////////////////////////////////////////
	for (ULONG ii = 0; ii < m_AcceptThreadHandles.size(); ++ ii)
	{
		if (m_AcceptThreadHandles[ii])
		{
			CloseHandle(m_AcceptThreadHandles[ii]);
			m_AcceptThreadHandles[ii] = NULL;
		}
	}
	m_AcceptThreadHandles.clear();

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
	for (list<WSAConnectedSocketT*>::iterator iter = m_ListClients.begin(); iter != m_ListClients.end(); ++iter)
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
	m_CSClients.Term();

	//////////////////////////////////////////////////////////////////////////
	return S_OK;
}

HRESULT CWSASocketServerModule::RunServer()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	DWORD dwWait = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	DWORD Count = m_AcceptThreadHandles.size();
	dwWait = WaitForMultipleObjects(
		Count,
		Count? &(m_AcceptThreadHandles[0]) : NULL,//__in  HANDLE hHandle,
		TRUE,
		INFINITE//__in  DWORD dwMilliseconds
		);
	if (WAIT_FAILED == dwWait)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("WaitForSingleObject"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = EndWorkerThreads();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("EndWorkerThreads"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CONST DWORD dwMilliseconds = 15 * 60 * 1000; 
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
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("WaitForMultipleObjects"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT CWSASocketServerModule::EndServer()
{
	HRESULT hr = S_OK;
	for (ULONG ii = 0; ii < m_ListenSockets.size(); ++ ii)
	{
		closesocket(m_ListenSockets[ii]);
		m_ListenSockets[ii] = INVALID_SOCKET;
	}
	m_ListenSockets.clear();
	return hr;
}

HRESULT CWSASocketServerModule::OnConnectIocp(SOCKET ConnectedSocket)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	IDataBufferHandler * pIDataBufferHandler = NULL;
	hr = m_pIDataBufferHandlerFactory->CreateInstance(&pIDataBufferHandler);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ResponceHandlerFactory::CreateInstance"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//CONST LONG AllocatedSize = 2 * 1024 * 1024;
	WSAConnectedSocketT * pWSAConnectedSocketT = new WSAConnectedSocketT(ConnectedSocket, m_BufferInitialSize, pIDataBufferHandler);
	hr = pWSAConnectedSocketT->CreateMemer();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateMemer"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = AddClientSocketContext(pWSAConnectedSocketT);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("AddClientSocketContext"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE hIocp = CreateIoCompletionPort((HANDLE)ConnectedSocket, m_hIOCP, (ULONG_PTR)pWSAConnectedSocketT, 0);
	if (NULL == hIocp)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		D_SET_LAST_STATUS(hr, LastError);
		D_API_ERR(0, TEXT("CreateIoCompletionPort"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	WSABUF WsaBuffer = { 0 };
	WsaBuffer.len = sizeof(pWSAConnectedSocketT->m_RecvBuffer.m_DataLength);
	WsaBuffer.buf = (CHAR*)&(pWSAConnectedSocketT->m_RecvBuffer.m_DataLength);

	//ULONG NumberOfBytesXfered = 0;
	ULONG SockIoFlags = 0;
	INT RetCode = WSARecv(
		ConnectedSocket,//__in     SOCKET s,
		&WsaBuffer,//__inout  LPWSABUF lpBuffers,
		1,//__in     DWORD dwBufferCount,
		NULL,//&NumberOfBytesXfered,//__out    LPDWORD lpNumberOfBytesRecvd,
		&SockIoFlags,//__inout  LPDWORD lpFlags,
		&pWSAConnectedSocketT->m_Overlapped,//__in     LPWSAOVERLAPPED lpOverlapped,
		NULL//__in     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
		);
	if (SOCKET_ERROR == RetCode)
	{
		LastError = WSAGetLastError();
		if (WSA_IO_PENDING != LastError)
		{
			hr = HRESULT_FROM_WIN32(LastError);
			D_SET_LAST_STATUS(hr, LastError);
			D_API_ERR(0, TEXT("WSARecv"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


HRESULT CreateInstanceCWSASocketServerModule(LPCTSTR pPort, LONG ThreadCount, LONG BufferInitialSize, IDataBufferHandlerFactory * pIDataBufferHandlerFactory, BOOL bUseThreadSocket, IWSASocketServerModule ** ppServerModule, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(
		/*CWSASocketServerModule, */
		(new CWSASocketServerModule(pPort, ThreadCount, BufferInitialSize, pIDataBufferHandlerFactory, bUseThreadSocket)),
		ppServerModule, 
		bCreateMember);
	//HRESULT hr = S_OK;

	//BEGIN_BLOCK(0);
	////////////////////////////////////////////////////////////////////////////
	//CWSASocketServerModule * pObj = new CWSASocketServerModule( pPort, ThreadCount, pIDataBufferHandlerFactory );
	//if (NULL == pObj)
	//{
	//	hr = E_OUTOFMEMORY;
	//	D_SET_LAST_STATUS(hr, hr);
	//	D_API_ERR(0, TEXT("new"), hr, TEXT(""));
	//	LEAVE_BLOCK(0);
	//}

	////////////////////////////////////////////////////////////////////////////
	//pObj->AddRef();
	//if (bCreateMember)
	//{
	//	hr = pObj->CreateMember();
	//	if (FAILED(hr))
	//	{
	//		pObj->Release();
	//		hr = E_OUTOFMEMORY;
	//		D_SET_LAST_STATUS(hr, hr);
	//		D_API_ERR(0, TEXT("new"), hr, TEXT(""));
	//		LEAVE_BLOCK(0);
	//	}
	//}

	////////////////////////////////////////////////////////////////////////////
	//*ppServerModule = pObj;
	//END_BLOCK(0);

	//return hr;
}