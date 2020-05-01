#pragma once

#include <iostream>
#include "longest-well-performing-interval.h"


namespace SmartLib
{
	using namespace ::std;

	class Longest_Well_Performing_Interval_Test
	{
	public:
		static void Case0()
		{
			vector<int> data{ 9,9,6,0,6,6,9 };
			int rr = SmartLib::Longest_Well_Performing_Interval::longestWPI(data);
			cout << "The result is " << rr << endl;
		}
	};
}