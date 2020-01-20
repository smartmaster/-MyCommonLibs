#pragma once


#include "SmlIFile.h" //must be put first

#include "SmlITask.h"
#include "SmlTaskRunnner.h"
#include "SmlSimpleEvent.h"
#include "SmlCommon.h"

#include "../SmlRefPtr.h"
#include "../SmlCircularBuffer.h"


#include <mutex>
#include <cassert>
#include <iostream>

namespace SmartLib
{
	using namespace ::std;

	class SocketRequestReplyTask : public IAsyncTask
	{
	private:
		RefPtr<IFile*> _file; /***INCOMMING***//***INTERNAL***/
		RefPtr<TaskRunner> _taskRunner; /***INCOMMING***/

		IFile* _fileShadow{ nullptr }; /***INTERNAL***/


		constexpr static const long _IO_BUFFER_SIZE{ BUFFER_SIZE::_IO_BUFFER_SIZE };
		char* _ioBuffer{ nullptr }; /***INTERNAL***//***CLEANUP***/
		ScopeResource<> _sr_ioBuffer{
			[this]()
			{
				if (_ioBuffer)
				{
					::VirtualFree(_ioBuffer, 0, MEM_RELEASE);
					_ioBuffer = nullptr;
				}
			}
		};


		enum class STEP
		{
			START_READ,
			DONE_READ,
			START_WRITE,
		};
		STEP _step{ STEP::START_READ }; /***INTERNAL***/

		OVERLAPPED _overlapped{ 0 }; /***INTERNAL***/
		
		SimpleEvent _event; /***INTERNAL***/


		long _currentTransfferedWrite{ 0 };
		long _currentTotalWrite{ 0 };



	private:
		virtual HRESULT Create() override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_ioBuffer = (char*)::VirtualAlloc(nullptr, _IO_BUFFER_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (nullptr == _ioBuffer)
			{
				LastError = ::GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), LastError, hr);
				LEAVE_BLOCK(0);
			}


			if (!_fileShadow->IsIocpAssociated())
			{
				//////////////////////////////////////////////////////////////////////////
				HANDLE iocp = ::CreateIoCompletionPort(
					_fileShadow->NativeHandle(),//_In_      HANDLE FileHandle,
					_taskRunner->Iocp(),//_In_opt_  HANDLE ExistingCompletionPort,
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

				_fileShadow->SetIocpAssociated();
			}



			//////////////////////////////////////////////////////////////////////////
			hr = _taskRunner->StartTask((IAsyncTask*)(this), &_overlapped);
			if (FAILED(hr))
			{
				//LastError = ::GetLastError();
				//hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("StartTask"), LastError, hr);
				LEAVE_BLOCK(0);
			}


			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

		virtual HRESULT OnNext(long transffered, LPOVERLAPPED lpoverlapped)/*override*/
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;



			BEGIN_BLOCK(0);

			//////////////////////////////////////////////////////////////////////////
			//unique_lock<mutex> lock{ _ioMtxRead };
			//////////////////////////////////////////////////////////////////////////
		START_SWICTH:
			switch (_step)
			{
				//////////////////////////////////////////////////////////////////////////
			case  STEP::START_READ:
			{
				
				ZeroMemory(&_overlapped, sizeof(OVERLAPPED));

				_step = STEP::DONE_READ;
				bool rr = _fileShadow->ReadFile(
					_ioBuffer,//_Out_        LPVOID lpBuffer,
					_IO_BUFFER_SIZE,//_In_         DWORD nNumberOfBytesToRead,
					nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
					&_overlapped//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
				);
				if (!rr)
				{
					LastError = _fileShadow->GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					if (LastError != WSA_IO_PENDING)
					{
						//Error(LastError, nullptr);
						//_fileShadow->Close();
						SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _IO_BUFFER_SIZE, LastError, hr);
						break;
					}
				}

			}
			break;
			//////////////////////////////////////////////////////////////////////////
			case  STEP::DONE_READ:
			{
				_currentTransfferedWrite = 0;
				_currentTotalWrite = transffered;
				transffered = 0;
				_step = STEP::START_WRITE;
				goto START_SWICTH;
				break;

			}
			break;
			case  STEP::START_WRITE:
			{
				_currentTransfferedWrite += transffered;
				if (_currentTransfferedWrite == _currentTotalWrite)
				{
					_step = STEP::START_READ;
					goto START_SWICTH;
				}
				else if (_currentTransfferedWrite < _currentTotalWrite)
				{
					ZeroMemory(&_overlapped, sizeof(OVERLAPPED));
					_step = STEP::START_WRITE;
					bool rr = _fileShadow->WriteFile(
						_ioBuffer + _currentTransfferedWrite,//_Out_        LPVOID lpBuffer,
						_currentTotalWrite - _currentTransfferedWrite,//_In_         DWORD nNumberOfBytesToRead,
						nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
						&_overlapped//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
					);
					if (!rr)
					{
						LastError = _fileShadow->GetLastError();
						hr = HRESULT_FROM_WIN32(LastError);
						if (LastError != WSA_IO_PENDING)
						{
							//Error(LastError, nullptr);
							SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("SocketIO::WriteFile"), _IO_BUFFER_SIZE, LastError, hr);
							break;
						}
					}
		
				}
				else
				{
					assert(false);
				}
			}
			break;
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

		
		

	public:
		virtual ~SocketRequestReplyTask() {};

		SocketRequestReplyTask(RefPtr<IFile*> file, RefPtr<TaskRunner> taskRunner) :
			_file{ move(file) },
			_taskRunner{ move(taskRunner) }//
		{
			if (_file)
			{
				_fileShadow = *_file.Ptr();
			}
		}

		virtual void AddRelatedTasks(IAsyncTask* task) override
		{
			//_relatedTasks.push_back(task);
		}

		virtual HRESULT OnError(long error, LPOVERLAPPED lpoverlapped)override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//unique_lock<mutex> lock{ _errorMtx };
			//////////////////////////////////////////////////////////////////////////
			//_cbRead->StopProvider();
			//_cbRead->StopConsumer();
			//_cbWrite->StopProvider();
			//_cbWrite->StopConsumer();
			//_fileShadow->Close();

			//for (long ii = 0; ii < _relatedTasks.size(); ++ii)
			//{
			//	_relatedTasks[ii]->OnError(error, nullptr);
			//	if (_taskRunner->TestSetCleanupFlag(_relatedTasks[ii]))
			//	{
			//		_taskRunner->CleanupTask(_relatedTasks[ii]);
			//	}
			//}
			_event.Set();
			//_taskRunner->CleanupTask((IAsyncTask*)(this));
			//if (_self)
			//{
			//	RefPtr<IAsyncTask*> self;
			//	self.Attach(_self);
			//	_self = nullptr;
			//	_taskRunner->CleanupTask(self);
			//}
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
}