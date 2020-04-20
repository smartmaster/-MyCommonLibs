#pragma once

//#include <long>
#include <vector>
#include <deque>
#include <iostream>

#include "sml-common.h"

namespace SmartLib
{
	using namespace ::std;

	class MaxValueInWindowAlg
	{
	private:
		vector<long>& _data; /***INCOMMING***/
		const long _windowSize; /***INCOMMING***/

		deque<long> _pos; /***INTERNAL***/

	private:
		void EnqueueMax(long pos)
		{
			while (_pos.size() && _data[pos] > _data[_pos.back()])
			{
				_pos.pop_back();
			}

			_pos.push_back(pos);
		}


	
		long Process(long currentPos)
		{
	
			if (_pos.size() && (currentPos - _pos.front() >= _windowSize))
			{
				_pos.pop_front();
			}

			EnqueueMax(currentPos);
			return _data[_pos.front()];
			
		}

	public:
		MaxValueInWindowAlg(vector<long>& data, long winSize) :
			_data{data},
			_windowSize{winSize}
		{
		}

		vector<long> Solution()
		{
			for (long ii = 0; ii < _windowSize - 1; ++ ii)
			{
				EnqueueMax(ii);
			}

			vector<long> result;
			for (long ii = _windowSize - 1; ii < _data.size(); ++ ii)
			{
				result.push_back(Process(ii));
			}


			return result;
		}


	public:
		static void Case0()
		{
			vector<long> data{4, 3, 5, 4, 3, 3, 6, 7 };

			auto result = MaxValueInWindowAlg{ data, 3 }.Solution();

			Common::PrintIterator(cout, result.begin(), result.end()) << endl;
		}
	};
}