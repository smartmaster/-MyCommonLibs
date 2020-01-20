#pragma once

#include <iostream>
using namespace std;

namespace SmartLib
{
	class ReverseSList
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
		static	ListNode * Reverse(ListNode * head)
		{
			ListNode * newHead = nullptr;
			auto cur = head;
			while (cur)
			{
				auto next = cur->next;
				cur->next = newHead;
				newHead = cur;
				cur = next;
			}

			return newHead;
		}

		struct Result
		{
			ListNode * _newHead{ nullptr };
			ListNode * _newTail{ nullptr };
			ListNode * _nextNode{ nullptr };
			int _size{ 0 };
		};

		static	Result ReverseK(ListNode * node, int kk)
		{
			Result rv;

			auto cur = node;
			int ii = 0;
			for (; ii < kk; ++ ii)
			{
				if (cur)
				{
					auto next = cur->next;
					cur->next = rv._newHead;
					rv._newHead = cur;
					cur = next;
				}
				else
				{
					break;
				}
			}
			
			rv._nextNode = cur;
			rv._newTail = node;
			rv._size = ii;

			if (ii < kk)
			{
				rv = ReverseK(rv._newHead, ii);
			}

			return rv;
		}

		static ListNode * ReverseKGroup(ListNode * head, int kk)
		{

			if (head == nullptr || 1 == kk)
			{
				return head;
			}

			Result rv = ReverseK(head, kk);
			auto rvHead = rv._newHead;
			while (rv._nextNode)
			{
				auto tail = rv._newTail;
				rv = ReverseK(rv._nextNode, kk);
				tail->next = rv._newHead;
			}
			return rvHead;
		}

	};

	class ReverseSListTest
	{
	public:
		typedef ReverseSList::ListNode ListNode;

	public:

		static ListNode * CreateList(int count)
		{
			ListNode * head = nullptr;
			ListNode * cur = nullptr;

			for (int ii = 1; ii <= count; ++ii)
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

		static void Case1()
		{
			int count = 8;
			int kk = 2;

			auto head = CreateList(count);
			Print(head);
			auto newhead = ReverseSList::ReverseKGroup(head, kk);
			Print(newhead);
		}


		static void Case0()
		{
			int count = 5;
			int kk = 2;

			auto head = CreateList(count);
			Print(head);
			auto newhead = ReverseSList::Reverse(head);
			Print(newhead);
		}
	};
}