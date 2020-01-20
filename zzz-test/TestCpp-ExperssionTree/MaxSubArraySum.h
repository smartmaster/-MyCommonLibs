#pragma once

#include <cassert>

namespace SmartLib
{
	class MaxSubArraySum
	{
	public:
		static int MaxSubArraySumProc(const int * arr, int nn)
		{
			assert(nn > 0);

			int max = arr[0];
			int current = arr[0];
			for (int ii = 1; ii < nn; ++ ii)
			{
				if (current > 0)
				{
					current += arr[ii];
				}
				else
				{
					current = arr[ii];
				}

				if (max < current)
				{
					max = current;
				}
			}

			return max;
		}
	};
}