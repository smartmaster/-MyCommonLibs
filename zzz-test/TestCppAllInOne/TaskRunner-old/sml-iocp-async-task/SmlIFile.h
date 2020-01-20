#pragma once

#include <Winsock2.h>

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "../SmlRefPtr.h"

namespace SmartLib
{
	class IFile
	{
	public:
		virtual ~IFile() {}


		virtual BOOL WINAPI ReadFile(
			_Out_        LPVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToRead,
			_Out_opt_    LPDWORD lpNumberOfBytesRead,
			_Inout_opt_  LPOVERLAPPED lpOverlapped
		) = 0;

		virtual BOOL WINAPI WriteFile(
			_In_         LPCVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToWrite,
			_Out_opt_    LPDWORD lpNumberOfBytesWritten,
			_Inout_opt_  LPOVERLAPPED lpOverlapped
		) = 0;

		virtual HANDLE NativeHandle() = 0;

		virtual DWORD GetLastError() = 0;
	};

	class Win32File : public IFile
	{
	private:
		HANDLE _file{ INVALID_HANDLE_VALUE }; /***INCOMMING***//***INTERNAL***//***CLEANUP***/
		ScopeResource<> _sr_file{ [this]()
		{
			if (INVALID_HANDLE_VALUE != _file)
			{
				::CloseHandle(_file);
				_file = INVALID_HANDLE_VALUE;
			}
		} };

	public:
		virtual ~Win32File() override {}


		Win32File(HANDLE file) :
			_file{ file }
		{
		}


		virtual BOOL WINAPI ReadFile(
			_Out_        LPVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToRead,
			_Out_opt_    LPDWORD lpNumberOfBytesRead,
			_Inout_opt_  LPOVERLAPPED lpOverlapped
		) override
		{
			return ::ReadFile(
				_file,
				lpBuffer,
				nNumberOfBytesToRead,
				lpNumberOfBytesRead,
				lpOverlapped
			);
		}

		virtual BOOL WINAPI WriteFile(
			_In_         LPCVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToWrite,
			_Out_opt_    LPDWORD lpNumberOfBytesWritten,
			_Inout_opt_  LPOVERLAPPED lpOverlapped
		) override
		{
			return ::WriteFile(
				_file,
				lpBuffer,
				nNumberOfBytesToWrite,
				lpNumberOfBytesWritten,
				lpOverlapped
			);
		}


		virtual HANDLE NativeHandle() override
		{
			return _file;
		}


		virtual DWORD GetLastError() override
		{
			return ::GetLastError();
		}
	};


	class SocketFile : public IFile
	{
	private:
		SOCKET _socket{ INVALID_SOCKET };/***INTERNAL***//***INCOMMING***//***CLEANUP***/
		ScopeResource<> _sr_socket{ [this]()
		{
			if (INVALID_SOCKET != _socket)
			{
				::closesocket(_socket);
				_socket = INVALID_SOCKET;
			}
		} };

	public:
		virtual ~SocketFile() override {}


		SocketFile(SOCKET sock) :
			_socket{ sock }
		{
		}



		virtual BOOL WINAPI ReadFile(
			_Out_        LPVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToRead,
			_Out_opt_    LPDWORD lpNumberOfBytesRead,
			_Inout_opt_  LPOVERLAPPED lpOverlapped
		) override
		{
			WSABUF Buffer{ 0 };
			Buffer.len = nNumberOfBytesToRead;
			Buffer.buf = (char*)lpBuffer;

			DWORD Flags = 0;
			int result = ::WSARecv(
				_socket,//_In_     SOCKET s,
				&Buffer,//_Inout_  LPWSABUF lpBuffers,
				1,//_In_     DWORD dwBufferCount,
				lpNumberOfBytesRead,//_Out_    LPDWORD lpNumberOfBytesRecvd,
				&Flags,//_Inout_  LPDWORD lpFlags,
				lpOverlapped,//_In_     LPWSAOVERLAPPED lpOverlapped,
				nullptr//,//_In_     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);

			return 0 == result;
		}

		virtual BOOL WINAPI WriteFile(
			_In_         LPCVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToWrite,
			_Out_opt_    LPDWORD lpNumberOfBytesWritten,
			_Inout_opt_  LPOVERLAPPED lpOverlapped
		) override
		{
			WSABUF Buffer{ 0 };
			Buffer.len = nNumberOfBytesToWrite;
			Buffer.buf = (char*)lpBuffer;

			DWORD dwFlags = 0;
			int result = ::WSASend(
				_socket,//_In_   SOCKET s,
				&Buffer,//_In_   LPWSABUF lpBuffers,
				1,//_In_   DWORD dwBufferCount,
				lpNumberOfBytesWritten,//_Out_  LPDWORD lpNumberOfBytesSent,
				dwFlags,//_In_   DWORD dwFlags,
				lpOverlapped,//_In_   LPWSAOVERLAPPED lpOverlapped,
				nullptr//,//_In_   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);

			return 0 == result;
		}


		virtual HANDLE NativeHandle() override
		{
			return (HANDLE)(_socket);
		}


		virtual DWORD GetLastError() override
		{
			return ::WSAGetLastError();
		}
	};

}