#pragma once
#include "frog-jump.h"


namespace SmartLib
{
	using namespace ::std;
	class Solution_frog_jumps_test
	{
	public:
		static void Case1()
		{
			vector<int> stones{ 0,1,2,3,4,8,9,11 };
			bool ok = SmartLib::Solution_frog_jumps::canCross(stones);
			return;
		}

		static void Case0()
		{
			vector<int> stones{ 0,1,3,5,6,8,12,17 };
			bool ok = SmartLib::Solution_frog_jumps::canCross(stones);
			return;
		}
	};
}