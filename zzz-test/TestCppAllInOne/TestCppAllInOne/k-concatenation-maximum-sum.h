#pragma once

#include <vector>
#include <algorithm>

//https://leetcode.com/problems/k-concatenation-maximum-sum/submissions/

namespace SmartLib
{
	using namespace ::std;


	class Solution_k_concatenation_maximum_sum2
	{
	public:

		static int kConcatenationMaxSum( vector<int>& arr, int k)
		{
			if (arr.empty())
			{
				return 0;
			}

			int sum = 0;
			int max = 0;

			int size = arr.size();
			int kk = k * size;
			int index = 0;

			int* start = &arr[0];
			int* stop = start + arr.size();
			int* first = start;
			for (int ii = 0; ii < kk; ++ii)
			{
				if (sum >= 0)
				{
					if (max < sum)
					{
						max = sum;
					}

					sum += *first;
				}
				else
				{
					sum = *first;
				}

				++first;
				if (first == stop)
				{
					first = start;
				}
			}

			if (max < sum)
			{
				max = sum;
			}

			return max % 1000000007LL;
		}
	};
	

	class Solution_k_concatenation_maximum_sum
	{
	public:

		static int kConcatenationMaxSum( vector<int>& arr, int k)
		{
			if (arr.empty())
			{
				return 0;
			}

			long long sum = 0;
			long long max = 0;

			long long size = arr.size();
			long long kk = k * size;
			int index = 0;
			
			int* start = &arr[0];
			int* stop = start + arr.size();
			int* first = start;
			for (int ii = 0; ii < kk; ++ii)
			{
				if (sum >= 0)
				{
					if (max < sum)
					{
						max = sum;
					}

					sum += *first;
				}
				else
				{
					sum = *first;
				}

				++first;
				if (first == stop)
				{
					first = start;
				}
			}

			if (max < sum)
			{
				max = sum;
			}

			return max % 1000000007LL;
		}
	};
}