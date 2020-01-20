#pragma once

#include "SmlIFile.h"
#include "SmlITask.h"
#include "SmlTaskRunnner.h"
#include "SmlSimpleEvent.h"
#include "SmlCommon.h"

#include "../SmlRefPtr.h"
#include "../SmlCircularBuffer.h"






namespace SmartLib
{
	class Win32FileReadAsyncTask : public IAsyncTask
	{
	private:
		RefPtr<IFile*> _file; /***INCOMMING***//***INTERNAL***/
		RefPtr<CircularBuffer> _cb;/***INCOMMING***//***INTERNAL***/
		RefPtr<TaskRunner> _taskRunner; /***INCOMMING***/

		//IAsyncTask** _self{ nullptr };

		IFile* _fileShadow{ nullptr };


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

		enum class STEP : long
		{
			START_READ,
			DONE_READ,
		};
		STEP _step{ STEP::START_READ }; /***INTERNAL***/
		OVERLAPPED _overlapped{ 0 }; /***INTERNAL***/
		SimpleEvent _event; /***INTERNAL***/

		CBDataHeader _header; /***INTERNAL***/


		Vector<IAsyncTask*, false, 1> _relatedTasks; /***INTERNAL***/

	public:
		virtual ~Win32FileReadAsyncTask() {};

		Win32FileReadAsyncTask(RefPtr<IFile*> file, RefPtr<CircularBuffer> cbReceive, RefPtr<TaskRunner> taskRunner) :
			_file{ move(file) },
			_cb{ move(cbReceive) },
			_taskRunner{ move(taskRunner) }//
		{
			if (_file)
			{
				_fileShadow = *_file.Ptr();
			}
		}

		virtual void AddRelatedTasks(IAsyncTask* task) override
		{
			_relatedTasks.push_back(task);
		}
		//void RefSelf(IAsyncTask** self) override
		//{
		//	_self = self;
		//}

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

		virtual HRESULT OnNext(long transffered, LPOVERLAPPED lpoverlapped)override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;


		START_SWICTH:
			switch (_step)
			{
			case STEP::START_READ:
			{
				assert(nullptr == lpoverlapped || lpoverlapped == &_overlapped);
				
				_header.offset.QuadPart += transffered;
				ZeroMemory(&_overlapped, sizeof(_overlapped));
				_overlapped.Offset = _header.offset.LowPart;
				_overlapped.OffsetHigh = _header.offset.HighPart;

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
					if (LastError != ERROR_IO_PENDING)
					{
						//Error(LastError, nullptr);
						SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _IO_BUFFER_SIZE, LastError, hr);
						//LEAVE_BLOCK(0);
					}
				}
			}
			break;
			case STEP::DONE_READ:
			{
				_header.dataLen = transffered;
				_cb->push_back_all((const char*)&_header, sizeof(_header));
				_cb->push_back_all(_ioBuffer, transffered);
				_step = STEP::START_READ;
				goto START_SWICTH;
			}
			break;
			}

			return hr;
		}

		virtual HRESULT OnError(long error, LPOVERLAPPED lpoverlapped)override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			//_cbReceive->StopProvider();
			//_cbReceive->StopConsumer();
			_header.dataLen = -1;
			_cb->push_back_all((const char*)&_header, sizeof(_header));

			_relatedTasks[0]->Wait();
			if (_taskRunner->CleanupTaskFlag(_relatedTasks[0]))
			{
				_taskRunner->CleanupTask(_relatedTasks[0]);
			}
			_event.Set();
			//_taskRunner->CleanupTask((IAsyncTask*)(this));
			/*if (_self)
			{
				RefPtr<IAsyncTask*> self;
				self.Attach(_self);
				_self = nullptr;
				_taskRunner->CleanupTask(self);
			}*/
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



	class Win32FileWriteAsyncTask : public IAsyncTask
	{
	private:
		RefPtr<IFile*> _file; /***INCOMMING***//***INTERNAL***/
		RefPtr<CircularBuffer> _cb;/***INCOMMING***//***INTERNAL***/
		RefPtr<TaskRunner> _taskRunner; /***INCOMMING***/

		//IAsyncTask** _self{ nullptr };
		IFile* _fileShadow{ nullptr };

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

		enum class STEP : long 
		{
			START_WRITE,
			WRITE_MORE,
		};
		STEP _step{ STEP::START_WRITE }; /***INTERNAL***/
		OVERLAPPED _overlapped{ 0 }; /***INTERNAL***/
		
		SimpleEvent _event; /***INTERNAL***/

		CBDataHeader _header; /***INTERNAL***/
		long _curentTransffered{ 0 };
		//long _currentTotal{ 0 };

	public:
		virtual ~Win32FileWriteAsyncTask() {};

		Win32FileWriteAsyncTask(RefPtr<IFile*> file, RefPtr<CircularBuffer> cbSend, RefPtr<TaskRunner> taskRunner) :
			_file{ move(file) },
			_cb{ move(cbSend) },
			_taskRunner{ move(taskRunner) }//
		{
			if (_file)
			{
				_fileShadow = *_file.Ptr();
			}

		}


		virtual void AddRelatedTasks(IAsyncTask* task) override
		{
		}

		//void RefSelf(IAsyncTask** self) override
		//{
		//	_self = self;
		//}

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

		virtual HRESULT OnNext(long transffered, LPOVERLAPPED lpoverlapped) override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;


		START_SWICTH:
			switch (_step)
			{
			case STEP::START_WRITE:
			{
				assert(nullptr == lpoverlapped || lpoverlapped == &_overlapped);
				

				_curentTransffered = 0;
				long len = _cb->pop_front_all((char*)&_header, sizeof(_header));
				if (len < sizeof(_header) || _header.dataLen < 0)
				{
					//Error(0, nullptr);
					_event.Set();
					break;
				}

				len = _cb->pop_front_all(_ioBuffer, _header.dataLen);
				if (len < _header.dataLen)
				{
					//Error(0, nullptr);
					_event.Set();
					break;
				}

				ZeroMemory(&_overlapped, sizeof(_overlapped));
				_overlapped.Offset = _header.offset.LowPart;
				_overlapped.OffsetHigh = _header.offset.HighPart;
				//len = _BUFFER_SIZE / 4; //debug!!! 
				_step = STEP::WRITE_MORE;
				bool rr = _fileShadow->WriteFile(
					_ioBuffer,//_Out_        LPVOID lpBuffer,
					len, //_In_         DWORD nNumberOfBytesToRead,
					nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
					&_overlapped//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
				);
				if (!rr)
				{
					LastError = _fileShadow->GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					if (LastError != ERROR_IO_PENDING)
					{
						//Error(LastError, nullptr);
						SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _IO_BUFFER_SIZE, LastError, hr);
						//LEAVE_BLOCK(0);
					}
				}
			}
			break;
			case STEP::WRITE_MORE:
			{
				_curentTransffered += transffered;
				if (_curentTransffered < _header.dataLen)
				{
					_header.offset.QuadPart += transffered;
					ZeroMemory(&_overlapped, sizeof(_overlapped));
					_overlapped.Offset = _header.offset.LowPart;
					_overlapped.OffsetHigh = _header.offset.HighPart;

					//long left = _header.dataLen - _curentTransffered;
					//long len = left < _BUFFER_SIZE / 4 ? left : _BUFFER_SIZE / 4;
					_step = STEP::WRITE_MORE;
					bool rr = _fileShadow->WriteFile(
						_ioBuffer + _curentTransffered,//_Out_        LPVOID lpBuffer,
						_header.dataLen - _curentTransffered,//_In_         DWORD nNumberOfBytesToRead,
						nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
						&_overlapped//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
					);
					if (!rr)
					{
						LastError = _fileShadow->GetLastError();
						hr = HRESULT_FROM_WIN32(LastError);
						if (LastError != ERROR_IO_PENDING)
						{
							//Error(LastError, nullptr);
							SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("VirtualAlloc"), _IO_BUFFER_SIZE, LastError, hr);
							//LEAVE_BLOCK(0);
						}
					}
				}
				else if(_curentTransffered == _header.dataLen)
				{
					_step = STEP::START_WRITE;
					goto START_SWICTH;
				}
				else
				{
					assert(false);
				}

			}
			break;
			}


			return hr;
		}



		virtual HRESULT OnError(long error, LPOVERLAPPED lpoverlapped)override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			//_cbSend->StopProvider();
			//_cbSend->StopConsumer();
			_event.Set();
			//_taskRunner->CleanupTask((IAsyncTask*)(this));
			/*if (_self)
			{
				RefPtr<IAsyncTask*> self;
				self.Attach(_self);
				_self = nullptr;
				_taskRunner->CleanupTask(self);
			}*/
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