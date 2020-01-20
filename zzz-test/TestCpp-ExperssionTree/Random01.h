#pragma once


#include <ctime>
#include <cstdlib>

#include "Helper.h"
#include "log.h"

namespace SmartLib
{
	class Random01
	{
	public:
		Random01()
		{
			std::srand(std::time(nullptr));
		}

		int Ran()
		{
			return (std::rand() & 1);
		}
	};





	class Random01Test
	{
	public:
		static void Case0()
		{
			Random01 rr;

			long long loopCount = 17;
			long long count1 = 0;
			long long count0 = 0;

			for (long long ii = 0; ii < loopCount; ++ ii)
			{
				if (rr.Ran())
				{
					++count1;
				}
				else
				{
					++count0;
				}
			}


			auto div0 = Helper::IntDivString(count0, loopCount);
			auto div1 = Helper::IntDivString(count1, loopCount);
			LOG_LINE_A("loopCount=%d, count0=%d(%s), count1=%d(%s)", loopCount, count0, div0.c_str(), count1, div1.c_str());

		}
	};
}