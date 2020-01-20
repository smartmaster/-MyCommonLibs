#pragma once

#include <Winsock2.h>
#include <Winsock.h>
#include <Ws2tcpip.h>
#include <mswsock.h>

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <cstdlib>
#include <ctime>

#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

#include "CodeBlock.h"
#include "log.h"

#include "../TaskRunner/SmlCircularBuffer.h"
#include "../TaskRunner/SmlVector.h"
#include "../TaskRunner/SmlRefPtr.h"
#include "../TaskRunner/SmlMTQueue.h"


namespace SmartLib
{
	using namespace ::std;

	//////////////////////////////////////////////////////////////////////////
	struct CBDataHeader
	{
		long version;
		long flag;
		long command;
		long dataLen;
		LARGE_INTEGER offset;
		LARGE_INTEGER source;
		LARGE_INTEGER destination;

		CBDataHeader()
		{
			ZeroMemory(this, sizeof(CBDataHeader));
		}
	};

	//////////////////////////////////////////////////////////////////////////
	class IAsyncTask
	{
	public:
		virtual ~IAsyncTask() {};

		virtual HRESULT Create() = 0;
		virtual HRESULT Next(long transffered, LPOVERLAPPED lpoverlapped) = 0;
		virtual HRESULT Error(long error, LPOVERLAPPED lpoverlapped) = 0;
		virtual HRESULT Wait() = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	class SimpleEvent
	{
	private:
		volatile bool _signaled{ false };
		mutex _mutex;
		condition_variable _cv;

	public:
		void Reset()
		{
			unique_lock<mutex> lock{ _mutex };
			_signaled = false;
		}

		void Set()
		{
			unique_lock<mutex> lock{ _mutex };
			_signaled = true;
			_cv.notify_all();
		}

		void Wait()
		{
			unique_lock<mutex> lock{ _mutex };
			while (!_signaled)
			{
				_cv.wait(lock);
			}
		}
	};


	//////////////////////////////////////////////////////////////////////////
	class TaskRunner
	{
	private:
		HANDLE _iocp; /***INTERNAL***/ /***CLEANUP***/
		ScopeResource<function<void()>> _scope_iocp{ [this]() 
		{
			if (nullptr != _iocp)
			{
				::CloseHandle(_iocp);
				_iocp = nullptr;
			}
		} };

		Vector<RefPtr<thread>, true, 8> _threads; /***INTERNAL***/
		RefPtr<thread> _cleanupThread; /***INTERNAL***/
		MTQueue<RefPtr<IAsyncTask*>, 32> _cleanupQueue; /***INTERNAL***/

	private:

		HRESULT Cleanup()
		{
			for (;;)
			{
				RefPtr<IAsyncTask*> pptask;
				long rr = _cleanupQueue.pop_front(pptask);
				if (rr < 0)
				{
					break;
				}

				(**pptask).Wait();
				pptask.Attach(nullptr); //delete
			}
						
		}

		//////////////////////////////////////////////////////////////////////////
		HRESULT RunTasks()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			for (;;)
			{
				//this_thread::sleep_for(chrono::milliseconds(rand()%50 + 50));
				DWORD NumberOfBytes{ 0 };
				ULONG_PTR CompletionKey{ 0 };
				LPOVERLAPPED pOverlapped{ nullptr };
				BOOL rr = ::GetQueuedCompletionStatus(
					_iocp,//_In_   HANDLE CompletionPort,
					&NumberOfBytes,//_Out_  LPDWORD lpNumberOfBytes,
					&CompletionKey,//_Out_  PULONG_PTR lpCompletionKey,
					&pOverlapped,//_Out_  LPOVERLAPPED * lpOverlapped,
					INFINITE//,//_In_   DWORD dwMilliseconds
				);

				if (!rr)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					if (ERROR_HANDLE_EOF != LastError)
					{
						SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("GetQueuedCompletionStatus"), LastError, hr);
						break;
					}
					else
					{
						IAsyncTask* task = (IAsyncTask*)(CompletionKey);
						if (nullptr == task)
						{
							break;
						}
						task->Error(LastError, pOverlapped);
					}
				}
				else
				{
					IAsyncTask* task = (IAsyncTask*)(CompletionKey);
					if (nullptr == task)
					{
						break;
					}
					task->Next(NumberOfBytes, pOverlapped);
				}
			}

			return hr;
		}

	public:

		TaskRunner() 
		{
		}



		HRESULT Create()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_iocp = ::CreateIoCompletionPort(
				INVALID_HANDLE_VALUE,//_In_      HANDLE FileHandle,
				nullptr,//_In_opt_  HANDLE ExistingCompletionPort,
				0,//_In_      ULONG_PTR CompletionKey,
				0//,//_In_      DWORD NumberOfConcurrentThreads
			);
			if (nullptr == _iocp)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateIoCompletionPort"), LastError, hr);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

		HANDLE Iocp()
		{
			return _iocp;
		}


		HRESULT PostTask(IAsyncTask* task, LPOVERLAPPED lpoverlapped)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			BOOL rr = ::PostQueuedCompletionStatus(
				_iocp,//_In_      HANDLE CompletionPort,
				0,//_In_      DWORD dwNumberOfBytesTransferred,
				(ULONG_PTR)(task),//_In_      ULONG_PTR dwCompletionKey,
				lpoverlapped//,//_In_opt_  LPOVERLAPPED lpOverlapped
			);
			if (!rr)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("PostQueuedCompletionStatus"), LastError, hr);
				LEAVE_BLOCK(0);
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

		void StartThreads()
		{
			SYSTEM_INFO si{ 0 };
			::GetSystemInfo(&si);


			_cleanupThread = RefPtr<thread>::Make([this] {this->Cleanup(); });

			for (long ii = 0; ii < si.dwNumberOfProcessors; ++ ii)
			{
				_threads.push_back(RefPtr<thread>::Make([this] {this->RunTasks(); }));
			}
		}


		void StopThreads()
		{
			for (long ii = 0; ii < _threads.size(); ++ii)
			{
				BOOL rr = ::PostQueuedCompletionStatus(
					_iocp,//_In_      HANDLE CompletionPort,
					0,//_In_      DWORD dwNumberOfBytesTransferred,
					0,//_In_      ULONG_PTR dwCompletionKey,
					nullptr//,//_In_opt_  LPOVERLAPPED lpOverlapped
				);
			}

			for (long ii = 0; ii < _threads.size(); ++ ii)
			{
				_threads[ii]->join();
			}


			_cleanupQueue.StopConsumer();
			_cleanupQueue.StopProvider();
			_cleanupThread->join();
		}

	};


	//////////////////////////////////////////////////////////////////////////
	class SocketIO
	{
	public:
		static BOOL WINAPI ReadFile(
			_In_         HANDLE hFile,
			_Out_        LPVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToRead,
			_Out_opt_    LPDWORD lpNumberOfBytesRead,
			_Inout_opt_  LPOVERLAPPED lpOverlapped
		)
		{
			WSABUF Buffer{ 0 };
			Buffer.len = nNumberOfBytesToRead;
			Buffer.buf = (char*)lpBuffer;

			DWORD Flags = 0;
			int result = ::WSARecv(
				(SOCKET)hFile,//_In_     SOCKET s,
				&Buffer,//_Inout_  LPWSABUF lpBuffers,
				1,//_In_     DWORD dwBufferCount,
				lpNumberOfBytesRead,//_Out_    LPDWORD lpNumberOfBytesRecvd,
				&Flags,//_Inout_  LPDWORD lpFlags,
				lpOverlapped,//_In_     LPWSAOVERLAPPED lpOverlapped,
				nullptr//,//_In_     LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);

			return 0 == result;
		}

		static BOOL WINAPI WriteFile(
			_In_         HANDLE hFile,
			_In_         LPCVOID lpBuffer,
			_In_         DWORD nNumberOfBytesToWrite,
			_Out_opt_    LPDWORD lpNumberOfBytesWritten,
			_Inout_opt_  LPOVERLAPPED lpOverlapped
		)
		{
			WSABUF Buffer{ 0 };
			Buffer.len = nNumberOfBytesToWrite;
			Buffer.buf = (char*)lpBuffer;

			DWORD dwFlags = 0;
			int result = ::WSASend(
				(SOCKET)hFile,//_In_   SOCKET s,
				&Buffer,//_In_   LPWSABUF lpBuffers,
				1,//_In_   DWORD dwBufferCount,
				lpNumberOfBytesWritten,//_Out_  LPDWORD lpNumberOfBytesSent,
				dwFlags,//_In_   DWORD dwFlags,
				lpOverlapped,//_In_   LPWSAOVERLAPPED lpOverlapped,
				nullptr//,//_In_   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);

			return 0 == result;
		}
	};

	class AsyncSocketTask : public IAsyncTask
	{
	private:
		SOCKET _socket; /***INCOMMING***/ /***INTERNAL***/ /***CLEANUP***/
		HANDLE _iocp; /***INCOMMING***/
		

		OVERLAPPED _olpReceive{ 0 }; /***INTERNAL***/
		OVERLAPPED _olpSend{ 0 }; /***INTERNAL***/

		constexpr static const long _CB_BUFFER_SIZE{ 2 * 1024 * 1024 };
		CircularBuffer _cbReceive{ _CB_BUFFER_SIZE };
		CircularBuffer _cbSend{ _CB_BUFFER_SIZE };

		constexpr static const long _IO_BUFFER_SIZE{ 64 * 1024};
		char* _receiveBuffer{ nullptr }; /***INTERNAL***//***CLEANUP***/
		char* _sendBuffer{ nullptr };/***INTERNAL***//***CLEANUP***/

		long _receiveStep{ 0 };
		long _sendStep{ 0 };


		//CBDataHeader _receiveHeader; /***INTERNAL***/
		//CBDataHeader _sendHeader; /***INTERNAL***/

		long _currentTransffered{ 0 };
		long _currentTotal{ 0 };


		SimpleEvent _event; /***INTERNAL***/

	public:
		AsyncSocketTask(SOCKET socket, HANDLE iocp) :
			_socket{ socket },
			_iocp{ iocp }
		{
		}

		virtual ~AsyncSocketTask() override
		{
			//////////////////////////////////////////////////////////////////////////
			if (_sendBuffer)
			{
				::VirtualFree(_sendBuffer, 0, MEM_RELEASE);
				_sendBuffer = nullptr;
			}


			//////////////////////////////////////////////////////////////////////////
			if (_receiveBuffer)
			{
				::VirtualFree(_receiveBuffer, 0, MEM_RELEASE);
				_receiveBuffer = nullptr;
			}

			//////////////////////////////////////////////////////////////////////////
			if (_socket != INVALID_SOCKET)
			{
				::closesocket(_socket);
				_socket = INVALID_SOCKET;
			}
		}
		virtual HRESULT Handler()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			char buffer[1024];
			long len = _cbReceive.pop_front(buffer, sizeof(buffer));
			if (len < 0)
			{
				LEAVE_BLOCK(0);
			}

			long lenSend = _cbSend.push_back_all(buffer, len);
			if (lenSend < len)
			{
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}
		virtual HRESULT Create() override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_receiveBuffer = (char*)VirtualAlloc(nullptr, _IO_BUFFER_SIZE, PAGE_READWRITE, MEM_COMMIT | MEM_RESERVE);
			if (nullptr == _receiveBuffer)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			_sendBuffer = (char*)VirtualAlloc(nullptr, _IO_BUFFER_SIZE, PAGE_READWRITE, MEM_COMMIT | MEM_RESERVE);
			if (nullptr == _sendBuffer)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), LastError, hr);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			HANDLE iocp = ::CreateIoCompletionPort(
				(HANDLE)_socket,//_In_      HANDLE FileHandle,
				_iocp,//_In_opt_  HANDLE ExistingCompletionPort,
				(ULONG_PTR)(IAsyncTask*)(this),//_In_      ULONG_PTR CompletionKey,
				0//,//_In_      DWORD NumberOfConcurrentThreads
			);
			if (nullptr == iocp)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateIoCompletionPort"), LastError, hr);
				LEAVE_BLOCK(0);
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}


		virtual HRESULT Next(long transffered, LPOVERLAPPED lpoverlapped) override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			if (lpoverlapped == &_olpReceive)
			{
			START_RECEIVE_SWICTH:
				switch (_receiveStep)
				{
					//////////////////////////////////////////////////////////////////////////
				case  0:
				{
					_receiveStep = 1;
					ZeroMemory(&_olpReceive, sizeof(OVERLAPPED));

					bool rr = SocketIO::ReadFile(
						(HANDLE)_socket,//_In_         HANDLE hFile,
						_receiveBuffer,//_Out_        LPVOID lpBuffer,
						_IO_BUFFER_SIZE,//_In_         DWORD nNumberOfBytesToRead,
						nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
						&_olpReceive//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
					);
					if (!rr)
					{
						LastError = ::WSAGetLastError();
						hr = HRESULT_FROM_WIN32(LastError);
						if (LastError != WSA_IO_PENDING)
						{
							Error(LastError, nullptr);
							SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _IO_BUFFER_SIZE, LastError, hr);
							//LEAVE_BLOCK(0);
						}
					}
				}
				break;
				//////////////////////////////////////////////////////////////////////////
				case  1:
				{
					//_receiveHeader.dataLen = transffered;
					//_cbReceive.ProvideAll((const char*)&_receiveHeader, sizeof(_receiveHeader));
					long len = _cbReceive.push_back_all(_receiveBuffer, transffered);
					if (len < transffered)
					{
						Error(0, nullptr);
					}
					else
					{
						_receiveStep = 0;
						goto START_RECEIVE_SWICTH;
					}
				}
				break;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			else if (lpoverlapped == &_olpSend)
			{
			START_WRITE_SWICTH:
				switch (_sendStep)
				{
					//////////////////////////////////////////////////////////////////////////
				case 0:
				{
					_sendStep = 1;

					_currentTransffered = 0;
					_currentTotal = _cbSend.pop_front(_sendBuffer, _IO_BUFFER_SIZE);
					if (_currentTotal < 0)
					{
						Error(0, nullptr);
						break;
					}

					ZeroMemory(&_olpSend, sizeof(OVERLAPPED));
					//len = _BUFFER_SIZE / 4; //debug!!! 
					bool rr = SocketIO::WriteFile(
						(HANDLE)_socket,//_In_         HANDLE hFile,
						_sendBuffer,//_Out_        LPVOID lpBuffer,
						_currentTotal, //_In_         DWORD nNumberOfBytesToRead,
						nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
						&_olpSend//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
					);
					if (!rr)
					{
						LastError = ::WSAGetLastError();
						hr = HRESULT_FROM_WIN32(LastError);
						if (LastError != WSA_IO_PENDING)
						{
							Error(LastError, nullptr);
							SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("SocketIO::WriteFile"), _currentTotal, LastError, hr);
							//LEAVE_BLOCK(0);
						}
					}
				}
				break;
				//////////////////////////////////////////////////////////////////////////
				case 1:
				{
					_currentTransffered += transffered;
					if (_currentTransffered < _currentTotal)
					{
						_sendStep = 1;

						ZeroMemory(&_olpSend, sizeof(OVERLAPPED));

						//long left = _header.dataLen - _curentTransffered;
						//long len = left < _BUFFER_SIZE / 4 ? left : _BUFFER_SIZE / 4;

						bool rr = SocketIO::WriteFile(
							(HANDLE)_socket,//_In_         HANDLE hFile,
							_sendBuffer + _currentTransffered,//_Out_        LPVOID lpBuffer,
							_currentTotal - _currentTransffered,//_In_         DWORD nNumberOfBytesToRead,
							nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
							&_olpSend//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
						);
						if (!rr)
						{
							LastError = ::WSAGetLastError();
							hr = HRESULT_FROM_WIN32(LastError);
							if (LastError != WSA_IO_PENDING)
							{
								Error(LastError, nullptr);
								SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("SocketIO::WriteFile"), _IO_BUFFER_SIZE, LastError, hr);
								//LEAVE_BLOCK(0);
							}
						}
					}
					else
					{
						_sendStep = 0;
						goto START_WRITE_SWICTH;
					}

				}
				break;
				}

			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}
		virtual HRESULT Error(long error, LPOVERLAPPED lpoverlapped) override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_cbSend.StopProvider();
			_cbSend.StopConsumer();
			
			_cbReceive.StopProvider();
			_cbReceive.StopConsumer();

			_event.Set();

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}
		virtual HRESULT Wait()  override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_event.Wait();
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

		
	};


	class  AsyncSocketAcceptTask : public IAsyncTask
	{
	private:
		SOCKET _listenSocket; /***INCOMMING***/ /***INTERNAL***/ /***CLEANUP***/
		SOCKET _AcceptSocket; /***INTERNAL***/
		ScopeResource<function<void()>> _scope_listenSocket{ [this]()
		{
			if (_listenSocket != INVALID_SOCKET)
			{
				::closesocket(_listenSocket);
				_listenSocket = INVALID_SOCKET;
			}
		}};

		OVERLAPPED _overlapped{ 0 };
		IAsyncTask** _self{ nullptr };

		long _steps{ 0 };

		SOCKADDR_STORAGE _addrs[2];
		LPFN_ACCEPTEX _func_AcceptEx {nullptr};

	public:

		virtual ~AsyncSocketAcceptTask() {};

		virtual HRESULT Create()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			ZeroMemory(&_overlapped, sizeof(OVERLAPPED));

			GUID guid = WSAID_ACCEPTEX ;

			DWORD cbBytesReturned = 0;
			int ret = ::WSAIoctl(
				_listenSocket,//_In_   SOCKET s,
				SIO_GET_EXTENSION_FUNCTION_POINTER,//_In_   DWORD dwIoControlCode,
				&guid,//_In_   LPVOID lpvInBuffer,
				sizeof(guid),//_In_   DWORD cbInBuffer,
				&_func_AcceptEx,//_Out_  LPVOID lpvOutBuffer,
				sizeof(_func_AcceptEx),//_In_   DWORD cbOutBuffer,
				&cbBytesReturned,//_Out_  LPDWORD lpcbBytesReturned,
				nullptr,//_In_   LPWSAOVERLAPPED lpOverlapped,
				nullptr//,//_In_   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
			);
			if (0 != ret)
			{
				LastError = ::WSAGetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("WSAIoctl"), LastError, hr);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}
		virtual HRESULT Next(long transffered, LPOVERLAPPED lpoverlapped)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
	
	
			switch (_steps)
			{
			case 0:
			{
				_steps = 1;

				_AcceptSocket = WSASocket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
				if (INVALID_SOCKET == _AcceptSocket)
				{
					LastError = ::WSAGetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("WSASocket"), LastError, hr);
					Error(0, nullptr);
					break;
				}

				ZeroMemory(_addrs, sizeof(_addrs));
				ZeroMemory(&_overlapped, sizeof(_overlapped));
				BOOL ba = _func_AcceptEx(
					_listenSocket,//_In_   SOCKET sListenSocket,
					_AcceptSocket,//_In_   SOCKET sAcceptSocket,
					(PVOID)_addrs,//_In_   PVOID lpOutputBuffer,
					0,//_In_   DWORD dwReceiveDataLength,
					sizeof(SOCKADDR_STORAGE),//_In_   DWORD dwLocalAddressLength,
					sizeof(SOCKADDR_STORAGE),//_In_   DWORD dwRemoteAddressLength,
					nullptr,//_Out_  LPDWORD lpdwBytesReceived,
					&_overlapped//,//_In_   LPOVERLAPPED lpOverlapped
				);
				if (!ba)
				{
					LastError = ::WSAGetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					if (LastError != WSA_IO_PENDING)
					{
						Error(LastError, nullptr);
						SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("AcceptEx"), sizeof(_addrs), LastError, hr);
						//LEAVE_BLOCK(0);
					}
				}
			}
			break;
			case 1:
			{

			}
			break;
			}

			return hr;
		}
		virtual HRESULT Error(long error, LPOVERLAPPED lpoverlapped) = 0;
		virtual HRESULT Wait() = 0;
	};

	class  AsyncAcceptTask : public IAsyncTask
	{
	private:
		CString _serviceName; /***INCOMMING***/


		Vector<SOCKET, false, 8> _listenSocket; /***INTERNAL***/ /***CLEANUP***/
		ScopeResource<function<void()>> _Scope_listenSocket{ [this]() 
		{
			for (long ii = 0; ii < _listenSocket.size(); ++ ii)
			{
				::closesocket(_listenSocket[ii]);
				_listenSocket[ii] = INVALID_SOCKET;
			}
		} };
		Vector<OVERLAPPED, false, 8> _overlapped;

		

	public:
		virtual ~AsyncAcceptTask() {};

		AsyncAcceptTask(LPCTSTR serviceName) :
			_serviceName{serviceName}
		{
		}

		virtual HRESULT Create() override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			ADDRINFOW hints;
			ZeroMemory(&hints, sizeof(hints));
			
			PADDRINFOW pResult = nullptr; /***CLEANUP***/
			ScopeResource <function<void()>> scope_pResult{ [&pResult]()
			{
				if (pResult)
				{
					::FreeAddrInfoW(pResult);
					pResult = nullptr;
				}
			} };

			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			int  gai = ::GetAddrInfoW(
				nullptr,//_In_opt_  PCWSTR pNodeName,
				_serviceName.GetString(),//_In_opt_  PCWSTR pServiceName,
				&hints,//_In_opt_  const ADDRINFOW * pHints,
				&pResult//,//_Out_     PADDRINFOW * ppResult
			);
			if (0 != gai)
			{
				LastError = ::WSAGetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("GetAddrInfoW"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			while (pResult)
			{
				SOCKET sock = ::WSASocket(
					pResult->ai_family,//_In_  int af,
					pResult->ai_socktype,//_In_  int type,
					pResult->ai_protocol,//_In_  int protocol,
					nullptr,//_In_  LPWSAPROTOCOL_INFO lpProtocolInfo,
					0,//_In_  GROUP g,
					WSA_FLAG_OVERLAPPED//,//_In_  DWORD dwFlags
				);
				if (INVALID_SOCKET != sock)
				{
					_listenSocket.push_back(sock);
					_overlapped.push_back(OVERLAPPED{});
				}

				pResult = pResult->ai_next;
			}

			
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}
		virtual HRESULT Next(long transffered, LPOVERLAPPED lpoverlapped) override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}
		virtual HRESULT Error(long error, LPOVERLAPPED lpoverlapped) override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}
		virtual HRESULT Wait() override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

	};

}