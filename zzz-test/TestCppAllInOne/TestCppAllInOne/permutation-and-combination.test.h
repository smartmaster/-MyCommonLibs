#pragma once
#include "permutation-and-combination.h"


namespace SmartLib
{
	class permutation_and_combinationTest
	{
	public:

		static void Case2()
		{
			Combination{ 5, 2, nullptr }.CombineStart(0);
		}

		static void Case1()
		{
			Permutation{ 5, 3, nullptr }.Permute();
		}


		static void Case0()
		{
			Combination{ 5, 2, nullptr }.Combine();
		} 
	};
}