#include "stdafx.h"

#include <atlsecurity.h>

#include "FileDeviceWin32Prealloc.h"


CFileDeviceWin32Prealloc::CFileDeviceWin32Prealloc( LONGLONG Delta, LPCTSTR strFileName, ULONG ulDesiredAccess, ULONG ulShareMode, ULONG ulCreationDisposition, ULONG ulFlagsAndAttributes ) :
	m_RefCount(0),
	m_hFile(INVALID_HANDLE_VALUE),
	m_Delata(Delta),
	m_CurPointer(0),
	m_MaxDataSize(0),
	m_AllocatedSize(0),
	m_strFileName(strFileName),
	m_ulDesiredAccess(ulDesiredAccess),
	m_ulShareMode(ulShareMode),
	m_ulCreationDisposition(ulCreationDisposition),
	m_ulFlagsAndAttributes(ulFlagsAndAttributes),
	m_bToEnablePrivilege(TRUE) //<sonmi01>2014-12-10 ###???
{
	CONST ULONG __128M = 128 * 1024 * 204;
	if (m_Delata < __128M)
	{
		m_Delata = __128M;
	}
}

LPCTSTR CFileDeviceWin32Prealloc::WhoAmI()
{
	static CONST TCHAR WHO_AM_I[] = TEXT("CFileDeviceWin32Prealloc:FileDevice");
	return WHO_AM_I;
}

HRESULT CFileDeviceWin32Prealloc::CreateMember()
{
	// TODO: Add your implementation code here
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	m_hFile = ::CreateFile(
		m_strFileName.GetString(),//__in      LPCTSTR lpFileName,
		m_ulDesiredAccess,//__in      DWORD dwDesiredAccess,
		m_ulShareMode,//__in      DWORD dwShareMode,
		NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		m_ulCreationDisposition,//__in      DWORD dwCreationDisposition,
		m_ulFlagsAndAttributes,//__in      DWORD dwFlagsAndAttributes,
		NULL//__in_opt  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == m_hFile)
	{
		dwLastError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_SET_LAST_STATUS(hr, dwLastError);
		D_API_ERR(0, TEXT("CreateFile"), hr, TEXT("[%s]"), m_strFileName.GetString());
	}

	return hr;
}

HRESULT CFileDeviceWin32Prealloc::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	if (INVALID_HANDLE_VALUE != m_hFile)
	{
		LARGE_INTEGER liToMove = {0};
		LARGE_INTEGER liNewPointer = {0};
		liToMove.QuadPart = m_MaxDataSize;
		BOOL bSFP = ::SetFilePointerEx(m_hFile, liToMove, &liNewPointer, FILE_BEGIN);
		if (!bSFP)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			D_SET_LAST_STATUS(hr, dwLastError);
			D_API_ERR(0, TEXT("SetFilePointerEx"), hr, TEXT("ToMove=%I64d, NewPointer=%I64d"), liToMove.QuadPart, liNewPointer.QuadPart);
		}

		//////////////////////////////////////////////////////////////////////////
		BOOL bSEF = ::SetEndOfFile(m_hFile);
		if (!bSEF)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			D_SET_LAST_STATUS(hr, dwLastError);
			D_API_ERR(0, TEXT("SetEndOfFile"), hr, TEXT(""));
		}

		CloseHandle(m_hFile); m_hFile = INVALID_HANDLE_VALUE;
		m_strFileName.Empty();
	}

	return hr;
}

HRESULT CFileDeviceWin32Prealloc::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	LARGE_INTEGER liDistanceToMove;
	liDistanceToMove.QuadPart = llDistanceToMove;
	LARGE_INTEGER liNewFilePointer = {0};
	BOOL bRet =  ::SetFilePointerEx(
		m_hFile,//__in       HANDLE hFile,
		liDistanceToMove,//__in       LARGE_INTEGER liDistanceToMove,
		&liNewFilePointer,//__out_opt  PLARGE_INTEGER lpNewFilePointer,
		ulMoveMethod//__in       DWORD dwMoveMethod
		);
	if (!bRet)
	{
		dwLastError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_SET_LAST_STATUS(hr, dwLastError);
		D_API_ERR(0, TEXT("SetFilePointerEx"), dwLastError, TEXT("[%s, %I64d, %u]"), m_strFileName.GetString(), liDistanceToMove.QuadPart, ulMoveMethod);
	}
	else
	{
		*lpNewFilePointer = liNewFilePointer.QuadPart;
		m_CurPointer = liNewFilePointer.QuadPart; 
	}

	return hr;
}

HRESULT CFileDeviceWin32Prealloc::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bRet = ::ReadFile(
		m_hFile,//__in         HANDLE hFile,
		pBuffer,//__out        LPVOID lpBuffer,
		nNumberOfBytesToRead,//__in         DWORD nNumberOfBytesToRead,
		lpNumberOfBytesRead,//__out_opt    LPDWORD lpNumberOfBytesRead,
		NULL//__inout_opt  LPOVERLAPPED lpOverlapped
		);
	if (!bRet)
	{
		dwLastError = GetLastError();
		D_SET_LAST_STATUS(dwLastError, -1);
		hr  = HRESULT_FROM_WIN32(dwLastError);
		D_API_ERR(0, TEXT("ReadFile"), dwLastError, TEXT("[%u, %u]"), nNumberOfBytesToRead, *lpNumberOfBytesRead);
	}
	else
	{
		m_CurPointer += *lpNumberOfBytesRead;
	}

	return hr;
}

HRESULT CFileDeviceWin32Prealloc::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	// TODO: Add your implementation code here
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	HRESULT hrTmp = S_OK;
	DWORD dwLastErrorTmp = 0;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	if (m_CurPointer + nNumberOfBytesToWrite > m_AllocatedSize)
	{
		//////////////////////////////////////////////////////////////////////////
		m_AllocatedSize = AtlAlignUp(m_CurPointer + nNumberOfBytesToWrite, m_Delata);
		LARGE_INTEGER liToMove = {0};
		LARGE_INTEGER liNewPointer = {0};
		liToMove.QuadPart = m_AllocatedSize;
		BOOL bSFP = ::SetFilePointerEx(m_hFile, liToMove, &liNewPointer, FILE_BEGIN);
		if (!bSFP)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			D_SET_LAST_STATUS(hr, dwLastError);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("SetFilePointerEx"), hr, TEXT("ToMove=%I64d, NewPointer=%I64d"), liToMove.QuadPart, liNewPointer.QuadPart);
			LEAVE_BLOCK(0);
		}

		//////////////////////////////////////////////////////////////////////////
		BOOL bSEF = ::SetEndOfFile(m_hFile);
		if (!bSEF)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			D_SET_LAST_STATUS(hr, dwLastError);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("SetEndOfFile"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}

	

		if (m_bToEnablePrivilege)
		{
			CAccessToken accTok;
			CTokenPrivileges PreviousState;
			BOOL bGET = accTok.GetEffectiveToken(TOKEN_ALL_ACCESS/*TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY*/);
			if (!bGET)
			{
				dwLastErrorTmp = GetLastError();
				hrTmp = HRESULT_FROM_WIN32(dwLastErrorTmp); //do not make it an error
				D_API_ERR(0, TEXT("GetEffectiveToken"), hrTmp, TEXT(""));
			}
			else
			{
				BOOL bEnablePrivilege = accTok.EnablePrivilege(SE_MANAGE_VOLUME_NAME, &PreviousState);
				if (!bEnablePrivilege)
				{
					dwLastErrorTmp = GetLastError();
					hrTmp = HRESULT_FROM_WIN32(dwLastErrorTmp); //do not make it an error
					D_API_ERR(0, TEXT("EnablePrivilege"), hrTmp, TEXT(""));
				}
				else
				{
					m_bToEnablePrivilege = FALSE;
				}
			}
		}

		BOOL bSFVD = ::SetFileValidData(
			m_hFile,//_In_  HANDLE hFile,
			m_AllocatedSize//_In_  LONGLONG ValidDataLength
			);
		if (!bSFVD)
		{
			dwLastErrorTmp = GetLastError();
			hrTmp = HRESULT_FROM_WIN32(dwLastErrorTmp);
			D_API_ERR(0, TEXT("SetFileValidData"), hr, TEXT("SetFileValidData [file=0x%p, DistanceToMove=%I64d, hr=0x%08x]"), m_hFile, m_AllocatedSize, hrTmp);
			//LEAVE_BLOCK(0);  //do not make it an error
		}


		//////////////////////////////////////////////////////////////////////////
		liToMove.QuadPart = m_CurPointer; //reset for write position
		liNewPointer.QuadPart = 0;
		bSFP = ::SetFilePointerEx(m_hFile, liToMove, &liNewPointer, FILE_BEGIN);
		if (!bSFP)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			D_SET_LAST_STATUS(hr, dwLastError);
			D_API_ERR(0, /*DRNPOS,*/ TEXT("SetFilePointerEx"), hr, TEXT("ToMove=%I64d, NewPointer=%I64d"), liToMove.QuadPart, liNewPointer.QuadPart);
			LEAVE_BLOCK(0);
		}
	}

	CONST ULONG SECTOR_SIZE = 512; 
	ULONG ulPad = 0;
	if (m_ulFlagsAndAttributes & FILE_FLAG_NO_BUFFERING)
	{
		if (nNumberOfBytesToWrite % SECTOR_SIZE)
		{
			ulPad = SECTOR_SIZE - ( nNumberOfBytesToWrite % SECTOR_SIZE);
			nNumberOfBytesToWrite = AtlAlignUp(nNumberOfBytesToWrite, SECTOR_SIZE);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	BOOL bRet = ::WriteFile(
		m_hFile,//__in         HANDLE hFile,
		pBuffer,//__in         LPCVOID lpBuffer,
		nNumberOfBytesToWrite,//__in         DWORD nNumberOfBytesToWrite,
		lpNumberOfBytesWritten,//__out_opt    LPDWORD lpNumberOfBytesWritten,
		NULL//__inout_opt  LPOVERLAPPED lpOverlapped
		);
	if (!bRet)
	{
		dwLastError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_SET_LAST_STATUS(hr, dwLastError);
		D_API_ERR(0, /*DRNPOS,*/ TEXT("WriteFile"), dwLastError, TEXT("[%u, %u]"), nNumberOfBytesToWrite, *lpNumberOfBytesWritten);
		LEAVE_BLOCK(0);
	}

	//////////////////////////////////////////////////////////////////////////
	m_CurPointer += (*lpNumberOfBytesWritten);
	if (m_CurPointer > m_MaxDataSize)
	{
		m_MaxDataSize = m_CurPointer;
	}

	END_BLOCK(0);

	return hr;
}

HRESULT CFileDeviceWin32Prealloc::GetFileSize( LONGLONG * pFileSize )
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bRC = GetFileSizeEx(
		m_hFile,//_In_   HANDLE hFile,
		(PLARGE_INTEGER)pFileSize//_Out_  PLARGE_INTEGER lpFileSize
		);
	if (!bRC)
	{
		dwLastError = GetLastError();
		D_SET_LAST_STATUS(dwLastError, -1);
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_API_ERR(0, TEXT("GetFileSizeEx"), hr, TEXT(""));
	}

	return hr;
}

HANDLE CFileDeviceWin32Prealloc::GetRawHandle()
{
	return m_hFile;
}


HRESULT CreateInstanceCFileDeviceWin32Prealloc(
	LONGLONG Delta, 
	LPCTSTR strFileName, 
	ULONG ulDesiredAccess, 
	ULONG ulShareMode, 
	ULONG ulCreationDisposition, 
	ULONG ulFlagsAndAttributes,
	IFileDevice ** ppObj,
	BOOL bCreateMember)
{
	return CREATE_INSTCANCE_COBJECT(/*CFileDeviceWin32Prealloc,*/
		new CFileDeviceWin32Prealloc(
		Delta, 
		strFileName, 
		ulDesiredAccess, 
		ulShareMode, 
		ulCreationDisposition, 
		ulFlagsAndAttributes),
		ppObj,
		bCreateMember
		);
}