#pragma once
#include "partition-array-for-maximum-sum.h"

#include <vector>
#include <iostream>

namespace SmartLib
{
	using namespace ::std;

	class partition_array_for_maximum_sum_test
	{
	public:
		static void Case0()
		{
			vector<int> data{ 1, 15, 7, 9, 2, 5, 10 };
			long len = 3;
			long res = SmartLib::partition_array_for_maximum_sum::maxSumAfterPartitioning(data, len);
			cout << "the result is " << res << endl;
			return;
		}
	};
}

