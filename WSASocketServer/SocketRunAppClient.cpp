#include "stdafx.h"

#include "SocketRunAppClient.h"
#include "ParameterRPC.h"



CRunAppClient::CRunAppClient( ISocketClient * pISocketClient, LONG MaxBinaryDataLen ) :
	m_RefCount(0),
	m_pISocketClient(pISocketClient),
	m_SendBuffer(MaxBinaryDataLen + 512 * 1024),
	m_RcvBuffer(MaxBinaryDataLen + 512 * 1024)
{

}

HRESULT CRunAppClient::CreateMember()
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	hr = m_SendBuffer.CreateMember();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SendBufferAllocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_RcvBuffer.CreateMember();
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("RcvBufferBufferAllocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	END_BLOCK(0);

	return hr;
}

HRESULT CRunAppClient::DestroyMember()
{
	m_SendBuffer.DestroyMember();
	m_RcvBuffer.DestroyMember();

	return S_OK;
}

HRESULT CRunAppClient::RunApp( LPCTSTR Username, LPCTSTR Domain, LPCTSTR Password, LPCTSTR CommandLine, LPCTSTR CurDir, ULONG TimeOut, BOOL TerminateOnTimeOut, ULONG & ExitCode)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ParameterStruct(RunApp) ParamsMethod;
	ParamsMethod.Username = Username;
	ParamsMethod.Domain = Domain;
	ParamsMethod.Password = Password;
	ParamsMethod.CommandLine = CommandLine;
	ParamsMethod.CurDir = CurDir;
	ParamsMethod.TimeOut = TimeOut;
	ParamsMethod.TerminateOnTimeOut = TerminateOnTimeOut;
	ParameterStruct(ResultRunApp) ParamsResult;

	//////////////////////////////////////////////////////////////////////////
	hr = ClientCallMethod(m_pISocketClient, ParamsMethod, 0, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	ExitCode = ParamsResult.ExitCode;

	END_BLOCK(0);

	return hr;
}

HRESULT CreateInstanceCRunAppClient( ISocketClient * pISocketClient, LONG MaxBinaryDataLen, IRunAppClient ** ppObj, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CRunAppClient,*/ 
		new CRunAppClient(
		pISocketClient, 
		MaxBinaryDataLen
		), 
		ppObj, 
		bCreateMember);
}
