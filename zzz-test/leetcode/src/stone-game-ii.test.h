#pragma once
#include "stone-game-ii.h"


namespace SmartLib
{
	using namespace ::std;

	class StoneGameIITest
	{
	public:
		static void Case0()
		{
			vector<int> vec{ 8,9,5,4,5,4,1,1,9,3,1,10,5,9,6,2,7,6,6,9 };
			long rr = SmartLib::StoneGameII::stoneGameII(vec);
			return;
		}
	};
}