#pragma once


//https://leetcode-cn.com/problems/best-sightseeing-pair/description/

#include <vector>

namespace SmartLib
{
	using namespace ::std;

	class best_sightseeing_pair
	{
	private:
		const vector<int>& _pos;

	private:

		best_sightseeing_pair(vector<int>& A) :
			_pos{ A }
		{

		}

		long Solution()
		{
			long val1 = _pos[0] + 0;
			long val2 = _pos[1] - 1;

			long result = val1 + val2;

			for (long ii = 2; ii < _pos.size(); ++ ii)
			{
				long tempVal1 = _pos[ii - 1] + ii - 1;
				if (val1 < tempVal1)
				{
					val1 = tempVal1;
				}

				val2 = _pos[ii] - ii;

				long tempResult = val1 + val2;
				if (result < tempResult)
				{
					result = tempResult;
				}
			}

			return result;
		}


	public:

		static int maxScoreSightseeingPair(vector<int>& A) 
		{
			best_sightseeing_pair bsp{A};
			return bsp.Solution();
		}

	};
}