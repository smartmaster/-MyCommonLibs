#pragma once
#include "sort-stack-using-one-stack.h"


namespace SmartLib
{
	using namespace ::std;

	class SortStackUsingStackAlgTest
	{
	public:
		static void Case0()
		{
			stack<long> stack;
			for (long ii = 1; ii <= 6; ++ii)
			{
				stack.push(ii);
			}

			SortStackUsingStackAlg{ stack }.Sort();


		}

	};
}