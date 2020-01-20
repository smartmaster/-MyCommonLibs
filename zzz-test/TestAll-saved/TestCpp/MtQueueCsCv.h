#pragma once
#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <Synchapi.h>
#include <Processthreadsapi.h>

#include <vector>
#include <memory>

namespace SmartLib
{
	template<typename T>
	class IQueue
	{
	public:
		virtual void Add(T item) = 0;
		virtual T Remove() = 0;
		virtual void Remains(std::vector<T> & items) = 0;
		virtual void Stop() = 0;

		virtual ~IQueue() {};
	};


	template<typename T>
	class CMTqueue : 
		public IQueue<T>
	{
	private:
		CONDITION_VARIABLE _bufferCV;
		CONDITION_VARIABLE _itemCV;
		CRITICAL_SECTION   _lock;
		std::vector<T> _items;
		size_t _offset{ 0 };
		size_t _count{ 0 };
		volatile bool _stop{ false };
		const size_t _totalCount;



	public:
		CMTqueue(size_t totalCount) :
			_totalCount(totalCount),
			_items(totalCount)
		{
			InitializeConditionVariable(&_bufferCV);
			InitializeConditionVariable(&_itemCV);
			const DWORD SPIN_COUNT = 0x400;
			InitializeCriticalSectionAndSpinCount(&_lock, SPIN_COUNT);
		}


		virtual ~CMTqueue() override
		{
			DeleteCriticalSection(&_lock);
		}


		virtual void Add(T item) override
		{
			EnterCriticalSection(&_lock);

			while (_items.size() == _count && !_stop)
			{
				SleepConditionVariableCS(&_bufferCV, &_lock, INFINITE);
			}

			if (!_stop)
			{
				_items[(_offset + _count) % _items.size()] = item;
				++_count;
			}
			
			LeaveCriticalSection(&_lock);

			WakeAllConditionVariable(&_itemCV);
		}

		virtual T Remove() override
		{
			EnterCriticalSection(&_lock);

			while (0 == _count && !_stop)
			{
				SleepConditionVariableCS(&_itemCV, &_lock, INFINITE);
			}

			T rv = T{};
			if (!_stop)
			{
				rv = _items[_offset];
				--_count;
				++_offset;
				if (_offset == _items.size())
				{
					_offset = 0;
				}
			}

			LeaveCriticalSection(&_lock);

			WakeAllConditionVariable(&_bufferCV);

			return rv;
		}


		virtual void Remains(std::vector<T> & items)
		{
			EnterCriticalSection(&_lock);
			for (size_t ii = 0; ii < _count; ++ ii)
			{
				items.push_back(_items[(_offset + ii) % _items.size()]);
			}
			_offset = 0;
			_count = 0;
			LeaveCriticalSection(&_lock);
		}

		virtual void Stop() override
		{
			_stop = true;
			WakeAllConditionVariable(&_itemCV);
			WakeAllConditionVariable(&_bufferCV);
		}
	};


	class CMTqueueTest
	{
		
		volatile bool _stop{ false };

		LONG _data{ 0 };

		CRITICAL_SECTION _lock;

	public:
		std::unique_ptr<CMTqueue<LONG>> _queue;

	public:
		
		CMTqueueTest()
		{
			_queue = std::make_unique<CMTqueue<LONG>>(7);
			InitializeCriticalSectionAndSpinCount(&_lock, 0x400);
		}


		void Provider()
		{
			while (!_stop)
			{
				int item = InterlockedIncrement(&_data);
				_queue->Add(item);

				EnterCriticalSection(&_lock);
				_ftprintf_s(stdout, TEXT("Provider: tid %08d, item %08d") TEXT("\r\n"), GetCurrentThreadId(), item);
				LeaveCriticalSection(&_lock);

			}
		}

		void Consumer()
		{
			while (!_stop)
			{
				int item = _queue->Remove();

				EnterCriticalSection(&_lock);
				_ftprintf_s(stdout, TEXT("Consumer: tid %08d, item %08d") TEXT("\r\n"), GetCurrentThreadId(), item);
				LeaveCriticalSection(&_lock);
			}
		}

		void Stop()
		{
			_stop = true;
			_queue->Stop();
		}


	static	DWORD WINAPI ThreadProcProvider(
			_In_ LPVOID lpParameter
		)
		{
			CMTqueueTest * p = (CMTqueueTest *)lpParameter;
			p->Provider();
			return 0;
		}

	static	DWORD WINAPI ThreadProcConsumer(
			_In_ LPVOID lpParameter
		)
		{
			CMTqueueTest * p = (CMTqueueTest *)lpParameter;
			p->Consumer();
			return 0;
		}


		static void Run()
		{
			std::unique_ptr<CMTqueueTest> p = std::make_unique<CMTqueueTest>();

			const int consumerCount = 7;
			const int providerCount = 7;
			

			std::vector<HANDLE> handles(providerCount + consumerCount);
			
			for (int ii = 0; ii < consumerCount; ++ii)
			{
				DWORD                ThreadId = 0;
				handles[ii] = CreateThread(
					nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
					0,//_In_      SIZE_T                 dwStackSize,
					(LPTHREAD_START_ROUTINE)(CMTqueueTest::ThreadProcConsumer),//_In_      LPTHREAD_START_ROUTINE lpStartAddress,
					p.get(),//_In_opt_  LPVOID                 lpParameter,
					0,//_In_      DWORD                  dwCreationFlags,
					&ThreadId//_Out_opt_ LPDWORD                lpThreadId
				);
			}


			for (int ii = consumerCount; ii < consumerCount + providerCount; ++ii)
			{
				DWORD                ThreadId = 0;
				handles[ii] = CreateThread(
					nullptr,//_In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
					0,//_In_      SIZE_T                 dwStackSize,
					(LPTHREAD_START_ROUTINE)(CMTqueueTest::ThreadProcProvider),//_In_      LPTHREAD_START_ROUTINE lpStartAddress,
					p.get(),//_In_opt_  LPVOID                 lpParameter,
					0,//_In_      DWORD                  dwCreationFlags,
					&ThreadId//_Out_opt_ LPDWORD                lpThreadId
				);
			}


			_ftprintf_s(stdout, TEXT("Press key to stop...") TEXT("\r\n"));
			_gettch();


			p->Stop();
			WaitForMultipleObjects(handles.size(), &handles[0], TRUE, INFINITE);
			for (HANDLE th : handles)
			{
				CloseHandle(th);
			}

			std::vector<LONG> remaining;
			p->_queue->Remains(remaining);
			_ftprintf_s(stdout, TEXT("The remains are ...") TEXT("\r\n"));
			for (LONG val : remaining)
			{
				_ftprintf_s(stdout, TEXT("%08d") TEXT("\r\n"), val);
			}
		}

	};
}