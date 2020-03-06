#include "stdafx.h"
#include "DTraceLib.h"
#include "FileDeviceWin32.h"



CFileDeviceWin32::CFileDeviceWin32( IN LPCTSTR strFileName, IN ULONG ulDesiredAccess, IN ULONG ulShareMode, IN ULONG ulCreationDisposition, IN ULONG ulFlagsAndAttributes ) :
m_strFileName(strFileName),
	m_ulDesiredAccess(ulDesiredAccess),
	m_ulShareMode(ulShareMode),
	m_ulCreationDisposition(ulCreationDisposition),
	m_ulFlagsAndAttributes(ulFlagsAndAttributes),
	m_hFile(INVALID_HANDLE_VALUE),
	m_RefCount(0)
{

}

CFileDeviceWin32::~CFileDeviceWin32()
{
	DestroyMember();
}

HRESULT CFileDeviceWin32::CreateMember()
{
	DSTART(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	m_hFile = ::CreateFile(
		m_strFileName,//__in      LPCTSTR lpFileName,
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
		DSETLASTSTATUS(dwLastError, -1);
		hr = HRESULT_FROM_WIN32(dwLastError);
		DAPIERR(0, TEXT("CreateFile"), dwLastError, TEXT("[%s]"), m_strFileName.GetString());
	}

	return hr;
}

HRESULT CFileDeviceWin32::DestroyMember()
{
	if (INVALID_HANDLE_VALUE != m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	return S_OK;
}

HRESULT CFileDeviceWin32::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	DSTART(999);

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
		DSETLASTSTATUS(dwLastError, -1);
		hr = HRESULT_FROM_WIN32(dwLastError);
		DAPIERR(0, TEXT("SetFilePointerEx"), dwLastError, TEXT("[%s, %I64d, %u]"), m_strFileName.GetString(), liDistanceToMove.QuadPart, ulMoveMethod);
	}
	else
	{
		*lpNewFilePointer = liNewFilePointer.QuadPart;
	}

	return hr;
}

HRESULT CFileDeviceWin32::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	DSTART(999);

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
		DSETLASTSTATUS(dwLastError, -1);
		hr  = HRESULT_FROM_WIN32(dwLastError);
		DAPIERR(0, TEXT("ReadFile"), dwLastError, TEXT("[%u, %u]"), nNumberOfBytesToRead, *lpNumberOfBytesRead);
	}

	return hr;
}

HRESULT CFileDeviceWin32::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	DSTART(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

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
		DSETLASTSTATUS(dwLastError, -1);
		hr  = HRESULT_FROM_WIN32(dwLastError);
		DAPIERR(0, TEXT("WriteFile"), dwLastError, TEXT("[%u, %u]"), nNumberOfBytesToWrite, *lpNumberOfBytesWritten);
	}

	return hr;
}

HRESULT CFileDeviceWin32::GetFileSize( LONGLONG * pFileSize )
{
	DSTART(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bRC = GetFileSizeEx(
		m_hFile,//_In_   HANDLE hFile,
		(PLARGE_INTEGER)pFileSize//_Out_  PLARGE_INTEGER lpFileSize
		);
	if (!bRC)
	{
		dwLastError = GetLastError();
		DSETLASTSTATUS(dwLastError, -1);
		hr = HRESULT_FROM_WIN32(dwLastError);
		DAPIERR(0, TEXT("GetFileSizeEx"), hr, TEXT(""));
	}

	return hr;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CreateInstanceFileDeviceWin32(
	IN LPCTSTR strFileName, 
	IN ULONG ulDesiredAccess, 
	IN ULONG ulShareMode, 
	IN ULONG ulCreationDisposition, 
	IN ULONG ulFlagsAndAttributes, 
	OUT IFileDevice ** ppFileDevice, 
	BOOL bCreateMember)
{
	HRESULT hr = S_OK;

	BEGIN_BLOCK(0);
	//////////////////////////////////////////////////////////////////////////
	CFileDeviceWin32 * pFileDeviceWin32 = new CFileDeviceWin32(
		strFileName, 
		ulDesiredAccess, 
		ulShareMode, 
		ulCreationDisposition, 
		ulFlagsAndAttributes);
	if (NULL == pFileDeviceWin32)
	{
		hr = E_OUTOFMEMORY;
		DSETLASTSTATUS(hr, -1);
		DAPIERR(0, TEXT("new CFileDeviceWin32"), hr, TEXT(""));
		LEAVE_BLOCK(0);
	}
	
	//////////////////////////////////////////////////////////////////////////
	pFileDeviceWin32->AddRef();
	if (bCreateMember)
	{
		hr = pFileDeviceWin32->CreateMember();
		if (FAILED(hr))
		{
			pFileDeviceWin32->Release();
			DSETLASTSTATUS(hr, -1);
			DAPIERR(0, TEXT("CreateMember"), hr, TEXT(""));
			LEAVE_BLOCK(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	*ppFileDevice = pFileDeviceWin32;
	END_BLOCK(0);

	return hr;
}