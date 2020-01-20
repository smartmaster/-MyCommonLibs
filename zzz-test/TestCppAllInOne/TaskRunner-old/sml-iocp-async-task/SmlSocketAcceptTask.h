#pragma once

#include <WinSock2.h>
#include <MSWSock.h>

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "../SmlCircularBuffer.h"


#include "SmlITask.h"
#include "SmlIFile.h"
#include "SmlTaskRunnner.h"
#include "SmlSimpleEvent.h"
#include "SmlSocketIOTask.h"
#include "SmlSimpleHandlerTask.h"


namespace SmartLib
{
	class SocketAcceptAsyncTask : public IAsyncTask
	{
	private:
		ADDRINFOW _addrinfor{ 0 }; /***INCOMMING***/ /***INTERNAL***/
		RefPtr<IFile*> _file; /***INCOMMING***//***INTERNAL***/
		RefPtr<TaskRunner> _taskRunner; /***INCOMMING***/

		RefPtr<IFile*> _acceptSocket; /***INTERNAL***/

		IAsyncTask** _self{ nullptr };

		IFile* _fileShadow{ nullptr };

		SOCKADDR_STORAGE _addrs[2];
		LPFN_ACCEPTEX _func_AcceptEx{ nullptr };


		OVERLAPPED _overlapped{ 0 }; /***INTERNAL***/
		long _step{ 0 }; /***INTERNAL***/
		SimpleEvent _event; /***INTERNAL***/

	public:
		virtual ~SocketAcceptAsyncTask() override {};

		SocketAcceptAsyncTask(const ADDRINFOW* paddrinfor, RefPtr<IFile*> file, RefPtr<TaskRunner> taskRunner) :
			_file{move(file)},
			_taskRunner{ move(taskRunner) }
		{
			CopyMemory(&_addrinfor, paddrinfor, sizeof(ADDRINFOW));

			if (_file)
			{
				_fileShadow = *_file.Ptr();
			}
		}


		virtual void RefSelf(IAsyncTask** self) override
		{
			_self = self;
		}

		virtual HRESULT Create()  override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			ZeroMemory(&_overlapped, sizeof(OVERLAPPED));
			ZeroMemory(_addrs, sizeof(_addrs));

			GUID guid = WSAID_ACCEPTEX;
			DWORD cbBytesReturned = 0;
			int ret = ::WSAIoctl(
				(SOCKET)_fileShadow->NativeHandle(),//_In_   SOCKET s,
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
				LastError = _fileShadow->GetLastError();
				hr = HRESULT_FROM_WIN32(LastError);
				SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("WSAIoctl"), LastError, hr);
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

		virtual HRESULT Next(long transffered, LPOVERLAPPED lpoverlapped)  override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;


		SWITCH_START:
			switch (_step)
			{
			case 0:
			{
				_step = 1;

				SOCKET acceptSocket = WSASocket(
					_addrinfor.ai_family, 
					_addrinfor.ai_socktype, 
					_addrinfor.ai_protocol, 
					nullptr, 0, WSA_FLAG_OVERLAPPED);
				if (INVALID_SOCKET == acceptSocket)
				{
					LastError = _fileShadow->GetLastError();
					hr = HRESULT_FROM_WIN32(LastError);
					SML_LOG_LINE(TEXT("api=[%s], error=[%d], hresult=[0x%08x]"), TEXT("WSASocket"), LastError, hr);
					Error(0, nullptr);
					break;
				}

				_acceptSocket = RefPtr<IFile*>::Make(new SocketFile{ acceptSocket });
				_acceptSocket.SetDispose( Disposer::DeletePtr<IFile> );


				ZeroMemory(_addrs, sizeof(_addrs));
				ZeroMemory(&_overlapped, sizeof(_overlapped));
				BOOL ba = _func_AcceptEx(
					(SOCKET)(_fileShadow->NativeHandle()),//_In_   SOCKET sListenSocket,
					(SOCKET)((*_acceptSocket)->NativeHandle()),//_In_   SOCKET sAcceptSocket,
					(PVOID)_addrs,//_In_   PVOID lpOutputBuffer,
					0,//_In_   DWORD dwReceiveDataLength,
					sizeof(SOCKADDR_STORAGE),//_In_   DWORD dwLocalAddressLength,
					sizeof(SOCKADDR_STORAGE),//_In_   DWORD dwRemoteAddressLength,
					nullptr,//_Out_  LPDWORD lpdwBytesReceived,
					&_overlapped//,//_In_   LPOVERLAPPED lpOverlapped
				);
				if (!ba)
				{
					LastError = _fileShadow->GetLastError();
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
				//////////////////////////////////////////////////////////////////////////
				//TODO: handler task here somewhere


				//////////////////////////////////////////////////////////////////////////
				RefPtr<CircularBuffer> spcbRead = RefPtr<CircularBuffer>::Make(BUFFER_SIZE::_CIRCULAR_BUFFER_SIZE);
				RefPtr<IAsyncTask*> reader = RefPtr<IAsyncTask*>::Make(new SocketReadAsyncTask{ _acceptSocket, spcbRead, _taskRunner });
				reader.SetDispose(Disposer::DeletePtr<IAsyncTask>);
				IAsyncTask::RefSelf(reader);


				//////////////////////////////////////////////////////////////////////////
				RefPtr<CircularBuffer> spcbWrite = RefPtr<CircularBuffer>::Make(BUFFER_SIZE::_CIRCULAR_BUFFER_SIZE);
				RefPtr<IAsyncTask*> writer = RefPtr<IAsyncTask*>::Make(new SocketWriteAsyncTask{ _acceptSocket, spcbWrite, _taskRunner });
				writer.SetDispose(Disposer::DeletePtr<IAsyncTask>);
				IAsyncTask::RefSelf(writer);


				//////////////////////////////////////////////////////////////////////////
				RefPtr<IAsyncTask*> handler = RefPtr<IAsyncTask*>::Make(new SimpleHandlerTask{ spcbRead, spcbWrite, _taskRunner });
				handler.SetDispose(Disposer::DeletePtr<IAsyncTask>);
				IAsyncTask::RefSelf(handler);

				//////////////////////////////////////////////////////////////////////////
				_acceptSocket.Attach(nullptr);

				//////////////////////////////////////////////////////////////////////////
				hr = (*reader)->Create();
				if (FAILED(hr))
				{
					Error(LastError, nullptr);
					SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("Create"), sizeof(_addrs), LastError, hr);
					break;
				}

				hr = (*writer)->Create();
				if (FAILED(hr))
				{
					Error(LastError, nullptr);
					SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("Create"), sizeof(_addrs), LastError, hr);
					break;
				}


				hr = (*handler)->Create();
				if (FAILED(hr))
				{
					Error(LastError, nullptr);
					SML_LOG_LINE(TEXT("api=[%s], size=[%d], error=[%d], hresult=[0x%08x]"), TEXT("Create"), sizeof(_addrs), LastError, hr);
					break;
				}

				//////////////////////////////////////////////////////////////////////////
				_step = 0;
				goto SWITCH_START;
			}
			break;
			}

			return hr;
		}

		virtual HRESULT Error(long error, LPOVERLAPPED lpoverlapped)  override
		{
			HRESULT hr = S_OK;
			DWORD LastError = 0;

			BEGIN_BLOCK(0);
			//////////////////////////////////////////////////////////////////////////
			_event.Set();
			if (_self)
			{
				RefPtr<IAsyncTask*> self;
				self.Attach(_self);
				_self = nullptr;
				_taskRunner->CleanupTask(self);
			}
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