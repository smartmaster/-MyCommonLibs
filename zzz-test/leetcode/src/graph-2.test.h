#pragma once
#include "graph-2.h"


namespace SmartLib
{
	class Graph2Test
	{
	public:
		static void Case0()
		{
			FastPriorityQueue2 fpq2;
			fpq2.Insert(100, 20);
			fpq2.Insert(200, 50);
			fpq2.Insert(10, 300);

			auto [node, distance] = fpq2.GetMin();
		}
	};
}