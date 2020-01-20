#pragma once


#include "SmlIFile.h" //must be put first

#include "SmlITask.h"
#include "SmlTaskRunnner.h"
#include "SmlSimpleEvent.h"
#include "SmlCommon.h"

#include "../SmlRefPtr.h"
#include "../SmlCircularBuffer.h"


#include <mutex>


namespace SmartLib
{
	using namespace ::std;

	class SocketReadAsyncTask : public IAsyncTask
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



		OVERLAPPED _overlapped{ 0 }; /***INTERNAL***/
		long _step{ 0 }; /***INTERNAL***/
		SimpleEvent _event; /***INTERNAL***/

		//mutex _errorMtx; /***INTERNAL***/
		mutex _ioMtx; /***INTERNAL***/

		Vector<IAsyncTask*, false, 2> _relatedTasks;

	public:
		virtual ~SocketReadAsyncTask() {};

		SocketReadAsyncTask(RefPtr<IFile*> file, RefPtr<CircularBuffer> cbReceive, RefPtr<TaskRunner> taskRunner) :
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

		virtual HRESULT OnNext(long transffered, LPOVERLAPPED lpoverlapped)override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);

			//////////////////////////////////////////////////////////////////////////
			unique_lock<mutex> lock{_ioMtx};
			//////////////////////////////////////////////////////////////////////////
		START_SWICTH:
			switch (_step)
			{
				//////////////////////////////////////////////////////////////////////////
			case  0:
			{
				_step = 1;
				ZeroMemory(&_overlapped, sizeof(OVERLAPPED));

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
				else
				{
					DWORD cbTransfer = 0;
					DWORD dwFlags = 0;
					BOOL bgor = ::WSAGetOverlappedResult(
						(SOCKET)_fileShadow->NativeHandle(),//_In_   SOCKET s,
						&_overlapped,//_In_   LPWSAOVERLAPPED lpOverlapped,
						&cbTransfer,//_Out_  LPDWORD lpcbTransfer,
						TRUE,//_In_   BOOL fWait,
						&dwFlags//,//_Out_  LPDWORD lpdwFlags
					);
					if (!bgor)
					{
						LastError = _fileShadow->GetLastError();
						hr = HRESULT_FROM_WIN32(LastError);
						if (LastError != WSA_IO_PENDING)
						{
							//Error(LastError, nullptr);
							SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("WSAGetOverlappedResult"), LastError, hr);
							break;
						}
					}
					transffered = cbTransfer;
					_step = 1;
					goto START_SWICTH;
				}
			}
			break;
			//////////////////////////////////////////////////////////////////////////
			case  1:
			{
				//_receiveHeader.dataLen = transffered;
				//_cbReceive.ProvideAll((const char*)&_receiveHeader, sizeof(_receiveHeader));
				long len = _cb->push_back_all(_ioBuffer, transffered);
				if (len < transffered)
				{
					//Error(0, nullptr);
					break;
				}
				else
				{
					_step = 0;
					goto START_SWICTH;
				}
			}
			break;
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

	/*	void AddRelated(IAsyncTask* task)
		{
			
		}*/

		virtual HRESULT OnError(long error, LPOVERLAPPED lpoverlapped)override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//unique_lock<mutex> lock{ _errorMtx };
			//////////////////////////////////////////////////////////////////////////
			_cb->StopProvider();
			_cb->StopConsumer();
			//_fileShadow->Close();

			for (long ii = 0; ii < _relatedTasks.size(); ++ ii)
			{
				_relatedTasks[ii]->OnError(error, nullptr);
				if (_taskRunner->TestSetCleanupFlag(_relatedTasks[ii]))
				{
					_taskRunner->CleanupTask(_relatedTasks[ii]);
				}
			}
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



	class SocketWriteAsyncTask : public IAsyncTask
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

		OVERLAPPED _overlapped{ 0 }; /***INTERNAL***/
		long _step{ 0 }; /***INTERNAL***/
		SimpleEvent _event; /***INTERNAL***/

		long _currentTransffered{ 0 };
		long _currentTotal{ 0 };

		mutex _ioMtx; /***INTERNAL***/

	public:
		virtual ~SocketWriteAsyncTask() {};

		SocketWriteAsyncTask(RefPtr<IFile*> file, RefPtr<CircularBuffer> cbSend, RefPtr<TaskRunner> taskRunner) :
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

		virtual HRESULT OnNext(long transffered, LPOVERLAPPED lpoverlapped)override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			unique_lock<mutex> lock{ _ioMtx };

			//////////////////////////////////////////////////////////////////////////
		START_SWICTH:
			switch (_step)
			{
				//////////////////////////////////////////////////////////////////////////
			case 0:
			{
				_step = 1;

				_currentTransffered = 0;
				_currentTotal = _cb->pop_front(_ioBuffer, _IO_BUFFER_SIZE);
				if (_currentTotal < 0)
				{
					//Error(0, nullptr);
					//if (_taskRunner->CheckCleanupFlag(this))
					//{
					//	_taskRunner->CleanupTask(this);
					//}
					break;
				}

				ZeroMemory(&_overlapped, sizeof(OVERLAPPED));
				//len = _BUFFER_SIZE / 4; //debug!!! 
				bool rr = _fileShadow->WriteFile(
					_ioBuffer,//_Out_        LPVOID lpBuffer,
					_currentTotal, //_In_         DWORD nNumberOfBytesToRead,
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
						SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("SocketIO::WriteFile"), _currentTotal, LastError, hr);
						break;
					}
				}
				else
				{
					DWORD cbTransfer = 0;
					DWORD dwFlags = 0;
					BOOL bgor = ::WSAGetOverlappedResult(
						(SOCKET)_fileShadow->NativeHandle(),//_In_   SOCKET s,
						&_overlapped,//_In_   LPWSAOVERLAPPED lpOverlapped,
						&cbTransfer,//_Out_  LPDWORD lpcbTransfer,
						TRUE,//_In_   BOOL fWait,
						&dwFlags//,//_Out_  LPDWORD lpdwFlags
					);
					if (!bgor)
					{
						LastError = _fileShadow->GetLastError();
						hr = HRESULT_FROM_WIN32(LastError);
						if (LastError != WSA_IO_PENDING)
						{
							//Error(LastError, nullptr);
							SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("SocketIO::WriteFile"), _currentTotal, LastError, hr);
							break;
						}
					}
					transffered = cbTransfer;
					_step = 1;
					goto START_SWICTH;
				}
			}
			break;
			//////////////////////////////////////////////////////////////////////////
			case 1:
			{
				_currentTransffered += transffered;
				if (_currentTransffered < _currentTotal)
				{
					_step = 1;

					ZeroMemory(&_overlapped, sizeof(OVERLAPPED));

					//long left = _header.dataLen - _curentTransffered;
					//long len = left < _BUFFER_SIZE / 4 ? left : _BUFFER_SIZE / 4;

					bool rr = _fileShadow->WriteFile(
						_ioBuffer + _currentTransffered,//_Out_        LPVOID lpBuffer,
						_currentTotal - _currentTransffered,//_In_         DWORD nNumberOfBytesToRead,
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
					else
					{
						DWORD cbTransfer = 0;
						DWORD dwFlags = 0;
						BOOL bgor = ::WSAGetOverlappedResult(
							(SOCKET)_fileShadow->NativeHandle(),//_In_   SOCKET s,
							&_overlapped,//_In_   LPWSAOVERLAPPED lpOverlapped,
							&cbTransfer,//_Out_  LPDWORD lpcbTransfer,
							TRUE,//_In_   BOOL fWait,
							&dwFlags//,//_Out_  LPDWORD lpdwFlags
						);
						if (!bgor)
						{
							LastError = _fileShadow->GetLastError();
							hr = HRESULT_FROM_WIN32(LastError);
							if (LastError != WSA_IO_PENDING)
							{
								//Error(LastError, nullptr);
								SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("SocketIO::WriteFile"), _currentTotal, LastError, hr);
								break;
							}
						}
						transffered = cbTransfer;

						_step = 1;
						goto START_SWICTH;
					}
				}
				else
				{
					_step = 0;
					goto START_SWICTH;
				}

			}
			break;
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

		virtual HRESULT OnError(long error, LPOVERLAPPED lpoverlapped)override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//unique_lock<mutex> lock{_errorMtx};
			//////////////////////////////////////////////////////////////////////////
			_cb->StopProvider();
			_cb->StopConsumer();
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