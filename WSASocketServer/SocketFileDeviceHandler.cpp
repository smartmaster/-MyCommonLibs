#include "stdafx.h"

#include "SocketFileDeviceHandler.h"



HRESULT CSocketFileDeviceHandler::ProcessCreateInstanceFileDeviceWin32( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	ParameterStruct(Result) ParamsResult;
	ParamsResult.hrStatus = S_OK;

	BEGIN_BLOCK(0);

	ParameterStruct(CreateInstanceFileDeviceWin32) ParamsMethod;


	hr = RPCReadParams(ParamsMethod, /*ParamsResult, */InBuffer);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ServerReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (!!m_spIFileDevice)
	{
		hr = HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS);
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("The file already exists %s, hr=0x%08x"),  ParamsMethod.strFileName.GetString(), hr);
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("The file already exists %s"),  ParamsMethod.strFileName.GetString());
		LEAVE_BLOCK(0);
	}

	hr = CreateInstanceFileDeviceWin32(
		ParamsMethod.strFileName.GetString(),//IN LPCTSTR strFileName, 
		ParamsMethod.ulDesiredAccess,//IN ULONG ulDesiredAccess, 
		ParamsMethod.ulShareMode,//IN ULONG ulDesiredAccess, 
		ParamsMethod.ulCreationDisposition,//IN ULONG ulDesiredAccess, 
		ParamsMethod.ulFlagsAndAttributes,//IN ULONG ulDesiredAccess, 
		&m_spIFileDevice,//OUT IFileDevice ** ppFileDevice, 
		ParamsMethod.bCreateMember//BOOL bCreateMember
		);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("CreateInstanceFileDeviceWin32 %s, hr=0x%08x"),  ParamsMethod.strFileName.GetString(), hr);
		D_API_ERR(0, TEXT("CreateInstanceFileDeviceWin32"), hr, TEXT("%s"),  ParamsMethod.strFileName.GetString());
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

HRESULT CSocketFileDeviceHandler::ProcessSetPointerEx( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	ParameterStruct(ResultSetPointerEx) ParamsResult;
	ParamsResult.hrStatus = S_OK;

	BEGIN_BLOCK(0);

	ParameterStruct(SetPointerEx) ParamsMethod;


	hr = RPCReadParams(ParamsMethod, /*ParamsResult, */InBuffer);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ServerReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_spIFileDevice->SetPointerEx(
		ParamsMethod.llDistanceToMove,//LONGLONG llDistanceToMove, 
		&ParamsResult.NewFilePointer,//LONGLONG* lpNewFilePointer, 
		ParamsMethod.ulMoveMethod//ULONG ulMoveMethod
		);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("SetPointerEx, hr=0x%08x"),  hr);
		D_API_ERR(0, TEXT("SetPointerEx"), hr, TEXT("DistanceToMove=%I64d, MoveMethod=%u"), ParamsMethod.llDistanceToMove, ParamsMethod.ulMoveMethod);
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

HRESULT CSocketFileDeviceHandler::ProcessRead( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	ParameterStruct(ResultRead) ParamsResult;
	ParamsResult.hrStatus = S_OK;

	BEGIN_BLOCK(0);

	ParameterStruct(Read) ParamsMethod;


	hr = RPCReadParams(ParamsMethod, /*ParamsResult, */InBuffer);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ServerReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_spIFileDevice->Read(
		ParamsMethod.nNumberOfBytesToRead,//ULONG nNumberOfBytesToRead, 
		&ParamsResult.NumberOfBytesRead,//ULONG* lpNumberOfBytesRead, 
		OutBuffer.m_pData//BYTE* pBuffer
		);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("Read, hr=0x%08x"),  hr);
		D_API_ERR(0, TEXT("Read"), hr, TEXT("NumberOfBytesToRead=%u, NumberOfBytesRead=%u"), ParamsMethod.nNumberOfBytesToRead, ParamsResult.NumberOfBytesRead);
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);


	hr = RPCWriteParams(ParamsResult, ParamsResult.NumberOfBytesRead, OutBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ServerWriteResult"), hr, TEXT(""));
	}

	return hr;
}

HRESULT CSocketFileDeviceHandler::ProcessWrite( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	ParameterStruct(ResultWrite) ParamsResult;
	ParamsResult.hrStatus = S_OK;

	BEGIN_BLOCK(0);

	ParameterStruct(Write) ParamsMethod;


	hr = RPCReadParams(ParamsMethod, /*ParamsResult, */InBuffer);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ServerReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_spIFileDevice->Write(
		ParamsMethod.nNumberOfBytesToWrite,//ULONG nNumberOfBytesToWrite, 
		&ParamsResult.NumberOfBytesWritten,//ULONG* lpNumberOfBytesWritten, 
		InBuffer.m_pData//CONST BYTE* pBuffer
		);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("Read, hr=0x%08x"),  hr);
		D_API_ERR(0, TEXT("Read"), hr, TEXT("NumberOfBytesToWrite=%u, NumberOfBytesWrite=%u"), ParamsMethod.nNumberOfBytesToWrite, ParamsResult.NumberOfBytesWritten);
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

HRESULT CSocketFileDeviceHandler::ProcessGetFileSize( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	ParameterStruct(ResultGetFileSize) ParamsResult;
	ParamsResult.hrStatus = S_OK;

	BEGIN_BLOCK(0);

	ParameterStruct(GetFileSize) ParamsMethod;


	hr = RPCReadParams(ParamsMethod, /*ParamsResult, */InBuffer);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ServerReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = m_spIFileDevice->GetFileSize(
		&ParamsResult.FileSize//LONGLONG * pFileSize
		);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("GetFileSize, hr=0x%08x"),  hr);
		D_API_ERR(0, TEXT("GetFileSize"), hr, TEXT(""));
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


HRESULT CSocketFileDeviceHandler::Process( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
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

	PROCESS_METHOD(hr, CreateInstanceFileDeviceWin32, szMethodName, InBuffer, OutBuffer)
	ELSE_PROCESS_METHOD(hr, SetPointerEx, szMethodName, InBuffer, OutBuffer)
	ELSE_PROCESS_METHOD(hr, Read, szMethodName, InBuffer, OutBuffer)
	ELSE_PROCESS_METHOD(hr, Write, szMethodName, InBuffer, OutBuffer)
	ELSE_PROCESS_METHOD(hr, GetFileSize, szMethodName, InBuffer, OutBuffer)
	ELSE_PROCESS_METHOD(hr, ReadAtOffsetHelper, szMethodName, InBuffer, OutBuffer)
	ELSE_PROCESS_METHOD(hr, WriteAtOffsetHelper, szMethodName, InBuffer, OutBuffer)
	ELSE_PROCESS_METHOD(hr, Release, szMethodName, InBuffer, OutBuffer)
	ELSE_PROCESS_METHOD(hr, CreateMember, szMethodName, InBuffer, OutBuffer)
	ELSE_PROCESS_METHOD(hr, DestroyMember, szMethodName, InBuffer, OutBuffer)
	else
	{
		hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		D_API_ERR(0, TEXT("ProcessMethod"), hr, TEXT("Method %s cannot be recognizd"), szMethodName);
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}

HRESULT CSocketFileDeviceHandler::ProcessReadAtOffsetHelper( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	ParameterStruct(ResultRead) ParamsResult;
	ParamsResult.hrStatus = S_OK;

	BEGIN_BLOCK(0);

	ParameterStruct(ReadAtOffsetHelper) ParamsMethod;


	hr = RPCReadParams(ParamsMethod, /*ParamsResult, */InBuffer);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ServerReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = ::ReadAtOffsetHelper(m_spIFileDevice,
		ParamsMethod.Offset,
		ParamsMethod.nNumberOfBytesToRead,//ULONG nNumberOfBytesToRead, 
		&ParamsResult.NumberOfBytesRead,//ULONG* lpNumberOfBytesRead, 
		OutBuffer.m_pData//BYTE* pBuffer
		);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("Read, hr=0x%08x"),  hr);
		D_API_ERR(0, TEXT("Read"), hr, TEXT("NumberOfBytesToRead=%u, NumberOfBytesRead=%u"), ParamsMethod.nNumberOfBytesToRead, ParamsResult.NumberOfBytesRead);
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);


	hr = RPCWriteParams(ParamsResult, ParamsResult.NumberOfBytesRead, OutBuffer);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ServerWriteResult"), hr, TEXT(""));
	}

	return hr;
}

HRESULT CSocketFileDeviceHandler::ProcessWriteAtOffsetHelper( IN CONST WSADataBufferT & InBuffer, OUT WSADataBufferT & OutBuffer )
{
	HRESULT hr = S_OK;

	ParameterStruct(ResultWrite) ParamsResult;
	ParamsResult.hrStatus = S_OK;

	BEGIN_BLOCK(0);

	ParameterStruct(WriteAtOffsetHelper) ParamsMethod;


	hr = RPCReadParams(ParamsMethod, /*ParamsResult, */InBuffer);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("%s, hr=0x%08x"),  TEXT("ReadFromBuffer"), hr);
		D_API_ERR(0, TEXT("ServerReadParams"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	hr = ::WriteAtOffsetHelper(m_spIFileDevice,
		ParamsMethod.Offset,
		ParamsMethod.nNumberOfBytesToWrite,//ULONG nNumberOfBytesToWrite, 
		&ParamsResult.NumberOfBytesWritten,//ULONG* lpNumberOfBytesWritten, 
		InBuffer.m_pData//CONST BYTE* pBuffer
		);
	if (FAILED(hr))
	{
		ParamsResult.hrStatus = hr;
		ParamsResult.strErrorMessage.Format(TEXT("Read, hr=0x%08x"),  hr);
		D_API_ERR(0, TEXT("Read"), hr, TEXT("NumberOfBytesToWrite=%u, NumberOfBytesWrite=%u"), ParamsMethod.nNumberOfBytesToWrite, ParamsResult.NumberOfBytesWritten);
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


HRESULT CreateInstanceCSocketFileDeviceHandler(IDataBufferHandler ** ppResponceHandler, BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(/*CSocketFileDeviceHandler,*/ new CSocketFileDeviceHandler(), ppResponceHandler, bCreateMember);
}



LPCTSTR CSocketFileDeviceHandlerFactory::WhoAmI()
{
	static CONST TCHAR WHO_AM_I[] = TEXT("CSocketFileDeviceHandlerFactory:IDataBufferHandlerFactory");
	return WHO_AM_I;
}

HRESULT CSocketFileDeviceHandlerFactory::CreateMember()
{
	return S_OK;
}

HRESULT CSocketFileDeviceHandlerFactory::DestroyMember()
{
	return S_OK;
}

HRESULT CSocketFileDeviceHandlerFactory::CreateInstance( IDataBufferHandler ** ppResponceHandler )
{
	return CreateInstanceCSocketFileDeviceHandler(ppResponceHandler, TRUE);
}

CSocketFileDeviceHandlerFactory::CSocketFileDeviceHandlerFactory() :
	m_RefCount(0)
{

}

HRESULT CreateInstanceCSocketFileDeviceHandlerFactory( IDataBufferHandlerFactory ** ppResponceHandlerFactory, BOOL bCreateMember )
{
	return CREATE_INSTCANCE_COBJECT(/*CSocketFileDeviceHandlerFactory,*/ new CSocketFileDeviceHandlerFactory, ppResponceHandlerFactory, bCreateMember);
}
