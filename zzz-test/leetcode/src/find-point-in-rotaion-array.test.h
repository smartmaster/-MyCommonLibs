#pragma once

#include <vector>
#include <iostream>

#include <cassert>

#include "sml-common.h"
#include "find-point-in-rotaion-array.h"


namespace SmartLib
{
	using namespace  ::std;

	class FindUpperPointAlgTest
	{
	public:
		static void Case0()
		{
			vector<long> array(100);
			long points[] = {0, 4, 5, 6, 7, 9, 99};

			for (auto point : points)
			{
				for (long ii = 0; ii < array.size(); ++ii)
				{
					array[ii] = (ii + point) % array.size();
				}

				long result = FindUpperPointAlg{ &array[0], (long)array.size() }.FindUpperPoint(0, array.size() - 1);
				assert(result == array.size() - 1 - point);

				Common::Print(cout, "the result is ", result, ".") << endl;
			}
			
 		}
	};
}