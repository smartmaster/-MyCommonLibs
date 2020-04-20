#pragma once


#include <vector>
#include <iostream>
#include "sml-common.h"

namespace SmartLib
{
	using namespace  ::std;

	class WalkAlg
	{
	private:
		long _totalPos;
		long _destination;

		long _startPos;
		long _steps;
	public:

		long Walk(long remainSteps, long curPos)
		{
			if (0 == remainSteps)
			{
				return curPos == _destination? 1 : 0;
			}


			if (curPos == 1)
			{
				return Walk(remainSteps - 1, 2);
			}

			if (curPos == _totalPos)
			{
				return Walk(remainSteps - 1, _totalPos - 1);
			}


			return Walk(remainSteps - 1, curPos - 1) + Walk(remainSteps - 1, curPos + 1);
		}


		long Walk()
		{
			return Walk(_steps, _startPos);
		}


		long WalkDP()
		{
			vector<vector<long>> matrix;
			matrix.resize(_steps + 1);
			for (auto& subvec : matrix)
			{
				subvec.resize(_totalPos + 1);
			}

			matrix[0][_destination] = 1;

			for (long reaminSteps = 1; reaminSteps <= _steps; ++reaminSteps)
			{
				for (long curPos = 1; curPos <= _totalPos; ++ curPos)
				{
					matrix[reaminSteps][curPos] =
						curPos == 1 ? matrix[reaminSteps - 1][2] :
						curPos == _totalPos ? matrix[reaminSteps - 1][_totalPos - 1] :
						matrix[reaminSteps - 1][curPos - 1] + matrix[reaminSteps - 1][curPos + 1];
				}
			}

			return matrix[_steps][_startPos];
		}


		WalkAlg(long totalPos, long destination, long startPos, long steps) :
			_totalPos{ totalPos },
			_destination{ destination },
			_startPos{ startPos },
			_steps{ steps }
		{
		}



	};



	class KnapPack
	{
	private:
		const vector<long>& _itemsSize;
		const vector<long>& _itemsValue;
		const long _packSize;

	public:
		KnapPack(const vector<long>& itemsSize, const vector<long>& itemsValue, long packSize) :
			_itemsSize{ itemsSize },
			_itemsValue{ itemsValue },
			_packSize{ packSize }
		{

		}

		long Max(long count, long packSize)
		{
			if (count == 0 || packSize == 0)
			{
				return 0;
			}


			long maxValue = 0;
			if (_itemsSize[count - 1] > packSize)
			{
				maxValue = Max(count - 1, packSize);
			}
			else
			{
				long maxValue1 = Max(count - 1, packSize);
				long maxValue2 = Max(count - 1, packSize - _itemsSize[count - 1]) + _itemsValue[count - 1];

				maxValue = maxValue1 > maxValue2? maxValue1: maxValue2;
			}

			return maxValue;
		}


		long Max()
		{
			return Max(_itemsSize.size(), _packSize);
		}


		long MyMax(long val1, long val2)
		{
			return val1 > val2 ? val1 : val2;
		}

		long MaxDP()
		{
			vector<vector<long>> dp;
			dp.resize(_itemsSize.size() + 1);
			for (auto& subvec : dp)
			{
				subvec.resize(_packSize + 1);
			}

			for (long count = 1; count <= _itemsSize.size(); ++ count)
			{
				for (long packSize = 1; packSize <= _packSize; ++ packSize)
				{
					if (_itemsSize[count - 1] > packSize)
					{
						dp[count][packSize] = dp[count - 1][packSize];
					}
					else
					{
						dp[count][packSize] = MyMax(dp[count - 1][packSize], dp[count - 1][packSize - _itemsSize[count - 1]] + _itemsValue[count - 1]);
					}
				}
			}


			return dp.back().back();
		}
	};


	class MaxIncSeq
	{
	private:
		const vector<long>& _data;

	public:
		MaxIncSeq(const vector<long>& data) :
			_data{data}
		{

		}
		void Solution()
		{
			vector<vector<long>> result(_data.size(), {-1L});

			for (long val : _data)
			{
				for (auto& rr : result)
				{
					if (val > rr.back())
					{
						rr.push_back(val);
						break;
					}
				}
			}

			long size = result[0].size();
			long index = 0;
			for (long ii = 1; ii < result.size(); ++ ii)
			{
				if (size < result[ii].size())
				{
					size = result[ii].size();
					index = ii;
				}
			}


			for (const auto& vec : result)
			{
				Common::PrintIterator(cout, vec.begin(), vec.end()) << endl;
			}

		}

		static void Case0()
		{
			vector<long> data{2, 1, 5, 3, 6, 4, 8, 9, 7};
			MaxIncSeq{ data }.Solution();
		}
	};
}


