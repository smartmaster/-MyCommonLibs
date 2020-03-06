#include "stdafx.h"

#include "SocketRunAppHandler.h"
#include "InterfaceRunApp.h"
#include "ParameterRPC.h"



CRunAppHandler::CRunAppHandler() :
	m_RefCount(0)
{

}

HRESULT CRunAppHandler::CreateMember()
{
	return S_OK;
}

HRESULT CRunAppHandler::DestroyMember()
{
	return S_OK;
}

HRESULT CRunAppHandler::Process( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	TCHAR szMethodName[256] = {0};
	hr = ReadMethodName(InBuffer, szMethodName, _countof(szMethodName));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadMethodName"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	PROCESS_METHOD(hr, RunApp, szMethodName, InBuffer, OutBuffer)
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		D_API_ERR(0, TEXT("ProcessMethod"), hr, TEXT("Method %s cannot be recognizd"), szMethodName);
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

HRESULT CRunAppHandler::ProcessRunApp( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	ParameterStruct(ResultRunApp) ParamsResult;
	ParamsResult.hrStatus = S_OK;

	BEGIN_BLOCK(0);

	ParameterStruct(RunApp) ParamsMethod;


	hr = RPCReadParams(ParamsMethod, /*ParamsResult, */InBuffer);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ServerReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = RunApp(
		ParamsMethod.Username.GetString(), 
		ParamsMethod.Domain.GetString(), 
		ParamsMethod.Password.GetString(), 
		ParamsMethod.CommandLine.GetString(), 
		ParamsMethod.CurDir.GetString(), 
		ParamsMethod.TimeOut, 
		ParamsMethod.TerminateOnTimeOut,
		ParamsResult.ExitCode
		);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s@%s, %s, %s. hr=0x%08x"), 
			ParamsMethod.Username.GetString(), 
			ParamsMethod.Domain.GetString(), 
			ParamsMethod.CommandLine.GetString(), 
			ParamsMethod.CurDir.GetString(),  
			hr
			);
		D_API_ERR(0, TEXT("RunApp"), hr, TEXT("%s@%s, %s, %s"), 
			ParamsMethod.Username.GetString(), 
			ParamsMethod.Domain.GetString(), 
			ParamsMethod.CommandLine.GetString(), 
			ParamsMethod.CurDir.GetString()
			);
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);


	hr = RPCWriteParams(ParamsResult, 0, OutBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ServerWriteResult"), hr, TEXT(""));
	}

	return hr;
}

HRESULT CreateInstanceRunAppHandler( IDataBufferHandler ** ppResponceHandler, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CRunAppHandler,*/ new CRunAppHandler(), ppResponceHandler, bCreateMember);
}

HRESULT CreateInstanceCRunAppHandlerFactory( IDataBufferHandlerFactory ** ppResponceHandlerFactory, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CRunAppHandlerFactory,*/ new CRunAppHandlerFactory, ppResponceHandlerFactory, bCreateMember);
}

HRESULT CRunAppHandlerFactory::CreateMember()
{
	return S_OK;
}

HRESULT CRunAppHandlerFactory::DestroyMember()
{
	return S_OK;
}

HRESULT CRunAppHandlerFactory::CreateInstance( IDataBufferHandler ** ppResponceHandler )
{
	return CreateInstanceRunAppHandler(ppResponceHandler, TRUE);
}

CRunAppHandlerFactory::CRunAppHandlerFactory() :
	m_RefCount(0)
{

}
