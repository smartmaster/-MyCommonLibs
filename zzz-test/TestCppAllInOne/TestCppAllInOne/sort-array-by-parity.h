#pragma once

#include <vector>

//https://leetcode.com/problems/sort-array-by-parity/

namespace SmartLib
{
	using namespace ::std;

	class sort_array_by_parity
	{
	public:
		static int* FindEven(int* first, int* stop)
		{
			while (first != stop)
			{
				if (0 == (*first & 1))
				{
					break;
				}
				++first;
			}

			return first;
		}

		static int* FindOdd(int* first, int* stop)
		{
			while (first != stop)
			{
				if (*first & 1)
				{
					break;
				}
				++first;
			}

			return first;
		}

		static void SwapRange(int* first, int count1, int count2)
		{
			int count = count1 < count2 ? count1 : count2;
			int* last = first + count1 + count2 - 1;
			while (count)
			{
				int temp = *first;
				*first = *last;
				*last = temp;

				++first;
				--last;

				--count;
			}
		}

		static void Partition(int* first, int* stop)
		{
			int* odd = FindOdd(first, stop);
			int* even = FindEven(odd, stop);
			int* nextodd = FindOdd(even, stop);
			while (even != stop)
			{
				int count1 = even - odd;
				int count2 = nextodd - even;
				SwapRange(odd, count1, count2);
				odd += count2;
				even = FindEven(nextodd, stop);
				nextodd = FindOdd(even, stop);
			}
		}
	};

	class Solution_sort_array_by_parity
	{
	public:
		static vector<int> sortArrayByParity(vector<int>& A) {
			vector<int> newA = A;
			int* first = &newA.front();
			int* stop = first + newA.size();
			sort_array_by_parity::Partition(first, stop);
			return newA;
		}
	};

}