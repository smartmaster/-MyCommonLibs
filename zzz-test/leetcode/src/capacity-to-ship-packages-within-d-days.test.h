#pragma once


#include <iostream>

#include "logger.h"
#include "capacity-to-ship-packages-within-d-days.h"


namespace SmartLib
{
	using namespace ::std;

	class ShipCapacityTest
	{
	public:
		static void Case0()
		{
			//vector<int> weights{ 1,2,3,4,5,6,7,8,9,10 };
			vector<int> weights{10, 50, 100, 100, 50, 100, 100, 100};
			long days = 5;

			long res = SmartLib::ShipCapacity::shipWithinDays(weights, days);

			SmartLib::Logger::Print(cout, "the result is ", res, ".") << endl;
		}
	};
}