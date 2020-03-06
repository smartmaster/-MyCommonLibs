#include "stdafx.h"
#include "ntdll.dll.api.h"

NTDllAPITable::NTDllAPITable() :
m_hDll(nullptr),
RtlDecompressBuffer(nullptr),
RtlDecompressBufferEx(nullptr),
RtlDecompressFragment(nullptr),
RtlCompressBuffer(nullptr),
RtlGetCompressionWorkSpaceSize(nullptr)
{

}

NTDllAPITable::~NTDllAPITable()
{
	if (nullptr != m_hDll)
	{
		::FreeLibrary(m_hDll);
		m_hDll = nullptr;
	}
}

HRESULT NTDllAPITable::Init()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	static CONST TCHAR DLL_NAME[] = TEXT("ntdll.dll");
	m_hDll = ::LoadLibrary(DLL_NAME);
	if (nullptr == m_hDll)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("LoadLibrary"), hr, TEXT("%s"), DLL_NAME);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	NTDllAPITable_GetProcAddress(m_hDll, RtlDecompressBuffer);
	NTDllAPITable_GetProcAddress(m_hDll, RtlDecompressBufferEx);
	NTDllAPITable_GetProcAddress(m_hDll, RtlDecompressFragment);
	NTDllAPITable_GetProcAddress(m_hDll, RtlCompressBuffer);
	NTDllAPITable_GetProcAddress(m_hDll, RtlGetCompressionWorkSpaceSize);


	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}
