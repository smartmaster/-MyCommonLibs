#pragma once

#include <vector>
using namespace std;

#include "log.h"

namespace SmartLib
{
	class SearchinRotatedSortedArray
	{
	private:
		const vector<int> & _vec;
	private:

		SearchinRotatedSortedArray(const vector<int> & vec) :
			_vec(vec)
		{

		}

		int FindSmallest()
		{
			int lo = 0;
			int hi = _vec.size() - 1;
			int mid = -1;
			while (lo < hi)
			{
				mid = ((lo + hi) >> 1);
				if (_vec[mid] > _vec[hi])
				{
					lo = mid + 1;
				}
				else
				{
					hi = mid;
				}
			}
			return lo;
		}


		int RealIndex(int vindex, int offset)
		{
			return (vindex + offset) % _vec.size();
		}

		int FindTarget(int target)
		{
			int offset = FindSmallest();

			int lo = 0;
			int hi = _vec.size();
			int mid = -1;
			int realMid = -1;
			int midValue = -1;
			while (lo <= hi)
			{
				mid = ((lo + hi) >> 1);
				realMid = RealIndex(mid, offset);
				midValue = _vec[realMid];
				if (midValue == target)
				{
					return realMid;
				}
				else if (midValue < target)
				{
					lo = mid + 1;
				}
				else
				{
					hi = mid - 1;
				}
			}


			return -1;
		}

	public:
		static int SearchAPI(vector<int>& nums, int target)
		{
			if (nums.size() == 0)
			{
				return -1;
			}

			SearchinRotatedSortedArray srsa(nums);
			return srsa.FindTarget(target);
		}
	};


	class SearchinRotatedSortedArrayTest
	{
	public:
		static void Case0()
		{
			{
				vector<int> vec{ 8,9,2,3,4 };
				int target = 9;
				int rr = SearchinRotatedSortedArray::SearchAPI(vec, target);
				LOG_LINE_A("index = %d", rr);
			}



			{
				vector<int> vec{ 4,5,6,7,0,1,2 };
				int target = 0;
				int rr = SearchinRotatedSortedArray::SearchAPI(vec, target);
				LOG_LINE_A("index = %d", rr);
			}


			{
				vector<int> vec{ 4,5,6,7,0,1,2 };
				int target = 3;
				int rr = SearchinRotatedSortedArray::SearchAPI(vec, target);
				LOG_LINE_A("index = %d", rr);
			}
			
		}
	};
}