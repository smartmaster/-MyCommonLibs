#include "stdafx.h"


#include <atlsecurity.h>

#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"

#include "VhdxHelper.h"

LPCTSTR GuidToStringHelper(CONST GUID & guid, LPTSTR pBuffer, INT Len)
{
	StringFromGUID2(
		guid,//_In_   REFGUID rguid,
		pBuffer,//_Out_  LPOLESTR lpsz,
		Len//_In_   int cchMax
		);
	return pBuffer;
}

VOID GetStringInBuffer( LPCBYTE pBufferStart, UINT Offset, UINT LengthInByte, CString & str )
{
	INT Bufferlen = LengthInByte / sizeof(TCHAR) + 2;
	LPTSTR pBufferStr = str.GetBuffer(Bufferlen);
	ZeroMemory(pBufferStr, Bufferlen * sizeof(TCHAR));
	CopyMemory(pBufferStr, pBufferStart + Offset, LengthInByte);
	str.ReleaseBuffer();
}


#if 0
//<sonmi01>2014-3-2 ###???
BOOL LineIntersect(LONGLONG s1, LONGLONG e1, LONGLONG s2, LONGLONG e2, LONGLONG & s, LONGLONG & e)
{
	s = max(s1, s2);
	e = min(e1, e2);
	return s < e;

#if 0
	BOOL bConnected = TRUE;
	if (s1 >= s2 && s1 < e2)
	{
		s = s1;
	}
	else if (s2 >= s1 && s2 < e1)
	{
		s = s2;
	}
	else
	{
		bConnected = FALSE;
	}

	if (e1 > s2 && e1 <= e2)
	{
		e = e1;
	}
	else if (e2 > s1 && e2 <= e1)
	{
		e = e2;
	}
	else
	{
		bConnected = FALSE;
	}

	return bConnected;
#endif
}

BOOL LineConnect(
	LONGLONG s1, LONGLONG e1, 
	LONGLONG s2, LONGLONG e2, 
	LONGLONG & sInner, LONGLONG & eInner,
	LONGLONG & sOuter, LONGLONG & eOuter
	)
{
	sOuter = min(s1, s2);
	eOuter = max(e1, e2);

	sInner = max(s1, s2);
	eInner = min(e1, e2);
	return sInner <= eInner;

#if 0
	BOOL bConnected = TRUE;

	//////////////////////////////////////////////////////////////////////////
	if (s1 >= s2 && s1 <= e2)
	{
		sInner = s1;
	}
	else if (s2 >= s1 && s2 <= e1)
	{
		sInner = s2;
	}
	else
	{
		bConnected = FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	if (e1 >= s2 && e1 <= e2)
	{
		eInner = e1;
	}
	else if (e2 >= s1 && e2 <= e1)
	{
		eInner = e2;
	}
	else
	{
		bConnected = FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	sOuter = min(s1, s2);
	eOuter = max(e1, e2);


	//////////////////////////////////////////////////////////////////////////
	return bConnected;
#endif
}


#endif

//<sonmi01>2014-4-21 ###???
HRESULT SetFileSizeFast(HANDLE hFile, LONGLONG FileSize)
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
#if 1
	//////////////////////////////////////////////////////////////////////////
	LARGE_INTEGER liDistanceToMove;
	liDistanceToMove.QuadPart = FileSize;
	LARGE_INTEGER NewFilePointer = { 0 };
	BOOL bRet = SetFilePointerEx(
		hFile,//_In_       HANDLE hFile,
		liDistanceToMove,//_In_       LARGE_INTEGER liDistanceToMove,
		&NewFilePointer,//_Out_opt_  PLARGE_INTEGER lpNewFilePointer,
		FILE_BEGIN//_In_       DWORD dwMoveMethod
		);
	if (!bRet)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		D_API_ERR(0, TEXT("SetFilePointerEx"), hr, TEXT("SetFilePointerEx [file=0x%p, DistanceToMove=%I64d, hr=0x%08x]"), hFile, liDistanceToMove.QuadPart, hr);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	bRet = SetEndOfFile(hFile);
	if (!bRet)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		D_API_ERR(0, TEXT("SetEndOfFile"), hr, TEXT("SetEndOfFile [file=0x%p, DistanceToMove=%I64d, hr=0x%08x]"), hFile, liDistanceToMove.QuadPart, hr);
		LEAVE_BLOCK(0);
	}
#endif

	CAccessToken accTok;
	CTokenPrivileges PreviousState;
	bRet = accTok.GetEffectiveToken(TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY);
	if (bRet)
	{
		bRet = accTok.EnablePrivilege(SE_MANAGE_VOLUME_NAME, &PreviousState);
		D_INFO(0, TEXT("EnablePrivilege %s, bRet=%d"), SE_MANAGE_VOLUME_NAME, bRet);
	}


	bRet = SetFileValidData(
		hFile,//_In_  HANDLE hFile,
		FileSize//_In_  LONGLONG ValidDataLength
		);
	if (!bRet)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		D_API_ERR(0, TEXT("SetFileValidData"), hr, TEXT("SetFileValidData [file=0x%p, DistanceToMove=%I64d, hr=0x%08x]"), hFile, FileSize, hr);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	END_BLOCK(0);

	return hr;
}


