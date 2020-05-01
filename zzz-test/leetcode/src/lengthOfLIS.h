#pragma once

#include <map>
#include <vector>
#include <functional>
#include <tuple>

namespace SmartLib
{
	using namespace ::std;

	class lengthOfLISAlg
	{
		struct PosLen
		{
			long _pos{ 0 };
			long _len{ 0 };

			PosLen(long pos = 0L, long len = 0L) :
				_pos{ pos },
				_len{ len }
			{
			}
		};

	private:
		const vector<int>& _items; /***INCOMMING***/


		function<bool(long, long)> _compare = [](long aa, long bb)
		{
			return aa > bb;
		};
		map<long, PosLen, decltype(_compare)> _mm{_compare}; /***INTERNAL***/

		long _max{ 1L };

	private:
		lengthOfLISAlg(const vector<int>& items) :
			_items{ items }
		{
		}


		void RemoveUselessHead()
		{
			while (_mm.size() >= 2)
			{
				auto iter = _mm.begin();
				auto next = iter;
				++next;

				if (iter->second._len <= next->second._len)
				{
					_mm.erase(iter);
				}
				else
				{
					break;
				}
			}
		}

		void ProcessData(long index)
		{

			long data = _items[index];
			auto iter = _mm.upper_bound(data);
			if (iter == _mm.end())
			{
				_mm[data] = PosLen{index, 1};
			}
			else
			{

				PosLen pdmax{index, iter->second._len + 1};
				_mm[data] = pdmax;

				if (_max < pdmax._len)
				{
					_max = pdmax._len;
				}
			}

			RemoveUselessHead();
		}

		void Solution()
		{
			long size = _items.size();
			for (long ii = 0; ii < size; ++ ii)
			{
				ProcessData(ii);
			}
		}

		long GetResult()
		{
			return _max;
		}


	public:
		static int Solution(const vector<int>& items)
		{
			if (items.size() == 0)
			{
				return 0;
			}
			lengthOfLISAlg alg{items};
			alg.Solution();
			return alg.GetResult();
		}

	};
}