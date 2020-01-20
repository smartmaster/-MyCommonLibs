#pragma once



#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "SmlMTQueue.h"

namespace SmartLib
{
	using namespace ::std;

	class MTQueueTest
	{
	private:
		MTQueue<string, 9> queque;
	public:
		MTQueueTest()
		{
			srand(time(nullptr));
		}
		void Provider()
		{
			long long data = 1;
			for (;;)
			{
				long len = 0;
				string temp = to_string(data);
				if (data & 1)
				{
					len = queque.push_back(move(temp));
				}
				else
				{
					len = queque.push_back(temp);
				}
				
				
				if (len < 0)
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
				string data;
				long len = queque.pop_front(data);
				if (len < 0)
				{
					break;
				}

				if (data != to_string(expected))
				{
					cout << "unexpected!!!!!!!! " << data << ", " << expected << endl;
					assert(false);
				}

				cout << "expected " << data << ", " << expected << endl;
				//cout << data << endl;

				++expected;

				this_thread::sleep_for(chrono::milliseconds(rand() % 50));
			}
		}


		void ConsumerM()
		{
			long long expected = 1;
			for (;;)
			{
				string data;
				long len = queque.pop_front(data);
				if (len < 0)
				{
					break;
				}

				//if (data != to_string(expected))
				//{
				//	cout << "unexpected!!!!!!!! " << data << ", " << expected << endl;
				//	assert(false);
				//}

				//cout << "expected " << data << ", " << expected << endl;
				cout << data << endl;

				++expected;

				this_thread::sleep_for(chrono::milliseconds(rand() % 50));
			}
		}


		void Run()
		{
			thread provider1{ [this] {Provider(); } };
			//thread provider2{ [this] {Provider(); } };
			//thread provider3{ [this] {Provider(); } };

			thread consumer1{ [this] {Consumer(); } };
			//thread consumer2{ [this] {Consumer(); } };
			//thread consumer3{ [this] {Consumer(); } };

			this_thread::sleep_for(chrono::minutes(1));

			for (long ii = 0; ii < 3; ii++)
			{
				queque.StopProvider();
			}
			
			this_thread::sleep_for(chrono::seconds(10));

			for (long ii = 0; ii < 3; ii++)
			{
				queque.StopConsumer();
			}
			

			provider1.join();
			//provider2.join();
			//provider3.join();

			consumer1.join();
			//consumer2.join();
			//consumer3.join();

			long lastRemaining = queque.GetRemainingDataCount();

			return;
		}


		void RunM()
		{
			thread provider1{ [this] {Provider(); } };
			thread provider2{ [this] {Provider(); } };
			thread provider3{ [this] {Provider(); } };

			thread consumer1{ [this] {ConsumerM(); } };
			thread consumer2{ [this] {ConsumerM(); } };
			thread consumer3{ [this] {ConsumerM(); } };

			this_thread::sleep_for(chrono::minutes(1));

			for (long ii = 0; ii < 3; ii++)
			{
				queque.StopProvider();
			}

			this_thread::sleep_for(chrono::seconds(10));

			for (long ii = 0; ii < 3; ii++)
			{
				queque.StopConsumer();
			}


			provider1.join();
			provider2.join();
			provider3.join();

			consumer1.join();
			consumer2.join();
			consumer3.join();

			long lastRemaining = queque.GetRemainingDataCount();

			return;
		}

		static void Case1()
		{
			MTQueueTest tt;
			tt.RunM();
		}


		static void Case0()
		{
			MTQueueTest tt;
			tt.Run();
		}

	};
}

