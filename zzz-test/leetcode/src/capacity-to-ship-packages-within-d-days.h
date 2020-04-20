#pragma once

//https://leetcode-cn.com/problems/capacity-to-ship-packages-within-d-days/

#include <vector>

namespace SmartLib
{
	using namespace ::std;

	class ShipCapacity
	{
	private:
		long _days{ 0 };
		const vector<int>& _weights;

		long _maxWeight{ 0 };
		long _total{ 0 };


	private:
		void FindMaxAndTotal()
		{
			for (const auto ww : _weights)
			{
				if (_maxWeight < ww)
				{
					_maxWeight = ww;
				}

				_total += ww;
			}
		}

		bool IsOkay(long capacity)
		{
			long sum = 0;
			long days = 0;
			for (long ii = 0; ii < _weights.size(); ++ ii)
			{
				sum += _weights[ii];
				if (sum > capacity)
				{
					++days;
					--ii;
					sum = 0;
				}
			}
			++days;

			return days <= _days;
		}


		long Solution()
		{
			FindMaxAndTotal();

			long result = -1;
			long left = _maxWeight;
			long right = _total;

			while (left <= right)
			{
				long mid = (left + right) / 2;
				if (IsOkay(mid))
				{
					result = mid;
					right = mid - 1;
				}
				else
				{
					left = mid + 1;
				}
			}

			return result;
		}


		ShipCapacity(vector<int>& weights, int D) :
			_days{D},
			_weights{weights}
		{

		}

	public:
		static int shipWithinDays(vector<int>& weights, int D) {
			ShipCapacity sc{ weights , D};
			return sc.Solution();
		}

	};
}