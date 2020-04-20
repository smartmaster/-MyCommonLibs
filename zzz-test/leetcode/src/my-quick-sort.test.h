#pragma once


#include <vector>
#include <iostream>

#include "sml-common.h"
#include "my-quick-sort.h"


namespace SmartLib
{
	using namespace ::std;

	class QuickSortAlgTest
	{
	public:
		static void Case0()
		{
			vector<long> data(19);
			Common::FillRandom(data.begin(), data.end(), 10, 100);
			Common::PrintIterator(cout, data.begin(), data.end()) << endl;

			QuickSortAlg{ data }.Sort();
			Common::PrintIterator(cout, data.begin(), data.end()) << endl;
			if (Common::VerifyOrder(data.begin(), data.end()))
			{
				Common::Print(cout, "sort is okay.") << endl;
			}
			else
			{
				Common::Print(cout, "something wrong with sort, pleae fix it!") << endl;
			}

		}

	};
}