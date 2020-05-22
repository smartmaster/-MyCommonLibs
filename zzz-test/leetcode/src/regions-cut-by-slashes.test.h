#pragma once
#include "regions-cut-by-slashes.h"
#include <iostream>

namespace SmartLib
{
	using namespace ::std;
	class regions_cut_by_slashes_test
	{
	public:
		static void Case0()
		{
			vector<string> grid{ " /","/ " };
			long res = SmartLib::regions_cut_by_slashes::regionsBySlashes(grid);
			cout << "the result is " << res << endl;
		}
	};
}