#pragma once

#include <vector>
#include "log.h"

using std::vector;

namespace SmartLib
{
	class FindCow
	{
	private:
		int _max;
		int _farmer;
		int _cow;

		vector<int> _min;
		vector<int> _current;
		vector<bool> _flags;

	private:
		FindCow(int max, int farmer, int cow) :
			_max(max),
			_farmer(farmer),
			_cow(cow)
		{
			_flags.resize(max + 1);

			if (farmer == cow)
			{
				_min.push_back(farmer);
			}
			else if (farmer < cow)
			{
				_min.push_back(farmer);
				while (farmer * 2 <= cow)
				{
					_min.push_back(farmer * 2);
					farmer *= 2;
				}
				
				int back = _min.back();
				for (int ii = back + 1; ii <= cow; ++ ii)
				{
					_min.push_back(ii);
				}
			}
			else
			{
				for (int ii = farmer; ii >= cow; -- ii)
				{
					_min.push_back(ii);
				}
			}
		}

		void DFS(int farmer)
		{
			if (_current.size() >= _min.size())
			{
				return;
			}

			if (_flags[farmer])
			{
				return;
			}

			_current.push_back(farmer);
			_flags[farmer] = true;

			if (farmer == _cow)
			{
				if (_min.size() > _current.size())
				{
					_min = _current;
				}
			}
			else
			{
				if (farmer * 2 <= _max)
				{
					DFS(farmer * 2);
				}

				if (farmer + 1 <= _max)
				{
					DFS(farmer + 1);
				}

				if (farmer - 1 >= 1)
				{
					DFS(farmer - 1);
				}
			}

			_current.pop_back();
			_flags[farmer] = false;
		}

		void Print()
		{
			LOG_LINE_A("path length = %d", (int)_min.size());
			for (auto val : _min)
			{
				LOG_A("%d ", val);
			}
			LOG_LINE_A("");
		}


		void BFS(int farmer)
		{

		}

	public:
		static void Run(int max, int farmer, int cow)
		{
			FindCow fw(max, farmer, cow);
			fw.DFS(farmer);
			fw.Print();
		}
	};


	class FindCowTest
	{
	public:
		static void Case0()
		{
			FindCow::Run(100, 7, 75);
		}
	};
}
