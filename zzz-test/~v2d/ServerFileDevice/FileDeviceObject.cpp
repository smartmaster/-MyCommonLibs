// FileDeviceObject.cpp : Implementation of CFileDeviceObject

#include "stdafx.h"
#include "FileDeviceObject.h"


// CFileDeviceObject

STDMETHODIMP CFileDeviceObject::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	// TODO: Add your implementation code here
	ObjectLock objectLock(this);
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
		_OLD_DSETLASTSTATUS(dwLastError);
		hr = HRESULT_FROM_WIN32(dwLastError);
		_OLD_DAPIERR((0, DRNPOS, TEXT("SetFilePointerEx"), dwLastError, TEXT("[%s, %I64d, %u]"), m_strFileName.GetString(), liDistanceToMove.QuadPart, ulMoveMethod));
	}
	else
	{
		*lpNewFilePointer = liNewFilePointer.QuadPart;
	}

	return hr;
}


STDMETHODIMP CFileDeviceObject::Read(ULONG nNumberOfBytesToRead , ULONG* lpNumberOfBytesRead , BYTE* pBuffer)
{
	// TODO: Add your implementation code here
	ObjectLock objectLock(this);
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
		_OLD_DSETLASTSTATUS(dwLastError);
		hr  = HRESULT_FROM_WIN32(dwLastError);
		_OLD_DAPIERR((0, DRNPOS, TEXT("ReadFile"), dwLastError, TEXT("[%u, %u]"), nNumberOfBytesToRead, *lpNumberOfBytesRead));
	}

	return hr;
}


STDMETHODIMP CFileDeviceObject::Write(ULONG nNumberOfBytesToWrite ,  ULONG* lpNumberOfBytesWritten ,  BYTE* pBuffer)
{
	// TODO: Add your implementation code here
	ObjectLock objectLock(this);
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
		_OLD_DSETLASTSTATUS(dwLastError);
		hr  = HRESULT_FROM_WIN32(dwLastError);
		_OLD_DAPIERR((0, DRNPOS, TEXT("WriteFile"), dwLastError, TEXT("[%u, %u]"), nNumberOfBytesToWrite, *lpNumberOfBytesWritten));
	}

	return hr;
}


STDMETHODIMP CFileDeviceObject::Create(BSTR bstrFileName , ULONG ulDesiredAccess , ULONG ulShareMode , ULONG ulCreationDisposition , ULONG ulFlagsAndAttributes)
{
	// TODO: Add your implementation code here
	// TODO: Add your implementation code here
	ObjectLock objectLock(this);
	DSTART(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	m_hFile = ::CreateFile(
		bstrFileName,//__in      LPCTSTR lpFileName,
		ulDesiredAccess,//__in      DWORD dwDesiredAccess,
		ulShareMode,//__in      DWORD dwShareMode,
		NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		ulCreationDisposition,//__in      DWORD dwCreationDisposition,
		ulFlagsAndAttributes,//__in      DWORD dwFlagsAndAttributes,
		NULL//__in_opt  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == m_hFile)
	{
		dwLastError = GetLastError();
		_OLD_DSETLASTSTATUS(dwLastError);
		hr = HRESULT_FROM_WIN32(dwLastError);
		_OLD_DAPIERR((0, DRNPOS, TEXT("CreateFile"), dwLastError, TEXT("[%s]"), bstrFileName));
	}
	else
	{
		m_strFileName = bstrFileName;
	}

	return hr;
}


STDMETHODIMP CFileDeviceObject::Close(void)
{
	// TODO: Add your implementation code here
	ObjectLock objectLock(this);
	DSTART(999);
	DestroyMember();
	return S_OK;
}


STDMETHODIMP CFileDeviceObject::GetFileSizeEx(LONGLONG* pllFileSize)
{
	// TODO: Add your implementation code here
	ObjectLock objectLock(this);
	DSTART(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	LARGE_INTEGER liFileSize = {0};
	BOOL bRet = ::GetFileSizeEx(
		m_hFile,//__in   HANDLE hFile,
		&liFileSize//__out  PLARGE_INTEGER lpFileSize
		);
	if (!bRet)
	{
		dwLastError = GetLastError();
		_OLD_DSETLASTSTATUS(dwLastError);
		hr  = HRESULT_FROM_WIN32(dwLastError);
		_OLD_DAPIERR((0, DRNPOS, TEXT("GetFileSizeEx"), dwLastError));
	}
	else
	{
		*pllFileSize = liFileSize.QuadPart;
	}

	return hr;
}

VOID CFileDeviceObject::InitMember()
{
	m_hFile = INVALID_HANDLE_VALUE;
}

VOID CFileDeviceObject::DestroyMember()
{
	if (INVALID_HANDLE_VALUE != m_hFile)
	{
		::CloseHandle(m_hFile); m_hFile = INVALID_HANDLE_VALUE;
		m_strFileName.Empty();
	}
}