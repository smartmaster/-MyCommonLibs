#pragma once
#include "strings-to-max-number.h"
#include <iostream>

namespace SmartLib
{
	using namespace ::std;

	class numbers_to_max_number_test
	{
	public:
		static void Case0()
		{
			vector<long> nums{123, 789, 456, 7, 9};
			string maxNum = SmartLib::numbers_to_max_number::NumbersToMax(nums);
			cout << "(1) The maximum number is " << maxNum << endl;


			string maxNumQ = SmartLib::numbers_to_max_number::NumbersToMaxQ(nums);
			cout << "(2) The maximum number is " << maxNumQ << endl;
		}
	};
}