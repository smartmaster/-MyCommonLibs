#pragma once

#include <iostream>

#include "sml-common.h"
#include "graph-lib.h"




namespace SmartLib
{
	class MinCostDirectionGridAlg3Test
	{
	public:
		static void Case2()
		{
			vector<vector<int>> direction
			{
			{4,1,4,1,1,4,4,3,3,4,4,4,1,2,2,4,4,3,3,1,1,1,4,3,2,4,2,1,1,1,4,3,3,3,4,2,1,1,3,3,4,2,3,4,1,1},
			{2,4,4,1,3,1,1,3,4,2,3,1,1,4,2,3,2,2,2,1,1,3,2,4,1,3,1,2,3,4,4,1,1,2,3,3,2,4,2,4,4,4,4,2,2,3},
			{4,2,4,3,4,1,2,4,2,1,4,2,3,3,3,2,4,1,1,4,2,4,2,2,2,3,4,3,2,4,2,1,1,2,3,3,3,1,4,1,3,2,2,2,2,2},
			{3,4,3,4,4,2,4,2,1,2,3,2,1,1,4,1,3,3,2,3,2,4,1,4,2,4,3,2,2,3,1,3,4,3,4,4,3,1,1,1,4,2,3,3,3,1},
			{4,3,4,2,4,2,1,3,3,3,2,2,2,1,1,2,4,1,1,3,1,4,1,4,1,1,2,1,2,4,1,4,1,2,4,4,3,1,1,1,1,4,3,1,2,4},
			{2,4,4,1,1,4,2,3,1,3,4,3,4,1,4,1,3,1,2,1,4,2,3,3,4,1,1,4,4,4,3,1,3,3,2,3,1,3,1,1,2,2,2,3,4,3},
			{2,4,1,4,1,2,2,4,2,4,4,2,1,2,4,1,1,2,3,4,1,4,4,4,4,2,1,2,1,4,1,4,1,4,1,1,3,4,3,4,3,2,1,2,2,1},
			{3,4,2,2,3,4,1,4,1,4,1,1,2,4,3,2,1,3,1,2,4,4,2,3,3,2,3,4,1,3,4,1,1,1,2,2,4,1,1,3,4,3,2,4,2,1},
			{3,2,2,2,2,1,2,2,1,4,3,2,2,4,1,4,3,4,3,4,1,1,4,3,3,3,1,2,4,1,2,1,2,4,1,1,2,3,4,2,1,3,2,2,4,4},
			{1,3,1,1,1,1,1,2,2,3,4,3,3,2,1,4,4,4,3,2,3,3,3,4,2,1,3,1,2,1,1,3,1,1,1,4,1,2,1,3,4,2,1,3,4,1},
			{3,1,2,1,4,4,3,1,4,2,3,2,3,2,4,3,3,4,4,3,3,3,1,4,1,4,4,4,2,2,1,1,4,1,2,2,4,4,4,3,1,4,3,3,3,4},
			{4,3,2,3,4,3,1,1,3,4,1,1,2,1,4,2,3,3,4,4,4,1,4,4,4,3,2,2,4,2,1,4,1,3,2,3,4,3,3,4,2,2,1,1,4,3},
			{3,2,2,3,4,3,3,1,4,3,3,4,4,2,1,3,2,2,2,3,3,2,2,4,1,2,1,3,2,1,4,1,3,4,4,2,1,3,1,3,3,3,1,2,1,4},
			{4,3,3,1,1,3,4,2,1,2,1,2,4,3,1,4,2,2,3,4,2,2,1,1,2,2,1,1,3,4,1,1,2,2,1,2,3,4,3,4,2,1,1,2,1,3},
			{2,2,1,1,3,4,3,4,3,4,3,3,2,4,2,2,1,1,2,3,2,3,3,3,3,4,4,2,2,4,1,3,1,4,4,1,2,1,2,2,3,4,2,3,3,3},
			{1,3,1,2,2,4,1,1,3,4,1,3,2,1,3,4,2,1,3,2,2,1,1,1,2,2,2,4,4,4,1,4,1,4,2,1,1,3,3,4,1,2,4,4,3,2},
			{2,4,2,1,2,2,2,2,3,2,4,4,4,1,3,4,1,3,4,3,4,3,2,4,2,2,3,1,3,1,4,4,1,4,4,3,2,3,1,2,4,4,1,4,2,4},
			{3,1,4,3,2,1,2,1,1,2,4,1,4,3,1,3,1,2,1,4,2,1,3,2,1,2,4,4,4,3,2,3,3,3,4,1,1,1,4,2,4,3,2,1,3,2},
			{3,2,1,3,1,1,4,3,2,2,3,3,1,1,2,4,1,3,4,3,4,2,3,4,4,2,3,3,2,1,4,4,4,1,2,1,3,1,2,2,3,1,2,4,4,3},
			{3,1,2,4,2,3,3,2,4,4,3,4,1,3,4,2,1,3,4,3,4,2,4,3,1,3,2,3,2,2,4,4,4,1,2,4,1,2,3,4,2,2,2,4,1,4},
			{2,4,2,2,1,2,1,2,4,4,1,1,2,4,3,2,4,1,2,1,4,2,1,2,2,1,1,4,4,2,3,3,3,2,1,2,2,1,2,3,2,1,4,1,4,1},
			{4,3,4,3,4,3,3,4,4,4,1,2,3,4,3,1,2,2,4,4,3,1,2,2,4,4,3,4,4,4,2,4,1,2,1,3,3,1,1,2,3,3,4,1,3,3},
			{3,3,3,2,3,3,3,4,3,2,4,2,2,2,1,2,2,1,2,4,1,2,4,4,4,1,3,1,2,4,2,2,3,1,3,1,4,4,3,2,3,4,3,2,1,4},
			{1,3,1,2,4,4,3,4,2,2,4,3,2,4,3,1,1,3,3,3,3,1,3,4,4,4,1,2,2,4,1,2,2,4,1,3,2,2,2,3,4,2,1,2,2,2},
			{3,2,2,1,1,4,4,4,2,2,1,2,1,1,1,4,1,3,3,3,2,4,3,3,1,2,1,2,4,2,3,2,2,2,3,2,4,4,1,3,2,3,1,1,1,4},
			{2,3,3,1,4,2,4,2,3,1,4,4,3,1,2,3,2,1,3,4,2,1,1,1,3,3,2,2,3,4,4,2,4,3,4,3,2,4,3,4,3,1,3,3,4,4},
			{1,2,2,3,4,1,1,4,4,4,2,1,1,1,3,4,3,1,2,3,4,2,3,4,1,1,4,3,4,1,2,1,2,1,2,3,1,3,2,1,1,1,3,3,2,2},
			{3,3,2,1,2,1,4,4,3,1,4,2,2,2,4,3,3,4,1,4,3,2,1,1,2,4,4,1,2,2,4,3,1,2,3,2,2,3,4,4,1,2,3,3,2,1},
			{1,3,2,2,4,4,3,1,1,1,4,3,1,1,2,4,2,2,1,4,2,1,3,1,3,2,4,3,3,3,1,4,2,1,3,3,2,1,3,3,4,3,3,1,1,1},
			{1,1,3,4,2,3,3,1,1,4,2,4,1,4,2,4,1,2,3,2,3,1,3,2,4,1,4,4,3,4,4,3,2,4,2,3,1,4,1,1,1,1,1,3,1,3},
			{3,2,2,3,4,4,2,2,4,1,4,2,3,3,3,2,4,4,2,2,2,3,1,4,2,3,2,1,2,3,4,4,1,3,2,3,2,2,2,4,3,3,1,3,3,1},
			{1,4,1,4,1,4,3,3,2,1,1,1,2,2,2,3,2,3,2,1,3,2,2,1,3,1,1,3,4,2,4,2,1,3,3,2,3,4,3,3,4,3,2,1,2,1},
			{2,2,3,4,2,3,1,4,3,2,3,4,3,1,1,1,4,1,2,1,1,4,4,2,3,3,1,4,4,3,2,3,1,1,2,1,3,2,1,2,3,4,2,3,2,2},
			{2,3,1,2,2,3,2,2,2,2,1,3,1,3,2,1,2,1,4,4,2,4,3,2,1,1,2,1,4,1,2,4,4,4,4,1,4,2,4,2,4,3,3,1,4,2},
			{1,3,2,2,1,1,2,2,4,3,4,3,3,3,3,3,3,1,3,3,3,3,1,1,3,3,3,3,1,3,3,3,2,1,4,3,4,1,2,1,1,4,1,4,1,4},
			{2,1,3,4,4,3,1,4,4,3,4,2,2,3,1,2,1,1,3,3,2,1,4,1,1,3,3,2,2,2,3,3,2,3,4,3,4,1,3,3,2,4,4,2,2,4},
			{4,2,4,3,3,1,2,4,2,4,1,3,4,3,1,3,4,3,2,2,4,3,3,3,4,2,3,2,1,4,3,3,2,1,4,3,3,3,2,3,2,4,1,1,2,2},
			{3,2,4,3,3,1,4,2,4,4,2,2,4,2,2,4,1,4,4,4,1,4,2,3,1,3,3,4,2,2,2,2,2,1,3,1,3,2,1,2,1,2,3,4,3,1},
			{1,1,2,2,4,1,1,3,1,3,4,2,4,3,2,4,4,1,4,3,4,1,4,3,2,4,4,2,3,1,2,4,4,4,1,3,1,4,2,1,1,1,4,2,3,1},
			{4,2,4,2,1,3,3,4,2,2,4,2,1,1,4,4,3,2,4,2,4,1,3,2,2,1,3,4,3,4,3,4,3,1,3,1,3,4,4,3,1,2,3,2,1,1},
			{2,4,1,1,4,2,1,2,1,4,3,1,2,4,1,1,3,4,2,3,4,2,3,2,3,3,2,4,2,3,2,4,1,2,3,3,4,1,4,3,1,3,3,3,3,3},
			{2,2,2,4,3,1,1,2,1,4,1,2,1,2,2,3,1,2,2,2,1,4,2,2,2,2,4,3,3,2,2,1,2,3,4,1,1,3,2,4,2,1,2,1,3,3},
			{2,3,2,2,4,1,2,4,3,4,1,1,3,2,3,2,3,2,4,2,2,3,3,3,4,2,1,2,3,1,2,4,3,3,2,2,4,2,1,3,3,2,3,1,3,4},
			{4,3,4,3,1,1,4,2,3,1,3,2,3,1,4,3,4,1,3,3,3,3,4,1,4,2,4,4,4,2,4,3,2,4,4,1,3,4,3,4,2,2,4,4,4,1},
			{1,3,4,3,1,1,3,2,1,1,1,4,2,2,1,2,4,1,3,3,2,1,1,4,4,3,3,1,4,4,1,2,4,3,4,3,2,4,2,1,4,3,3,4,2,2},
			{4,1,3,2,2,1,4,3,1,4,4,1,2,3,4,3,2,4,2,3,1,3,3,3,2,1,1,2,2,1,4,3,4,4,4,2,2,2,1,4,3,2,3,3,4,3},
			{4,2,4,3,1,4,3,2,3,2,1,2,1,1,2,3,4,1,4,1,2,4,4,2,1,3,3,2,1,1,2,1,1,3,2,4,3,3,3,3,1,1,4,1,3,4},
			{3,1,2,2,4,3,4,2,4,3,4,1,3,3,2,3,1,3,1,2,1,3,2,3,1,4,4,4,1,4,2,3,1,3,4,4,4,3,1,2,3,2,4,2,4,1},
			{4,2,2,1,2,3,1,2,3,1,2,3,2,1,1,2,4,1,3,1,1,4,1,4,3,1,2,4,3,3,4,2,1,1,1,3,3,4,3,1,3,4,2,1,2,2},
			{2,4,2,4,1,3,2,4,3,3,1,3,2,4,3,2,4,2,4,1,4,1,1,3,2,1,4,3,3,2,3,4,1,1,4,4,2,2,3,2,2,3,2,2,4,4},
			{2,1,3,2,1,3,2,4,2,4,4,1,4,4,2,1,1,4,1,2,4,4,2,2,3,3,3,1,3,1,4,3,3,2,4,2,4,2,2,2,1,3,4,2,2,2},
			{4,4,1,3,1,3,1,3,2,2,4,4,4,2,3,1,3,4,3,2,3,4,2,1,2,1,3,3,3,3,3,3,3,1,4,1,3,1,3,2,1,4,2,1,1,3},
			{1,4,2,3,3,2,4,3,2,3,2,1,1,2,3,2,2,2,1,4,3,3,3,2,3,1,1,3,2,3,4,1,3,3,3,1,4,3,1,1,3,2,3,4,2,4},
			{3,4,3,1,3,3,1,4,1,1,4,3,4,2,4,3,3,1,3,2,2,3,4,1,4,4,3,3,1,3,4,3,3,3,3,3,3,4,2,2,4,4,3,2,2,2},
			{4,4,4,2,1,2,4,1,4,1,2,4,4,1,3,1,4,1,4,1,4,1,2,4,2,4,3,3,3,3,2,2,2,4,3,3,1,2,4,3,3,3,2,3,1,4},
			{2,1,4,4,2,2,3,4,1,2,3,2,3,2,1,2,1,4,2,2,4,1,1,1,2,4,3,2,4,1,2,4,2,1,1,1,4,4,2,2,1,3,3,2,1,2},
			{2,4,2,1,2,1,2,1,1,4,4,1,2,4,1,1,1,1,3,4,2,3,4,1,4,1,2,4,2,4,4,1,4,2,4,2,2,2,4,3,4,2,1,1,3,3},
			{4,4,3,4,1,3,3,1,1,3,3,2,2,3,3,2,1,3,4,1,2,3,1,4,2,4,2,2,4,1,1,3,3,4,2,2,2,3,4,1,4,3,3,3,1,4},
			{1,2,3,4,3,3,3,3,1,3,1,4,1,4,4,4,2,4,3,2,3,3,2,2,3,2,2,1,4,2,3,4,1,3,1,3,1,2,2,1,3,2,2,4,3,1},
			{1,4,4,1,4,4,3,1,2,1,4,1,1,2,1,2,2,3,1,2,4,2,4,2,1,3,1,3,3,4,3,3,2,1,1,1,3,3,3,2,1,3,1,3,3,1},
			{4,3,2,1,1,4,1,3,1,4,1,2,2,2,1,2,3,3,1,4,4,3,2,1,3,2,1,4,3,2,2,3,2,1,3,4,1,1,4,3,3,3,1,1,4,2},
			{4,4,4,2,4,2,2,4,1,4,3,2,3,2,4,4,4,3,3,4,1,3,2,4,1,2,2,4,3,1,1,3,2,1,1,4,4,2,1,2,4,3,3,4,4,2},
			{3,2,3,2,4,3,1,4,1,3,3,4,4,1,2,3,3,1,1,2,4,3,2,2,1,2,1,1,2,2,3,2,1,4,2,2,4,4,2,4,4,2,4,4,2,4},
			{4,1,3,4,3,4,4,3,4,4,4,2,1,4,1,1,1,1,4,3,1,4,1,4,3,2,2,1,1,2,4,3,4,1,2,2,2,4,4,1,1,4,2,1,2,4},
			{2,2,4,3,4,3,1,2,4,1,4,2,3,2,2,4,4,3,3,2,1,4,3,3,4,2,4,4,3,3,4,1,1,1,1,1,4,1,3,2,2,1,1,2,2,4},
			{3,3,4,1,3,2,1,4,2,1,4,2,1,3,4,4,3,4,1,1,4,1,3,3,2,2,4,4,3,1,3,2,1,2,4,1,1,1,1,2,2,3,2,4,4,1},
			{4,1,1,4,3,1,1,2,1,3,3,3,4,4,4,3,3,3,1,2,4,4,4,2,2,3,3,1,2,2,3,1,4,2,3,3,4,1,2,4,2,3,4,2,3,2},
			{2,3,4,3,1,3,3,3,1,1,1,2,2,4,2,3,4,3,2,1,2,1,3,4,3,4,1,1,4,3,3,2,3,2,1,1,1,2,2,4,4,3,3,1,1,1},
			{4,3,4,2,4,4,3,1,3,4,4,3,4,3,2,4,4,2,1,4,2,1,1,4,1,4,3,2,4,4,2,4,1,4,1,2,1,3,4,4,2,3,2,4,2,2},
			{3,2,4,4,4,3,2,1,2,1,2,2,1,2,1,2,4,3,1,1,2,1,1,1,3,3,4,1,1,2,1,3,3,2,1,3,3,4,2,1,3,4,4,1,3,1},
			{2,1,4,1,2,2,1,4,4,1,2,4,2,2,4,1,1,2,2,4,3,2,3,3,4,3,3,4,2,4,2,2,1,2,4,2,2,2,3,4,3,4,3,3,3,2},
			{3,4,2,2,2,1,1,2,3,1,2,4,2,2,2,1,3,1,2,1,1,2,2,2,4,4,1,2,3,1,3,2,3,2,4,1,1,1,2,2,2,1,2,3,1,2},
			{4,1,4,3,2,3,2,4,4,2,4,4,4,1,2,4,1,2,2,4,4,1,4,2,3,3,3,4,2,3,1,2,1,4,3,4,3,4,3,2,1,4,2,3,3,1},
			{4,2,3,1,4,4,2,2,1,4,2,3,3,1,4,3,2,2,1,4,1,4,3,3,2,1,3,2,2,1,2,1,1,3,3,4,1,4,2,3,1,3,4,2,2,3},
			{4,4,1,3,2,1,4,2,3,3,2,2,3,4,1,4,2,4,4,1,4,1,3,3,3,1,1,2,3,1,3,4,3,2,4,1,1,2,3,3,2,2,3,2,2,4},
			{4,1,3,3,3,4,1,4,3,2,3,4,1,2,4,1,4,2,4,4,4,1,1,3,4,4,3,1,3,1,1,3,4,3,3,3,4,4,1,4,1,2,3,4,1,2},
			{2,2,1,1,3,2,3,2,3,4,4,2,4,3,2,1,2,1,3,2,3,4,1,4,3,3,4,3,2,4,3,4,1,2,4,2,2,3,1,3,3,1,3,1,3,4},
			{1,1,3,2,1,4,1,1,2,3,3,2,4,3,4,3,3,2,2,1,2,1,4,3,1,4,4,3,4,2,3,4,1,1,1,4,3,4,4,1,3,1,3,4,2,4},
			{2,3,3,4,4,1,1,2,1,4,2,4,3,3,1,2,4,3,2,1,2,1,2,4,1,4,2,4,2,3,1,3,4,4,3,1,3,4,2,3,1,1,1,1,2,4},
			{2,4,1,4,4,3,4,3,4,1,1,3,1,4,4,1,3,2,2,2,1,1,4,2,4,1,3,4,2,3,1,4,1,2,1,4,2,4,1,3,4,1,2,3,2,2},
			{3,3,4,4,1,1,3,3,2,4,1,1,1,3,1,1,4,3,2,1,1,3,3,2,2,2,2,3,1,1,4,1,1,4,1,4,4,3,3,3,2,4,4,4,4,3},
			{4,4,1,4,1,4,2,1,4,1,1,3,3,3,3,3,2,4,2,4,4,2,4,3,4,2,2,4,1,4,1,3,4,4,3,3,1,1,3,3,2,1,2,4,1,1},
			{3,2,1,3,2,4,1,4,4,4,1,4,2,1,4,2,3,4,4,1,1,4,4,4,4,2,4,2,3,4,2,4,4,1,4,3,1,1,2,1,1,2,2,1,2,1},
			{3,3,2,4,2,1,3,4,1,3,3,2,2,2,3,3,3,4,3,3,2,3,4,2,3,3,4,2,3,3,4,2,2,2,1,2,2,4,1,1,3,1,1,4,1,2},
			{4,1,2,1,2,1,4,1,3,4,1,4,4,2,3,4,1,1,1,4,2,3,4,2,2,1,3,3,4,3,2,2,1,3,2,4,4,1,2,3,1,1,2,4,1,1}
			};

			int result = SmartLib::MinCostDirectionGridAlg3::SolutionD(direction);
			long r1 = SmartLib::MinCostDirectionGridAlg3::SolutionF(direction);

			Common::Print(cout, "result is ", result, ", and the other result is ", r1) << endl;
		}

		static void Case1()
		{
			vector<vector<int>> direction
			{
				{4,3,3,4,4,3,3,3,2,1,1,2,3},
				{4,2,2,3,2,2,4,2,1,2,3,2,2},
				{1,2,4,3,1,4,4,3,1,1,3,2,3},
				{1,1,3,4,4,3,3,2,3,3,2,1,1},
				{4,3,2,2,4,2,1,4,4,1,1,3,4},
				{1,1,1,4,2,3,2,2,3,2,4,3,3},
				{4,4,1,1,4,3,3,4,2,3,3,1,2},
				{1,4,4,2,3,4,3,4,4,3,2,3,2},
				{4,1,1,1,1,1,4,3,4,4,1,3,2},
				{2,2,2,1,2,1,1,2,1,4,3,4,3},
				{1,4,1,2,4,2,2,1,1,1,1,1,2},
				{3,3,2,3,4,2,4,4,3,4,4,2,3},
				{2,4,3,2,2,4,1,2,1,4,2,3,2},
				{1,3,1,2,3,3,1,2,2,3,4,2,3},
				{2,3,1,1,2,3,1,2,1,4,4,1,3},
				{3,3,2,4,3,2,4,2,4,1,4,4,1},
				{1,4,3,3,3,2,4,3,1,3,4,3,2},
				{3,1,1,2,4,3,1,2,2,4,1,1,4},
				{2,1,4,2,4,4,4,2,4,2,2,1,2},
				{1,2,2,2,2,2,2,3,1,3,2,2,2},
				{3,4,3,1,3,1,1,1,2,1,4,3,4},
				{1,3,3,2,4,1,3,1,4,2,2,2,1},
				{1,1,3,2,3,3,4,1,3,1,2,1,3},
				{1,1,3,1,4,4,3,3,1,2,3,2,3},
				{1,2,2,2,4,3,1,4,1,2,2,2,4},
				{1,2,4,3,4,3,1,1,2,4,4,3,1},
				{2,1,1,3,1,4,3,4,3,2,2,1,3},
				{1,1,1,1,1,3,3,1,2,2,3,1,2},
				{2,3,3,1,4,3,3,1,4,2,3,2,4},
				{2,4,3,2,2,1,2,2,3,1,1,2,4},
				{1,3,2,4,2,4,2,2,3,4,2,1,4},
				{3,4,4,2,2,4,1,4,3,2,1,4,4},
				{4,3,1,4,3,4,2,1,3,3,1,2,2},
				{2,1,4,3,2,3,1,1,4,4,1,4,4},
				{3,1,2,2,2,4,2,3,3,2,4,1,1},
				{2,1,1,3,2,2,2,2,4,3,4,1,1},
				{2,2,3,2,2,1,3,3,4,2,1,4,1},
				{2,1,3,2,1,1,1,4,2,4,3,1,2},
				{1,2,1,3,1,3,1,4,2,2,1,3,3},
				{3,4,3,3,3,2,2,4,4,2,2,1,2},
				{2,3,3,2,3,4,1,3,4,4,1,4,2},
				{4,3,4,2,3,4,4,4,1,3,2,1,3},
				{4,1,3,4,4,2,2,2,3,4,3,4,2},
				{1,3,3,3,4,3,3,2,3,3,1,4,3},
				{3,3,3,2,4,2,2,2,3,2,4,3,4},
				{2,1,4,1,4,3,1,4,4,2,4,2,3},
				{2,2,3,1,2,4,3,3,1,2,2,3,3},
				{2,3,4,4,1,1,1,2,3,4,3,3,4},
				{4,1,4,3,3,4,4,1,1,3,2,3,4}
			};

			int result = SmartLib::MinCostDirectionGridAlg3::SolutionD(direction);
			long r1 = SmartLib::MinCostDirectionGridAlg3::SolutionF(direction);

			Common::Print(cout, "result is ", result, ", and the other result is ", r1) << endl;
		}

		static void Case0()
		{
			const int ROW = 2;
			const int COL = 3;
			vector<vector<int>> direction;
			direction.resize(ROW);
			for (auto& vec : direction)
			{
				vec.resize(COL, MinCostDirectionGridAlg3::LEFT);
			}
			int result = SmartLib::MinCostDirectionGridAlg3::SolutionD(direction);
			long r1 = SmartLib::MinCostDirectionGridAlg3::SolutionF(direction);

			Common::Print(cout, "result is ", result, ", and the other result is ", r1) << endl;
		}
	};
}