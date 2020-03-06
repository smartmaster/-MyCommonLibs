#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"
#include "FileDeviceHandle.h"




CFileDeviceHandle::CFileDeviceHandle( HANDLE Handle,  BOOL bAttach ) :
	m_Handle(Handle),
	m_RefCount(0),
	m_bAttacch(bAttach)
{

}

HRESULT CFileDeviceHandle::CreateMember()
{
	return S_OK;
}

HRESULT CFileDeviceHandle::DestroyMember()
{
	if (m_bAttacch)
	{
		CloseHandle(m_Handle);
		m_Handle = INVALID_HANDLE_VALUE;
		m_bAttacch = FALSE;
	}
	return S_OK;
}

HRESULT CFileDeviceHandle::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	LARGE_INTEGER liDistanceToMove;
	liDistanceToMove.QuadPart = llDistanceToMove;
	LARGE_INTEGER liNewFilePointer = {0};
	BOOL bRet =  ::SetFilePointerEx(
		m_Handle,//__in       HANDLE hFile,
		liDistanceToMove,//__in       LARGE_INTEGER liDistanceToMove,
		&liNewFilePointer,//__out_opt  PLARGE_INTEGER lpNewFilePointer,
		ulMoveMethod//__in       DWORD dwMoveMethod
		);
	if (!bRet)
	{
		dwLastError = GetLastError();
		D_SET_LAST_STATUS(dwLastError, -1);
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_API_ERR(0, TEXT("SetFilePointerEx"), dwLastError, TEXT("[0x%p, %I64d, %u]"), m_Handle, liDistanceToMove.QuadPart, ulMoveMethod);
	}
	else
	{
		*lpNewFilePointer = liNewFilePointer.QuadPart;
	}

	return hr;
}


static HRESULT ReadFileInSmallBuffer(HANDLE hFile, BYTE * pBuffer, LONG nTotalNumberOfBytesToRead, ULONG * pnTotalNumberOfBytesRead)
{
	LONG Offset = 0;
	LONG ToReadLen = 0;
	ULONG Readlen = 0;

	BOOL bRet = FALSE;
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (pnTotalNumberOfBytesRead)
	{
		*pnTotalNumberOfBytesRead = 0;
	}

	CONST LONG SMALL_BLOCK_SIZE = 1024 * 1024;

	while (nTotalNumberOfBytesToRead)
	{
		//////////////////////////////////////////////////////////////////////////
		if (nTotalNumberOfBytesToRead > SMALL_BLOCK_SIZE)
		{
			ToReadLen = SMALL_BLOCK_SIZE;
		}
		else
		{
			ToReadLen = nTotalNumberOfBytesToRead;
		}


		//////////////////////////////////////////////////////////////////////////
		Readlen = 0;
		bRet = ::ReadFile(
			hFile,//__in         HANDLE hFile,
			pBuffer + Offset,//__out        LPVOID lpBuffer,
			ToReadLen,//__in         DWORD nNumberOfBytesToRead,
			&Readlen,//__out_opt    LPDWORD lpNumberOfBytesRead,
			NULL//__inout_opt  LPOVERLAPPED lpOverlapped
			);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("ReadFile"), hr, TEXT("SMALL_BLOCK_SIZE=%d"), SMALL_BLOCK_SIZE);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		Offset += Readlen;
		nTotalNumberOfBytesToRead -= Readlen;
		if (pnTotalNumberOfBytesRead)
		{
			*pnTotalNumberOfBytesRead += Readlen;
		}

		//////////////////////////////////////////////////////////////////////////
		if (Readlen < ToReadLen)
		{
			D_INFO(0, TEXT("Readlen is less than ToReadLen, assume file end. [%d < %d]"), Readlen, ToReadLen);
			break;
		}
	}

	return hr;
}

HRESULT CFileDeviceHandle::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bRet = ::ReadFile(
		m_Handle,//__in         HANDLE hFile,
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
		if (ERROR_NO_SYSTEM_RESOURCES == dwLastError)
		{
			D_API_ERR(0, TEXT("ReadFile"), dwLastError, TEXT("[%u, %u] ERROR_NO_SYSTEM_RESOURCES - read 1M data until finish"), nNumberOfBytesToRead, *lpNumberOfBytesRead);
			hr = ReadFileInSmallBuffer(m_Handle, pBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead);
			if (FAILED(hr))
			{
				D_API_ERR(0, TEXT("ReadFileInSmallBuffer"), hr, TEXT("small buffer size read"));
				//LEAVE_BLOCK(0);
			}
		}
	}

	return hr;
}


//<sonmi01>2014-7-10 ###???
static HRESULT WriteFileInSmallBuffer(HANDLE hFile, CONST BYTE * pBuffer, LONG nTotalNumberOfBytesToWrite, ULONG * pnTotalNumberOfBytesWritten)
{
	LONG Offset = 0;
	LONG ToWriteLen = 0;
	ULONG Writelen = 0;

	BOOL bRet = FALSE;
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (pnTotalNumberOfBytesWritten)
	{
		*pnTotalNumberOfBytesWritten = 0;
	}

	CONST LONG SMALL_BLOCK_SIZE = 64 * 1024;

	while (nTotalNumberOfBytesToWrite)
	{
		//////////////////////////////////////////////////////////////////////////
		if (nTotalNumberOfBytesToWrite > SMALL_BLOCK_SIZE)
		{
			ToWriteLen = SMALL_BLOCK_SIZE;
		}
		else
		{
			ToWriteLen = nTotalNumberOfBytesToWrite;
		}


		//////////////////////////////////////////////////////////////////////////
		Writelen = 0;
		bRet = ::WriteFile(
			hFile,//__in         HANDLE hFile,
			pBuffer + Offset,//__out        LPVOID lpBuffer,
			ToWriteLen,//__in         DWORD nNumberOfBytesToRead,
			&Writelen,//__out_opt    LPDWORD lpNumberOfBytesRead,
			NULL//__inout_opt  LPOVERLAPPED lpOverlapped
			);
		if (!bRet)
		{
			LastError = GetLastError();
			hr = HRESULT_FROM_WIN32(LastError);
			D_API_ERR(0, TEXT("WriteFile"), hr, TEXT("SMALL_BLOCK_SIZE=%d"), SMALL_BLOCK_SIZE);
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		Offset += Writelen;
		nTotalNumberOfBytesToWrite -= Writelen;
		if (pnTotalNumberOfBytesWritten)
		{
			*pnTotalNumberOfBytesWritten += Writelen;
		}

		//////////////////////////////////////////////////////////////////////////
		if (Writelen < ToWriteLen)
		{
			D_INFO(0, TEXT("Writelen is less than ToWriteLen, assume file end. [%d < %d]"), Writelen, ToWriteLen);
			break;
		}
	}

	return hr;
}


HRESULT CFileDeviceHandle::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bRet = ::WriteFile(
		m_Handle,//__in         HANDLE hFile,
		pBuffer,//__in         LPCVOID lpBuffer,
		nNumberOfBytesToWrite,//__in         DWORD nNumberOfBytesToWrite,
		lpNumberOfBytesWritten,//__out_opt    LPDWORD lpNumberOfBytesWritten,
		NULL//__inout_opt  LPOVERLAPPED lpOverlapped
		);
	if (!bRet)
	{
		dwLastError = GetLastError();
		D_SET_LAST_STATUS(dwLastError, -1);
		hr  = HRESULT_FROM_WIN32(dwLastError);
		D_API_ERR(0, TEXT("WriteFile"), dwLastError, TEXT("[%u, %u]"), nNumberOfBytesToWrite, *lpNumberOfBytesWritten);
		hr = WriteFileInSmallBuffer(m_Handle, pBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten); //<sonmi01>2014-7-10 ###???
		if (FAILED(hr))
		{
			D_API_ERR(0, TEXT("WriteFileInSmallBuffer"), hr, TEXT("small buffer size write %d"));
		}
	}

	return hr;
}

HRESULT CFileDeviceHandle::GetFileSize( LONGLONG * pFileSize )
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bRC = GetFileSizeEx(
		m_Handle,//_In_   HANDLE hFile,
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

VOID CFileDeviceHandle::InitParams( HANDLE Handle, BOOL bAttach )
{
	m_Handle = Handle;
	m_bAttacch = bAttach;
}

HANDLE CFileDeviceHandle::GetRawHandle()
{
	return m_Handle;
}

HRESULT CreateInstanceCFileDeviceHandle(
	IN HANDLE Handle, 
	IN BOOL bAttach,
	OUT IFileDevice ** ppFileDevice, 
	BOOL bCreateMember
	)
{
	return CREATE_INSTCANCE_COBJECT(new CFileDeviceHandle(Handle, bAttach), ppFileDevice, bCreateMember);
}