#pragma once

#include <iostream>

#include "binary-search.h"


namespace SmartLib
{
	using namespace ::std;

	class BinarySearchALgTest
	{
	public:
		static void Case0()
		{
			vector<long> data;
			for (long ii = 2; ii <= 100; ii += 2)
			{
				data.push_back(ii);
			}

			for (long ii = 1; ii <= 105; ii += 2)
			{
				long last = data.size() - 1;
				//long index = BinarySearchALg{ data }.BinarySearchLastSmaller(ii, 0, last);
				long index = BinarySearchALg{ data }.BinarySearchFirtLarger(ii, 0, last);
				if (index >= 0)
				{
					cout << ii << " " << data[index] << endl;
				}
				else
				{
					cout << "not found!" << endl;
				}
			}
		}

	};
}