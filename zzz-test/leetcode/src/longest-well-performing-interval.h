#pragma once

//https://leetcode.com/problems/longest-well-performing-interval/

#include <map>
#include <unordered_map>
#include <tuple>
#include <vector>

namespace SmartLib
{
	using namespace  ::std;

	class Longest_Well_Performing_Interval
	{
	private:
		const vector<int>& _data; /***INCOMMING***/

		//unordered_map<long, long> _minIndex; //(sum, minIndex + 1)
		//unordered_map<long, long> _maxIndex; //(sum, maxIndex + 1)

		unordered_map<long, tuple<long, long>> _sum_min_max;

	private:
		Longest_Well_Performing_Interval(const vector<int>& data) :
			_data{data}
		{
		}

	private:
		long Traverse()
		{
			long sum = 0;
			//_minIndex[0] = 0;
			//_maxIndex[0] = 0;
			_sum_min_max.insert({ 0, {0, 0} });
			for (long ii = 0; ii < _data.size(); ++ ii)
			{
				sum += (_data[ii] > 8? 1 : -1);
				auto iter = _sum_min_max.find(sum);
				if (iter == _sum_min_max.end())
				{
					_sum_min_max.insert({ sum, {ii + 1, ii + 1} });
				}
				else
				{
					get<1>(iter->second) = ii + 1;
				}
				
			}

			return sum;
		}

	private:
		long Solution()
		{
			long total= Traverse();
			if (total > 0)
			{
				return (long)_data.size();
			}

			long len = 0;
			for (auto [sum, min_max] : _sum_min_max)
			{
				auto iter = _sum_min_max.find(sum+ 1);
				if (iter != _sum_min_max.end())
				{
					long temp = get<1>(iter->second) - get<0>(min_max);
					if (len < temp)
					{
						len = temp;
					}
				}
			}

			return len;
		}

	public:
		static int longestWPI(vector<int>& hours) 
		{
			Longest_Well_Performing_Interval alg{hours};
			return alg.Solution();
		}
	};
}