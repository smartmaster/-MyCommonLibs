#pragma once

#include <iostream>

#include "sml-common.h"
#include "my-list.h"



namespace SmartLib
{
	using namespace ::std;

	class MySListTest
	{
	public:

		static void Case0()
		{
			vector<MySList::Node*> nodes;
			MySList::Node* head = MySList::CreateList(19, &nodes);
			MySList::Print(head);


			head = MySList::ReverseListK(head, 2);
			MySList::Print(head);


			nodes.back()->_next = nodes[7]; //make a circle

			MySList::Node* joint = MySList::FirstJoint(head);
			if (joint)
			{
				Common::Print(cout, "the first joint is ", joint->_data, ".") << endl;
			}


			nodes.back()->_next = nullptr;
			MySList::Destroy(head);
			head = nullptr;
		}
	};
}