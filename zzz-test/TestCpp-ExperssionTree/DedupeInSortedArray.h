#pragma once

#include <vector>

#include "log.h"

namespace SmartLib
{
	class DedupeInSortedArray
	{
	public:
		static int FindUnique(int * arr, int start, int end)
		{
			while (start <= end && arr[start] == arr[start - 1])
			{
				++start;
			}

			return start;
		}

		static int Dedupe(int  *arr, int size)
		{
			int pos = 1;
			int uniq = 1;

			for(;;)
			{
				uniq = FindUnique(arr, uniq, size - 1);
				if (uniq < size)
				{
					arr[pos++] = arr[uniq++];
				}
				else
				{
					break;
				}
			}

			return pos;
		}

	static	int removeDuplicates(std::vector<int>& nums) 
		{
			if (nums.size() == 0)
			{
				return 0;
			}
			else
			{
				return Dedupe(&nums[0], nums.size());
			}
		}
	};

	class DedupeInSortedArrayTest
	{
	public:
		static void Case0()
		{
			std::vector<int> vec{ 1, 2 };
			int rr = DedupeInSortedArray::removeDuplicates(vec);
			for (int ii = 0; ii < rr; ++ii)
			{
				LOG_A("%d ", vec[ii]);
			}
			LOG_LINE_A("");
		}
	};
}
