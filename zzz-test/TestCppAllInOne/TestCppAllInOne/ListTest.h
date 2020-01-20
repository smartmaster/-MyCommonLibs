#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <functional>

namespace SmartLib
{
	using namespace ::std;

	struct ListNode
	{
		int val;
		ListNode* next;
		ListNode(int x) : val(x), next(nullptr) {}
	};

	//https://leetcode.com/problems/longest-substring-without-repeating-characters/
	class LongestSubstringWithoutRepeatingCharacters
	{
		class ArrayMap
		{
			int _array[128];
			int _size{ 0 };

		public:
			ArrayMap()
			{
				memset(_array, 0xff, sizeof(_array));
			}

			void Insert(char ch, int pos)
			{
				_array[ch] = pos;
				++_size;
			}

			int Find(char ch)
			{
				return _array[ch];
			}

			void Remove(char ch)
			{
				_array[ch] = -1;
				--_size;
			}

			int Size()
			{
				return _size;
			}
		};

		static int lengthOfLongestSubstring(const string& str)
		{
			int maxlen = 0;
			ArrayMap mm{};
			int size = str.size();
			for (int ii = 0; ii < size; ++ii)
			{
				char ch = str[ii];
				int pos = mm.Find(ch);
				if (-1 == pos)
				{
					mm.Insert(ch, ii);
				}
				else
				{
					if (maxlen < mm.Size())
					{
						maxlen = mm.Size();
					}

					int removeCount = mm.Size() - (ii - pos) + 1;
					while (removeCount > 0)
					{
						mm.Remove(str[pos]);
						--pos;
						--removeCount;
					}
					
					mm.Insert(ch, ii);
				}
			}

			if (maxlen < mm.Size())
			{
				maxlen = mm.Size();
			}

			return maxlen;
		}
	};

	//https://leetcode.com/problems/intersection-of-two-linked-lists/
	class ListTest8
	{
	public:

		static ListNode* FindIntersection(ListNode* cur, ListNode* longer, ListNode* shorter)
		{
			int count = 0;
			while (cur)
			{
				++count;
				cur = cur->next;
			}

			while (count > 0)
			{
				longer = longer->next;
				--count;
			}


			while (longer && shorter)
			{
				if (longer == shorter)
				{
					return longer;
				}

				longer = longer->next;
				shorter = shorter->next;
			}

			return nullptr;
		}

		static ListNode* getIntersectionNode(ListNode* headA, ListNode* headB)
		{
			ListNode* curA = headA;
			ListNode* curB = headB;

			while (curA && curB)
			{
				curA = curA->next;
				curB = curB->next;
			}

			if (curA)
			{
				return FindIntersection(curA, headA, headB);
			}
			else
			{
				return FindIntersection(curB, headB, headA);
			}
		}
	};
	 



	class CFastQuickSort
	{
	public:
		static int* FindLarger(int data, int* first, int* stop)
		{
			while (first != stop)
			{
				if (*first > data)
				{
					break;
				}
				++first;
			}

			return first;
		}

		static int* FindSmallerOrEqual(int data, int* first, int* stop)
		{
			while (first != stop)
			{
				if (*first <= data)
				{
					break;
				}
				++first;
			}

			return first;
		}

		static void SwapRange(int* p1, int* p2, int* p3)
		{
			int count1 = p2 - p1;
			int count2 = p3 - p2;
			int count = count1 < count2 ? count1 : count2;
			--p3;
			while (count > 0)
			{
				int temp = *p1;
				*p1 = *p3;
				*p3 = temp;
				++p1;
				--p3;
				--count;
			}
		}

		template<typename T>
		static void MySwap(T& aa, T& bb)
		{
			T temp = move(aa);
			aa = move(bb);
			bb = move(temp);
		}

		static int* Partition(int* first, int* stop)
		{
			int pivot = *first;
			int* larger = FindLarger(pivot, first, stop);
			int* lessOrEqual = FindSmallerOrEqual(pivot, larger, stop);
			int* nextLarger = FindLarger(pivot, lessOrEqual, stop);
			while (lessOrEqual != stop)
			{
				SwapRange(larger, lessOrEqual, nextLarger);
				larger += (nextLarger - lessOrEqual);
				lessOrEqual = FindSmallerOrEqual(pivot, nextLarger, stop);
				nextLarger = FindLarger(pivot, lessOrEqual, stop);
			}
			MySwap(first[0], larger[-1]);
			return larger;
		}

		static void QuickSort(int* first, int* stop)
		{
			if (first == stop)
			{
				return;
			}

			int* larger = Partition(first, stop);
			QuickSort(first, larger - 1);
			QuickSort(larger, stop);
		}


		static vector<int> sortArray(vector<int>& nums) 
		{
			if (nums.empty())
			{
				return {};
			}

			int* first = &nums[0];
			int* stop = first + nums.size();
			QuickSort(first, stop);
			return nums;
		}
	};


	class ListTest7
	{
	public:
		static ListNode* FindLarger(int data, ListNode* head)
		{
			while (head->next)
			{
				if (head->next->val >= data)
				{
					break;
				}
				head = head->next;
			}
			return head;
		}

		static ListNode* insertionSortList(ListNode* head)
		{
			if (nullptr == head)
			{
				return nullptr;
			}

			ListNode* next = nullptr;
			ListNode start{ 0 };
			while (head)
			{
				next = head->next;
			
				ListNode* pos = FindLarger(head->val , &start);
				head->next = pos->next;
				pos->next = head;
				
				head = next;
			}

			return start.next;
		}
	};


	class ListTest6
	{

	public:
		static ListNode* Partition(ListNode* head, ListNode* stop, ListNode*& efirst, ListNode*& elast)
		{
			int pivot = head->val;
			ListNode* cur = head;
			

			ListNode smaller{ 0 };
			ListNode equal{ 0 };
			ListNode larger{ 0 };

			ListNode* curSmaller = &smaller;
			ListNode* curLarger = &larger;
			ListNode* curEqual = &equal;

			while (cur != stop)
			{
				if (cur->val < pivot )
				{
					curSmaller->next = cur;
					curSmaller = cur;
				}
				else if (cur->val == pivot)
				{
					curEqual->next = cur;
					curEqual = cur;
				}
				else
				{
					curLarger->next = cur;
					curLarger = cur;
				}
				cur = cur->next;
			}

			curSmaller->next = equal.next;
			if (larger.next)
			{
				curEqual->next = larger.next;
				curLarger->next = stop;
			}
			else
			{
				curEqual->next = stop;
			}


			efirst = equal.next;
			elast = curEqual;

			return smaller.next;
		}

		static ListNode* QuickSort(ListNode* head, ListNode* stop)
		{
			if (nullptr == head || stop == head)
			{
				return nullptr;
			}

			if (stop == head->next)
			{
				return head;
			}


			ListNode* efirst = nullptr; 
			ListNode* elast = nullptr;
			ListNode* newHead = Partition(head, stop, efirst, elast);


			ListNode* left = QuickSort(newHead, efirst);
			if (left == nullptr)
			{
				left = newHead;
			}

			ListNode* right = QuickSort(elast->next, stop);
			if (right)
			{
				elast->next = right;
			}
			else
			{
				elast->next = stop;
			}
			return left;
		}


		static ListNode* sortList(ListNode* head)
		{
			return QuickSort(head, nullptr);
		}
	};

	class ListTest5
	{
	public:
		static ListNode* Split2(ListNode* head)
		{
			ListNode start{ 0 };
			start.next = head;

			ListNode* slow = &start;
			ListNode* fast = &start;

			while (fast && fast->next)
			{
				slow = slow->next;
				fast = fast->next->next;
			}

			ListNode* right = slow->next;
			slow->next = nullptr;
			return right;
		}

		
		static ListNode* MergeList(ListNode* left, ListNode* right)
		{
			ListNode start{ 0 };
			ListNode* cur = &start;

			while (left && right)
			{
				if (left->val < right->val)
				{
					cur->next = left;
					cur = left;
					left = left->next;
				}
				else
				{
					cur->next = right;
					cur = right;
					right = right->next;
				}
			}

			cur->next = left ? left : right;
			return start.next;
		}

		static ListNode* MergeSort2(ListNode* head)
		{
			if (nullptr == head || nullptr == head->next)
			{
				return head;
			}

			ListNode* right = Split2(head);
			head = MergeSort2(head);
			right = MergeSort2(right);
			return MergeList(head, right);
		}

		

		static ListNode* sortList(ListNode* head)
		{
			return MergeSort2(head);
		}
	};

	class ListTest4
	{
	public:
		
		

		static void Split(ListNode* head, ListNode*& left, ListNode*& right)
		{
			ListNode start1{ 0 };
			ListNode start2{ 0 };

			ListNode* curStart1 = &start1;
			ListNode* curStart2 = &start2;
			bool flag = true;
			while (head)
			{
				if (flag)
				{
					curStart1->next = head;
					curStart1 = head;
				}
				else
				{
					curStart2->next = head;
					curStart2 = head;
				}
				head = head->next;
				flag = !flag;
			}

			curStart1->next = nullptr;
			curStart2->next = nullptr;

			left = start1.next;
			right = start2.next;
		}

		static ListNode* MergeList(ListNode* left, ListNode* right)
		{
			ListNode start{ 0 };
			ListNode* cur = &start;

			while (left && right)
			{
				if (left->val < right->val)
				{
					cur->next = left;
					cur = left;
					left = left->next;
				}
				else
				{
					cur->next = right;
					cur = right;
					right = right->next;
				}
			}

			cur->next = left ? left : right;
			return start.next;
		}

		

		static ListNode* MergeSort(ListNode* head)
		{
			if (nullptr == head || nullptr == head->next)
			{
				return head;
			}
			ListNode* left = nullptr;
			ListNode* right = nullptr;
			Split(head, left, right);
			left = MergeSort(left);
			right = MergeSort(right);
			return MergeList(left, right);
		}

		static ListNode* sortList(ListNode* head)
		{
			return MergeSort(head);
		}
	};


	class ListTest3
	{
	public:
		static ListNode* FindMid(ListNode* head)
		{
			if (nullptr == head || nullptr == head->next)
			{
				return head;
			}


			ListNode start{ 0 };
			start.next = head;

			auto slow = &start;
			auto fast = &start;

			while (fast && fast->next)
			{
				fast = fast->next->next;
				slow = slow->next;
			}
			return slow;
		}

		static ListNode* Merge(ListNode* head1, ListNode* head2)
		{
			ListNode start{ 0 };
			auto cur = &start;
			while (head1 && head2)
			{
				if (head1->val < head2->val)
				{
					cur->next = head1;
					head1 = head1->next;
				}
				else
				{
					cur->next = head2;
					head2 = head2->next;
				}

				cur = cur->next;
			}

			cur->next = head1 ? head1 : head2;
			return start.next;
		}

		static ListNode* MergeSort(ListNode* head)
		{
			if (nullptr == head || nullptr == head->next)
			{
				return head;
			}

			ListNode* mid = FindMid(head);
			ListNode* left = nullptr;
			ListNode* right = nullptr;
			if (mid)
			{
				right = mid->next;
				mid->next = nullptr;
				left = MergeSort(head);
			}
			
			if (right)
			{
				right = MergeSort(right);
			}
			
			return Merge(left, right);
		}

		static ListNode* sortList(ListNode* head)
		{
			return MergeSort(head);
		}
	};

	class ListTest2
	{
	public:

		//https://leetcode.com/problems/sort-list/
//Sort a linked list in O(n log n) time using constant space complexity.

		struct Pair3
		{
			ListNode* p1{ nullptr };
			ListNode* p2{ nullptr };
			ListNode* p3{ nullptr };

			Pair3(ListNode* p1_, ListNode* p2_, ListNode* p3_) :
				p1{ p1_ },
				p2{ p2_ },
				p3{ p3_ }
			{
			}

			void MoveNext()
			{
				p1 = p2;
				p2 = p3;
				p3 = p3 ? p3->next : nullptr;
			}
		};



		static Pair3 FindLarger(int val, ListNode* first, ListNode* stop)
		{
			Pair3 ppp{ nullptr, nullptr, first };

			while (ppp.p3 != stop)
			{
				if (ppp.p3->val > val)
				{
					break;
				}
				ppp.MoveNext();
			}
			return ppp;
		}


		static Pair3 FindEqualSmaller(int val, ListNode* first, ListNode* stop)
		{
			Pair3 ppp{ nullptr, nullptr, first };

			while (ppp.p3 != stop)
			{
				if (ppp.p3->val <= val)
				{
					break;
				}
				ppp.MoveNext();
			}

			return ppp;
		}





		static Pair3 Partition(ListNode* first, ListNode* stop)
		{
			int val = first->val;
			Pair3 larger = FindLarger(val, first, stop);
			Pair3 smaller = FindEqualSmaller(val, larger.p3, stop);

			while (smaller.p3 != stop)
			{
				int& aa = larger.p3->val;
				int& bb = smaller.p3->val;
				int temp = aa;
				aa = bb;
				bb = temp;
				larger.MoveNext();
				smaller = FindEqualSmaller(val, smaller.p3, stop);
			}

			if (larger.p2 != first)
			{
				int& aa = larger.p2->val;
				int& bb = first->val;
				int temp = aa;
				aa = bb;
				bb = temp;
			}

			return larger;
		}

		static void QuickSort(ListNode* first, ListNode* stop)
		{
			auto ppp = Partition(first, stop);

			if (ppp.p1)
			{
				QuickSort(first, ppp.p2);
			}

			if (ppp.p3 && ppp.p3 != stop)
			{
				QuickSort(ppp.p3, stop);
			}

		}

		static ListNode* sortList(ListNode* head)
		{
			if (nullptr == head || nullptr == head->next)
			{
				return head;
			}

			QuickSort(head, nullptr);
			return head;
		}
	};

	class ListTest
	{
	public:

		static ListNode* MakeList(const vector<int> & data)
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

		static ListNode* removeElements(ListNode* head, int val) 
		{
			ListNode start{ 0 };
			start.next = head;
			ListNode* cur = &start;
			while (cur && cur->next)
			{
				if (cur->next->val == val)
				{
					cur->next = cur->next->next;
				}
				else
				{
					cur = cur->next;
				}

			}
			return start.next;
		}

		static ListNode* Reverse(ListNode* nn)
		{
			if (nullptr == nn)
			{
				return nullptr;
			}

			ListNode* p1 = nullptr;
			ListNode* p2 = nn;
			ListNode* p3 = nn->next;

			while (p3)
			{
				p2->next = p1;
				p1 = p2;
				p2 = p3;
				p3 = p3->next;
			}

			return p2;
		}

		//https://leetcode.com/problems/sort-list/
		//Sort a linked list in O(n log n) time using constant space complexity.
		struct Pair3
		{
			ListNode* p1{ nullptr };
			ListNode* p2{ nullptr };
			ListNode* p3{ nullptr };

			Pair3(ListNode* p1_, ListNode* p2_, ListNode* p3_) :
				p1{ p1_ },
				p2{ p2_ },
				p3{ p3_ }
			{
			}

			void MoveNext()
			{
				p1 = p2;
				p2 = p3;
				p3 = p3 ? p3->next : nullptr;
			}
		};

		

		static Pair3 Find(function<bool(const ListNode*)> && test, ListNode* first, ListNode* stop)
		{

			Pair3 ppp{nullptr, nullptr, first};

			while (ppp.p3 != stop)
			{
				if (test(ppp.p3))
				{
					break;
				}
				ppp.MoveNext();
			}
			
			return ppp;
		}

		

		template<typename T>
		static void SwapHelper(T& aa, T& bb)
		{
			if (&aa != &bb)
			{
				T temp = move(aa);
				aa = move(bb);
				bb = move(temp);
			}
			
		}

		static void SwapPtr(Pair3& larger, Pair3& smaller)
		{
			if (smaller.p2)
			{
				smaller.p2->next = smaller.p3->next;
			}
			
			if (larger.p2)
			{
				larger.p2->next = smaller.p3;
			}
			
			if (smaller.p3)
			{
				smaller.p3->next = larger.p3;
			}
			
			larger.p1 = larger.p2;
			larger.p2 = smaller.p3;

			if (smaller.p2)
			{
				smaller.p3 = smaller.p2->next;
			}
			
		}


		static Pair3 Partition(ListNode* first, ListNode* stop)
		{
			auto largerFunc = [first](const ListNode* nn)
			{
				return nn->val > first->val;
			};

			auto equalSmallerFunc = [first](const ListNode* nn)
			{
				return nn->val <= first->val;
			};

			Pair3 larger = Find(largerFunc, first, stop);
			Pair3 smaller = Find(equalSmallerFunc, larger.p3, stop);

			while (smaller.p3 != stop)
			{
				SwapHelper(larger.p3->val, smaller.p3->val);
				larger.MoveNext();
				//SwapPtr(larger, smaller);
				smaller = Find(equalSmallerFunc, smaller.p3, stop);
			}

			if (larger.p2)
			{
				SwapHelper(larger.p2->val, first->val);
			}
			
			return larger;
		}

		static void QuickSort(ListNode* first, ListNode* stop)
		{
			auto ppp = Partition(first, stop);

			if (ppp.p1)
			{
				QuickSort(first, ppp.p2);
			}

			if (ppp.p3 && ppp.p3 != stop)
			{
				QuickSort(ppp.p3, stop);
			}

		}

		static ListNode* sortList(ListNode* head)
		{
			if (nullptr == head || nullptr == head->next)
			{
				return head;
			}

			QuickSort(head, nullptr);
			return head;
		}

		static ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
		{
			ListNode start{ 0 };
			ListNode* cur = &start;

			while (l1 && l2)
			{
				if (l1->val < l2->val)
				{
					cur->next = l1;
					l1 = l1->next;
				}
				else
				{
					cur->next = l2;
					l2 = l2->next;
				}

				cur = cur->next;
			}


			cur->next = l1 ? l1 : l2;
			return start.next;
		}

	};

}