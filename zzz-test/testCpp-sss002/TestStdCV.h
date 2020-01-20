#pragma once

#include <tchar.h>
#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <vector>
#include <iostream>

namespace TestCpp
{
	template<class T>
	class CSimpleCircleBuffer
	{
	private:
		std::mutex _mutex;
		std::condition_variable _cvData;
		std::condition_variable _cvBuffer;
		std::vector<T> _buffer;
		int _start{ 0 };
		int _end{ 0 };
	public:
		CSimpleCircleBuffer(int bufferSize):
			_buffer{ bufferSize }
		{
		}


		T Get()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_cvData.wait(lock, [this]() {return (_end - _start) > 0; });
			if (_start >= _buffer.size())
			{
				_start -= _buffer.size();
				_end -= _buffer.size();
			}
			T rr = _buffer[_start];
			++_start;
			lock.unlock();
			_cvBuffer.notify_all();
			return rr;
		}

		void Put(const T & data)
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_cvBuffer.wait(lock, [this]() {return (_end - _start) < _buffer.size(); });
			_buffer[_end % _buffer.size()] = data;
			++_end;
			lock.unlock();
			_cvData.notify_all();
		}


	
	};


	class CSimpleCircleBufferTest
	{

	public:
		static void Case0()
		{
			const int BUFFER_SIZE = 23;
			CSimpleCircleBuffer<int> cb(BUFFER_SIZE);

			std::thread t1
			{
				[&cb]()
			{
				for (int ii = 0; ii < 1'000; ++ii)
				{
					cb.Put(ii);
					//std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
				cb.Put(-1);
			}
			};

			std::thread t2
			{
				[&cb]()
			{
				long long sum = 0;
				long long count = 0;
				for (;;)
				{
					int data = cb.Get();
					if (data == -1)
					{
						break;
					}

					sum += data;
					++count;
					std::cout << "get data [" << data << "]." << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}

				std::cout << "sum is " << sum << ", and count is " << count << "." << std::endl;
			}
			};


			t1.join();
			t2.join();
		}

	};
}