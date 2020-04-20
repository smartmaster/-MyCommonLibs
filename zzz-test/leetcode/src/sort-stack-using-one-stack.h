#pragma once

#include <stack>

namespace SmartLib
{
	using namespace ::std;

	class SortStackUsingStackAlg
	{
	private:
		stack<long>& _result;
		stack<long> _helper;


	private:
		long SortHelper(long data)
		{
			long count = 0;
			while (_helper.size() && _helper.top() > data)
			{
				_result.push(_helper.top());
				_helper.pop();
				++count;
			}

			_helper.push(data);

			for (long ii = 0; ii < count; ++ ii)
			{
				_helper.push(_result.top());
				_result.pop();
			}

			return count;
		}

	
	public:
		SortStackUsingStackAlg(stack<long>& ss) :
			_result{ ss }
		{
		}

		void Sort()
		{
			while (_result.size())
			{
				long data = _result.top();
				_result.pop();
				SortHelper(data);
			}

			while (_helper.size())
			{
				long top = _helper.top();
				_helper.pop();
				_result.push(top);
			}
		}



	};

}