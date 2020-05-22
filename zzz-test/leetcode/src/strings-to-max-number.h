#pragma once


//given a sequence of numbers, cat them all to make a maximum number
//for example, {2, 3, 1} => 321

#include <vector>
#include <string>
#include <algorithm>

namespace SmartLib
{
	using namespace  ::std;

	class numbers_to_max_number
	{
	private:
		vector<string> _nums;

	private:
		numbers_to_max_number(const vector<long>& nums)
		{
			for (auto num : nums)
			{
				_nums.push_back(to_string(num));
			}
		}


	private:
		string PosToString(const vector<long>& index)
		{
			string res;
			for (auto pp : index)
			{
				res += _nums[pp];
			}
			return res;
		}

	private:
		vector<long> Sort(long size)
		{
			if (size == 0)
			{
				return {};
			}

			if (size == 1)
			{
				return {0};
			}


			vector<long> curOrder = Sort(size - 1); //sort the first (size - 1) numbers
			curOrder.push_back(size - 1);

			string max = PosToString(curOrder);
			vector<long> maxOrder = curOrder;

			for (long index = size - 2; index >= 0; -- index)
			{
				::std::swap(curOrder[index], curOrder[index + 1]);
				string tempMax = PosToString(curOrder);
				if (max < tempMax)
				{
					max = tempMax;
					maxOrder = curOrder;
				}
			}

			return maxOrder;
		}

	private:
		string Solution()
		{
			vector<long> index = Sort(_nums.size());
			string maxStr = PosToString(index);
			return maxStr;
		}



	private:
		string SolutionQ()
		{
			vector<long> order;
			for (long ii = 0; ii < _nums.size(); ++ ii)
			{
				order.push_back(ii);
			}

			auto compare = [this](long pos1, long pos2)
			{
				return (_nums[pos1] + _nums[pos2]) > (_nums[pos2] + _nums[pos1]);
			};
			::std::sort(begin(order), end(order), compare);

			return PosToString(order);
		}



	public:
		static string NumbersToMax(const vector<long>& nums)
		{
			numbers_to_max_number alg{ nums };
			return alg.Solution();
		}

		static string NumbersToMaxQ(const vector<long>& nums)
		{
			numbers_to_max_number alg{ nums };
			return alg.SolutionQ();
		}
	};
}