#pragma once

#include <stack>

namespace SmartLib
{
	using namespace ::std;

	class StackQueue
	{
	private:
		stack<long> _push;
		stack<long> _pop;

	private:
		static void Transfer(stack<long>& s1, stack<long>& s2)
		{
			while (s1.size())
			{
				s2.push(s1.top());
				s1.pop();
			}
		}

	public:

		void Enqueue(long val)
		{
			Transfer(_pop, _push);
			_push.push(val);
		}

		long Dequeue()
		{
			Transfer(_push, _pop);
			long val = _pop.top();
			_pop.pop();
			return val;
		}

		long Size()
		{
			return _pop.size() + _push.size();
		}
	};
}