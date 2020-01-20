#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace SmartLib
{
	using namespace ::std;

	class TestCV
	{
		mutex _mutex;
		condition_variable _cvData;
		condition_variable _cvBuffer;
		volatile bool _stop{ false };
		int _count{ 0 };
		const int MAX{ 10 };

	public:
		TestCV()
		{
			srand(time(nullptr));
		}

		void Provider()
		{
			unique_lock<mutex> lock{ _mutex };
			while (_count == MAX && !_stop)
			{
				_cvBuffer.wait(lock);
			}

			if (_stop)
			{
				return;
			}

			this_thread::sleep_for(chrono::milliseconds(rand() % 1000));
			++_count;
			if (_count > MAX)
			{
				throw logic_error("error in Provider");
			}
			cout << _count << ", " << this_thread::get_id() << ", " << __FUNCTION__ << endl;
			_cvData.notify_all();
		}

		void Consumer()
		{
			unique_lock<mutex> lock{ _mutex };

			while (0 == _count && !_stop)
			{
				_cvData.wait(lock);
			}

			if (_stop)
			{
				return;
			}

			this_thread::sleep_for(chrono::milliseconds(rand() % 1000));
			--_count;
			if (_count < 0)
			{
				throw logic_error("error in consumer");
			}
			cout << _count << ", " << this_thread::get_id() << ", " << __FUNCTION__ << endl;
			_cvBuffer.notify_all();
		}

		void Stop()
		{
			_stop = true;
			_cvData.notify_all();
			_cvBuffer.notify_all();
		}

		void RunProvider()
		{
			while (!_stop)
			{
				Provider();
			}
		}

		void RunConsumer()
		{
			while (!_stop)
			{
				Consumer();
			}
		}

		void Test()
		{
			thread c1{ [this]() {RunConsumer(); } };
			thread c2{ [this]() {RunConsumer(); } };
			thread c3{ [this]() {RunConsumer(); } };

			thread p1{ [this]() {RunProvider(); } };
			thread p2{ [this]() {RunProvider(); } };
			thread p3{ [this]() {RunProvider(); } };

			this_thread::sleep_for(chrono::minutes(2));

			for (int ii = 0; ii < 6; ++ii)
			{
				Stop();
			}

			c1.join();
			c2.join();
			c3.join();

			p1.join();
			p2.join();
			p3.join();
		}
	};
}