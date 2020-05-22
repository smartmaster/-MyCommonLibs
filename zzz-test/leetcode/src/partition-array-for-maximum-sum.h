#pragma once

//https://leetcode.com/problems/partition-array-for-maximum-sum/

#include <vector>
#include <climits>

namespace SmartLib
{
	using namespace ::std;

	class partition_array_for_maximum_sum
	{
	private:
		const vector<int>& _data; /***INCOMMING***/
		long _len; /***INCOMMING***/

		vector<long> _dp;

	public:
		partition_array_for_maximum_sum(const vector<int>& data, long len) :
			_data{data},
			_len{ len }//,
		{
			_dp.resize(_data.size() + 1, -LONG_MAX);
			_dp[0] = 0;
		}

	public:
		long MaxPartSumDP()
		{
			for (long size = 1; size <= _data.size(); ++ size)
			{
				long count = 1;
				long maxData = _data[size - 1];
				for (;;)
				{
					long temp = _dp[size - count] + maxData * count;
					if (_dp[size] < temp)
					{
						_dp[size] = temp;
					}

					++count;
					if (count > _len || count > size)
					{
						break;;
					}

					maxData = maxData < _data[size - count] ? _data[size - count] : maxData;
				}
			}

			return _dp.back();
		}

		long MaxPartSum(long size)
		{
			if (size == 0)
			{
				return 0;
			}

			long dp = 0;
			if ((dp = _dp[size]) != -LONG_MAX)
			{
				return dp;
			}

			long count = 1;
			long maxData = _data[size - 1];

			long res = -LONG_MAX;
			for (;;)
			{
				long tempRes = MaxPartSum(size - count) + maxData * count;
				if (res < tempRes)
				{
					res = tempRes;
				}

				++count;
				if (count > _len || count > size)
				{
					break;
				}

				if (maxData < _data[size - count])
				{
					maxData = _data[size - count];
				}
			}

			_dp[size] = res;

			return res;
		}


	public:
		static int maxSumAfterPartitioning(vector<int>& A, int K) 
		{
			::SmartLib::partition_array_for_maximum_sum alg{A, K};
			//long res = alg.MaxPartSum(A.size());
			long res = alg.MaxPartSumDP();
			return res;
		}

	};
}