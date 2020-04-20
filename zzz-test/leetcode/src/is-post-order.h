#pragma once


#include <vector>

namespace SmartLib
{

	using namespace ::std;

	class IsPostOrderAlg
	{
	private:
		const vector<long>& _data;

	public:

		IsPostOrderAlg(const vector<long>& data) :
			_data{ data }
		{
		}

		long FindSmaller(long val, long right, long left)
		{
			while (right >= left)
			{
				if (_data[right] < val)
				{
					break;
				}

				--right;
			}

			return right;
		}

		bool VerifyOrder(long val, long left, long right)
		{
			bool result = true;
			for (long ii = left; ii <= right; ++ ii)
			{
				if (_data[ii] > val)
				{
					result = false;
					break;
				}
			}

			return result;
		}

		bool IsPostOrder(long first, long last)
		{
			if (first >= last)
			{
				return true;
			}


			long rootVal = _data[last];
			long leftRoot = FindSmaller(rootVal, last, first);
			long rightRoot = last - 1;



			return VerifyOrder(rootVal, first, leftRoot) && IsPostOrder(first, leftRoot) && IsPostOrder(leftRoot + 1, rightRoot);
		}

		static bool Solution(const vector<long>& data)
		{
			return IsPostOrderAlg{ data }.IsPostOrder(0, data.size() - 1);
		}
	};
}