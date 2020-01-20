#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "..\PublicHeader\PublicDTraceLib.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicVMemPtr.h"


//////////////////////////////////////////////////////////////////////////
//byte order is little endian always
//////////////////////////////////////////////////////////////////////////

#include <pshpack1.h>
struct HPTHeader
{
	USHORT		m_HeaderSize;			//2 // size of header
	USHORT		m_HeaderVersion;		//4 // version of header
	ULONG		m_CommandCode;			//8 // command code - XXX_REQ or XXX_RSP
	ULONG		m_CommandCodeEx;		//12 
	ULONG		m_Flags;				//16 // flags 
	ULONG		m_FlagsEx;				//20	
	ULONG		m_Enum;					//24 // enumeration value
	ULONG		m_EnumEx;				//28	
	ULONG		m_MoreData;				//32 // customized data explained by m_Flags or m_Enum
	ULONG		m_MoreDataEx;			//36
	LONG		m_PayloadLength;		//40 // payload data length following the header
	LONGLONG	m_PayloadFileOffset;	//48 // payload data offset in file
	ULONGLONG	m_ClientHandler;		//56 // client handler to handle response
	ULONGLONG	m_ServerHandler;		//64 // server handler to handle request

	//////////////////////////////////////////////////////////////////////////
	VOID Init();
};
#include <poppack.h>


struct HPTBody
{
	LONG m_DataSize; /***INTERNAL***/
	LONG m_AllocatedSize; /***INTERNAL***/
	BYTE * m_pData; /***INTERNAL***/
	LONG m_PageSize; /***INTERNAL***/

	//////////////////////////////////////////////////////////////////////////
	HPTBody();
	~HPTBody();

	HRESULT Allocate(LONG NewDataSize);
	HRESULT Free();
};

#if 0
VOID HPTHeader_HPTBody_sample()
{
	m_TempBody.Allocate(vvvvv.GetLength() * sizeof(TCHAR) + sizeof(TCHAR));
	if (FAILED(hr)zzzzzz)
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT(""), hr, TEXT("")zzzzzz);
		LEAVE_BLOCK(0);
	}

	CopyMemory(m_TempBody.m_pData, vvvvv.GetString(), m_TempBody.m_DataSize);

	m_TempHeader.Init();
	m_TempHeader.m_HeaderSize = vvvv;
	m_TempHeader.m_HeaderVersion = vvvv;
	m_TempHeader.m_CommandCode = vvv;
	m_TempHeader.m_Flags = vvvv;
	m_TempHeader.m_Enum = vvvv;
	m_TempHeader.m_MoreData = vvvv;
	m_TempHeader.m_PayloadLength = m_TempBody.m_DataSize;
	m_TempHeader.m_PayloadFileOffset = vvvv;
	m_TempHeader.m_ClientHandler = vvv;
	m_TempHeader.m_ServerHandler = vvv;
}
#endif


HRESULT SerializeHeaderOnly(
	IN ULONG CommandCode,
	IN ULONGLONG ClientHandler,
	IN ULONGLONG ServerHandler,
	OUT HPTHeader & Header,
	OUT HPTBody & Body
	);

template<typename _TYPE>
HRESULT SerializeHeaderBody(
	IN CONST _TYPE & Data, 
	IN LPCTSTR Name, 
	IN ULONG CommandCode, 
	IN ULONGLONG ClientHandler, 
	IN ULONGLONG ServerHandler, 
	OUT CString & strXml,
	OUT HPTHeader & Header,
	OUT HPTBody & Body
	)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	strXml.Empty();
	Data.ToString(Name, strXml);

	//////////////////////////////////////////////////////////////////////////
	hr = Body.Allocate(strXml.GetLength() * sizeof(TCHAR) + sizeof(TCHAR));
	if (FAILED(hr))
	{
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	CopyMemory(Body.m_pData, strXml.GetString(), Body.m_DataSize);

	//////////////////////////////////////////////////////////////////////////
	Header.Init();

	//Heade.m_HeaderSize;			//2 // size of header
	//Heade.m_HeaderVersion;		//4 // version of header
	Header.m_CommandCode = CommandCode;			//8 // command code - XXX_REQ or XXX_RSP
	//Heade.m_CommandCodeEx;		//12 
	//Heade.m_Flags;				//16 // flags 
	//Heade.m_FlagsEx;				//20	
	//Heade.m_Enum;					//24 // enumeration value
	//Heade.m_EnumEx;				//28	
	//Heade.m_MoreData;				//32 // customized data explained by m_Flags or m_Enum
	//Heade.m_MoreDataEx;			//36
	Header.m_PayloadLength = Body.m_DataSize;		//40 // payload data length following the header
	//Heade.m_PayloadFileOffset;	//48 // payload data offset in file
	Header.m_ClientHandler = ClientHandler;		//56 // client handler to handle response
	Header.m_ServerHandler = ServerHandler;		//64 // server handler to handle request
	
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);
	
	return hr;
}



template<typename _TYPE>
HRESULT UnserializeBody(
	OUT _TYPE & Data,
	IN LPCTSTR Name,
	OUT CString & strXml,
	OUT HPTBody & Body
	)
{
	strXml = (LPTSTR)(Body.m_pData);
	Data.FromString(Name, strXml);
}

#define USE_NEW_SERIALIZE_FUNCTION 1