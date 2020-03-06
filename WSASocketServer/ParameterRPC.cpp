#include "stdafx.h"

#include "ParameterRPC.h"
#include "NameValueStream.h"
#include "ParameterStreamDef.h"

LONG ReadParamOffset( IN CONST WSADataBufferT & Buffer )
{
	LONG ParamOffset = 0;
	CopyMemory(&ParamOffset, Buffer.m_pData + Buffer.m_DataLength - sizeof(LONG), sizeof(LONG));
	return ParamOffset;
}

VOID WriteParamOffset( WSADataBufferT & Buffer, LONG ParamOffset )
{
	CopyMemory(Buffer.m_pData + Buffer.m_DataLength, &ParamOffset, sizeof(LONG));
	Buffer.m_DataLength += sizeof(LONG);
}

HRESULT ReadMethodName( CONST WSADataBufferT & Buffer, LPTSTR MethodName, LONG MethodNameBufferLen )
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	LONG ParamOffset = ReadParamOffset(Buffer);
	CComPtr<IFileDeviceMem> spFileDevice;
	hr = CreateInstanceCFileDeviceMem(Buffer.m_pData + ParamOffset, Buffer.m_DataLength - ParamOffset - sizeof(LONG), &spFileDevice, TRUE);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("CreateInstanceCFileDeviceMem"), hr, TEXT("Buffer length=%u"), Buffer.m_DataLength - ParamOffset - sizeof(LONG));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CNameValueStream nvs(spFileDevice);
	//GET_BYTE_ORDER(hr, nvs);
	CNameValueStream::DATA_BYTE_ORDER ByteOrder;
	hr = nvs.GetByteOrder(ByteOrder);
	ATLASSERT(CNameValueStream::LITTLE_ENDIAN == ByteOrder);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("GetByteOrder"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG ValueSize = 0;
	wstring Name;
	hr = nvs.ReadNameValueSize(Name, ValueSize);
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("ReadNameValueSize"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	if (IS_STRING_EQUAL(METHOD_META_NAME, Name))
	{
		ATLASSERT(MethodNameBufferLen >= ValueSize/sizeof(TCHAR));
		hr = nvs.ReadValueData(MethodName, ValueSize/sizeof(TCHAR));
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("ReadValueData"), hr, TEXT("%s"), Name.c_str());
			LEAVE_BLOCK(0);
		}
		D_INFO(99, TEXT("Method Name=%s"), MethodName);
	}
	else
	{
		MethodName[0] = 0;
		hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		D_API_ERR(0, TEXT("MethodName"), hr, TEXT("%s"), Name.c_str());
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);

	return hr;
}
