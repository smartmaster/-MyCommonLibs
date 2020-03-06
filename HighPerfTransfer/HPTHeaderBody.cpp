#include "stdafx.h"

#include "HPTHeaderBody.h"


VOID HPTHeader::Init()
{
	ZeroMemory(this, sizeof(HPTHeader));
	m_HeaderSize = sizeof(HPTHeader);
	m_HeaderVersion = MAKEWORD(0, 0);
}

HPTBody::HPTBody() :
m_DataSize(0),
m_AllocatedSize(0),
m_pData(NULL)
{
	SYSTEM_INFO SystemInfo = { 0 };
	GetSystemInfo(&SystemInfo);
	m_PageSize = SystemInfo.dwPageSize;
}

HPTBody::~HPTBody()
{
	Free();
}

HRESULT HPTBody::Allocate(LONG NewDataSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (NewDataSize <= m_AllocatedSize)
	{
		m_DataSize = NewDataSize;
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	LONG NewAllocateSize = AtlAlignUp(NewDataSize, m_PageSize);
	CVMemPtr<BYTE> apNewBuffer;
	apNewBuffer.Allocate(NewAllocateSize);
	if (NULL == apNewBuffer.m_pData)
	{
		hr = E_OUTOFMEMORY;
		D_API_ERR(0, TEXT("Allocate"), hr, TEXT("size=%d"), NewAllocateSize);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CVMemPtr<BYTE> apBufferTemp(m_pData);
	apBufferTemp.Free();

	m_AllocatedSize = NewAllocateSize;
	m_DataSize = NewDataSize;
	m_pData = apNewBuffer.Detach();

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

HRESULT HPTBody::Free()
{
	if (m_pData)
	{
		CVMemPtr<BYTE> apBufferTemp(m_pData);
		apBufferTemp.Free();
		m_pData = NULL;
		m_DataSize = 0;
		m_AllocatedSize = 0;
	}
	return S_OK;
}

HRESULT SerializeHeaderOnly(IN ULONG CommandCode, IN ULONGLONG ClientHandler, IN ULONGLONG ServerHandler, OUT HPTHeader & Header, OUT HPTBody & Body)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = Body.Allocate(0);

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
