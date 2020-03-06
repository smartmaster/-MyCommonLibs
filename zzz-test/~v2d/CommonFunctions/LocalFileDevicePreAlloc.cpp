#include "stdafx.h"
#include "LocalFileDevicePreAlloc.h"


CLocalFileDevicePreAlloc::CLocalFileDevicePreAlloc( LONGLONG Delta )
{
	InitMember();
	m_Delata = Delta;
	CONST ULONG __8M = 8*1024*204;
	if (m_Delata < __8M)
	{
		m_Delata = __8M;
	}
}

VOID CLocalFileDevicePreAlloc::InitMember()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_CurPointer = 0;
	m_MaxDataSize = 0;
	m_AllocatedSize = 0;
	m_Delata = 0;
	m_dwFlagsAndAttributeDest = 0; //<mycode> 2011-4-26
}

HRESULT CLocalFileDevicePreAlloc::DestroyMember()
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
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetFilePointerEx"), hr, TEXT("ToMove=%I64d, NewPointer=%I64d"), liToMove.QuadPart, liNewPointer.QuadPart));
			//break;
		}

		//////////////////////////////////////////////////////////////////////////
		BOOL bSEF = ::SetEndOfFile(m_hFile);
		if (!bSEF)
		{
			dwLastError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DSETLASTSTATUS(hr);
			_OLD_DAPIERR((0, DRNPOS, TEXT("SetEndOfFile"), hr));
			//break;
		}

		::CloseHandle(m_hFile); m_hFile = INVALID_HANDLE_VALUE;
		m_strFileName.Empty();
	}

	return hr;
}

CLocalFileDevicePreAlloc::~CLocalFileDevicePreAlloc()
{
	DestroyMember();
}

HRESULT STDMETHODCALLTYPE CLocalFileDevicePreAlloc::QueryInterface( REFIID riid, void **ppvObject )
{
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CLocalFileDevicePreAlloc::AddRef( void )
{
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CLocalFileDevicePreAlloc::Release( void )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CLocalFileDevicePreAlloc::Create( BSTR bstrFileName, ULONG ulDesiredAccess, ULONG ulShareMode, ULONG ulCreationDisposition, ULONG ulFlagsAndAttributes )
{
	// TODO: Add your implementation code here
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
		m_dwFlagsAndAttributeDest = ulFlagsAndAttributes; //<mycode> 2011-4-26
	}

	return hr;
}

HRESULT STDMETHODCALLTYPE CLocalFileDevicePreAlloc::SetPointerEx( LONGLONG llDistanceToMove, LONGLONG* lpNewFilePointer, ULONG ulMoveMethod )
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
		_OLD_DSETLASTSTATUS(dwLastError);
		hr = HRESULT_FROM_WIN32(dwLastError);
		_OLD_DAPIERR((0, DRNPOS, TEXT("SetFilePointerEx"), dwLastError, TEXT("[%s, %I64d, %u]"), m_strFileName.GetString(), liDistanceToMove.QuadPart, ulMoveMethod));
	}
	else
	{
		*lpNewFilePointer = liNewFilePointer.QuadPart;
		m_CurPointer = liNewFilePointer.QuadPart; //<mycode> 2011-4-24
	}

	return hr;
}

HRESULT STDMETHODCALLTYPE CLocalFileDevicePreAlloc::Read( ULONG nNumberOfBytesToRead, ULONG* lpNumberOfBytesRead, BYTE* pBuffer )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CLocalFileDevicePreAlloc::Write( ULONG nNumberOfBytesToWrite, ULONG* lpNumberOfBytesWritten, BYTE* pBuffer )
{
	// TODO: Add your implementation code here
	DSTART(999);

	HRESULT hr = S_OK;
	DWORD dwLastError = 0;

	do 
	{
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
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("SetFilePointerEx"), hr, TEXT("ToMove=%I64d, NewPointer=%I64d"), liToMove.QuadPart, liNewPointer.QuadPart));
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			BOOL bSEF = ::SetEndOfFile(m_hFile);
			if (!bSEF)
			{
				dwLastError = GetLastError();
				hr = HRESULT_FROM_WIN32(dwLastError);
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("SetEndOfFile"), hr));
				break;
			}


			//////////////////////////////////////////////////////////////////////////
			liToMove.QuadPart = m_CurPointer; //reset for write position
			liNewPointer.QuadPart = 0;
			bSFP = ::SetFilePointerEx(m_hFile, liToMove, &liNewPointer, FILE_BEGIN);
			if (!bSFP)
			{
				dwLastError = GetLastError();
				hr = HRESULT_FROM_WIN32(dwLastError);
				_OLD_DSETLASTSTATUS(hr);
				_OLD_DAPIERR((0, DRNPOS, TEXT("SetFilePointerEx"), hr, TEXT("ToMove=%I64d, NewPointer=%I64d"), liToMove.QuadPart, liNewPointer.QuadPart));
				break;
			}
		}

		CONST ULONG SECTOR_SIZE = 512; //<mycode> 2011-4-26
		ULONG ulPad = 0;
		if (m_dwFlagsAndAttributeDest & FILE_FLAG_NO_BUFFERING)
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
			_OLD_DSETLASTSTATUS(dwLastError);
			hr  = HRESULT_FROM_WIN32(dwLastError);
			_OLD_DAPIERR((0, DRNPOS, TEXT("WriteFile"), dwLastError, TEXT("[%u, %u]"), nNumberOfBytesToWrite, *lpNumberOfBytesWritten));
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		m_CurPointer += (*lpNumberOfBytesWritten);
		if (m_CurPointer > m_MaxDataSize)
		{
			m_MaxDataSize = m_CurPointer/* - ulPad*/; //<mycode> 2011-4-26
		}

	} while (FALSE);

	return hr;
}

HRESULT STDMETHODCALLTYPE CLocalFileDevicePreAlloc::Close( void )
{
	// TODO: Add your implementation code here
	DSTART(999);
	return DestroyMember();
}

HRESULT STDMETHODCALLTYPE CLocalFileDevicePreAlloc::GetFileSizeEx( LONGLONG* pllFileSize )
{
	return E_NOTIMPL;
}
