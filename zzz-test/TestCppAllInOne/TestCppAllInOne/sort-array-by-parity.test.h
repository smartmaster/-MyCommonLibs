#pragma once
#include "sort-array-by-parity.h"
#include <vector>

namespace SmartLib
{
	using namespace ::std;

	class sort_array_by_parity_test
	{
	public:
		static void Case0()
		{
			vector<int> vec{ 1,0,3,2 };
			auto vecnew = SmartLib::Solution_sort_array_by_parity::sortArrayByParity(vec);

		}
	};
}