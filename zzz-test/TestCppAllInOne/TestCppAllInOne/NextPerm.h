#pragma once

//https://leetcode.com/problems/next-permutation/

/*
Implement next permutation, which rearranges numbers into the lexicographically next greater permutation of numbers.

If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).

The replacement must be in-place and use only constant extra memory.

Here are some examples. Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.

1,2,3 ¡ú 1,3,2
3,2,1 ¡ú 1,2,3
1,1,5 ¡ú 1,5,1
*/

#include <vector>

namespace SmartLib
{
	using namespace ::std;

	class NextPerm
	{
	private:
		vector<int>& _data;

		void SwapByIndex(int ii, int jj)
		{
			int temp = _data[ii];
			_data[ii] = _data[jj];
			_data[jj] = temp;
		}

		void Reverse(int first, int last)
		{
			while (first < last)
			{
				SwapByIndex(first, last);
				++first;
				--last;
			}
		}

		int FindDescendingSequence()
		{
			int last = _data.size();
			last -= 2;
			while (last >= 0)
			{
				if (_data[last] < _data[last + 1])
				{
					break;
				}
				--last;
			}

			return last;
		}

		int FindFirstlargerBin(int data, int first, int last)
		{
			
			while (first <= last)
			{
				int mid = (first + last) / 2;
				int midData = _data[mid];

				if (midData <= data)
				{
					first = mid + 1;
				}
				else
				{
					if (first == last)
					{
						return mid;
					}
					last = mid;
				}
			}

			return -1;
		}


	public:
		NextPerm(vector<int>& data) :
			_data(data)
		{
		}

		void Perm()
		{
			int pos = FindDescendingSequence();
			if (pos < 0)
			{
				Reverse(0, _data.size() - 1);
			}
			else
			{
				Reverse(pos + 1, _data.size() - 1);
				int largerPos = FindFirstlargerBin(_data[pos], pos + 1, _data.size() -1);
				SwapByIndex(pos, largerPos);
			}
		}
	};
}


//class Solution {
//public:
//	void nextPermutation(std::vector<int>& nums) {
//		SmartLib::NextPerm(nums).Perm();
//	}
//};