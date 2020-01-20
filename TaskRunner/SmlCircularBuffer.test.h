#pragma once

#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "SmlCircularBuffer.h"

namespace SmartLib
{
	using namespace ::std;

	class CircularBufferTest
	{
	private:
		CircularBuffer cb{ 37 };
	public:

		CircularBufferTest()
		{
			srand(time(nullptr));
		}

		void Provider()
		{
			long long data = 1;
			for (;;)
			{
				long len = cb.push_back_all((const char*)&data, sizeof(data));
				if (len < sizeof(data))
				{
					break;
				}
				++data;

				this_thread::sleep_for(chrono::milliseconds(rand() % 50));
			}
		}

		void Consumer()
		{
			long long expected = 1;
			for (;;)
			{
				long long data = 0;
				long len = cb.pop_front_all((char*)&data, sizeof(data));
				if (len < sizeof(data))
				{
					break;
				}

				if (data != expected)
				{
					cout << "unexpected!!!!!!!! " << data << ", " << expected << endl;
					assert(false);
				}

				cout << "expected " << data << ", " << expected << endl;

				++expected;

				this_thread::sleep_for(chrono::milliseconds(rand() % 50));
			}
		}
		

		void Run()
		{
			thread provider{ [this] {Provider(); } };
			thread consumer{ [this] {Consumer(); } };

			this_thread::sleep_for(chrono::minutes(1));

			cb.StopProvider();
			this_thread::sleep_for(chrono::seconds(10));
			cb.StopConsumer();

			provider.join();
			consumer.join();

			long lastRemaining = cb.GetRemainingDataCount();

			return;
		}


		static void Case0()
		{
			CircularBufferTest tt;
			tt.Run();
		}

	};
}