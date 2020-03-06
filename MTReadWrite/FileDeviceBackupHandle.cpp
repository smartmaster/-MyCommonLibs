#include "stdafx.h"

#include "..\PublicHeader\PublicDTraceLib.h"
#include "FileDeviceBackupHandle.h"


CFileDeviceBackupHandle::CFileDeviceBackupHandle(HANDLE Handle, BOOL bAttach, E_BACKUP_RW BACKUP_RW) :
m_Handle(Handle),
m_RefCount(0),
m_bAttacch(bAttach),
m_pContext(nullptr),
m_BACKUP_RW(BACKUP_RW)
{

}

VOID CFileDeviceBackupHandle::InitParams(HANDLE Handle, BOOL bAttach)
{
	m_Handle = Handle;
	m_bAttacch = bAttach;
}

HRESULT CFileDeviceBackupHandle::CreateMember()
{
	return S_OK;
}

HRESULT CFileDeviceBackupHandle::DestroyMember()
{
	HRESULT hr = S_OK;
	DWORD LastError = 0;

	if (m_pContext != nullptr && m_pContext != INVALID_HANDLE_VALUE)
	{
		BOOL bRC = FALSE;
		if (m_BACKUP_RW == E_BACKUP_RW::READ)
		{
			bRC = BackupRead(
				m_Handle,//_In_   HANDLE hFile,
				nullptr,//_Out_  LPBYTE lpBuffer,
				0,//_In_   DWORD nNumberOfBytesToRead,
				0,//_Out_  LPDWORD lpNumberOfBytesRead,
				TRUE,//_In_   BOOL bAbort,
				FALSE,//_In_   BOOL bProcessSecurity,
				&m_pContext//_Out_  LPVOID *lpContext
				);
			if (!bRC)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("BackupRead"), hr, TEXT(""));
			}
		}
		else
		{
			bRC = BackupWrite(
				m_Handle,//_In_   HANDLE hFile,
				nullptr,//_Out_  LPBYTE lpBuffer,
				0,//_In_   DWORD nNumberOfBytesToRead,
				0,//_Out_  LPDWORD lpNumberOfBytesRead,
				TRUE,//_In_   BOOL bAbort,
				FALSE,//_In_   BOOL bProcessSecurity,
				&m_pContext//_Out_  LPVOID *lpContext
				);
			if (!bRC)
			{
				LastError = GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				D_API_ERR(0, TEXT("BackupWrite"), hr, TEXT(""));
			}
		}
	}

	if (m_bAttacch)
	{
		CloseHandle(m_Handle);
		m_Handle = INVALID_HANDLE_VALUE;
		m_bAttacch = FALSE;
	}

	return S_OK;
}

HRESULT CFileDeviceBackupHandle::SetPointerEx(LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod)
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	LARGE_INTEGER liDistanceToMove;
	liDistanceToMove.QuadPart = llDistanceToMove;
	LARGE_INTEGER liNewFilePointer = { 0 };
	BOOL bRC = BackupSeek(
		m_Handle,//_In_   HANDLE hFile,
		liDistanceToMove.LowPart,//_In_   DWORD dwLowBytesToSeek,
		liDistanceToMove.HighPart,//_In_   DWORD dwHighBytesToSeek,
		&liNewFilePointer.LowPart,//_Out_  LPDWORD lpdwLowByteSeeked,
		(LPDWORD)(&liNewFilePointer.HighPart),//_Out_  LPDWORD lpdwHighByteSeeked,
		&m_pContext//_In_   LPVOID *lpContext
		);
	if (!bRC)
	{
		dwLastError = GetLastError();
		D_SET_LAST_STATUS(dwLastError, -1);
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_API_ERR(0, TEXT("BackupSeek"), dwLastError, TEXT("[0x%p, %I64d, %u]"), m_Handle, liDistanceToMove.QuadPart, ulMoveMethod);
	}
	else
	{
		*lpNewFilePointer = liNewFilePointer.QuadPart;
	}

	return hr;
}

HRESULT CFileDeviceBackupHandle::Read(ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer)
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bRet = ::BackupRead(
		m_Handle,//_In_   HANDLE hFile,
		pBuffer,//_Out_  LPBYTE lpBuffer,
		nNumberOfBytesToRead,//_In_   DWORD nNumberOfBytesToRead,
		lpNumberOfBytesRead,//_Out_  LPDWORD lpNumberOfBytesRead,
		FALSE,//_In_   BOOL bAbort,
		TRUE,//_In_   BOOL bProcessSecurity,
		&m_pContext//_Out_  LPVOID *lpContext
		);
	if (!bRet)
	{
		dwLastError = GetLastError();
		D_SET_LAST_STATUS(dwLastError, -1);
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_API_ERR(0, TEXT("BackupRead"), hr, TEXT("[%u, %u]"), nNumberOfBytesToRead, *lpNumberOfBytesRead);
	}

	if (!bRet || 0 == *lpNumberOfBytesRead)
	{
		BOOL bRCTmp = BackupRead(
			m_Handle,//_In_   HANDLE hFile,
			nullptr,//_Out_  LPBYTE lpBuffer,
			0,//_In_   DWORD nNumberOfBytesToRead,
			0,//_Out_  LPDWORD lpNumberOfBytesRead,
			TRUE,//_In_   BOOL bAbort,
			FALSE,//_In_   BOOL bProcessSecurity,
			&m_pContext//_Out_  LPVOID *lpContext
			);
		if (!bRCTmp)
		{
			DWORD LastErrorTmp = GetLastError();
			HRESULT hrTmp = HRESULT_FROM_WIN32(LastErrorTmp);
			if (0 == *lpNumberOfBytesRead)
			{
				hr = hrTmp;
			}
			D_API_ERR(0, TEXT("BackupRead"), hrTmp, TEXT(""));
		}
	}

	return hr;
}

HRESULT CFileDeviceBackupHandle::Write(ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer)
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	BOOL bRet = ::BackupWrite(
		m_Handle,//_In_         HANDLE hFile,
		(LPBYTE)pBuffer,//_In_         LPCVOID lpBuffer,
		nNumberOfBytesToWrite,//_In_         DWORD nNumberOfBytesToWrite,
		lpNumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
		FALSE,
		TRUE,
		&m_pContext//_Inout_opt_  LPOVERLAPPED lpOverlapped
		);
	if (!bRet)
	{
		dwLastError = GetLastError();
		D_SET_LAST_STATUS(dwLastError, -1);
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_API_ERR(0, TEXT("BackupWrite"), hr, TEXT("[%u, %u]"), nNumberOfBytesToWrite, *lpNumberOfBytesWritten);
	}


	if (!bRet || 0 == nNumberOfBytesToWrite)
	{
		BOOL bRCTmp = BackupWrite(
			m_Handle,//_In_   HANDLE hFile,
			nullptr,//_Out_  LPBYTE lpBuffer,
			0,//_In_   DWORD nNumberOfBytesToRead,
			0,//_Out_  LPDWORD lpNumberOfBytesRead,
			TRUE,//_In_   BOOL bAbort,
			FALSE,//_In_   BOOL bProcessSecurity,
			&m_pContext//_Out_  LPVOID *lpContext
			);
		if (!bRCTmp)
		{
			DWORD LastErrorTmp = GetLastError();
			HRESULT hrTmp = HRESULT_FROM_WIN32(LastErrorTmp);
			if (0 == nNumberOfBytesToWrite)
			{
				hr = hrTmp;
			}
			D_API_ERR(0, TEXT("BackupWrite"), hrTmp, TEXT(""));
		}
	}

	return hr;
}

HRESULT CFileDeviceBackupHandle::GetFileSize(LONGLONG * pFileSize)
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

HANDLE CFileDeviceBackupHandle::GetRawHandle()
{
	return m_Handle;
}




HRESULT CreateInstanceCFileDeviceBackupHandle(
	IN HANDLE Handle,
	IN BOOL bAttach,
	IN E_BACKUP_RW BACKUP_RW,
	OUT IFileDevice ** ppFileDevice,
	BOOL bCreateMember
	)
{
	return CREATE_INSTCANCE_COBJECT(new CFileDeviceBackupHandle(Handle, bAttach, BACKUP_RW), ppFileDevice, bCreateMember);
}