// WSASocketEchoClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "InterfaceSocketClient.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicInterfaceSocketClient.h"
#include "..\PublicHeader\PublicFileDeviceInterface.h"
#include "..\PublicHeader\PublicSocketFileDeviceClient.h"
#include "..\PublicHeader\PublicInterfaceRunAppClient.h"
#include "..\PublicHeader\PublicStopTimer.h"

#include "SocketAddrNameHelper.h"

#define PrintLine(format, ...) _ftprintf_s(stdout, format L"\r\n", __VA_ARGS__)

VOID TestSocket(LPCTSTR pServer, LPCTSTR Port, LONG LoopCount)
{
	HRESULT hr = S_OK;

	LONG SendLen = 0;
	CHAR * pSendBuffer = NULL;

	LONG RcvLen = 0;
	CHAR * pRcvBuffer = NULL;

	CONST LONG __1M = 1024 * 1024;

	pSendBuffer = new CHAR[__1M + __1M];
	pRcvBuffer = new CHAR[__1M + __1M];


	std::fill(pSendBuffer, pSendBuffer + __1M + __1M, '*');
	std::fill(pRcvBuffer, pRcvBuffer + __1M + __1M, '*');


	CComPtr<ISocketClient> spISocketClient;
	CreateInstanceCSocketClient(pServer, Port, &spISocketClient, TRUE, TRUE, FALSE);
	hr = spISocketClient->CreateMember();

	srand((unsigned)time(NULL));

	for (LONG ii = 0; ii < LoopCount; ++ ii)
	{
		SendLen = __1M + rand()%__1M;
		pSendBuffer[0] = pSendBuffer[SendLen - 1] = rand()%25 + 'A';

		PrintLine(L"To Send: %u '%c...%c'", SendLen, pSendBuffer[0], pSendBuffer[SendLen - 1]);

		SendLen = htonl(SendLen);
		hr = spISocketClient->Write(sizeof(SendLen), NULL, (LPCBYTE)&SendLen);
		if (FAILED(hr))
		{
			break;
		}

		SendLen = ntohl(SendLen);
		hr = spISocketClient->Write(SendLen, NULL, (LPCBYTE)pSendBuffer);
		if (FAILED(hr))
		{
			break;
		}

		Sleep(100);

		hr = spISocketClient->Read(sizeof(RcvLen), NULL, (LPBYTE)&RcvLen);
		if (FAILED(hr))
		{
			break;
		}

		RcvLen = ntohl(RcvLen);
		hr = spISocketClient->Read(RcvLen, NULL, (LPBYTE)pRcvBuffer);
		if (FAILED(hr))
		{
			break;
		}

		PrintLine(L"Received: %u '%c...%c'", RcvLen, pRcvBuffer[0], pRcvBuffer[RcvLen-1]);
	}

	spISocketClient->Disconnect();

	delete[] pSendBuffer;
	delete[] pRcvBuffer;

}

static VOID FillBuffer(BYTE * pBuffer, LONG Size, BYTE ch)
{
	CONST LONG _4KB = 4 * 1024;

	LONG Written = 0;
	for (LONG ii = 0; ii < Size / _4KB; ++ ii)
	{
		if (0 == ii)
		{
			for (int jj = 0; jj < _4KB; ++ jj)
			{
				pBuffer[jj] = ch;
			}
			Written += _4KB;
		}
		else
		{
			CopyMemory(pBuffer + ii * _4KB, pBuffer + (ii - 1) * _4KB, _4KB);
			Written += _4KB;
		}
	}

	for (LONG ii = Written; ii < Size; ++ ii)
	{
		pBuffer[ii] = ch;
	}
}

HRESULT TestSocketFileDeviceClient(LPCTSTR pServer, LPCTSTR Port, LPCTSTR pFileName)
{
	HRESULT hr = S_OK;

	CComPtr<ISocketClient> spISocketClient;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CreateInstanceCSocketClient(pServer, Port, &spISocketClient, TRUE, TRUE, FALSE);
	hr = spISocketClient->CreateMember();
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	const LONG BUFFER_SZIE = 2 * 1024 * 1024;

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDeviceSocketClient> spIFileDeviceSocketClient;
	hr = CreateInstanceCSocketFileDeviceClient( 
		spISocketClient,//ISocketClient * pISocketClient, 
		BUFFER_SZIE,//LONG MaxBinaryDataLen, 
		//pFileName,//IN LPCTSTR strFileName, 
		//GENERIC_ALL,//IN ULONG ulDesiredAccess, 
		//0,//IN ULONG ulShareMode, 
		//CREATE_ALWAYS,//IN ULONG ulCreationDisposition, 
		//0,//IN ULONG ulFlagsAndAttributes,
		&spIFileDeviceSocketClient,//IFileDeviceWithIOBuffer ** ppObj, 
		TRUE//BOOL bCreateMember 
		);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	hr = spIFileDeviceSocketClient->ClientCreateInstanceFileDeviceWin32(
		pFileName,//LPCTSTR strFileName, 
		GENERIC_ALL,//ULONG ulDesiredAccess, 
		0,//ULONG ulShareMode, 
		CREATE_ALWAYS,//ULONG ulCreationDisposition, 
		0,//ULONG ulFlagsAndAttributes, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	//std::fill_n(spIFileDeviceSocketClient->GetSendBuffer(), BUFFER_SZIE, 'X');
	FillBuffer(spIFileDeviceSocketClient->GetSendBuffer(), BUFFER_SZIE, 'X');
	spIFileDeviceSocketClient->GetSendBuffer()[0] = 'a';
	spIFileDeviceSocketClient->GetSendBuffer()[BUFFER_SZIE - 1] = 'z';

	ULONG NumberOfBytesWritten = 0;
	hr = spIFileDeviceSocketClient->Write(BUFFER_SZIE, &NumberOfBytesWritten, spIFileDeviceSocketClient->GetSendBuffer());
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	LONGLONG NewFilePointer = 0;
	hr = spIFileDeviceSocketClient->SetPointerEx(0, &NewFilePointer, FILE_BEGIN);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	ULONG NumberOfBytesRead = 0;
	hr = spIFileDeviceSocketClient->Read(BUFFER_SZIE, &NumberOfBytesRead, spIFileDeviceSocketClient->GetReceiveBuffer());
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}


	hr = spIFileDeviceSocketClient->GetFileSize(&NewFilePointer);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}


	LONGLONG Offset = 1234;
	BYTE Buffer[] = "hello world";
	ULONG nNumberOfBytesToWrite = sizeof(Buffer);
	ULONG NumberOfBytesWrtten = 0;
	hr = spIFileDeviceSocketClient->WriteAtOffsetHelper(Offset, nNumberOfBytesToWrite, &NumberOfBytesWrtten, Buffer);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	NumberOfBytesRead = 0;
	BYTE Buffer2[256] = {0};
	hr = spIFileDeviceSocketClient->ReadAtOffsetHelper(
		Offset,//LONGLONG Offset, 
		NumberOfBytesWrtten,//ULONG nNumberOfBytesToRead, 
		&NumberOfBytesRead,//ULONG * pNumberOfBytesRead, 
		Buffer2//BYTE* pBuffer
		);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	spIFileDeviceSocketClient.Release();

	END_BLOCK(0);

	if (spISocketClient)
	{
		spISocketClient->Disconnect();
		spISocketClient.Release();
	}

	return hr;
}


HRESULT TestSocketRunApp(LPCTSTR pServer, LPCTSTR Port)
{
	HRESULT hr = S_OK;

	CComPtr<ISocketClient> spISocketClient;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CreateInstanceCSocketClient(pServer, Port, &spISocketClient, TRUE, TRUE, FALSE);
	hr = spISocketClient->CreateMember();
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	const LONG BUFFER_SZIE = 0;

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IRunAppClient> spIRunAppClient;
	hr = CreateInstanceCRunAppClient( 
		spISocketClient,//ISocketClient * pISocketClient, 
		BUFFER_SZIE,//LONG MaxBinaryDataLen,
		&spIRunAppClient,//IFileDeviceWithIOBuffer ** ppObj, 
		TRUE//BOOL bCreateMember 
		);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	ULONG ExitCode = -12345678;
	hr = spIRunAppClient->RunApp(
		TEXT("administrator"),//LPCTSTR Username, 
		TEXT("."),//LPCTSTR Domain, 
		TEXT("Master16516336"),//LPCTSTR Password, 
		TEXT("C:\\Windows\\notepad.exe"),//LPCTSTR CommandLine, 
		TEXT("C:\\Windows"),//LPCTSTR CurDir, 
		10 * 1000,//ULONG TimeOut, 
		TRUE,//BOOL TerminateOnTimeOut
		ExitCode);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}


	END_BLOCK(0);

	if (spISocketClient)
	{
		spISocketClient->Disconnect();
		spISocketClient.Release();
	}

	return hr;
}


HRESULT TestSocketFileDeviceClientPerformance(LPCTSTR pServer, LPCTSTR Port, LPCTSTR pFileName, LONG SingleWriteLen, LONG LoopCount)
{
	HRESULT hr = S_OK;

	CComPtr<ISocketClient> spISocketClient;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CreateInstanceCSocketClient(pServer, Port, &spISocketClient, TRUE, TRUE, FALSE);
	hr = spISocketClient->CreateMember();
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	const LONG BUFFER_SZIE = 2 * 1024 * 1024;

	//////////////////////////////////////////////////////////////////////////
	CComPtr<IFileDeviceSocketClient> spIFileDeviceSocketClient;
	hr = CreateInstanceCSocketFileDeviceClient( 
		spISocketClient,//ISocketClient * pISocketClient, 
		BUFFER_SZIE,//LONG MaxBinaryDataLen, 
		&spIFileDeviceSocketClient,//IFileDeviceWithIOBuffer ** ppObj, 
		TRUE//BOOL bCreateMember 
		);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	hr = spIFileDeviceSocketClient->ClientCreateInstanceFileDeviceWin32(
		pFileName,//LPCTSTR strFileName, 
		GENERIC_ALL,//ULONG ulDesiredAccess, 
		0,//ULONG ulShareMode, 
		CREATE_ALWAYS,//ULONG ulCreationDisposition, 
		0,//ULONG ulFlagsAndAttributes, 
		TRUE//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}

	if (SingleWriteLen > BUFFER_SZIE)
	{
		SingleWriteLen = BUFFER_SZIE;
	}
	BYTE * Buffer = spIFileDeviceSocketClient->GetSendBuffer();

	FillBuffer(Buffer, SingleWriteLen, 'X');
	Buffer[0] = 'a';
	Buffer[SingleWriteLen - 1] = 'z';

	LONGLONG TotalWritten = 0;
	ULONG NumberOfBytesWritten = 0;

	CStopTimer stopTimer(TRUE);
	stopTimer.Start();
	for (LONG ii = 0; ii < LoopCount; ++ ii)
	{
		NumberOfBytesWritten = 0;
		hr = spIFileDeviceSocketClient->Write(SingleWriteLen, &NumberOfBytesWritten, Buffer);
		if (FAILED(hr))
		{
			break;;
		}
		TotalWritten += NumberOfBytesWritten;
	}
	if (FAILED(hr))
	{
		LEAVE_BLOCK(0);
	}
	stopTimer.End();
	LONGLONG ElapesdMilSec = stopTimer.Timing();
	if (0 == ElapesdMilSec) { ElapesdMilSec = 1; }
	D_INFO(0, TEXT("TotalWritten=%I64d Bytes, ElapesdMilSec=%I64d MilSec, AverageSpeed=%I64d Bytes/Sec"), TotalWritten, ElapesdMilSec, TotalWritten * 1000 / ElapesdMilSec);

	spIFileDeviceSocketClient.Release();

	END_BLOCK(0);

	if (spISocketClient)
	{
		spISocketClient->Disconnect();
		spISocketClient.Release();
	}

	return hr;
}


int _tmain(int argc, _TCHAR* argv[])
{
	D_SETTINGS(9, FALSE, TEXT("client-"), NULL);
	WSADATA WsaDat = {0};
	WSAStartup(MAKEWORD(2, 2), &WsaDat);
	CString server = argc > 1? argv[1] : TEXT("");
	CString FileName = argc > 2 ? argv[2] : TEXT("");

	LONG SingleWriteLen = 4 * 1024;
	LONG LoopCount = 1024 * 512;

	//volatile int testCase =  3;
	//volatile int testCase = 5;
	volatile int testCase = 7;
	switch (testCase)
	{
	case 8:
	{
		//LPCTSTR pszServer = argv[1];
		//LPCTSTR pPort = argv[2];
		//CComPtr<ISocketClient> spObj;
		//HRESULT hr = CreateInstanceCSslSocketClient(
		//	argv[1],//LPCTSTR pszServer,
		//	pPort,//LPCTSTR pPort,
		//	&spObj,//ISocketClient ** ppObj,
		//	FALSE,//BOOL bSendAll,
		//	FALSE,//BOOL bReceiveAll,
		//	nullptr,//LPCTSTR pCertFile,
		//	nullptr,//LPCTSTR PrivateKeyFile,
		//	nullptr,//LPCTSTR pVerifyPeerCert,
		//	nullptr,//LPCTSTR pKeyFilePwd,
		//	TRUE//BOOL bCreateMember
		//	);
		//spObj->Disconnect();
	}
	break;
	case 0:
		TestSocketRunApp(server.GetString(), TEXT("30001"));
		break;
	case 1:
		TestSocketFileDeviceClient(server, TEXT("30001"), FileName);
		break;
	case 2:
		TestSocket(server, TEXT("30001"), 1024 * 1024);
		break;
	case 3:
		if (argc > 3)
		{
			SingleWriteLen = _tcstol(argv[3], NULL, 0);
		}
		if (argc > 4)
		{
			LoopCount = _tcstol(argv[4], NULL, 0);
		}
		TestSocketFileDeviceClientPerformance(server, TEXT("30001"), FileName, SingleWriteLen, LoopCount);
		break;
	case 4:
		{
			extern LONG PopTransferedBuffers(LPWSABUF & Buffers, LONG & BufferCount, LONG Transferred);
			WSABUF buf = {100, 0};
			LPWSABUF pBuffer = &buf;
			LONG BufferCount = 1;
			LONG transferred = 30;
			PopTransferedBuffers(pBuffer, BufferCount, transferred);
		}
		break;
	case 5:
	{
		LPCTSTR pHost = (argc > 1 ? argv[1] : TEXT("sonmi01-hv12"));
		vector<SockAddrName_NS::IPName> vecIpAndName;
		SockAddrName_NS::GetIPName(pHost, vecIpAndName);
	}
		break;
	case 6:
	{
		LPCTSTR name1 = argc > 1 ? argv[1] : TEXT("");
		LPCTSTR name2 = argc > 2 ? argv[2] : TEXT("");
		BOOL bIsSameMachine = SockAddrName_NS::IsSameMachine(name1, name2);
		bIsSameMachine = SockAddrName_NS::IsSameMachine(name1, name2);
	}
		break;
	case 7:
	{
		LPCTSTR name1 = argc > 1 ? argv[1] : TEXT("");
		BOOL bIsLocalMachine = SockAddrName_NS::IsLocalMachine(name1);
		bIsLocalMachine = SockAddrName_NS::IsLocalMachine(name1);
	}
		break;
	default:
		break;
	}
	
	WSACleanup();
	return 0;
}

