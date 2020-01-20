#pragma once

#include <time.h>
#include <stdlib.h>

namespace TestCpp
{
	class RandFill
	{
	public:
		template<typename ITER, typename VALUE_TYPE>
		static void Fill(ITER begin, ITER end, VALUE_TYPE min, VALUE_TYPE max)
		{
			srand((unsigned)time(nullptr));
			while (begin != end)
			{
				*begin = rand() % (max - min) + min;
				++begin;
			}
		}
	};
}