#pragma once

#include "sml-common.h"
#include "robot-walk.h"


namespace SmartLib
{
	using namespace ::std;

	class WalkAlgTest
	{
	public:
		static void Case1()
		{
			vector<long> itemSizes{ 5,6,5,1,19,7 };
			vector<long> itemValues{ 2,3,1,4,6,5 };
			long packSize = 10;

			long r1 = KnapPack{ itemSizes, itemValues, packSize }.Max();
			long r2 = KnapPack{ itemSizes, itemValues, packSize }.MaxDP();

			Common::Print(cout, r1, " ", r2) << endl;

		}
		static void Case0()
		{
			long totolLen = 7;
			long destination = 5;
			long curPos = 4;
			long steps = 9;
			long r1 = WalkAlg{ totolLen, destination, curPos, steps }.Walk();
			long r2 = WalkAlg{ totolLen, destination, curPos, steps }.WalkDP();

			Common::Print(cout, r1, " ", r2) << endl;
		}
	};
}
