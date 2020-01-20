#pragma once
#include "k-concatenation-maximum-sum.h"


namespace SmartLib
{
	class Solution_k_concatenation_maximum_sum_test
	{
	public:
		static void Case0()
		{
			vector<int> vec{ 1, 0, 4, 1, 4 };
			int k = 4;
			int sum = SmartLib::Solution_k_concatenation_maximum_sum::kConcatenationMaxSum(vec, k);
			return;
		}
	};
}