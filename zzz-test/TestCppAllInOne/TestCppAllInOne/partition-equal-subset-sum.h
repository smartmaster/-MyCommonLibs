#pragma once
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

//https://leetcode.com/problems/partition-equal-subset-sum/

namespace SmartLib_Knapsack_DFS
{
	using namespace  ::std;

	class Knapsack
	{
	private:
		int* _values{ nullptr }; /***INCOMMING***/
		int* _volumes{ nullptr };/***INCOMMING***/
		int _totalCount{ 0 }; /***INCOMMING***/
		int _totalVolume{ 0 }; /***INCOMMING***/


		int _reusedCount{ 0 };

		int _CurLeft{0};
		int _curMax{ 0 };
		int _max{ 0 };

	private:
		void Combine(int start)
		{
			for (int ii = start; ii < _totalCount; ++ ii)
			{
				if (_CurLeft >= _volumes[ii])
				{
					_CurLeft -= _volumes[ii];
					_curMax += _values[ii];
					Combine(ii + 1);
					_CurLeft += _volumes[ii];
					_curMax -= _values[ii];
				}
				else
				{
					if (_max < _curMax)
					{
						_max = _curMax;
					}
				}
			}
		}

	public:

		Knapsack(int* values, int* volumes, int totalCount, int totalVolume) :
			_values{ values },
			_volumes{ volumes },
			_totalCount{ totalCount },
			_totalVolume{ totalVolume },
			_CurLeft{ totalVolume }
		{
		}

		int MaxValue()
		{
			Combine(0);
			return _max;
		}
		
	};


	class Solution
	{
	public:
		static bool canPartition(vector<int>& nums)
		{
			int sum = 0;
			for (auto num : nums)
			{
				sum += num;
			}

			if (sum & 1)
			{
				return false;
			}

			nums.push_back(sum + 1); //end marker
			int* values = &nums[0];
			int* volumes = &nums[0];
			int totalCount = nums.size();
			sum >>= 1;

			Knapsack ks{ values, volumes, totalCount, sum };
			int maxValue = ks.MaxValue();
			//int maxValue = ks.MaxValueDP();

			return sum == maxValue;
		}
	};

}

namespace SmartLib_partition_equal_subset_sum
{
	using namespace ::std;

	union KnapsackUnion
	{
		long long _ll;
		struct 
		{
			int _count;
			int _volume;
		} _ss;

		KnapsackUnion(int count, int volume)
		{
			_ss._count = count;
			_ss._volume = volume;
		}
	};

	class KnapsackDict
	{
	private:
		//unordered_map<long long, int> _mm;
		//map<long long, int> _mm;
		vector<vector<int>> _maxVlaues;

	public:

		KnapsackDict(int totalCount, int totalVolume)
		{
			_maxVlaues.resize(totalCount + 1);
			for (auto& vec : _maxVlaues)
			{
				//vec.resize(totalVolume + 1, -1);
				vec.resize(totalVolume + 1);
			}

		}
		void Insert(int count, int volume, int result)
		{
			_maxVlaues[count][volume] = result + 1;
			//KnapsackUnion uu{ count, volume };
			//_mm.insert({ uu._ll, result});
		}

		bool Find(int count, int volume, int& result)
		{
			//KnapsackUnion uu{ count, volume };
			//auto iter = _mm.find(uu._ll);
			//if (iter != _mm.end())
			//{
			//	result = iter->second;
			//	return true;
			//}

			//return false;

			result = _maxVlaues[count][volume] - 1;
			return result >= 0;
		}
	};

	class Knapsack
	{
	private:
		int* _values{ nullptr }; /***INCOMMING***/
		int* _volumes{ nullptr };/***INCOMMING***/
		int _totalCount{ 0 }; /***INCOMMING***/
		int _totalVolume{ 0 }; /***INCOMMING***/


		KnapsackDict _dict; /***INTERNAL***/
		int _reusedCount{ 0 };


	private:
		int MaxValue(int totalCount, int totalVolume)
		{
			if (0 == totalCount || 0 == totalVolume)
			{
				return 0;
			}

			int saved = 0;
			if (_dict.Find(totalCount, totalVolume, saved))
			{
				++_reusedCount;
				return saved;
			}

			int result = 0;
			int volume = _volumes[totalCount - 1];
			if (volume > totalVolume)
			{
				result = MaxValue(totalCount - 1, totalVolume);
			}
			else
			{
				int rr1 = MaxValue(totalCount - 1, totalVolume);
				int rr2 = _values[totalCount - 1] + MaxValue(totalCount - 1, totalVolume - volume);
				result = rr1 > rr2 ? rr1 : rr2;
			}

			_dict.Insert(totalCount, totalVolume, result);

			return result;
		}

	public:

		Knapsack(int* values, int* volumes, int totalCount, int totalVolume) :
			_values{ values },
			_volumes{ volumes },
			_totalCount{ totalCount },
			_totalVolume{ totalVolume },
			_dict{ totalCount , totalVolume }
		{
		}

		int MaxValue()
		{
			return MaxValue(_totalCount, _totalVolume);
		}

		int MaxValueDP()
		{
			vector<vector<int>> maxValues;
			maxValues.resize(_totalCount + 1);
			for (auto& vv : maxValues)
			{
				vv.resize(_totalVolume + 1);
			}


			for (int count = 1; count <= _totalCount; ++ count)
			{
				int itemVolume = _volumes[count - 1];
				int itemValue = _values[count - 1];
				for (int volume = 1; volume <= _totalVolume; ++volume)
				{
					if (itemVolume > volume)
					{
						maxValues[count][volume] = maxValues[count - 1][volume];
					}
					else
					{
						int rr1 = maxValues[count - 1][volume];
						int rr2 = itemValue + maxValues[count - 1][volume - itemVolume];
						maxValues[count][volume] = rr1 > rr2 ? rr1 : rr2;
					}
				}
			}
			
			return maxValues[_totalCount][_totalVolume];
		}
	};

	class Solution 
	{
	public:
		static bool canPartition(vector<int>& nums)
		{
			int sum = 0;
			for (auto num : nums)
			{
				sum += num;
			}

			if (sum & 1)
			{
				return false;
			}

			
			int* values = &nums[0];
			int* volumes = &nums[0];
			int totalCount = nums.size();
			sum >>= 1;

			Knapsack ks{ values, volumes, totalCount, sum };
			int maxValue = ks.MaxValue();
			//int maxValue = ks.MaxValueDP();

			return sum == maxValue;
		}
	};
}

namespace SmartLib
{
	using namespace ::std;

	class Knapsack01
	{
	public:

		//ks[count][ww]
		//where
		//count - count of items having been processed, taken or not taken
		//ww - current total volume of a knapsack
		static int knapSack(int weigt[], int val[], int NN, int WW)
		{
			vector<vector<int>> ks(NN + 1);
			for (auto& sub : ks)
			{
				sub.resize(WW + 1);
			}


			for (int count = 1; count <= NN; ++count)
			{
				int curentw = weigt[count - 1];
				int curentval = val[count - 1];
				for (int ww = 1; ww <= WW; ++ ww)
				{
					if (curentw > ww)
					{
						ks[count][ww] = ks[count - 1][ww]; //too large to take
					}
					else
					{
						int val1 = ks[count - 1][ww];						 //do not take it
						int val2 = curentval + ks[count - 1][ww - curentw];	 //take it	
						ks[count][ww] = val1 > val2 ? val1 : val2;
					}
				}
				
			}

			return ks[NN][WW];
		}

		static int Case0()
		{
			int weigt[] = { 10, 20, 30 };
			int val[] = { 60, 100, 120 };
			int NN = _countof(val);
			int  WW = 50;
			
			printf("%d", knapSack(weigt, val, NN, WW));
			return 0;
		}
	};

	class partition_equal_subset_sum2
	{
	private:
		const vector<int>& _nums;
		vector<vector<int>> _knapsack;

	public:
		partition_equal_subset_sum2(const vector<int>& nums) : 
			_nums(nums)
		{
		}

		//knapsack[cc][ss] 
		//where
		//cc - count of items having been processed, taken or not taken
		//ss - current total volume of knapsack
		int Calc(int count, int sum)
		{

			for (int cc = 1; cc <= count; ++ cc)
			{
				int num = _nums[cc - 1];
				for (int ss = 1; ss <= sum; ++ ss)
				{
					if (num > ss)
					{
						_knapsack[cc][ss] = _knapsack[cc - 1][ss]; //too large to take
					}
					else
					{
						int val1 = _knapsack[cc - 1][ss]; //do not take it
						int val2 = num + _knapsack[cc - 1][ss - num]; //take it
						_knapsack[cc][ss] = val1 > val2 ? val1 : val2;
					}
				}
			}
			return _knapsack[count][sum];
		}

		bool canPartition()
		{
			int sum = 0;
			for (const auto num : _nums)
			{
				sum += num;
			}

			if (sum & 1)
			{
				return false;
			}

			sum >>= 1;
			int count = _nums.size();
			_knapsack.resize(count + 1);
			for (auto& subarr : _knapsack)
			{
				subarr.resize(sum + 1);
			}

			int sum1 = Calc(count, sum);
			return sum == sum1;
		}

		static bool canPartition(vector<int>& nums)
		{
			partition_equal_subset_sum2 pess{nums};
			return pess.canPartition();
		}
	};

	class partition_equal_subset_sum
	{
		union MapKey
		{
			long long _ll;
			struct 
			{
				short _start;
				short _length;
				int _sum;
			} _parts;

			static void Assign(MapKey& key, long long val)
			{
				key._ll = val;
			}

			static void Assign(MapKey& key, short start, short length, int sum)
			{
				key._parts._start = start;
				key._parts._length = length;
				key._parts._sum = sum;
			}
		};

		unordered_map<long long, bool> _mm;
		const vector<int>& _nums;

		static vector<int>& EmptyVector()
		{
			static vector<int> _empty{};
			return _empty;
		}
		

	public:

		partition_equal_subset_sum():
			_nums{ EmptyVector() }
		{
		}

		partition_equal_subset_sum(const vector<int>& nums) :
			_nums{nums}
		{
			for (int ii = 0; ii < _nums.size(); ++ ii)
			{
				MapKey key;
				MapKey::Assign(key, ii, 1, _nums[ii]);
				_mm.insert({ key._ll, true });
			}
		}


		bool canPartition(int start, int length, int target)
		{
			MapKey key;
			MapKey::Assign(key, start, length, target);
			auto iter = _mm.find(key._ll);
			if (iter != _mm.end())
			{
				return iter->second;
			}

			bool result = false;
			if (length == 1)
			{
				result = (_nums[start] == target);
			}
			else
			{
				result = canPartition(start, length - 1, target); 
				if (!result)
				{
					int last = start + length - 1;
					if (target >= _nums[last])
					{
						result = canPartition(last, 1, target) || canPartition(start, length - 1, target - _nums[last]);
					}
				}
			}

			_mm.insert({ key._ll, result });
			return result;
		}

		
		
		static bool canPartition(vector<int>& nums)
		{
			int sum = 0;
			for (const auto num : nums)
			{
				sum += num;
			}

			if (sum & 1)
			{
				return false;
			}

			sum >>= 1;
			
			partition_equal_subset_sum pess{nums};
			return pess.canPartition(0, nums.size(), sum);

		}
	};
};