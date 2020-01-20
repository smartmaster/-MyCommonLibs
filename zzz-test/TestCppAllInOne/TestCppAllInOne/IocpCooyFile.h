#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <intsafe.h>

#include <assert.h>

#include "CodeBlock.h"
#include "log.h"


namespace SmartLib
{
	enum class IocpState
	{
		Unknown,
		READ,
		WRITE,
		STOP
	};




	class CIocpCopyFile
	{
	private:
		CString _srcFileName;
		CString _destFileName;

		HANDLE _src{ INVALID_HANDLE_VALUE };
		HANDLE _dest{ INVALID_HANDLE_VALUE };
		HANDLE _iocp{ NULL };

		IocpState _state{IocpState::Unknown};
		OVERLAPPED _ov{ 0 };

		LPBYTE _buffer{ nullptr };
		int _bufferLen{ 0 };

		DWORD _readLen{ 0 };
		DWORD _writeLen{ 0 };

		LONGLONG _offset{ 0 };

	public:
		CIocpCopyFile(LPCTSTR srcFileName, LPCTSTR destFileName, int bufferLen) :
			_srcFileName(srcFileName),
			_destFileName(destFileName),
			_bufferLen(bufferLen)
		{
			_buffer = (LPBYTE)::VirtualAlloc(
				nullptr,//_In_opt_  LPVOID lpAddress,
				_bufferLen,//_In_      SIZE_T dwSize,
				MEM_COMMIT | MEM_RESERVE,//_In_      DWORD flAllocationType,
				PAGE_READWRITE//,//_In_      DWORD flProtect
			);
		}

		~CIocpCopyFile()
		{
			if (INVALID_HANDLE_VALUE != _src)
			{
				::CloseHandle(_src);
				_src = INVALID_HANDLE_VALUE;
			}

			if (INVALID_HANDLE_VALUE != _dest)
			{
				::CloseHandle(_dest);
				_dest = INVALID_HANDLE_VALUE;
			}


			if (nullptr != _iocp)
			{
				::CloseHandle(_iocp);
				_iocp = nullptr;
			}

			if (_buffer)
			{
				::VirtualFree(
					_buffer,//_In_  LPVOID lpAddress,
					0,//_In_  SIZE_T dwSize,
					MEM_RELEASE//,//_In_  DWORD dwFreeType
				);
				_buffer = nullptr;
			}
		}

	private:
		HRESULT OnReadCompletion(DWORD NumberOfBytesTransferred)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);

			DWORD divider = 2; //to simulate partial buffer written

			//////////////////////////////////////////////////////////////////////////
			_readLen = NumberOfBytesTransferred;
			DWORD NumberOfBytesWritten = 0;
			_state = IocpState::WRITE;
			ZeroMemory(&_ov, sizeof(_ov));
			_ov.Offset = LODWORD(_offset);
			_ov.OffsetHigh = HIDWORD(_offset);
			BOOL bwr = ::WriteFile(
				_dest,//_In_         HANDLE hFile,
				_buffer,//_In_         LPCVOID lpBuffer,
				NumberOfBytesTransferred >= divider ? NumberOfBytesTransferred / divider : NumberOfBytesTransferred,//_In_         DWORD nNumberOfBytesToWrite,
				&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
				&_ov//_Inout_opt_  LPOVERLAPPED lpOverlapped
			);
			if (!bwr)
			{
				LastError = ::GetLastError();
				if (ERROR_IO_PENDING != LastError)
				{
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("Failed to WriteFile. [FileName=%s, hr=0x%08x, LastError=%d]."), _destFileName.GetString(), hr, LastError);
					LEAVE_BLOCK(0);
				}
			}
			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

		DWORD OnWriteCompletion(DWORD NumberOfBytesTransferred)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_writeLen += NumberOfBytesTransferred;
			_offset += NumberOfBytesTransferred;

			if (_writeLen == _readLen) //fully wrirte
			{
				_writeLen = 0;
				_readLen = 0;

				_state = IocpState::READ;
				ZeroMemory(&_ov, sizeof(_ov));
				_ov.Offset = LODWORD(_offset);
				_ov.OffsetHigh = HIDWORD(_offset);
				DWORD NumberOfBytesRead = 0;

				BOOL bRead = ::ReadFile(
					_src,//_In_         HANDLE hFile,
					_buffer,//_Out_        LPVOID lpBuffer,
					_bufferLen,//_In_         DWORD nNumberOfBytesToRead,
					&NumberOfBytesRead,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
					&_ov//_Inout_opt_  LPOVERLAPPED lpOverlapped
				);
				if (!bRead)
				{
					LastError = ::GetLastError();
					if (ERROR_IO_PENDING != LastError)
					{
						hr = HRESULT_FROM_WIN32(LastError);
						SML_LOG_LINE(TEXT("Failed to read file. [FileName=%s, hr=0x%08x, LastError=%d]."), _srcFileName.GetString(), hr, LastError);
						LEAVE_BLOCK(0);
					}
				}
			}
			else
			{
				DWORD NumberOfBytesWritten = 0; //partially write, write remaining
				_state = IocpState::WRITE;
				ZeroMemory(&_ov, sizeof(_ov));
				_ov.Offset = LODWORD(_offset);
				_ov.OffsetHigh = HIDWORD(_offset);
				BOOL bwr = ::WriteFile(
					_dest,//_In_         HANDLE hFile,
					_buffer + _writeLen,//_In_         LPCVOID lpBuffer,
					_readLen - _writeLen,//_In_         DWORD nNumberOfBytesToWrite,
					&NumberOfBytesWritten,//_Out_opt_    LPDWORD lpNumberOfBytesWritten,
					&_ov//_Inout_opt_  LPOVERLAPPED lpOverlapped
				);
				if (!bwr)
				{
					LastError = ::GetLastError();
					if (ERROR_IO_PENDING != LastError)
					{
						hr = HRESULT_FROM_WIN32(LastError);
						SML_LOG_LINE(TEXT("Failed to WriteFile. [FileName=%s, hr=0x%08x, LastError=%d]."), _destFileName.GetString(), hr, LastError);
						LEAVE_BLOCK(0);
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}


	public:
		HRESULT Copy()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_src = ::CreateFile(
				_srcFileName.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_READ,//_In_      DWORD dwDesiredAccess,
				FILE_SHARE_READ,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _src)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("CreateFile failed. [file, hr, error]=[%s, 0x%08x, %d]"), _srcFileName.GetString(), hr, LastError);
				LEAVE_BLOCK(0);
			}


			_dest = ::CreateFile(
				_destFileName.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
				0,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _dest)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("CreateFile failed. [file, hr, error]=[%s, 0x%08x, %d]"), _destFileName.GetString(), hr, LastError);
				LEAVE_BLOCK(0);
			}

			const DWORD NumberOfConcurrentThreads = 2;
			_iocp = ::CreateIoCompletionPort(
				INVALID_HANDLE_VALUE,//_In_      HANDLE FileHandle,
				nullptr,//_In_opt_  HANDLE ExistingCompletionPort,
				0,//_In_      ULONG_PTR CompletionKey,
				NumberOfConcurrentThreads//_In_      DWORD NumberOfConcurrentThreads
			);
			if (nullptr == _iocp)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("CreateIoCompletionPort failed. [hr, error]=[0x%08x, %d]"), hr, LastError);
				LEAVE_BLOCK(0);
			}



			HANDLE iocpTemp = ::CreateIoCompletionPort(
				_src,//_In_      HANDLE FileHandle,
				_iocp,//_In_opt_  HANDLE ExistingCompletionPort,
				(ULONG_PTR)(this),//_In_      ULONG_PTR CompletionKey,
				0//_In_      DWORD NumberOfConcurrentThreads
			);
			if (nullptr == iocpTemp)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("CreateIoCompletionPort failed to associate file. [hr, error]=[0x%08x, %d]"), hr, LastError);
				LEAVE_BLOCK(0);
			}


			iocpTemp = ::CreateIoCompletionPort(
				_dest,//_In_      HANDLE FileHandle,
				_iocp,//_In_opt_  HANDLE ExistingCompletionPort,
				(ULONG_PTR)(this),//_In_      ULONG_PTR CompletionKey,
				0//_In_      DWORD NumberOfConcurrentThreads
			);
			if (nullptr == iocpTemp)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("CreateIoCompletionPort failed to associate file. [hr, error]=[0x%08x, %d]"), hr, LastError);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			LONGLONG offset = 0;
			ZeroMemory(&_ov, sizeof(_ov));
			DWORD NumberOfBytesRead = 0;

			_state = IocpState::READ;
			BOOL bRead = ::ReadFile(
				_src,//_In_         HANDLE hFile,
				_buffer,//_Out_        LPVOID lpBuffer,
				_bufferLen,//_In_         DWORD nNumberOfBytesToRead,
				&NumberOfBytesRead,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
				&_ov//_Inout_opt_  LPOVERLAPPED lpOverlapped
			);
			if (!bRead)
			{
				LastError = ::GetLastError();
				if (ERROR_IO_PENDING != LastError)
				{
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("Failed to read file. [FileName=%s, hr=0x%08x, LastError=%d]."), _srcFileName.GetString(), hr, LastError);
					LEAVE_BLOCK(0);
				}
			}

			for (;;)
			{
				DWORD NumberOfBytesTransferred = 0;
				CIocpCopyFile * CompletionKey = nullptr;
				LPOVERLAPPED pov = nullptr;
				BOOL bgolr = ::GetQueuedCompletionStatus(
					_iocp,//_In_   HANDLE CompletionPort,
					&NumberOfBytesTransferred,//_Out_  LPDWORD lpNumberOfBytes,
					(PULONG_PTR)&CompletionKey,//_Out_  PULONG_PTR lpCompletionKey,
					&pov,//_Out_  LPOVERLAPPED *lpOverlapped,
					INFINITE//_In_   DWORD dwMilliseconds
				);
				if (!bgolr)
				{
					LastError = ::GetLastError(); 
					if (ERROR_HANDLE_EOF != LastError)
					{
						hr = HRESULT_FROM_WIN32(LastError);
						SML_LOG_LINE(TEXT("GetQueuedCompletionStatus. [hr, error]=[0x%08x, %d]"), hr, LastError);
					}
					else
					{
						SML_LOG_LINE(TEXT("Reached end of file, stop copying. [hr, error]=[0x%08x, %d]"), hr, LastError);
					}
					
					break;
				}

				assert(this == CompletionKey);
				assert(&CompletionKey->_ov == pov);

				if (0 == NumberOfBytesTransferred)
				{
					SML_LOG_LINE(TEXT("No more data to be processed."));
					break;
				}

				switch (_state)
				{
				case IocpState::READ:
				{
					hr = OnReadCompletion(NumberOfBytesTransferred);
				}
				break;

				case IocpState::WRITE:
				{
					hr = OnWriteCompletion(NumberOfBytesTransferred);
				}
				break;
				}
				
			}
			if (FAILED(hr))
			{
				LEAVE_BLOCK(0);
			}
			::FlushFileBuffers(_dest);


			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			
			return hr;
		}

		
	};

	class CIocpCopyFileTest
	{
	public:
		static void Case0(LPCTSTR srcName, LPCTSTR destName)
		{
			const int BUFFER_LEN = 1024 * 1024;
			CIocpCopyFile cf(srcName, destName, BUFFER_LEN);
			HRESULT hr = cf.Copy();
		}
	};

}