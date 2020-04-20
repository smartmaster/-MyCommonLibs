#pragma once

#include <tuple>
#include <iostream>

namespace SmartLib
{
	using namespace  ::std;


	class BitwiseAddAlg
	{
	public:

		struct BitSum
		{
			long _carry{ 0 };
			long _sum{ 0 };

			BitSum(long carry, long sum) :
				_carry{ carry },
				_sum{ sum }
			{
			}
		};

		static BitSum BitAdd(long val1, long val2, long carry)
		{
			return BitSum
			{
				(val2 && carry) || (val1 && carry) || (val1 && val2),
				val1 ^ val2 ^ carry,
			};
		}

		static long Add(unsigned long num1, unsigned long num2)
		{
#if true
			long sum = 0;
			long carry = 0;
			long shif = 1;
			while (num1 || num2)
			{
				auto bitSum = BitAdd(num1 & 1L, num2 & 1L, carry);

				if (bitSum._sum)
				{
					sum |= shif;
				}
				

				shif <<= 1;
				num1 >>= 1;
				num2 >>= 1;
				carry = bitSum._carry;
			}

			return sum;
#else
			return ((num1 ^ num2) | ((num1 & num2) << 1));
#endif
		}
	};
}