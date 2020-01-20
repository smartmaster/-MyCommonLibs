#pragma once

#include <iostream>
using namespace std;

namespace SmartLib
{
	class ReverseNodesK2
	{
	public:
		struct ListNode
		{
			const int val;
			ListNode *next;

			ListNode(int x) :
				val(x),
				next(nullptr)
			{
			}
		};

	public:
		static	void ReverseOne(ListNode * p1, ListNode * p2, ListNode * & newp1, ListNode * & newp2)
		{
			newp1 = p2;
			newp2 = newp1 ? newp1->next : nullptr;
			if (p2)
			{
				p2->next = p1;
			}
		}

		static	void ReverseK(ListNode * node, int kk, ListNode * & newHead, ListNode * & newTail, ListNode * & nxtHead)
		{
			int ii = 0;
			ListNode * p1 = nullptr;
			ListNode * p2 = node;
			ListNode * newp1 = nullptr;
			ListNode * newp2 = nullptr;
			for (; ii < kk; ++ii)
			{
				if (p2)
				{
					ReverseOne(p1, p2, newp1, newp2);
					p1 = newp1;
					p2 = newp2;
				}
				else
				{
					break;
				}

			}

			if (ii == kk)
			{
				newHead = newp1;
				newTail = node;
				nxtHead = newp2;
				newTail->next = nxtHead;
			}
			else
			{
				newHead = newTail = nxtHead = nullptr;
				ReverseK(p1, ii, newHead, newTail, nxtHead);
			}
		}

		static	ListNode * ReverseKGroup(ListNode * head, int kk)
		{
			if (head == nullptr || 1 == kk)
			{
				return head;
			}

			ListNode * newHead = nullptr;
			ListNode * newTail = nullptr;
			ListNode * nxtHead = nullptr;
			ReverseK(head, kk, newHead, newTail, nxtHead);

			ListNode * rv = newHead;
			while (nxtHead)
			{
				auto tail = newTail;
				ReverseK(nxtHead, kk, newHead, newTail, nxtHead);
				tail->next = newHead;
			}

			return rv;
		}
	};

	class ReverseNodesK2Test
	{
		typedef ReverseNodesK2::ListNode ListNode;
	public:

		static ListNode * CreateList(int count)
		{
			ListNode * head = nullptr;
			ListNode * cur = nullptr;

			for (int ii = 1; ii <= count; ++ ii)
			{
				if (head == nullptr)
				{
					cur = new ListNode(ii);
					head = cur;
				}
				else
				{
					cur->next = new ListNode(ii);
					cur = cur->next;
				}
			}

			return head;
		}


		static void Print(ListNode * node)
		{
			while (node)
			{
				cout << node->val << " ";
				node = node->next;
			}
			cout << endl;
		}
		

		static void Case0()
		{
			int count = 5;
			int kk = 2;

			auto head = CreateList(count);
			Print(head);
			auto newhead = ReverseNodesK2::ReverseKGroup(head, kk);
			Print(newhead);
		}
	};

}
