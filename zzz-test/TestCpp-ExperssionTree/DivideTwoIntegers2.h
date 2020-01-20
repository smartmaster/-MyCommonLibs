#pragma once

#include <climits>

#include "log.h"

namespace SmartLib
{

#define GetBitAt(val, index) (((val) & (1LL << index))? 1 : 0)

	class DivideTwoIntegers2
	{
	public:
		static long long Div(long long nn, long long dd)
		{
			int s1 = 0;
			if (nn < 0)
			{
				s1 = 1;
				nn = -nn;
			}

			int s2 = 0;
			if (dd < 0)
			{
				s2 = 1;
				dd = -dd;
			}

			long long result = 0;
			if (dd == 0)
			{
				result = INT_MAX;
			}
			else if (dd == 1)
			{
				result = nn;
			}
			else if (nn < dd)
			{
				result = 0;
			}
			else if (nn == dd)
			{
				result = 1;
			}
			else
			{
				long long remainder = 0;
				result = 0;
				for (int ii = 63; ii >= 0; -- ii)
				{
					result <<= 1;
					remainder <<= 1;
					remainder += GetBitAt(nn, ii);
					if (remainder >= dd)
					{
						++result;
						remainder -= dd;
					}
				}
			}

			result = (s1 == s2) ? result : -result;
			if (result > INT_MAX)
			{
				result = INT_MAX;
			}

			return result;
		}
	};


	class DivideTwoIntegers2Test
	{
	public:
		static void Case0()
		{
			long long nn = -183789237564;
			long long dd = 8678456;
			long long div = nn / dd;
			LOG_LINE_A("%I64d / %I64d = %I64d", nn, dd, div);

			long long mydiv = DivideTwoIntegers2::Div(nn, dd);
			LOG_LINE_A("%I64d / %I64d = %I64d", nn, dd, mydiv);
		}

	};
}