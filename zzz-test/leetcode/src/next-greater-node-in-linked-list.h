#pragma once

#include <vector>

namespace SmartLib
{
	using namespace ::std;

	struct ListNode
	{
		int val;
		ListNode* next;
		ListNode(int x) : val(x), next(NULL) {}
	};

	class next_greater_node_in_linked_list
	{
	private:
		struct Pair
		{
			ListNode* _node;
			long _index;


			Pair(ListNode* node, long index) :
				_node{ node },
				_index{ index }
			{
			}
		};
	private:
		static vector<int> nextLargerNodes(ListNode* head)
		{
			if (head == nullptr)
			{
				return {};
			}

			vector<int> vec;
			vector<Pair> stack;

			stack.push_back(Pair{ head, 0 });
			vec.push_back(0);

			long index = 1;
			ListNode* node = head->next;
			while (node)
			{
				while (stack.size() && node->val > stack.back()._node->val)
				{
					vec[stack.back()._index] = node->val;
					stack.pop_back();
				}

				stack.push_back(Pair{ node, index++ });
				vec.push_back(0);

				node = node->next;
			}


			return vec;

		}

	};
}