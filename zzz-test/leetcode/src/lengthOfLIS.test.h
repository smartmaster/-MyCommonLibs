#pragma once

#include <vector>
#include <iostream>


#include "lengthOfLIS.h"
#include "sml-common.h"


namespace SmartLib
{
	using namespace ::std;

	class lengthOfLISAlgTest
	{
	public:
		static void Case0()
		{
			vector<int> vec{ 10,9,2,5,3,7,101,18 };
			int res = SmartLib::lengthOfLISAlg::Solution(vec);
			Common::Print(cout, "the longest sequence is of length ", res) << endl;
			
		}
	};
}