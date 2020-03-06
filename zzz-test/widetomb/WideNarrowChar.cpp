#include "stdafx.h"

#include <atlstr.h>

#include "..\..\PublicHeader\PublicCodeBlock.h"
#include "..\..\PublicHeader\PublicDTraceLib.h"

HRESULT Utf16To8String(CONST CStringW & strUtf16, CStringA & strUtf8)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	int BufferSize = WideCharToMultiByte(
		CP_UTF8,//_In_       UINT CodePage,
		0,//_In_       DWORD dwFlags,
		strUtf16.GetString(),//_In_       LPCWSTR lpWideCharStr,
		-1,//_In_       int cchWideChar,
		NULL,//_Out_opt_  LPSTR lpMultiByteStr,
		0,//_In_       int cbMultiByte,
		NULL,//_In_opt_   LPCSTR lpDefaultChar,
		NULL//_Out_opt_  LPBOOL lpUsedDefaultChar
		);
	if (0 == BufferSize)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_API_ERR(0, TEXT("WideCharToMultiByte"), hr, TEXT("[%s]"), strUtf16.GetString());
		LEAVE_BLOCK(0);
	}

	CONST INT DELTA = 4;
	LPSTR pBufferUtf8 = strUtf8.GetBuffer(BufferSize + DELTA);
	BufferSize = WideCharToMultiByte(
		CP_UTF8,//_In_       UINT CodePage,
		0,//_In_       DWORD dwFlags,
		strUtf16.GetString(),//_In_       LPCWSTR lpWideCharStr,
		-1,//_In_       int cchWideChar,
		pBufferUtf8,//_Out_opt_  LPSTR lpMultiByteStr,
		BufferSize,//_In_       int cbMultiByte,
		NULL,//_In_opt_   LPCSTR lpDefaultChar,
		NULL//_Out_opt_  LPBOOL lpUsedDefaultChar
		);
	strUtf8.ReleaseBuffer();
	if (0 == BufferSize)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_API_ERR(0, TEXT("WideCharToMultiByte"), hr, TEXT("[%s]"), strUtf16.GetString());
		LEAVE_BLOCK(0);
	}
		
	END_BLOCK(0);
	
	return hr;
}

HRESULT Utf8To16String(CONST CStringA & strUtf8, CStringW & strUtf16)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;
	
	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	int BufferSize = MultiByteToWideChar(
		CP_UTF8,//_In_       UINT CodePage,
		0,//_In_       DWORD dwFlags,
		strUtf8.GetString(),//_In_       LPCSTR lpMultiByteStr,
		-1,//_In_       int cbMultiByte,
		NULL,//_Out_opt_  LPWSTR lpWideCharStr,
		0//_In_       int cchWideChar
		);
	if (0 == BufferSize)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_API_ERR(0, TEXT("MultiByteToWideChar"), hr, TEXT("[%S]"), strUtf8.GetString());
		LEAVE_BLOCK(0);
	}

	CONST INT DELTA = 4;
	LPWSTR pBufferUtf16 = strUtf16.GetBuffer(BufferSize + DELTA);
	BufferSize = MultiByteToWideChar(
		CP_UTF8,//_In_       UINT CodePage,
		0,//_In_       DWORD dwFlags,
		strUtf8.GetString(),//_In_       LPCSTR lpMultiByteStr,
		-1,//_In_       int cbMultiByte,
		pBufferUtf16,//_Out_opt_  LPWSTR lpWideCharStr,
		BufferSize//_In_       int cchWideChar
		);
	strUtf16.ReleaseBuffer();
	if (0 == BufferSize)
	{
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_API_ERR(0, TEXT("MultiByteToWideChar"), hr, TEXT("[%S]"), strUtf8.GetString());
		LEAVE_BLOCK(0);
	}

	END_BLOCK(0);
	
	return hr;
}