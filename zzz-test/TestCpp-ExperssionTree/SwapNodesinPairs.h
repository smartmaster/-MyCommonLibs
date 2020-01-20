#pragma once

/*******************

https://leetcode.com/problems/swap-nodes-in-pairs/description/

24. Swap Nodes in Pairs
DescriptionHintsSubmissionsDiscussSolution
Given a linked list, swap every two adjacent nodes and return its head.

Example:

Given 1->2->3->4, you should return the list as 2->1->4->3.
Note:

Your algorithm should use only constant extra space.
You may not modify the values in the list's nodes, only nodes itself may be changed.

***************/

#include <vector>
#include <map>
using namespace std;

#include "log.h"

namespace SmartLib
{
	class SwapNodesinPairs
	{
	public:
		struct ListNode
		{
			const int val;
			ListNode *next;
			ListNode(int x) : val(x), next(nullptr) {}
		};

	public:
		static ListNode * ProcessNode(ListNode * p1)
		{
			ListNode * p2 = p1 ? p1->next : nullptr;
			ListNode * p3 = p2 ? p2->next : nullptr;

			if (p2)
			{
				p2->next = p1;
			}
			if (p1)
			{
				p1->next = p3;
			}
			return p2 ? p2 : p1;
		}


		static ListNode * StepForward(ListNode * node, int steps)
		{
			for (int ii = 0; ii < steps; ++ ii)
			{
				if (node)
				{
					node = node->next;
				}
				else
				{
					break;
				}
			}


			return node;
		}


		static ListNode* swapPairs(ListNode* head)
		{
			ListNode * newHead = head == nullptr ? nullptr :
				head->next == nullptr ? head :
				head->next;

			auto first = ProcessNode(head);
			while (first)
			{
				auto second = first ? first->next : nullptr;
				auto newFirst = second ? second->next : nullptr;
				newFirst = ProcessNode(newFirst);
				if (second)
				{
					second->next = newFirst;
				}
				first = newFirst;
			}

			return newHead;
		}


		static pair<ListNode*, ListNode*> Process(pair<ListNode*, ListNode*> & cur)
		{
			pair<ListNode*, ListNode*> next{ cur.second, cur.second ? cur.second->next : nullptr };

			if (cur.second)
			{
				cur.second->next = cur.first;
			}
			return next;
		}

		static ListNode* Reverse(ListNode * node)
		{
			pair<ListNode*, ListNode*> cur{node, node? node->next : nullptr};

			while (cur.second)
			{
				cur = Process(cur);
			}

			if (node)
			{
				node->next = nullptr;
			}

			return cur.first;
		}


		static ListNode * RemoveNthReserve(ListNode * head, int n)
		{
			if (head == nullptr)
			{
				return nullptr;
			}

			int count = 0;
			auto end = head;
			for (; count < n; ++ count)
			{
				if (end)
				{
					end = end->next;
				}
				else
				{
					break;
				}
			}

			if (end == nullptr && count == n)
			{
				auto temp = head->next;
				delete head;
				return temp;
			}
			else if (end != nullptr)
			{
				auto first = head;
				while (end->next)
				{
					end = end->next;
					first = first->next;
				}

				auto temp = first->next;
				first->next = temp->next;
				delete temp;
			}

			return head;
		}


		
	};


	class SwapNodesinPairsTest
	{
	public:

		static	void PrintList(SwapNodesinPairs::ListNode * node)
		{
			while (node)
			{
				LOG_A("%d, ", node->val);
				node = node->next;
			}
			LOG_LINE_A("");
		}

		static SwapNodesinPairs::ListNode * CreateList(const vector<int> & vec)
		{
			SwapNodesinPairs::ListNode * head = nullptr;
			SwapNodesinPairs::ListNode * cur = nullptr;
			for (int ii = 0; ii < vec.size(); ++ ii)
			{
				if (cur == nullptr)
				{
					cur = new SwapNodesinPairs::ListNode(vec[ii]);
					head = cur;
				}
				else
				{
					cur->next = new SwapNodesinPairs::ListNode(vec[ii]);
					cur = cur->next;
				}
			}

			return head;
		}

		static void Case0()
		{
			vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			auto node = CreateList(vec);
			PrintList(node);
			node = SwapNodesinPairs::swapPairs(node);
			PrintList(node);
		}


		static void Case1()
		{
			vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			auto node = CreateList(vec);
			PrintList(node);
			node = SwapNodesinPairs::Reverse(node);
			PrintList(node);
		}


		static void Case2()
		{
			vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			auto node = CreateList(vec);
			PrintList(node);
			node = SwapNodesinPairs::RemoveNthReserve(node, 3);
			PrintList(node);
		}
	};
}