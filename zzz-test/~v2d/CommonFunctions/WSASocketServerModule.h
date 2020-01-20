#pragma once

#include "stdafx.h"

#include <list>
using namespace std;

#include "WSASocketContext.h"

class CWSASocketServerModule
{
public:
	static CONST ULONG	 MAX_THREAD_COUNT = 32;

private:
	CString			m_Port;
	ULONG			m_WorkerThreadCount;
	HANDLE		m_hIOCP; //INTERNAL - TO cleanup
	SOCKET		m_ListenSocket; //INTERNAL - TO cleanup
	HANDLE		m_WorkerThreadHandles[MAX_THREAD_COUNT]; //INTERNAL - TO cleanup
	HANDLE		m_AcceptThreadHandle; //INTERNAL - TO cleanup
	
	list<TWSASocketContext*>	m_ListClients; //INTERNAL - TO cleanup
	CComCriticalSection				m_CS; //INTERNAL - TO cleanup

	IResponceHandlerFactory *				m_pResponceHandlerFactory;

public:
	CWSASocketServerModule(LPCTSTR pPort, ULONG ThreadCount, IResponceHandlerFactory * pResponceHandlerFactory);
	HRESULT CreateMember(BOOL bInet4);
	HRESULT DestroyMember();
	~CWSASocketServerModule();
	
	HRESULT Run();  //it is a blocking function, call EndServer() in another thread to end server
	HRESULT EndServer();

private:
	HRESULT CreateServerSocket(BOOL bInet4);

	HRESULT AcceptProc();
	static DWORD WINAPI AcceptThreadProc(LPVOID lpParameter);


	HRESULT WorkerProc();
	static DWORD WINAPI WorkerThreadProc(LPVOID lpParameter);
	HRESULT EndWorkerThreads();

	HRESULT AddClientSocketContext( TWSASocketContext * pSocketContext );
	HRESULT RemoveClientSocketContext(TWSASocketContext * pSocketContext);
};