#pragma once

//https://leetcode.com/problems/partition-list/

#include <vector>
#include <functional>

namespace SmartLib_partition_list
{
	using namespace ::std;

	struct ListNode
	{
		int val;
		ListNode* next;
		ListNode(int x) : val(x), next(nullptr) {}

	};



	struct Pair2
	{
		ListNode* pre{ nullptr };
		ListNode* cur{ nullptr };


		Pair2(ListNode* p1, ListNode* p2) :
			pre{ p1 },
			cur{ p2 }
		{
		}

		void Next()
		{
			pre = cur;
			cur = cur ? cur->next : nullptr;
		}
	};


	struct Pair3
	{
		ListNode* newHead{ nullptr };
		ListNode* midfirst{ nullptr };
		ListNode* midlast{ nullptr };


		Pair3(ListNode* p0, ListNode* p1, ListNode* p2) :
			newHead{ p0 },
			midfirst{ p1 },
			midlast{ p2 }
		{
		}
	};



	class partition_list2
	{
	public:
		static Pair2 FindEqual(function<int(const ListNode*)> mappingfunc, int val, ListNode* pre, ListNode* first, ListNode* stop)
		{
			Pair2 pp{ pre, first };
			while (pp.cur != stop)
			{
				if (mappingfunc(pp.cur) == val)
				{
					break;
				}
				pp.Next();
			}

			return pp;
		}


		static Pair2 FindNotEqual(function<int(const ListNode*)> mappingfunc, int val, ListNode* pre, ListNode* first, ListNode* stop)
		{
			Pair2 pp{ pre, first };
			while (pp.cur != stop)
			{
				if (mappingfunc(pp.cur) != val)
				{
					break;
				}
				pp.Next();
			}

			return pp;
		}

		static void RemoveSublist(Pair2& first, Pair2& stop)
		{
			first.pre->next = stop.cur;
			stop.pre->next = nullptr;
		}

		static Pair3 partition(ListNode* head, ListNode* stop, int pivot)
		{
			ListNode start{ 0 };
			start.next = head;

			ListNode nnSmall{ 0 };
			ListNode nnEqual{ 0 };
			ListNode nnLarge{ 0 };
			ListNode* arrStart[] = { &nnSmall, &nnEqual, &nnLarge };
			ListNode* arrCur[] = {&nnSmall, &nnEqual, &nnLarge };


			auto mappingfunc = [pivot](const ListNode* nn)
			{
				if (nn->val < pivot)
				{
					return 0;
				}
				else if (nn->val == pivot)
				{
					return 1;
				}
				else
				{
					return 2;
				}
			};

			Pair2 pp{ &start, head };
			while (pp.cur != stop)
			{
				int mappedVal = mappingfunc(pp.cur);
				Pair2 newpp = FindNotEqual(mappingfunc, mappedVal, pp.pre, pp.cur, stop);
				RemoveSublist(pp, newpp);
				arrCur[mappedVal]->next = pp.cur;
				arrCur[mappedVal] = newpp.pre;
				pp.cur = newpp.cur;
			}

			start.next = nullptr;
			ListNode* startCur = &start;
			for (int ii = 0; ii < sizeof(arrCur)/sizeof(arrCur[0]); ++ ii)
			{
				if (arrStart[ii]->next)
				{
					startCur->next = arrStart[ii]->next;
					startCur = arrCur[ii];
				}
			}
			startCur->next = stop;
			return Pair3{start.next , arrStart[1]->next , arrCur[1]};
		}

		static ListNode* QuickSort(ListNode* head, ListNode* stop)
		{
			if (head == stop)
			{
				return nullptr;
			}


			Pair3 newheadmid = partition(head, stop, head->val);
			ListNode* left = QuickSort(newheadmid.newHead, newheadmid.midfirst);
			if (left == nullptr)
			{
				left = newheadmid.newHead;
			}


			ListNode* right = QuickSort(newheadmid.midlast->next, stop);
			if (right == nullptr)
			{
				right = newheadmid.midlast->next;
			}

			newheadmid.midlast->next = right;

			return left;
		}


		static ListNode* sortList(ListNode* head) 
		{
			return QuickSort(head, nullptr);
		}

	};

	class partition_list
	{
	public:
		static Pair2 FindSmaller(int data, ListNode* pre, ListNode* first, ListNode* stop)
		{
			Pair2 pp{ pre, first };
			while (pp.cur != stop)
			{
				if (pp.cur->val < data)
				{
					break;
				}
				pp.Next();
			}

			return pp;
		}

		static Pair2 FindLargerOrEqual(int data, ListNode* pre, ListNode* first, ListNode* stop)
		{
			Pair2 pp{ pre, first };
			while (pp.cur != stop)
			{
				if (pp.cur->val >= data)
				{
					break;
				}
				pp.Next();
			}

			return pp;
		}

		static void RemoveSublist(Pair2& first, Pair2& stop)
		{
			first.pre->next = stop.cur;
			stop.pre->next = nullptr;
		}
	};


	class Solution
	{
	public:
		static ListNode* partition(ListNode* head, int data)
		{
			ListNode startList{ 0 };
			startList.next = head;

			ListNode smallList{ 0 };

			Pair2 smaller = partition_list::FindSmaller(data, &startList, head, nullptr);
			Pair2 largerOrEqual = partition_list::FindLargerOrEqual(data, smaller.pre, smaller.cur, nullptr);

			ListNode* smallListCur = &smallList;
			while (smaller.cur)
			{
				partition_list::RemoveSublist(smaller, largerOrEqual);
				smallListCur->next = smaller.cur;
				smallListCur = largerOrEqual.pre;

				smaller = partition_list::FindSmaller(data, smaller.pre, largerOrEqual.cur, nullptr);
				largerOrEqual = partition_list::FindLargerOrEqual(data, smaller.pre, smaller.cur, nullptr);
			}

			smallListCur->next = startList.next;
			return smallList.next;
		}
	};
}