#pragma once

#include <stack>
#include <vector>

namespace SmartLib
{
	using namespace ::std;

	class RecurReverseStack
	{
	private:
		vector<long>& _stack;

	public:

		RecurReverseStack(vector<long>& stack) :
			_stack{ stack }
		{
		}


		long RemoveBottom()
		{
			long top = _stack.back();
			_stack.pop_back();
			if (_stack.empty())
			{
				return top;
			}
			else
			{
				long bottom = RemoveBottom();
				_stack.push_back(top);
				return bottom;
			}
		}

		void Reverse()
		{
			if (_stack.empty())
			{
				return;
			}

			long bottom = RemoveBottom();
			Reverse();
			_stack.push_back(bottom);
		}
	};
}