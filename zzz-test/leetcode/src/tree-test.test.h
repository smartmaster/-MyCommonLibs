#pragma once
#include "tree-test.h"


namespace SmartLib
{
	using namespace ::std;

	class TreeTestTest
	{
	public:
		static void Case0()
		{
			const long NODE_COUNT = 19;
			vector < TreeTest::Node*> nodes;
			TreeTest::CreateTree(nodes, NODE_COUNT);

			cout << "BFS:" << endl;
			TreeTest::BFS(nodes[0]);

			cout << "ZigZag:" << endl;
			TreeTest::ZigZag(nodes[0]);
		}
	};
}