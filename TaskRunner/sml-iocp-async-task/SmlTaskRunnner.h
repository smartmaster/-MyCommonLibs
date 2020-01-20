#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <functional>
#include <thread>
#include <mutex>
#include <memory>
#include <set>
#include <map>

#include "../SmlVector.h"
#include "../SmlMTQueue.h"
#include "../SmlTimeList.h"
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
		ScopeResource<function<void()>> _sr_iocp{ [this]()
		{
			if (nullptr != _iocp)
			{
				::CloseHandle(_iocp);
				_iocp = nullptr;
			}
		} };

		Vector<RefPtr<thread>, true, 8> _threads; /***INTERNAL***/
		
		mutex _runningTasksMtx;
		List<RefPtr<IAsyncTask*>> _runningTasks;
		map<IAsyncTask*, List<RefPtr<IAsyncTask*>>::Node*> _runningTasksMap;
		set<IAsyncTask*> _runningTaskCleanupFlags;

		TimeList<RefPtr<IAsyncTask*>> _timeCleanupList{3'000, 10'000};
		
			   
	private:

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
					SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("GetQueuedCompletionStatus"), LastError, hr);
					IAsyncTask* task = (IAsyncTask*)(CompletionKey);
					if (nullptr == task)
					{
						break;
					}
					if (CleanupTaskFlag(task))
					{
						task->OnError(LastError, pOverlapped);
						CleanupTask(task);
					}
				}
				else
				{
					IAsyncTask* task = (IAsyncTask*)(CompletionKey);
					if (nullptr == task)
					{
						break;
					}
					task->OnNext(NumberOfBytes, pOverlapped);
				}
			}

			return hr;
		}

		void DisposeTask(RefPtr<IAsyncTask*>& task)
		{
			IAsyncTask* tempTask = *task;

			(*task)->Wait();
			task.Attach(nullptr);

			{
				unique_lock<mutex> lock{ _runningTasksMtx };
				_runningTaskCleanupFlags.erase(tempTask);
			}
		}

	public:

		TaskRunner()
		{
			_timeCleanupList.SetDispose( [this](RefPtr<IAsyncTask*>& task)
			{
				this->DisposeTask(task);
			});
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


		void AddRunning(RefPtr<IAsyncTask*> task)
		{
			unique_lock<mutex> _lock{ _runningTasksMtx };
			auto nn = _runningTasks.push_back(task);
			_runningTasksMap.insert({task.Ref(), nn});
		}


		bool CleanupTaskFlag(IAsyncTask* task)
		{
			bool bb = false;

			unique_lock<mutex> lock{ _runningTasksMtx };


			auto iterRunning = _runningTasksMap.find(task);
			if (iterRunning != _runningTasksMap.end())
			{
				auto iter = _runningTaskCleanupFlags.find(task);
				if (iter == _runningTaskCleanupFlags.end())
				{
					bb = true;
					_runningTaskCleanupFlags.insert(task);
				}
			}

			return bb;
		}

		HRESULT CleanupTask(IAsyncTask* task)
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			unique_lock<mutex> lock{ _runningTasksMtx };

			auto iter = _runningTasksMap.find(task);
			if (iter != _runningTasksMap.end())
			{
				RefPtr<IAsyncTask*> temp = static_cast<RefPtr<IAsyncTask*>&&>(iter->second->Data);
				_timeCleanupList.push_back(temp);

				_runningTasks.DeleteNode(iter->second);
				_runningTasksMap.erase(iter);
			}

			/*auto first = _runningTasks.First();
			auto stop = _runningTasks.Stop();
			while (first != stop)
			{
				if (task == *first->Data)
				{
					RefPtr<IAsyncTask*> temp = static_cast<RefPtr<IAsyncTask*>&&>(first->Data);
					_timeCleanupList.push_back(temp);
					first = _runningTasks.DeleteNode(first);
					break;
				}
				else
				{
					first = first->Next;
				}
			}*/
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
			SYSTEM_INFO si{ 0 };
			::GetSystemInfo(&si);
			long threadCpount = si.dwNumberOfProcessors * 2;
			//long threadCpount = 1;
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


			_timeCleanupList.StopScan();
			
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