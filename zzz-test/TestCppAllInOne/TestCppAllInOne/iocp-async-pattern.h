#pragma once


#include "iocp-async-socket.h"

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

#include "../TaskRunner-old/SmlCircularBuffer.h"


namespace SmartLib
{
	using namespace ::std;





	

	
	class AsyncFileCopyInParallel
	{
	private:

		CString _srcName;	/***INCOMMING***/
		CString _destName;	/***INCOMMING***/


		CircularBuffer _cb{ 1024 * 1024 }; /***INTERNAL***/

		HANDLE _iocp{ nullptr }; /***INTERNAL***/ /***CLEANUP***/
		HANDLE _src{ INVALID_HANDLE_VALUE }; /***INTERNAL***/ /***CLEANUP***/
		HANDLE _dest{ INVALID_HANDLE_VALUE }; /***INTERNAL***/ /***CLEANUP***/

	public:
		AsyncFileCopyInParallel(LPCTSTR srcName, LPCTSTR destName) :
			_srcName{ srcName },
			_destName{ destName }
		{
		}

		virtual ~AsyncFileCopyInParallel() /*override*/
		{

			if (INVALID_HANDLE_VALUE != _dest)
			{
				::CloseHandle(_dest);
				_dest = INVALID_HANDLE_VALUE;
			}

			if (INVALID_HANDLE_VALUE != _src)
			{
				::CloseHandle(_src);
				_src = INVALID_HANDLE_VALUE;
			}

			if (nullptr != _iocp)
			{
				::CloseHandle(_iocp);
				_iocp = INVALID_HANDLE_VALUE;
			}
		}

		HRESULT Create()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			srand(time(nullptr));


			//////////////////////////////////////////////////////////////////////////
			_src = ::CreateFile(
				_srcName.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_READ,//_In_      DWORD dwDesiredAccess,
				FILE_SHARE_READ,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _src)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateFile"), _srcName.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			_dest = ::CreateFile(
				_destName.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
				0,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _dest)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateFile"), _destName.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}

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


		HRESULT Copy()
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			const long THREAD_COUNT{ 4 };
			vector<unique_ptr<thread>> threads;
			for (long ii = 0; ii < THREAD_COUNT; ++ii)
			{
				threads.push_back(make_unique<thread>([this] {RunTasks(); }));
			}

			//////////////////////////////////////////////////////////////////////////
			AsyncReadTask reader{ this };
			AsyncWriteTask writer{ this };

			//////////////////////////////////////////////////////////////////////////
			hr = reader.Create();
			if (FAILED(hr))
			{
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("reader.Create"), LastError, hr);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			hr = writer.Create();
			if (FAILED(hr))
			{
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("writer.Create"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			reader.Next(0, nullptr);
			writer.Next(0, nullptr);

			//////////////////////////////////////////////////////////////////////////
			reader.Wait();
			writer.Wait();

			//////////////////////////////////////////////////////////////////////////
			for (long ii = 0; ii < THREAD_COUNT; ++ii)
			{
				BOOL rr = ::PostQueuedCompletionStatus(
					_iocp,//_In_      HANDLE CompletionPort,
					0,//_In_      DWORD dwNumberOfBytesTransferred,
					0,//_In_      ULONG_PTR dwCompletionKey,
					nullptr//,//_In_opt_  LPOVERLAPPED lpOverlapped
				);
			}


			for (long ii = 0; ii < THREAD_COUNT; ++ii)
			{
				threads[ii]->join();
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}


		static HRESULT Copy(LPCTSTR srcName, LPCTSTR destName)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			AsyncFileCopyInParallel afcp{ srcName , destName };

			//////////////////////////////////////////////////////////////////////////
			hr = afcp.Create();
			if (FAILED(hr))
			{
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("AsyncFileCopyInParallel.Create"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			//////////////////////////////////////////////////////////////////////////
			hr = afcp.Copy();
			if (FAILED(hr))
			{
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("AsyncFileCopyInParallel.Copy"), LastError, hr);
				LEAVE_BLOCK(0);
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/

			return hr;
		}
	private:


		class AsyncReadTask : public IAsyncTask
		{
		private:
			AsyncFileCopyInParallel* _context{ nullptr }; /***INCOMMING***/
			SimpleEvent _event; /***INTERNAL***/
			long _step{ 0 };/***INTERNAL***/

			const long _BUFFER_SIZE{ 64 * 1024 };
			char* _buffer{ nullptr }; /***INTERNAL***//***CLEANUP***/
			CBDataHeader _header; /***INTERNAL***/
			OVERLAPPED _overlapped{ 0 }; /***INTERNAL***/

		public:
			AsyncReadTask(AsyncFileCopyInParallel* context) :
				_context{ context }
			{
			}

			virtual ~AsyncReadTask() override
			{
				if (_buffer)
				{
					::VirtualFree(_buffer, 0, MEM_RELEASE);
					_buffer = nullptr;
				}
			}


			virtual HRESULT Create() override
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				_header.offset.QuadPart = 0;
				ZeroMemory(&_overlapped, sizeof(_overlapped));

				//////////////////////////////////////////////////////////////////////////
				_buffer = (char*)::VirtualAlloc(
					nullptr,//_In_opt_  LPVOID lpAddress,
					_BUFFER_SIZE,//_In_      SIZE_T dwSize,
					MEM_COMMIT | MEM_RESERVE,//_In_      DWORD flAllocationType,
					PAGE_READWRITE//,//_In_      DWORD flProtect
				);
				if (nullptr == _buffer)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _BUFFER_SIZE, LastError, hr);
					LEAVE_BLOCK(0);
				}

				//////////////////////////////////////////////////////////////////////////
				HANDLE iocp = ::CreateIoCompletionPort(
					_context->_src,//_In_      HANDLE FileHandle,
					_context->_iocp,//_In_opt_  HANDLE ExistingCompletionPort,
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


			START_SWICTH:
				switch (_step)
				{
				case 0:
				{
					assert(nullptr == lpoverlapped || lpoverlapped == &_overlapped);
					_step = 1;
					_header.offset.QuadPart += transffered;
					ZeroMemory(&_overlapped, sizeof(_overlapped));
					_overlapped.Offset = _header.offset.LowPart;
					_overlapped.OffsetHigh = _header.offset.HighPart;

					bool rr = ::ReadFile(
						_context->_src,//_In_         HANDLE hFile,
						_buffer,//_Out_        LPVOID lpBuffer,
						_BUFFER_SIZE,//_In_         DWORD nNumberOfBytesToRead,
						nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
						&_overlapped//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
					);
					if (!rr)
					{
						LastError = ::GetLastError();
						hr = HRESULT_FROM_WIN32(LastError);
						if (LastError != ERROR_IO_PENDING)
						{
							Error(LastError, nullptr);
							SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _BUFFER_SIZE, LastError, hr);
							//LEAVE_BLOCK(0);
						}
					}
				}
				break;
				case 1:
				{
					_header.dataLen = transffered;
					_context->_cb.push_back_all((const char*)&_header, sizeof(_header));
					_context->_cb.push_back_all(_buffer, transffered);
					_step = 0;
					goto START_SWICTH;
				}
				break;
				default:
				{
					//////////////////////////////////////////////////////////////////////////
				}
				break;
				}

				return hr;
			}

			virtual HRESULT Error(long error, LPOVERLAPPED lpoverlapped) override
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				_header.dataLen = -1;
				_context->_cb.push_back_all((const char*)&_header, sizeof(_header));
				_event.Set();
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
				_event.Wait();
				//////////////////////////////////////////////////////////////////////////
				END_BLOCK(0);
				/***CLEANUPS***/
				return hr;

			}
		};

		class AsyncWriteTask : public IAsyncTask
		{
		private:
			AsyncFileCopyInParallel* _context{ nullptr }; /***INCOMMING***/
			SimpleEvent _event; /***INTERNAL***/
			long _step{ 0 };/***INTERNAL***/

			const long _BUFFER_SIZE{ 64 * 1024 };
			char* _buffer{ nullptr }; /***INTERNAL***//***CLEANUP***/
			CBDataHeader _header; /***INTERNAL***/
			OVERLAPPED _overlapped{ 0 };
			long _curentTransffered{ 0 };


		public:
			AsyncWriteTask(AsyncFileCopyInParallel* context) :
				_context{ context }
			{
			}

			virtual ~AsyncWriteTask() override
			{
				if (_buffer)
				{
					::VirtualFree(_buffer, 0, MEM_RELEASE);
					_buffer = nullptr;
				}
			}

			virtual HRESULT Error(long error, LPOVERLAPPED lpoverlapped) override
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				BEGIN_BLOCK(0);
				//////////////////////////////////////////////////////////////////////////
				_event.Set();
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
				_header.offset.QuadPart = 0;
				//_transfered.QuadPart = 0;
				ZeroMemory(&_overlapped, sizeof(_overlapped));

				//////////////////////////////////////////////////////////////////////////
				_buffer = (char*)::VirtualAlloc(
					nullptr,//_In_opt_  LPVOID lpAddress,
					_BUFFER_SIZE,//_In_      SIZE_T dwSize,
					MEM_COMMIT | MEM_RESERVE,//_In_      DWORD flAllocationType,
					PAGE_READWRITE//,//_In_      DWORD flProtect
				);
				if (nullptr == _buffer)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _BUFFER_SIZE, LastError, hr);
					LEAVE_BLOCK(0);
				}

				//////////////////////////////////////////////////////////////////////////
				HANDLE iocp = ::CreateIoCompletionPort(
					_context->_dest,//_In_      HANDLE FileHandle,
					_context->_iocp,//_In_opt_  HANDLE ExistingCompletionPort,
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


			START_SWICTH:
				switch (_step)
				{
				case 0:
				{
					assert(nullptr == lpoverlapped || lpoverlapped == &_overlapped);
					_step = 1;

					_curentTransffered = 0;
					long len = _context->_cb.pop_front_all((char*)&_header, sizeof(_header));
					if (_header.dataLen < 0)
					{
						Error(0, nullptr);
						break;
					}

					len = _context->_cb.pop_front_all(_buffer, _header.dataLen);

					ZeroMemory(&_overlapped, sizeof(_overlapped));
					_overlapped.Offset = _header.offset.LowPart;
					_overlapped.OffsetHigh = _header.offset.HighPart;
					//len = _BUFFER_SIZE / 4; //debug!!! 
					bool rr = ::WriteFile(
						_context->_dest,//_In_         HANDLE hFile,
						_buffer,//_Out_        LPVOID lpBuffer,
						len, //_In_         DWORD nNumberOfBytesToRead,
						nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
						&_overlapped//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
					);
					if (!rr)
					{
						LastError = ::GetLastError();
						hr = HRESULT_FROM_WIN32(LastError);
						if (LastError != ERROR_IO_PENDING)
						{
							Error(LastError, nullptr);
							SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _BUFFER_SIZE, LastError, hr);
							//LEAVE_BLOCK(0);
						}
					}
				}
				break;
				case 1:
				{
					_curentTransffered += transffered;
					if (_curentTransffered < _header.dataLen)
					{
						_step = 1;

						_header.offset.QuadPart += transffered;
						ZeroMemory(&_overlapped, sizeof(_overlapped));
						_overlapped.Offset = _header.offset.LowPart;
						_overlapped.OffsetHigh = _header.offset.HighPart;

						//long left = _header.dataLen - _curentTransffered;
						//long len = left < _BUFFER_SIZE / 4 ? left : _BUFFER_SIZE / 4;

						bool rr = ::WriteFile(
							_context->_dest,//_In_         HANDLE hFile,
							_buffer + _curentTransffered,//_Out_        LPVOID lpBuffer,
							_header.dataLen - _curentTransffered,//_In_         DWORD nNumberOfBytesToRead,
							nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
							&_overlapped//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
						);
						if (!rr)
						{
							LastError = ::GetLastError();
							hr = HRESULT_FROM_WIN32(LastError);
							if (LastError != ERROR_IO_PENDING)
							{
								Error(LastError, nullptr);
								SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _BUFFER_SIZE, LastError, hr);
								//LEAVE_BLOCK(0);
							}
						}
					}
					else
					{
						_step = 0;
						goto START_SWICTH;
					}

				}
				break;
				default:
				{
					//////////////////////////////////////////////////////////////////////////
				}
				break;
				}


				return hr;
			}




			virtual HRESULT Wait() override
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
	};

	class AsyncFileCopyInSequence : public IAsyncTask
	{

	private:
		CString _src;	/***INCOMMING***/
		CString _dest;	/***INCOMMING***/
		HANDLE _iocp{ nullptr }; /***INCOMMING***/

		HANDLE _hsrc{ INVALID_HANDLE_VALUE }; /***INTERNAL***/ /***CLEANUP***/
		HANDLE _hdest{ INVALID_HANDLE_VALUE };/***INTERNAL***/ /***CLEANUP***/
		char* _buffer{ nullptr };		/***INTERNAL***/ /***CLEANUP***/
		const long _BUFFER_SIZE{ 4 * 1024 };
		OVERLAPPED _olpRead{ 0 };/***INTERNAL***/
		OVERLAPPED _olpWrite{ 0 };/***INTERNAL***/
		long _step{ 0 };/***INTERNAL***/
		LARGE_INTEGER _offset{ 0 }; /***INTERNAL***/
		SimpleEvent _event; /***INTERNAL***/

	public:
		AsyncFileCopyInSequence(LPCTSTR src, LPCTSTR dest, HANDLE iocp) :
			_src{ src },
			_dest{ dest },
			_iocp{ iocp }
		{
		}

		virtual ~AsyncFileCopyInSequence() override
		{
			if (_buffer)
			{
				::VirtualFree(
					_buffer,//_In_  LPVOID lpAddress,
					0,//_In_  SIZE_T dwSize,
					MEM_RELEASE//,//_In_  DWORD dwFreeType
				);
				_buffer = nullptr;
			}

			if (INVALID_HANDLE_VALUE != _hdest)
			{
				::CloseHandle(_hdest);
				_hdest = INVALID_HANDLE_VALUE;
			}

			if (INVALID_HANDLE_VALUE != _hsrc)
			{
				::CloseHandle(_hsrc);
				_hsrc = INVALID_HANDLE_VALUE;
			}
		}
		virtual HRESULT Create() override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			ZeroMemory(&_olpRead, sizeof(_olpRead));
			ZeroMemory(&_olpWrite, sizeof(_olpWrite));

			//////////////////////////////////////////////////////////////////////////
			_buffer = (char*)::VirtualAlloc(
				nullptr,//_In_opt_  LPVOID lpAddress,
				_BUFFER_SIZE,//_In_      SIZE_T dwSize,
				MEM_COMMIT | MEM_RESERVE,//_In_      DWORD flAllocationType,
				PAGE_READWRITE//,//_In_      DWORD flProtect
			);
			if (nullptr == _buffer)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _BUFFER_SIZE, LastError, hr);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			_hsrc = ::CreateFile(
				_src.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_READ,//_In_      DWORD dwDesiredAccess,
				FILE_SHARE_READ,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				OPEN_EXISTING,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _hsrc)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateFile"), _src.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}

			


			//////////////////////////////////////////////////////////////////////////
			_hdest = ::CreateFile(
				_dest.GetString(),//_In_      LPCTSTR lpFileName,
				GENERIC_WRITE,//_In_      DWORD dwDesiredAccess,
				0,//_In_      DWORD dwShareMode,
				nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
				CREATE_ALWAYS,//_In_      DWORD dwCreationDisposition,
				FILE_FLAG_OVERLAPPED,//_In_      DWORD dwFlagsAndAttributes,
				nullptr//,//_In_opt_  HANDLE hTemplateFile
			);
			if (INVALID_HANDLE_VALUE == _hdest)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateFile"), _dest.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}



			//////////////////////////////////////////////////////////////////////////
			HANDLE iocp = ::CreateIoCompletionPort(
				_hsrc,//_In_      HANDLE FileHandle,
				_iocp,//_In_opt_  HANDLE ExistingCompletionPort,
				(ULONG_PTR)(IAsyncTask*)(this),//_In_      ULONG_PTR CompletionKey,
				0//,//_In_      DWORD NumberOfConcurrentThreads
			);
			if (nullptr == iocp)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateIoCompletionPort"), _src.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}



			//////////////////////////////////////////////////////////////////////////
			iocp = ::CreateIoCompletionPort(
				_hdest,//_In_      HANDLE FileHandle,
				_iocp,//_In_opt_  HANDLE ExistingCompletionPort,
				(ULONG_PTR)(IAsyncTask*)(this),//_In_      ULONG_PTR CompletionKey,
				0//,//_In_      DWORD NumberOfConcurrentThreads
			);
			if (nullptr == iocp)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], file=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateIoCompletionPort"), _dest.GetString(), LastError, hr);
				LEAVE_BLOCK(0);
			}

#if false //test duplicated handle
			//////////////////////////////////////////////////////////////////////////
			HANDLE duppedHandle = INVALID_HANDLE_VALUE;
			BOOL bdup = ::DuplicateHandle(
				::GetCurrentProcess(),//_In_   HANDLE hSourceProcessHandle,
				_hsrc,//_In_   HANDLE hSourceHandle,
				::GetCurrentProcess(),//_In_   HANDLE hTargetProcessHandle,
				&duppedHandle,//_Out_  LPHANDLE lpTargetHandle,
				0,//_In_   DWORD dwDesiredAccess,
				FALSE,//_In_   BOOL bInheritHandle,
				DUPLICATE_SAME_ACCESS//,//_In_   DWORD dwOptions
			);
			if (bdup)
			{
				::CloseHandle(_hsrc);
				_hsrc = duppedHandle;
			}


			duppedHandle = INVALID_HANDLE_VALUE;
			bdup = ::DuplicateHandle(
				::GetCurrentProcess(),//_In_   HANDLE hSourceProcessHandle,
				_hdest,//_In_   HANDLE hSourceHandle,
				::GetCurrentProcess(),//_In_   HANDLE hTargetProcessHandle,
				&duppedHandle,//_Out_  LPHANDLE lpTargetHandle,
				0,//_In_   DWORD dwDesiredAccess,
				FALSE,//_In_   BOOL bInheritHandle,
				DUPLICATE_SAME_ACCESS//,//_In_   DWORD dwOptions
			);
			if (bdup)
			{
				::CloseHandle(_hdest);
				_hdest = duppedHandle;
			}
#endif

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}



		virtual HRESULT Next(long transffered, LPOVERLAPPED lpoverlapped) override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;


			switch (_step)
			{
			case 0:
			{
				assert(nullptr == lpoverlapped || lpoverlapped == &_olpWrite);
				_step = 1;
				_offset.QuadPart += transffered;
				ZeroMemory(&_olpRead, sizeof(_olpRead));
				_olpRead.OffsetHigh = _offset.HighPart;
				_olpRead.Offset = _offset.LowPart;
				bool rr = ::ReadFile(
					_hsrc,//_In_         HANDLE hFile,
					_buffer,//_Out_        LPVOID lpBuffer,
					_BUFFER_SIZE,//_In_         DWORD nNumberOfBytesToRead,
					nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
					&_olpRead//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
				);
				if (!rr)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					if (ERROR_IO_PENDING != ERROR_IO_PENDING)
					{
						Error(LastError, nullptr);
					}
				}
			}
			break;
			case 1:
			{
				assert(nullptr == lpoverlapped || lpoverlapped == &_olpRead);
				_step = 0;
				ZeroMemory(&_olpWrite, sizeof(_olpWrite));
				_olpWrite.OffsetHigh = _offset.HighPart;
				_olpWrite.Offset = _offset.LowPart;
				bool rr = ::WriteFile(
					_hdest,//_In_         HANDLE hFile,
					_buffer,//_Out_        LPVOID lpBuffer,
					(DWORD)_olpRead.InternalHigh,//_In_         DWORD nNumberOfBytesToRead,
					nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
					&_olpWrite//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
				);
				if (!rr)
				{
					LastError = ::GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					if (ERROR_IO_PENDING != ERROR_IO_PENDING)
					{
						Error(LastError, nullptr);
					}
				}
			}
			break;
			default:
			{
				//////////////////////////////////////////////////////////////////////////
			}
			break;
			}

			return hr;
		}

		virtual HRESULT Error(long error, LPOVERLAPPED lpoverlapped) override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_event.Set();
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
			_event.Wait();
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}


		//////////////////////////////////////////////////////////////////////////
		static HRESULT Copy(LPCTSTR src, LPCTSTR dest)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			HANDLE iocp{ nullptr }; /***CLEANUP***/

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			iocp = ::CreateIoCompletionPort(
				INVALID_HANDLE_VALUE,//_In_      HANDLE FileHandle,
				nullptr,//_In_opt_  HANDLE ExistingCompletionPort,
				0,//_In_      ULONG_PTR CompletionKey,
				0//,//_In_      DWORD NumberOfConcurrentThreads
			);
			if (nullptr == iocp)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateIoCompletionPort"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			AsyncFileCopyInSequence afc{ src , dest, iocp };
			afc.Create();
			afc.Next(0, nullptr);


			srand(time(nullptr));


			auto threadProc = [iocp]()
			{
				HRESULT hr = S_OK;
				DWORD LastError = 0;

				for (;;)
				{
					//this_thread::sleep_for(chrono::milliseconds(rand()%100 + 50));
					DWORD NumberOfBytes{ 0 };
					ULONG_PTR CompletionKey{ 0 };
					LPOVERLAPPED pOverlapped{ nullptr };
					BOOL rr = ::GetQueuedCompletionStatus(
						iocp,//_In_   HANDLE CompletionPort,
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
			};

			::std::thread t1{ threadProc };
			::std::thread t2{ threadProc };
			::std::thread t3{ threadProc };

			afc.Wait();



			const long THREAD_COUNT = 3;
			for (long ii = 0; ii < THREAD_COUNT; ++ii)
			{
				BOOL rr = ::PostQueuedCompletionStatus(
					iocp,//_In_      HANDLE CompletionPort,
					0,//_In_      DWORD dwNumberOfBytesTransferred,
					0,//_In_      ULONG_PTR dwCompletionKey,
					nullptr//,//_In_opt_  LPOVERLAPPED lpOverlapped
				);
			}

			t1.join();
			t2.join();
			t3.join();

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			if (iocp)
			{
				::CloseHandle(iocp);
				iocp = nullptr;
			}
			return hr;
		}
	};
}