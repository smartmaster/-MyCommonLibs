#pragma once



/**********************

https://leetcode.com/problems/divide-two-integers/description/


Divide two integers without using multiplication, division and mod operator.

If it is overflow, return MAX_INT.



*************************/
#include <cstdint>
#include <vector>

#include "log.h"

namespace SmartLib
{
	class DivideTwoIntegers
	{
	private:
		static long long LowerAllOnes(int nn)
		{
			return ((1LL << nn) - 1);
		}


		static long long GetLower(long long val, int nn)
		{
			return (val & LowerAllOnes(nn));
		}

		static long long HigherAllOnes(int nn)
		{
			return ~LowerAllOnes(64 - nn);
		}

		static long long GetHigher(long long val, int nn)
		{
			return (val & HigherAllOnes(nn)) >> (64 - nn);
		}

		static long long SetHigerPart(long long val, long long hi, int nn)
		{
			return (hi << (64 - nn)) | GetLower(val, 64 - nn);
 		}

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


			long long rval = 0;
			
			if (dd == 0)
			{
				return INT_MAX;
			}
			else if(dd == 1)
			{
				rval = nn;
			}
			else if (nn < dd)
			{
				rval = 0;
			}
			else if (nn == dd)
			{
				rval = 1;
			}
			else
			{
				std::vector<int> result(64);

				for (int count = 1; count <= 64; ++count)
				{
					long long current = GetHigher(nn, count);
					if (current < dd)
					{
						result[count - 1] = 0;
					}
					else
					{
						result[count - 1] = 1;
						nn = SetHigerPart(nn, current - dd, count);
					}
				}


				for (int ii = 0; ii < result.size(); ++ii)
				{
					rval = (rval << 1) + result[ii];
				}
			}

			return (s1 == s2 ? rval : -rval);
		}


		int divide(int dividend, int divisor)
		{
			return (int)Div(dividend, divisor);
		}

	};



	class DivideTwoIntegersTest
	{
	public:
		static void Case0()
		{
			long long nn = -183789237564;
			long long dd = 8678456;
			long long div = nn / dd;
			LOG_LINE_A("%I64d / %I64d = %I64d", nn, dd, div);

			long long mydiv = DivideTwoIntegers::Div(nn, dd);
			LOG_LINE_A("%I64d / %I64d = %I64d", nn, dd, mydiv);
		}

	};
}