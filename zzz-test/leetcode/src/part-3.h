#pragma once


#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "sml-common.h"

namespace SmartLib::Part3
{
	using namespace ::std;
	class Partion3
	{
	public:

		static void P3(vector<int>& data)
		{
			int left = 0;
			int right = data.size() - 1;
			int mid = 0;

			while (mid <= right)
			{
				switch (data[mid])
				{
				case 0:
				{
					swap(data[mid], data[left]);
					++left;
					if (mid <= left)
					{
						mid = left;
					}
				}
				break;
				case 1:
				{
					++mid;
				}
				break;
				case 2:
				{
					swap(data[mid], data[right]);
					--right;
				}
				break;
				}
			}
		}

	public:
		static void Case0()
		{
			srand(time(nullptr));
			vector<int> data;
			for (int ii = 0; ii < 37; ++ ii)
			{
				data.push_back(rand() % 3);
			}
			Common::PrintIterator(cout, begin(data), end(data)) << endl;

			P3(data);

			Common::PrintIterator(cout, begin(data), end(data)) << endl;
		}

	public:
		static int D2I(double d)
		{
			return (int)(d + 0.01);
		}
		static vector<tuple<double, string>> Calc(vector<double>& nums)
		{
			if (nums.size() == 1)
			{
				return { {nums[0], to_string(D2I(nums[0]))} };
			}


		}
	};



}

