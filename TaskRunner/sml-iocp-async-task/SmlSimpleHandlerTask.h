#pragma once


#include "SmlCommon.h"
#include "SmlITask.h"
#include "SmlTaskRunnner.h"
#include "SmlSimpleEvent.h"

#include "../SmlCircularBuffer.h"



namespace SmartLib
{
	class SimpleHandlerTask : public IAsyncTask
	{
	private:
		RefPtr<CircularBuffer> _cbReceive; /***INCOMMING***/
		RefPtr<CircularBuffer> _cbSend; /***INCOMMING***/
		RefPtr<TaskRunner> _taskRunner; /***INCOMMING***/


		//IAsyncTask** _self{ nullptr };

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
			START_POP,
			START_PUSH,
			//WRITE_MORE,
		};
		STEP _step{ STEP::START_POP }; /***INTERNAL***/

		OVERLAPPED _overlapped{ 0 }; /***INTERNAL***/
		SimpleEvent _event; /***INTERNAL***/

		//mutex _ioMtxHanlder;

		long _currentPushTotal{ 0 };
		long _currentPush{ 0 };

		


	public:
		virtual ~SimpleHandlerTask() override {};


		SimpleHandlerTask(RefPtr<CircularBuffer> cbReceive, RefPtr<CircularBuffer> cbSend, RefPtr<TaskRunner> taskRunner) :
			_cbReceive{move(cbReceive)},
			_cbSend{ move(cbSend) },
			_taskRunner{ move(taskRunner) }
		{

		}


		virtual void AddRelatedTasks(IAsyncTask* task) override
		{
			
		}

		//virtual void RefSelf(IAsyncTask** self) override
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

			BEGIN_BLOCK(0);
			//unique_lock<mutex> lock{ _ioMtxHanlder };

			//////////////////////////////////////////////////////////////////////////
		START_SWITCH:
			switch (_step)
			{
			case STEP::START_POP:
			{
				long len = _cbReceive->pop_front(_ioBuffer, _IO_BUFFER_SIZE, /*-1*/50);
				if (len == (long)CB_STATUS::_TIMEOUT)
				{
					_step = STEP::START_POP;
					_taskRunner->StartTask(this, &_overlapped);
					break;
				}
				else if (len < 0)
				{
					//Error(0, nullptr);
					break;
				}

				_currentPush = 0;
				_currentPushTotal = len;
				_step = STEP::START_PUSH;
				goto START_SWITCH;
			}
			break;
			case STEP::START_PUSH:
			{
				long len = _cbSend->push_back(_ioBuffer + _currentPush, _currentPushTotal - _currentPush, /*-1*/50);
				if (len == (long)CB_STATUS::_TIMEOUT)
				{
					_step = STEP::START_PUSH;
					_taskRunner->StartTask(this, &_overlapped);
					break;
				}
				else if (len  < 0)
				{
					//Error(0, nullptr);
					break;
				}


				_currentPush += len;
				if (_currentPush == _currentPushTotal)
				{
					_step = STEP::START_POP;
					goto START_SWITCH;
					break;
				}
				else if (_currentPush < _currentPushTotal)
				{
					_step = STEP::START_PUSH;
					goto START_SWITCH;
					break;
				}
				else
				{
					assert(false);
				}

				//if (!_cbReceive->IsEmptyNoLock() && !_cbSend->IsFullNoLock())
				//{
				//	_step = 0;
				//	goto START_SWITCH;
				//}
				//else
				//{
				//	_taskRunner->StartTask((IAsyncTask*)(this), &_overlapped);
				//}
			}
			break;
			}

			//////////////////////////////////////////////////////////////////////////
			END_BLOCK(0);
			/***CLEANUPS***/
			return hr;
		}


		virtual HRESULT OnError(long error, LPOVERLAPPED lpoverlapped) override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			//_cbReceive->StopProvider();
			//_cbReceive->StopConsumer();
			//_cbSend->StopProvider();
			//_cbSend->StopConsumer();
			//_header.dataLen = -1;
			//_cb->push_back_all((const char*)&_header, sizeof(_header));
			_event.Set();
			//_taskRunner->CleanupTask((IAsyncTask*)(this));

			//if (_self)
			//{
			//	RefPtr<IAsyncTask*> self;
			//	self.Attach(_self);
			//	_self = nullptr;
			//	_taskRunner->CleanupTask((self));
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
