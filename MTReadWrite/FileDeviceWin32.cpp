#include "stdafx.h"
#include "..\PublicHeader\PublicCodeBlock.h"
#include "..\PublicHeader\PublicDTraceLib.h"
#include "FileDeviceWin32.h"



CFileDeviceWin32::CFileDeviceWin32( IN LPCTSTR strFileName, IN ULONG ulDesiredAccess, IN ULONG ulShareMode, IN ULONG ulCreationDisposition, IN ULONG ulFlagsAndAttributes ) :
m_strFileName(strFileName),
	m_ulDesiredAccess(ulDesiredAccess),
	m_ulShareMode(ulShareMode),
	m_ulCreationDisposition(ulCreationDisposition),
	m_ulFlagsAndAttributes(ulFlagsAndAttributes),
	m_FileDeviceImp(INVALID_HANDLE_VALUE, FALSE),
	m_RefCount(0)
{

}


HRESULT CFileDeviceWin32::CreateMember()
{
	D_START(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	HANDLE hFileInternal = ::CreateFile(
		m_strFileName,//__in      LPCTSTR lpFileName,
		m_ulDesiredAccess,//__in      DWORD dwDesiredAccess,
		m_ulShareMode,//__in      DWORD dwShareMode,
		NULL,//__in_opt  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		m_ulCreationDisposition,//__in      DWORD dwCreationDisposition,
		m_ulFlagsAndAttributes,//__in      DWORD dwFlagsAndAttributes,
		NULL//__in_opt  HANDLE hTemplateFile
		);
	if (INVALID_HANDLE_VALUE == hFileInternal)
	{
		dwLastError = GetLastError();
		D_SET_LAST_STATUS(dwLastError, -1);
		hr = HRESULT_FROM_WIN32(dwLastError);
		D_API_ERR(0, TEXT("CreateFile"), dwLastError, TEXT("[%s]"), m_strFileName.GetString());
	}

	m_FileDeviceImp.InitParams(hFileInternal, TRUE);
	hFileInternal = INVALID_HANDLE_VALUE;

	return hr;
}

HRESULT CFileDeviceWin32::DestroyMember()
{
	m_FileDeviceImp.DestroyMember();
	return S_OK;
}

HRESULT CFileDeviceWin32::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
{
	return m_FileDeviceImp.SetPointerEx( llDistanceToMove, lpNewFilePointer, ulMoveMethod );
}

HRESULT CFileDeviceWin32::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	return m_FileDeviceImp.Read( nNumberOfBytesToRead, lpNumberOfBytesRead, pBuffer );
}

HRESULT CFileDeviceWin32::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, CONST BYTE* pBuffer )
{
	return m_FileDeviceImp.Write( nNumberOfBytesToWrite, lpNumberOfBytesWritten, pBuffer );
}

HRESULT CFileDeviceWin32::GetFileSize( LONGLONG * pFileSize )
{
	return m_FileDeviceImp.GetFileSize( pFileSize );
}

HANDLE CFileDeviceWin32::GetRawHandle()
{
	return m_FileDeviceImp.GetRawHandle();
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
	return CREATE_INSTCANCE_COBJECT(/*CFileDeviceWin32,*/  
						(new CFileDeviceWin32(
						strFileName, 
						ulDesiredAccess, 
						ulShareMode, 
						ulCreationDisposition, 
						ulFlagsAndAttributes)),
						ppFileDevice,
						bCreateMember);
}