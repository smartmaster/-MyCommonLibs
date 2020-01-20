#pragma once

#include <vector>
using namespace std;

#include "log.h"

namespace SmartLib
{
	class  SwapNodesinPairs2
	{
	public:
		struct ListNode
		{
			const int val;
			ListNode *next;
			ListNode(int x) : val(x), next(nullptr) {}
		};

		static void Process(ListNode * & p1, ListNode * & p2)
		{
			if (p1 && p2)
			{
				p1->next = p2->next;
			}
			
			if (p2)
			{
				p2->next = p1;
			}
			
			if (p2)
			{
				auto temp = p1;
				p1 = p2;
				p2 = temp;
			}
		}


		static ListNode* SwapPairs(ListNode * node)
		{

			auto newHead = node == nullptr ? nullptr :
				node->next == nullptr ? node :
				node->next;

			auto p1 = node;
			auto p2 = p1 ? p1->next : nullptr;
			Process(p1, p2);
			while (true)
			{
				if (p2 == nullptr)
				{
					break;
				}
				auto newp1 = p2->next;
				auto newp2 = newp1 ? newp1->next : nullptr;
				Process(newp1, newp2);
				p2->next = newp1;
				p1 = newp1;
				p2 = newp2;
			}


			return newHead;
		}
	};


	class SwapNodesinPairs2Test
	{
	public:
		static	void PrintList(SwapNodesinPairs2::ListNode * node)
		{
			while (node)
			{
				LOG_A("%d, ", node->val);
				node = node->next;
			}
			LOG_LINE_A("");
		}

		static SwapNodesinPairs2::ListNode * CreateList(const vector<int> & vec)
		{
			SwapNodesinPairs2::ListNode * head = nullptr;
			SwapNodesinPairs2::ListNode * cur = nullptr;
			for (int ii = 0; ii < vec.size(); ++ii)
			{
				if (cur == nullptr)
				{
					cur = new SwapNodesinPairs2::ListNode(vec[ii]);
					head = cur;
				}
				else
				{
					cur->next = new SwapNodesinPairs2::ListNode(vec[ii]);
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
			node = SwapNodesinPairs2::SwapPairs(node);
			PrintList(node);
		}


		static void Case1()
		{
			vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			auto node = CreateList(vec);
			PrintList(node);
			node = SwapNodesinPairs2::SwapPairs(node);
			PrintList(node);
		}


		static void Case2()
		{
			vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			auto node = CreateList(vec);
			PrintList(node);
			node = SwapNodesinPairs2::SwapPairs(node);
			PrintList(node);
		}

	};
}