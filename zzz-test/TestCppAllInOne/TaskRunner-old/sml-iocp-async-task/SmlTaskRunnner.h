#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <functional>
#include <thread>
#include <mutex>

#include "../SmlVector.h"
#include "../SmlMTQueue.h"
#include "../SmlRefPtr.h"

#include "../sml-common/CodeBlock.h"
#include "../sml-common/log.h"

#include "SmlITask.h"

namespace SmartLib
{
	using namespace ::std;

	//////////////////////////////////////////////////////////////////////////
	class TaskRunner
	{
	private:
		constexpr static const long CLEANUP_THREAD_COUNT{ 2 };

	private:
		HANDLE _iocp{ nullptr }; /***INTERNAL***/ /***CLEANUP***/
		ScopeResource<function<void()>> _scope_iocp{ [this]()
		{
			if (nullptr != _iocp)
			{
				::CloseHandle(_iocp);
				_iocp = nullptr;
			}
		} };

		Vector<RefPtr<thread>, true, 8> _threads; /***INTERNAL***/
		Vector<RefPtr<thread>, true, 8> _cleanupThreads; /***INTERNAL***/
		MTQueue<RefPtr<IAsyncTask*>, 32> _cleanupQueue; /***INTERNAL***/

	private:

		HRESULT CleanupTasks()
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

			return S_OK;
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

		HRESULT CleanupTask(RefPtr<IAsyncTask*> task)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_cleanupQueue.push_back(task);
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;

		}

		HRESULT StartTask(IAsyncTask* task, LPOVERLAPPED lpoverlapped)
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
			//////////////////////////////////////////////////////////////////////////
			long cleanup_thread_count = CLEANUP_THREAD_COUNT;
			for (long ii = 0; ii < cleanup_thread_count; ++ii)
			{
				_cleanupThreads.push_back(RefPtr<thread>::Make([this] {this->CleanupTasks(); }));
			}


			//////////////////////////////////////////////////////////////////////////
			SYSTEM_INFO si{ 0 };
			::GetSystemInfo(&si);
			long threadCpount = si.dwNumberOfProcessors * 2;
			for (long ii = 0; ii < threadCpount; ++ii)
			{
				_threads.push_back(RefPtr<thread>::Make([this] {this->RunTasks(); }));
			}

		}


		void StopThreads()
		{
			//////////////////////////////////////////////////////////////////////////
			for (long ii = 0; ii < _threads.size(); ++ii)
			{
				BOOL rr = ::PostQueuedCompletionStatus(
					_iocp,//_In_      HANDLE CompletionPort,
					0,//_In_      DWORD dwNumberOfBytesTransferred,
					0,//_In_      ULONG_PTR dwCompletionKey,
					nullptr//,//_In_opt_  LPOVERLAPPED lpOverlapped
				);
			}

			for (long ii = 0; ii < _threads.size(); ++ii)
			{
				_threads[ii]->join();
			}


			//////////////////////////////////////////////////////////////////////////
			for (long ii = 0; ii < _cleanupThreads.size(); ++ii)
			{
				_cleanupQueue.StopConsumer();
				_cleanupQueue.StopProvider();
			}

			for (long ii = 0; ii < _cleanupThreads.size(); ++ii)
			{
				_cleanupThreads[ii]->join();
			}
		}


	public:
		static RefPtr<TaskRunner> Instance()
		{
			static RefPtr<TaskRunner> singleton;
			static mutex mtx;
			static ScopeResource<> sr_singleton{
			[]()
			{
				if (singleton)
				{
					RefPtr<TaskRunner> sptr;
					sptr.Attach(singleton.Detach());
					sptr->StopThreads();
				}
			}};

			if (!singleton)
			{
				unique_lock<mutex> lock{mtx};
				if (!singleton)
				{
					RefPtr<TaskRunner> sptr = RefPtr<TaskRunner>::Make();
					TaskRunner* sptrShadow = sptr.Ptr();
					sptrShadow->Create();
					sptrShadow->StartThreads();
					singleton.Attach(sptr.Detach());
				}
			}

			return singleton;
		}


	};

}