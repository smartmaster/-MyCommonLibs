#pragma once


/***************************
https://leetcode.com/problems/merge-k-sorted-lists/description/
Merge k sorted linked lists and return it as one sorted list. Analyze and describe its complexity.

Example:

Input:
[
1->4->5,
1->3->4,
2->6
]
Output: 1->1->2->3->4->4->5->6

********************************/

#include <vector>
using namespace std;

#include "HeapSort.h"

namespace SmartLib
{
	class MergeSortedLists
	{
	public:
		struct ListNode
		{
			int val;
			ListNode *next;
			ListNode(int x) : val(x), next(NULL) {}
		};

	private:
		vector<ListNode*> & _lists;
	
	private:

		MergeSortedLists(vector<ListNode*> & lists) :
			_lists(lists)
		{

		}

		ListNode * Merge()
		{
			vector<ListNode*> current;
			current.reserve(_lists.size());
			for (int ii = 0; ii < _lists.size(); ++ii)
			{
				if (_lists[ii])
				{
					current.push_back(_lists[ii]);
				}
			}

			auto comp = [](ListNode* val1, ListNode* val2)
			{
				return val1->val <= val2->val;
			};

			SmlBinHeap<ListNode*> heap(current, comp);
			heap.MakeHeap();

			ListNode * head = new ListNode(0);
			ListNode * cur = head;
			ListNode * prev = nullptr;
			while (heap.HeapSize())
			{
				ListNode * p = heap.Remove();
				cur->val = p->val;

				prev = cur;
				cur->next = new ListNode(0);
				cur = cur->next;
				if (p->next)
				{
					heap.Add(p->next);
				}
			}

			if (prev == nullptr)
			{
				delete head;
				head = nullptr;
			}
			else if(prev->next)
			{
				delete prev->next;
				prev->next = nullptr;
			}

			return head;
		}


	public:
		static ListNode * Merge(vector<ListNode*> & lists)
		{
			MergeSortedLists msl(lists);
			return msl.Merge();
		}

	};
}