#pragma once

#include <vector>
#include <algorithm>
using namespace std;

namespace SmartLib
{
	class TwoSum
	{
		vector<int> twoSum(vector<int>& nums, int target) {

			auto comp = [&nums](int ii, int jj)
			{
				return nums[ii] <= nums[jj];
			};

			vector<int> ind(nums.size());
			for (int ii = 0; ii < nums.size(); ++ii)
			{
				ind[ii] = ii;
			}

			std::sort(ind.begin(), ind.end(), comp);


			auto bin_search = [&nums, &ind](int target)
			{
				int lo = 0;
				int hi = ind.size() - 1;
				int mid = -1;
				int temp = -1;
				while (lo <= hi)
				{
					mid = ((lo + hi) >> 1);
					temp = nums[ind[mid]];
					if (temp == target)
					{
						return mid;
					}
					else if (temp > target)
					{
						hi = mid - 1;
					}
					else
					{
						lo = mid + 1;
					}
				}


				return -1;
			};

			for (int ii = 0; ii < ind.size(); ++ii)
			{
				int tt = target - nums[ind[ii]];
				int pos = bin_search(tt);
				if (pos != -1 && pos != ii)
				{
					return { ind[ii], ind[pos] };
				}
			}


			return {};
		}
	};
}