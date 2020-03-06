#include "stdafx.h"
#include "SocketFileDeviceClient.h"

#include "ParameterStreamRW.h"
#include "ParameterStreamHelper.h"
#include "ParameterRPC.h"


CSocketFileDeviceClient::CSocketFileDeviceClient( ISocketClient * pISocketClient, LONG MaxBinaryDataLen ) :
	m_RefCount(0),
	m_pISocketClient(pISocketClient),
	m_RemoteObjectCreated(FALSE),
	m_SendBuffer(MaxBinaryDataLen + 512 * 1024),
	m_RcvBuffer(MaxBinaryDataLen + 512 * 1024)

{

}

HRESULT CSocketFileDeviceClient::CreateMember()
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

HRESULT CSocketFileDeviceClient::DestroyMember()
{
	if (m_RemoteObjectCreated)
	{
		ClientRelease();
		m_RemoteObjectCreated = FALSE;
	}

	m_SendBuffer.DestroyMember();
	m_RcvBuffer.DestroyMember();

	return S_OK;
}

HRESULT CSocketFileDeviceClient::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ParameterStruct(SetPointerEx) ParamsMethod;
	ParamsMethod.llDistanceToMove = llDistanceToMove;
	ParamsMethod.ulMoveMethod = ulMoveMethod;

	ParameterStruct(ResultSetPointerEx) ParamsResult;
	
	hr = ClientCallMethod(m_pISocketClient, ParamsMethod, 0, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (lpNewFilePointer)
	{
		*lpNewFilePointer = ParamsResult.NewFilePointer;
	}

	END_BLOCK(0);

	return hr;
}

HRESULT CSocketFileDeviceClient::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ParameterStruct(Read) ParamsMethod;
	ParamsMethod.nNumberOfBytesToRead = nNumberOfBytesToRead;
	ParameterStruct(ResultRead) ParamsResult;


	hr = ClientCallMethod(m_pISocketClient, ParamsMethod, 0, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//ATLASSERT(pBuffer == m_RcvBuffer.m_pData);
	if (pBuffer != m_RcvBuffer.m_pData)
	{
		CopyMemory(pBuffer, m_RcvBuffer.m_pData, ParamsResult.NumberOfBytesRead);
	}


	if (lpNumberOfBytesRead)
	{
		*lpNumberOfBytesRead = ParamsResult.NumberOfBytesRead;
	}

	END_BLOCK(0);

	return hr;
}

HRESULT CSocketFileDeviceClient::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//ATLASSERT(pBuffer == m_SendBuffer.m_pData);
	if (pBuffer != m_SendBuffer.m_pData)
	{
		CopyMemory(m_SendBuffer.m_pData, pBuffer, nNumberOfBytesToWrite);
	}
	ParameterStruct(Write) ParamsMethod;
	ParamsMethod.nNumberOfBytesToWrite = nNumberOfBytesToWrite;

	ParameterStruct(ResultWrite) ParamsResult;


	hr = ClientCallMethod(m_pISocketClient, ParamsMethod, ParamsMethod.nNumberOfBytesToWrite, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (lpNumberOfBytesWritten)
	{
		*lpNumberOfBytesWritten = ParamsResult.NumberOfBytesWritten;
	}
	END_BLOCK(0);

	return hr;
}

HRESULT CSocketFileDeviceClient::GetFileSize( LONGLONG * pFileSize )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ParameterStruct(GetFileSize) ParamsMethod;
	ParameterStruct(ResultGetFileSize) ParamsResult;
	
	hr = ClientCallMethod(m_pISocketClient, ParamsMethod, 0, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (pFileSize)
	{
		*pFileSize = ParamsResult.FileSize;
	}

	END_BLOCK(0);

	return hr;
}

HRESULT CSocketFileDeviceClient::ClientCreateInstanceFileDeviceWin32( IN LPCTSTR strFileName, IN ULONG ulDesiredAccess, IN ULONG ulShareMode, IN ULONG ulCreationDisposition, IN ULONG ulFlagsAndAttributes, BOOL bCreateMember )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ParameterStruct(CreateInstanceFileDeviceWin32) ParamsMethod;
	ParamsMethod.strFileName = strFileName;
	ParamsMethod.ulDesiredAccess = ulDesiredAccess;
	ParamsMethod.ulShareMode = ulShareMode;
	ParamsMethod.ulCreationDisposition = ulCreationDisposition;
	ParamsMethod.ulFlagsAndAttributes = ulFlagsAndAttributes;
	ParamsMethod.bCreateMember = bCreateMember;

	ParameterStruct(Result) ParamsResult;

	hr = ClientCallMethod(m_pISocketClient, ParamsMethod, 0, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	if (SUCCEEDED(hr))
	{
		m_RemoteObjectCreated = TRUE;
	}

	return hr;
}

ULONG CSocketFileDeviceClient::ClientRelease()
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ParameterStruct(Release) ParamsMethod;
	ParameterStruct(Result) ParamsResult;
	
	hr = ClientCallMethod(m_pISocketClient, ParamsMethod, 0, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

BYTE * CSocketFileDeviceClient::GetSendBuffer()
{
	return m_SendBuffer.m_pData;
}

BYTE * CSocketFileDeviceClient::GetReceiveBuffer()
{
	return m_RcvBuffer.m_pData;
}

HRESULT CSocketFileDeviceClient::ReadAtOffsetHelper( LONGLONG Offset, ULONG nNumberOfBytesToRead, ULONG * pNumberOfBytesRead, BYTE* pBuffer )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	ParameterStruct(ReadAtOffsetHelper) ParamsMethod;
	ParamsMethod.Offset = Offset;
	ParamsMethod.nNumberOfBytesToRead = nNumberOfBytesToRead;
	ParameterStruct(ResultRead) ParamsResult;


	hr = ClientCallMethod(m_pISocketClient, ParamsMethod, 0, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//ATLASSERT(pBuffer == m_RcvBuffer.m_pData);
	if (pBuffer != m_RcvBuffer.m_pData)
	{
		CopyMemory(pBuffer, m_RcvBuffer.m_pData, ParamsResult.NumberOfBytesRead);
	}


	if (pNumberOfBytesRead)
	{
		*pNumberOfBytesRead = ParamsResult.NumberOfBytesRead;
	}

	END_BLOCK(0);

	return hr;
}

HRESULT CSocketFileDeviceClient::WriteAtOffsetHelper( LONGLONG Offset, ULONG nNumberOfBytesToWrite, ULONG * pNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	//ATLASSERT(pBuffer == m_SendBuffer.m_pData);
	if (pBuffer != m_SendBuffer.m_pData)
	{
		CopyMemory(m_SendBuffer.m_pData, pBuffer, nNumberOfBytesToWrite);
	}
	ParameterStruct(WriteAtOffsetHelper) ParamsMethod;
	ParamsMethod.Offset = Offset;
	ParamsMethod.nNumberOfBytesToWrite = nNumberOfBytesToWrite;

	ParameterStruct(ResultWrite) ParamsResult;


	hr = ClientCallMethod(m_pISocketClient, ParamsMethod, ParamsMethod.nNumberOfBytesToWrite, ParamsResult, m_SendBuffer, m_RcvBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ClientCallMethod"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (pNumberOfBytesWritten)
	{
		*pNumberOfBytesWritten = ParamsResult.NumberOfBytesWritten;
	}
	END_BLOCK(0);

	return hr;
}

HANDLE CSocketFileDeviceClient::GetRawHandle()
{
	return m_pISocketClient->GetRawHandle();
}

HRESULT CreateInstanceCSocketFileDeviceClient( ISocketClient * pISocketClient, 
											  LONG MaxBinaryDataLen, 
											  IFileDeviceSocketClient ** ppObj, 
											  BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CSocketFileDeviceClient,*/ 
		new CSocketFileDeviceClient(
		pISocketClient, 
		MaxBinaryDataLen
		), 
		ppObj, 
		bCreateMember);
}

