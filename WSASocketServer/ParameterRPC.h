#pragma once

#include "..\PublicHeader\PublicInterfaceSocketClient.h"

#include "WSADataBuffer.h"


//<sonmi01>2013-7-4 ###???
//////////////////////////////////////////////////////////////////////////
LONG ReadParamOffset(IN CONST WSADataBufferT & Buffer);
VOID WriteParamOffset(WSADataBufferT & Buffer, LONG ParamOffset);
HRESULT ReadMethodName(CONST WSADataBufferT & Buffer, LPTSTR MethodName, LONG MethodNameBufferLen);

//////////////////////////////////////////////////////////////////////////
#define DEFINE_PROCESS_METHOD_WITHOUT_PARAM_HR(m_pObj, PtrOrDot, MethodWithoutParam) \
	virtual HRESULT Process##MethodWithoutParam( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	) \
{ \
	HRESULT hr = S_OK; \
	ParameterStruct(Result) ParamsResult; \
	ParamsResult.hrStatus = S_OK; \
	BEGIN_BLOCK(0); \
	ParameterStruct(MethodWithoutParam) ParamsMethod; \
	LONG ParamOffset = ReadParamOffset(InBuffer); \
	hr = ParamsMethod.ReadFromBuffer(InBuffer.m_pData + ParamOffset, InBuffer.m_DataLength - ParamOffset - sizeof(LONG)); \
	if (FAILED(hr)) \
	{ \
		ParamsResult.hrStatus = hr; \
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr); \
		D_API_ERR(0, TEXT("ReadFromBuffer"), hr, TEXT("")); \
		LEAVE_BLOCK(0); \
	} \
	hr = m_pObj PtrOrDot MethodWithoutParam(); \
	if (FAILED(hr)) \
	{ \
		ParamsResult.hrStatus = hr; \
		ParamsResult.strErrorMessage.Format( L#MethodWithoutParam L", hr=0x%08x",  hr); \
		D_API_ERR(0, L#MethodWithoutParam, hr, TEXT("")); \
		LEAVE_BLOCK(0); \
	} \
	END_BLOCK(0); \
	LONGLONG WrittenLength = 0; \
	hr = ParamsResult.WriteToBuffer(OutBuffer.m_pData, OutBuffer.m_AllocatedSize, WrittenLength); \
	OutBuffer.m_DataLength = (LONG)WrittenLength; \
	WriteParamOffset(OutBuffer, 0); \
	if (FAILED(hr)) \
	{ \
		D_API_ERR(0, TEXT("Result.WriteToBuffer"), hr, TEXT("")); \
	} \
	return hr; \
};

#define DEFINE_PROCESS_METHOD_WITHOUT_PARAM_VOID(m_pObj, PtrOrDot, MethodWithoutParam) \
	virtual HRESULT Process##MethodWithoutParam( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer 	) \
{ \
	HRESULT hr = S_OK; \
	ParameterStruct(Result) ParamsResult; \
	ParamsResult.hrStatus = S_OK; \
	BEGIN_BLOCK(0); \
	ParameterStruct(MethodWithoutParam) ParamsMethod; \
	LONG ParamOffset = ReadParamOffset(InBuffer); \
	hr = ParamsMethod.ReadFromBuffer(InBuffer.m_pData + ParamOffset, InBuffer.m_DataLength - ParamOffset - sizeof(LONG)); \
	if (FAILED(hr)) \
	{ \
		ParamsResult.hrStatus = hr; \
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr); \
		D_API_ERR(0, TEXT("ReadFromBuffer"), hr, TEXT("")); \
		LEAVE_BLOCK(0); \
	} \
	m_pObj PtrOrDot MethodWithoutParam(); \
	END_BLOCK(0); \
	LONGLONG WrittenLength = 0; \
	hr = ParamsResult.WriteToBuffer(OutBuffer.m_pData, OutBuffer.m_AllocatedSize, WrittenLength); \
	OutBuffer.m_DataLength = (LONG)WrittenLength; \
	WriteParamOffset(OutBuffer, 0); \
	if (FAILED(hr)) \
	{ \
		D_API_ERR(0, TEXT("Result.WriteToBuffer"), hr, TEXT("")); \
	} \
	return hr; \
};


//////////////////////////////////////////////////////////////////////////

#define PROCESS_METHOD(hr, MethodName, szMethodName, InBuffer, OutBuffer) \
if (0 == _tcsicmp(MethodString(MethodName), szMethodName)) \
{ \
	hr = Process##MethodName(InBuffer, OutBuffer); \
}

#define ELSE_PROCESS_METHOD(hr, MethodName, szMethodName, InBuffer, OutBuffer) else PROCESS_METHOD(hr, MethodName, szMethodName, InBuffer, OutBuffer)

//////////////////////////////////////////////////////////////////////////


template<typename PARAMS_T/*, typename PARAMS_RESULT*/>
HRESULT RPCReadParams(PARAMS_T & ParamsMethod, /*PARAMS_RESULT & ParamsResult,*/ CONST WSADataBufferT & InBuffer)
{
	LONG ParamOffset = ReadParamOffset(InBuffer);
	HRESULT hr = ParamsMethod.ReadFromBuffer(InBuffer.m_pData + ParamOffset, InBuffer.m_DataLength - ParamOffset - sizeof(LONG));
	if (FAILED(hr))
	{
		//ParamsResult.hrStatus = hr;
		//ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ReadFromBuffer"), hr, TEXT(""));
		//LEAVE_BLOCK(0);
	}

	return hr;
}

template<typename PARAMS_T>
HRESULT RPCWriteParams(PARAMS_T & ParamsResult, LONG ParamsOffset, WSADataBufferT & OutBuffer)
{
	LONGLONG WrittenLength = 0;
	HRESULT hr = ParamsResult.WriteToBuffer(OutBuffer.m_pData + ParamsOffset, OutBuffer.m_AllocatedSize - ParamsOffset, WrittenLength);
	OutBuffer.m_DataLength = ParamsOffset + (LONG)WrittenLength;
	WriteParamOffset(OutBuffer, ParamsOffset);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Result.WriteToBuffer"), hr, TEXT(""));
	}

	return hr;
};

template<typename PARAMS_METHOD, typename PARAMS_RESULT>
HRESULT ClientCallMethod(ISocketClient * pISocketClient, PARAMS_METHOD & ParamsMethod, LONG ParamsMethodOffset, PARAMS_RESULT & ParamsResult, WSADataBufferT & SendBuffer, WSADataBufferT & RcvBuffer)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = RPCWriteParams(ParamsMethod, ParamsMethodOffset, SendBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("RPCWriteResult"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = SendLengthAndBuffer(pISocketClient, SendBuffer.m_DataLength, SendBuffer.m_pData, NULL);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("SendLengthAndBuffer"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = ReceiveLengthAndBuffer(pISocketClient, RcvBuffer.m_DataLength, RcvBuffer.m_pData, NULL);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReceiveLengthAndBuffer"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = RPCReadParams(ParamsResult, RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("RPCReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	hr = ParamsResult.hrStatus;
	if (FAILED(hr))
	{
		D_INFO(0, TEXT("Result=[0x%08x, %s]"), ParamsResult.hrStatus, ParamsResult.strErrorMessage.GetString());
	}
	

	END_BLOCK(0);

	return hr;
}


#if 0 //sample begin
HRESULT xxx( ULONG xxx, ULONG* xxx, BYTE* xxx )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ParameterStruct(xxx) ParamsMethod;
	ParamsMethod.xxx = xxx;
	ParameterStruct(xxx) xxx;


	hr = ClientCallMethod(m_p xxx, ParamsMethod, 0, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//ATLASSERT(pBuffer == m_RcvBuffer.m_pData);
	if (xxx != m_RcvBuffer.m_pData)
	{
		CopyMemory(pBuffer, m_RcvBuffer.m_pData, ParamsResult.xxx);
	}


	if (xxx)
	{
		*xxx = ParamsResult.xxx;
	}

	END_BLOCK(0);

	return hr;
}

HRESULT xxxx( ULONG xxx, ULONG* xxx, CONST BYTE* xxx )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//ATLASSERT(pBuffer == m_SendBuffer.m_pData);
	if (xx != m_SendBuffer.m_pData)
	{
		CopyMemory(m_SendBuffer.m_pData, pBuffer, xxx);
	}
	ParameterStruct(xxx) ParamsMethod;
	ParamsMethod.xxx = xxx;

	ParameterStruct(xxx) ParamsResult;


	hr = ClientCallMethod(m_p xxx, ParamsMethod, ParamsMethod.xxx, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (xxx)
	{
		*xxx = ParamsResult.xxx;
	}
	END_BLOCK(0);

	return hr;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

HRESULT Process xxx( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	ParameterStruct(xxx) ParamsResult;
	ParamsResult.hrStatus = S_OK;

	BEGIN_BLOCK(0);

	ParameterStruct(xxx) ParamsMethod;


	hr = RPCReadParams(ParamsMethod, /*ParamsResult, */InBuffer);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ServerReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_sp xxx->xxx(
		ParamsMethod.xxx,//ULONG nNumberOfBytesToRead, 
		&ParamsResult.xxx,//ULONG* lpNumberOfBytesRead, 
		OutBuffer.xxx//BYTE* pBuffer
		);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("Read, hr=0x%08x"),  hr);
		D_API_ERR(0, TEXT("Read"), hr, TEXT("NumberOfBytesToRead=%u, NumberOfBytesRead=%u"), ParamsMethod.xxx, ParamsResult.xxx);
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);


	hr = RPCWriteParams(ParamsResult, ParamsResult.xxx, OutBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ServerWriteResult"), hr, TEXT(""));
	}

	return hr;
}

HRESULT Process xxx( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	ParameterStruct(xxx) ParamsResult;
	ParamsResult.hrStatus = S_OK;

	BEGIN_BLOCK(0);

	ParameterStruct(xxx) ParamsMethod;


	hr = RPCReadParams(ParamsMethod, /*ParamsResult, */InBuffer);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ServerReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_sp XXX->xxx(
		ParamsMethod.xxx,//ULONG nNumberOfBytesToWrite, 
		&ParamsResult.xxx,//ULONG* lpNumberOfBytesWritten, 
		InBuffer.xxx//CONST BYTE* pBuffer
		);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("Read, hr=0x%08x"),  hr);
		D_API_ERR(0, TEXT("Read"), hr, TEXT("NumberOfBytesToWrite=%u, NumberOfBytesWrite=%u"), ParamsMethod.xxx, ParamsResult.xx);
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	hr = RPCWriteParams(ParamsResult, xxxx, OutBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ServerWriteResult"), hr, TEXT(""));
	}

	return hr;
}
#endif //sample end
