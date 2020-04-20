#pragma once

#include <stack>
#include <vector>

namespace SmartLib
{
	using namespace ::std;

	class MonoStackAlg
	{
	private:
		const vector<long> _data; /***INCOMMING***/
		stack<long> _pos; /***INTERNAL***/
		vector<long> _result; /***INTERNAL***/

	public:

		MonoStackAlg(const vector<long>& data) :
			_data{ data }
		{
			_result.resize(_data.size(), -1L);
		}

		void Process(long pos)
		{
			while (_pos.size() && _data[pos] < _data[_pos.top()])
			{
				long top = _pos.top();
				_pos.pop();

				_result[top] = pos;
			}

			_pos.push(pos);
		}


		vector<long> Solution()
		{
			for (long ii = 0; ii < _data.size(); ++ ii)
			{
				Process(ii);
			}


			return move(_result);
		}
	};
}