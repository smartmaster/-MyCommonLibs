#pragma once

#include <cassert>
#include <vector>
#include <functional>

namespace SmartLib
{
	using namespace std;

	class CQuickSort2
	{
	private:
		static int* Find(int* first, int* last, function<bool(int)> compare)
		{
			while (first <= last)
			{
				if (compare(*first))
				{
					break;
				}

				++first;
			}
			return first;
		}

		static int* Partition(int* first, int* last)
		{
			int pivot = *first;

			auto cmpLarger = [pivot](int value) { return value > pivot; };
			auto cmpSmaller = [pivot](int value) { return value <= pivot; };

			int* larger = Find(first + 1, last, cmpLarger);
			int* smaller = Find(larger + 1, last, cmpSmaller);
			while (smaller <= last)
			{
				swap(*smaller, *larger);
				++larger;
				++smaller;
				smaller = Find(smaller, last, cmpSmaller);
			}

			swap(*first, *(larger - 1));

			return larger - 1;
		}

	public:

		static void Sort(int* first, int* last)
		{
			if (first >= last)
			{
				return;
			}

			int* part = Partition(first, last);
			Sort(first, part - 1);
			Sort(part + 1, last);
		}
	};

	class CQuickSort
	{
	private:
		static int* FindLarger(int* first, int* last, int* pivot)
		{
			while (first <= last)
			{
				if(*first > *pivot)
				{
					break;
				}
				++first;
			}
			return first;
		}

		static int* ReverseFindSmaller(int* rfirst, int* rlast, int* pivot)
		{
			while (rfirst >= rlast)
			{
				if (*rfirst <= *pivot)
				{
					break;
				}
				--rfirst;
			}
			return rfirst;
		}

		static int* Partition(int* first, int* last)
		{
			int* head = first;
			while (first <= last)
			{
				first = FindLarger(first, last, head);
				last = ReverseFindSmaller(last, first, head);
				if (first < last)
				{
					int temp = *first;
					*first = *last;
					*last = temp;
					++first;
					--last;
				}
				else
				{
					assert(first != last);
				}
			}

			int temp = *head;
			*head = *last;
			*last = temp;
			return last;
		}
	public:

		static void Sort(int* first, int* last)
		{
			if (first >= last)
			{
				return;
			}

			int* part = Partition(first, last);
			Sort(first, part - 1);
			Sort(part + 1, last);
		}
		

		static void SortNR(int* first, int * last)
		{
			struct Tree
			{
				int* _first;
				int* _last;

				Tree(int* first, int* last) :
					_first(first),
					_last(last)
				{
				}
			};

			std::vector<Tree> stack;
			stack.push_back(Tree{first, last});

			while (stack.size())
			{
				Tree top = stack.back();
				stack.pop_back();

				int* part = Partition(top._first, top._last);

				if (top._first < part - 1)
				{
					stack.push_back(Tree{ top._first, part - 1 });
				}

				if (part + 1 < top._last)
				{
					stack.push_back(Tree{ part + 1, top._last});
				}
			}
		}
	};

#if false
	class CQuickSortGeneric
	{
	private:
		template<typename TITERATOR>
		static TITERATOR FindLarger(TITERATOR first, TITERATOR last, TITERATOR pivot)
		{
			while (first <= last)
			{
				if (*first > *pivot)
				{
					break;
				}
				++first;
			}
			return first;
		}

		template<typename TITERATOR>
		static TITERATOR ReverseFindSmaller(TITERATOR rfirst, TITERATOR rlast, TITERATOR pivot)
		{
			while (rfirst >= rlast)
			{
				if (*rfirst <= *pivot)
				{
					break;
				}
				--rfirst;
			}
			return rfirst;
		}


		template<typename TITERATOR>
		static TITERATOR Partition(TITERATOR first, TITERATOR last)
		{
			TITERATOR head = first;
			while (first <= last)
			{
				first = FindLarger(first, last, head);
				last = ReverseFindSmaller(last, first, head);
				if (first < last)
				{
					auto temp = *first;
					*first = *last;
					*last = temp;
					++first;
					--last;
				}
				else
				{
					assert(first != last);
				}
			}

			auto temp = *head;
			*head = *last;
			*last = temp;
			return last;
		}
	public:

		template<typename TITERATOR>
		static void Sort(TITERATOR first, TITERATOR last)
		{
			if (first >= last)
			{
				return;
			}

			TITERATOR part = Partition(first, last);

			TITERATOR left = part;
			--left;
			Sort(first, left);

			TITERATOR right = part;
			++right;
			Sort(right, last);
		}

	};
#endif

}