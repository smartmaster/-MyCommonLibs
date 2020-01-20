#pragma once

#include <vector>

#include "log.h"
#include "MultiDimArray.h"

namespace SmartLib
{
	class KnapsackAlg
	{
	private:
		int _totalWeight{ 0 }; /***INCOMMING***/
		std::vector<int> _itemWeight; /***INCOMMING***/
		std::vector<int> _itemValue; /***INCOMMING***/
		MDArray<int> _saved; /***INTERNAL***/

		int Knapsack(int nn, int ww)
		{
			int saved = _saved({ nn, ww });
			if (saved >= 0)
			{
				return saved;
			}

			int result = 0;
			if (nn == 0 || ww == 0)
			{
				result = 0;
			}
			else if (_itemWeight[nn - 1] > ww)
			{
				result = Knapsack(nn - 1, ww);
			}
			else
			{
				int r1 = Knapsack(nn - 1, ww - _itemWeight[nn - 1]) + _itemValue[nn - 1];
				int r2 = Knapsack(nn - 1, ww);
				result = r1 > r2 ? r1 : r2;
			}

			_saved({ nn, ww }) = result;
			return result;
		}


	public:
		KnapsackAlg(int totalWeight, const std::vector<int> & itemWeight, const std::vector<int> & itemValue) :
			_totalWeight(totalWeight),
			_itemWeight(itemWeight),
			_itemValue(itemValue),
			_saved({ (int)itemWeight.size() + 1, totalWeight + 1})
		{
			for (int ii = 0; ii < _saved.Rank(1); ++ii)
			{
				for (int jj = 0; jj < _saved.Rank(0); ++jj)
				{
					_saved({ ii, jj }) = -1;
				}
			}
		}

		int Knapsack()
		{
			return Knapsack(_itemWeight.size(), _totalWeight);
		}
	};

	class KnapsackAlgTest
	{
	public:
		static void Case0()
		{
			int totalWeight = 50;
			std::vector<int> weight = {10, 20, 30};
			std::vector<int> value = {60, 100, 120};

			KnapsackAlg alg(totalWeight, weight, value);
			int result = alg.Knapsack();

			LOG_LINE(L"result=%d", result);
		}
	};
}