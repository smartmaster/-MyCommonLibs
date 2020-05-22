#pragma once

//https://leetcode.com/problems/numbers-with-same-consecutive-differences/

#include <vector>
#include <string>

namespace SmartLib
{
	using namespace ::std;

	class numbers_with_same_consecutive_differences
	{
	private:
		const long _diff;		/***INCOMMING***/
		const long _total;		/***INCOMMING***/

		long _count{ 0 };		/***INTERNAL***/
		long _sum{ 0 };
		vector<int> _result;	/***INTERNAL***/
		
	private:
		void DFS(long num)
		{
			if (num < 0 || num > 9)
			{
				return;
			}

			if (_count < _total)
			{
				++_count;
				_sum = _sum * 10 + num;

				if (_count == _total)
				{
					_result.push_back(_sum);
				}
				else
				{
					if (_diff == 0)
					{
						DFS(num);
					}
					else
					{
						DFS(num - _diff);
						DFS(num + _diff);
					}
				}

				--_count;
				_sum /= 10;
			}
		}

	private:
		vector<int> DFS_ALL()
		{
			for (long ii = 1; ii <= 9; ++ ii)
			{
				DFS(ii);
			}

			return move(_result);
		}

	private:
		numbers_with_same_consecutive_differences(const long diff, const long total) :
			_diff{ diff },
			_total{ total }//,
		{
		}

	public:
		static vector<int> numsSameConsecDiff(int N, int K) 
		{
			if (N == 1)
			{
				vector<int> singles;
				for (int ii = 0; ii <= 9; ++ ii)
				{
					singles.push_back(ii);
				}

				return singles;
			}

			//if (K == 0)
			//{
			//	vector<int> dup;
			//	for (int ii = 1; ii <= 9; ++ ii)
			//	{
			//		string str;
			//		str.assign(N, '0' + ii);
			//		dup.push_back(stoi(str));
			//	}

			//	return dup;
			//}

			numbers_with_same_consecutive_differences alg{K, N};
			return alg.DFS_ALL();
		}
	};
}