#pragma once

#include <iostream>

#include "mono-stack.h"
#include "sml-common.h"

namespace SmartLib
{
	using namespace ::std;

	class MonoStackAlgTest
	{
	public:
		static void Case0()
		{
			vector<long> data = {3, 4, 1, 5, 6, 2, 7, };

			auto result = MonoStackAlg{ data }.Solution();

			Common::PrintIterator(cout, result.begin(), result.end()) << endl;
		}
	};
}