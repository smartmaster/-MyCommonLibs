#pragma once

#include <map>
#include <vector>
#include <climits>
#include <tuple>

namespace SmartLib
{
	using namespace  ::std;

	class SparseMatrix
	{
	public:
		inline static const long SML_NAN = -LONG_MAX;


	private:
		vector<map<long, long>> _matrix;
		//unordered_map<long, unordered_map<long, long >> _matrix;


	public:
		SparseMatrix(long nodeCount = 0)
		{
			if (nodeCount > 0)
			{
				_matrix.resize(nodeCount);
			}

		}

	public:
		void Add(long ii, long jj, long len)
		{
			if (ii >= _matrix.size())
			{
				_matrix.resize(ii + 1);
			}

			_matrix[ii][jj] = len;

		}

	public:
		tuple<bool, long> Get(long ii, long jj)
		{
			if (ii >= _matrix.size())
			{
				return make_tuple(false, SML_NAN);
			}

			auto iter = _matrix[ii].find(jj);
			if (iter == _matrix[ii].end())
			{
				return make_tuple(false, SML_NAN);
			}
			

			return make_tuple(true, iter->second);

#if 0
			auto iter1 = _matrix.find(ii);
			if (iter1 == _matrix.end())
			{
				return make_tuple(false, SML_NAN);;
			}

			auto iter2 = iter1->second.find(jj);
			if (iter2 == iter1->second.end())
			{
				return make_tuple(false, SML_NAN);
			}


			return make_tuple(true, iter2->second);
#endif

		}
	};

	class StoneGameII
	{
	private:
		int* _piles{ nullptr };
		int _size{ 0 };

		vector<long> _sum;
		SparseMatrix _matrix;

	public:
		StoneGameII(int* piles, int size) :
			_piles{ piles },
			_size{ size },
			_matrix{size}
		{
			_sum.resize(size + 1);

			_sum[0] = 0;
			for (long ii = 0; ii < _size; ++ii)
			{
				_sum[ii + 1] = _sum[ii] + _piles[ii];
			}
		}

	public:

		long Diff(long startPos, long takenCount)
		{
			if (startPos >= _size)
			{
				return 0;
			}

			auto [found, number] = _matrix.Get(startPos, takenCount);
			if (found)
			{
				return number;
			}

			long rr = 0;
			if (startPos + takenCount + takenCount >= _size)
			{
				rr = _sum.back() - _sum[startPos];
				_matrix.Add(startPos, takenCount, rr);
				return rr;
			}

			
			long max = -_sum.back() - 1;
			for (long tc = 1; tc <= (takenCount + takenCount); ++tc)
			{
				long tempSum = _sum[startPos + tc] - _sum[startPos];
				long tempDiff = Diff(startPos + tc, tc > takenCount? tc : takenCount);
				if (max < tempSum - tempDiff)
				{
					max = tempSum - tempDiff;
				}
			}


			_matrix.Add(startPos, takenCount, max);

			return max;
		}


	public:
		long Solution()
		{
			long diff = Diff(0, 1);
			return (_sum.back() + diff) / 2;
		}


	public:
		static  int stoneGameII(vector<int>& piles)
		{
			if (piles.size() == 0)
			{
				return 0;
			}

			StoneGameII alg{ &piles[0], (int)piles.size() };
			long rr = alg.Solution();
			return rr;
		}
	};
}


#if false

class Solution {
public:
	int stoneGameII(vector<int>& piles) {
		return SmartLib::StoneGameII::stoneGameII(piles);
	}
};

#endif