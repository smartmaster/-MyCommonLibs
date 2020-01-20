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

	class SocketIOAsyncTask : public IAsyncTask
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


		enum  class STEP_READ
		{
			START_READ,
			DONE_READ,
			START_PUSH,
		};
		STEP_READ _stepRead{ STEP_READ::START_READ }; /***INTERNAL***/
		OVERLAPPED _overlappedRead{ 0 }; /***INTERNAL***/
		SimpleEvent _event; /***INTERNAL***/

		long _currentPushTotal{ 0 };
		long _currentPush{ 0 };

		//mutex _errorMtx; /***INTERNAL***/
		//mutex _ioMtxRead; /***INTERNAL***/

		Vector<IAsyncTask*, false, 2> _relatedTasks;


		//////////////////////////////////////////////////////////////////////////
		//RefPtr<IFile*> _file; /***INCOMMING***//***INTERNAL***/
		RefPtr<CircularBuffer> _cbWrite;/***INCOMMING***//***INTERNAL***/
		//RefPtr<TaskRunner> _taskRunner; /***INCOMMING***/

		//IAsyncTask** _self{ nullptr };
		//IFile* _fileShadow{ nullptr };

		//constexpr static const long _IO_BUFFER_SIZE{ BUFFER_SIZE::_IO_BUFFER_SIZE };
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


		enum  class STEP_WRITE
		{
			START_POP,
			START_WRITE
		};
		STEP_WRITE _stepWrite{ STEP_WRITE::START_POP }; /***INTERNAL***/

		OVERLAPPED _overlappedWrite{ 0 }; /***INTERNAL***/
		//SimpleEvent _event; /***INTERNAL***/

		long _currentTransfferedWrite{ 0 };
		long _currentTotalWrite{ 0 };

		//mutex _ioMtxWrite; /***INTERNAL***/


	private:
		virtual HRESULT CreateForRead() /*override*/
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

		virtual HRESULT OnNextForRead(long transffered, LPOVERLAPPED lpoverlapped)/*override*/
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);

			//////////////////////////////////////////////////////////////////////////
			//unique_lock<mutex> lock{ _ioMtxRead };
			//////////////////////////////////////////////////////////////////////////
		START_SWICTH:
			switch (_stepRead)
			{
				//////////////////////////////////////////////////////////////////////////
			case  STEP_READ::START_READ:
			{
				
				ZeroMemory(&_overlappedRead, sizeof(OVERLAPPED));

				_stepRead = STEP_READ::DONE_READ;
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
	
			}
			break;
			//////////////////////////////////////////////////////////////////////////
			case  STEP_READ::DONE_READ:
			{
				_currentPushTotal = transffered;
				_currentPush = 0;
				_stepRead = STEP_READ::START_PUSH;
				goto START_SWICTH;
				break;
			}
			break;
			case STEP_READ::START_PUSH:
			{
				//_receiveHeader.dataLen = transffered;
				//_cbReceive.ProvideAll((const char*)&_receiveHeader, sizeof(_receiveHeader));
				long len = _cbRead->push_back(_ioReadBuffer + _currentPush, _currentPushTotal - _currentPush, /*-1*/50);
				if (len == (long)CB_STATUS::_TIMEOUT)
				{
					_stepRead = STEP_READ::START_PUSH;
					_taskRunner->StartTask((IAsyncTask*)(this), &_overlappedRead);
					break;
				}
				else if (len  < 0)
				{
					break;
				}
				
				_currentPush += len;
				if (_currentPush == _currentPushTotal)
				{
					_stepRead = STEP_READ::START_READ;
					goto START_SWICTH;
					break;
				}
				else if (_currentPush < _currentPushTotal)
				{
					_stepRead = STEP_READ::START_PUSH;
					goto START_SWICTH;
					break;
				}
				else
				{
					assert(false);
				}

				
				//if (len < transffered)
				//{
				//	//Error(0, nullptr);
				//	break;
				//}
				//else
				//{
				//	//string str{_ioReadBuffer, (string::size_type)len};
				//	//cout << "Received: [" << str << "]" << endl;
				//	//ZeroMemory(_ioReadBuffer, len);
				//	_ioReadBuffer[0] = 0;
				//	_stepRead = STEP_READ::START_READ;
				//	goto START_SWICTH;
				//}
			}
			break;
			}
			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}

		virtual HRESULT CreateForWrite() /*override*/
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

		virtual HRESULT OnNextForWrite(long transffered, LPOVERLAPPED lpoverlapped)/*override*/
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//unique_lock<mutex> lock{ _ioMtxWrite };

			//////////////////////////////////////////////////////////////////////////
		START_SWICTH:
			switch (_stepWrite)
			{
				//////////////////////////////////////////////////////////////////////////
			case STEP_WRITE::START_POP:
			{
				//if (_cbWrite->IsStoppedNoLock())
				//{
				//	break;
				//}

				//if (_cbWrite->IsEmptyNoLock())
				//{
				//	//this_thread::sleep_for(chrono::milliseconds(50));
				//	
				//}


				
				long len = _cbWrite->pop_front(_ioWriteBuffer, _IO_BUFFER_SIZE, /*-1*/50);
				if (len == (long)CB_STATUS::_TIMEOUT)
				{
					_stepWrite = STEP_WRITE::START_POP;
					_taskRunner->StartTask(this, lpoverlapped);
					break;
				}
				else if (len < 0)
				{
					//Error(0, nullptr);
					//if (_taskRunner->CheckCleanupFlag(this))
					//{
					//	_taskRunner->CleanupTask(this);
					//}
					break;
				}

				_currentTransfferedWrite = 0;
				_currentTotalWrite = len;
				transffered = 0;
				_stepWrite = STEP_WRITE::START_WRITE;
				goto START_SWICTH;
				break;


			}
			break;
			//////////////////////////////////////////////////////////////////////////
			case STEP_WRITE::START_WRITE:
			{
				_currentTransfferedWrite += transffered;
				if (_currentTransfferedWrite < _currentTotalWrite)
				{
					ZeroMemory(&_overlappedWrite, sizeof(OVERLAPPED));

					//long left = _header.dataLen - _curentTransffered;
					//long len = left < _BUFFER_SIZE / 4 ? left : _BUFFER_SIZE / 4;
					_stepWrite = STEP_WRITE::START_WRITE;
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
					
				}
				else if(_currentTransfferedWrite == _currentTotalWrite)
				{
					_stepWrite = STEP_WRITE::START_POP;
					goto START_SWICTH;
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
		virtual ~SocketIOAsyncTask() {};

		SocketIOAsyncTask(RefPtr<IFile*> file, RefPtr<CircularBuffer> cbReceive, RefPtr<CircularBuffer> cbSend, RefPtr<TaskRunner> taskRunner) :
			_file{ move(file) },
			_cbRead{ move(cbReceive) },
			_cbWrite{ move(cbSend) },
			_taskRunner{ move(taskRunner) }//
		{
			if (_file)
			{
				_fileShadow = _file.Ref();
			}
		}

		virtual void AddRelatedTasks(IAsyncTask* task) override
		{
			_relatedTasks.push_back(task);
		}




		virtual HRESULT Create() override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			hr = CreateForRead();
			if (FAILED(hr))
			{
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateForRead"), LastError, hr);
				LEAVE_BLOCK(0);
			}

			hr = CreateForWrite();
			if (FAILED(hr))
			{
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("CreateForWrite"), LastError, hr);
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
			
			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			if (lpoverlapped == &_overlappedRead)
			{
				hr = OnNextForRead(transffered, lpoverlapped);
			}
			else if (lpoverlapped == &_overlappedWrite)
			{
				hr = OnNextForWrite(transffered, lpoverlapped);
			}
			else
			{
				assert(false);
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
			//unique_lock<mutex> lock{ _errorMtx };
			//////////////////////////////////////////////////////////////////////////
			_cbRead->StopProvider();
			_cbRead->StopConsumer();
			_cbWrite->StopProvider();
			_cbWrite->StopConsumer();
			//_fileShadow->Close();

			for (long ii = 0; ii < _relatedTasks.size(); ++ ii)
			{
				_relatedTasks[ii]->OnError(error, nullptr);
				if (_taskRunner->CleanupTaskFlag(_relatedTasks[ii]))
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
}