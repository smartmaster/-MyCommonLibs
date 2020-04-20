#pragma once

#include <vector>
#include <iostream>
#include "two-stack-queue.h"

namespace SmartLib
{
	class StackQueueTest
	{
	public:
		static void Case0()
		{
			StackQueue que;
			vector<long> in;
			vector<long> out;
			for (long ii = 1; ii <= 5; ++ ii)
			{
				que.Enqueue(ii);
				in.push_back(ii);
			}

			for (long ii = 0; ii < 2; ++ ii)
			{
				long val = que.Dequeue();
				out.push_back(val);
			}


			for (long ii = 6; ii < 10; ++ ii)
			{
				que.Enqueue(ii);
				in.push_back(ii);
			}


			while (que.Size())
			{
				long val = que.Dequeue();
				out.push_back(val);
			}


			if (in == out)
			{
				for (auto data : in)
				{
					cout << data << ' ';
				}
				cout << endl;
			}
			else
			{
				cout << "ERROR" << endl;
			}
		}
	};
}