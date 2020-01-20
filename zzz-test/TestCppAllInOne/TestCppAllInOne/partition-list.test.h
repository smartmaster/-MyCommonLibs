#pragma once

#include <vector>
#include <iostream>

#include "partition-list.h"


namespace  SmartLib_partition_list
{
	using namespace ::std;

	class partition_list_test
	{
	public:
		static ListNode* MakeList(const vector<int>& data)
		{
			ListNode start{ 0 };
			auto cur = &start;

			for (auto dd : data)
			{
				cur->next = new ListNode{ dd };
				cur = cur->next;
			}

			return start.next;
		}

		static void Print(ListNode* nn)
		{
			while (nn)
			{
				cout << nn->val << ",";
				nn = nn->next;
			}
			cout << endl;
		}


		static void Case0()
		{
			vector<int> vec{ 1, 4, 3, 2, 5, 2 };
			int data = 3;

			ListNode* head = MakeList(vec);
			Print(head);


			head = SmartLib_partition_list::Solution::partition(head, data);
			Print(head);
		}
	};
}