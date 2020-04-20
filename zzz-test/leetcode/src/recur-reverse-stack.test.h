#pragma once

#include <iostream>
#include <vector>


#include "sml-common.h"
#include "recur-reverse-stack.h"


namespace SmartLib
{
	using namespace ::std;

	class RecurReverseStackTest
	{
	public:
		static void Case0()
		{
			vector<long> stack;
			for (long ii = 1; ii < 5; ++ ii)
			{
				stack.push_back(ii);
			}

			Common::PrintIterator(cout, stack.begin(), stack.end()) << endl;

			RecurReverseStack{ stack }.Reverse();
			Common::PrintIterator(cout, stack.begin(), stack.end()) << endl;

		}
	};
}