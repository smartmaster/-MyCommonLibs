#include "stdafx.h"

#include <algorithm>
#include "FileVerInfo.h"


HRESULT CFileVerInfo::GetImageFileType(LPCWSTR pwszModulePath, USHORT & usType)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (nullptr == pwszModulePath || 0 == pwszModulePath[0])
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT("GetImageType"), hr, TEXT("empty file name"));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//see http://www.microsoft.com/whdc/system/platform/firmware/PECOFF.mspx 
	//offset to PE header is always at 0x3C 
	//PE header starts with "PE\0\0" =  0x50 0x45 0x00 0x00 
	//followed by 2-byte machine type field (see document above for enum)
	CHandle hFile(::CreateFile(
		pwszModulePath,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
		));
	if (INVALID_HANDLE_VALUE == hFile.m_h)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("[%s]"), pwszModulePath);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	DWORD dwRead = 0;
	DWORD dwPEOffset = 0;
	OVERLAPPED op = { 0 };
	op.Offset = 0x3c;
	BOOL bret = ReadFile(hFile, &dwPEOffset, sizeof(DWORD), &dwRead, &op);
	if (!bret)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ReadFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	ZeroMemory(&op, sizeof(op));
	op.Offset = dwPEOffset;
	DWORD dwPEHead = 0;
	bret = ReadFile(hFile, &dwPEHead, sizeof(DWORD), &dwRead, &op);
	if (!bret)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ReadFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	if (dwPEHead != 0x00004550) // "PE\0\0", little-endian 
	{
		hr = E_INVALIDARG;
		D_API_ERR(0, TEXT(""), hr, TEXT("PEHeader is incorrect, path = %s, headSig = 0x%08X"), pwszModulePath, dwPEHead);
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	bret = ReadFile(hFile, &usType, sizeof(__int16), &dwRead, nullptr);
	if (!bret)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("ReadFile"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}

BOOL CFileVerInfo::QueryHelper(LPCVOID pBlock, LPCTSTR Name, LANGANDCODEPAGE * lpTranslate, UINT cbTranslate, CString & result)
{
	CString SubBlock;
	LPCTSTR lpBuffer = nullptr;
	UINT dwBytes = 0;
	BOOL bRet = FALSE;
	for (int ii = 0; ii < cbTranslate / sizeof(LANGANDCODEPAGE); ++ii)
	{
		lpBuffer = nullptr;
		dwBytes = 0;
		SubBlock.Format(TEXT("\\StringFileInfo\\%04x%04x\\%s"), lpTranslate[ii].wLanguage, lpTranslate[ii].wCodePage, Name);
		bRet = VerQueryValueW(pBlock, SubBlock.GetString(), (void**)&lpBuffer, &dwBytes);
		if (bRet)
		{
			result = lpBuffer;
			break;
		}
	}
	return bRet;
}

HRESULT CFileVerInfo::GetImageFileVersion(LPCTSTR wstrPath, USHORT & usImageType, VS_FIXEDFILEINFO & ffi, vector<CString> & vecstr)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	hr = GetImageFileType(wstrPath, usImageType);
	if (FAILED(hr))
	{
		//LastError = WSAGetLastError();
		//LastError = GetLastError();
		//hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetImageFileType"), hr, TEXT("[file=%s, type=%d]"), wstrPath, usImageType);
		LEAVE_BLOCK(0);
	}


	//////////////////////////////////////////////////////////////////////////
	DWORD dwHandle = 0;
	DWORD dwSize = ::GetFileVersionInfoSize(wstrPath, &dwHandle);
	if (dwSize == 0)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetFileVersionInfoSizeW"), hr, TEXT("[%s]"), wstrPath);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	CAutoVectorPtr<BYTE> spData(new BYTE[dwSize]);
	BOOL bRet = GetFileVersionInfo(wstrPath, 0, dwSize, spData.m_p);
	if (!bRet)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("GetFileVersionInfoW"), hr, TEXT("[%s]"), wstrPath);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	VS_FIXEDFILEINFO * pffi = nullptr;
	UINT   uLenRoot = 0;
	LPCTSTR ROOT = TEXT("\\");
	bRet = VerQueryValue(
		spData.m_p,//_In_  LPCVOID pBlock,
		ROOT,//_In_  LPCTSTR lpSubBlock,
		(LPVOID  *)&pffi,//_Out_ LPVOID  *lplpBuffer,
		&uLenRoot//_Out_ PUINT   puLen
		);
	if (!bRet)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("VerQueryValue"), hr, TEXT("[%s]"), ROOT);
		LEAVE_BLOCK(0);
	}
	CopyMemory(&ffi, pffi, std::min<int>(uLenRoot, sizeof(VS_FIXEDFILEINFO)));


	//////////////////////////////////////////////////////////////////////////

	UINT cbTranslate = 0;
	LANGANDCODEPAGE * lpTranslate = nullptr; //this is a weak pointer, do not free!
	LPCTSTR TRANSLATION = TEXT("\\VarFileInfo\\Translation");
	bRet = VerQueryValue(spData.m_p, TRANSLATION, (LPVOID*)&lpTranslate, &cbTranslate);
	if (!bRet)
	{
		//LastError = WSAGetLastError();
		LastError = GetLastError();
		hr = HRESULT_FROM_WIN32(LastError);
		//D_SET_FISRT_STATUS(hr, LastError);
		//D_SET_FISRT_ERROR_MESSAGE(TEXT("")zzzzzz);
		//D_SET_LAST_STATUS(hr, LastError);
		//D_SET_LAST_ERROR_MESSAGE(TEXT("")zzzzzz);
		D_API_ERR(0, TEXT("VerQueryValueW"), hr, TEXT("[%s]"), TRANSLATION);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	static CONST LPCTSTR Names[] =
	{
		TEXT("Comments"),
		TEXT("InternalName"),
		TEXT("ProductName"),
		TEXT("CompanyName"),
		TEXT("LegalCopyright"),
		TEXT("ProductVersion"),
		TEXT("FileDescription"),
		TEXT("LegalTrademarks"),
		TEXT("PrivateBuild"),
		TEXT("FileVersion"),
		TEXT("OriginalFilename"),
		TEXT("SpecialBuild")
	};
	vecstr.resize(_countof(Names));
	for (int ii = 0; ii < _countof(Names); ++ii)
	{
		QueryHelper(spData.m_p, Names[ii], lpTranslate, cbTranslate, vecstr[ii]);
	}
	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}
