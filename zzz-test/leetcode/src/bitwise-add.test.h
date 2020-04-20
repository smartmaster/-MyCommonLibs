#pragma once
#include "bitwise-add.h"

#include <iostream>

namespace SmartLib
{
	using namespace ::std;

	class BitwiseAddAlgTest
	{
	public:
		static void Case0()
		{
			long num1 = 23492475;
			long num2 = 54679746;

			//long num1 = 11;
			//long num2 = 20;

			long sum = BitwiseAddAlg::Add(num1, num2);
			cout << num1 + num2 << ' ' << sum << endl;

			num1 = -num1;
			sum = BitwiseAddAlg::Add(num1, num2);
			cout << num1 + num2 << ' ' << sum << endl;

			num2 = -num2;
			sum = BitwiseAddAlg::Add(num1, num2);
			cout << num1 + num2 << ' ' << sum << endl;

		}
	};
}