#pragma once

//https://leetcode.com/articles/number_of-digit-one/

namespace SmartLib
{
	class number_of_digit_one
	{
	public:
		static long countDigitOne(long number)
		{
			++number; //start from zero

			long div = 1;
			long total = 0;

			for (;;)
			{
				long ten_div = div * 10;
				long qq = number / ten_div * div;
				long rr = number % ten_div;

				total += qq;
				if (rr <= div)
				{
					//do nothing
				}
				else if (rr <= (div * 2))
				{
					total += (rr - div);
				}
				else
				{
					total += div;
				}

				if (qq == 0)
				{
					break;
				}
				div = ten_div;
			}


			return total;
		}
	};
}