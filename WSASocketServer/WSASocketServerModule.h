#pragma once

#include <list>
#include <vector>
using namespace std;

#include <atlstr.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "InterfaceWSASocketServerModule.h"
#include "WSAConnectedSocket.h"
#include "InterfaceWSADataBufferHandler.h"

class CWSASocketServerModule : public IWSASocketServerModule
{
private:
	static CONST LONG MAX_THREAD_COUNT = MAXIMUM_WAIT_OBJECTS;

private:
	ULONG m_RefCount; /***Internal***/

	CString		m_Port;

	HANDLE		m_hIOCP; //INTERNAL - TO cleanup

	vector<SOCKET>		m_ListenSockets; //INTERNAL - TO cleanup
	vector<HANDLE>		m_AcceptThreadHandles; //INTERNAL - TO cleanup

	LONG		m_WorkerThreadCount;
	HANDLE		m_WorkerThreadHandles[MAX_THREAD_COUNT]; //INTERNAL - TO cleanup

	list<WSAConnectedSocketT*>	m_ListClients; //INTERNAL - TO cleanup
	CComCriticalSection			m_CSClients; //INTERNAL - TO cleanup

	IDataBufferHandlerFactory *	m_pIDataBufferHandlerFactory; /***Incomming***/

	LONG m_BufferInitialSize; /***INCOMMING***/
	BOOL m_bUseThreadSocket; /***INCOMMING***/

public:
	CWSASocketServerModule(LPCTSTR pPort, LONG ThreadCount, LONG BufferInitialSize, IDataBufferHandlerFactory * pIDataBufferHandlerFactory, BOOL bUseThreadSocket);
	IMP_CLASS_DESTRUCTOR(CWSASocketServerModule);

private:
	HRESULT AddClientSocketContext( WSAConnectedSocketT * pSocketContext );
	HRESULT RemoveAndDestroyClientSocket(WSAConnectedSocketT * pSocketContext);

	HRESULT CreateListenSockets();

	HRESULT AcceptProc(LONG Index);
	static DWORD WINAPI AcceptThreadProc(LPVOID lpParameter);

	HRESULT WorkerProc();
	static DWORD WINAPI WorkerThreadProc(LPVOID lpParameter);
	HRESULT EndWorkerThreads();

	HRESULT OnConnectIocp(SOCKET ConnectedSocket);


public:
	//struct IWSASocketServerModule : public ISimpleUnknown
	//{
	//	//struct ISimpleUnknown : public IUnknown
	//	//{
	//	//	//MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
	//	//	//	IUnknown
	//	//	//{
	//	//	//public:
	//	//	//	BEGIN_INTERFACE
	//	//	//		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
	//	//	//		/* [in] */ REFIID riid,
	//	//	//		/* [iid_is][out] */ void **ppvObject) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE AddRef( void) = 0;

	//	//	//		virtual ULONG STDMETHODCALLTYPE Release( void) = 0;

	//	//	//	END_INTERFACE
	//	//	//};
	IMP_IUNKNOWN_METHODS(m_RefCount);

	virtual LPCTSTR WhoAmI();

	virtual HRESULT CreateMember();
	virtual HRESULT DestroyMember();
	//	//};

	virtual HRESULT RunServer();
	virtual HRESULT EndServer();
	//};

};