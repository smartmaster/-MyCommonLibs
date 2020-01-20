#pragma once


/*
https://leetcode.com/problems/reverse-nodes-in-k-group/description/

Given a linked list, reverse the nodes of a linked list k at a time and return its modified list.

k is a positive integer and is less than or equal to the length of the linked list. If the number of nodes is not a multiple of k then left-out nodes in the end should remain as it is.

Example:

Given this linked list: 1->2->3->4->5

For k = 2, you should return: 2->1->4->3->5

For k = 3, you should return: 3->2->1->4->5

Note:

Only constant extra memory is allowed.
You may not alter the values in the list's nodes, only nodes itself may be changed.

*/

//#include <vector>

#include <iostream>
using namespace std;

namespace SmartLib
{
	class ReverseNodesK
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


	private:
		const int _kk;
		ListNode** _vec{ nullptr };
		int _size{ 0 };
	private:

	public:
		ReverseNodesK(int kk) :
			_kk(kk)
		{
			_vec = new ListNode*[_kk];
		}

		~ReverseNodesK()
		{
			delete _vec;
			_vec = nullptr;
			_size = 0;
		}


		void PushKNodes(ListNode * node)
		{
			for (int ii = 0; ii < _kk; ++ii)
			{
				if (node)
				{
					_vec[ii] = node;
					++_size;
					node = node->next;
				}
				else
				{
					break;
				}
			}
		}


		void ProcessNodes(ListNode * & newHead, ListNode * & newTail, ListNode * & nexthead)
		{
			if (_size == _kk)
			{
				newHead = _vec[_kk - 1];
				newTail = _vec[0];
				nexthead = newHead->next;

				for (int ii = _kk - 1; ii >= 1; --ii)
				{
					_vec[ii]->next = _vec[ii - 1];
				}

				newTail->next = nexthead;
			}
			else
			{
				newHead = _vec[0];
				newTail = _vec[_size - 1];
				nexthead = nullptr;
			}
			_size = 0;
		}

		ListNode* ReverseKGroup(ListNode* head)
		{
			ListNode * newHead = nullptr;
			ListNode * newTail = nullptr;
			ListNode * nextHead = nullptr;

			PushKNodes(head);
			ProcessNodes(newHead, newTail, nextHead);

			ListNode * result = newHead;

			while (nextHead)
			{
				auto tempTail = newTail;
				PushKNodes(nextHead);
				ProcessNodes(newHead, newTail, nextHead);
				tempTail->next = newHead;
			}


			return result;
		}


		static ListNode * API(ListNode* head, int kk)
		{
			if (nullptr == head || 1 == kk)
			{
				return head;
			}

			ReverseNodesK rnk(kk);
			return rnk.ReverseKGroup(head);
		}
	};


	class ReverseNodesKTest
	{
		typedef ReverseNodesK::ListNode ListNode;

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


		static void Case0()
		{
			int count = 5;
			int kk = 2;

			auto head = CreateList(count);
			Print(head);
			auto newhead = ReverseNodesK::API(head, kk);
			Print(newhead);
		}
	};

}