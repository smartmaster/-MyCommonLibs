#include "stdafx.h"
#include "DataTransferPipe.h"
#include "DataTransferBuffer.h"

namespace /*NS_NONAME*/
{
	static CONST ULONG MAX_PIPE_LENGTH = 64*1024;

	static HRESULT PipeWriteOnce(HANDLE hPipe, LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent)
	{
		HRESULT hr = S_OK;
		DWORD dwLastError = 0;

		if (nToSend > MAX_PIPE_LENGTH) //<MyCode> 2011-4-16
		{
			nToSend = MAX_PIPE_LENGTH;
		}

		BOOL bRet = WriteFile( 
			hPipe,        // handle to pipe 
			pBuffer,     // buffer to write from 
			nToSend, // number of bytes to write 
			pnSent,   // number of bytes written 
			NULL);        // not overlapped I/O 
		if (!bRet)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("WriteFile"), hr, TEXT("ToSend=%u, Sent=%u"), nToSend, *pnSent));
			//break;
		}
		return hr;
	}

	static HRESULT PipeWrite(HANDLE hPipe, LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent)
	{
		HRESULT hr = S_OK;
		DWORD dwLastError = 0;

		ULONG xfered = 0;
		ULONG nCurrentWrite = 0;
		ULONG nCurrentToWrite = 0;

		while(nToSend)
		{
			if (nToSend > MAX_PIPE_LENGTH) //<MyCode> 2011-4-16
			{
				nCurrentToWrite = MAX_PIPE_LENGTH;
			}
			else
			{
				nCurrentToWrite = nToSend;
			}

			BOOL bRet = WriteFile( 
				hPipe,        // handle to pipe 
				(BYTE*)pBuffer + xfered,     // buffer to write from 
				nCurrentToWrite, // number of bytes to write 
				&nCurrentWrite,   // number of bytes written 
				NULL);        // not overlapped I/O 
			if (!bRet)
			{
				dwLastError = GetLastError();
				hr = HRESULT_FROM_WIN32(dwLastError);
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("WriteFile"), hr, TEXT("ToSend=%u, Sent=%u"), nToSend, *pnSent));
				break;
			}
			else
			{
				if (0 == nCurrentWrite)
				{
					hr = HRESULT_FROM_WIN32(ERROR_BAD_LENGTH);
					_OLD_DSETLASTSTATUS(hr);
					_OLD_DAPIERR((0, DRNPOS, TEXT("WriteFile"), hr, TEXT("CurrentWrite=%u"), nCurrentWrite));
					break;
				}
				xfered += nCurrentWrite;
				nToSend -= nCurrentWrite; //<MyCode> 2011-4-16
			}
		}

		if (SUCCEEDED(hr))
		{
			*pnSent = xfered;
		}

		return hr;
	}

	static HRESULT PipeReadOnce(HANDLE hPipe, LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived)
	{
		HRESULT hr = S_OK;
		DWORD dwLastError = 0;

		BOOL bRet = ReadFile( 
			hPipe,        // handle to pipe 
			pBuffer,    // buffer to receive data 
			nToReceive, // size of buffer 
			pnReceived, // number of bytes read 
			NULL);        // not overlapped I/O
		if (!bRet)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFile"), hr, TEXT("ToReceive=%u, Received=%u"), nToReceive, *pnReceived));
			//break;
		}
		return hr;
	}

	static HRESULT PipeRead(HANDLE hPipe, LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived)
	{
		HRESULT hr = S_OK;
		DWORD dwLastError = 0;

		BOOL bRet = FALSE;
		ULONG xfered = 0;
		ULONG CurrentRead = 0;

		while (nToReceive)
		{ 
			bRet = ReadFile( 
				hPipe,    // pipe handle 
				(BYTE*)pBuffer + xfered,    // buffer to receive reply 
				nToReceive,  // size of buffer 
				&CurrentRead,  // number of bytes read 
				NULL);    // not overlapped 
			if (!bRet)
			{
				dwLastError = GetLastError();
				if (ERROR_MORE_DATA != dwLastError)
				{
					dwLastError = GetLastError();
					hr = HRESULT_FROM_WIN32(dwLastError);
					_OLD_DSETLASTSTATUS(hr);
					_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFile"), hr, TEXT("ToReceive=%u, CurrentRead=%u"), nToReceive, CurrentRead));
					break;
				}
			}

			xfered += CurrentRead;
			nToReceive -= CurrentRead;

		} // repeat loop if ERROR_MORE_DATA 

		if (nToReceive == 0)
		{
			*pnReceived = xfered;
		}

		return hr;
	}
}

NS_DATATRANSFER::CPipeServer::CPipeServer( LPCTSTR pszPipeName, ULONG nOutBufferSize, ULONG nInBufferSize,  BOOL bMessageMode) :
m_strPipeName(pszPipeName),
m_nOutBufferSize(nOutBufferSize),
m_nInBufferSize(nInBufferSize),
m_hPipe(INVALID_HANDLE_VALUE),
m_bMessageMode(bMessageMode)
{
}

HRESULT NS_DATATRANSFER::CPipeServer::DestroyMember()
{
	if (INVALID_HANDLE_VALUE != m_hPipe )
	{
		CloseHandle(m_hPipe);
		m_hPipe = INVALID_HANDLE_VALUE;
	}
	return S_OK;
}

NS_DATATRANSFER::CPipeServer::~CPipeServer()
{
	DestroyMember();
}

HRESULT NS_DATATRANSFER::CPipeServer::Release()
{
	delete this;
	return S_OK;
}

HRESULT NS_DATATRANSFER::CPipeServer::Create()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	CString strFullPipeName;
	strFullPipeName.Format(TEXT("\\\\.\\pipe\\%s"), m_strPipeName.GetString());
	
	DWORD dwPipeMode = 0;
	if (m_bMessageMode)
	{
		dwPipeMode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT;
	}
	else
	{
		dwPipeMode = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;
	}

	CONST ULONG INSTANCE_COUNT = 1;

	m_hPipe = CreateNamedPipe( 
		strFullPipeName.GetString(),             // pipe name 
		PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,       // read/write access 
		dwPipeMode/*PIPE_TYPE_MESSAGE |   PIPE_READMODE_MESSAGE |   PIPE_WAIT*/, // message type pipe // message-read mode  // blocking mode 
		INSTANCE_COUNT/*PIPE_UNLIMITED_INSTANCES*/, // max. instances  
		m_nOutBufferSize,                  // output buffer size 
		m_nInBufferSize,                  // input buffer size 
		0,                        // client time-out 
		NULL);                    // default security attribute 
	if (INVALID_HANDLE_VALUE == m_hPipe)
	{
		dwLastError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwLastError);
		_OLD_DSETLASTSTATUS(hr);
		_OLD_DAPIERR((0, DRNPOS, TEXT("CreateNamedPipe"), hr, TEXT("%s"), strFullPipeName.GetString()));
		//break;
	}
	return hr;
}

HRESULT NS_DATATRANSFER::CPipeServer::WaiForConnect()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bConnected = ConnectNamedPipe(m_hPipe, NULL);
	if (!bConnected)
	{
		dwLastError = GetLastError();
		if (ERROR_PIPE_CONNECTED != dwLastError)
		{
			//dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("ConnectNamedPipe"), hr));
			//break;
		}
	}
	return hr;
}

HRESULT NS_DATATRANSFER::CPipeServer::SendOnce( LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent )
{
	return PipeWriteOnce(m_hPipe, pBuffer, nToSend, pnSent);
}

HRESULT NS_DATATRANSFER::CPipeServer::Send( LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent )
{
	return PipeWrite(m_hPipe, pBuffer, nToSend, pnSent);
}

HRESULT NS_DATATRANSFER::CPipeServer::ReceiveOnce( LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived )
{
	return PipeReadOnce(m_hPipe, pBuffer, nToReceive, pnReceived);
}

HRESULT NS_DATATRANSFER::CPipeServer::Receive( LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived )
{
	return PipeRead(m_hPipe, pBuffer, nToReceive, pnReceived);
}

HRESULT NS_DATATRANSFER::CPipeServer::Disconnect()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bRet = FlushFileBuffers(m_hPipe); 
	if (!bRet)
	{
		dwLastError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwLastError);
		_OLD_DSETLASTSTATUS(hr);
		_OLD_DAPIERR((0, DRNPOS, TEXT("FlushFileBuffers"), hr));
		//break;
	}
	bRet = DisconnectNamedPipe(m_hPipe); 
	if (!bRet)
	{
		dwLastError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwLastError);
		_OLD_DSETLASTSTATUS(hr);
		_OLD_DAPIERR((0, DRNPOS, TEXT("FlushFileBuffers"), hr));
		//break;
	}
	return hr;
}

HRESULT NS_DATATRANSFER::CPipeServer::Close()
{
	DestroyMember();
	return S_OK;
}

NS_DATATRANSFER::CPipeClient::CPipeClient( LPCTSTR pszServer, LPCTSTR pszPipeName, BOOL bMessgaeMode ) : //<mycode> 2011-4-14
m_strServer(pszServer),
m_strPipeName(pszPipeName),
m_hPipe(INVALID_HANDLE_VALUE),
m_bMessageMode(bMessgaeMode) //<mycode> 2011-4-14
{

}

HRESULT NS_DATATRANSFER::CPipeClient::DestroyMember()
{
	if (INVALID_HANDLE_VALUE != m_hPipe )
	{
		CloseHandle(m_hPipe);
		m_hPipe = INVALID_HANDLE_VALUE;
	}
	return S_OK;
}

NS_DATATRANSFER::CPipeClient::~CPipeClient()
{
	DestroyMember();
}

HRESULT NS_DATATRANSFER::CPipeClient::Release()
{
	delete this;
	return S_OK;
}

HRESULT NS_DATATRANSFER::CPipeClient::Create()
{
	return S_OK;
}

HRESULT NS_DATATRANSFER::CPipeClient::ConnetToServer()
{
	HRESULT hr S_OK;
	DWORD dwLastError = 0;

	do 
	{
		//////////////////////////////////////////////////////////////////////////
		CString strFullPipeName;
		strFullPipeName.Format(TEXT("\\\\%s\\pipe\\%s"), m_strServer.GetString(), m_strPipeName.GetString());
		m_hPipe =  CreateFile( 
			strFullPipeName.GetString(),   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE, 
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 
		if (INVALID_HANDLE_VALUE == m_hPipe)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFile"), hr, TEXT("%s"), strFullPipeName.GetString()));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		DWORD dwMode = 0; 
		if (m_bMessageMode) //<mycode> 2011-4-14
		{
			dwMode = PIPE_READMODE_MESSAGE;
		}
		else
		{
			dwMode = PIPE_READMODE_BYTE;
		}
		BOOL bRet = SetNamedPipeHandleState( 
			m_hPipe,    // pipe handle 
			&dwMode,  // new pipe mode 
			NULL,     // don't set maximum bytes 
			NULL);    // don't set maximum time
		if (!bRet)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetNamedPipeHandleState"), hr));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
	} while (FALSE);

	return hr;
}

HRESULT NS_DATATRANSFER::CPipeClient::SendOnce( LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent )
{
	return PipeWriteOnce(m_hPipe, pBuffer, nToSend, pnSent);
}

HRESULT NS_DATATRANSFER::CPipeClient::Send( LPCVOID pBuffer, ULONG nToSend, ULONG * pnSent )
{
	return PipeWrite(m_hPipe, pBuffer, nToSend, pnSent);
}

HRESULT NS_DATATRANSFER::CPipeClient::ReceiveOnce( LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived )
{
	return PipeReadOnce(m_hPipe, pBuffer, nToReceive, pnReceived);
}

HRESULT NS_DATATRANSFER::CPipeClient::Receive( LPVOID pBuffer, ULONG nToReceive, ULONG * pnReceived )
{
	return PipeRead(m_hPipe, pBuffer, nToReceive, pnReceived);
}

HRESULT NS_DATATRANSFER::CPipeClient::Disconnect()
{
	return E_NOTIMPL;
}

HRESULT NS_DATATRANSFER::CPipeClient::Close()
{
	DestroyMember();
	return S_OK;
}


NS_DATATRANSFER::IServer * NS_DATATRANSFER::CreatePipeServerInterface(LPCTSTR pszPipeName, ULONG nOutBufferSize, ULONG nInBufferSize, BOOL bMessgaeMode)
{
	IServer * pServer = new CPipeServer(pszPipeName, nOutBufferSize, nInBufferSize, bMessgaeMode);
	return pServer;
}

NS_DATATRANSFER::IClient * NS_DATATRANSFER::CreatePipeClientInterface(LPCTSTR pszServer, LPCTSTR pszPipeName, BOOL bMessgaeMode)
{
	IClient * pClient = new CPipeClient(pszServer, pszPipeName, bMessgaeMode);
	return pClient;
}


INT ut_pipe_main(int argc, _TCHAR* argv[])
{

	CONST ULONG COUNT = 1024*1024*10;
	CONST ULONG PIPE_BUFFER_SIZE = 4*1024;
	CONST LPCTSTR PIPE_NAME = TEXT("hello#smart");

	//CONST ULONG DataCount = 2*1024*1024;

	if (1 == argc)
	{
		NS_DATATRANSFER::IServer * pServer = NS_DATATRANSFER::CreatePipeServerInterface(PIPE_NAME, PIPE_BUFFER_SIZE, PIPE_BUFFER_SIZE, FALSE);

		pServer->Create();
		pServer->WaiForConnect();
		
		//CAutoVectorPtr<INT> apBuffer(new INT[COUNT]);
		//for (INT ii=0; ii<5; ++ii)
		//{
		//	ULONG nReceived = 0;
		//	pServer->Receive(apBuffer.m_p, DataCount, &nReceived);
		//}

		CLengthBuffer LenBuffer;
		for (INT ii=0; ii<5; ++ii)
		{
			NS_DATATRANSFER::ReceiveLengthBuffer(pServer, LenBuffer);
		}

		

		pServer->Disconnect();
		pServer->Close();

		if (pServer)
		{
			pServer->Release();
			pServer = NULL;
		}
	}
	else
	{
		NS_DATATRANSFER::IClient * pClient = NS_DATATRANSFER::CreatePipeClientInterface(argv[1], PIPE_NAME, FALSE);

		pClient->Create();
		pClient->ConnetToServer();


		//CAutoVectorPtr<INT> apBuffer(new INT[COUNT]);
		//for (ULONG ii=0; ii<COUNT; ++ii)
		//{
		//	apBuffer.m_p[ii] = ii;
		//}

		//for (INT ii=0; ii<5; ++ii)
		//{
		//	ULONG nSent = 0;
		//	pClient->Send(apBuffer.m_p, DataCount, &nSent);
		//}

		CONST ULONG BLOCK = 1024*1024;
		CLengthBuffer LenBuffer;
		for (INT ii=0; ii<5; ++ii)
		{
			LenBuffer.Allocate(ii*BLOCK);
			if (LenBuffer.GetDataLength())
			{
				memset(LenBuffer.GetDataBuffer(), ii, LenBuffer.GetDataLength());
			}
			NS_DATATRANSFER::SendLengthBuffer(pClient, LenBuffer);
		}


		pClient->Disconnect();
		pClient->Close();

		if (pClient)
		{
			pClient->Release();
			pClient = NULL;
		}
	}
	return 0;
}