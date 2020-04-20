#pragma once


#include <vector>
#include <stack>

namespace SmartLib
{
	using namespace ::std;

	class StackOrderAlg
	{
	public:
		static bool StackOrder(const vector<long>& push_order, const vector<long>& pop_order)
		{
			if (push_order.empty() && pop_order.empty())
			{
				return true;
			}

			if (push_order.size() != pop_order.size())
			{
				return false;
			}

			stack<long> stack;
			long pos = 0;
			for (auto val : push_order)
			{
				stack.push(val);

				while (stack.size() && stack.top() == pop_order[pos])
				{
					stack.pop();
					++pos;
				}
			}

			return stack.empty() && pos == pop_order.size();


		}
	};
}