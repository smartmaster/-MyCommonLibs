#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace SmartLib
{
	using namespace ::std;

	union frog_jumps_union
	{
		long long key;
		struct 
		{
			int pos;
			int len;
		} parts;

		frog_jumps_union(int pp, int ll)
		{
			parts.pos = pp;
			parts.len = ll;
		}
	};

	class frog_jumps_map
	{
	private:
		unordered_map<long long, bool> _mm;
	public:
		void insert(int pos, int len, bool result)
		{
			frog_jumps_union uu{pos, len};
			_mm.insert({uu.key, result});
		}

		bool find(int pos, int len, bool& result)
		{
			frog_jumps_union uu{pos, len};
			auto iter = _mm.find(uu.key);
			if (iter != _mm.end())
			{
				result = iter->second;
				return true;
			}
			return false;
		}
	};

	class frog_jumps
	{
	private:
		const vector<int>& _stones; /***INCOMMING***/
		unordered_set<int> _setStones; /***INTERNAL***/
		frog_jumps_map _saved; /***INTERNAL***/
	public:

		frog_jumps(const vector<int>& stones) :
			_stones{stones}
		{
			for (const auto stone : stones)
			{
				_setStones.insert(stone);
			}
		}


		bool CanJump(long long pos, long long len)
		{
			if (pos < 0 || len < 0)
			{
				return false;
			}

			if (pos == 1 && len == 1)
			{
				return true;
			}

			if (pos == 1)
			{
				return false;
			}

			if (pos == 0 && len == 0)
			{
				return true;
			}

			if (pos == 0 || len == 0)
			{
				return false;
			}

			auto iter = _setStones.find(pos);
			if (iter == _setStones.end())
			{
				return false;
			}

			bool saved = false;
			if (_saved.find(pos, len, saved))
			{
				return saved;
			}

			bool result = false;
			for (int ii = -1; ii <= 1; ++ ii)
			{
				result = CanJump(pos - len, len + ii);
				if (result)
				{
					break;
				}
			}

			_saved.insert(pos, len, result);

			return result;
		}

		bool canCross()
		{
			vector<int> lens;
			int pos = _stones.back();
			for (int ii = _stones.size() - 2; ii >= 0; --ii)
			{
				lens.push_back(pos - _stones[ii]);
			}

			bool result = false;
			for (const auto len : lens)
			{
				result = CanJump(pos, len);
				if (result)
				{
					break;
				}
			}

			return result;
		}
	};

	
	class Solution_frog_jumps
	{
	public:
		static bool canCross(vector<int>& stones) 
		{
			frog_jumps fj{stones};
			return fj.canCross();
			
		}
	};
}