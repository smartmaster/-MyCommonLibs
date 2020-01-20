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
		RefPtr<CircularBuffer> _cbRead;/***INCOMMING***//***INTERNAL***/
		RefPtr<TaskRunner> _taskRunner; /***INCOMMING***/

		//IAsyncTask** _self{ nullptr };

		IFile* _fileShadow{ nullptr };


		constexpr static const long _IO_BUFFER_SIZE{ BUFFER_SIZE::_IO_BUFFER_SIZE };
		char* _ioReadBuffer{ nullptr }; /***INTERNAL***//***CLEANUP***/
		ScopeResource<> _sr_ioReadBuffer{
			[this]()
			{
				if (_ioReadBuffer)
				{
					::VirtualFree(_ioReadBuffer, 0, MEM_RELEASE);
					_ioReadBuffer = nullptr;
				}
			}
		};



		OVERLAPPED _overlappedRead{ 0 }; /***INTERNAL***/
		long _stepRead{ 0 }; /***INTERNAL***/
		SimpleEvent _event; /***INTERNAL***/

		//mutex _errorMtx; /***INTERNAL***/
		//mutex _ioMtx; /***INTERNAL***/

		Vector<IAsyncTask*, false, 2> _relatedTasks;

	public:
		virtual ~SocketReadAsyncTask() {};

		SocketReadAsyncTask(RefPtr<IFile*> file, RefPtr<CircularBuffer> cbReceive, RefPtr<TaskRunner> taskRunner) :
			_file{ move(file) },
			_cbRead{ move(cbReceive) },
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

		virtual HRESULT CreateForRead() override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_ioReadBuffer = (char*)::VirtualAlloc(nullptr, _IO_BUFFER_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (nullptr == _ioReadBuffer)
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
			hr = _taskRunner->StartTask((IAsyncTask*)(this), &_overlappedRead);
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

		virtual HRESULT OnNextForRead(long transffered, LPOVERLAPPED lpoverlapped)override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);

			//////////////////////////////////////////////////////////////////////////
			unique_lock<mutex> lock{_ioMtx};
			//////////////////////////////////////////////////////////////////////////
		START_SWICTH:
			switch (_stepRead)
			{
				//////////////////////////////////////////////////////////////////////////
			case  0:
			{
				_stepRead = 1;
				ZeroMemory(&_overlappedRead, sizeof(OVERLAPPED));

				bool rr = _fileShadow->ReadFile(
					_ioReadBuffer,//_Out_        LPVOID lpBuffer,
					_IO_BUFFER_SIZE,//_In_         DWORD nNumberOfBytesToRead,
					nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
					&_overlappedRead//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
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
					_stepRead = 1;
					goto START_SWICTH;
				}
			}
			break;
			//////////////////////////////////////////////////////////////////////////
			case  1:
			{
				//_receiveHeader.dataLen = transffered;
				//_cbReceive.ProvideAll((const char*)&_receiveHeader, sizeof(_receiveHeader));
				long len = _cbRead->push_back_all(_ioReadBuffer, transffered);
				if (len < transffered)
				{
					//Error(0, nullptr);
					break;
				}
				else
				{
					_stepRead = 0;
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
			_cbRead->StopProvider();
			_cbRead->StopConsumer();
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
		RefPtr<CircularBuffer> _cbWrite;/***INCOMMING***//***INTERNAL***/
		RefPtr<TaskRunner> _taskRunner; /***INCOMMING***/

		//IAsyncTask** _self{ nullptr };
		IFile* _fileShadow{ nullptr };

		constexpr static const long _IO_BUFFER_SIZE{ BUFFER_SIZE::_IO_BUFFER_SIZE };
		char* _ioWriteBuffer{ nullptr }; /***INTERNAL***//***CLEANUP***/
		ScopeResource<> _sr_ioWriteBuffer{
			[this]()
			{
				if (_ioWriteBuffer)
				{
					::VirtualFree(_ioWriteBuffer, 0, MEM_RELEASE);
					_ioWriteBuffer = nullptr;
				}
			}
		};

		OVERLAPPED _overlappedWrite{ 0 }; /***INTERNAL***/
		long _stepWrite{ 0 }; /***INTERNAL***/
		SimpleEvent _event; /***INTERNAL***/

		long _currentTransfferedWrite{ 0 };
		long _currentTotalWrite{ 0 };

		//mutex _ioMtx; /***INTERNAL***/

	public:
		virtual ~SocketWriteAsyncTask() {};

		SocketWriteAsyncTask(RefPtr<IFile*> file, RefPtr<CircularBuffer> cbSend, RefPtr<TaskRunner> taskRunner) :
			_file{ move(file) },
			_cbWrite{ move(cbSend) },
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

		virtual HRESULT CreateForWrite() override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_ioWriteBuffer = (char*)::VirtualAlloc(nullptr, _IO_BUFFER_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (nullptr == _ioWriteBuffer)
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
			hr = _taskRunner->StartTask((IAsyncTask*)(this), &_overlappedWrite);
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

		virtual HRESULT OnNextForWrite(long transffered, LPOVERLAPPED lpoverlapped)override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			unique_lock<mutex> lock{ _ioMtx };

			//////////////////////////////////////////////////////////////////////////
		START_SWICTH:
			switch (_stepWrite)
			{
				//////////////////////////////////////////////////////////////////////////
			case 0:
			{
				_stepWrite = 1;

				_currentTransfferedWrite = 0;
				_currentTotalWrite = _cbWrite->pop_front(_ioWriteBuffer, _IO_BUFFER_SIZE);
				if (_currentTotalWrite < 0)
				{
					//Error(0, nullptr);
					//if (_taskRunner->CheckCleanupFlag(this))
					//{
					//	_taskRunner->CleanupTask(this);
					//}
					break;
				}

				ZeroMemory(&_overlappedWrite, sizeof(OVERLAPPED));
				//len = _BUFFER_SIZE / 4; //debug!!! 
				bool rr = _fileShadow->WriteFile(
					_ioWriteBuffer,//_Out_        LPVOID lpBuffer,
					_currentTotalWrite, //_In_         DWORD nNumberOfBytesToRead,
					nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
					&_overlappedWrite//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
				);
				if (!rr)
				{
					LastError = _fileShadow->GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					if (LastError != WSA_IO_PENDING)
					{
						//Error(LastError, nullptr);
						SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("SocketIO::WriteFile"), _currentTotalWrite, LastError, hr);
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
							SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("SocketIO::WriteFile"), _currentTotalWrite, LastError, hr);
							break;
						}
					}
					transffered = cbTransfer;
					_stepWrite = 1;
					goto START_SWICTH;
				}
			}
			break;
			//////////////////////////////////////////////////////////////////////////
			case 1:
			{
				_currentTransfferedWrite += transffered;
				if (_currentTransfferedWrite < _currentTotalWrite)
				{
					_stepWrite = 1;

					ZeroMemory(&_overlappedWrite, sizeof(OVERLAPPED));

					//long left = _header.dataLen - _curentTransffered;
					//long len = left < _BUFFER_SIZE / 4 ? left : _BUFFER_SIZE / 4;

					bool rr = _fileShadow->WriteFile(
						_ioWriteBuffer + _currentTransfferedWrite,//_Out_        LPVOID lpBuffer,
						_currentTotalWrite - _currentTransfferedWrite,//_In_         DWORD nNumberOfBytesToRead,
						nullptr,//_Out_opt_    LPDWORD lpNumberOfBytesRead,
						&_overlappedWrite//,//_Inout_opt_  LPOVERLAPPED lpOverlapped
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
								SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("SocketIO::WriteFile"), _currentTotalWrite, LastError, hr);
								break;
							}
						}
						transffered = cbTransfer;

						_stepWrite = 1;
						goto START_SWICTH;
					}
				}
				else
				{
					_stepWrite = 0;
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
			_cbWrite->StopProvider();
			_cbWrite->StopConsumer();
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